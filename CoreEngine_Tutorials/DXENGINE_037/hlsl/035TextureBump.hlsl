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

#define PS_USE_LIGHT		 //23
#define PS_USE_ALFA_TEXTURE	 //33
#define PS_USE_ALFACOLOR 	 //33
#define PS_USE_SPECULAR		 //34
#define PS_USE_BUMP		 	 //35

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn						
{
	float3 position : POSITION;	//21
	float2 texCoords: TEXCOORD; //22
	float3 normal	: NORMAL;	//23
	float3 tangent	: TANGENT;	//35 NEW!!
};

// PIXEL:
struct PSIn						
{
	float4 position				: SV_POSITION;			// 21
	float2 texCoords			: TEXCOORD;				// 22
	float3 normal				: NORMAL;				// 23 LIGHT
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
	float3 tangent				: TANGENT;				// 35 BUMP
};



/////////////
// GLOBALS //
/////////////

//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture;		// 22: Texture
Texture2D AlfaMapTexture;		// 33: AlfaMap
Texture2D TangentMapTexture;	// 35: TangentMap NEW!!
#endif
#if DXAPI12 == 1
Texture2D AlfaMapTexture:		register(t0); // 33: AlfaMap	//DX12: Descriptor: 2
Texture2D shaderTexture:		register(t1); // 22: Texture	//DX12: Descriptor: 3
Texture2D TangentMapTexture:	register(t1); // 35: TangentMap //DX12: Descriptor: 4
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
#include "cbuffer.hlsl"
#include "light.hlsl"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader035TextureBump(VSIn input)
{
	PSIn output;
	float4 cameraPosition;

	//21: POSITION: Calculate the position of the vertex against the world, view, and projection matrices
if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
} else {
	float4 position = float4(input.position, 1);
	position = mul(position, worldMatrix);
	position = mul(position, view);			//viewMatrix
	position = mul(position, projection);	//projectionMatrix
	output.position = position;
}

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	//Used by PS_USE_SPECULAR and PS_USE_FOG
	cameraPosition = mul(float4(input.position, 1), WV);

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

	//35: BUMP NEW!!
#if defined PS_USE_BUMP
    if (VShasNormMap)
        output.tangent = mul(input.tangent, worldMatrix);
#endif

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader035TextureBump(PSIn input) : SV_TARGET
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
	//35: BUMP NEW!!
	#if defined PS_USE_BUMP
		float3 normal = input.normal;
		
		if (hasNormMap) 
		{
			float4 normalMap	= TangentMapTexture.Sample( SampleType, input.texCoords);	// Load normal from normal map
			normalMap			= (2.0f*normalMap) - 1.0f;							// Change normal map range from [0, 1] to [-1, 1]
			float3 tangent		= normalize(input.tangent - dot(input.tangent, input.normal)*input.normal);	//Make sure tangent is completely orthogonal to normal
			float3 biTangent	= cross(input.normal, input.tangent);				// Create the biTangent
			float3x3 texSpace	= float3x3(tangent, biTangent, input.normal);		// Create the "Texture Space"
			normal				= normalize(mul(normalMap, texSpace));				// BUMP: Convert normal from normal map to texture space and store in input.normal
		}
	#endif
	
		//if (lightType == 1)	
			lightIntensity = PSlightFunc1(input.normal);
		//else
		//	lightIntensity = PSlightFunc2(input.normal);

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
