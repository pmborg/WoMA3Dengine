#version 450

// --------------------------------------------------------------------------------------------
// Filename: lightshadow.vert
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan LIGHTSHADOW shader - LEVEL 36
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// VERTEX INPUT
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
layout(location = 3) out vec4 outLightViewPosition; // LIGHT_VIEW_POSITION (Linha 42)

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
    //if (vs.VS_USE_WVP > 0)
		gl_Position = vs.WVP * vec4(inPosition, 1.0);
	//else
    //   gl_Position = vs.projection * vs.view * vs.worldMatrix * vec4(inPosition, 1.0);
	
	gl_Position.y = -gl_Position.y;	
    outTexCoords = inTexCoords;

	//vec4 cameraPosition = vs.WV * vec4(inPosition, 1.0);

    if (vs.VShasLight != 0 || vs.VShasSpecular != 0)
        outNormal = normalize(mat3(vs.worldMatrix) * inNormal);
    else
        outNormal = inNormal;
	
	if (vs.VShasSpecular > 0)
	{
		vec3 worldPos = vec3(vs.worldMatrix * vec4(inPosition, 1.0));
		outViewDir = normalize(vs.VSCameraPosition.xyz - worldPos);
	}
	else
	{
		outViewDir = vec3(0.0);
	}

	//NORMAL:
	// LEVEL 36 SHADOWS
	//if (vs.VShasShadowMap > 0)
		outLightViewPosition = vs.ViewToLightProj * vs.worldMatrix * vec4(inPosition, 1.0);
	//else
	//	outLightViewPosition = vec4(0.0);



//vec4 p = vs.ViewToLightProj * vs.worldMatrix * vec4(inPosition, 1.0);
//outLightViewPosition = p;
//
//// DEBUG
//gl_Position = p;
//gl_Position.y = -gl_Position.y;
}
