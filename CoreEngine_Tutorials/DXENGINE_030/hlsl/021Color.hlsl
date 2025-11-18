// --------------------------------------------------------------------------------------------
// Filename: 021Color.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
    DXshaderClass::InitializeShader(...)
    ...
	    vsFilename.append(L"hlsl/021Color.hlsl");
	    psFilename = vsFilename;
	    vertexHLSL.append("VS_Main");
	    pixelHLSL.append("PS_Main");
     ...
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

//For Debug only!:
//#define WITHOUT_CONSTANT_BUFFER

#if !defined DXAPI11 && !defined DXAPI12
#define DXAPI11 1
#endif

//////////////
// TYPEDEFS //
//////////////

// VERTEX: need to match: 
// [DXshaderClass.cpp] colorPolygonLayout11 / colorPolygonLayout
struct VSIn
{
    float3 position : POSITION;
    float4 color : COLOR; //21
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

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
    // VERTEX: need to match: 
    // [DXshaderClass.h] VSconstantBufferType

    // BLOCK: VS1
    matrix worldMatrix; //worldMatrix
    matrix view; //view
    matrix projection; //projection
    matrix WV; //worldMatrix+viewMatrix
    matrix WVP; //worldMatrix+viewMatrix+projectionMatrix

    // 23 BLOCK: VS2
    //bool   VShasLight;
    //bool   VShasSpecular;
    //bool   VShasNormMap;
    //bool   VShasFog;

    //// 23 BLOCK: VS3
    //float3 VSlightDirection; // LIGHT
    //float  VSlightPAD; // 3+1=XMFLOAT4
    //float4 VSambientColor; // LIGHT
    //float4 VSdiffuseColor; // LIGHT
    //float4 VSemissiveColor; // LIGHT: Ke
	//
    //// 31 BLOCK: VS4
    //float  VSfogStart;
    //float  VSfogEnd;
    //bool   VShasShadowMap;
    //bool   VS_USE_WVP;
	//
    //// 45 BLOCK: VS5
    //matrix ViewToLightProj;
	
	//Future properties later...
	//
	//...
};

///////////////
// PIXEL BUFFER
///////////////
//Note: Color shader dont have it!

////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;

#if defined WITHOUT_CONSTANT_BUFFER
	matrix WVPMatrix = {
		1.24942219f, 0.000000001f, 0.000000002f, 0.000000003f,	// row 1
		0.000000006f, 2.35672379f, 0.523716450f, -0.523716271f,	// row 2
		0.000000007f, -0.216972843f, 0.976377785f, 8.14494896f,	// row 3
		0.000000008f, -0.216930464f, 0.976187050f, 8.24335766f,	// row 4
	};
    
    //HLSL dif. GLSL:
	output.position = mul(float4(input.position, 1), transpose(WVPMatrix));	// Calculate the position of the vertex against the world, view, and projection matrices
#else
    //if (VS_USE_WVP)
    {
        output.position = mul(float4(input.position, 1), WVP); // Calculate the position of the vertex against the world, view, and projection matrices
    }
    //else
    //{
    //    float4 position = float4(input.position, 1);
    //    position = mul(position, worldMatrix);
    //    position = mul(position, view); //viewMatrix
    //    position = mul(position, projection); //projectionMatrix
    //    output.position = position;
    //}
#endif
    output.color = input.color; // Store the vertex color for the pixel shader

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
    return input.color; // Use Vertex Color
}