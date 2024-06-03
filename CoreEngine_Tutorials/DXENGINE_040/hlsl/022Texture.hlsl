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

//////////////
// TYPEDEFS //
//////////////

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
};

//Set on: DXmodelClass::RenderSubMesh
Texture2D shaderTexture:	register(t0);	//DX12: SRV
SamplerState SampleType;	//3D (default) WRAP

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsl"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader022Texture(VSIn input)
{
    PSIn output;

if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
} else {
	float4 position = float4(input.position, 1);
	position = mul(position, worldMatrix);
	position = mul(position, view);			//viewMatrix
	position = mul(position, projection);	//projectionMatrix
	output.position = position;
}

    output.texCoords = input.texCoords;						// TEXTURE: Store the texture coordinates for the pixel shader:

    return output;
}

float4 MyPixelShader022Texture(PSIn input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location:
	float4 color = shaderTexture.Sample(SampleType, input.texCoords);

	color.rgb*=fade;

	return color;
}
