// --------------------------------------------------------------------------------------------
// Filename: DXshaderClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#pragma warning ( push )
#pragma warning ( disable : 4101 ) //warning C4101 : 'cameraPosition' : unreferenced local variable

#include "OSengine.h"
#if defined DX_ENGINE
#include <d3d11.h>
#if (DX_ENGINE_LEVEL >= 21 || defined USE_VIEW2D_SPRITES) && ( defined DX11 || defined DX9 /*defined DX9*/ || defined DX12 )

#include "DXshaderClass.h"
#include "fileLoader.h"

#if defined DX11 || defined DX9
#include "dx11Class.h"
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
#include "dx12Class.h"
#endif

#if D3D11_SPEC_DATE_YEAR > 2009
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#endif

#include "WinSystemClass.h"

shaderTree* shaderManager = NULL;

extern shaderTree shaderManager_40[];
extern shaderTree shaderManager_41[];
extern shaderTree shaderManager_50[];
extern shaderTree shaderManager_51[];

//-------------------------------------------------------------------------------------------------------------
#if defined DX11 || defined DX9
        // 21: SHADER_COLOR: v + Kd
        // 54: SHADER_TEXTURE_WATER:
static const D3D11_INPUT_ELEMENT_DESC colorPolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#endif
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
static const D3D12_INPUT_ELEMENT_DESC colorPolygonLayout[] =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};
#endif

//-------------------------------------------------------------------------------------------------------------
                            // 27: SHADER_TEXTURE_FONT :	//27
							// 72: SHADER_FIRE

#if defined DX11 || defined DX9
static const D3D11_INPUT_ELEMENT_DESC texturePolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#endif
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
static const D3D12_INPUT_ELEMENT_DESC texturePolygonLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,							 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};
#endif

//-------------------------------------------------------------------------------------------------------------

#if defined DX11 || defined DX9 // 36: SHADER_TEXTURE_LIGHT_RENDERSHADOW (Render: 3D + Shadow)
static const D3D11_INPUT_ELEMENT_DESC lightPolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#endif
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
static const D3D12_INPUT_ELEMENT_DESC lightPolygonLayout[] =
{
	{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};
#endif

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
static const D3D11_INPUT_ELEMENT_DESC lightNormalPolygonLayout11[] =
{
	{ "POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,	 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//+4*3
	{ "NORMAL", 0,	DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//+4*2
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//+4*3
	{ "BINORMAL", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }	//+4*3
};

//-------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
static const D3D11_INPUT_ELEMENT_DESC shadowMapPolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#endif

static const D3D11_INPUT_ELEMENT_DESC lightInstancedPolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT,1, 0,							 D3D11_INPUT_PER_INSTANCE_DATA, 1 },//Instance Position
};

static const D3D11_INPUT_ELEMENT_DESC shadowMapInstancedPolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT,1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },//Instance Position
};

static const D3D11_INPUT_ELEMENT_DESC lightNormalInstancedPolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,		D3D11_INPUT_PER_VERTEX_DATA,	0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },//Instance Position
};

//-------------------------------------------------------------------------------------------------

namespace DirectX {

	DXshaderClass::DXshaderClass(UINT ShaderVersion_H, UINT ShaderVersion_L, bool shader_3D)
	{
		CLASSLOADER();
		WomaIntegrityCheck = 1234525217;
		
#if defined DX11 || defined DX9
		m_driver11 = ((DirectX::DX11Class*)m_Driver);
#endif
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
		m_driver = ((DirectX::DX12Class*)m_Driver);
#endif

		ShaderVersionH = ShaderVersion_H;
		ShaderVersionL = ShaderVersion_L;

		//private:
#if defined DX11 || defined DX9
		m_layout11 = NULL;	//Used Vetex Description
		m_VertexShaderBuffer11 = NULL;	//Parameters: VS

		//Shader CODE:
		m_vertexShader11 = NULL;		//Code: VS
		m_pixelShader11 = NULL;			//Code: PS
#endif

		m_shaderType = SHADER_AUTO;
		shader2D = !shader_3D;

		//public:
#if defined DX11 || defined DX9
		m_PixelShaderBuffer11 = NULL;		//Parameters: PS
		m_sampleState11 = NULL;		//Sample State

		texture11 = NULL;
		texture11_2 = NULL;

		m_sampleStateClamp11 = NULL;
#endif	

#if TUTORIAL_CHAP >= 62 // FIRE
		m_sampleStateFire = 0;
#endif

		// VERTEX CBUFFER:
		// --------------------------------------------------------------------------------------------
		// BLOCK: VS1
		//-----------
		//	worldMatrix;	//worldMatrix
		//	view;			//view
		//	projection;		//projection
		//  WV;				//worldMatrix+viewMatrix
		//  WVP;			//worldMatrix+viewMatrix+projectionMatrix

		// BLOCK2: VS2
		//------------
		hasLight = false;
		hasSpecular = false;
		hasNormMap = false;
		hasFog = false;

		// BLOCK: VS3
		//-----------
		//lightDirection;	// Not Initialized
		ambientColor = XMFLOAT4(0, 0, 0, 0);;	// LIGHT: Ka
		diffuseColor = XMFLOAT4(1, 1, 1, 0);;	// LIGHT: Kd
		emissiveColor = XMFLOAT4(0, 0, 0, 0);;	// LIGHT: Ke

		// BLOCK4:
		//-----------
		fogStart = 0;
		fogEnd = 0;
		castShadow = false;

		// PIXEL CBUFFER:
		// --------------------------------------------------------------------------------------------
		// BLOCK: PS1

		// BLOCK1:
		pixelColor = XMFLOAT4(0, 0, 0, 0);

		// BLOCK2:
		hasTexture = false;
		//hasLight = false;		//repeated in VS
		//hasSpecular = false;	//repeated in VS
		isFontShader = false;

		// BLOCK3:
		//XMFLOAT4	ambientColor;	// LIGHT: Ka //repeated in VS
		//XMFLOAT4	diffuseColor;	// LIGHT: Kd //repeated in VS
		//XMFLOAT4	emissiveColor;	// LIGHT: Ke //repeated in VS
		//XMVECTOR	lightDirection;	// LIGHT	 //repeated in VS

		// BLOCK4:
		hasColorMap = false;		// 66
		lightType = 1;				// 29
		isDay = false;				
		shaderTypeParameter = 0;	// Future

		// BLOCK5:
		hasAlfaColor = false;
		alfaColor = 1;
		PSfade = 1;		// Time since Beg.
#if defined INTRO_DEMO
		frameTime = 0;	//29: Fadeout / Fadein
#endif

		// BLOCK6:
		//BOOL		hasFog;		//repeated in VS
		isSky = false;
		hasAlfaMap = false;
		//BOOL		hasNormMap;	//repeated in VS

		// BLOCK6:
		XMFLOAT3	cameraPosition;	//TODO: soon
		//BOOL		castShadow;		//repeated in VS
		specularColor = XMFLOAT3(0, 0, 0);
		nShininess = 0;

		// BLOCK7:
		VSshaderType = 0;			// 54:WaterMovement
		//float	vsPAD2;
		//float	vsPAD3;
		//float	vsPAD4;

		// --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
		m_instanceCount = NULL;
#endif

		bUseGS = false;	// GS
		// --------------------------------------------------------------------------------------------
	}

	DXshaderClass::~DXshaderClass() { CLASSDELETE(); } // Note: DxModel Call ShutDown()

#if !defined USE_PRECOMPILED_SHADERS && ( defined DX11 || defined DX9)
	void DXshaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, TCHAR* shaderFilename)
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());	// Get a pointer to the error message text buffer
		//ULONG bufferSize = (ULONG) errorMessage->GetBufferSize();			// Get the length of the message

		womalogauto(TEXT("LOADING DX SHADER ERROR: %s\n"), compileErrors);
		SAFE_RELEASE(errorMessage);	// Release the error message

		// Pop a message up on the screen to notify the user to check the text file for compile errors.
		WomaMessageBox(TEXT("Error compiling shader.  Check shader-error.txt for message."), shaderFilename);
	}
