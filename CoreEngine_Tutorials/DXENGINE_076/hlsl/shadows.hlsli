// --------------------------------------------------------------------------------------------
// Filename: shadows.hlsli
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2026
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234526920;

float HasShadows_bad99(float4 lightViewPosition)
{
    float2 uv;
    float bias = 1.0f / 4096.0f;

    // Project to 0..1 shadow UV
    uv.x = lightViewPosition.x / lightViewPosition.w * 0.5f + 0.5f;
    uv.y = -lightViewPosition.y / lightViewPosition.w * 0.5f + 0.5f;

    // ----------------------------------------
    // 1) OUTSIDE SHADOW MAP? → FULL LIGHT
    // ----------------------------------------
    if (uv.x < 0.0 || uv.x > 1.0 ||
        uv.y < 0.0 || uv.y > 1.0)
    {
        return 1.0f; // No shadow at all
    }

    // ----------------------------------------
    // 2) INSIDE → NORMAL shadow compare
    // ----------------------------------------
    float depthValue = ShadowMapTexture.Sample(SampleType, uv).r;
    float lightDepth = lightViewPosition.z / lightViewPosition.w;

    return (lightDepth > depthValue + bias) ? 0.5f : 1.0f;
}

////////////////////////////////////////////////////////////////////////////////
float HasShadows(float4 lightViewPosition)
////////////////////////////////////////////////////////////////////////////////
{
    float2 uv;
    float bias = 5.0f / 256.0f; //45: Set the bias value for fixing the floating point precision issues.
    
    // Calculate the projected texture coordinates
    uv.x = lightViewPosition.x / lightViewPosition.w * 0.5f + 0.5f;
    uv.y = -lightViewPosition.y / lightViewPosition.w * 0.5f + 0.5f;
    
    // ----------------------------------------
    // 1) OUTSIDE SHADOW MAP? → FULL LIGHT
    // ----------------------------------------
    if (uv.x < 0.0 || uv.x > 1.0 ||
        uv.y < 0.0 || uv.y > 1.0)
    {
        return 1.0f; // No shadow at all
    }

    float depthValue = ShadowMapTexture.Sample(SampleType, uv).r;
    
    // Calculate the depth of the pixel
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue = lightDepthValue / 2.0f + 0.5;
        
    // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel
    // If the light is in front of the object then light the pixel, 
    // if not then shadow this pixel since an object (occlude) is casting a shadow on it
    return (lightDepthValue > depthValue + bias) ? 0.25f : 1.0f;
}