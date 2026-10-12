#version 450

// --------------------------------------------------------------------------------------------
// Filename: bump.vert
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan BUMP shader - LEVEL 35
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// VERTEX INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent; // LEVEL 35: Tangent attribute

// --------------------------------------------------------------------------------------------
// VERTEX OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outViewDir;
layout(location = 3) out vec3 outTangent; // LEVEL 35: Passed to Fragment

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
	float	VSfogStart;
	float	VSfogEnd;
	int		VShasShadowMap;
	int		VS_USE_WVP;

	// 36 BLOCK: VS5
	mat4	ViewToLightProj;
	mat4	WorldInverseTranspose;
	vec4	VSCameraPosition;
} vs;

void main()
{
 	if (vs.VS_USE_WVP > 0)
		gl_Position = vs.WVP * vec4(inPosition, 1.0);
	else
         gl_Position = vs.projection * vs.view * vs.worldMatrix * vec4(inPosition, 1.0);
	
	// Vulkan clip-space coordinate inversion
	gl_Position.y = -gl_Position.y;	
	
    outTexCoords = inTexCoords;

    if (vs.VShasLight != 0 || vs.VShasSpecular != 0)
        outNormal = normalize(mat3(vs.worldMatrix) * inNormal);
    else
        outNormal = inNormal;
	
	// LEVEL 35:
#if defined PS_USE_BUMP
    if (vs.VShasNormMap > 0)
        outTangent = normalize(mat3(vs.worldMatrix) * inTangent);
    else
		outTangent = vec3(0.0); // Fallback
#endif

    // LEVEL 34:
    vec3 worldPos = vec3(vs.worldMatrix * vec4(inPosition, 1.0));
    outViewDir = normalize(vs.VSCameraPosition.xyz - worldPos);
}
