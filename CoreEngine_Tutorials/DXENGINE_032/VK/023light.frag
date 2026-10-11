#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: light.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader
// LEVEL 23 - LIGHT
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inNormal;

// --------------------------------------------------------------------------------------------
// FRAGMENT OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec4 outColor;

// --------------------------------------------------------------------------------------------
// PS CONSTANT BUFFER
// --------------------------------------------------------------------------------------------
layout(binding = 2) uniform PSShaderParametersBuffer
{
    // BLOCK 1
    //vec4 pixelColor;

    // BLOCK 2
    //int hasTexture;
    //int hasLight;
    //int hasSpecular;
    //int isFont;

    // BLOCK 3
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 emissiveColor;
    vec4 lightDirection;

    // BLOCK 4
    //int   hasColorMap;
    //float lightType;
    //float isDay;
    //float shaderTypeParameter;

    // BLOCK 5
    int   pad1; //hasAlfaColor;
    float pad2; //alfaColor;
    float fade;
    float frameTime;

    // BLOCK 6
    //int hasFog;
    //int isSky;
    //int hasAlfaMap;
    //int hasNormMap;

} ps;

// --------------------------------------------------------------------------------------------
// PC CONSTANTS
// --------------------------------------------------------------------------------------------
layout(push_constant) uniform PushConstants
{
	// BLOCK 1
    vec4  pixelColor;

	// BLOCK 2
    int   hasTexture;
    int   hasLight;
    int	  hasSpecular;
    int   isFont;

    // BLOCK 4
    int   hasColorMap;
    float lightType;
    float isDay;
    float shaderTypeParameter;

	// BLOCK 5 - half of the data is in PSShaderParametersBuffer
    int   hasAlfaColor;
	float alfaColor;
    float pad1; //fade;
    float pad2; //frameTime;

    // BLOCK 6
    int hasFog;
    int isSky;
    int hasAlfaMap;
    int hasNormMap;
} pc;

// --------------------------------------------------------------------------------------------
// TEXTURE
// DX12: register(t0)
// Vulkan: binding 1
// --------------------------------------------------------------------------------------------
layout(binding = 1) uniform sampler2D shaderTexture;

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc1(vec3 Normal)
{
    return vec4(
        max(dot(Normal, ps.lightDirection.xyz), 0.0)
    );
}

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc2(vec3 Normal)
{
    return vec4(
        max(dot(Normal, -ps.lightDirection.xyz), 0.0)
    );
}

#include "TextureSampling.glsl"


// --------------------------------------------------------------------------------------------
// MAIN
// --------------------------------------------------------------------------------------------
void main()
{
    //vec4 textureColor = vec4(0.0);
    //float lightIntensity = 0.0;

    // ------------------------------------------------------------------------
    // Texture filtering demo
    //
    // 0 = Nearest
    // 1 = Bilinear
    // 2 = Trilinear
    // 3 = Cubic
    // 4 = Default hardware sampler
    // ------------------------------------------------------------------------

    int mode = int(clamp(inTexCoords.x, 0.0, 1.0) * 5.0);
    vec4 textureColor =  GetShaderTexture(shaderTexture, inTexCoords, 0, mode);

    // ------------------------------------------------------------------------
    // LIGHT
    // ------------------------------------------------------------------------
    float lightIntensity = PSlightFunc2(inNormal).x;

    vec4 lighting = ps.emissiveColor + ps.ambientColor + (lightIntensity * ps.diffuseColor);

    lighting = clamp(lighting, 0.0, 1.0);

    textureColor *= lighting;

    textureColor.a = 1.0;

    
    //outColor = vec4(1, 1, 1, 1);
    outColor = textureColor;
}