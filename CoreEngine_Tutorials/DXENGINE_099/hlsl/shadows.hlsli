// --------------------------------------------------------------------------------------------
// Filename: shadows.hlsli
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

float HasShadows(float4 lightViewPosition)
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
