// --------------------------------------------------------------------------------------------
// 097smoke.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Implement SHADER_USE_SMOKE
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
    float3 pos : POSITION;
    float2 sizeLife : TEXCOORD0; // x=size, y=life
};
// PIXEL:
struct PSIn
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    //float life : TEXCOORD1;
};

#define VSOut PSIn

Texture2D smokeTex : register(t0);

SamplerState SampleType : register(s0);

////////////////
// CBUFFERS
////////////////
cbuffer VSConstants : register(b0)
{ //c++: struct VSSmokeCB
    matrix WVP;
    float3 cameraPos;
    float fade;
};
cbuffer SmokeParticleCB : register(b1)
{
    float g_particleAlpha; // set by C++ per-particle
};

VSOut VS_Main(VSIn input)
{
    VSOut output;
    output.pos = mul(float4(input.pos, 1.0), WVP);
    output.tex = input.sizeLife.xy; // or pass proper quad texcoord from VB

    return output;
}

float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 tex = smokeTex.Sample(SampleType, input.tex);
    tex.a *= g_particleAlpha; // fade over lifetime
    clip(tex.a - 0.03f); // softer edges  

    if (fade < 1)
        tex.rgb *= 0.7f;
    else
        tex.rgb = saturate(tex.rgb * 1.2f);
    return tex;
}
