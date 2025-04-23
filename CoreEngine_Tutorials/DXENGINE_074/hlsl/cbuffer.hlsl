// --------------------------------------------------------------------------------------------
// Filename: cbuffer.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0) //Register is needed for DX12: Descriptor: 0
#endif
{
	// BLOCK: VS1
	matrix	worldMatrix;	//worldMatrix
	matrix	view;			//view
	matrix	projection;		//projection
	matrix  WV;				//worldMatrix+viewMatrix
	matrix  WVP;			//worldMatrix+viewMatrix+projectionMatrix

	// 23 BLOCK: VS2
	bool	VShasLight;
	bool	VShasSpecular;
	bool	VShasNormMap;
	bool	VShasFog;

	// 23 BLOCK: VS3
    float3  VSlightDirection;   // LIGHT (XMVECTOR = XMFLOAT4)
	float   VSlightPAD;         // 3+1=XMFLOAT4
	float4	VSambientColor;		// LIGHT: Ka
	float4	VSdiffuseColor;		// LIGHT: Kd
    float4  VSemissiveColor;    // LIGHT: Ke

	// 31 BLOCK: VS4
	float	VSfogStart;
	float	VSfogEnd;
	bool	VShasShadowMap;
	bool	VS_USE_WVP;

	// 45 BLOCK: VS5
	matrix	ViewToLightProj;
	matrix	WorldInverseTranspose;	// WorldInverseTranspose
	float4	vEye;					// camera position													   

	// 42 BLOCK: VS6
	float	VSrotX;
	float	VSrotY;
	float	VSrotZ;
	float	time;

	// 42 BLOCK: VS7
	float	VSshaderType;
	float	vsPAD2;
	float	vsPAD3;
	float	vsPAD4;

    // FIRE:
    float   vsframeTime;
    float3  scrollSpeeds;
    float3  scales;
    bool    isAnimatedBill;
};

///////////////
// PIXEL BUFFER
///////////////
//Note: on DX11 dont use: register()

// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer PSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer PSShaderParametersBuffer : register(b1)	//Register is needed for DX12: Descriptor: 1
#endif
{
	// BLOCK1:
	float4	pixelColor;

	// BLOCK2:
	bool	hasTexture;		// No? Use pixelColor, then.
	bool    hasLight;		// Future Load Obj. Engine Level
	bool	hasSpecular;	// Future Load Obj. Engine Level
	bool	isFont;			// Future Load Obj. Engine Level

	// BLOCK3:
	float4	ambientColor;	// LIGHT: Ka
	float4	diffuseColor;	// LIGHT: Kd
	float4	emissiveColor;	// LIGHT: Ke 
	float4	lightDirection;	// LIGHT

	// BLOCK4:
	bool	hasColorMap;		// 66
	float	lightType;			// Future
	float	PSshaderType;			// Future
	float	shaderTypeParameter;// Future

	// BLOCK5:
	bool	hasAlfaColor;
	float	alfaColor;
	float	fade;			// Fade from 0 to 1
	float	frameTime;		// For animations

	// BLOCK6:
	bool	hasFog;
	bool	isSky;
	bool    hasAlfaMap;
	bool	hasNormMap;

	// BLOCK7:
	float3	cameraPosition;	// Future
	bool	castShadow;
	float3	specularColor;
	float	nShininess;

    // FIRE:
    float2  distortion1;
    float2  distortion2;
    float2  distortion3;
    float   distortionScale;
    float   distortionBias;
};