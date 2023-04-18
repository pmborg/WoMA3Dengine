// --------------------------------------------------------------------------------------------
// Filename: 023Light.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
#define DESKTOP_GL 1

//#define WITHOUT_CONSTANT_BUFFER

#define PS_USE_ALFA_TEXTURE	 //33
#define PS_USE_ALFACOLOR 	 //33


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
	//float3 originalPosition	: ORIGINAL_POSITION;	// 40 SKY
	//float  fogFactor			: FOG;					// 41 FOG
	//float3 viewDirection		: TEXCOORD1;			// 34 Specular
	//float4 lightViewPosition	: LIGHT_VIEW_POSITION;	// 35 & 51 SHADOWS : SHADER_TEXTURE_LIGHT_CASTSHADOW_INSTANCED
	//float3 tangent			: TANGENT;				// 37 & 51 BUMP   : SHADER_NORMAL_BUMP_INSTANCED
	//float4 cameraPosition		: WS;
};

////////////////
// VERTEX BUFFER
////////////////

// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0) //Register is needed for DX12: Descriptor: 0
#endif
{
	// BLOCK: VS1
	matrix	worldMatrix;	//worldMatrix
	matrix  WV;				//worldMatrix+viewMatrix
	matrix  WVP;			//worldMatrix+viewMatrix+projectionMatrix

	// 23 BLOCK: VS2
	bool	VShasLight;
	bool	VShasSpecular;
	bool	VShasNormMap;
	bool	VShasFog;

	// 23 BLOCK: VS3
	float3	VSlightDirection;	// LIGHT
	float   VSPad1;
	float4	VSambientColor;		// LIGHT
	float4	VSdiffuseColor;		// LIGHT
	float4	VSemissiveColor;	// LIGHT: Ke

	// 31 BLOCK: VS4
	float	VSfogStart;
	float	VSfogEnd;
	bool	VShasShadowMap;
	float	VSpad2;

	// 45 BLOCK: VS5
	matrix	ViewToLightProj;
};

///////////////
// PIXEL BUFFER
///////////////
//Note: on DX11 dont use: register()

// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer PSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer PSShaderParametersBuffer : register(b1)	//Register is needed for DX12: Descriptor: 1
#endif
{
	// BLOCK1:
	float4	pixelColor;

	// BLOCK2:
	bool	hasTexture;		// No? Use pixelColor, then.
	bool    hasLight;		// Future Load Obj. Engine Level
	bool	hasSpecular;	// Future Load Obj. Engine Level
	bool	isFont;			// Future Load Obj. Engine Level

	// BLOCK3:
	float4	ambientColor;	// LIGHT: Ka
	float4	diffuseColor;	// LIGHT: Kd
	float4	emissiveColor;	// LIGHT: Ke 
	float4	lightDirection;	// LIGHT

	// BLOCK4:
	bool	hasColorMap;		// 66
	float	lightType;			// Future
	float	shaderType;			// Future
	float	shaderTypeParameter;// Future

	// BLOCK5:
	bool	hasAlfaColor;
	float	alfaColor;
	float	fade;			// Fade from 0 to 1
	float	frameTime;		// For animations

	// BLOCK6:
	bool	hasFog;
	bool	isSky;
	bool    hasAlfaMap;
	bool	hasNormMap;

	// BLOCK7:
	float3	cameraPosition;	// NOT USED!
	bool	castShadow;
	float3	specularColor;
	float	nShininess;
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

////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc1(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
	return saturate(dot(Normal, -lightDirection));							// Calculate the amount of light on this pixel
}
////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc2(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
	return saturate(dot(Normal, lightDirection));							// Calculate the amount of light on this pixel
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader023Light(VSIn input)
{
	PSIn output;

#if defined WITHOUT_CONSTANT_BUFFER
	matrix WVPMatrix = {
		1.24942219f, 0.000000001f, 0.000000002f, 0.000000003f,	// row 1
		0.000000006f, 2.35672379f, 0.523716450f, -0.523716271f,	// row 2
		0.000000007f, -0.216972843f, 0.976377785f, 8.14494896f,	// row 3
		0.000000008f, -0.216930464f, 0.976187050f, 8.24335766f,	// row 4
	};

	output.position = mul(float4(input.position, 1), transpose(WVPMatrix));	// Calculate the position of the vertex against the world, view, and projection matrices
#else
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
#endif

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	//23: LIGHT: NORMAL
	if (VShasLight || VShasSpecular) 
		output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));// Calculate the normal vector against the world matrix only

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader023Light(PSIn input) : SV_TARGET
{
	float4	textureColor = pixelColor;    // SET PIXEL COLOR
	float	lightIntensity = 0;

	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
	if (hasTexture) 
		textureColor = shaderTexture.Sample(SampleType, input.texCoords);

	// 23 & 43: LIGHT
	//if (hasLight) 
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

#if defined PS_USE_ALFACOLOR
	if (hasAlfaColor)
		textureColor.a = alfaColor;
#endif

	return textureColor;
}
