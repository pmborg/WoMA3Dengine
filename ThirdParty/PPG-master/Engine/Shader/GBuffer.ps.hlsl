// --------------------------------------------------------------------------------------------
// Filename: GBuffer.ps.hlsl
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG
// WomaIntegrityCheck = 1234525256;

#define PS_USE_SPECULAR

sampler LinearSampler : register(s0);
sampler PointSampler : register(s1);

Texture2D Albedo : register(t0);
//Texture2D NormalMap : register(t1);
//Texture2D OcclusionRoughnessMetal : register(t2);
//Texture2D AoMap : register(t3);
//Texture2D Emissive : register(t4);


cbuffer LigthInfo : register(b0)
{
    float4 lightDirection;
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
#if defined PS_USE_SPECULAR
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
#endif
};

//////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc1(float3 Normal)
//////////////////////////////////////////////////////////////////////////////////
{
    return saturate(dot(Normal, lightDirection.xyz)); // Calculate the amount of light on this pixel
}

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 ambientColor = { 0.550000012, 0.550000012, 0.550000012, 0 };
    float4 textureColor = Albedo.Sample(LinearSampler, input.texCoord);
    float lightIntensity = PSlightFunc1(input.normal);
    
    textureColor = textureColor * saturate(ambientColor + lightIntensity);
    
#if defined PS_USE_SPECULAR //34: If enabled on material, calculate the Specular LIGHT
    {
        if (lightIntensity > 0.0f)
        {
            float4 color = ambientColor;
			
            //color += (diffuseColor * lightIntensity);
            color += (lightIntensity);
		
            color = saturate(color);
            float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection.xyz);
            float fPhoneValue = saturate(dot(Reflection, input.viewDirection)); // (R.V)
            float4 specular = pow(fPhoneValue, 52/*nShininess*/); // Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

            color = color * textureColor;
            textureColor = saturate(textureColor + specular); // specular = Ls (contribution of the light source) * Ks (specular component of the material)
        }
    }
#endif
    
    return textureColor;
}
