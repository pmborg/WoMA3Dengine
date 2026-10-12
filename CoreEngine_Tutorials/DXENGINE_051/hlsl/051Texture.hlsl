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
    float2 texCoords : TEXCOORD; 	//TEXCOORD0
    float fogFactor : FOG; 			// 51 FOG
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

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;

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
    if (VShasFog)
    {
        float4 cameraViewPosition = mul(float4(input.position, 1), WV);
        output.fogFactor = saturate((VSfogEnd - cameraViewPosition.z) / (VSfogEnd - VSfogStart));   // Calculate linear fog.  
    }
	
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

    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
    
    if (hasFog)
        textureColor.rgb = lerp(textureColor.rgb, fogColor.rgb, 1 - input.fogFactor);

#if DXAPI12 != 1
    else
        textureColor.rgb = pow(textureColor.rgb, 1.0 / 2.2); //Apply sRGB workflow: NEW 2026 review!
#endif
    
    textureColor.a = 0.5f;
    return textureColor;
}
