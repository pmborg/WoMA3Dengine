#include "Common/Sampler.hlsli"
#include "Common/BRDF.hlsli"

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

cbuffer SpecularCBuffer : register(b0)
{
    float roughness;
};

TextureCube SkyboxTexture : register(t0);

static const uint SAMPLE_COUNT = 1024u;

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float3 N = normalize(IN.texCoord);
    float3 R = N;
    float3 V = R;

    float totalWeight = 0.0;
    float3 prefilteredColor = float3(0.0, 0.0, 0.0);
    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        float2 st = Hammersley(i, SAMPLE_COUNT);
        float3 H = ImportanceSampleGGX(st, N, roughness);
        float3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = saturate(dot(N, L));
        if (NdotL > 0.0)
        {
            prefilteredColor += SkyboxTexture.Sample(LinearSampler, L).rgb * NdotL;
            totalWeight += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / totalWeight;

    return float4(prefilteredColor, 1);
}
