#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
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
// Corresponds to DX: PSShaderParametersBuffer
// HLSL register(b1)
// Vulkan binding 2
// --------------------------------------------------------------------------------------------
layout(binding = 2) uniform PSShaderParametersBuffer
{
    // BLOCK1
    vec4 pixelColor;

    // BLOCK2
    // HLSL bool = 32-bit value in constant buffer
    int hasTexture;
    int hasLight;
    int hasSpecular;
    int isFont;

    // BLOCK3
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 emissiveColor;
    vec4 lightDirection;

} ps;

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

    outColor = textureColor;
}