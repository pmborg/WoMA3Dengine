#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform UBO
{
    //mat4 worldMatrix;
    //mat4 view;
    //mat4 projection;
    //mat4 WV;
    mat4 WVP;
} ubo;

void main()
{
    gl_Position = ubo.WVP * vec4(inPosition, 1.0);
	gl_Position.y = -gl_Position.y;
    fragTexCoord = inTexCoord;
}