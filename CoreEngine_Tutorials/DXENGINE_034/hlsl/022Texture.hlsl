// --------------------------------------------------------------------------------------------
// Filename: 022Texture.hlsl [LATEST]
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
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
//WomaIntegrityCheck = 1234525217;

#if (!defined DXAPI11 && !defined DXAPI12)
    #define DXAPI11 1
#endif

// mode:
// -1 = hardware sampler
//  0 = nearest
//  1 = bilinear
//  2 = trilinear
//  3 = cubic

#define TEXTURE_MODE -1 //Default is -1

//#define   PS_USE_FOG           //51
#define   PS_USE_ALFACOLOR
#define   PS_USE_FADE


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

//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture;
#endif
#if DXAPI12 == 1
Texture2D shaderTexture:	register(t0);
#endif

#if DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;
#if defined PS_USE_FOG
    float4 cameraPosition;
#endif

if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
} else {
	float4 position = float4(input.position, 1);
	position = mul(position, worldMatrix);
	position = mul(position, view);			//viewMatrix
	position = mul(position, projection);	//projectionMatrix
	output.position = position;
}

    output.texCoords = input.texCoords;						// TEXTURE: Store the texture coordinates for the pixel shader:

    //51:
#if defined PS_USE_FOG
    cameraPosition = mul(float4(input.position, 1), WV);
    if (VShasFog)
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
#endif
	
    return output;
}

#include "TextureSampling.hlsli"
// mode:
// 0 = nearest
// 1 = bilinear
// 2 = trilinear
// 3 = cubic
// 4 = hardware sampler

//Mode	Filter Type	    FPS
//0	    Nearest	        13111 FPS
//1	    Bilinear	    12945 FPS
//2	    Trilinear	    12989 FPS
//3	    Cubic 	        13141 FPS

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


float4 PS_Main(PSIn input) : SV_TARGET
{
	//-----------------------------------------------------------------------------------
	// lvl >=21: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
    //replace:
        //textureColor = shaderTexture.Sample(SampleType, input.texCoords);    
    //with:
    #define TEXTURE_MODE 0
    float4 textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0, TEXTURE_MODE);
    
#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
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

#if defined PS_USE_FADE
     textureColor.rgb *= fade;
#endif

    textureColor.rgb = pow(textureColor.rgb, 1.0 / 2.2); //Apply sRGB workflow
    return textureColor;
}
