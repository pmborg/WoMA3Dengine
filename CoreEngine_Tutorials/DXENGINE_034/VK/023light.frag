#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: light.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan LIGHT shader - LEVEL 33
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
// ABI must match vkShaderClass::PSPushConstants exactly.
// Do not reorder fields.
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
               //0  VS UBO
layout(binding = 1) uniform sampler2D shaderTexture;
               //2  PS UBO

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc1(vec3 Normal) { return vec4(max(dot(Normal, ps.lightDirection.xyz), 0.0)); }

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc2(vec3 Normal) { return vec4(max(dot(Normal, -ps.lightDirection.xyz), 0.0)); }

#include "TextureSampling.glsl"

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

// --------------------------------------------------------------------------------------------
// MAIN
// --------------------------------------------------------------------------------------------
void main()
{
    vec4 textureColor = pc.pixelColor;

	if (pc.hasTexture>0)
	{
		int mode = 0;
		textureColor =  GetShaderTextureSimple(shaderTexture, inTexCoords, 0, mode);
	}
	
    // ------------------------------------------------------------------------
    // LIGHT
    // ------------------------------------------------------------------------
	if (pc.hasLight>0)
	{
		float lightIntensity = 0.0;
		
        //if (pc.isSky==0)
        //    lightIntensity = PSlightFunc2(inNormal)2x;
        //else
            lightIntensity = PSlightFunc2(inNormal).x;
	
		vec4 lighting;
		if (pc.hasTexture>0)
		{
			lighting = ps.emissiveColor + ps.ambientColor + (lightIntensity);
		}
		else
		{
			lighting = ps.emissiveColor + ps.ambientColor + (lightIntensity * ps.diffuseColor);
		}

		lighting = clamp(lighting, 0.0, 1.0);

		textureColor *= lighting;
	}
	
    if (pc.hasAlfaColor>0)
        textureColor.a = pc.alfaColor;
	
    outColor = textureColor;
}