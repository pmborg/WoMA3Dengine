// --------------------------------------------------------------------------------------------
// Filename: 035TextureBump.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	code by : Pedro Borges - pmborg@yahoo.com
**********************************************************************************************/

#if (!defined DXAPI11 && !defined DXAPI12)
    #define DXAPI11 1
#endif

#define PS_USE_LIGHT
#define PS_USE_ALFA_TEXTURE
#define PS_USE_ALFACOLOR
#define PS_USE_SPECULAR
#define PS_USE_BUMP

#define TEXTURE_MODE -1 //Default is -1

//////////////
// TYPEDEFS //
//////////////

struct VSIn
{
	float3 position : POSITION;
	float2 texCoords: TEXCOORD;
	float3 normal	: NORMAL;
    float3 tangent : TANGENT;
};

struct PSIn
{
	float4 position				: SV_POSITION;
	float2 texCoords			: TEXCOORD;
	float3 normal				: NORMAL;
	float3 viewDirection		: TEXCOORD1;
	float4 cameraPosition		: WS;
	float3 tangent				: TANGENT;
};

/////////////
// GLOBALS //
/////////////

Texture2D shaderTexture : register(t0);     // albedo
Texture2D AlfaMapTexture : register(t1);    // alpha map (optional)
Texture2D shadowTexture : register(t2);     // shadow map (optional)
Texture2D normalMapTexture : register(t3);  // NORMAL MAP


#if DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"
#include "light.hlsli"
#include "TextureSampling.hlsli"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
	PSIn output;
	float4 cameraPosition;

	if (VS_USE_WVP) {
		output.position = mul(float4(input.position, 1), WVP);
	} else {
		float4 position = float4(input.position, 1);
		position = mul(position, worldMatrix);
		position = mul(position, view);
		position = mul(position, projection);
		output.position = position;
	}

	output.texCoords = input.texCoords;

	cameraPosition = mul(float4(input.position, 1), WV);

	if (VShasLight || VShasSpecular)
		output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));

#if defined PS_USE_SPECULAR
	output.cameraPosition = cameraPosition;
	if (VShasSpecular)
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);
		output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);
	}
#endif

#if defined PS_USE_BUMP
    if (VShasNormMap)
    {
        // Transform tangent to world space (worldMatrix only)
        output.tangent = normalize(mul(input.tangent, (float3x3)worldMatrix));
    }
#endif

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Helpers - reuse your sampling function for consistency
////////////////////////////////////////////////////////////////////////////////
float4 GetShaderTexture(Texture2D tex, float2 texCoords, uint mipLevel, int mode)
{
    switch (mode)
    {
        case 0: return NearestInterpolation(SampleType, tex, texCoords);
        case 1: return BilinearInterpolation(SampleType, tex, texCoords);
        case 2: return TrilinearInterpolation(SampleType, tex, texCoords, mipLevel);
        case 3: return CubicInterpolation(SampleType, tex, texCoords);
        default: return tex.Sample(SampleType, texCoords);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
	float4 textureColor = pixelColor;
	float lightIntensity = 0;
    
	// Sample base albedo
	if (hasTexture)
	{
		textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0, TEXTURE_MODE);
	}

	// === BUMP / NORMAL MAPPING ===
	// start with the interpolated (world-space) normal from VS
	float3 normal = normalize(input.normal);

#if defined PS_USE_BUMP
	if (hasNormMap)
	{
		// Sample the normal map using your texture sampling helper (consistent filtering)
		float3 nmap = GetShaderTexture(normalMapTexture, input.texCoords, 0, TEXTURE_MODE).xyz * 2.0f - 1.0f;
        // Fix for OpenGL-style normal maps in a DirectX engine:
        nmap.g = -nmap.g;
        
		// Optional: normal strength parameter for tuning/debugging
		float normalStrength = 1.0f; // <---- tune this or expose in cbuffer
		nmap = saturate(nmap * normalStrength);

		// Ensure tangent is orthogonal to the (transformed) normal
		float3 T = normalize(input.tangent - dot(input.tangent, normal) * normal);

		// Compute bitangent consistently (handedness depends on exporter - this is standard)
		float3 B = normalize(cross(normal, T));

		// Build TBN (columns = T, B, N)
		float3x3 TBN = float3x3(T, B, normal);

		// Transform normal from tangent space to world space
		normal = normalize(mul(nmap, TBN));
	}
#endif
    
    //return float4(normalize(normal) * 0.5f + 0.5f, 1);
    
	// === LIGHTING ===
	if (isSky)
    {
        lightIntensity = PSlightFunc2(normal);
    }
    else
    {
        lightIntensity = PSlightFunc1(normal);
    }

    //return float4(lightIntensity, lightIntensity, lightIntensity, 1);
    
    if (hasTexture) {
        textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);
    } else {
        textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
    }

#if defined PS_USE_ALFA_TEXTURE
	if (hasAlfaMap)
		textureColor.a = GetShaderTexture(AlfaMapTexture, input.texCoords, 0, TEXTURE_MODE).r;
#endif

#if defined PS_USE_ALFACOLOR
	if (hasAlfaColor)
		textureColor.a = alfaColor;
#endif

#if defined PS_USE_SPECULAR
	if (hasSpecular)
	{
		if (lightIntensity > 0.0f)
		{
			float4 color = ambientColor;
			color += (diffuseColor * lightIntensity);
			color = saturate(color);

			// Use bumped normal here for specular (was using unbumped input.normal previously)
            // Keep your reflect convention: reflect(lightDirection, normal) if that matched earlier levels,
            // otherwise consider reflect(-lightDirection, normal) depending on how you define lightDirection.
			float3 Reflection = normalize(reflect(lightDirection, normal));
			float fPhoneValue = max(dot(Reflection, input.viewDirection), 0.0f);
			float4 specular = pow(fPhoneValue, nShininess);

			// Optionally multiply by material specular color (diffuseColor or separate specular color)
			// specular *= specularColor;

			color = color * textureColor;
			textureColor = saturate(textureColor + specular);
		}
	}
#endif
    
    textureColor.rgb = pow(textureColor.rgb, 1.0 / 2.2); //Apply sRGB workflow
	return textureColor;
}
