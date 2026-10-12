// --------------------------------------------------------------------------------------------
// Filename: 099texture2D.hlsl
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

//////////////
// TYPEDEFS //
//////////////
#define CAMERA_NORMAL       0
#define CAMERA_SKY          1
#define CAMERA_MINIMAP      2

#define PROJECTION_PERSPECTIVE 0
#define PROJECTION_ORTHO       1
#define PROJECTION_MINIMAP     2

// VERTEX:
struct VSIn
{
    float4 position : POSITION; // .w = object ID (0–255)
    float2 texCoord : TEXCOORD0;   // UV coordinates
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

////////////////
// CBUFFERS
////////////////
//#include "cbufferONE99.hlsli"  // b0 and b1
cbuffer CB_ColorObjects : register(b0)
{
    matrix gWorld[256];
}

cbuffer CB_ViewProj : register(b1)
{
    matrix gVP[3][3];
};

// ------------------------------------------------------------------
// SHADER TEXTURES
// ------------------------------------------------------------------
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

#define ID input.position.w

//#define DEBUG_WVP

////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;
    
#if defined DEBUG_WVP
    matrix WVPMatrix =
    {
        1.24942219f, 0.000000001f, 0.000000002f, 0.000000003f, // row 1
	    0.000000006f, 2.35672379f, 0.523716450f, -0.523716271f, // row 2
	    0.000000007f, -0.216972843f, 0.976377785f, 8.14494896f, // row 3
	    0.000000008f, -0.216930464f, 0.976187050f, 8.24335766f, // row 4
    };
#endif

	// Transform vertex position using the per-instance WVP
#if defined DEBUG_WVP
    output.position = mul(float4(input.position.xyz, 1), transpose(WVPMatrix)); // Calculate the position of the vertex against the world, view, and projection matrices
#else
    output.position = mul(float4(input.position.xyz, 1.0f), transpose(mul(gWorld[ID], gVP[CAMERA_NORMAL][PROJECTION_ORTHO])));
#endif
    
	output.texCoord = input.texCoord;
    
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 texColor = shaderTexture.Sample(SampleType, input.texCoord);
    return texColor;
    //return float4(1,1,1,1);
}