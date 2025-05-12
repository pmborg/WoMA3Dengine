#ifndef PHONG_COMMON
#define PHONG_COMMON

// Currently unused, migrated to PBR

float3 PhongDiffuse(float NdotL, float3 lightColour)
{
    return NdotL * lightColour;
}

float3 PhongSpecular(float3 L, float3 N, float3 V, float3 lightColour, float shininess)
{
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));
    return lightColour * pow(RdotV, shininess);
}

#endif
