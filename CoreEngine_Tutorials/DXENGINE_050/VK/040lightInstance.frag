#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: lightInstance.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan INSTANCED LIGHT shader - LEVEL 40
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

#define PS_USE_SPECULAR

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inViewDir;

layout(location = 0) out vec4 outColor;

// --------------------------------------------------------------------
// Pixel constant buffer
// --------------------------------------------------------------------
layout(binding = 2) uniform PSShaderParametersBuffer
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 emissiveColor;
    vec4 lightDirection;

    int   pad1;
    float pad2;
    float fade;
    float frameTime;

    vec3 cameraPosition;
    int  castShadow;

    vec3 specularColor;
    float nShininess;
} ps;

// --------------------------------------------------------------------
// Push constants
// --------------------------------------------------------------------
layout(push_constant) uniform PushConstants
{
    vec4 pixelColor;

    int hasTexture;
    int hasLight;
    int hasSpecular;
    int isFont;

    int hasColorMap;
    float lightType;
    float isDay;
    float shaderTypeParameter;

    int hasAlfaColor;
    float alfaColor;
    float pad1;
    float pad2;

    int hasFog;
    int isSky;
    int hasAlfaMap;
    int hasNormMap;
} pc;

// --------------------------------------------------------------------
// Textures
// --------------------------------------------------------------------
layout(binding = 1) uniform sampler2D shaderTexture;  // albedo
layout(binding = 3) uniform sampler2D alfaMapTexture; // alpha map (optional)

#include "TextureSampling.glsl"

vec4 GetShaderTextureSimple(sampler2D tex, vec2 uv, float mipLevel, int mode)
{
    return texture(tex, uv);
}

float PSlightFunc1(vec3 normal)
{
    return max(dot(normal,-ps.lightDirection.xyz),0.0);
}

float PSlightFunc2(vec3 normal)
{
    return max(dot(normal, ps.lightDirection.xyz),0.0);
}

void main()
{
    vec4 textureColor = pc.pixelColor;
    float lightIntensity = 0.0;
    int mode = 0;

    // --------------------------------------------------------------------
    // Base texture
    // --------------------------------------------------------------------
    if (pc.hasTexture > 0)
        textureColor = GetShaderTextureSimple(shaderTexture,inTexCoords,0,mode);

    vec3 normal = normalize(inNormal);

    // --------------------------------------------------------------------
    // Lighting (LEVEL 23)
    // --------------------------------------------------------------------
    if (pc.hasLight > 0)
    {
        if (pc.lightType == 1.0)
            lightIntensity = PSlightFunc2(normal);
        else
            lightIntensity = PSlightFunc1(normal);

        if (pc.hasTexture > 0)
            textureColor *= clamp(ps.emissiveColor + ps.ambientColor + lightIntensity,0.0,1.0);
        else
            textureColor *= clamp(ps.emissiveColor + ps.ambientColor + (lightIntensity * ps.diffuseColor),0.0,1.0);
    }

    // --------------------------------------------------------------------
    // Specular (LEVEL 34)
    // --------------------------------------------------------------------
#if defined PS_USE_SPECULAR
    if (pc.hasSpecular > 0 && lightIntensity > 0.0)
    {
        vec4 color = clamp(ps.ambientColor + ps.diffuseColor * lightIntensity,0.0,1.0);

        vec3 reflection = normalize(2.0 * lightIntensity * normal + ps.lightDirection.xyz);
        float fPhong = max(dot(reflection,inViewDir),0.0);

        vec4 specular = vec4(pow(fPhong,ps.nShininess));

        color *= textureColor;
        textureColor = clamp(textureColor + specular,0.0,1.0);
    }
#endif

    // --------------------------------------------------------------------
    // Alpha
    // --------------------------------------------------------------------
    if (pc.hasAlfaMap > 0)
        textureColor.a = texture(alfaMapTexture,inTexCoords).r;

    if (pc.hasAlfaColor > 0)
        textureColor.a = pc.alfaColor;

    textureColor.rgb *= ps.fade;

    outColor = textureColor;
}