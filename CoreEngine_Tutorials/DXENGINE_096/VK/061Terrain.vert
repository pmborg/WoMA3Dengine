#version 450

// --------------------------------------------------------------------------------------------
// Filename: 061Terrain.vert
// World of Middle Age (WoMA) - Vulkan
// LEVEL 61: TERRAIN - Multi-layer (512x512)
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT FROM VERTEX
// --------------------------------------------------------------------------------------------
layout(location=0) in vec3 inPosition;
layout(location=1) in vec4 inTex;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec3 inColor;
layout(location=4) in vec4 inTexMapping;
layout(location=5) in vec3 inTangent;
layout(location=6) in vec3 inBinormal;

// --------------------------------------------------------------------------------------------
// FRAGMENT OUTPUT
// --------------------------------------------------------------------------------------------
layout(location=0) out vec4 outTex;
layout(location=1) out vec3 outNormal;

layout(location=2) out vec4 outColor;
layout(location=3) out vec4 outTexMapping;

layout(location=4) out float outFogFactor;
layout(location=5) out vec4 outDepthPosition;

layout(location=6) out vec4 outverticePosition;
layout(location=7) out vec3 outTangent;
layout(location=8) out vec3 outBinormal;

// --------------------------------------------------------------------------------------------
// ABI must match vkShaderClass::vkVSconstantBufferType exactly.
// Do not reorder fields.
// --------------------------------------------------------------------------------------------
// VS CONSTANT BUFFER
// --------------------------------------------------------------------------------------------
layout(binding = 0, std140) uniform VSShaderParametersBuffer
{
    // VS1: Matrizes (Alinhamento nativo de 64 bytes cada)
    mat4 worldMatrix;
    mat4 viewMatrix;
    mat4 projection;
    mat4 WV;
    mat4 WVP;

    // VS2: Flags Inteiras
    int VShasLight;
    int VShasSpecular;
    int VShasNormMap;
    int VShasFog;

    // VS3: Vetores de Iluminação
    vec4 VSlightDirection;
    vec4 VSambientColor;
    vec4 VSdiffuseColor;
    vec4 VSemissiveColor;

    // VS4: Configurações de Fog e Shadow
    float VSfogStart;
    float VSfogEnd;
    int   VShasShadowMap;
    int   VS_USE_WVP;		//ignored on this level

    // VS5: Projeções Avançadas e Câmara
    mat4  ViewToLightProj;
    mat4  WorldInverseTranspose;
    vec3  VSCameraPosition;
    int   Pad1;

    // VS6: Rotações e Variável do Tempo (Animação da Água)
    float VSrotX;
    float VSrotY;
    float VSrotZ;
    float time;   // FIX: Trancado estritamente no offset 572

    // VS7: Tipos de Shaders e Preenchimento (PAD)
    float VSshaderType;
    int   vsIsSky;
    float vsPAD3;
    float vsPAD4;
} vs;

void main()
{
	vec3 position = inPosition;

	if (position.y < 0.0)
		position.y *= 3.0;
	
    //POSITION:
    gl_Position = vs.WVP * vec4(position,1.0);
	gl_Position.y = -gl_Position.y;	// Vulkan flip:

    //TEXTURE:0
    outTex = inTex;

    //NORMAL:1
    outNormal = normalize(mat3(vs.worldMatrix) * inNormal);
	outColor = vec4(inColor,1);

	outDepthPosition = gl_Position;

	outverticePosition = vec4(position,1.0);

	vec4 cameraPosition = vs.WV * vec4(position,1.0);
	outFogFactor = (vs.VSfogEnd - (cameraPosition.z)) / (vs.VSfogEnd - vs.VSfogStart);
    
	outTexMapping = inTexMapping;

	outTangent=normalize(mat3(vs.worldMatrix)*inTangent);
	outBinormal=normalize(mat3(vs.worldMatrix)*inBinormal);
}