// --------------------------------------------------------------------------------------------
// Filename: 041ShadowMapInstance.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : http://woma.servegame.com
*
*   is equivalent to 036ShadowMap.hlsl but with instances.
**********************************************************************************************/
#define DESKTOP_GL 1

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
    float3 position : POSITION;
	float3 instancePosition : INSTANCEPOS;	// 51: D3D11_INPUT_PER_INSTANCE_DATA
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

PSIn MyVertexShader041ShadowMapInstance(VSIn input, uint instanceID : SV_InstanceID)
{
    PSIn output;

	/*
	_m00, _m01,_m02, _m03 
	_m10, _m11,_m12, _m13 
	_m20, _m21,_m22, _m23 
	_m30, _m31,_m32, _m33 
	*/
	// 51 - Update the position of the vertices based on the data for this particular instance.
	input.position.x += input.instancePosition.x * (1/worldMatrix._m00);
	input.position.y += input.instancePosition.y * (1/worldMatrix._m11);
	input.position.z += input.instancePosition.z * (1/worldMatrix._m22);

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

float4 MyPixelShader041ShadowMapInstance(PSIn input) : SV_TARGET
{
	// Depth is z / w
	return  float4 (input.fDepth, 0, 0, 1);		// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
}

