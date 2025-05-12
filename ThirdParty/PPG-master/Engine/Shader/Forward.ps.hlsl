#include "Common/Lighting.hlsli"
#include "Common/BRDF.hlsli"
#include "Common/Shading.hlsli"

#define VERTEX_NORMALS 0
#define NORMAL_MAP 1
#define BUMP_MAP 2

#ifndef PI
#define PI 3.14159265359
#endif

Texture2D Albedo : register(t0);
Texture2D NormalMap : register(t1);

cbuffer PBRMaterial : register(b0)
{
    float4 gAlbedo;
    float gMetallic;
    float gRoughness;

    int gUseAlbedoMap;
    int gNormalState;
}

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 pos : POSITION0;
    float4 wPosition : POSITION1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
    SurfaceInfo surf;
    surf.posW = IN.wPosition;
    surf.T = normalize(IN.tangent);
    surf.B = normalize(IN.binormal);
    surf.N = normalize(IN.normal);
    if (gNormalState == NORMAL_MAP)
    {
        surf.N = CalcNormalFromNormMap(NormalMap, IN.texCoord, surf);
    }
    else if (gNormalState == BUMP_MAP)
    {
        surf.N = CalcNormalFromBumpMap(NormalMap, IN.texCoord, surf);
    }
    surf.V = normalize(eyePosition.xyz - IN.wPosition.xyz);
    surf.NdotV = dot(surf.N, surf.V);

    float3 Lo = float3(0, 0, 0);
    float3 albedo = gAlbedo.rgb;
    if (gUseAlbedoMap)
        albedo = Albedo.Sample(LinearSampler, IN.texCoord);
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, gMetallic);
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        if (light.status == LIGHT_DISABLED)
            continue;
        LightingInfo li = EvalLightingInfo(surf, light);

        // cook-torrance brdf
        float NDF = DistributionGGX(max(0.0, li.NdotH), gRoughness);
        float G = GeometrySmith(max(0.0, surf.NdotV), max(0.0, li.NdotL), gRoughness);
        float3 F = fresnelSchlick(max(dot(li.H, surf.V), 0.0), F0);
        float3 kS = F;
        float3 kD = float3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - gMetallic;

        float NdotL = max(li.NdotL, 0.0); // clamped n dot l

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(surf.NdotV, 0.0) * NdotL;
        float3 specular = numerator / max(denominator, 0.001);

        Lo += (kD * albedo / PI + specular) * light.color.rgb * li.attenuation * NdotL * (li.shadowFactor);

    }

    float3 ambient = globalAmbient.rgb * albedo;
    float3 colour = ambient + Lo;

    return float4(colour, 1.0);
}
