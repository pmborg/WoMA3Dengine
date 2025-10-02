// --------------------------------------------------------------------------------------------
// 097waterfall.hlsl - v2
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
	float4 color : COLOR;				  
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD0;
    float4 color    : COLOR;
    float3 worldPos : TEXCOORD1; // pass world-space pos from VS
};

Texture2D waterfallTex  : register(t0); // Water streaks
Texture2D noiseTex      : register(t1); // Noise for turbulence
Texture2D foamTex       : register(t2); // Foam texture

SamplerState SampleType;

////////////////
// CBUFFERS
////////////////


// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0) //Register is needed for DX12: Descriptor: 0
#endif
{   //c++: struct VSWaterfallConstantBufferType
    matrix World;
    matrix WVP;
    float  time;
    float  scrollSpeed;
    float  noiseStrength;
    float  foamY; // bottom foam start height
    float4 foamColor; // foam tint
    float  fade;
    float3 pad;
};


////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;
    
	// Make sure position is a 4D vector
    input.position.w = 1.0f;

    // Compute world position:
    float4 worldPos4 = mul(input.position, World);
    output.worldPos = worldPos4.xyz;
    
    // Final space position:
	output.position = mul(input.position, WVP);	// Calculate the position of the vertex against the world, view, and projection matrices.

    output.tex = input.tex; // Store the texture coordinates for the PIXEL SHADER.
    output.color = input.color; // Store the particle color for the pixel shader.
    
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////

float4 PS_Main_(PSIn IN) : SV_TARGET
{
    // --- Scroll texture ---
    float2 uv = IN.tex;
    uv.y += time * scrollSpeed;

    // --- Add turbulence with noise ---
    float2 nUV = uv * 0.5 + time * 0.1;
    float2 noise = noiseTex.Sample(SampleType, nUV).rg - 0.5;
    uv += noise * noiseStrength;

    // --- Base waterfall color ---
    float4 water = waterfallTex.Sample(SampleType, uv) * IN.color;

    // --- Foam at bottom ---
    float foamMask = saturate((foamY - IN.worldPos.y) * 0.3);
    float4 foam = foamTex.Sample(SampleType, uv * 0.8) * foamColor * foamMask;

    // --- Combine ---
    float4 finalColor = lerp(water, foam, foamMask);
    finalColor.a *= 0.7 + foamMask * 0.3; // stronger alpha near foam
    
    //finalColor.a = max(0.15f, finalColor.a); // stronger alpha near foam
    return finalColor;
}

float4 PS_Main_ori(PSIn input) : SV_TARGET
{
    float4 textureColor;
    float4 finalColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = waterfallTex.Sample(SampleType, input.tex);

	// Combine the texture color and the particle color to get the final color result.
    finalColor = (textureColor * input.color);
    finalColor.a = 0.15f;

    return finalColor;
}

float4 PS_Main(PSIn input) : SV_TARGET
{
	float4 textureColor;
	//float4 waterColor;
	float4 finalColor;
    float2 uv = input.tex;
    
    float2 nUV = input.tex * 0.4 + time * 0.05;
    float2 noise = noiseTex.Sample(SampleType, nUV).rg - 0.5;
    uv += noise * 0.02; // very subtle wobble
    textureColor = waterfallTex.Sample(SampleType, uv) * input.color;
    
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = waterfallTex.Sample(SampleType, input.tex);

	// Combine the texture color and the particle color to get the final color result.
    finalColor =  (textureColor * input.color);
    finalColor.a = 0.05f; // experiment 0.1-0.18

    if (fade < 1)
        finalColor.rgb *= fade;
    return finalColor;
    
    //return float4(fade, fade, fade, 1);
}