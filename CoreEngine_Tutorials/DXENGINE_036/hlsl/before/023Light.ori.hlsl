// --------------------------------------------------------------------------------------------
// Filename: 023Light.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : http://woma.servegame.com
*
**********************************************************************************************/
#include "include/000_ENGINE_LEVEL.hlsl"

/*
	#define PS_USE_ALFACOLOR		//26
	#define PS_USE_FONT				//28
	#define PS_USE_SKY				//30
	#define PS_USE_FOG				//31
	#define PS_USE_FADE				//34
	#define PS_USE_ALFA_TEXTURE		//43
	#define PS_USE_SPECULAR			//44
	#define PS_USE_SHADOWMAP_TEXTURE//45
*/

//define GS_USE

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
	float4 position			: SV_POSITION;			// 21
	float2 texCoords		: TEXCOORD2;			// 22
	float3 normal			: NORMAL;				// 23 LIGHT
	float3 originalPosition	: ORIGINAL_POSITION;	// 30 SKY
	float  fogFactor		: FOG;					// 31 FOG
	float3 viewDirection	: TEXCOORD1;			// 44 Specular
	//float4 lightViewPosition: LIGHT_VIEW_POSITION;	// 45 & 51 SHADOWS : SHADER_TEXTURE_LIGHT_CASTSHADOW_INSTANCED
	//float3 tangent			: TANGENT;				// 47 & 51 BUMP   : SHADER_NORMAL_BUMP_INSTANCED
	float4 cameraPosition	: WS;
};

/////////////
// GLOBALS //
/////////////

//Set on: DXmodelClass::RenderSubMesh
Texture2D shaderTexture:	register(t0);			//DX12: SRV

#if defined PS_USE_ALFA_TEXTURE
Texture2D AlfaMapTexture:	register(t1);			// 43: AlfaMap
#endif

#if defined PS_USE_SHADOWMAP_TEXTURE
Texture2D ShadowMapTextureTexture: register(t2);	//45: SHADOWS (REMOVED FROM 023)
#endif

//------------------------------------------------------------------------------
SamplerState SampleType /*: register(s0)*/;	//3D (default) WRAP

#include "include/VSbuffer.hlsl" //DX12: CBV
#include "include/PSbuffer.hlsl" //DX12: CBV


#pragma message( "TARGET: VS" )  
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader023Light(VSIn input)
{
	PSIn output;
	float4 cameraPosition;

#if defined PS_USE_SKY
	output.originalPosition = input.position;
#endif

	//21: COMMON
	#ifdef OLDWAY
	//----- 4 Lines in 1 -----
	//input.position.w = 1.0f;
	//posWorld = mul(input.position, worldMatrix);
	//PosView = mul(posWorld, viewMatrix);
	//Position = mul(PosView, projectionMatrix);
	#endif

	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	cameraPosition = mul(float4(input.position, 1), WV);								// FOG: Calculate the camera position.
#if defined GS_USE || defined PS_USE_SKY
	output.cameraPosition = cameraPosition;
#endif

	//23: LIGHT: NORMAL
	if (VShasLight || VShasSpecular) 
	{
	#ifdef OLDWAY
		//----- 2 Lines in 1 -----
		//normal = mul(input.normal, (float3x3)worldMatrix);
		//normal = normalize(output.normal);
	#endif
		output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));// Calculate the normal vector against the world matrix only
	}

#if defined PS_USE_FOG
	//31: // FOG: Calculate the camera position.
	if (VShasFog)
	{
	#ifdef OLDWAY
		//----- 2 Lines in 1 -----
		//cameraViewPosition = mul(input.position, worldMatrix);
		//cameraViewPosition = mul(cameraPosition, viewMatrix);
	#endif
		float4 cameraViewPosition = mul(float4(input.position, 1), WV);
		output.fogFactor = saturate((VSfogEnd - cameraViewPosition.z) / (VSfogEnd - VSfogStart));	// Calculate linear fog.  
	}
#endif

#if defined PS_USE_SPECULAR
	//44: SPECULAR
	// http://3dgep.com/texturing-lighting-directx-11/#Specular
	if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);			// P
		output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);	// L = Lp - p (L = lightDirection)
	}
#endif

	//45: SHADOWS (REMOVED FROM 023)
	//if (VShasShadowMap)
	//	output.lightViewPosition = mul(mul(float4(input.position, 1), worldMatrix), ViewToLightProj);

	return output;
}



#if defined PS_USE_LIGHT
	#include "include/light.hlsl"	
#endif
#if defined PS_USE_SHADOWMAP_TEXTURE
	#include "include/shadows.hlsl"	
#endif
#if defined PS_USE_SKY
	#include "include/sky.hlsl"		
#endif

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader023Light(PSIn input) : SV_TARGET
{
	float4	textureColor = pixelColor;    // SET PIXEL COLOR
	float	lightIntensity = 0;
	float4	fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);

	//#METHOD:1 - 023Light.hlsl
	// 100% FOG: Need to be after sky?? Sky as no fog.... (No fog on Sky) ;)
#if defined PS_USE_FOG
	if (hasFog && input.fogFactor == 0)
		return fogColor	* fade;
#endif

#if defined PS_USE_SKY
	// 30: SKY
	if (isSky)
		if (hasFog)
			return fogColor;
		else
			return Sky(input.cameraPosition.y, input.originalPosition.y, input.texCoords);
#endif

	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
	if (hasTexture)
		textureColor = shaderTexture.Sample(SampleType, input.texCoords);

	//return textureColor;
	// 23 & 43: LIGHT
	if (hasLight) {
		lightIntensity = PSlightFunc2(input.normal);

		if (hasTexture) {
			//textureColor = textureColor * lightIntensity;
			textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);	
		} else {
			textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
		}
	}

#if defined PS_USE_SPECULAR
	//44:
	// http://3dgep.com/texturing-lighting-directx-11/#Specular
	if (hasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		if (lightIntensity > 0.0f)
		{
			float4 color = ambientColor;
			color += (diffuseColor * lightIntensity);
			color = saturate(color);

			float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
			float  fPhoneValue = saturate(dot(Reflection, input.viewDirection));	// (R.V)
			float4 specular = pow(fPhoneValue, nShininess);							// Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

			color = color * textureColor;
			textureColor = saturate(textureColor + specular);		// specular = Ls (contribution of the light source) * Ks (specular component of the material)
		}
	}
#endif

#if defined PS_USE_FOG
	// 31: FOG: Calculate the final color using the fog effect equation.
	if (hasFog)
		textureColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;
#endif

#if defined PS_USE_ALFA_TEXTURE
	// 43: Alfa Map: (Optional AlfaMap for blending textutres)
	if (hasAlfaMap)
		textureColor.a = AlfaMapTexture.Sample(SampleType, input.texCoords).r;
#endif

//#if defined PS_USE_SHADOWMAP_TEXTURE
	//45: SHADOWS (REMOVED FROM 023)
	//if (castShadow)
	//	textureColor.rgb = textureColor.rgb * HasShadows(input.lightViewPosition);
//#endif

#if defined PS_USE_FADE
	// FADE: Used by Sun or All (at night)
	if (fade < 1)
		textureColor.rgb = textureColor.rgb * fade;
#endif

#if defined PS_USE_ALFACOLOR
	if (hasAlfaColor)
		textureColor.a = alfaColor;
#endif

	return textureColor;
}


