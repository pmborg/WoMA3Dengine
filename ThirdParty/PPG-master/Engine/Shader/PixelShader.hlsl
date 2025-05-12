#include "Common/Sampler.hlsli"
#include "Common/Lighting.hlsli"
#include "Common/Shading.hlsli"

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

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 pos      : POSITION0;
    float4 wPosition: POSITION1;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float4 diffuse = float4(0, 0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);

    SurfaceInfo surf;
    surf.posW = IN.wPosition;
    surf.N = normalize(IN.normal);
    surf.T = normalize(IN.tangent);
    surf.B = normalize(IN.binormal);
    surf.V = normalize(eyePosition.xyz - surf.posW.xyz);

    if (normalState == NORMAL_MAP)
    {
        surf.N = CalcNormalFromNormMap(NormalMap, IN.texCoord, surf);
    }
    else if (normalState == BUMP_MAP)
    {
        surf.N = CalcNormalFromBumpMap(NormalMap, IN.texCoord, surf);
    }

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        if (light.status == LIGHT_DISABLED)
            continue;
        LightingInfo li = EvalLightingInfo(surf, light);
        diffuse.rgb += PhongDiffuse(max(0.0, li.NdotL), light.color.rgb) * li.attenuation * li.shadowFactor;
        specular.rgb += PhongSpecular(li.L, surf.N, surf.V, light.color.rgb, matShininess) * li.attenuation * li.shadowFactor;
    }

    float4 finalDiffuse = matDiffuse;
    if (useDiffuse)
        finalDiffuse *= Diffuse.Sample(LinearSampler, IN.texCoord);
    float4 finalSpecular = matSpecular;
    if (useSpecular)
        finalSpecular *= Specular.Sample(LinearSampler, IN.texCoord).rrrr;
    float4 finalAmbient = globalAmbient * finalDiffuse;

    float4 final = finalAmbient + finalDiffuse * saturate(diffuse) + finalSpecular * saturate(specular);
    return float4(final.rgb, 1);
}