#endif

	bool DXshaderClass::Initialize(INT Id, TCHAR* objectName, SHADER_TYPE shaderType, /*ID3D11Device*/ void* device, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology, bool useGS)
	{
		bool result = false;
		m_ObjId = Id;
		m_shaderType = shaderType;
		MODEL_NAME = objectName;
        bUseGS = useGS;

		//GLOBAL:
	#ifdef USE_PRECOMPILED_SHADERS
		if (ShaderVersionH == 4 && ShaderVersionL == 0)
			shaderManager = shaderManager_40; // 4.0
		else
			if (ShaderVersionH == 4 && ShaderVersionL == 1)
				shaderManager = shaderManager_41; // 4.1
			else
				if (ShaderVersionH == 5 && ShaderVersionL == 0)
					shaderManager = shaderManager_50; // 5.0
				else
					shaderManager = shaderManager_51; // 5.1 or Future?
	#endif

		hasTexture =	(shaderType != SHADER_COLOR && 
						shaderType != SHADER_TEXTURE_LIGHT_SAVESHADOW)
						;
		result = InitializeShader(shaderType, device, hwnd, PrimitiveTopology); //LOAD: HLSL code

		return result;
	}

	void DXshaderClass::Shutdown()
	{
		womalog("DXshaderClass() DESTROYING: %s\n", MODEL_NAME.c_str());

		// 21
#if defined DX11 || defined DX9
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
			SAFE_RELEASE(m_layout11);				// Release the layout.
			SAFE_RELEASE(m_VertexShaderBuffer11);	// Release the VS constant buffer.

			SAFE_RELEASE(m_pixelShader11);			// Release the pixel shader.	
			SAFE_RELEASE(m_vertexShader11);			// Release the vertex shader.
			SAFE_RELEASE(m_PixelShaderBuffer11);	// Release the PS constant buffer.
			SAFE_RELEASE(m_sampleState11);			// Release the sampler state.
		}
