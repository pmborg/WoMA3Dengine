#ifndef SHADING_COMMON
#define SHADING_COMMON

#include "Sampler.hlsli"
#include "Lighting.hlsli"
#include "Phong.hlsli"

struct SurfaceInfo
{
    float4 posW;
    float3 N;
    float3 T;
    float3 B;
    float3 V;
    float NdotV;
};

struct LightingInfo
{
    // general terms
    float3 L;

    float3 H;
    float NdotH;
    float NdotL;

    //currently for directional light source
    float shadowFactor;

    // for point sources
    float distance;
    float attenuation;
};

inline float3 CalcNormalFromNormMap(Texture2D normalMap, float2 uv, SurfaceInfo surf)
{
    float3 normalMapVal = normalMap.Sample(LinearSampler, uv).xyz;
    normalMapVal = normalize((normalMapVal * 2) - 1.0);
    float3 N = (normalMapVal.x * surf.T) +
               (normalMapVal.y * surf.B) +
               (normalMapVal.z * surf.N);
    return normalize(N);
}

inline float3 CalcNormalFromBumpMap(Texture2D bumpMap, float2 uv, SurfaceInfo surf)
{
    float3 N = surf.N;
    float2 bumpTextureSize;
    bumpMap.GetDimensions(bumpTextureSize[0], bumpTextureSize[1]);
    float2 pixelSize = 1.0 / bumpTextureSize;

    float mid = bumpMap.Sample(PointSampler, uv) * 2.0 - 1.0;
    float left = bumpMap.Sample(PointSampler, uv + float2(-pixelSize.x, 0)) * 2.0 - 1.0;
    float right = bumpMap.Sample(PointSampler, uv + float2(pixelSize.x, 0)) * 2.0 - 1.0;
    float top = bumpMap.Sample(PointSampler, uv + float2(0, -pixelSize.y)) * 2.0 - 1.0;
    float bottom = bumpMap.Sample(PointSampler, uv + float2(0, pixelSize.y)) * 2.0 - 1.0;
    float3 p1 = ((bottom - mid) - (top - mid)) * normalize(surf.B);
    float3 p2 = ((left - mid) - (right - mid)) * normalize(surf.T);
    return normalize(N - (p1 + p1));
}

inline void CalcCommonLightInfo(SurfaceInfo surf, inout LightingInfo li)
{
    li.H = normalize(li.L + surf.V);
    li.NdotH = dot(surf.N, li.H);
    li.NdotL = dot(surf.N, li.L);
}

inline void EvalDirectionalLight(SurfaceInfo surf, Light light, inout LightingInfo li)
{
    li.L = normalize(-light.direction);
    li.distance = -1; // infinity
    li.attenuation = 1;

    li.shadowFactor = (light.status == LIGHT_ENABLED_W_SHADOWMAP) ? 1 - ShadowFactor(surf.posW) : 1;
    CalcCommonLightInfo(surf, li);

}

inline void EvalPointLight(SurfaceInfo surf, Light light, inout LightingInfo li)
{
    li.L = light.position.xyz - surf.posW.xyz;
    li.distance = length(li.L);
    li.L = normalize(li.L);
    li.attenuation = 1.0 / (light.constantAtt + light.linearAtt * li.distance + light.quadAtt * (li.distance * li.distance));

    li.shadowFactor = 1;
    CalcCommonLightInfo(surf, li);
}

inline void EvalSpotLight(SurfaceInfo surf, Light light, inout LightingInfo li)
{
    li.L = light.position.xyz - surf.posW.xyz;
    li.distance = length(li.L);
    li.L = normalize(li.L);
    li.attenuation = 1.0 / (light.constantAtt + light.linearAtt * li.distance + light.quadAtt * (li.distance * li.distance));
    float minCos = cos(light.spotAngle);
    float maxCos = (minCos + 1.0f) / 2.0f; // squash between [0, 1]
    float cosAngle = dot(light.direction.xyz, -li.L);
    float intensity = smoothstep(minCos, maxCos, cosAngle);
    li.attenuation = intensity * li.attenuation;

    li.shadowFactor = 1;
    CalcCommonLightInfo(surf, li);
}


LightingInfo EvalLightingInfo(SurfaceInfo surf, Light light)
{
    LightingInfo li;
    switch (abs(light.lightType))
    {
        case DIRECTIONAL_LIGHT:
            EvalDirectionalLight(surf, light, li);
            break;
        case POINT_LIGHT:
            EvalPointLight(surf, light, li);
            break;
        case SPOT_LIGHT:
            EvalSpotLight(surf, light, li);
            break;
        default:
            break;
    }
    return li;
}

static const float GAMMA = 2.2;
static const float INV_GAMMA = 1.0 / GAMMA;

// linear to sRGB approximation
// see http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
float3 LINEARtoSRGB(float3 color)
{
    return pow(color, INV_GAMMA);
}

// sRGB to linear approximation
// see http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
float4 SRGBtoLINEAR(float4 srgbIn)
{
    return float4(pow(srgbIn.xyz, GAMMA), srgbIn.w);
}

#endif
