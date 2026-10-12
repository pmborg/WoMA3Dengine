#version 450

// --------------------------------------------------------------------------------------------
// Filename: font.vert
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan FONT shader
// Level 27 - FONT
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 0) out vec2 fragTexCoord;

// Descriptor set 0, binding 0: VS constant buffer.
// Matches the Level 26..35 vkShaderClass layout used by Level 27.
layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
{
	//VS1:
    mat4 worldMatrix;
    mat4 view;
    mat4 projection;
    mat4 WV;
    mat4 WVP;

	//VS2:
    int  VShasLight;
    int  VShasSpecular;
    int  VShasNormMap;
    int  VShasFog;

	//VS3:
    vec4 VSlightDirection;
    vec4 VSambientColor;
    vec4 VSdiffuseColor;
    vec4 VSemissiveColor;

	//VS4:
    float VSfogStart;
    float VSfogEnd;
    int   VShasShadowMap;
    int   VS_USE_WVP;
	
	// 36 BLOCK: VS5
	mat4	ViewToLightProj;
	mat4	WorldInverseTranspose;
	vec4	VSCameraPosition;
} vs;

void main()
{
    //gl_Position = vsParams.WVP * vec4(inPosition, 1.0);
	vec4 position = vec4(inPosition, 1.0);
	
	if (vs.VS_USE_WVP != 0)
	{
		gl_Position = vs.WVP * position;
	}
	else
	{
		position = vs.worldMatrix * position;
		position = vs.view * position;
		position = vs.projection * position;
		gl_Position = position;
	}
	
	// Vulkan clip-space correction
	gl_Position.y = -gl_Position.y;
    fragTexCoord = inTexCoord;
}
