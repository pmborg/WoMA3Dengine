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

////////////////
// VERTEX BUFFER
////////////////
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