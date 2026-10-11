#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: light.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan LIGHT shader - LEVEL 34
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inViewDir;


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
	int   castShadow;
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
    int hasFog;
    int isSky;
    int hasAlfaMap;
    int hasNormMap;
} pc;

// --------------------------------------------------------------------------------------------
// TEXTURE
// --------------------------------------------------------------------------------------------
               //0  VS UBO
layout(binding = 1) uniform sampler2D shaderTexture;
               //2  PS UBO
layout(binding = 3) uniform sampler2D alfaMapTexture; // LEVEL 33 Alfa Color - 34 Add Alfa Map

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc1(vec3 Normal) { return vec4(max(dot(Normal, -ps.lightDirection.xyz), 0.0)); }

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
vec4 PSlightFunc2(vec3 Normal) { return vec4(max(dot(Normal, ps.lightDirection.xyz), 0.0)); }

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
	float lightIntensity = 0.0;
	int mode = 0;	//Texture Mode for now Hardcoded.

	// 1. Base Texture or Pixel Color
	if (pc.hasTexture > 0)
	{
		textureColor =  GetShaderTextureSimple(shaderTexture, inTexCoords, 0, mode);
	}
	
    // 2 LIGHT: Level 23 Difuse and Ambient Light
	if (pc.hasLight>0)
	{
        //if (pc.isSky==0)
        //    lightIntensity = PSlightFunc2(inNormal).x;
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
	
	// 3. Alfa Color (LEVEL 33)
    if (pc.hasAlfaColor > 0)
        textureColor.a = pc.alfaColor;
	
	// 4. Alfa Mapping (LEVEL 34)
    if (pc.hasAlfaMap > 0) {
        textureColor.a = texture(alfaMapTexture, inTexCoords).r;
    }
	
    // 5. SPECULAR LIGHT (LEVEL 34)
    if (pc.hasSpecular > 0) 
	{
        if (lightIntensity > 0) 
		{
            vec3 reflection = reflect(normalize(ps.lightDirection.xyz), inNormal);
			
            float fPhongValue = max(dot(reflection, inViewDir), 0.0);
			
            vec4 specular = vec4(pow(fPhongValue, ps.nShininess));
            
            textureColor = clamp(textureColor + specular, 0.0, 1.0);
        }
    }
	
    // Gloval Fase Effect (LEVEL 29)
    textureColor.rgb *= ps.fade;
	
    outColor = textureColor;
}