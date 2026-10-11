#version 450
#extension GL_EXT_debug_printf : enable

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
    //mat4 worldMatrix;
    //mat4 view;
    //mat4 projection;
    //mat4 WV;
    mat4 WVP;
} cb;


// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------

void main()
{
//DEBUG for op2:
//const mat4 WVP_DEBUG = mat4(
//    1.24942219, 0.000000001, 0.000000002, 0.000000003,
//    0.000000006, 2.35672379, 0.523716450, -0.523716271,
//    0.000000007, -0.216972843, 0.976377785, 8.14494896,
//    0.000000008, -0.216930464, 0.976187050, 8.24335766
//);

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
	//outColor = vec4(1.0, 1.0, 1.0, 1.0);				//DEBUG
}



//#version 450
//#extension GL_EXT_debug_printf : enable
//
//layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec4 inColor;
//
//layout(location = 0) out vec4 outColor;
//
//layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
//{
//    mat4 worldMatrix;
//    mat4 view;
//    mat4 projection;
//    mat4 WV;
//    mat4 WVP;
//} cb;
//
//void main()
//{
//    vec4 v = vec4(inPosition, 1.0);
//
//    debugPrintfEXT(
//        "VERTEX: (%f,%f,%f,%f)\n",
//        v.x, v.y, v.z, v.w
//    );
//
//    debugPrintfEXT(
//        "WVP ROW0: (%f,%f,%f,%f)\n",
//        cb.WVP[0][0],
//        cb.WVP[0][1],
//        cb.WVP[0][2],
//        cb.WVP[0][3]
//    );
//
//    debugPrintfEXT(
//        "WVP ROW1: (%f,%f,%f,%f)\n",
//        cb.WVP[1][0],
//        cb.WVP[1][1],
//        cb.WVP[1][2],
//        cb.WVP[1][3]
//    );
//
//    debugPrintfEXT(
//        "WVP ROW2: (%f,%f,%f,%f)\n",
//        cb.WVP[2][0],
//        cb.WVP[2][1],
//        cb.WVP[2][2],
//        cb.WVP[2][3]
//    );
//
//    debugPrintfEXT(
//        "WVP ROW3: (%f,%f,%f,%f)\n",
//        cb.WVP[3][0],
//        cb.WVP[3][1],
//        cb.WVP[3][2],
//        cb.WVP[3][3]
//    );
//
//    vec4 p = cb.WVP * v;
//
//    debugPrintfEXT(
//        "CLIP: (%f,%f,%f,%f)\n",
//        p.x, p.y, p.z, p.w
//    );
//
//    gl_Position = p;
//
//    outColor = inColor;
//}