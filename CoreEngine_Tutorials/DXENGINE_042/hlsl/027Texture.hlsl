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

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsl"

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
if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
} else {
	float4 position = float4(input.position, 1);
	position = mul(position, worldMatrix);
	position = mul(position, view);			//viewMatrix
	position = mul(position, projection);	//projectionMatrix
	output.position = position;
}
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
