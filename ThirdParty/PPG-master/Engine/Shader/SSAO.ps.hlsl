#include "Common/Sampler.hlsli"

struct PixelShaderInput // see Fullscreen.vs.hlsl
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

cbuffer CBuffer : register(b3)
{
    float4x4 viewProjection;
    float4x4 inverseViewProjection;
    float4 kernel[64];
    float4 cameraPos;
    float radius;
    float2 windowSize;
}

Texture2D gDepthTex : register(t0);
Texture2D gNormalTex : register(t1);
Texture2D gNoise : register(t2);

float3 CalculateWorldFromDepth(float depth, float2 texCoord)
{
    // clip space between [-1, 1]
    // flip y so that +ve y is upwards
    float2 clipXY = texCoord * 2.0 - 1.0;
    clipXY.y = -clipXY.y;

    // NOTE: depth is not linearized
    // Also in range [0, 1] due to DirectX Convention
    float4 clipSpace = float4(clipXY, depth, 1);
    float4 worldSpace = mul(inverseViewProjection, clipSpace);
    worldSpace /= worldSpace.w;
    return worldSpace.xyz;
}

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float depth = gDepthTex.Sample(PointSampler, IN.texCoord).r;
    if (depth >= 1)
        return float4(1.0, 1.0, 1.0, 1.0);

    float3 posW = CalculateWorldFromDepth(depth, IN.texCoord);
    float3 normal = gNormalTex.Sample(PointSampler, IN.texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    float centerDepth = distance(posW, cameraPos.xyz);
    float2 noiseScale = windowSize / 4.0;
    float3 randDir = gNoise.Sample(PointSampler, IN.texCoord * noiseScale).rgb;

    float3 tangent = normalize(randDir - normal * dot(randDir, normal));
    float3 bitangent = cross(normal, tangent);
    float3x3 tbn = transpose(float3x3(tangent, bitangent, normal));

    float occlusion = 0.0;
    for (int i = 0; i < 64; ++i)
    {
        // find out a desired world position to sample
        float3 kernelPosW = mul(tbn, kernel[i].rgb);
        float3 samplePosW = posW + kernelPosW * radius;
        float sampleDepth = distance(samplePosW, cameraPos.xyz);

        // project it to the clip space so we know where can sample from the depth buffer
        float4 samplePosClip = mul(viewProjection, float4(samplePosW, 1));
        samplePosClip /= samplePosClip.w;

        // invert y and put to [0 - 1]
        float2 sampleUV = float2(samplePosClip.x, -samplePosClip.y) * 0.5f + 0.5f;

        // reject samples outside of the range
        if (sampleUV.x < 0 || sampleUV.x > 1 || sampleUV.y < 0 || sampleUV.y > 1)
        {
            occlusion += 0.0;
            continue;
        }

        // sample our scene for actual depth
        float depthFromTex = gDepthTex.Sample(PointSampler, sampleUV.xy).r;
        float3 scenePos = CalculateWorldFromDepth(depthFromTex, sampleUV.xy);
        float sceneDepth = distance(scenePos, cameraPos.xyz);

        float depthDiff = abs(sceneDepth - centerDepth);
        float rangeCheck = smoothstep(0.0, 1.0, radius / depthDiff);
        occlusion += step(sceneDepth, sampleDepth) * rangeCheck;
    }
    occlusion /= 64.0;
    float factor = 1 - occlusion;
    return float4(factor.rrr, 1);
}
