// --------------------------------------------------------------------------------------------
// Filename: 034Light.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2026 [LATEST]
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//
// Level 33 upgrade:
// - BLOCK4 activated
// - BLOCK5 activated
// - BLOCK6 activated
// - BLOCK7 introduced (cameraPosition, castShadow, specularColor, nShininess)
// - Restores conditional VS/PS lighting logic
// --------------------------------------------------------------------------------------------

//WomaIntegrityCheck = 1234526920;

#if DXAPI11 != 1 && DXAPI12 != 1
    #define DXAPI9 1
#endif

#if DXAPI9 == 1
    #define PS_POSITION POSITION
    #define PS_TARGET   COLOR0
#else
    #define PS_POSITION SV_POSITION
    #define PS_TARGET   SV_TARGET
#endif
#if DXAPI9 == 1
    #define PS_NORMAL TEXCOORD1
#else
    #define PS_NORMAL NORMAL
#endif

#define PS_USE_SPECULAR

// mode:
// -1 = hardware sampler
//  0 = nearest
//  1 = bilinear
//  2 = trilinear
//  3 = cubic

#define TEXTURE_MODE -1 //Default is -1

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
	float3 position : POSITION;	//21
	float2 texCoords: TEXCOORD; //22
	float3 normal	: NORMAL;	//23
};

// PIXEL:
struct PSIn						
{
	float4 position				: SV_POSITION;			// 21
	float2 texCoords			: TEXCOORD;				// 22
	float3 normal				: NORMAL;				// 23 LIGHT
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
};

/////////////
// GLOBALS //
/////////////

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"
#include "light.hlsli"											 

//Set on: DX_modelClass::RenderSubMesh
#if DXAPI9 == 1 || DXAPI11 == 1
Texture2D shaderTexture;	// 22: Texture
Texture2D AlfaMapTexture;	// 33: AlfaMap
#endif
#if DXAPI12 == 1
Texture2D AlfaMapTexture:	register(t0); // 33: AlfaMap	//DX12: Descriptor: 2
Texture2D shaderTexture:	register(t1); // 22: Texture	//DX12: Descriptor: 3
#endif

#if DXAPI9 == 1 || DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
	PSIn output;
	float4 cameraPosition;

    if (VS_USE_WVP)
    {
        output.position = mul(float4(input.position, 1), WVP); // Calculate the position of the vertex against the world, view, and projection matrices
    }
    else
    {
        float4 position = float4(input.position, 1);
        position = mul(position, worldMatrix);
        position = mul(position, view);         //viewMatrix
        position = mul(position, projection);   //projectionMatrix
        output.position = position;
    }

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
    output.texCoords = input.texCoords;
	
	cameraPosition = mul(float4(input.position, 1), WV);

	//23: LIGHT: NORMAL
    if (VShasLight || VShasSpecular) 
        output.normal = normalize(mul(input.normal, (float3x3) worldMatrix)); // Calculate the normal vector against the world matrix only

	//34: SPECULAR
#if defined PS_USE_SPECULAR
	output.cameraPosition = cameraPosition;

	if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);			// P
        output.viewDirection = normalize(VSCameraPosition.xyz - worldPosition.xyz); // L = Lp - p (L = lightDirection)
    }
#endif

    return output;
}

#if DXAPI9 == 0
#include "TextureSampling.hlsli"
// mode:
// 0 = nearest
// 1 = bilinear
// 2 = trilinear
// 3 = cubic
// 4 = hardware sampler
#endif

//Mode	Filter Type	    FPS
//0	    Nearest	        13111 FPS
//1	    Bilinear	    12945 FPS
//2	    Trilinear	    12989 FPS
//3	    Cubic 	        13141 FPS

float4 GetShaderTexture(Texture2D tex, float2 texCoords, uint mipLevel, int mode)
{
#if DXAPI9 == 0
    switch (mode)
    {
        case 0:
            return NearestInterpolation(SampleType, tex, texCoords);
        case 1:
            return BilinearInterpolation(SampleType, tex, texCoords);
        case 2:
            return TrilinearInterpolation(SampleType, tex, texCoords, mipLevel);
        case 3:
            return CubicInterpolation(SampleType, tex, texCoords);
        
        default:
            return tex.Sample(SampleType, texCoords);
    }
#endif
    
    // Legacy fallback (DX9 / ps_4_0_level_9_3)
    return tex.Sample(SampleType, texCoords);
}
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : PS_TARGET
{
    float4 textureColor = pixelColor; // 21: SET PIXEL COLOR
    float lightIntensity = 0;

	 if (hasTexture)
		textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0, TEXTURE_MODE);
		
	// 23: LIGHT
    if (hasLight)
    {
        if (lightType == 1)//(!isSky)
            lightIntensity = PSlightFunc1(input.normal);
        else
            lightIntensity = PSlightFunc2(input.normal);

        if (hasTexture)
        {
            textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);
        }
        else
        {
            textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
        }
    }

#if defined PS_USE_ALFA_TEXTURE // 33: Alfa Map: (Optional AlfaMap for blending textutres)
	if (hasAlfaMap)
		textureColor.a = AlfaMapTexture.Sample(SampleType, input.texCoords).r;
#endif

#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
	if (hasAlfaColor)
		textureColor.a = alfaColor;
#endif

#if defined PS_USE_SPECULAR //34: If enabled on material, calculate the Specular LIGHT
	if (hasSpecular)	
	{
		if (lightIntensity > 0.0f)
		{
			//float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
            float3 Reflection = reflect(lightDirection, input.normal);
			
			//float  fPhoneValue = saturate(dot(Reflection, input.viewDirection));	// (R.V)
            float fPhoneValue = max(dot(Reflection, input.viewDirection), 0.0f);	// (R.V)
			
			float4 specular = pow(fPhoneValue, nShininess);		// Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)
			textureColor = saturate(textureColor + specular);	// specular = Ls (contribution of the light source) * Ks (specular component of the material)
		}
    }
#endif
    
#if DXAPI12 != 1
    textureColor.rgb = pow(textureColor.rgb, 1.0 / 2.2); //Apply sRGB workflow: NEW 2026 review!
#endif
    
    return textureColor;
}
