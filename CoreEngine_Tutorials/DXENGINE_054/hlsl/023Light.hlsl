// --------------------------------------------------------------------------------------------
// Filename: 023Light.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#if (!defined DXAPI11 && !defined DXAPI12)
    #define DXAPI11 1
#endif

#define PS_USE_LIGHT		 //23
//#define PS_USE_ALFA_TEXTURE	 //33
#define PS_USE_ALFACOLOR 	 //33
//#define PS_USE_SPECULAR		 //34

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
#if defined PS_USE_SPECULAR
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
#endif
#if defined PS_USE_FOG
    float fogFactor				: FOG;		// 51 FOG
#endif	
};

/////////////
// GLOBALS //
/////////////

//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture;	// 22: Texture
Texture2D AlfaMapTexture;	// 33: AlfaMap
#endif
#if DXAPI12 == 1
Texture2D AlfaMapTexture:	register(t0); // 33: AlfaMap	//DX12: Descriptor: 2
Texture2D shaderTexture:	register(t1); // 22: Texture	//DX12: Descriptor: 3
#endif

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

#if defined PS_USE_FOG
	//51:
    if (VShasFog) 
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
#endif
	//23: LIGHT: NORMAL
    if (VShasLight || VShasSpecular) 
        output.normal = normalize(mul(input.normal, (float3x3) worldMatrix)); // Calculate the normal vector against the world matrix only

	//34: SPECULAR
#if defined PS_USE_SPECULAR
	
	output.cameraPosition = cameraPosition;

	if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);			// P
		output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);	// L = Lp - p (L = lightDirection)
	}
#endif

    return output;
}

#include "TextureSampling.hlsli"
// mode:
// 0 = nearest
// 1 = bilinear
// 2 = trilinear
// 3 = cubic
// 4 = hardware sampler

//Mode	Filter Type	    FPS
//0	    Nearest	        13111 FPS
//1	    Bilinear	    12945 FPS
//2	    Trilinear	    12989 FPS
//3	    Cubic 	        13141 FPS

float4 GetShaderTexture(Texture2D tex, float2 texCoords, uint mipLevel, int mode)
{
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

    // fallback
    return tex.Sample(SampleType, texCoords);
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 textureColor = pixelColor; // 21: SET PIXEL COLOR
    float lightIntensity = 0;
#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif
	//-----------------------------------------------------------------------------------
	// lvl >=21: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
    //replace:
        //textureColor = shaderTexture.Sample(SampleType, input.texCoords);
    //with:
    #define TEXTURE_MODE 0
	 if (hasTexture)
		textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0, TEXTURE_MODE);
		
	// 23: LIGHT
    //if (hasLight)
    {
        if (!isSky)
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
	
#if defined PS_USE_FOG
    // 31: FOG: Calculate the final color using the fog effect equation.
    if (hasFog)
        textureColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;
#endif

#if defined PS_USE_SHADOWMAP_TEXTURE//36
    //36: SHADOWS [NEW PART!]
    if (castShadow)
        textureColor.rgb = textureColor.rgb * HasShadows(input.lightViewPosition);
#endif

#if defined PS_USE_FADE
    // FADE: Used by Sun or All (at night)
    if (fade < 1)
        textureColor.rgb = textureColor.rgb * fade;
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

#if defined PS_USE_FOG
    if (hasFog)
    {
		//textureColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor; // FOG: Calculate the final color using the fog effect equation.
        float4 fog4 = 0;
        if (isSky)
        {
            fog4.r = 0.9;
        }
        else
        {
            fog4.r = (1.0 - input.fogFactor);
        }
        fog4.g = fog4.r;
        fog4.b = fog4.r;
        textureColor.rgb = lerp(textureColor.rgb, fogColor.rgb, fog4.rgb);
    }
#endif

#if defined PS_USE_ALFA_TEXTURE // 33: Alfa Map: (Optional AlfaMap for blending textutres)
	if (hasAlfaMap)
		textureColor.a = AlfaMapTexture.Sample(SampleType, input.texCoords).r;
#endif

#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
	if (hasAlfaColor)
		textureColor.a = alfaColor;
#endif
   	
    return textureColor;
}
