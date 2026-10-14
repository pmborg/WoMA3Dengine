#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: font.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader - LEVEL 27
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec2 fragTexCoord;

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
// DX12: register(t0)
// Vulkan: binding 1
// --------------------------------------------------------------------------------------------
               //0  VS UBO
layout(binding = 1) uniform sampler2D shaderTexture;
               //2  PS UBO

// --------------------------------------------------------------------------------------------
// MAIN
// --------------------------------------------------------------------------------------------
void main()
{
    vec4 color = texture(shaderTexture, fragTexCoord);

    // Same semantic behavior as the mature HLSL font path:
    // black = transparent, non-black = font pixel.
    if (pc.isFont != 0)
    {
        if (color.r <= 0.1 &&
            color.g <= 0.1 &&
            color.b <= 0.1)
        {
            discard;
        }
        else
        {
            color.rgb = pc.pixelColor.rgb;
        }
    }
    color.rgb *= ps.fade;

    outColor = color;
}