#endif

	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
		SAFE_RELEASE(m_sampleStateClamp11);
	#endif
	}

	// ----------------------------------------------------------------------------------------------
	bool DXshaderClass::InitializeShader(SHADER_TYPE shaderType, void* DriverDevice, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology)
	// ----------------------------------------------------------------------------------------------
	{
		UINT numElements = 0;
		HRESULT result;

#if defined DX11 || defined DX9
		const D3D11_INPUT_ELEMENT_DESC* polygonLayout11 = NULL;
		DX11Class* driver11 = m_driver11;
#define device11 ((ID3D11Device*)DriverDevice)
#endif
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
		const D3D12_INPUT_ELEMENT_DESC* polygonLayout = NULL;
		DX12Class* driver = m_driver;
		ID3D12Device* device = (ID3D12Device*)DriverDevice;// ((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device;
#endif

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// DX 11
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// DEFINE Vertex Type
	// -------------------------------------------------------------------------------------------
	// 
	// Initialize the vertex and pixel shaders.
		switch (shaderType)
		{

		case SHADER_COLOR:
		case SHADER_TEXTURE_WATER:
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
			if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
			{
				polygonLayout = &colorPolygonLayout[0];
				numElements = _countof(colorPolygonLayout);
			}
#endif
#if defined DX11 || defined DX9
			if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
			{
				polygonLayout11 = &colorPolygonLayout11[0];
				numElements = _countof(colorPolygonLayout11);
			}
#endif
			break;


		//	float3 position		: POSITION;
		//	float2 texCoords	: TEXCOORD0; //22
		case SHADER_TEXTURE:		//22
		case SHADER_TEXTURE_FONT:	//27
		case SHADER_FIRE:			//072fire.hlsl
		case SHADER_USE_CURVED_REAL_SKY_PLANE:
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
			if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
			{
				polygonLayout = &texturePolygonLayout[0];
				numElements = _countof(texturePolygonLayout);
			}
#endif
#if defined DX11 || defined DX9
			if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
			{
				polygonLayout11 = &texturePolygonLayout11[0];
				numElements = _countof(texturePolygonLayout11);
			}
#endif
			break;

		// float3 position	: POSITION;			//21
		// float2 texCoords	: TEXCOORD;			//22
		// float3 normal	: NORMAL;			//23
		case SHADER_TEXTURE_LIGHT:				//23
		case SHADER_TEXTURE_LIGHT98:			//98
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:	//36
        case SHADER_TEXTURE_LIGHT_FAST:			//83
#if defined DX12
			if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
			{
				polygonLayout = &lightPolygonLayout[0];
				numElements = _countof(lightPolygonLayout);
			}
#endif
#if defined DX11 || defined DX9
			if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
			{
				polygonLayout11 = &lightPolygonLayout11[0];
				numElements = _countof(lightPolygonLayout11);
			}
#endif
			break;

		case SHADER_NORMAL_BUMP:
			polygonLayout11 = &lightNormalPolygonLayout11[0];
			numElements = sizeof(lightNormalPolygonLayout11) / sizeof(lightNormalPolygonLayout11[0]);
			break;
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
		case SHADER_TEXTURE_LIGHT_SAVESHADOW:
			polygonLayout11 = &shadowMapPolygonLayout11[0];
			numElements = sizeof(shadowMapPolygonLayout11) / sizeof(shadowMapPolygonLayout11[0]);	// Get a count of the elements in the layout.			
			break;
#endif

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
		case SHADER_TEXTURE_LIGHT_INSTANCED:			//40
		case SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED: //41
		case SHADER_TEXTURE_POINTS_OF_LIGHT_INSTANCED:	//98
			polygonLayout11 = &lightInstancedPolygonLayout11[0];
			numElements = sizeof(lightInstancedPolygonLayout11) / sizeof(lightInstancedPolygonLayout11[0]);
			break;
#endif
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES //40: Aux. Shader (render in texture), but using Instances
		case SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED:
			polygonLayout11 = &shadowMapInstancedPolygonLayout11[0];
			numElements = sizeof(shadowMapInstancedPolygonLayout11) / sizeof(shadowMapInstancedPolygonLayout11[0]);
			break;
#endif

		default:
			throw woma_exception("WRONG SHADER! (That shader is not supported yet)", __FILE__, __FUNCTION__, __LINE__);
		}

		std::wstring vsFilename = L"";
		std::wstring psFilename = vsFilename;
#if D3D11_SPEC_DATE_YEAR == 2009
		std::string vertexHLSL = "VS_Main";
		std::string pixelHLSL = "PS_Main";
#else
		std::string vertexHLSL = "";
		std::string pixelHLSL = "";
#endif
		//Define Procedure name to invoke on VERTEX and PIXEL SHADERs...
		switch (shaderType)
		{
		case SHADER_COLOR:
			vsFilename.append(L"hlsl/021Color.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
		case SHADER_TEXTURE:
			vsFilename.append(L"hlsl/022Texture.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
		case SHADER_TEXTURE_LIGHT:
			vsFilename.append(L"hlsl/023Light.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
		case SHADER_TEXTURE_FONT:
			vsFilename.append(L"hlsl/027Texture.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
		case SHADER_NORMAL_BUMP:
			vsFilename.append(L"hlsl/035TextureBump.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:			//36:			Draw Shadows
			vsFilename.append(L"hlsl/036LightRenderShadow.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
		case SHADER_TEXTURE_LIGHT_SAVESHADOW:			//36:			Aux. Shader (render in texture)
			vsFilename.append(L"hlsl/036ShadowMap.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES
		case SHADER_TEXTURE_LIGHT_INSTANCED:			//40: INSTANCED like 23 light, but using Instances
			vsFilename.append(L"hlsl/040LightInstance.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("VS_Main");
			pixelHLSL.append("PS_Main");
			break;
#endif
//NEW SHADER:

//93

		default:
			WomaFatalExceptionW(TEXT("This Shader type is not supported yet!"));
			break;
		};

#if _DEBUG
		WCHAR WMODEL_NAME[10 * MAX_STR_LEN] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, (char*)MODEL_NAME.c_str(), -1, WMODEL_NAME, MAX_STR_LEN);
		womalogw(L"START: INIT SHADER MODEL %s - Use HLSL [%s]\n", WMODEL_NAME, vsFilename.c_str());
#endif

#if defined DX11 || defined DX9
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
			// GET SHADER CODE:
			// --------------------------------------------------------------------------------------------
#if !defined USE_PRECOMPILED_SHADERS
			ID3D10Blob* errorMessage = NULL;
			ID3D10Blob* vertexShaderBuffer = NULL;
			ID3D10Blob* pixelShaderBuffer = NULL;

#if D3D11_SPEC_DATE_YEAR == 2009
			// Compile the vertex shader code:
			STRING vertVer = TEXT("vs_");
			vertVer.append(driverList[WOMA::AppSettings->DRIVER]->ShaderModel);  //TEXT("vs_5_0")
			result = D3DX11CompileFromFile(vsFilename.c_str(), NULL, NULL, vertexHLSL.c_str(), "vs_5_0"/*vertVer.c_str()*/, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, hwnd, (TCHAR*)vsFilename.c_str());// If the shader failed to compile it should have writen something to the error message
				else
					WomaMessageBox((TCHAR*)vsFilename.c_str(), TEXT("Missing Shader File")); // ... Then it simply could not find the shader file itself

				return false;
			}
			vertVer[0] = 'p';  //TEXT("ps_5_0")
			// Compile the pixel shader code:
			result = D3DX11CompileFromFile(psFilename.c_str(), NULL, NULL, pixelHLSL.c_str(), "ps_5_0"/*vertVer.c_str()*/, D3D10_SHADER_ENABLE_STRICTNESS,
				0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, hwnd, (TCHAR*)vsFilename.c_str());		 // If the shader failed to compile it should have writen something to the error message
				else
					WomaMessageBox((TCHAR*)vsFilename.c_str(), TEXT("Missing Shader File")); // ... Then it simply could not find the shader file itself

				return false;
			}
#else
			std::stringstream sstm;
			sstm << DX_ENGINE_LEVEL;
			std::string level = sstm.str().c_str();
			D3D_SHADER_MACRO defines[] =
			{
				"ENGINE_LEVEL", level.c_str(),
				"DXAPI11", "1",
				NULL, NULL
			};

#ifdef _DEBUG
			// Enable better shader debugging with the graphics debugging tools.
			UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#else
			UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3; // 0;
#endif

			std::string vertVer = "vs_";
#if UNICODE
			CHAR str[MAX_STR_LEN] = { 0 }; wtoa(str, (TCHAR*)driverList[WOMA::AppSettings->DRIVER]->ShaderModel, MAX_STR_LEN); // wchar ==> char
			vertVer.append(str);  //TEXT("vs_5_0")
#else
			vertVer.append(driverList[WOMA::AppSettings->DRIVER]->ShaderModel);  //TEXT("vs_5_0")
#endif
			LPCWSTR file = (LPCWSTR)WOMA::LoadFileW((WCHAR*)vsFilename.c_str());

            // VERTEX:
			result = D3DCompileFromFile(file, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, vertexHLSL.c_str(), /*"vs_5_0"*/vertVer.c_str(), compileFlags, 0, &vertexShaderBuffer, &errorMessage);
			if (FAILED(result))
			{
				if (errorMessage) {
					WomaMessageBox((TCHAR*)errorMessage->GetBufferPointer(), TEXT("SHADER Error description :"));
					womalog("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				}
				return false;
			}

            //GS:

            // PIXEL:
			vertVer[0] = 'p';  //TEXT("ps_5_0")
			result = D3DCompileFromFile(file, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pixelHLSL.c_str(), /*"ps_5_0"*/vertVer.c_str(), compileFlags, 0, &pixelShaderBuffer, &errorMessage);
			if (FAILED(result))
			{
				WomaMessageBox((TCHAR*)errorMessage->GetBufferPointer(), TEXT("SHADER Error description :"));
				if (errorMessage)
					womalog("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				return false;
			}
#endif

			// Create the vertex shader from the buffer: (LOAD & COMPILE EXTERNAL FILE)
			result = device11->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader11);
			if (FAILED(result))
				{ WomaMessageBox(TEXT("ERROR"), TEXT("CreateVertexShader:")); return false; }

			// Create the pixel shader from the buffer: (LOAD & COMPILE EXTERNAL FILE)
			result = device11->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader11);
			if (FAILED(result))
				{ WomaMessageBox(TEXT("ERROR"), TEXT("pixelShaderBuffer:")); return false; }

			// Create the vertex input layout.
#if !defined USE_PRECOMPILED_SHADERS
			result = device11->CreateInputLayout(polygonLayout11, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout11);
#else
			result = device11->CreateInputLayout(polygonLayout11, numElements, shaderManager[shaderType].blobVS, shaderManager[shaderType].sizeVS, &m_layout);
#endif
			IF_FAILED_RETURN_FALSE(result);

			// Release the vertex shader buffer and pixel shader buffer since they are no longer needed:
			SAFE_RELEASE(vertexShaderBuffer);
			SAFE_RELEASE(pixelShaderBuffer);
#else
			// Create the Vertex Shader from the buffer: (GET CODE ON "EXE")
			result = device11->CreateVertexShader(shaderManager[shaderType].blobVS, shaderManager[shaderType].sizeVS, NULL, &m_vertexShader);
			if (FAILED(result)) { WomaFatalExceptionW(TEXT("Error: CreateVertexShader")); /*return false;*/ }

			// Create the Pixel Shader from the buffer: (GET CODE ON "EXE")
			result = device11->CreatePixelShader(shaderManager[shaderType].blobPS, shaderManager[shaderType].sizePS, NULL, &m_pixelShader);
			if (FAILED(result)) { WomaFatalExceptionW(TEXT("Error: CreatePixelShader")); /*return false;*/ }

			// GS
			if (bUseGS)
			{   // Create the Geometry Shader from the buffer.
				result = device11->CreateGeometryShader(shaderManager[shaderType].blobGS, shaderManager[shaderType].sizeGS, NULL, &m_geometryShader);
				if (FAILED(result)) { WomaFatalExceptionW(TEXT("Error: CreateGeometryShader")); /*return false;*/ }
			}
			// GS
#endif
		}
#endif

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DX 12
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// [*] Describe and create a shader resource view (SRV) heap for the texture.

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
		{

			//
			// Create a descriptor heap for the constant buffers.
			//
			D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
			//
			// <SYNC> SHADER IMPUT BLOCKS: DXmodelClass.cpp
			//
			switch (m_shaderType)
			{
			case SHADER_COLOR:
				// SHADER_COLOR:
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				srvHeapDesc.NumDescriptors = 1; // = Num. InitAsDescriptorTable: D3D12_DESCRIPTOR_RANGE_TYPE_CBV
				break;

			case SHADER_TEXTURE:		//22
			{
				//1 + N.of Textures:
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				// |1| DescriptorTable  | t0				|
				// ...
				// |10| DescriptorTable | t9				|
				srvHeapDesc.NumDescriptors = 1 + 10; //NOTE: Allow max: 10 Texture per object in DX12... if need more need to increase here!
				break;
			}
 
			case SHADER_TEXTURE_FONT:	//27
			{
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				// |1| DescriptorTable  | t0				|
				// |2| DescriptorTable  | b1				|
									 
				srvHeapDesc.NumDescriptors = 3;
				break;
			}

			case SHADER_TEXTURE_LIGHT:
			case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
			{
				#if !defined RENDER_OBJ_WITH_ALFA
					// | Root Signature		| Shader Registers	|
					// |0| DescriptorTable  | b0				|
					// |1| DescriptorTable  | b1				|
					// |2| DescriptorTable  | t0				|
					srvHeapDesc.NumDescriptors = 3 + 10; //NOTE: Allow max: 10 Texture per object in DX12... if need more need to increase here!
				#else
					// | Root Signature		| Shader Registers	|
					// |0| DescriptorTable  | b0				|
					// |1| DescriptorTable  | b1				|
					// |2| DescriptorTable  | t0 AlfaMap		|
					// |3| DescriptorTable  | t1 Texture		|
					srvHeapDesc.NumDescriptors = 4 + 10; //NOTE: Allow max: 10 Texture per object in DX12... if need more need to increase here!
				#endif
				break;
			}
			default:
				throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
			}

			srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			// IF Failed ?: | Root Signature		| Shader Registers	|   ---> don't match with HLSL code:
			result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&DX12mSrvDescriptorHeap));
			if (FAILED(result))
			{
				WomaMessageBox(TEXT("CreateDescriptorHeap"), TEXT("DX12 ERROR:"));
				ThrowIfFailed(result);
			}

#ifdef _DEBUG
			DX12mSrvDescriptorHeap->SetName(L"Constant Buffer(s) View Descriptor Heap");
#endif

			m_CbvSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			// [*] Create root Signature - START:
			// ----------------------------------------------------------------------------
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			//CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc; //NEW!

			D3D12_STATIC_SAMPLER_DESC sampler = {};

			switch (shaderType)
			{
			case SHADER_COLOR:
				break;
			case SHADER_TEXTURE:					// ENGINE_LEVEL 22
			case SHADER_TEXTURE_LIGHT:				// ENGINE_LEVEL 23
			case SHADER_TEXTURE_FONT:
			case SHADER_TEXTURE_LIGHT_RENDERSHADOW:	// ENGINE_LEVEL 45
			#if defined TEX_MODE_BORDER
				sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			#else
				sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			#endif
				sampler.MipLODBias = 0;
				sampler.MaxAnisotropy = 0;
				sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
				sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
				sampler.MinLOD = 0.0f;
				sampler.MaxLOD = D3D12_FLOAT32_MAX;
				sampler.ShaderRegister = 0;
				sampler.RegisterSpace = 0;
				sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				break;

			default:
				throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
			}

			switch (shaderType)
			{
			case SHADER_COLOR:
			{
				// SHADER_COLOR:
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|

				CD3DX12_DESCRIPTOR_RANGE ranges[1];
				//b0
				ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// CBV: Constant Buffer View: VERTEX SHADER Constants

				CD3DX12_ROOT_PARAMETER rootParameters[1];
				rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);

				// rootSignatureFlags: Allow input layout and deny uneccessary access to certain pipeline stages.
				D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
					D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
/*
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
*/
				int numStaticSamplers = 0;
				rootSignatureDesc.Init(_countof(rootParameters), rootParameters, numStaticSamplers, nullptr, rootSignatureFlags);
				break;
			}

			case SHADER_TEXTURE:		//22
			{
					// SHADER_TEXTURE_LIGHT
					// | Root Signature		| Shader Registers	|
					// |0| DescriptorTable  | b0				|
					// |1| DescriptorTable  | b1				|
					// |2| DescriptorTable  | t0				|<-- HERE: 2+
					// ...
					// |10| DescriptorTable | t9				|
				CD3DX12_DESCRIPTOR_RANGE ranges[2];
				//b0
				ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// CBV: Constant Buffer View: VERTEX SHADER Constants
				//t0
				ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// SRV: Shader Resource View: TEXTURE

				CD3DX12_ROOT_PARAMETER rootParameters[2];
				rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
				rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);

				//rootParameters[2].InitAsConstants(1, 0, 0, D3D12_SHADER_VISIBILITY_PIXEL);				// Set UINT CONSTANT (test)

				// rootSignatureFlags: Allow input layout and deny uneccessary access to certain pipeline stages.
				D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

				int numStaticSamplers = 1; //SamplerState SampleType : register(s0); //3D (default) WRAP
				// [*] Init Root Signature:
				rootSignatureDesc.Init(_countof(rootParameters), rootParameters, numStaticSamplers, &sampler, rootSignatureFlags);
				//rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, numStaticSamplers, &sampler, rootSignatureFlags); //NEW
				break;
			}

			case SHADER_TEXTURE_FONT:	//27
			{
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|buffer
				// |1| DescriptorTable  | b1				|buffer
				// |2| DescriptorTable  | t0				|texture

				CD3DX12_DESCRIPTOR_RANGE ranges[3];
				//b0
				ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// CBV: Constant Buffer View: VERTEX SHADER Constants
				//t0
				ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// SRV: Shader Resource View: TEXTURE
				//b1
				ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// CBV: Constant Buffer View: PIXEL SHADER Constants

				CD3DX12_ROOT_PARAMETER rootParameters[3];
				rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
				rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
				rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_PIXEL);

				// Allow input layout and deny uneccessary access to certain pipeline stages.
				D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

				rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);
				break;
			}

			case SHADER_TEXTURE_LIGHT:
			case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
			{
			#if !defined RENDER_OBJ_WITH_ALFA
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|buffer		VERTEX
				// |1| DescriptorTable  | b1				|buffer		PIXEL
				// |2| DescriptorTable  | t0				|texture	PIXEL
				
				CD3DX12_DESCRIPTOR_RANGE ranges[3];
				//b0
				ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// CBV0: Constant Buffer View: VERTEX SHADER Constants
				//#if DX_ENGINE_LEVEL < 31
				//b1
				ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// CBV1: Constant Buffer View: PIXEL SHADER Constants
				//t0
				ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// SRV0: Shader Resource View: TEXTURE

				//#else
				////b1
				//ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// CBV: Constant Buffer View: PIXEL SHADER Constants
				////t0
				//ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// SRV: Shader Resource View: TEXTURE
				//#endif
				CD3DX12_ROOT_PARAMETER rootParameters[3];
				rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
				rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
				rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_PIXEL);
			#else
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|buffer
				// |1| DescriptorTable  | b1				|buffer
				// |2| DescriptorTable  | t0				|alfa
				// |3| DescriptorTable  | t1				|texture

				CD3DX12_DESCRIPTOR_RANGE ranges[4];
				//b0
				ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// CBV: Constant Buffer View: VERTEX SHADER Constants
				//b1
				ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// CBV: Constant Buffer View: PIXEL SHADER Constants
				//t0
				ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// SRV: Shader Resource View: TEXTURE
				//t1
				ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	// SRV: Shader Resource View: TEXTURE ALFA

				CD3DX12_ROOT_PARAMETER rootParameters[4];
				rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
				rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
				rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_PIXEL);
				rootParameters[3].InitAsDescriptorTable(1, &ranges[3], D3D12_SHADER_VISIBILITY_PIXEL);
			#endif

				// Allow input layout and deny uneccessary access to certain pipeline stages.
				D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

				rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);
				break;
			}
			default:
				throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
			}

			ComPtr<ID3DBlob> error;
			ComPtr<ID3DBlob> signature;
			result = D3D12SerializeRootSignature(&rootSignatureDesc, driver->featureData.HighestVersion, &signature, &error);
			if (FAILED(result))
			{
				WomaMessageBox(TEXT("D3D12SerializeRootSignature"), TEXT("DX12 ERROR:"));
				womalogauto((char*)error->GetBufferPointer());
				//OutputDXError(error.Get());
				//ThrowIfFailed(result);
				return false;
			}

			// [*] Create Root Signature:
			result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
			if (FAILED(result))
			{
				WomaMessageBox(TEXT("CreateRootSignature"), TEXT("DX12 ERROR:"));
				char* compileErrors = (char*)(error->GetBufferPointer());
#if defined UNICODE
				TCHAR WcompileErrors[MAX_STR_LEN] = { 0 };	atow(WcompileErrors, compileErrors, MAX_STR_LEN);
				WomaMessageBox(WcompileErrors, TEXT("DX12 ERROR: CreateRootSignature"));
#else
				WomaMessageBox(compileErrors, TEXT("DX12 ERROR: CreateRootSignature"));
#endif
				ThrowIfFailed(result);
			}

			if (!(polygonLayout && numElements && m_rootSignature))
			{
				WomaMessageBox(TEXT("ASSERT(polygonLayout && numElements && m_rootSignature)"), TEXT("DX12 ERROR:"));
				ASSERT(polygonLayout && numElements && m_rootSignature);
			}

			//
			// LoadAssets()- PARTE 2 ----> bool DXshaderClass::InitializeShader()
			//
			// ------------------------------------------------------------------------------
			// [*] Describe and create the graphics pipeline state object (PSO).
			// ------------------------------------------------------------------------------
			D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc = {};

			opaquePsoDesc.InputLayout = { polygonLayout, numElements };
			opaquePsoDesc.pRootSignature = m_rootSignature.Get();

