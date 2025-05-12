#include "Common/Sampler.hlsli"

TextureCube CubeMap : register(t0);

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

static const float PI = 3.14159265359;
static const float SAMPLE_DELTA = 0.05;


float4 main(PixelShaderInput IN) : SV_TARGET
{
    float3 irradiance = float3(0.0, 0.0, 0.0);
    float3 normal = normalize(IN.texCoord);

    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = cross(up, normal);
    up = cross(normal, right);

    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += SAMPLE_DELTA)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += SAMPLE_DELTA)
        {
            // spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            irradiance += CubeMap.Sample(LinearSampler, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    return float4(irradiance, 1);
}

