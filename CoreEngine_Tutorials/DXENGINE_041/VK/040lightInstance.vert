#version 450
#extension GL_GOOGLE_include_directive : enable

// --------------------------------------------------------------------------------------------
// Filename: lightInstance.vert
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - Vulkan INSTANCED LIGHT shader - LEVEL 40
// Vulkan Vertex Shader
// --------------------------------------------------------------------------------------------

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inInstancePosition;   // LEVEL 40

layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outViewDir;

layout(binding = 0) uniform VSShaderParametersBuffer
{
    mat4 worldMatrix;
    mat4 view;
    mat4 projection;
    mat4 WV;
    mat4 WVP;

    int VShasLight;
    int VShasSpecular;
    int VShasNormMap;
    int VShasFog;

    vec4 VSlightDirection;
    vec4 VSambientColor;
    vec4 VSdiffuseColor;
    vec4 VSemissiveColor;

    float VSfogStart;
    float VSfogEnd;
    int VShasShadowMap;
    int VS_USE_WVP;

    mat4 ViewToLightProj;
    mat4 WorldInverseTranspose;
    vec4 VSCameraPosition;
} vs;

void main()
{
    // --------------------------------------------------------------------
    // LEVEL 40: Rotate instance around Y
    // --------------------------------------------------------------------
    float c = cos(vs.VSlightDirection.w);     // VSrotY
    float s = sin(vs.VSlightDirection.w);

    mat4 rotationAroundY = mat4(
         c, 0.0,  s, 0.0,
        0.0,1.0,0.0,0.0,
        -s,0.0,  c, 0.0,
        0.0,0.0,0.0,1.0);

    vec4 position = rotationAroundY * vec4(inPosition,1.0);

    // --------------------------------------------------------------------
    // LEVEL 40: Translate instance
    // --------------------------------------------------------------------
    position.x += inInstancePosition.x / vs.worldMatrix[0][0];
    position.y += inInstancePosition.y / vs.worldMatrix[1][1];
    position.z += inInstancePosition.z / vs.worldMatrix[2][2];

    // --------------------------------------------------------------------
    // Projection
    // --------------------------------------------------------------------
    if (vs.VS_USE_WVP > 0)
        gl_Position = vs.WVP * position;
    else
        gl_Position = vs.projection * vs.view * vs.worldMatrix * position;

    // Vulkan clip-space
    gl_Position.y = -gl_Position.y;

    outTexCoords = inTexCoords;

    if (vs.VShasLight != 0 || vs.VShasSpecular != 0)
        outNormal = normalize(mat3(vs.worldMatrix) * inNormal);
    else
        outNormal = inNormal;

    vec3 worldPos = vec3(vs.worldMatrix * position);
    outViewDir = normalize(vs.VSCameraPosition.xyz - worldPos);
}