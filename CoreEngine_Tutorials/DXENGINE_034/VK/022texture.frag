#version 450
#extension GL_GOOGLE_include_directive : enable
// --------------------------------------------------------------------------------------------
// Filename: texture.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader
// LEVEL 22 - TEXTURE
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

layout(location = 0) in vec2 inTexCoords;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1)
uniform sampler2D shaderTexture;

#include "TextureSampling.glsl"

void main()
{
	// Demo: split the quad in 5 vertical bands for 5 filters
	//int mode = int(saturate(input.texCoords.x) * 5.0); // 0..4
	#define TEXTURE_MODE 0
	outColor = GetShaderTexture(shaderTexture, inTexCoords, 0, TEXTURE_MODE);
}
