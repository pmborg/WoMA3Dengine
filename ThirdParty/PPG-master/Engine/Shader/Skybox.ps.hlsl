#include "Common/Sampler.hlsli"

TextureCube SkyboxTexture : register(t0);

struct PixelShaderInput
{
    float4 position: SV_POSITION;
    float3 texCoord: TEXCOORD;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
    return float4(SkyboxTexture.SampleLevel(LinearSampler, IN.texCoord, 0.0).rgb, 1);
}