#if !defined USE_PRECOMPILED_SHADERS
	#ifdef _DEBUG
			// Enable better shader debugging with the graphics debugging tools.
			UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
	#else
			UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
	#endif

			ComPtr<ID3DBlob> vertexShader;
			ComPtr<ID3DBlob> pixelShader;
			ID3DBlob* errorMessage = NULL;

			std::stringstream sstm;
			sstm << DX_ENGINE_LEVEL;
			std::string level = sstm.str().c_str();
			D3D_SHADER_MACRO defines[] =
			{
				"ENGINE_LEVEL", level.c_str(),
				"DXAPI12", "1",
				NULL, NULL
			};

			// NOTE! The run time compiler support only Shader 5.0, for more use: USE_PRECOMPILED_SHADERS option 
			//std::string vertVer = "vs_"; //cant be: STRING
			//vertVer.append(driverList[WOMA::AppSettings->DRIVER]->szShaderModel);  //TEXT("vs_5_0")
			//vertVer[4] = '_';  //TEXT("vs_5_0")
			result = D3DCompileFromFile(vsFilename.c_str(), defines/*nullptr*/, D3D_COMPILE_STANDARD_FILE_INCLUDE, vertexHLSL.c_str(), ("vs_5_0")/*vertVer.c_str()*/, compileFlags, 0, &vertexShader, &errorMessage);
			if (FAILED(result))
			{
				if (errorMessage)
					womalog("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				return false;
			}

			//vertVer[0] = 'p';  //TEXT("ps_5_0")
			result = D3DCompileFromFile(psFilename.c_str(), defines/*nullptr*/, D3D_COMPILE_STANDARD_FILE_INCLUDE, pixelHLSL.c_str(), ("ps_5_0")/*vertVer.c_str()*/, compileFlags, 0, &pixelShader, &errorMessage);
			if (FAILED(result))
			{
				if (errorMessage)
					womalog("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				return false;
			}

			//------------------------------------------------------------------------------------------------------------------------------------------------------
			//
			// RasterizerStates
			//

			opaquePsoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
			opaquePsoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
#else
			opaquePsoDesc.VS = { shaderManager[shaderType].blobVS, shaderManager[shaderType].sizeVS };
			opaquePsoDesc.PS = { shaderManager[shaderType].blobPS, shaderManager[shaderType].sizePS };
#endif


			//------------------------------------------------------------------------------------------------------------------------------------------------------
			//
			// Blend State
			//
			opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

			//
			// Depth Stencil View
			//
	#if !defined USE_DSV//24
			opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			opaquePsoDesc.DepthStencilState.DepthEnable = FALSE;
			opaquePsoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			opaquePsoDesc.DepthStencilState.StencilEnable = FALSE;
			opaquePsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	#else
			opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	#endif

			opaquePsoDesc.SampleMask = UINT_MAX;
			if (PrimitiveTopology == LINELIST)
				opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			else
				opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

			opaquePsoDesc.NumRenderTargets = 1;
			opaquePsoDesc.RTVFormats[0] = driver->BUFFER_COLOR_FORMAT; // DXGI_FORMAT_B8G8R8A8_UNORM;
			opaquePsoDesc.SampleDesc.Count = driver->MSAA_COUNT;	//opaquePsoDesc.SampleDesc.Count = 1;
			opaquePsoDesc.SampleDesc.Quality = driver->MSAA_QUALITY;
			opaquePsoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; //NOTE: This format (DXGI_FORMAT_D32_FLOAT) can be used with/without DSV, and this one DXGI_FORMAT_D24_UNORM_S8_UINT (CANT!)


#if defined USE_RASTERIZER_STATE
			for (UINT zBufferMode = 0; zBufferMode < 2; zBufferMode++)
			{
				if (zBufferMode == 1)
					opaquePsoDesc.DepthStencilState.DepthEnable = TRUE;

				for (UINT cullMode = 0; cullMode < 3; cullMode++)			//0..2
				{
					for (UINT fillMode = 0; fillMode < 2; fillMode++)		//0..1
					{
						opaquePsoDesc.RasterizerState = driver->m_rasterState[cullMode][fillMode];
						//EQUAL TO:   device->CreateGraphicsPipelineState(&opaquePsoDesc, IID_ID3D12PipelineState, &m_pipelineState[zBufferMode][cullMode][fillMode][SOLID_PIPELINE_STATES]);
						ThrowIfFailed(device->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&m_pipelineState[zBufferMode][cullMode][fillMode][SOLID_PIPELINE_STATES])));
					}
				}
			}
