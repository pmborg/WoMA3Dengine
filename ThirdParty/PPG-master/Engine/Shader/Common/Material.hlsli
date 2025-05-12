#ifndef MATERIAL_COMMON
#define MATERIAL_COMMON

#include "Sampler.hlsli"

#define VERTEX_NORMALS 0
#define NORMAL_MAP 1
#define BUMP_MAP 2

Texture2D Diffuse : register(t0);
Texture2D NormalMap : register(t1);
Texture2D Specular : register(t2);

cbuffer Material : register(b0)
{
    float4 matDiffuse;
    float4 matSpecular;

    float matShininess;
    int useDiffuse;
    int normalState;
    int useSpecular;
}

float3 CalculateNormal(float3 normal, float3 tangent, float3 binormal, float2 texCoord)
{
    float3 N = normal;
    switch (normalState)
    {
        case NORMAL_MAP:
            float3 normalMapVal = NormalMap.Sample(LinearSampler, texCoord).xyz;
            normalMapVal = normalize((normalMapVal * 2) - 1.0);
            N = (normalMapVal.x * normalize(tangent)) + (normalMapVal.y * normalize(binormal)) + (normalMapVal.z * normal);
            N = normalize(N);
            return N;
            break;
        case BUMP_MAP:
            float2 bumpTextureSize;
            NormalMap.GetDimensions(bumpTextureSize[0], bumpTextureSize[1]);
            float2 pixelSize = 1.0 / bumpTextureSize;

            float mid = NormalMap.Sample(PointSampler, texCoord) * 2.0 - 1.0;
            float left = NormalMap.Sample(PointSampler, texCoord + float2(-pixelSize.x, 0)) * 2.0 - 1.0;
            float right = NormalMap.Sample(PointSampler, texCoord + float2(pixelSize.x, 0)) * 2.0 - 1.0;
            float top = NormalMap.Sample(PointSampler, texCoord + float2(0, -pixelSize.y)) * 2.0 - 1.0;
            float bottom = NormalMap.Sample(PointSampler, texCoord + float2(0, pixelSize.y)) * 2.0 - 1.0;
            float3 p1 = ((bottom - mid) - (top - mid)) * normalize(binormal);
            float3 p2 = ((left - mid) - (right - mid)) * normalize(tangent);
            N = normalize(N - (p1 + p1));
            return N;
            break;
    }
    return normal;
}
#endif

