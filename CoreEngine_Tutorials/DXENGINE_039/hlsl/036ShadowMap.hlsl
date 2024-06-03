// --------------------------------------------------------------------------------------------
// Filename: 045ShadowMap.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : http://woma.servegame.com
*
**********************************************************************************************/

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
    float3  position : POSITION;
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float  fDepth	: TEXCOORD0;
};

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsl"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

PSIn MyVertexShader036ShadowMap(VSIn input)
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
	output.fDepth = output.position.z / output.position.w;		// Store the position value in a second input value for depth value calculations.

	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

float4 MyPixelShader036ShadowMap(PSIn input) : SV_TARGET
{
	// Depth is z / w
	return  float4 (input.fDepth, 0, 0, 1);		// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
}

