#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: lightShadow.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan LIGHTSHADOW shader - LEVEL 36
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

#define PS_USE_SPECULAR

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inViewDir;
layout(location = 3) in vec4 inLightViewPosition; // Recebido do VS

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
	int     castShadow;
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
// SAMPLERS / TEXTURES
// --------------------------------------------------------------------------------------------
               //0  VS UBO
layout(binding = 1) uniform sampler2D shaderTexture;
               //2  PS UBO
layout(binding = 3) uniform sampler2D alfaMapTexture; 
	           //4  Normal Map (not used on this shader/level)
layout(binding = 5) uniform sampler2D ShadowMapTextureTexture; // binding = 5 (t2 no DX)

vec4 PSlightFunc1(vec3 Normal) { return vec4(max(dot(Normal, -ps.lightDirection.xyz), 0.0)); }
vec4 PSlightFunc2(vec3 Normal) { return vec4(max(dot(Normal, ps.lightDirection.xyz), 0.0)); }

#include "TextureSampling.glsl"

vec4 GetShaderTextureSimple(sampler2D tex, vec2 uv, float mipLevel, int mode)
{
    return texture(tex, uv);
}

// --------------------------------------------------------------------------------------------
// LEVEL 36: Function Similar of 'HasShadows' in shadows.hlsli
// --------------------------------------------------------------------------------------------
float HasShadows1(vec4 lightViewPosition)
{
    vec2 uv;
    uv.x = lightViewPosition.x / lightViewPosition.w * 0.5 + 0.5;
    uv.y = -lightViewPosition.y / lightViewPosition.w * 0.5 + 0.5;

    if (uv.x < 0.0 || uv.x > 1.0 ||
        uv.y < 0.0 || uv.y > 1.0)
        return 1.0;

    float depthValue = texture(ShadowMapTextureTexture, uv).r;

    float lightDepth = min(1.0, lightViewPosition.z / lightViewPosition.w);

    const float   bias = 5.0f / 256.0f;
    return (lightDepth > depthValue + bias) ? 0.505 : 1.0;
}

float HasShadows(vec4 lightViewPosition)
{
    vec2 uv;
    uv.x = lightViewPosition.x / lightViewPosition.w * 0.5 + 0.5;
    uv.y = -lightViewPosition.y / lightViewPosition.w * 0.5 + 0.5;

    if (uv.x < 0.0 || uv.x > 1.0 ||
        uv.y < 0.0 || uv.y > 1.0)
        return 1.0;

    //float lightDepth = lightViewPosition.z / lightViewPosition.w;
	float lightDepth = min(1.0, lightViewPosition.z / lightViewPosition.w);
    if (lightDepth > 1.0)
        return 1.0;

	float NdotL = max(dot(normalize(inNormal), ps.lightDirection.xyz), 0.0);
	float bias = max(5.0 / 256.0, (1.0 - NdotL) * (2.0 / 256.0));

    // Shadow map is currently 1024x1024
    const vec2 texelSize = vec2(1.0 / 1024.0, 1.0 / 1024.0);

    float shadow = 0.0;

    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            vec2 sampleUV = uv + vec2(float(x), float(y)) * texelSize;

            float depthValue = texture(ShadowMapTextureTexture, sampleUV).r;

            shadow += (lightDepth > depthValue + bias) ? 0.505 : 1.0;
        }
    }

    return shadow / 9.0;
}

void main()
{
    vec4 textureColor = pc.pixelColor;
	float lightIntensity = 0.0;
	int mode = 0; //Texture Mode for now Hardcoded.

	// 1. textureColor = Base Texture Albedo or Pixel Color
	if (pc.hasTexture > 0)
	{
		textureColor = GetShaderTextureSimple(shaderTexture, inTexCoords, 0, mode);
	}

	// 2. Light Normals (LEVEL 23)
	//vec3 normal = normalize(inNormal);

	// 3 LIGHT: get lightIntensity for sky and terrain
    if (pc.lightType == 1.0)
        lightIntensity = PSlightFunc2(inNormal).x;
    else
        lightIntensity = PSlightFunc1(inNormal).x;
	
	// 4 LIGHT: Level 23 Difuse and Ambient Light
    if (pc.hasTexture > 0) 
        textureColor = textureColor * clamp(ps.emissiveColor + ps.ambientColor + lightIntensity, 0.0, 1.0);  
    else 
        textureColor = textureColor * clamp(ps.emissiveColor + ps.ambientColor + (lightIntensity * ps.diffuseColor), 0.0, 1.0);

    // 5. Shadow test:
    textureColor.rgb = textureColor.rgb * HasShadows(inLightViewPosition);

    // 6. SPECULAR LIGHT (LEVEL 34) + NORMAL BUMPED (LEVEL 35)
#if defined PS_USE_SPECULAR
    if (pc.hasSpecular > 0)    
    {
        if (lightIntensity > 0.0)
        {
            vec4 color = ps.ambientColor + (ps.diffuseColor * lightIntensity);
            color = clamp(color, 0.0, 1.0);
            
            // Native Reflex like in DX11 level 34
            vec3 reflection = normalize(2 * lightIntensity * inNormal + ps.lightDirection.xyz);
            float fPhoneValue = clamp(dot(reflection, inViewDir), 0.0, 1.0);
            vec4 specular = vec4(pow(fPhoneValue, ps.nShininess));

            color = color * textureColor;
            textureColor = clamp(textureColor + specular, 0.0, 1.0);
        }
    }
#endif

	// 7. Add alpha color
    if (pc.hasAlfaColor > 0)
        textureColor.a = pc.alfaColor;
	
    // 8. Global Fase Effect (LEVEL 29)
    textureColor.rgb *= ps.fade;
	

	//normal:
	outColor = textureColor;
}
