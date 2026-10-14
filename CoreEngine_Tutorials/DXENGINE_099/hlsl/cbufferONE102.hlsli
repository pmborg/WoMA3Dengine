// --------------------------------------------------------------------------------------------
// Filename: cbufferONE99.hlsli
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	World of Middle Age (WoMA) - ENGINE 3D 2025
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
*	PURPOSE:
*		Level 99 unified color shader.
*		Supports the USE_ONE_CBUFFER system using per-instance data.
**********************************************************************************************/
//WomaIntegrityCheck = 1234526920;

// ───────────────────────────────────────────────
// PER-FRAME DATA
// ───────────────────────────────────────────────
cbuffer CB_PerFrame : register(b2)
{
    bool   VShasFog;        // 0 = no fog, 1 = fog
    float  VSfogStart;      // defined in world.xml
    float  VSfogEnd;        // defined in world.xml
    bool   isSky;           // 0 = normal object, 1 = sky
    
    float4 cameraPosition;  // xyz = eye position, w = padding
    float4 ambientColor;    // common .rgb ambient light color
    float4 diffuseColor;    // common .rgb diffuse light color
    float4 lightDirection;  // world-space light direction

    float4 vEye;            // camera position
    
    float  time;            // water and bill animation
    float3 scrollSpeeds;    // FIRE
    float3 scales;          // FIRE
    float  vsframeTime;     // FIRE
};


// ============================================================================================
// LIGHTING HELPER FUNCTIONS
// ============================================================================================

// --------------------------------------------------------------------
// Standard lighting (for regular outward-facing geometry)
//   N = surface normal (world space)
//   Returns diffuse light intensity [0,1]
// --------------------------------------------------------------------
float PSlightFunc1(float3 normal)
{
    float3 N = normalize(normal);
    float3 L = normalize(-lightDirection);
    return saturate(dot(N, L));
}

// --------------------------------------------------------------------
// Inverted lighting (for inward-facing geometry, e.g., sky dome)
//   N = surface normal (world space)
//   Returns diffuse light intensity [0,1]
// --------------------------------------------------------------------
float PSlightFunc2(float3 normal)
{
    float3 N = normalize(normal);
    float3 L = normalize(lightDirection);
    return saturate(dot(N, L));
}

/*
// ───────────────────────────────────────────────
// PER-OBJECT / INSTANCE DATA
// ───────────────────────────────────────────────
struct WomaObj
{
    float4x4 worldMatrix;
    float4x4 WVP;
 
    bool     VShasLight;
    bool     VShasSpecular;
    bool     VShasNormMap;
    bool     VShasShadowMap;
 
    uint     VSshaderType;
    bool     vsIsSky;
    float2   pad;
     
    float    VSrotX; 
    float    VSrotY; 
    float    VSrotZ; 
    bool     isAnimatedBill;
     
    float4   VSambientColor;         // LIGHT: Ka
    float4   VSdiffuseColor;         // LIGHT: Kd
    float4   VSemissiveColor;        // LIGHT: Ke
    matrix   ViewToLightProj;        // For Shadow
    matrix   WorldInverseTranspose;  // WorldInverseTranspose    
};

WomaObj objects[256];
*/
