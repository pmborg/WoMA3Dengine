#include "Common/Sampler.hlsli"

Texture2D SkyboxTexture : register(t0);

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), -asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(IN.texCoord));
    return float4(SkyboxTexture.Sample(LinearSampler, uv).rgb, 1.0);
}
