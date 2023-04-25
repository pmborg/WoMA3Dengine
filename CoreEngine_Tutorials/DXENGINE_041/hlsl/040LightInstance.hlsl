// --------------------------------------------------------------------------------------------
// Filename: 040LightInstance.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : http://woma.servegame.com
*
**********************************************************************************************/
#define DESKTOP_GL 1

#define PS_USE_LIGHT		 //23
#define PS_USE_ALFA_TEXTURE	 //33
#define PS_USE_ALFACOLOR 	 //33
#define PS_USE_SPECULAR		 //34

//#define PS_USE_BUMP             
//#define PS_USE_FADE
//#define PS_USE_SKY

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
	float3 position : POSITION;				//21
	float2 texCoords: TEXCOORD; 			//22
	float3 normal	: NORMAL;				//23
	float3 instancePosition : INSTANCEPOS;	//40: D3D11_INPUT_PER_INSTANCE_DATA
};

// PIXEL:
struct PSIn
{
    float4 position			: SV_POSITION;			// 21
    float2 texCoords		: TEXCOORD0;			// 22
	float3 normal			: NORMAL;				// 23 & 47: LIGHT+BUMP
	float3 viewDirection	: TEXCOORD1;			// 44 Specular: SHADER_TEXTURE_LIGHT_INSTANCED
	float4 cameraPosition	: WS;					// FOG & SPECULAR
	
	//float3 originalPosition	: ORIGINAL_POSITION;	// SKY
	//float  fogFactor			: FOG;					// FOG
	//float4 lightViewPosition	: LIGHT_VIEW_POSITION;	// SHADOWS : SHADER_TEXTURE_LIGHT_CASTSHADOW_INSTANCED
	//float3 tangent			: TANGENT;				// BUMP   : SHADER_NORMAL_BUMP_INSTANCED
};

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsl"

#include "light.hlsl"

/////////////
// GLOBALS //
/////////////
//Set on: DXmodelClass::RenderSubMesh

Texture2D shaderTexture;			//: register(t0);	// 21:
Texture2D AlfaMapTexture;			//: register(t1);	// 43: AlfaMap
//Texture2D ShadowMapTextureTexture;	//: register(t2);	// 45: ShadowMap

SamplerState SampleType;		//: register(s0);		// 3D (default) WRAP
//SamplerState SampleTypeClamp;	//: register(s1);

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader040LightInstance(VSIn input, uint instanceID : SV_InstanceID)
{
    PSIn output;
	float4 cameraPosition;

	/*
	_m00, _m01,_m02, _m03 
	_m10, _m11,_m12, _m13 
	_m20, _m21,_m22, _m23 
	_m30, _m31,_m32, _m33 
	*/
	// 40 - Update the position of the vertices based on the data for this particular instance.
	input.position.x += input.instancePosition.x * (1/worldMatrix._m00);
	input.position.y += input.instancePosition.y * (1/worldMatrix._m11);
	input.position.z += input.instancePosition.z * (1/worldMatrix._m22);

	//21: POSITION: Calculate the position of the vertex against the world, view, and projection matrices
	output.position = mul(float4(input.position,1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices:

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	cameraPosition = mul(float4(input.position, 1), WV);								// FOG: Calculate the camera position.

	//23: LIGHT: NORMAL
	if (VShasLight || VShasSpecular) 
		output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));// Calculate the normal vector against the world matrix only
	
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



////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader040LightInstance(PSIn input) : SV_TARGET
{
	float4	textureColor = pixelColor;    // SET PIXEL COLOR
	float	lightIntensity = 0;

	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
	if (hasTexture) 
		textureColor = shaderTexture.Sample(SampleType, input.texCoords);

	// 23: LIGHT
	if (hasLight) 
	{
		if (lightType == 1)	
			lightIntensity = PSlightFunc1(input.normal);
		else
			lightIntensity = PSlightFunc2(input.normal);

		if (hasTexture) {
			textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);	
		} else {
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

    return textureColor;
}