// --------------------------------------------------------------------------------------------
// Filename: 054Texture.hlsl [LATEST]
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
*   water_waves
**********************************************************************************************/
//WomaIntegrityCheck = 1234526920;

#if DXAPI11 != 1 && DXAPI12 != 1
#define DXAPI9 1
#endif

//////////////
// TYPEDEFS //
//////////////
#define PS_USE_FOG           //51
#define PS_USE_ALFACOLOR
//#if defined PS_USE_FADE

#define TEXTURE_MODE -1 //Default is -1

// VERTEX:
struct VSIn
{
    float3 position : POSITION;
};

// PIXEL:
struct PSIn
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float2 textCoord : TEXCOORD0;
#if defined PS_USE_FOG
    float fogFactor : FOG; // 51 FOG
#endif
    float3 worldPos : TEXCOORD1;
};

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"

//Set on: DXmodelClass::RenderSubMesh
#if DXAPI9 == 1 || DXAPI11 == 1
Texture2D shaderTexture;
#endif
#if DXAPI12 == 1
Texture2D shaderTexture:	register(t0);
#endif

#if DXAPI9 == 1 || DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif

#include "light.hlsli"
#include "TextureSampling.hlsli"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;

    float4 Pos = float4(input.position, 1);
    
    float s_X = 0;
    float s_Y = 0;
    float s_Z = 0;
    
    // -------------------------------------------------------------------------
    // LEVEL 51: Fog
    // -------------------------------------------------------------------------
#if defined PS_USE_FOG
    float4 cameraPosition = mul(float4(input.position, 1), WV);
    if (VShasFog)
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
    else
        output.fogFactor = 0;
#endif
    
    // -------------------------------------------------------------------------
    // Procedural UV + scrolling (LEVEL 54)
    // -------------------------------------------------------------------------
    output.textCoord = float2(Pos[0] * 150.0 / 1024.0, Pos[2] * 150.0 / 1024.0);

    // Strong river current
    output.textCoord += float2(time * 0.15, time * 0.04);
    
    // -------------------------------------------------------------------------
    // LEVEL 54: Water/River waves
    // -------------------------------------------------------------------------
    float2 X0 = float2(Pos[0], Pos[2]);
    
    float height = 0;
    for (int i = 0; i < 3; i++)
    {
        float2 K = float2(1, i); // K is a wave vector
        float w = 1.5 * (i + 1); // w is frequency
        float a = 0.0035; // a is amplitude
        
        float phase = dot(K, X0) - (w * time);
        
        // FORMULA: X = X0 - (K/k)(a*sin(K*X0 - w.t);
        float2 X = X0 - K * a * sin(phase);
		
        // FORMULA: y = a * cos (K*X0 - w.t);
        float y = a * cos(phase);
        
        s_X += X[0] / 16;
        s_Y += X[1] / 16;
        s_Z += y; //water altitude
    }

    Pos[0] = s_X;
    Pos[2] = s_Y;
    Pos[1] = s_Z;
    
    
    Pos *= 7.5f;
    Pos[3] /= 7.5f;

    // Set world position AFTER all math, before projection
    output.worldPos = Pos.xyz;
    
    output.Pos = mul(Pos, WVP); // Calculate the position of the vertex against the world, view, and projection matrices
    
	// Base water tint
    output.Color = 0.9;
    output.Color += float4(0.1, 0.2, 0.5, 1);
    output.Color /= 1.75;
    
    return output;
}

float4 GetShaderTexture(Texture2D tex, float2 texCoords, uint mipLevel, int mode)
{
    switch (mode)
    {
        case 0:
            return NearestInterpolation(SampleType, tex, texCoords);
        case 1:
            return BilinearInterpolation(SampleType, tex, texCoords);
        case 2:
            return TrilinearInterpolation(SampleType, tex, texCoords, mipLevel);
        case 3:
            return CubicInterpolation(SampleType, tex, texCoords);
        
        default:
            return tex.Sample(SampleType, texCoords);
    }

    // fallback
    return tex.Sample(SampleType, texCoords);
}

float rand_1_05(in float2 uv)
{
    float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}

float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 textureColor = input.Color;
    textureColor[2] += (rand_1_05(float2(input.Pos[2], input.Pos[2]))) * 0.1;
	
    float4 color = input.Color;
    
	float3 text = GetShaderTexture(shaderTexture, float2(input.textCoord[0], input.textCoord[1]), 0, TEXTURE_MODE);
	
    color[0] = text[0];
    color[1] = text[1];
    color[2] = text[2];
 
    if ((input.worldPos.x > 0.5f && input.worldPos.z > 0.5f) && (input.worldPos.x < 128 && input.worldPos.z < 128))
        textureColor = (textureColor + color) / 2.0;
    else
        textureColor = textureColor * color;

#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
    if (hasAlfaColor)
        textureColor.a = alfaColor;
#endif
    
	// Gloval Fade Effect (LEVEL 29)
	textureColor.rgb *= fade;
	
    return textureColor;
}
