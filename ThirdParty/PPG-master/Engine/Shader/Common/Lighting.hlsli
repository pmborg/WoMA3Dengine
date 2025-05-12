#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

#include "Sampler.hlsli"

#define MAX_LIGHTS 4
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLED 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_W_SHADOWMAP 2

Texture2D ShadowMap : register(t4); // support 1 for now, future use array

struct Light
{
    float4 position;
    float4 direction;
    float4 color;

    float constantAtt;
    float linearAtt;
    float quadAtt;
    float spotAngle;

    int lightType;
    int status;
};

cbuffer LightProperties : register(b1)
{
    float4 eyePosition;
    float4 globalAmbient;
    Light Lights[MAX_LIGHTS];
}

cbuffer ShadowMapConstants : register(b2) // supports one
{
    float mapWidth;
    float mapHeight;
    float4x4 lightViewProjection;
}

float ShadowFactor(float4 worldPosition) // assumes only one shadow map cbuffer
{
    float4 lightSpacePosition = mul(lightViewProjection, worldPosition);

    float3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
    float currentDepth = projCoords.z;

    if (currentDepth > 1)
        return 0;

    projCoords = (projCoords + 1) / 2.0; // change to [0 - 1]
    projCoords.y = -projCoords.y; // bottom right corner is (1, -1) in NDC so we have to flip it

    float2 texelSize = float2(1, 1) / float2(mapWidth, mapHeight);

    float shadow = 0;
    float epsilon = 0.01f;
    for (int x = -1; x < 2; ++x)
        for (int y = -1; y < 2; ++y)
        {
            float closestDepth = ShadowMap.Sample(LinearSampler, projCoords.xy + float2(x, y) * texelSize).r;
            shadow += (closestDepth < currentDepth - epsilon);
        }
    shadow /= 9;
    return shadow;
}
#endif
