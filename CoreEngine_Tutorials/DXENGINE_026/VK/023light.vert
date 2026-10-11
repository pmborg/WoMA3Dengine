#version 450

// --------------------------------------------------------------------------------------------
// LEVEL 23 - LIGHT
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// VERTEX INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;

// --------------------------------------------------------------------------------------------
// VERTEX OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec3 outNormal;

// --------------------------------------------------------------------------------------------
// VS CONSTANT BUFFER
// Corresponds to DX: VSShaderParametersBuffer
// HLSL register(b0)
// Vulkan binding 0
// --------------------------------------------------------------------------------------------
layout(binding = 0) uniform VSShaderParametersBuffer
{
    // VS1
    mat4 worldMatrix;
    mat4 view;
    mat4 projection;
    mat4 WV;
    mat4 WVP;

    // VS2
    // HLSL bools occupy 4 bytes in a cbuffer.
    int VShasLight;
    int VShasSpecular;
    int VShasNormMap;
    int VShasFog;

    // VS3
    vec4 VSlightDirection;
    vec4 VSambientColor;
    vec4 VSdiffuseColor;
    vec4 VSemissiveColor;

} vs;

// --------------------------------------------------------------------------------------------
// MAIN
// --------------------------------------------------------------------------------------------
void main()
{
    // Same as:
    //
    // output.position = mul(float4(input.position, 1), WVP);
    //
    gl_Position = vs.WVP * vec4(inPosition, 1.0);
	gl_Position.y = -gl_Position.y;

    // Texture coordinates (fallback)
    outTexCoords = inTexCoords;

    // Same as:
    //
    // normalize(mul(input.normal, (float3x3) worldMatrix))
    //
    outNormal = normalize(mat3(vs.worldMatrix) * inNormal);
}