// --------------------------------------------------------------------------------------------
// 096curved_real_sky_plane.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Adopt the base idea of https://rastertek.com/tertut12.html
// --------------------------------------------------------------------------------------------
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#define DXAPI11             1   //force

//////////////
// TYPEDEFS //
//////////////
// VERTEX:
struct VSIn
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

Texture2D cloudTexture : register(t0);
Texture2D perturbTexture : register(t1);
SamplerState SampleType; //3D (default) WRAP

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"

////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;
    
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	output.position = mul(input.position, WVP);	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.tex = input.tex;	                    // Store the texture coordinates for the PIXEL SHADER.
	
    return output;
}

#define brightness 0.5f
#define scale 1.5f //0.3f
#define translation distortionBias

////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 perturbValue;
    float4 cloudColor;

    // Translate the texture coordinate sampling location by the translation value.
    input.tex.x = input.tex.x + translation;

    // Sample the texture value from the perturb texture using the translated texture coordinates.
    perturbValue = perturbTexture.Sample(SampleType, input.tex);

    // Multiply the perturb value by the perturb scale.
    perturbValue = perturbValue * scale;

    // Add the texture coordinates as well as the translation value to get the perturbed texture coordinate sampling location.
    perturbValue.xy = perturbValue.xy + input.tex.xy + translation;

    // Now sample the color from the cloud texture using the perturbed sampling coordinates.
    cloudColor = cloudTexture.Sample(SampleType, perturbValue.xy);

    // Reduce the color cloud by the brightness value.
    cloudColor = cloudColor * brightness;
 
    if (fade < 1)
        cloudColor.rgb *= fade;
    return cloudColor;
}