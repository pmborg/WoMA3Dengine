#version 450

// --------------------------------------------------------------------------------------------
// Filename: 060Terrain.frag
// World of Middle Age (WoMA) - Vulkan
// LEVEL 60: TERRAIN - Multi-layer (256x256)
// --------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// INPUT
// -----------------------------------------------------------------------------
layout(location=0) in vec4 inTex;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec4 inColor;
layout(location=3) in vec4 inTexMapping;
layout(location=4) in float inFogFactor;
layout(location=5) in vec4 inDepthPosition;
layout(location=6) in vec4 inInputPosition;
layout(location=7) in vec3 inTangent;
layout(location=8) in vec3 inBinormal;

// -----------------------------------------------------------------------------
// OUTPUT
// -----------------------------------------------------------------------------
layout(location=0) out vec4 outColor;

// -----------------------------------------------------------------------------
// TEXTURES (bindings iguais ao DX11)
// -----------------------------------------------------------------------------
layout(binding=1)  uniform sampler2D grassTexture;
layout(binding=3)  uniform sampler2D slopeTexture;
layout(binding=4)  uniform sampler2D rockTexture;
layout(binding=5)  uniform sampler2D detailTexture;
layout(binding=6)  uniform sampler2D textureMappingTexture;
layout(binding=7)  uniform sampler2D sidewalkTexture;
layout(binding=8)  uniform sampler2D sandTexture;
layout(binding=9)  uniform sampler2D mudTexture;
layout(binding=10) uniform sampler2D alphaMappingTexture;
layout(binding=11) uniform sampler2D normalTexture;
layout(binding=12) uniform sampler2D shaderTexture;


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

// -----------------------------------------------------------------------------
// Helper
// -----------------------------------------------------------------------------
float HeightBlend(float h, float center, float width)
{
    return smoothstep(center - width, center + width, h);
}

// -----------------------------------------------------------------------------
// CH21 - Normal Mapping
// -----------------------------------------------------------------------------
vec3 NormalMapFunc(float depthValue)
{
    if(depthValue >= 0.95)
        return normalize(inNormal);

    vec4 bump = texture(normalTexture, inTex.zw);

    bump = bump * 2.0 - 1.0;

    vec3 n =
        inNormal +
        bump.x * inTangent +
        bump.y * inBinormal;

    return normalize(n);
}

// -----------------------------------------------------------------------------
// PIXEL SHADER
// -----------------------------------------------------------------------------
void main()
{
    //if(inFogFactor <= 0.0)
    //    discard;

    float depthValue = inDepthPosition.z / inDepthPosition.w;

    // CH17
    vec4 grassColor = texture(grassTexture, inTex.xy);

    // CH18
    vec4 detailColor = texture(detailTexture, inTex.zw);
    grassColor *= detailColor * 0.85;
    grassColor.a = 1.0;

    // CH19
    vec4 mappingColor = texture(textureMappingTexture, inTexMapping.xy);

    // CH20
    vec4 alphaValue = texture(alphaMappingTexture, inTexMapping.xy);

    vec4 textureColor;

    // -------------------------------------------------------------------------
    // Special painted terrain
    // -------------------------------------------------------------------------
    if(mappingColor.g >= 0.1 && mappingColor.b >= 0.25)
    {
        discard;
    }
    else
    if(mappingColor.g >= 0.1)
    {
        textureColor = texture(sidewalkTexture, inTex.zw);
        textureColor = mix(grassColor, textureColor, alphaValue.r);
    }
    else
    if(mappingColor.b >= 0.1)
    {
        textureColor = texture(sandTexture, inTex.zw) * 0.75;
	
        if(mappingColor.b < 0.65)
            textureColor = mix(grassColor, textureColor, alphaValue.r);
    }
    else
    if(mappingColor.r >= 0.1)
    {
        textureColor = texture(mudTexture, inTex.zw) * 0.75;
        textureColor = mix(grassColor, textureColor, alphaValue.r);
    }
    else
    {
        vec4 slopeColor = texture(slopeTexture, inTex.xy);
        vec4 rockColor  = texture(rockTexture,  inTex.xy);

        float slope = 1.0 - inNormal.y;

        if(slope < 0.2)
        {
            textureColor = mix(grassColor, slopeColor, slope / 0.2);
        }
        else
        if(slope < 0.7)
        {
            textureColor = mix(slopeColor, rockColor, (slope-0.2)/(0.7-0.2));
        }
        else
        {
            textureColor = rockColor;
        }
    }

    // -------------------------------------------------------------------------
    // CH40 underwater fog
    // -------------------------------------------------------------------------
    vec4 fogColor = vec4(0.5,0.5,0.5,1.0);

    if(inInputPosition.y <= 0.0)
        fogColor = vec4(0.0,0.0,0.2,1.0);

    // -------------------------------------------------------------------------
    // Lighting
    // -------------------------------------------------------------------------
    float lightIntensity;

    if(mappingColor.b >= 0.1)
        lightIntensity = clamp(dot(NormalMapFunc(depthValue),-ps.lightDirection.xyz),0.0,1.0);
    else
        lightIntensity = clamp(dot(normalize(inNormal),-ps.lightDirection.xyz),0.0,1.0);

    vec4 color = ps.ambientColor +
                 ps.diffuseColor * lightIntensity;

    textureColor.a = 1.0;

    color *= textureColor;

    // -------------------------------------------------------------------------
    // CH23 Light Map
    // -------------------------------------------------------------------------
    if(inInputPosition.y > 0.0)
        color *= texture(shaderTexture,inTexMapping.xy);

    // -------------------------------------------------------------------------
    // Fog
    // -------------------------------------------------------------------------
    color = mix(fogColor,color,inFogFactor);

    // -------------------------------------------------------------------------
    // Global Fade
    // -------------------------------------------------------------------------
    color.rgb *= ps.fade;

    outColor = color;
}