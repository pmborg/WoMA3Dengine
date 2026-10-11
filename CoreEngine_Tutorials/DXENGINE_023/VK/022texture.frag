#version 450
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in
vec2 fragTexCoord;
layout(location = 0) out
vec4 outColor;

layout(set = 0, binding = 1)
uniform sampler2D texSampler;

#include "TextureSampling.glsl"

void main()
{
    int mode = int(clamp(fragTexCoord.x, 0.0, 1.0) * 5.0);

    outColor = GetShaderTexture(texSampler, fragTexCoord, 0, mode);
}
