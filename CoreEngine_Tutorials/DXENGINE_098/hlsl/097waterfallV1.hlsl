// --------------------------------------------------------------------------------------------
// 097waterfall.hlsl - Fast Billboards + Atlas + Optional Diffuse Lighting
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
    float2 tex : TEXCOORD0;
    float4 color : COLOR;
};

Texture2D shaderTexture : register(t0);
Texture2D waterTexture : register(t1);
SamplerState SampleType; //3D (default) WRAP

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
{
    matrix WVP;
};


////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;
    
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	output.position = mul(input.position, WVP);	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.tex = input.tex;	                    // Store the texture coordinates for the PIXEL SHADER.
	output.color = input.color;					// Store the particle color for the pixel shader.
	
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
	float4 textureColor;
	float4 waterColor;
	float4 finalColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	waterColor = waterTexture.Sample(SampleType, input.tex);


	// Combine the texture color and the particle color to get the final color result.
    finalColor =  (textureColor * input.color);
    finalColor.a = 0.15f;
    
    return finalColor;
}