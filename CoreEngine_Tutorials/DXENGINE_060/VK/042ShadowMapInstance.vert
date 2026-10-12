#version 450

// --------------------------------------------------------------------------------------------
// Filename: 041ShadowMapInstance.vert
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan SHADOW shader - LEVEL 41
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

// ------------------------------------------------------------------
// Vertex Input
// ------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inInstancePosition;

// --------------------------------------------------------------------------------------------
// VERTEX OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out float outDepth;

// ------------------------------------------------------------------
// VS CONSTANT BUFFER
// ------------------------------------------------------------------
layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
{
    mat4  worldMatrix;
    mat4  view;
    mat4  projection;
    mat4  WVP;

    float VSrotY;
    float padding0;
    float padding1;
    float padding2;
} vs;

// ------------------------------------------------------------------
void main()
{
	vec4 position = vec4(inPosition, 1.0);

    // --------------------------------------------------------------------
    // [1st] ROTATE the instance
    // --------------------------------------------------------------------
    mat4 rotationAroundY = mat4(
        vec4( cos(vs.VSrotY), 0.0, -sin(vs.VSrotY), 0.0),
        vec4( 0.0,            1.0,  0.0,            0.0),
        vec4( sin(vs.VSrotY), 0.0,  cos(vs.VSrotY), 0.0),
        vec4( 0.0,            0.0,  0.0,            1.0)
    );
    position = rotationAroundY * position;

    // --------------------------------------------------------------------
    // [2nd] TRANSLATE the instance
    // --------------------------------------------------------------------
    position.x += inInstancePosition.x * (1.0 / vs.worldMatrix[0][0]);
    position.y += inInstancePosition.y * (1.0 / vs.worldMatrix[1][1]);
    position.z += inInstancePosition.z * (1.0 / vs.worldMatrix[2][2]);

	// Same result as world → view → projection
	gl_Position = vs.WVP * position;

    // Vulkan clip-space
    gl_Position.y = -gl_Position.y;
	
	outDepth = gl_Position.z / gl_Position.w;
}