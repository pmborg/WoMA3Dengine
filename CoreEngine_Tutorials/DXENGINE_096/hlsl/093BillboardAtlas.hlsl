// --------------------------------------------------------------------------------------------
// 093BillboardAtlas.hlsl - Fast Billboards + Atlas + Optional Diffuse Lighting
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#define DXAPI11             1   //force

// ------------------------------------------------------------------------------------------------
// Vertex Input
// ------------------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 position : POSITION; // quad-local vertex position (-0.5..+0.5)
    float2 uv : TEXCOORD0; // quad-local UV
    float3 normal : NORMAL; // normal
    nointerpolation uint atlasIndex : TEXCOORD1; // atlas index
    float3 origin : TEXCOORD2; // world-space origin (pivot)
    float scale : TEXCOORD3; // uniform scale
    float rotY : TEXCOORD4; // rotation around Y in radians
};

// ------------------------------------------------------------------------------------------------
// Vertex Output
// ------------------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    nointerpolation uint atlasIndex : TEXCOORD1;
};

// ------------------------------------------------------------------------------------------------
// Atlas UV Data from Engine
// ------------------------------------------------------------------------------------------------


//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D billboardAtlasSRV;
#endif
#if DXAPI12 == 1
Texture2D billboardAtlasSRV:	register(t0);
#endif

#if DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif


// Represents a tile inside the atlas
struct AtlasRegion
{
    float2 offset; // UV start
    float2 scale; // UV size
};

// Adjust this based on your maximum number of billboards
#define MAX_ATLAS_REGIONS 128

// ------------------------------------------------------------------------------------------------
// Constant Buffer Layout (Synced with SetShaderParameters)
// ------------------------------------------------------------------------------------------------
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer
#endif
#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0)
#endif
{   //c++: struct VSBillboardAtlasConstantBufferType
    matrix worldMatrix;
    matrix WVP;

    // Lighting
    float4 lightDirection;

    // Vegetation / Wind (optional)
    float   vsframeTime;
    float3  scrollSpeeds;
    float3  scales;
    bool    isAnimatedBill;
    
    float   fade;
    float3  pad;
    
    AtlasRegion billboardAtlasRegions[MAX_ATLAS_REGIONS];
};

#include "light.hlsli"

////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 1) Scale local quad
    float3 p = input.position * input.scale;

    // 2) Build rotation matrix around Y
    float c = cos(input.rotY);
    float s = sin(input.rotY);
    float3 r;
    r.x = p.x * c - p.z * s;
    r.y = p.y;
    r.z = p.x * s + p.z * c;

    // 3) Translate
    float3 worldPos = input.origin + r;

    // 4) Apply WVP for clip-space position
    output.position = mul(float4(worldPos, 1.0f), WVP);

    // 5) Pass UV and normal through
    output.uv = input.uv;
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));
    output.atlasIndex = input.atlasIndex;

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(VS_OUTPUT input) : SV_TARGET
{
    // Get the atlas region for this billboard
    AtlasRegion region = billboardAtlasRegions[input.atlasIndex];

    // Remap the quad-local UVs into atlas space
    float2 atlasUV = region.offset + input.uv * region.scale;
    
    // Sample the correct texture tile
    float4 color = billboardAtlasSRV.Sample(SampleType, atlasUV);

    // --- CUTOUT: don't write color *or depth* where alpha is low
    // (prevents the "square holes / wrong order" issues)
    const float AlphaCutoff = 0.33f; // tweak 0.25–0.5 to taste
    clip(color.a - AlphaCutoff);
    
    // Apply lighting (if needed)
    float lightIntensity = saturate(0.6f + PSlightFunc1(input.normal));
    color.rgb *= lightIntensity;
	
    if (fade < 1)
        color.rgb *= fade;
    return color;
}