#include "Common/Sampler.hlsli"

Texture2D Albedo : register(t0);
Texture2D NormalMap : register(t1);
Texture2D OcclusionRoughnessMetal : register(t2);
Texture2D AoMap : register(t3);
Texture2D Emissive : register(t4);

cbuffer PBRMaterial : register(b0)
{
    float4 gAlbedo;
    float gMetallic;
    float gRoughness;

    int gUseAlbedoMap;
    int gUseOccMetalRough;
    int gUseAoMap;
    int gUseEmmisive;
    int gNormalState;
    int gConvertToLinear;
}

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    //float4 pos : POSITION0;
    //float4 wPosition : POSITION1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

struct GBufferOutput
{
    float4 diffuse : SV_TARGET0;
};

//float3 lightDirection = { 0.228761971, -0.915644038, 0.199814045 };
//
//////////////////////////////////////////////////////////////////////////////////
//float4 PSlightFunc1(float3 Normal)
//////////////////////////////////////////////////////////////////////////////////
//{
//    return saturate(dot(Normal, lightDirection)); // Calculate the amount of light on this pixel
//}

GBufferOutput main(PixelShaderInput IN)
{
    GBufferOutput OUT;
    //float lightIntensity = 0;
    //
    //lightIntensity = PSlightFunc1(IN.normal);
    //float4 ambientColor = { 0.550000012, 0.550000012, 0.550000012, 0};
    //OUT.diffuse = Albedo.Sample(PointSampler, IN.texCoord) * saturate(ambientColor + lightIntensity);
    
    //OUT.diffuse = Albedo.Sample(PointSampler, IN.texCoord);
    OUT.diffuse = Albedo.Sample(LinearSampler, IN.texCoord);
    
    //OUT.diffuse = float4(1,1,1,1);
    return OUT;
}
