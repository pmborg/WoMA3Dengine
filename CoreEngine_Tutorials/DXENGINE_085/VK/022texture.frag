#version 450
#extension GL_GOOGLE_include_directive : enable
// --------------------------------------------------------------------------------------------
// Filename: texture.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader
// LEVEL 22 - TEXTURE
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

layout(location = 0) in vec2  inTexCoords;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D shaderTexture;

#include "TextureSampling.glsl"

// --------------------------------------------------------------------------------------------
// PC CONSTANTS
// --------------------------------------------------------------------------------------------
layout(push_constant) uniform PushConstants
{
	// BLOCK 1: 16 bytes
    vec4  pixelColor;

	// BLOCK 2: 16 bytes
    int   hasTexture;
    int   hasLight;
    int	  hasSpecular;
    int   isFont;

    // BLOCK 4: 16 bytes
    int   hasColorMap;
    float lightType;
    float isDay;
    float shaderTypeParameter;

	// BLOCK 5: 16 bytes
    int   hasAlfaColor;
	float alfaColor;
    float pad1; //fade;
    float pad2; //frameTime;

    // BLOCK 6: 16 bytes
    int hasFog;
    int isSky;
    int hasAlfaMap;
    int hasNormMap;
} pc;

void main()
{
	#define TEXTURE_MODE -1
	outColor = GetShaderTexture(shaderTexture, inTexCoords, 0, TEXTURE_MODE);
	if (pc.hasAlfaColor > 0)
		outColor.a = pc.alfaColor;
}
