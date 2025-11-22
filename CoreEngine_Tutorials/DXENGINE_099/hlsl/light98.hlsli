// --------------------------------------------------------------------------------------------
// Filename: light.hlsli
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc1(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
	return saturate(dot(Normal, -lightDirection));							// Calculate the amount of light on this pixel
}
////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc2(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
	return saturate(dot(Normal, lightDirection));							// Calculate the amount of light on this pixel
}

// Scalar intensity (no RGB), good for multiplying into totalLight
static const float3 LUMA = float3(0.299, 0.587, 0.114); // Rec.601

float ComputePointLights(float3 worldPos, float3 normal)
{
    float total = 0.0f;

    [loop]
    for (int i = 0; i < numPointLights; ++i)
    {
        float3 L = pointLights[i].position - worldPos;
        float d = length(L);
        if (d > pointLights[i].radius)
            continue;

        L = normalize(L);
        float NdotL = max(dot(normal, L), 0.0);
        float att = saturate(1.0 - d / pointLights[i].radius);

        // convert lamp color to a scalar brightness
        float lampLuma = dot(pointLights[i].color, LUMA);
        total += lampLuma * NdotL * att * pointLights[i].intensity;
    }
    return total;
}

// Spotlight-style lighting for street-lamps
// Lamps cast a cone of light pointing downwards (or in any direction you set)
// ---------------------------------------------------------------------------
float ComputeSpotLights(float3 worldPos, float3 normal)
{
    if (numPointLights == 0)
        return 0.0; // nothing to light

    float total = 0.0f;

    [loop]
    for (int i = 0; i < numPointLights; ++i)
    {
        float3 L = pointLights[i].position - worldPos;
        float d = length(L);
        if (d > pointLights[i].radius)
            continue;

        L = normalize(L);
        float NdotL = max(dot(normal, L), 0.0);
        float att = saturate(1.0 - d / pointLights[i].radius);

        // Downward cone
        const float3 lampDir = float3(0.0, -1.0, 0.0);
        const float innerCos = cos(radians(25.0));
        const float outerCos = cos(radians(40.0));
        float cosTheta = dot(-L, lampDir);
        float spot = saturate((cosTheta - outerCos) / (innerCos - outerCos));

        float lampLuma = dot(pointLights[i].color, float3(0.299, 0.587, 0.114));
        total += lampLuma * NdotL * att * spot * pointLights[i].intensity;
    }
    return total;
}