#endif

#if defined USE_ALPHA_BLENDING
			//
			// BlendState & DepthStencil: TRANSPARENT
			//
			D3D12_GRAPHICS_PIPELINE_STATE_DESC transparentPsoDesc = opaquePsoDesc;

			D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
			transparencyBlendDesc.BlendEnable = true;
			transparencyBlendDesc.LogicOpEnable = false;
			transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
			transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
			transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
			transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
			transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			transparentPsoDesc.BlendState.RenderTarget[0] = transparencyBlendDesc;

			//
			// RasterizerStates
			//
			for (UINT zBufferMode = 0; zBufferMode < 2; zBufferMode++)
			{
				if (zBufferMode == 1)
					transparentPsoDesc.DepthStencilState.DepthEnable = TRUE;

				for (UINT cullMode = 0; cullMode < 3; cullMode++)			//0..2
				{
					for (UINT fillMode = 0; fillMode < 2; fillMode++)		//0..1
					{
						transparentPsoDesc.RasterizerState = driver->m_rasterState[cullMode][fillMode];
						//EQUAL TO:   device->CreateGraphicsPipelineState(&transparentPsoDesc, IID_ID3D12PipelineState, &m_pipelineState[cullMode][fillMode][TRANSPARENT_PIPELINE_STATES]);
						ThrowIfFailed(device->CreateGraphicsPipelineState(&transparentPsoDesc, IID_PPV_ARGS(&m_pipelineState[zBufferMode][cullMode][fillMode][TRANSPARENT_PIPELINE_STATES])));
					}
				}
			}
