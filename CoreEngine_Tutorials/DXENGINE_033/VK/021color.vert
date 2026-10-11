#version 450

// --------------------------------------------------------------------------------------------
// Filename: color.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader
// LEVEL 21 - COLOR
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Vertex Input
// Equivalent to:
// struct VSIn
// {
//     float3 position : POSITION;
//     float4 color    : COLOR;
// };
// -----------------------------------------------------------------------------

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;


// -----------------------------------------------------------------------------
// Vertex Output
// Equivalent to:
// struct PSIn
// {
//     float4 position : SV_POSITION;
//     float4 color    : COLOR;
// };
// -----------------------------------------------------------------------------

layout(location = 0) out vec4 outColor;


// -----------------------------------------------------------------------------
// Constant Buffer
//
// DX12:
//     cbuffer VSShaderParametersBuffer : register(b0)
//
// Vulkan:
//     set = 0, binding = 0
// -----------------------------------------------------------------------------

layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
{
    mat4 WVP;
} cb;


// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------

void main()
{
//DEBUG for op2:
/*
const mat4 WVP_DEBUG = mat4(
    1.24942219, 0.000000001, 0.000000002, 0.000000003,
    0.000000006, 2.35672379, 0.523716450, -0.523716271,
    0.000000007, -0.216972843, 0.976377785, 8.14494896,
    0.000000008, -0.216930464, 0.976187050, 8.24335766
);
*/
	//OP1: (better)
	gl_Position = cb.WVP * vec4(inPosition, 1.0);
	gl_Position.y = -gl_Position.y;
	
	//OP2:
	//gl_Position = WVP_DEBUG * vec4(inPosition, 1.0);	//DEBUG

    //op3: windows debug
	//vec4 p = cb.WVP * vec4(inPosition, 1.0);
	//gl_Position = p;
	//debugPrintfEXT ("CLIP: (%f,%f,%f,%f)\n", p.x, p.y, p.z, p.w);

	outColor = inColor;
	outColor.a = 1.0;
}
