#version 450

// --------------------------------------------------------------------------------------------
// Filename: 053Double_Color_Terrain.vert
// WoMA Engine 3D - Vulkan LEVEL 53
// Equivalent to DX: 053Double_Color_Terrain.hlsl
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// VERTEX INPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec4 inColor;

// --------------------------------------------------------------------------------------------
// VERTEX OUTPUT
// --------------------------------------------------------------------------------------------
layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outViewDir;
layout(location = 3) out vec4 outColor;

// --------------------------------------------------------------------------------------------
// VS CONSTANT BUFFER
// ABI must match vkVSconstantBufferType exactly.
// --------------------------------------------------------------------------------------------
layout(binding=0) uniform VSShaderParametersBuffer
{
    // VS1
    mat4 worldMatrix;
    mat4 view;
    mat4 projection;
    mat4 WV;
    mat4 WVP;

    // VS2
    int VShasLight;
    int VShasSpecular;
    int VShasNormMap;
    int VShasFog;

    // VS3
    vec4 VSlightDirection;
    vec4 VSambientColor;
    vec4 VSdiffuseColor;
    vec4 VSemissiveColor;

    // VS4
    float VSfogStart;
    float VSfogEnd;
    int VShasShadowMap;
    int VS_USE_WVP;

    // VS5
    mat4 ViewToLightProj;
    mat4 WorldInverseTranspose;
    vec4 VSCameraPosition;

    // VS6
    float VSrotX;
    float VSrotY;
    float VSrotZ;
    float time;

    // VS7
    float VSshaderType;
    int vsIsSky;
    float vsPAD3;
    float vsPAD4;
} vs;

void main()
{
    if (vs.VS_USE_WVP > 0)
        gl_Position = vs.WVP * vec4(inPosition,1.0);
    else
        gl_Position = vs.projection * vs.view * vs.worldMatrix * vec4(inPosition,1.0);

    gl_Position.y = -gl_Position.y;

    outTexCoords = inTexCoords;
    outColor     = inColor;

    if (vs.VShasLight != 0 || vs.VShasSpecular != 0)
        outNormal = normalize(mat3(vs.worldMatrix) * inNormal);
    else
        outNormal = inNormal;

    vec3 worldPos = vec3(vs.worldMatrix * vec4(inPosition,1.0));
    outViewDir = normalize(vs.VSCameraPosition.xyz - worldPos);
}