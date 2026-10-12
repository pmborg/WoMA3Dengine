#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: 054Texture.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan LIGHT shader - LEVEL 54
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------
// LEVEL 54: Water Movement
// --------------------------------------------------------------------------------------------

#include "TextureSampling.glsl"

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT FROM VERTEX
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in float inFogFactor;
layout(location = 3) in vec3 inWorldPos;

// --------------------------------------------------------------------------------------------
// FRAGMENT OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec4 outColor;

// --------------------------------------------------------------------------------------------
// ABI must match vkShaderClass::vkPSconstantBufferType exactly.
// Do not reorder fields.
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

    // BLOCK 3: 64 bytes
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 emissiveColor;
    vec4 lightDirection;

    // BLOCK 4
    //int   hasColorMap;
    //float lightType;
    //float isDay;
    //float shaderTypeParameter;

    // BLOCK 5: 16 bytes
	int   pad1;      // Reserved (old: hasAlfaColor)
	float pad2;      // Reserved (old: alfaColor)
    float fade;		 // Fade from 0 to 1
    float frameTime;

    // BLOCK 6
    //int hasFog;
    //int isSky;
    //int hasAlfaMap;
    //int hasNormMap;

	// BLOCK 7: 32 bytes
	vec3	cameraPosition;	// Future
	int		castShadow;
	vec3    specularColor;
	float   nShininess;
} ps;

// --------------------------------------------------------------------------------------------
// ABI must match vkShaderClass::PSPushConstants exactly.
// Do not reorder fields.
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
    int	  hasFog;
    int	  isSky;
    int	  hasAlfaMap;
    int	  hasNormMap;
} pc;

// -------------------------------------------------------------------------------------------- 
// TEXTURE DESCRIPTORS
// --------------------------------------------------------------------------------------------
               //0  VS UBO
layout(binding = 1) uniform sampler2D shaderTexture;
               //2  PS UBO

// -----------------------------------------------------------------------------
// Random helper (same as HLSL)
// -----------------------------------------------------------------------------
float rand_1_05(in vec2 uv)
{
    vec2 noise = vec2(fract(sin(dot(uv, vec2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc1(vec3 Normal) { return vec4(max(dot(Normal, -ps.lightDirection.xyz), 0.0)); }

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc2(vec3 Normal) { return vec4(max(dot(Normal, ps.lightDirection.xyz), 0.0)); }

// --------------------------------------------------------------------------------------------
// Hardware texture sampling.
// The Vulkan sampler created in C++ already controls:
//
//   - Wrap / Clamp / Mirror
//   - Nearest / Bilinear
//   - Trilinear (when mipmaps exist)
//   - Anisotropic filtering
//
// Only Cubic filtering requires a custom shader implementation.
// --------------------------------------------------------------------------------------------
// LEVEL33 NOTE:
// Modes 0,1,2 and 4 use Vulkan hardware texture sampling.
// Wrap/Clamp, Bilinear, Trilinear and Anisotropic filtering
// are controlled by the VkSampler created in C++.
// Only Cubic filtering requires a custom shader implementation.
vec4 GetShaderTextureSimple(sampler2D tex, vec2 uv, float mipLevel, int mode)
{
    switch (mode)
    {
        case 3: // Cubic (future/custom implementation)
            return CubicInterpolation(tex, uv);

        default:
            // Modes 0,1,2,4 use the hardware sampler.
            return texture(tex, uv);
    }
}

// -----------------------------------------------------------------------------
// Fragment Shader
// -----------------------------------------------------------------------------
void main()
{
	int mode = 0;	//Texture Mode for now Hardcoded.
	
    vec4 textureColor = inColor;
    textureColor.b += rand_1_05(vec2(gl_FragCoord.y)) * 0.1;

    vec4 color = inColor;

	//DEBUG ONLY!!!!
	//outColor = color;
	//return;

    //vec3 text = GetShaderTexture(shaderTexture, inTexCoord, 0, -1).rgb;
	vec3 text = GetShaderTextureSimple(shaderTexture, inTexCoord, 0, mode).rgb;

    color.rgb = text;

    if ((inWorldPos.x > 0.5 && inWorldPos.z > 0.5) && (inWorldPos.x < 128.0 && inWorldPos.z < 128.0))
    {
        textureColor = (textureColor + color) * 0.5;
    }
    else
    {
        textureColor *= color;
    }

    // -------------------------------------------------------------------------
    // LEVEL 33: Alpha Color
    // -------------------------------------------------------------------------
    if (pc.hasAlfaColor > 0)
        textureColor.a = pc.alfaColor;

	// Gloval Fade Effect (LEVEL 29)
	textureColor.rgb *= ps.fade;
	
    outColor = textureColor;
}