#endif

			//------------------------------------------------------------------------------------------------------------------------------------------------------


			// ------------------------------------------------------------------------------
			// Create the constant buffer(s).
			// ------------------------------------------------------------------------------
			{
				//
				// CreateConstantBufferView(): Describe and create a constant buffer view.
				//
				c_alignedVSConstantBufferSize = (sizeof(VSconstantBufferType) + 255) & ~255;

				// [*] CreateCommittedResource - ALOCATE SPACE ON GPU RAM for VS
				ThrowIfFailed(device->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)
					, D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(c_alignedVSConstantBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&mVS_constantBuffer)));

				//
				// MAP the virtual GPU RAM
				//
				mVS_constantBuffer->SetName(L"VS Constant Buffer");
				CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
				ThrowIfFailed(mVS_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pMappedVSConstantBuffer))); // Pointer to RAM (virtual to GPU ConstantBufferView)

				switch (shaderType)
				{
				case SHADER_COLOR:
				case SHADER_TEXTURE:
				{
					D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc[1] = {};
					//[0]
					cbvDesc[0].BufferLocation = mVS_constantBuffer->GetGPUVirtualAddress();
					cbvDesc[0].SizeInBytes = c_alignedVSConstantBufferSize; 	// CB size is required to be 256-byte aligned.

					// CBV
					CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle0(DX12mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 0, 0);
					device->CreateConstantBufferView(&cbvDesc[0], cbvHandle0);
				}
				break;

				//case SHADER_TEXTURE:
				case SHADER_TEXTURE_FONT:
				case SHADER_TEXTURE_LIGHT:
				case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
				{
					//
					// CreateConstantBufferView(): Describe and create a constant buffer view.
					//
					c_alignedPSConstantBufferSize = (sizeof(PSconstantBufferType) + 255) & ~255;

					// [*] CreateCommittedResource - ALOCATE SPACE ON GPU RAM for PS
					ThrowIfFailed(device->CreateCommittedResource(
						&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)
						, D3D12_HEAP_FLAG_NONE,
						&CD3DX12_RESOURCE_DESC::Buffer(c_alignedPSConstantBufferSize),
						D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr,
						IID_PPV_ARGS(&mPS_constantBuffer)));

					//
					// MAP the virtual GPU RAM
					//
					mPS_constantBuffer->SetName(L"PS Constant Buffer");
					CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
					ThrowIfFailed(mPS_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pMappedPSConstantBuffer))); // Pointer to RAM (virtual to GPU ConstantBufferView)

					//SHADER_TEXTURE_FONT
					// | Root Signature		| Shader Registers	|
					// |0| DescriptorTable  | b0				| <-- HERE [0]
					// |1| DescriptorTable  | t0				| --> DXmodelClass::InitializeDXbuffers		1
					// |2| DescriptorTable  | b1				| <-- HERE [2]

					//SHADER_TEXTURE_LIGHT < 31
					// | Root Signature		| Shader Registers	|
					// |0| DescriptorTable  | b0				| <-- HERE [0]
					// |1| DescriptorTable  | b1				| <-- HERE [2]
					// |2| DescriptorTable  | t0				| --> DXmodelClass::InitializeDXbuffers		2

					D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc[2] = {};
					//---------------------
					//[0] Constant Buffer:1
					cbvDesc[0].BufferLocation = mVS_constantBuffer->GetGPUVirtualAddress();
					cbvDesc[0].SizeInBytes = c_alignedVSConstantBufferSize; 	// CB size is required to be 256-byte aligned.

					CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle0(DX12mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 0, 0); // C0 at: 0
					device->CreateConstantBufferView(&cbvDesc[0], cbvHandle0);

					//---------------------
					//[1] Constant Buffer:2
					cbvDesc[1].BufferLocation = mPS_constantBuffer->GetGPUVirtualAddress();
					cbvDesc[1].SizeInBytes = c_alignedPSConstantBufferSize; 	// CB size is required to be 256-byte aligned.

					int slot = (shaderType == SHADER_TEXTURE_FONT) ? 2 : 1;

					CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle2(DX12mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_CbvSrvDescriptorSize, slot); // C1 at: 2
					device->CreateConstantBufferView(&cbvDesc[1], cbvHandle2);

					break;
				}
				default:
					throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
				}

			}
		}
#endif

		// --------------------------------------------------------------------------------------------
		// Create/Setup Sampler State:
		// --------------------------------------------------------------------------------------------
		// [1]: dont change the order:
#if defined DX11 || defined DX9
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			if (m_shaderType > SHADER_COLOR)
			{
				{
					if (shader2D)
					{   //2D
						samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
					}
					else
					{   //3D
						samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
					}

					samplerDesc.BorderColor[0] = 1;
					samplerDesc.BorderColor[1] = 1;
					samplerDesc.BorderColor[2] = 1;
					samplerDesc.BorderColor[3] = 1;

					/*
					--------------------------------------------------------------------------------------------
					1    Point filtering (least expensive, worst visual quality)    D3D11_FILTER_MIN_MAG_MIP_POINT;
					2    Bilinear filtering                                         D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
					3    Trilinear filtering                                        D3D11_FILTER_MIN_MAG_MIP_LINEAR;
					4    Anisotropic filtering (most expensive, best visual quality)sampler_description.Filter = D3D11_FILTER_ANISOTROPIC;
																					sampler_description.MaxAnisotropy = 16; (Valid values are between 1 and 16)
					*/

					// Defaults:
					samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
					samplerDesc.MipLODBias = 0.0f;
					samplerDesc.MinLOD = 0.0f;
					samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;								// hasMipmaps(filter)? D3D11_FLOAT32_MAX : 0;

					samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;				// Point filtering: Faster
					samplerDesc.MaxAnisotropy = 1;

					if (WOMA::AppSettings->MSAA_bilinear)
						samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT; // Bilinear: (Rastertek default)

					else if (WOMA::AppSettings->MSAA_trilinear)
						samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		// Trilinear: 2x

					else if (WOMA::AppSettings->MSAA_Anisotropic) {
						samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;				// Anisotropic: 4x, 8x, 16x
						samplerDesc.MaxAnisotropy = MAX (1, MIN(WOMA::AppSettings->MSAA_AnisotropicLevel,16)); //value: between 1 and 16.
					}
				}

				// Create the texture sampler state:
				result = device11->CreateSamplerState(&samplerDesc, &m_sampleState11);
				if (FAILED(result)) { WomaFatalException ("CreateSamplerState error"); } //dont use TEXT!
			}
			// [2]: dont change the order:
#if TUTORIAL_CHAP >= 62 // FIRE
			if (m_shaderType == SHADER_FIRE)
			{
				D3D11_SAMPLER_DESC samplerDescFire;

				//FIRE: Create a second texture sampler state description for a Clamp sampler.
				samplerDescFire = samplerDesc;
				samplerDescFire.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDescFire.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDescFire.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

				// Create the texture sampler state.
				result = device11->CreateSamplerState(&samplerDescFire, &m_sampleStateFire);
				if (FAILED(result)) { WomaFatalException("error"); return false; }
			}
#endif

			// [3]: dont change the order:
			if (m_shaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW || 
				m_shaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW ||
				m_shaderType == SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED || 
				m_shaderType == SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED || 
				/*m_shaderType == SHADER_Terrain_Texture_DEMO61 ||*/
				m_shaderType == SHADER_Terrain_Texture_DEMO99
			)
		{
			// Create a clamp texture sampler state description.
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

			samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MaxAnisotropy = 1; // ignored for comparison samplers

			// Create the texture sampler state.
			result = device11->CreateSamplerState(&samplerDesc, &m_sampleStateClamp11);
			if (FAILED(result)) { WomaFatalException ("CreateSamplerState error"); }
		}

			// --------------------------------------------------------------------------------------------
			// CREATE unified Constant Buffer for "cbufferONE.hlsli"
			// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// LEGACY PATH – for backward shaders before level 99
// --------------------------------------------------------------------------------------------

			// --------------------------------------------------------------------------------------------
			// CREATE Buffer(s) DATA for "Vertex Shader"
			// --------------------------------------------------------------------------------------------
			// Setup the description of the dynamic matrix constant buffer that is in the vertex shader:
			D3D11_BUFFER_DESC BufferDesc = { 0 }; // ZeroMemory( &BufferDesc, sizeof( BufferDesc ) );

			BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;				// We want to "re-write" this
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// We want to "re-write" this
			//BufferDesc.MiscFlags = 0;
			//BufferDesc.StructureByteStride = 0;

			BufferDesc.ByteWidth = sizeof(VSconstantBufferType);
			ASSERT(BufferDesc.ByteWidth <= D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT && (BufferDesc.ByteWidth % 16) == 0); // Validate Size

			result = device11->CreateBuffer(&BufferDesc, NULL, &m_VertexShaderBuffer11);
			IF_FAILED_RETURN_FALSE(result);

			// --------------------------------------------------------------------------------------------
			// CREATE Buffer(s) DATA for "Pixel Shader":
			// --------------------------------------------------------------------------------------------
			if (m_shaderType > SHADER_COLOR)
			{
				BufferDesc.ByteWidth = sizeof(PSconstantBufferType);
				ASSERT(BufferDesc.ByteWidth <= D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT && (BufferDesc.ByteWidth % 16) == 0); // Validate Size

				result = device11->CreateBuffer(&BufferDesc, NULL, &m_PixelShaderBuffer11);
				IF_FAILED_RETURN_FALSE(result);
			}
		}
