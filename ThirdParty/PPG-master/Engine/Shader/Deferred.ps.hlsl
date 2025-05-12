#include "Common/Sampler.hlsli"
#include "Common/Lighting.hlsli"
#include "Common/Shading.hlsli"
#include "Common/BRDF.hlsli"

struct PixelShaderInput // see Fullscreen.vs.hlsl
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

cbuffer DeferredCBuffer : register(b3)
{
    float4x4 inverseProjection;
    float4x4 inverseView;
    int useAO;
    int useEnvMap;
}

Texture2D DepthTexture : register(t0);
Texture2D Diffuse : register(t1);
Texture2D MetalRough : register(t2);
Texture2D Normals : register(t3);
Texture2D AO : register(t5);

TextureCube EnvMap : register(t6);
TextureCube PrefilteredSpecMap : register(t7);
Texture2D BrdfLUT : register(t8);
Texture2D Emissive : register(t9);

float3 CalculateWorldFromDepth(float depth, float2 texCoord)
{
    // clip space between [-1, 1]
    // flip y so that +ve y is upwards
    float2 clipXY = texCoord * 2.0 - 1.0;
    clipXY.y = -clipXY.y;

    // NOTE: depth is not linearized
    // Also in range [0, 1] due to DirectX Convention
    float4 clipSpace = float4(clipXY, depth, 1);
    float4 viewSpace = mul(inverseProjection, clipSpace);

    // perspective divide
    viewSpace /= viewSpace.w;

    float4 worldSpace = mul(inverseView, viewSpace);
    return worldSpace.xyz;
}

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float depth = DepthTexture.Sample(PointSampler, IN.texCoord).r;
    float3 posW = CalculateWorldFromDepth(depth, IN.texCoord);
    float4 matMetalRough = MetalRough.Sample(PointSampler, IN.texCoord);
    float3 albedo = Diffuse.Sample(PointSampler, IN.texCoord).rgb;
    float metallic = matMetalRough.r;
    float roughness = matMetalRough.g;
    float occlusion = matMetalRough.b;
    float3 normal = Normals.Sample(PointSampler, IN.texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    float3 emissive = Emissive.Sample(PointSampler, IN.texCoord).rgb;

    SurfaceInfo surf;
    surf.posW = float4(posW, 1);
    surf.N = normal;
    surf.V = normalize(eyePosition.xyz - posW);
    surf.NdotV = dot(surf.N, surf.V);

    float3 Lo = float3(0, 0, 0);
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metallic);

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        if (light.status == LIGHT_DISABLED)
            continue;
        LightingInfo li = EvalLightingInfo(surf, light);

        // cook-torrance brdf
        float NDF = DistributionGGX(saturate(li.NdotH), roughness);
        float G = GeometrySmith(saturate(surf.NdotV), saturate(li.NdotL), roughness);
        float3 F = fresnelSchlick(saturate(dot(li.H, surf.V)), F0);
        float3 kS = F;
        float3 kD = float3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = saturate(li.NdotL); // clamped n dot l

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * saturate(surf.NdotV) * NdotL;
        float3 specular = numerator / max(denominator, 0.001);

        Lo += (kD * albedo / PI + specular) * light.color.rgb * li.attenuation * NdotL * (li.shadowFactor);

    }
    float3 ambient = globalAmbient.rgb * albedo;
    if (useEnvMap)
    {
        float3 kS = fresnelSchlickRoughness(saturate(surf.NdotV), F0, roughness);
        float3 kD = 1.0 - kS;
        kD *= 1.0 - metallic;
        float3 irradiance = EnvMap.Sample(LinearSampler, surf.N).rgb;
        float3 diffuse = irradiance * albedo;

        float3 R = normalize(reflect(-surf.V, surf.N));
        float3 prefilterdColour = PrefilteredSpecMap.SampleLevel(LinearSampler, R, roughness * 5.0).rgb;
        float2 envBrdf = BrdfLUT.Sample(PointSampler, float2(saturate(surf.NdotV), roughness)).rg;
        float3 specular = prefilterdColour * (kS * envBrdf.x + envBrdf.y);
        ambient = (kD * diffuse + specular);
    }

    float ao = useAO ? AO.Sample(PointSampler, IN.texCoord).r : 1.0;
    ambient *= ao * occlusion;
    float3 colour = ambient + Lo + emissive;
    
    //return float4(1,1,1, 1.0);
    return float4(colour, 1.0);
}
