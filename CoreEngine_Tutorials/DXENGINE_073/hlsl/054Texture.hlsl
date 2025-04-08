// --------------------------------------------------------------------------------------------
// Filename: 054Texture.hlsl [LATEST]
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

//////////////
// TYPEDEFS //
//////////////
#define PS_USE_FOG           //51
#define PS_USE_ALFACOLOR
//#if defined PS_USE_FADE

// VERTEX:
struct VSIn
{
    float3 position :	POSITION;
    float4 color    : COLOR; //21
};

// PIXEL:
struct PSIn
{
    float4	Pos         : SV_POSITION;
    float4  Color       : COLOR0;
    float2 textCoord    : TEXCOORD0;
#if defined PS_USE_FOG
    float fogFactor     : FOG; // 51 FOG
#endif	
};

//Set on: DXmodelClass::RenderSubMesh
Texture2D shaderTexture:	register(t0);	//DX12: SRV
SamplerState SampleType;					//3D (default) WRAP

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsl"

float rand_1_05(in float2 uv)
{
    float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader054Texture(VSIn input)
{
    PSIn output;
#if defined PS_USE_FOG
    float4 cameraPosition;
#endif
    float4 Pos = float4(input.position, 1);
    
    float s_X = 0;
    float s_Y = 0;
    float s_Z = 0;
    
   //51:
    cameraPosition = mul(float4(input.position, 1), WV);
#if defined PS_USE_FOG
    if (VShasFog)
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
    else
        output.fogFactor = 0;
#endif
    
    output.textCoord = float2(Pos[0] * 50.0 / 1024, Pos[2] * 50 / 1024);
    
    float2 X0 = float2(Pos[0], Pos[2]);
    float height = 0;
    for (int i = 0; i < 3; i++)
    {
        float2 K = float2(1, i);        // K is a wave vector
        float w = 1.5 * (i + 1) / 3.0; // w is frequency
        float a = 0.0075; // a is amplitude: 0.01;
		
        // X = X0 - (K/k)(a*sin(K*X0 - wt);
        float2 X = X0 - K * a * sin(dot(K, X0) - w * time * 10);
        // y = a * cos (K*X0 - wt);
        float y = a * cos(dot(K, X0) - w * time * 10);
        
        s_X += X[0] / 8 - 5;
        s_Y += X[1] / 8 - 5;
        s_Z += y; //water altitude
        
        // float h = 0.01 * sin(dot(K, X0) + w*time*3);
    }

    Pos[0] = s_X;
    Pos[2] = s_Y;
    Pos[1] = s_Z;
    Pos *= 7.5f;
    Pos[3] /= 7.5f;

    output.Pos = mul(Pos, WVP); // Calculate the position of the vertex against the world, view, and projection matrices
    
    //float3 vLightDirection = (-1, 0, 0.25);
    //float4 vLightColor = (0.2, 0.7, 0.8, 0.7);
    //output.Color = saturate(dot((float3) vLightDirection, output.Pos * 0.5f) * vLightColor);
    output.Color = 0.9;
    output.Color += float4(0.1, 0.2, 0.5, 1);
    output.Color /= 1.75;

    return output;
}

float4 MyPixelShader054Texture(PSIn input) : SV_TARGET
{
/*
	// Sample the pixel color from the texture using the sampler at this texture coordinate location:
    float4 textureColor = shaderTexture.Sample(SampleType, input.texCoords);
#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif
    
    textureColor.rgb *= fade;



#if defined PS_USE_FOG
    if (hasFog)
    {
        float4 fog4 = 0;
        if (isSky)
        {
            fog4.r = 0.9;
        }
        else
        {
            fog4.r = (1.0 - input.fogFactor);
        }
        fog4.g = fog4.r;
        fog4.b = fog4.r;
        textureColor.rgb = lerp(textureColor.rgb, fogColor.rgb, fog4.rgb);
    }
#endif
    
    return textureColor;
*/

    float4 textureColor = input.Color;
    textureColor[2] += (rand_1_05(float2(input.Pos[2], input.Pos[2] * 2))) * 0.1;
    float4 color = input.Color;
    float3 text = shaderTexture.Sample(SampleType, float2(input.textCoord[0], input.textCoord[1]));
    color[0] = text[0];
    color[1] = text[1];
    color[2] = text[2];
 
    textureColor = (textureColor + color) / 2.0;

#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
    if (hasAlfaColor)
        textureColor.a = alfaColor;
#endif
    
    return textureColor;
}
