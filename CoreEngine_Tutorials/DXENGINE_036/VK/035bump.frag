#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: bump.frag
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan BUMP shader - LEVEL 35
// Vulkan Fragment Shader
// --------------------------------------------------------------------------------------------

#define PS_USE_BUMP

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inViewDir;
layout(location = 3) in vec3 inTangent; // LEVEL 35

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
    int hasFog;
    int isSky;
    int hasAlfaMap;
    int hasNormMap;
} pc;

// --------------------------------------------------------------------------------------------
// SAMPLERS / TEXTURES
// --------------------------------------------------------------------------------------------
layout(binding = 1) uniform sampler2D shaderTexture;    // albedo
layout(binding = 3) uniform sampler2D alfaMapTexture;   // alpha map (optional)
layout(binding = 4) uniform sampler2D normalMapTexture; // LEVEL 35: Normal Map

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
float PSlightFunc1(vec3 normal) { return clamp(dot(normal, -ps.lightDirection.xyz), 0.0, 1.0); }

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
float PSlightFunc2(vec3 normal) { return clamp(dot(normal, ps.lightDirection.xyz), 0.0, 1.0); }

#include "TextureSampling.glsl"

vec4 GetShaderTextureSimple(sampler2D tex, vec2 uv, float mipLevel, int mode)
{
    switch (mode)
    {
        case 3: return CubicInterpolation(tex, uv);
        default: return texture(tex, uv);
    }
}

// --------------------------------------------------------------------------------------------
// MAIN
// --------------------------------------------------------------------------------------------
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

	// 2. === BUMP / NORMAL MAPPING (LEVEL 35) ===
	vec3 normal = normalize(inNormal);

	// 3 LIGHT: Level 23 Difuse and Ambient Light
	if (pc.hasNormMap > 0)
	{
		// Amostra a textura de normais e converte de [0,1] para [-1,1]
		vec3 nmap = texture(normalMapTexture, inTexCoords).xyz * 2.0 - 1.0;
        
        // Correção de normal estilo OpenGL em motor DirectX (Eixo Y invertido)
        nmap.g = -nmap.g;
        
		float normalStrength = 1.0; 
		nmap.xy *= normalStrength;
		nmap = normalize(nmap);

		// Garante ortogonalidade entre Tangente e Normal (Gram-Schmidt)
		vec3 T = normalize(inTangent - dot(inTangent, normal) * normal);

		// Calcula a Bitangente
		vec3 B = normalize(cross(normal, T));

		// Cria a matriz TBN (colunas T, B, N)
		mat3 TBN = mat3(T, B, normal);

		// Transforma a normal do espaço da textura (Tangent Space) para o Espaço do Mundo
		normal = normalize(TBN * nmap);
	}
	
    // 4. NEW BUMPED (LEVEL 23+35)
	if (pc.hasLight > 0)
	{
        if (pc.isSky == 0)
            lightIntensity = PSlightFunc1(normal);
        else
            lightIntensity = PSlightFunc2(normal);
	
		vec4 lighting;
		if (pc.hasTexture > 0)
			lighting = ps.emissiveColor + ps.ambientColor + lightIntensity;
		else
			lighting = ps.emissiveColor + ps.ambientColor + (lightIntensity * ps.diffuseColor);

		textureColor *= clamp(lighting, 0.0, 1.0);
	}
	
	// 5. Alfa Color (LEVEL 33)
    if (pc.hasAlfaColor > 0)
        textureColor.a = pc.alfaColor;
	
	// 6. Alfa Mapping (LEVEL 34)
    if (pc.hasAlfaMap > 0) {
        textureColor.a = texture(alfaMapTexture, inTexCoords).r;
    }
	
    // 7. SPECULAR LIGHT (LEVEL 34) + NORMAL BUMPED (LEVEL 35)
    if (pc.hasSpecular > 0) {
        if (lightIntensity > 0.0) {
			vec4 color = ps.ambientColor;
			color += (ps.diffuseColor * lightIntensity);
			color = clamp(color, 0.0, 1.0);

			vec3 reflection = normalize(reflect(normalize(ps.lightDirection.xyz), normal));
			float fPhongValue = max(dot(reflection, inViewDir), 0.0);
            
			vec4 specular = vec4(pow(fPhongValue, ps.nShininess));

			color = color * textureColor;
			textureColor = clamp(textureColor + specular, 0.0, 1.0);
        }
    }
	
    // 8. Global Fase Effect (LEVEL 29)
    textureColor.rgb *= ps.fade;
    
    outColor = textureColor;
}
