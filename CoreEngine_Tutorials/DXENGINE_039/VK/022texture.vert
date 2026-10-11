#version 450

// --------------------------------------------------------------------------------------------
// Filename: texture.vert
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader
// LEVEL 22 - TEXTURE
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// VERTEX INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

// --------------------------------------------------------------------------------------------
// VERTEX OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform UBO
{
    mat4 WVP;
} cb;

void main()
{
    gl_Position = cb.WVP * vec4(inPosition, 1.0);
	gl_Position.y = -gl_Position.y;
    fragTexCoord = inTexCoord;
}