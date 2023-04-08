// --------------------------------------------------------------------------------------------
// Filename: 022Texture.hlsl [LATEST]
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

//#define WITHOUT_CONSTANT_BUFFER

//////////////
// TYPEDEFS //
//////////////

// VERTEX Parameters:
struct VSIn
{
    float3 position:	POSITION;
    float2 texCoords:	TEXCOORD0; //22
};

// PIXEL Parameters:
struct PSIn
{
    float4	position:		SV_POSITION;
    float2	texCoords:		TEXCOORD0;
	//float3	originalPosition	: ORIGINAL_POSITION;	// 30 SKY
	//float	fogFactor:		FOG;							// 31 FOG
	//float4  cameraPosition: WS;
};

////////////////
// VERTEX BUFFER
////////////////
//CBV-srvHeapDesc	|0| DescriptorTable  | b0				|
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer// SYNC: DXshaderClass.h
#endif
#ifdef  DXAPI12 //#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0)
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
//CBV-srvHeapDesc	|1| DescriptorTable  | t0				|
#if DXAPI11 == 1
cbuffer PSShaderParametersBuffer// SYNC: DXshaderClass.h
#endif
#ifdef  DXAPI12
cbuffer PSShaderParametersBuffer : register(b1)// SYNC: DXshaderClass.h
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


//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture;
#endif
#if DXAPI12 == 1
Texture2D shaderTexture:	register(t0);
#endif

#if DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader027Texture(VSIn input)
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

    output.texCoords = input.texCoords;						// TEXTURE: Store the texture coordinates for the pixel shader:

    return output;
}


float4 MyPixelShader027Texture(PSIn input) : SV_TARGET
{
	float4 color = shaderTexture.Sample(SampleType, input.texCoords);

	if (isFont)	// isFont? clip by color...
	{
		
		if (color.r <= 0.1f && color.g <= 0.1f && color.b <= 0.1f)	// If the color is black on the texture then treat this pixel as transparent.
			discard; // Black is transparent: = clip (-1.0); return (float4)0;
		else
			color.rgb = pixelColor.rgb;							// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
	} 

	color.rgb*=fade;
	
	return color;
}