#endif

		//womalog(L"DONE: INIT SHADER MODEL %s - Use HLSL [%s]\n", WMODEL_NAME, vsFilename.c_str());
		return true;
	}

	// ----------------------------------------------------------------------------------------
	void DXshaderClass::SetShaderParameters(UINT pass, void* Device_Context,
		XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix,
		XMMATRIX* lightViewMatrix, XMMATRIX* ShadowProjectionMatrix, float m_particleAlpha)
		// ----------------------------------------------------------------------------------------
	{
		HRESULT result;
		ID3D11DeviceContext* deviceContext11 = ((ID3D11DeviceContext*)Device_Context);
		#define device11 ((ID3D11Device*)m_driver11->m_device11)

// END 99 TYPEs

// END: SHADER_BILLBOARD_ATLAS_FAST

//END: SHADER_USE_WATERFALL

	// ----------------------------------------------------------------------------
	// ALL LEGACY <99 SHADERS:
	// ----------------------------------------------------------------------------
	#if defined DX12 || defined DX11 || defined DX9 /*defined DX9*/
		VSconstantBufferType* dataVSptr = NULL;		// Reset Pointer, only once:
	#endif
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
			dataVSptr = &mVS_constantBufferData;	// Pointer to Static Buffer
	#endif

	#if defined DX11 || defined DX9
		D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
			// VERTEX SHADER: Lock the constant buffer so it can be written to
			// ----------------------------------------------------------------------------
			result = deviceContext11->Map(m_VertexShaderBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result)) { throw woma_exception("deviceContext->Map()", __FILE__, __FUNCTION__, __LINE__); }

			dataVSptr = (VSconstantBufferType*)mappedResource.pData;	// Get a pointer to the data in the constant buffer.
		}
	#endif

		LightClass* light = SystemHandle->m_Application->app_Light;

		// BOTH: DX11 and DX12

		// Copy the matrices into the constant buffer.
		dataVSptr->world = XMMatrixTranspose(*worldMatrix);

		dataVSptr->view = XMMatrixTranspose(*viewMatrix);
		dataVSptr->projection = XMMatrixTranspose(*projectionMatrix);

		XMMATRIX WV = (*worldMatrix) * (*viewMatrix);
		dataVSptr->WV = XMMatrixTranspose(WV);							// Pre compute WV to reuse in all Vertices
		dataVSptr->WVP = XMMatrixTranspose(WV * (*projectionMatrix));	// Pre compute WVP to reuse in all Vertices

		// BLOCK: VS2
		dataVSptr->VShasLight = hasLight;
		dataVSptr->VShasSpecular = hasSpecular;
		dataVSptr->VShasNormMap = hasNormMap; //BUMPMAP
		dataVSptr->VShasFog = hasFog;

		// BLOCK: VS3
		if (hasLight)
		{
			dataVSptr->VSlightDirection = light->m_lightDirection;

			if (m_shaderType < SHADER_TEXTURE_LIGHT_SAVESHADOW || m_shaderType >= SHADER_Slope_Texture_Terrain)
			{
				dataVSptr->VSambientColor = light->m_ambientColor;		// Use Global: Light
				dataVSptr->VSdiffuseColor = light->m_diffuseColor;
			}
			else {
				dataVSptr->VSambientColor = ambientColor;				// Use Material Light
				dataVSptr->VSdiffuseColor = diffuseColor;
			}

			dataVSptr->VSemissiveColor = emissiveColor;
		}

		// BLOCK: VS4

		dataVSptr->VShasShadowMap = castShadow;

		dataVSptr->VS_USE_WVP = VS_USE_WVP;

		// BLOCK: VS5
		// disable shadow transform for shadow pass or when both pointers are NULL
		if ((!lightViewMatrix && !ShadowProjectionMatrix) || pass == PASS_SHADOWS)
			castShadow = false;

		// terrain also needs shadows
		if ((castShadow || m_shaderType == SHADER_Terrain_Texture_DEMO99) &&
			lightViewMatrix && ShadowProjectionMatrix)
		{
			dataVSptr->ViewToLightProj = XMMatrixTranspose((*lightViewMatrix) * (*ShadowProjectionMatrix));
		}

#if TUTORIAL_CHAP >= 62 // FIRE
		if (m_shaderType == SHADER_FIRE)
		{
			dataVSptr->vsframeTime = shaderfireframeTime;
			dataVSptr->scrollSpeeds = scrollSpeeds;
			dataVSptr->scales = scales;
		}
#endif

        dataVSptr->vsIsSky = isSky;
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
		{
			// Update the constant buffer resource:
			memcpy(m_pMappedVSConstantBuffer, &mVS_constantBufferData, sizeof(mVS_constantBufferData));
		}
#endif

#if defined DX11 || defined DX9
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
			deviceContext11->Unmap(m_VertexShaderBuffer11, 0);						// Unlock the constant buffer.
			deviceContext11->VSSetConstantBuffers(0, 1, &m_VertexShaderBuffer11);	// Finally set the "Constant" buffer in the vertex shader with the updated values.
		}
#endif

		if (m_shaderType == SHADER_COLOR || 
            m_shaderType == SHADER_TEXTURE_LIGHT_FAST 
			|| m_shaderType == SHADER_Terrain_Texture_DEMO61
            )
			return;

		// --------------------------------------------------------------------------------------------
		// PIXEL SHADER: will need to have access to some variables also (Texturing / light parameters)
		// --------------------------------------------------------------------------------------------
		PSconstantBufferType* dataPSptr = NULL; // Use a Unique ConstantBuffer

#if defined DX11 || defined DX9
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
			// Lock the constant buffer so it can be written to.
			result = deviceContext11->Map(m_PixelShaderBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result)) { throw woma_exception("deviceContext->Map()", __FILE__, __FUNCTION__, __LINE__); }

			dataPSptr = (PSconstantBufferType*)mappedResource.pData;	// Get a pointer to the data in the constant buffer.
		}
#endif

#ifdef DX12
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
			dataPSptr = &mPS_constantBufferData; // Pointer to Static Buffer
#endif

		// ----------------------------------------------------------------------------
			// BLOCK1:
		dataPSptr->pixelColor = pixelColor;	// SET THE PIXEL COLOR IN PIXEL SHADER

		// BLOCK2:
		dataPSptr->hasTexture = hasTexture; // NO? use pixelColor
		dataPSptr->hasLight = hasLight;
		dataPSptr->hasSpecular = hasSpecular;
		dataPSptr->isFont = isFontShader;

		// BLOCK3:
		if (hasLight)
		{
			dataPSptr->lightDirection = light->m_lightDirection;

			if (m_shaderType < SHADER_TEXTURE_LIGHT_SAVESHADOW || m_shaderType >= SHADER_Slope_Texture_Terrain)
			{
				dataPSptr->ambientColor = light->m_ambientColor; // Use Global Light
				dataPSptr->diffuseColor = light->m_diffuseColor;
			}
			else {
				dataPSptr->ambientColor = ambientColor;			// Use Material Light
				dataPSptr->diffuseColor = diffuseColor;
				dataPSptr->emissiveColor = emissiveColor;
			}
		}

		// BLOCK4:
		dataPSptr->hasColorMap = hasColorMap;
		dataPSptr->lightType = lightType;
		dataPSptr->isDay = isDay;
		dataPSptr->shaderTypeParameter = shaderTypeParameter;

		// BLOCK5:
		dataPSptr->hasAlfaColor = hasAlfaColor;
		dataPSptr->alfaColor = alfaColor;
		dataPSptr->fade = PSfade;	// BASE TEXTURE!
#if defined INTRO_DEMO
		dataPSptr->frameTime = frameTime;		// dT
#endif

		// BLOCK6:
		dataPSptr->hasFog = hasFog;
		dataPSptr->isSky = isSky;
		dataPSptr->hasAlfaMap = hasAlfaMap;
		dataPSptr->hasNormMap = hasNormMap;

		// BLOCK7:
		// cameraPosition	// NOT USED!
		dataPSptr->castShadow = castShadow;
#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS // Shininess + Specular
		if (hasSpecular)
		{
			dataPSptr->specularColor = specularColor;
			dataPSptr->nShininess = nShininess;
		}
#endif

#if TUTORIAL_CHAP >= 62 // FIRE
		if (m_shaderType == SHADER_FIRE)
		{
			dataPSptr->distortion1 = distortion1;
			dataPSptr->distortion2 = distortion2;
			dataPSptr->distortion3 = distortion3;
			dataPSptr->distortionScale = distortionScale;
			dataPSptr->distortionBias = distortionBias;
		}
#endif


		// ----------------------------------------------------------------------------
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009 && DX_ENGINE_LEVEL >= 23
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
		{
			if (m_shaderType >= SHADER_TEXTURE_FONT)
			{
				// Update the constant buffer resource:
				memcpy(m_pMappedPSConstantBuffer, &mPS_constantBufferData, sizeof(mPS_constantBufferData));
			}
		}
