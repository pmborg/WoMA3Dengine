// --------------------------------------------------------------------------------------------
// Filename: 023Light.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2026
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234526920;

#if DXAPI11 != 1 && DXAPI12 != 1
    #define DXAPI9 1
#endif

// mode:
// -1 = hardware sampler
//  0 = nearest
//  1 = bilinear
//  2 = trilinear
//  3 = cubic

#define TEXTURE_MODE -1 //Default is -1

//#define   PS_USE_FOG          //not sup. at this level
//#define   PS_USE_ALFACOLOR	//not sup. at this level
//#define   PS_USE_FADE			//not sup. at this level


//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
    float3 position : POSITION;  //21
    float2 texCoords : TEXCOORD; //22
    float3 normal : NORMAL; 	 //23
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION; // 21
    float2 texCoords : TEXCOORD;   // 22
    float3 normal : NORMAL;        // 23 LIGHT
};

/////////////
// GLOBALS //
/////////////

////////////////
// CBUFFERS
////////////////
// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI9 == 1 || DXAPI11 == 1
cbuffer VSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0) //Register is needed for DX12: Descriptor: 0
#endif
{
    // VERTEX: need to match: 
    // [DXshaderClass.h] VSconstantBufferType

    // BLOCK: VS1
    matrix worldMatrix; //worldMatrix
    matrix view;        //view
    matrix projection;  //projection
    matrix WV;          //worldMatrix+viewMatrix
    matrix WVP;         //worldMatrix+viewMatrix+projectionMatrix

    //// 23 BLOCK: VS2
    bool VShasLight;
    bool VShasSpecular;
    bool VShasNormMap;
    bool VShasFog;
    
    // 23 BLOCK: VS3
    float3 VSlightDirection;// LIGHT
    float  VSlightPAD;      // 3+1=XMFLOAT4
    float4 VSambientColor;  // LIGHT
    float4 VSdiffuseColor;  // LIGHT
    float4 VSemissiveColor; // LIGHT: Ke
};

///////////////
// PIXEL BUFFER
///////////////
//Note: on DX11 dont use: register()

// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI9 == 1 || DXAPI11 == 1
cbuffer PSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer PSShaderParametersBuffer : register(b1)	//Register is needed for DX12: Descriptor: 1
#endif
{
    // BLOCK1:
    float4 pixelColor;

    // BLOCK2:
    bool hasTexture; // No? Use pixelColor, then.
    bool hasLight; // Future Load Obj. Engine Level
    bool hasSpecular; // Future Load Obj. Engine Level
    bool isFont; // Future Load Obj. Engine Level

    // BLOCK3:
    float4 ambientColor; // LIGHT: Ka
    float4 diffuseColor; // LIGHT: Kd
    float4 emissiveColor; // LIGHT: Ke 
    float4 lightDirection; // LIGHT
 };

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

////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc1(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
    return saturate(dot(Normal, lightDirection)); // Calculate the amount of light on this pixel
}
////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc2(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
    return saturate(dot(Normal, -lightDirection)); // Calculate the amount of light on this pixel
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;

    output.position = mul(float4(input.position, 1), WVP); // Calculate the position of the vertex against the world, view, and projection matrices

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
    output.texCoords = input.texCoords;

	//23: LIGHT: NORMAL
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix)); // Calculate the normal vector against the world matrix only

    return output;
}

#if DXAPI9 == 0
#include "TextureSampling.hlsli"
// mode:
// 0 = nearest
// 1 = bilinear
// 2 = trilinear
// 3 = cubic
// 4 = hardware sampler
#endif

//Mode	Filter Type	    FPS
//0	    Nearest	        13111 FPS
//1	    Bilinear	    12945 FPS
//2	    Trilinear	    12989 FPS
//3	    Cubic 	        13141 FPS

float4 GetShaderTexture(Texture2D tex, float2 texCoords, uint mipLevel, int mode)
{
#if DXAPI9 == 0
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
#endif

    // Legacy fallback (DX9 / ps_4_0_level_9_3)
    return tex.Sample(SampleType, texCoords);
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 textureColor = 0;
    float lightIntensity = 0;

	//-----------------------------------------------------------------------------------
    //Mode	Filter Type	    FULL-HD     4K
    //0	    Nearest	        13111 FPS   8949FPS
    //1	    Bilinear	    12945 FPS   9334FPS
    //2	    Trilinear	    12989 FPS   8913FPS
    //3	    Cubic 	        13141 FPS   9940FPS
    
    // NOTES:
    // - Cubic (Catmull–Rom 16 taps) is surprisingly fast due to cache coherence.
    // - Default hardware sampler = same as mode 0 (nearest) unless sampler state says otherwise.
    // - At this engine level (22), all filters have similar performance.
    
    //-----------------------------------------------------------------------------------
    // Demo: split the quad in 5 vertical bands for 5 filters
    //-----------------------------------------------------------------------------------
    
    int mode = int(saturate(input.texCoords.x) * 5.0); // 0..4
    textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0, mode);
    
    //texCoord.x range	mode	column
    //[0.00–0.199]	    0	    Nearest
    //[0.20–0.399]	    1	    Bilinear
    //[0.40–0.599]	    2	    Trilinear
    //[0.60–0.799]	    3	    Cubic
    //[0.80–1.00 ]	    4       DEFAULT	hardware sampler (equal to nearest by default)
    
    // NOTE: mode 4 automatically uses tex.Sample(SampleType, uv)
    //       because GetShaderTexture() falls back to the default case.
    lightIntensity = PSlightFunc2(input.normal);
    textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));

    textureColor.a = 1;
    return textureColor;
}
