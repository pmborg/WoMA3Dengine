// --------------------------------------------------------------------------------------------
// Filename: GBuffer.ps.hlsl
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG
// Version inspired: https://github.com/michal-z/SimpleDirectPBR/blob/master/Source/Shaders/SimpleForward.hlsl
// Free Assets: https://freepbr.com/

// WomaIntegrityCheck = 1234525217;

#define PS_USE_SPECULAR

sampler LinearSampler   : register(s0);
sampler PointSampler    : register(s1);

Texture2D Albedo        : register(t0);
Texture2D NormalMap     : register(t1);
Texture2D OcclusionRoughnessMetal : register(t2); // PBR: Type::GLTF
Texture2D AoMap         : register(t3);
Texture2D Emissive      : register(t4);
Texture2D Metallic      : register(t5);
Texture2D Roughness     : register(t6);
Texture2D AlphaMap      : register(t7);
Texture2D BumpMap       : register(t8);

//ALIGN WITH: PPG-master\Engine\Source\Scene\Material\PBRMaterial.h
cbuffer PBRMaterial : register(b0)
{
     // 0: Albedo/BaseColor
    float4  gAlbedo; 
    int     gUseAlbedoMap;
    
    // 1: Normal
    int     gNormalState; //gNormalState=1 means DO Normal | 2: means DO Bump | 3: means DO both
    
    // 3: Ambient Occlusion
    int     gUseAoMap;
    
    // 4: Emissive
    int     gUseEmissiveMap;
    
    // -------------------------------------
    float4  ambientColor;
    float4  lightColor;
    float4  lightDirection;
    float3  gLightPos;
    // 5: Opacity/Alpha
    int     gUseAlphaMap;
    
    // -------------------------------------
    //6: Metallic
    float   gMetallic;
    int     gUseMetallicMap;
    float   gRoughness;
    int     gUseRoughnessMap;
    
    // -------------------------------------
    // 8: Emissive color
    int     gUseEmissiveColor;
    
    int     gUseOccMetalRough;  //Type::GLTF
    int     gConvertToLinear;   //Type::GLTF
    int     gUseGLTF;           //Type::GLTF
    
    float   fade;
}

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
#if defined PS_USE_SPECULAR
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
    float4 wPosition : POSITION1;
#endif
    float3x3 TBN : TBN_MATRIX;
};

static const float PI = 3.14159265359f;

float DistributionGGX(float3 N, float3 H, float a)
{
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 FresnelSchlick(float HoV, float3 F0)
{
    return F0 + (float3(1.0f, 1.0f, 1.0f) - F0) * pow(1.0f - HoV, 5.0f);
}

//////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc1(float3 Normal)
//////////////////////////////////////////////////////////////////////////////////
{
    return saturate(dot(Normal, lightDirection.xyz)); // Calculate the amount of light on this pixel
}

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 textureColor = (gUseAlbedoMap) ? Albedo.Sample(LinearSampler, input.texCoord) : gAlbedo;

    float3 albedo = textureColor.rgb;
    
    // PBR: roughness
    float roughness;
    if (gUseRoughnessMap)
        roughness = Roughness.Sample(LinearSampler, input.texCoord).r;
    else
        roughness = gRoughness;

    // PBR: metallic
    float metallic;
    if (gUseMetallicMap)
        metallic = Metallic.Sample(LinearSampler, input.texCoord).r;
    else
        metallic = gMetallic;

    // PBR: ao
    float ao = 1;
    if (gUseAoMap)
        ao = AoMap.Sample(LinearSampler, input.texCoord).r;

    //Type::GLTF
    if (gUseOccMetalRough)
    {
        float3 orm = OcclusionRoughnessMetal.Sample(LinearSampler, input.texCoord).rgb;
        ao = orm.r;
        roughness = orm.g;
        metallic = orm.b;
    }

    //Some textures have noisy AO
    ao = saturate(ao);
        
    float Alpha = roughness * roughness;
    float K = Alpha + 1.0f;
    K = (K * K) / 8.0f;
        
    //float3 F0 = lerp(float3(0.04, 0.04f, 0.04f), textureColor.xyz, float3(metallic, metallic, metallic));
    float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);
        
    float3 V = normalize(input.viewDirection);
    float3 N = input.normal;
        
    // FOR each light:
    // ...
    float3 L = normalize(-lightDirection.xyz); // directional
    float3 H = normalize(V + L);
        
    float Distance = length(L);
    float Attenuation = 1.0f / (Distance * Distance);
    float3 radiance = lightColor * Attenuation;
        
    float3 tangentNormal = float3(0, 0, 1);
    if (gNormalState == 1 || gNormalState == 3)
    {
        tangentNormal = NormalMap.Sample(LinearSampler, input.texCoord).xyz * 2.0f - 1.0f;
    }
    if (gNormalState == 2 || gNormalState == 3)
    {
        float bumpHeight = BumpMap.Sample(LinearSampler, input.texCoord).r;
        float bumpStrength = 0.2f; // Adjustable
        tangentNormal.z += bumpHeight * bumpStrength;
    }
    if (gNormalState > 0)
        N = normalize(mul(tangentNormal, input.TBN));

    // Add emissive
    float3 emissiveColor = float3(0, 0, 0);
    if (gUseEmissiveMap)
    {
        emissiveColor = Emissive.Sample(LinearSampler, input.texCoord).rgb;
    }
    if (gUseEmissiveColor)
    {
        emissiveColor *= gAlbedo.rgb; // or another tint value
    }
        
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);

    float NDF = DistributionGGX(N, H, Alpha);
    float G = GeometrySmith(N, V, L, K);

    float3 numerator = NDF * G * F;
    float denominator = 4.0f * saturate(max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f));
    float3 specular = numerator / max(denominator, 0.001f);
        
    // calculate color based on rendering equation.
    // KD + KS = 1 according to energy conservation.
    float3 KS = F;
    float3 KD = float3(1.0f, 1.0f, 1.0f) - KS;
    KD *= 1.0f - metallic;
        
    //// Calculate Cook - Torrance BRDF for the specular light part.
    float NDOTL = max(dot(N, L), 0.0f); //NoL
         
    float3 Lo = (KD * albedo / PI + specular) * radiance * NDOTL;
    // ...
    //FOR LIGHT END!
        
    float3 ambient = ambientColor.rgb * albedo * ao;
    float3 Color = saturate(ambient + Lo + emissiveColor);
        
#if true
    float Gamma = 1.0f / 2.2f;
    Color = Color / (Color + float3(1.0f, 1.0f, 1.0f));
    Color = pow(Color, float3(Gamma, Gamma, Gamma));
#endif

    float rim = pow(1.0 - max(dot(N, V), 0.0), 2.0);
    Color += albedo * rim * 0.1;
        
    //textureColor.rgb = float4(Color*fade, 1.0f);
    textureColor.rgb = Color * fade;

	// Output alpha (if used)
    if (gUseAlphaMap)
    {
        float alpha = AlphaMap.Sample(LinearSampler, input.texCoord).r;
        textureColor.a = alpha;
    }
    
    return textureColor;
}
