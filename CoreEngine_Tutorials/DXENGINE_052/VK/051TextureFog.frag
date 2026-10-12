#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: 050TextureFog.frag
// WoMA Engine 3D - Vulkan Level 50
// Equivalent to DX: 051Texture.hlsl (Texture + Fog)
// --------------------------------------------------------------------------------------------

layout(location = 0) in vec2  inTexCoords;
layout(location = 1) in float fragFogFactor;
layout(location = 0) out vec4 outColor;

// --------------------------------------------------------------------------------------------
// TEXTURE
// --------------------------------------------------------------------------------------------
layout(set = 0, binding = 1) uniform sampler2D shaderTexture;

#include "TextureSampling.glsl"

void main()
{
	#define TEXTURE_MODE -1
	vec4 textureColor = GetShaderTexture(shaderTexture, inTexCoords, 0, TEXTURE_MODE);

    vec4 fogColor = vec4(87.0/256.0, 87.0/256.0, 87.0/256.0, 1.0);
	textureColor.rgb = mix(textureColor.rgb, fogColor.rgb, 1.0 - fragFogFactor);

	textureColor.a =0.5f;
    outColor = textureColor;
}