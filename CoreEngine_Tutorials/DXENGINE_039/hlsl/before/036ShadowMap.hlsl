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
#define DESKTOP_GL 1

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

	output.position = mul(float4 (input.position,1), WVP);		// POSITION of the vertex against the world, view, and projection matrices:
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

