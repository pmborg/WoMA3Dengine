#version 450

// --------------------------------------------------------------------------------------------
// Filename: color.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader
// LEVEL 21 - COLOR
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

layout(location = 0) in vec4 inColor;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = inColor;
}