// --------------------------------------------------------------------------------------------
// Filename: 022Texture.hlsl [LATEST]
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2026
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
    DXshaderClass::InitializeShader(...)
    ...
		vsFilename.append(L"hlsl/022Texture.hlsl");
		psFilename = vsFilename;
		vertexHLSL.append("VS_Main");
		pixelHLSL.append("PS_Main");
     ...
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

// VERTEX: need to match: 
// [DXshaderClass.cpp] texturePolygonLayout11 / texturePolygonLayout
struct VSIn
{
    float3 position : POSITION;
    float2 texCoords : TEXCOORD; //TEXCOORD0
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXCOORD; //TEXCOORD0
#if defined PS_USE_FOG
    float fogFactor : FOG; // 51 FOG
#endif	
};

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
    //bool VShasLight;
    //bool VShasSpecular;
    //bool VShasNormMap;
    //bool VShasFog;
	//
    //// 23 BLOCK: VS3
    //float3 VSlightDirection;// LIGHT
    //float VSlightPAD;       // 3+1=XMFLOAT4
    //float4 VSambientColor;  // LIGHT
    //float4 VSdiffuseColor;  // LIGHT
    //float4 VSemissiveColor; // LIGHT: Ke
	//
    //// 31 BLOCK: VS4
    //float VSfogStart;
    //float VSfogEnd;
    //bool VShasShadowMap;
    //bool VS_USE_WVP;
	//
    //// 45 BLOCK: VS5
    //matrix ViewToLightProj;
    //matrix WorldInverseTranspose;   // WorldInverseTranspose
    //float4 vEye;                    // camera position		
	//
    //// 42 BLOCK: VS6
    //float VSrotX;
    //float VSrotY;
    //float VSrotZ;
    //float time;
	//
    //// 42 BLOCK: VS7
    //float VSshaderType;
    //float vsPAD2;
    //float vsPAD3;
    //float vsPAD4;
};

///////////////
// PIXEL BUFFER
///////////////
// Note: Color shader dont have it!
// 022Texture.hlsl at level 22 dont have it!

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
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;
#if defined PS_USE_FOG
    float4 cameraPosition;
#endif

//if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
//} else {
//	float4 position = float4(input.position, 1);
//	position = mul(position, worldMatrix);
//	position = mul(position, view);			//viewMatrix
//	position = mul(position, projection);	//projectionMatrix
//	output.position = position;
//}

    output.texCoords = input.texCoords;						// TEXTURE: Store the texture coordinates for the pixel shader:

    //51:
#if defined PS_USE_FOG
    cameraPosition = mul(float4(input.position, 1), WV);
    if (VShasFog)
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
#endif
	
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
    float4 textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0, TEXTURE_MODE);

#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif
    
#if defined PS_USE_FADE
     textureColor.rgb *= fade;
#endif
    
#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
	if (hasAlfaColor)
        textureColor.a = alfaColor;
#endif

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
    
#if DXAPI12 != 1
    textureColor.rgb = pow(textureColor.rgb, 1.0 / 2.2); //Apply sRGB workflow: NEW 2026 review!
#endif
    
    return textureColor;
}
