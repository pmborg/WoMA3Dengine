// --------------------------------------------------------------------------------------------
// Filename: 022Texture.hlsl [LATEST]
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

//////////////
// TYPEDEFS //
//////////////

//#define WITHOUT_CONSTANT_BUFFER

// VERTEX:
struct VSIn
{
    float3 position:	POSITION;
    float2 texCoords:	TEXCOORD0; //22
};

// PIXEL:
struct PSIn
{
    float4	position:		SV_POSITION;
    float2	texCoords:		TEXCOORD0;
	//float3	originalPosition	: ORIGINAL_POSITION;	// 30 SKY
	//float	fogFactor:		FOG;						// 31 FOG
	//float4  cameraPosition: WS;
};

////////////////
// VERTEX BUFFER
////////////////
//CBV-srvHeapDesc	|0| DescriptorTable  | b0				|
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer// SYNC: DXshaderClass.h
#endif
#if DXAPI12 == 1
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

	// 36 BLOCK: VS5
	matrix	ViewToLightProj;
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
PSIn MyVertexShader022Texture(VSIn input)
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

float4 MyPixelShader022Texture(PSIn input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location:
	float4 color = shaderTexture.Sample(SampleType, input.texCoords);
	return color;
}
