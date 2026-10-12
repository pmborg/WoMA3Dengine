// --------------------------------------------------------------------------------------------
// Filename: 099text2D.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*   World of Middle Age (WoMA) - ENGINE 3D 2025
*   -------------------------------------------------------------------------------------------
*   code by : Pedro Borges - pmborg@yahoo.com
*   Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
*   PURPOSE:
*       Level 99 unified 2D text shader.
*       - Uses the USE_ONE_CBUFFER system.
*       - Supports per-instance transforms (gWorld[ID]).
*       - Adds font transparency & pixel color tint.
**********************************************************************************************/
//WomaIntegrityCheck = 1234526920;

//////////////
// TYPEDEFS //
//////////////
#define CAMERA_NORMAL 0
#define CAMERA_SKY    1
#define CAMERA_2D     2
#define PROJECTION_PERSPECTIVE  0
#define PROJECTION_ORTHOGRAPHIC 1

// VERTEX INPUT
struct VSIn
{
    float4 position : POSITION; // .w = object ID (0–255)
    float2 texCoord : TEXCOORD0;
};

// PIXEL INPUT
struct PSIn
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

////////////////
// CBUFFERS
////////////////
cbuffer CB_ColorObjects : register(b0)
{
    matrix gWorld[256];
}

cbuffer CB_ViewProj : register(b1)
{
    matrix gVP[3][2];
};

cbuffer CB_Font : register(b2)
{
    float4 pixelColor; // font tint
    bool isFont;
    float3 pad; // alignment
};

// ------------------------------------------------------------------
// SHADER TEXTURES
// ------------------------------------------------------------------
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

#define ID input.position.w

////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;

    // Per-instance world + camera 2D orthographic
    output.position = mul(float4(input.position.xyz, 1.0f),
                          transpose(mul(gWorld[ID], gVP[CAMERA_2D][PROJECTION_ORTHOGRAPHIC])));

    output.texCoord = input.texCoord;
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 color = shaderTexture.Sample(SampleType, input.texCoord);

    if (isFont)
    {
        // Black (background) = transparent
        if (color.r <= 0.1f && color.g <= 0.1f && color.b <= 0.1f)
            discard;
        else
            color.rgb = pixelColor.rgb; // tint font glyphs
    }

    return color;
}
