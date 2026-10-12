#version 450

// --------------------------------------------------------------------------------------------
// Filename: light34.vert
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan LIGHT shader - LEVEL 34
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// FRAGMENT INPUT FROM VERTEX
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;

// --------------------------------------------------------------------------------------------
// VERTEX OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outViewDir;

// --------------------------------------------------------------------------------------------
// ABI must match vkShaderClass::vkVSconstantBufferType exactly.
// Do not reorder fields.
// --------------------------------------------------------------------------------------------
// VS CONSTANT BUFFER
// --------------------------------------------------------------------------------------------
layout(binding = 0) uniform VSShaderParametersBuffer
{
    // VS1: 80 bytes
    mat4 worldMatrix;
    mat4 view;
    mat4 projection;
    mat4 WV;
    mat4 WVP;

    // VS2: 16 bytes
    // HLSL bools occupy 4 bytes in a cbuffer.
    int VShasLight;
    int VShasSpecular;
    int VShasNormMap;
    int VShasFog;

    // VS3: 64 bytes
    vec4 VSlightDirection;
    vec4 VSambientColor;
    vec4 VSdiffuseColor;
    vec4 VSemissiveColor;

	// VS4: 16 bytes
    float VSfogStart;
    float VSfogEnd;
    int   VShasShadowMap;
    int   VS_USE_WVP;

	// 36 BLOCK: VS5
	mat4	ViewToLightProj;
	mat4	WorldInverseTranspose;
	vec4	VSCameraPosition;
} vs;

// --------------------------------------------------------------------------------------------
// MAIN
// --------------------------------------------------------------------------------------------
void main()
{
 	if (vs.VS_USE_WVP>0)
	{
		gl_Position = vs.WVP * vec4(inPosition, 1.0);
	}
	else
	{
         gl_Position = vs.projection * vs.view * vs.worldMatrix * vec4(inPosition, 1.0);
	}
	
	// Vulkan clip-space
	gl_Position.y = -gl_Position.y;
	
    // Texture coordinates (fallback)
    outTexCoords = inTexCoords;

   if (vs.VShasLight != 0 || vs.VShasSpecular != 0)
        outNormal = normalize(mat3(vs.worldMatrix) * inNormal);
    else
        outNormal = inNormal;
	
    // LEVEL 34: Calcular a posição do vértice no espaço do mundo
    vec3 worldPos = vec3(vs.worldMatrix * vec4(inPosition, 1.0));

	//vec4 cameraPosition = vs.WV * vec4(inPosition, 1.0);
	//outViewDir = normalize(cameraPosition.xyz - worldPos.xyz);

    outViewDir = normalize(vs.VSCameraPosition.xyz - worldPos.xyz);
}