#endif

#if defined DX11 || defined DX9
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
			deviceContext11->Unmap(m_PixelShaderBuffer11, 0);	// Unlock the constant buffer.
			deviceContext11->PSSetConstantBuffers(0, 1, &m_PixelShaderBuffer11);

		}
#endif

}

void DXshaderClass::RenderShader(UINT pass, void* Device_Context, int texture_index, int indexCount, int start)
	{
#if defined DX11 || defined DX9
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		{
#define deviceContext ((ID3D11DeviceContext*)Device_Context)
			deviceContext->IASetInputLayout(m_layout11);					// Set the vertex input layout

			if (m_shaderType >= SHADER_TEXTURE)
			{
					deviceContext->PSSetSamplers(0, 1, &m_sampleState11);		// Set the Sampler state in the pixel shader (Bilinear, Trilinear: 2x, Anisotropic: 4x, 8x, 16x, ...)

	#if TUTORIAL_CHAP >= 62 // FIRE
			if (m_shaderType == SHADER_FIRE) {
				deviceContext->PSSetSamplers(1, 1, &m_sampleStateFire);
			}
	#endif
			if (castShadow)
				deviceContext->PSSetSamplers(2, 1, &m_sampleStateClamp11); // 2, 1 or 0, 2

			}
			// ---------------------------------------------------
			// Set Shader code to RUN:
			// ---------------------------------------------------
			// VS: Set CODE to Run on SHADERS:
			deviceContext->VSSetShader(m_vertexShader11, NULL, 0);		// Set the vertex code that will be used to process vertices

            if (m_Driver->RenderfirstTime) 
			{
				// [VS] -> HS -> DS -> GS -> [PS]
				// Set PIPE: VS => PS
				deviceContext->HSSetShader(NULL, NULL, 0);
				deviceContext->DSSetShader(NULL, NULL, 0);
				deviceContext->GSSetShader(NULL, NULL, 0);
			}

            // PS: Set CODE to Run on SHADERS:
            deviceContext->PSSetShader(m_pixelShader11, NULL, 0);		// Set the pixel code that will be used to process pixels

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
			if (m_instanceCount > 0)
				deviceContext->DrawIndexedInstanced(indexCount, m_instanceCount, start, 0, 0);	// Use: Instancing
			else
#endif
			{
				  deviceContext->DrawIndexed(indexCount, start, 0);	// Render Indexed mesh
			}
		}
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
		{
			// Set necessary state:
			m_driver->m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

			/*
			// For Debug:
			CULL_FRONT	0
			CULL_BACK	1
			CULL_NONE	2

			FILL_SOLID	0
			FILL_WIRE	1

			SOLID_PIPELINE_STATES		0
			TRANSPARENT_PIPELINE_STATES 1
			*/
			{
				UINT cullMode = m_driver->m_CullMode;
				UINT fillMode = m_driver->m_fillMode;

#if defined USE_ALPHA_BLENDING
				BOOL alphaBlend = m_driver->g_AlphaBlend;
#else
				BOOL alphaBlend = SOLID_PIPELINE_STATES;
#endif
				BOOL zBuffer = m_driver->g_Zbuffer;
#if defined USE_RASTERIZER_STATE
				m_driver->m_commandList->SetPipelineState(m_pipelineState[zBuffer][cullMode][fillMode][alphaBlend].Get());
#endif
			}

			switch (m_shaderType)
			{
			case SHADER_COLOR:
			{
				ID3D12DescriptorHeap* ppHeaps[] = { DX12mSrvDescriptorHeap.Get() };
				m_driver->m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
				// SHADER_COLOR:
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|

				// CBV
				auto offSet = DX12mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(0, offSet); //C0 Set "Constant Buffer View" 
			}
			break;

			case SHADER_TEXTURE:
			{
				ID3D12DescriptorHeap* ppHeaps[] = { DX12mSrvDescriptorHeap.Get() };
				m_driver->m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

					// SHADER_TEXTURE_LIGHT
					// | Root Signature		| Shader Registers	|
					// |0| DescriptorTable  | b0				|
					// |1| DescriptorTable  | b1				|
					// |2| DescriptorTable  | t0				|<-- HERE: 2+
					// ...
					// |10| DescriptorTable | t9				|

				// CBV
				auto offSet = DX12mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(0, offSet);	//C0 Set "Constant Buffer View" 
				offSet.ptr += m_CbvSrvDescriptorSize;

				// SRV:
				for (size_t i = 0; i < texture_index; i++)
					offSet.ptr += m_CbvSrvDescriptorSize;
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(1, offSet);	//T0 Set Texture
			}
			break;

			case SHADER_TEXTURE_FONT:
			{
				ID3D12DescriptorHeap* ppHeaps[] = { DX12mSrvDescriptorHeap.Get() };
				m_driver->m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				// |1| DescriptorTable  | t0				|
				// |2| DescriptorTable  | b1				|

				// 0 CBV
				auto offSet = DX12mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(0, offSet);	//C0 Set "Constant Buffer View" 

				// 1 SRV:
				offSet.ptr += m_CbvSrvDescriptorSize;
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(1, offSet);	//T0 Set Texture

				// 2 CBV
				offSet.ptr += m_CbvSrvDescriptorSize;
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(2, offSet);	//C1 Set "Constant Buffer View" 
				break;
			}

			case SHADER_TEXTURE_LIGHT:
			case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
			{
				ID3D12DescriptorHeap* ppHeaps[] = { DX12mSrvDescriptorHeap.Get() };
				m_driver->m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		#if !defined RENDER_OBJ_WITH_ALFA
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				// |1| DescriptorTable  | b1				|
				// |2| DescriptorTable  | t0				|

				// 0 CBV
				auto offSet = DX12mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(0, offSet);	//C0 Set "Constant Buffer View" 

				// 1 CBV
				offSet.ptr += m_CbvSrvDescriptorSize;
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(1, offSet);	//C1 Set "Constant Buffer View" 

				// 2 SRV:
				offSet.ptr += m_CbvSrvDescriptorSize;
				for (size_t i = 0; i < texture_index; i++)
					offSet.ptr += m_CbvSrvDescriptorSize;
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(2, offSet);	//T0 Set Texture
		#else
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				// |1| DescriptorTable  | b1				|
				// |2| DescriptorTable  | t0 AlfaMap		|
				// |3| DescriptorTable  | t1 Texture		|

				// 0 CBV
				auto offSet = DX12mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(0, offSet);	//C0 Set "Constant Buffer View" 

				// 1 CBV
				offSet.ptr += m_CbvSrvDescriptorSize;
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(1, offSet);	//C1 Set "Constant Buffer View" 

				// 2 SRV:
				offSet.ptr += m_CbvSrvDescriptorSize;
				if (hasAlfaMap)
					m_driver->m_commandList->SetGraphicsRootDescriptorTable(2, offSet);	//T0 Set AlfaMap Texture

				if (hasTexture) {
				// 3 SRV:
				offSet.ptr += m_CbvSrvDescriptorSize;
				for (size_t i = 0; i < texture_index; i++)
					offSet.ptr += m_CbvSrvDescriptorSize;
				m_driver->m_commandList->SetGraphicsRootDescriptorTable(3, offSet);	//T1 Set Shader Texture
				}
		#endif
				break;
			}
			default:
				throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
			}

			m_driver->m_commandList->DrawIndexedInstanced(indexCount, 1, start, 0, 0);	// Render Indexed mesh
		}
#endif
#if _DEBUG
		SystemHandle->TotalVertexCounter += indexCount;
#endif
	}

	void DXshaderClass::Render(UINT pass,void* Device_Context, int indexCount, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix)
	{
		ASSERT_DEBUG(indexCount > 0);
		SetShaderParameters(pass, Device_Context, worldMatrix, viewMatrix, projectionMatrix);	// Set the shader parameters that it will use for rendering

		RenderShader(pass, Device_Context, /*texture_index*/ 0, indexCount);						// Now render the prepared buffers with the shader
	}

} // DirectX

#endif

#pragma warning ( pop )
#endif
