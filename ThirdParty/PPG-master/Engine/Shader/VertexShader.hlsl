// --------------------------------------------------------------------------------------------
// Filename: VertexShader.hlsl
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG
// WomaIntegrityCheck = 1234525256;

#define PS_USE_SPECULAR

cbuffer PerObject : register(b0)
{
    matrix model;
}

cbuffer PerFrame : register(b1)
{
    matrix view;
}

cbuffer PerApplication : register(b2)
{
    matrix projection;
}

cbuffer BoneTransformation : register(b3)
{
    matrix BoneTransforms[128];
}

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    //float4 boneIds : BLENDINDICES; //AQUIFLOAT
    uint4 boneIds : BLENDINDICES0; //AQUIFLOAT
    float4 boneWeight : BLENDWEIGHT;
}; 

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    //float3 tangent : TANGENT;
    //float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
#if defined PS_USE_SPECULAR
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
    float4 wPosition : POSITION1;
#endif
    //float4 pos : POSITION0;
    float3x3 TBN : TBN_MATRIX;
};

VertexShaderOutput main(VertexShaderInput input)
{
    matrix vp;
    VertexShaderOutput OUT;
    
    bool skinningEnabled = (BoneTransforms[127]._11 == 127); //AQUI-ANIM
    if (skinningEnabled)                                      //AQUI-ANIM
    {
        matrix boneTransform = input.boneWeight[0] * BoneTransforms[input.boneIds[0]];
        for (int i = 1; i < 4; ++i)
        {
            boneTransform += input.boneWeight[i] * BoneTransforms[input.boneIds[i]];
        }
        input.position = mul(boneTransform, float4(input.position, 1.0f)).xyz;
        input.normal = normalize(mul(boneTransform, float4(input.normal, 0.0f)).xyz);
        input.tangent = normalize(mul(boneTransform, float4(input.tangent, 0.0f)).xyz);
        input.binormal = normalize(mul(boneTransform, float4(input.binormal, 0.0f)).xyz);
    }

    vp = mul(projection, view);
    OUT.position = mul(vp, mul(model, float4(input.position, 1.0f)));
    OUT.texCoord = input.texCoord;

    // assume a uniform scaling is observed
    // otherwise have have to multiply by transpose(inverse(model))
    // inverse should be calculated in the application (CPU)
    OUT.normal = normalize(input.normal);
    OUT.TBN = float3x3(normalize(input.tangent), normalize(input.binormal), OUT.normal);
    
    float4 cameraPosition = mul(float4(input.position, 1), vp);
    
	//34: SPECULAR
#if defined PS_USE_SPECULAR
    OUT.cameraPosition = cameraPosition;

    //if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
    {
        float4 worldPosition = mul(float4(input.position, 1), model); // P
        OUT.wPosition = worldPosition;
        OUT.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz); // L = Lp - p (L = lightDirection)
    }
#endif
    
    return OUT;
}


