// --------------------------------------------------------------------------------------------
// Filename: 099textureLIGHT28.hlsl
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
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};
// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

//CB0:
cbuffer CB_STATICWORLDCPU : register(b0)
{
    matrix gWorld[256];
};
//CB1:
cbuffer CB_VPCPU : register(b1)
{
    matrix gVP[3][3];
};
//CB2:
#include "cbufferONE99.hlsli"

// ------------------------------------------------------------------
// SHADER TEXTURES
// ------------------------------------------------------------------
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

#define ID input.position.w

//#define DEBUG_WVP

PSIn VS_Main(VSIn input)
{
    PSIn output;
    
#if defined DEBUG_WVP
/*    
    matrix WVPMatrix =
    {
        1.24942219f, 0.000000001f, 0.000000002f, 0.000000003f, // row 1
	    0.000000006f, 2.35672379f, 0.523716450f, -0.523716271f, // row 2
	    0.000000007f, -0.216972843f, 0.976377785f, 8.14494896f, // row 3
	    0.000000008f, -0.216930464f, 0.976187050f, 8.24335766f, // row 4
    };
*/    
    matrix WVPMatrix =
    {
        1.269f, 0,       0,    0, // row 1
	    0,      2.414f,  0,    0, // row 2
	    0,      0,       1,    1, // row 3
	    0,      0,      -0.1f, 0, // row 4
    };

#endif
    
    #if defined DEBUG_WVP
    output.position = mul(float4(input.position.xyz, 1), transpose(mul(gWorld[ID], WVPMatrix)));
    #else
    output.position = mul(float4(input.position.xyz, 1.0f), transpose(mul(gWorld[ID], gVP[CAMERA_SKY][PROJECTION_PERSPECTIVE])));
    #endif

    output.texCoord = input.texCoord;
    output.normal = normalize(mul(input.normal, (float3x3) gWorld[ID]));
    
    return output;
}

float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 texColor = shaderTexture.Sample(SampleType, input.texCoord);
    
    float lightIntensity;
    if (isSky == 1)
        lightIntensity = PSlightFunc2(input.normal);
    else
        lightIntensity = PSlightFunc1(input.normal);
    
    float4 finalcolor = texColor * saturate( /*VSemissiveColor +*/ambientColor + diffuseColor * lightIntensity);
    
    finalcolor.a = 1;
    return finalcolor;
}
