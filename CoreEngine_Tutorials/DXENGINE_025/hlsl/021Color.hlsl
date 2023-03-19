// --------------------------------------------------------------------------------------------
// Filename: 021Color.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

//#define WITHOUT_CONSTANT_BUFFER

/*
    PS 1.0 — Unreleased 3dfx Rampage, DirectX 8.
    PS 1.1 — GeForce 3, DirectX 8.
    PS 1.2 — 3Dlabs Wildcat VP, DirectX 8.0a.
    PS 1.3 — GeForce 4 Ti, DirectX 8.0a.
    PS 1.4 — Radeon 8500-9250, Matrox Parhelia, DirectX 8.1.
    Shader Model 2.0 — Radeon 9500-9800/X300-X600, DirectX 9.
    Shader Model 2.0a — GeForce FX/PCX-optimized model, DirectX 9.0a.
    Shader Model 2.0b — Radeon X700-X850 shader model, DirectX 9.0b.
    Shader Model 3.0 — Radeon X1000 and GeForce 6, DirectX 9.0c.
    Shader Model 4.0 — Radeon HD 2000 and GeForce 8, DirectX 10.
    Shader Model 4.1 — Radeon HD 3000 and GeForce 200, DirectX 10.1.
    Shader Model 5.0 — Radeon HD 5000 and GeForce 400, DirectX 11.
    Shader Model 5.1 — GCN 1+, Fermi+, DirectX 12 (11_0+) with WDDM 2.0.
    Shader Model 6.0 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.1.
    Shader Model 6.1 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.3.
    Shader Model 6.2 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.4.
    Shader Model 6.3 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.5.
    Shader Model 6.4 — GCN 1+, Kepler+, Skylake+, DirectX 12 (11_0+) with WDDM 2.6.
    Shader Model 6.5 — GCN 1+, Kepler+, Skylake+, DirectX 12 (11_0+) with WDDM 2.7.
    Shader Model 6.6 — GCN 4+, Maxwell+, DirectX 12 (11_0+) with WDDM 3.0.
    Shader Model 6.7 — GCN 4+, Maxwell+, DirectX 12 (12_0+) with WDDM 3.1.
*/

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
	float3 position : POSITION;
    float4 color	: COLOR;	//21
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
};

//CBV-srvHeapDesc   |0| DescriptorTable  | b0				|
cbuffer VSShaderParametersBuffer// SYNC: DXshaderClass.h
{
	// BLOCK: VS1
	matrix	worldMatrix;	//worldMatrix
	matrix  WV;				//worldMatrix+viewMatrix
	matrix  WVP;			//worldMatrix+viewMatrix+projectionMatrix
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader021Color(VSIn input)
{
    PSIn output;

#if defined WITHOUT_CONSTANT_BUFFER
	matrix WVPMatrix = {
		1.24942219f, 0.000000001f, 0.000000002f, 0.000000003f,	// row 1
		0.000000006f, 2.35672379f, 0.523716450f, -0.523716271f,	// row 2
		0.000000007f, -0.216972843f, 0.976377785f, 8.14494896f,	// row 3
		0.000000008f, -0.216930464f, 0.976187050f, 8.24335766f,	// row 4
	};

	output.position = mul(float4(input.position, 1), transpose(WVPMatrix));	// Calculate the position of the vertex against the world, view, and projection matrices
#else
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
#endif

	output.color = input.color;								// Store the vertex color for the pixel shader

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader021Color(PSIn input) : SV_TARGET
{
	return input.color; // Use Vertex Color
}