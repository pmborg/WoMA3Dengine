#version 450

// --------------------------------------------------------------------------------------------
// Filename: 061Terrain.frag
// World of Middle Age (WoMA) - Vulkan
// LEVEL 61: TERRAIN - Multi-layer (512x512)
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
// TEXTURES (bindings like DX11)
// -----------------------------------------------------------------------------
//0 PS Buffer
layout(binding=1)  uniform sampler2D grassTexture;					//reg0
//2 VS Buffer														
layout(binding=3)  uniform sampler2D slopeTexture;					//reg1
layout(binding=4)  uniform sampler2D rockTexture;					//reg2

layout(binding=5)  uniform sampler2D detailTexture;					//reg3
layout(binding=6)  uniform sampler2D textureMappingTexture;			//reg4

layout(binding=7)  uniform sampler2D stonePathTexture;				//reg5

layout(binding=8)  uniform sampler2D sandTexture;					//reg6
layout(binding=9)  uniform sampler2D mudTexture;					//reg7
layout(binding=10) uniform sampler2D detailMappingTexture;			//reg8
layout(binding=11) uniform sampler2D normalTexture;					//reg9
																	
layout(binding=12) uniform sampler2D generalNormalTexture;			//reg10
layout(binding=13) uniform sampler2D bigPathMappingTexture;			//reg11
layout(binding=14) uniform sampler2D smallStonePathTexture;			//reg12
layout(binding=15) uniform sampler2D lightMapTexture;				//reg13


// --------------------------------------------------------------------------------------------
// ABI must match vkShaderClass::vkPSconstantBufferType exactly.
// Do not reorder fields.
// --------------------------------------------------------------------------------------------
// PS CONSTANT BUFFER
// --------------------------------------------------------------------------------------------
layout(binding=2, std140) uniform PSShaderParametersBuffer
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

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 1
// --------------------------------------------------------------------------------------------
float PSlightFunc1(vec3 normal) { return clamp(dot(normal, -ps.lightDirection.xyz), 0.0, 1.0); }

// --------------------------------------------------------------------------------------------
// LIGHT FUNCTION 2
// --------------------------------------------------------------------------------------------
float PSlightFunc2(vec3 normal) { return clamp(dot(normal, ps.lightDirection.xyz), 0.0, 1.0); }

// -----------------------------------------------------------------------------
// PIXEL SHADER
// -----------------------------------------------------------------------------
void main()
{
    vec4 fogColor = vec4(87.0/256.0,87.0/256.0,87.0/256.0,1.0);
    float lightIntensity = PSlightFunc2(inNormal);

    // ------------------------------------------------------------------------
    // Mapping textures
    // ------------------------------------------------------------------------
    vec4 mappingColor = texture(textureMappingTexture,inTexMapping.xy);
    vec4 alphaValue   = texture(detailMappingTexture,inTexMapping.xy);

    // ------------------------------------------------------------------------
    // Base terrain textures
    // ------------------------------------------------------------------------
    vec4 grassColor  = texture(grassTexture,inTex.xy);
    vec4 slopeColor  = texture(slopeTexture,inTex.xy);
    vec4 rockColor   = texture(rockTexture,inTex.xy);

    vec4 detailColor = texture(detailTexture,inTex.xy);
    grassColor = mix(grassColor,detailColor, (mappingColor.r+mappingColor.g+mappingColor.b)/3.0);

    // ------------------------------------------------------------------------
    // Large path overlay
    // ------------------------------------------------------------------------
    vec4 bigPath = texture(bigPathMappingTexture,inTexMapping.xy);
    grassColor = mix(grassColor, texture(smallStonePathTexture,inTex.xy), bigPath.g);

    // ------------------------------------------------------------------------
    // Terrain slope blend
    // ------------------------------------------------------------------------
    float slope = 1.0-inNormal.y;

    vec4 textureColor;

    if(slope<0.2)
    {
        textureColor=mix(grassColor,slopeColor,slope/0.2);
    }
    else
    if(slope<0.7)
    {
        textureColor=mix(slopeColor,rockColor, (slope-0.2)/(0.7-0.2));
    }
    else
    {
        textureColor=rockColor;
    }

	if( inDepthPosition.z  < 128)
	{
		// ------------------------------------------------------------------------
		// Color-map overlays
		// ------------------------------------------------------------------------
		// GREEN: sidewalk:
		textureColor=mix(textureColor, texture(stonePathTexture,inTex.zw), mappingColor.g/3.0);

		// BLUE: Sand:
		textureColor=mix(textureColor, texture(sandTexture,inTex.zw), mappingColor.b/3.0);
					 
		//RED: Mud:
		textureColor=mix(textureColor, texture(mudTexture,inTex.zw), mappingColor.r/3.0);

		// ------------------------------------------------------------------------
		// Global bump lighting
		// ------------------------------------------------------------------------
		vec4 generalBump=texture(generalNormalTexture,inTex.zw);
		textureColor+=ps.diffuseColor/3.0*lightIntensity;

		// ------------------------------------------------------------------------
		// Cyan transparency
		// ------------------------------------------------------------------------
		if(alphaValue.g>=0.1 && alphaValue.b>=0.25)
			discard;

		// ------------------------------------------------------------------------
		// Flat terrain detail
		// ------------------------------------------------------------------------
		if(slope<0.2)
		{
			vec4 bumpMap=texture(normalTexture,inTex.xy);

			// Stone:
			textureColor=mix(textureColor, texture(stonePathTexture,inTex.xy), alphaValue.g*2.5);
			// Sand:
			textureColor=mix(textureColor, texture(sandTexture,inTex.xy), alphaValue.b);

			// Mud:
			textureColor=mix(textureColor, texture(mudTexture,inTex.xy), alphaValue.r/1.5);
		}
	}

    // ------------------------------------------------------------------------
    // Underwater darkening
    // ------------------------------------------------------------------------
    if(inInputPosition.y<0.0)
        textureColor.rgb-=0.2;

	// ------------------------------------------------------------------------
	// Fade
	// ------------------------------------------------------------------------
	if(ps.fade<1.0)
	{
		textureColor.rgb*=ps.fade*2.0;
		textureColor*=texture(lightMapTexture,inTexMapping.xy);
	}

    // ------------------------------------------------------------------------
    // Mini MAP Water Rendering.
    // ------------------------------------------------------------------------
    if(pc.shaderTypeParameter>0.0)
    {
        //textureColor*=1.5;
        textureColor.a=1.0;
		outColor=textureColor;
		return;
    }

    // ------------------------------------------------------------------------
    // Fog
    // ------------------------------------------------------------------------
    if(inFogFactor >= 0.0)
    {
        textureColor.rgb=mix(textureColor.rgb, fogColor.rgb, vec3(1.0-inFogFactor));
        textureColor.a=1.0;
    }

	outColor=textureColor;
}
