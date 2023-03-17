// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXshaderClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------

#include "OSengine.h"
#include <d3d11.h>

#include "DXshaderClass.h"

#if (defined DX9 && D3D11_SPEC_DATE_YEAR == 2009)
#include "dx9Class.h"
#endif
#include "dx11Class.h"
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
#include "dx12Class.h"
#endif

#if D3D11_SPEC_DATE_YEAR > 2009
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#endif

//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "WinSystemClass.h"

shaderTree* shaderManager = NULL;

extern shaderTree shaderManager_40[];
extern shaderTree shaderManager_41[];
extern shaderTree shaderManager_50[];
extern shaderTree shaderManager_51[];

// 21 Vertex: SHADER_COLOR: v + Kd
/*struct VSIn
{
	float3 position :	POSITION;
	float4 color :		COLOR;	//21
};*/
static const D3D11_INPUT_ELEMENT_DESC colorPolygonLayout11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009  && D3D11_SPEC_DATE_YEAR > 2009
static const D3D12_INPUT_ELEMENT_DESC colorPolygonLayout[] =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};
#endif
/*struct VSIn
{
	float3 position :	POSITION;
	float2 texCoords :	TEXCOORD0; //22
};*/

/*struct VSIn
{
	float3 position : POSITION;	//21
	float2 texCoords : TEXCOORD; //22
	float3 normal : NORMAL;	//23
};*/

// TERRAINS:

namespace DirectX {

	DXshaderClass::DXshaderClass(UINT ShaderVersion_H, UINT ShaderVersion_L, bool shader_3D)
	{
		CLASSLOADER();

		//DX_CLASS* m_driver;
#if defined DX9sdk
		m_driver9 = ((DirectX::DX9Class*)SystemHandle->m_Driver);
#endif
		m_driver11 = ((DirectX::DX11Class*)SystemHandle->m_Driver);
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
		m_driver = ((DirectX::DX12Class*)SystemHandle->m_Driver);
#endif

		ShaderVersionH = ShaderVersion_H;
		ShaderVersionL = ShaderVersion_L;

		//private:
		m_layout11 = NULL;	//Used Vetex Description
		m_VertexShaderBuffer11 = NULL;	//Parameters: VS

		//Shader CODE:
		m_vertexShader11 = NULL;		//Code: VS
		m_pixelShader11 = NULL;			//Code: PS
		m_geometryShader11 = NULL;		//Code: GS

		m_shaderType = SHADER_AUTO;
		shader2D = !shader_3D;

		//public:

		bUseGS = false;	// GS
		// --------------------------------------------------------------------------------------------
	}

	DXshaderClass::~DXshaderClass() { CLASSDELETE(); } // Note: DxModel Call ShutDown()

#if !defined USE_PRECOMPILED_SHADERS && ( defined DX11 || defined DX9)
	void DXshaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, TCHAR* shaderFilename)
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());	// Get a pointer to the error message text buffer
		//ULONG bufferSize = (ULONG) errorMessage->GetBufferSize();			// Get the length of the message

		WOMA_LOGManager_DebugMSGAUTO(TEXT("LOADING DX SHADER ERROR: %s\n"), compileErrors);
		SAFE_RELEASE(errorMessage);	// Release the error message

		// Pop a message up on the screen to notify the user to check the text file for compile errors.
		WOMA::WomaMessageBox(TEXT("Error compiling shader.  Check shader-error.txt for message."), shaderFilename);
	}
#endif

	bool DXshaderClass::Initialize(TCHAR* objectName, SHADER_TYPE shaderType, /*ID3D11Device*/ void* device, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology, bool useGS)
	{
		bool result = false;
		m_shaderType = shaderType;
		bUseGS = useGS;
		MODEL_NAME = objectName;

		//GLOBAL:
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("%s Initialize Shader %s: %d.%d\n"), (TCHAR*)objectName, (TCHAR*)(WOMA::SHADER_TYPE_NAME[m_shaderType+1] + TEXT(" ")).c_str(), ShaderVersionH, ShaderVersionL);

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

		//bUseGS = true;	//ON/OFF (default: OFF)
		result = InitializeShader(shaderType, device, hwnd, PrimitiveTopology);

		return result;
	}

	void DXshaderClass::Shutdown()
	{
		WOMA_LOGManager_DebugMSG("DXshaderClass() DESTROYING: %s\n", MODEL_NAME.c_str());

		// 21
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		{
			SAFE_RELEASE(m_layout11);				// Release the layout.
			SAFE_RELEASE(m_VertexShaderBuffer11);	// Release the VS constant buffer.

			SAFE_RELEASE(m_pixelShader11);			// Release the pixel shader.	
			SAFE_RELEASE(m_vertexShader11);			// Release the vertex shader.
			SAFE_RELEASE(m_geometryShader11);		// Release the Geometry Shader.

		}

	}

	// ----------------------------------------------------------------------------------------------
	bool DXshaderClass::InitializeShader(SHADER_TYPE shaderType, void* DriverDevice, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology)
		// ----------------------------------------------------------------------------------------------
	{
		UINT numElements = 0;
		HRESULT result;

		const D3D11_INPUT_ELEMENT_DESC* polygonLayout11 = NULL;
		DX11Class* driver11 = m_driver11;
#define device11 ((ID3D11Device*)DriverDevice)
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
			//#if ENGINE_LEVEL >= 21
		case SHADER_COLOR:
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
			{
				polygonLayout = &colorPolygonLayout[0];
				numElements = _countof(colorPolygonLayout);
			}
#endif
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
			{
				polygonLayout11 = &colorPolygonLayout11[0];
				numElements = _countof(colorPolygonLayout11);
			}
#if defined DX9sdk
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
			{
				polygonLayout9 = &colorPolygonLayout9[0];
				numElements = _countof(colorPolygonLayout9);
			}
#endif
			break;
			//#endif

		}

		std::wstring vsFilename = L"";
		std::wstring psFilename = vsFilename;
#if D3D11_SPEC_DATE_YEAR == 2009
		std::string vertexHLSL = "MyVertexShader021Color";
		std::string pixelHLSL = "MyPixelShader021Color";
#else
		std::string vertexHLSL = "";
		std::string pixelHLSL = "";
#endif

		switch (shaderType)
		{
		case SHADER_COLOR:
			vsFilename.append(L"hlsl/021Color.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("MyVertexShader021Color");
			pixelHLSL.append("MyPixelShader021Color");
			break;
		case SHADER_TEXTURE:
			vsFilename.append(L"hlsl/022Texture.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("MyVertexShader022Texture");
			pixelHLSL.append("MyPixelShader022Texture");
			break;
		case SHADER_TEXTURE_LIGHT:
			vsFilename.append(L"hlsl/023Light.hlsl");
			psFilename = vsFilename;
			vertexHLSL.append("MyVertexShader023Light");
			pixelHLSL.append("MyPixelShader023Light");
			break;
		};
		//AQUI
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
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
			vertVer.append(SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]->ShaderModel);  //TEXT("vs_5_0")
			result = D3DX11CompileFromFile(vsFilename.c_str(), NULL, NULL, vertexHLSL.c_str(), "vs_5_0"/*vertVer.c_str()*/, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, hwnd, (TCHAR*)vsFilename.c_str());// If the shader failed to compile it should have writen something to the error message
				else
					WOMA::WomaMessageBox((TCHAR*)vsFilename.c_str(), TEXT("Missing Shader File")); // ... Then it simply could not find the shader file itself

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
					WOMA::WomaMessageBox((TCHAR*)vsFilename.c_str(), TEXT("Missing Shader File")); // ... Then it simply could not find the shader file itself

				return false;
			}
#else
			std::stringstream sstm;
			sstm << DX_ENGINE_LEVEL;
			std::string lvl = sstm.str().c_str();
			D3D_SHADER_MACRO defines[] =
			{
				"ENGINE_LEVEL", lvl.c_str(),
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
			CHAR str[MAX_STR_LEN] = { 0 }; wtoa(str, (TCHAR*)SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]->ShaderModel, MAX_STR_LEN); // wchar ==> char
			vertVer.append(str);  //TEXT("vs_5_0")
#else
			vertVer.append(SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]->ShaderModel);  //TEXT("vs_5_0")
#endif
			result = D3DCompileFromFile(vsFilename.c_str(), defines/*NULL*/, D3D_COMPILE_STANDARD_FILE_INCLUDE, vertexHLSL.c_str(), vertVer.c_str(), compileFlags, 0, &vertexShaderBuffer, &errorMessage);
			if (FAILED(result))
			{
				WOMA::WomaMessageBox((TCHAR*)errorMessage->GetBufferPointer(), TEXT("SHADER Error description :"));
				if (errorMessage)
					WOMA_LOGManager_DebugMSG("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				return false;
			}
			vertVer[0] = 'p';  //TEXT("ps_5_0")
			result = D3DCompileFromFile(vsFilename.c_str(), defines/*NULL*/, D3D_COMPILE_STANDARD_FILE_INCLUDE, pixelHLSL.c_str(), vertVer.c_str(), compileFlags, 0, &pixelShaderBuffer, &errorMessage);
			if (FAILED(result))
			{
				WOMA::WomaMessageBox((TCHAR*)errorMessage->GetBufferPointer(), TEXT("SHADER Error description :"));
				if (errorMessage)
					WOMA_LOGManager_DebugMSG("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				return false;
			}
#endif


			// Create the vertex shader from the buffer: (LOAD & COMPILE EXTERNAL FILE)
			result = device11->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader11);
			if (FAILED(result))
				WOMA::WomaMessageBox(TEXT("ERROR"), TEXT("CreateVertexShader:"));
			IF_FAILED_RETURN_FALSE(result);

			// Create the pixel shader from the buffer: (LOAD & COMPILE EXTERNAL FILE)
			result = device11->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader11);
			if (FAILED(result))
				WOMA::WomaMessageBox(TEXT("ERROR"), TEXT("pixelShaderBuffer:"));
			IF_FAILED_RETURN_FALSE(result);


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
			if (FAILED(result)) { WomaFatalExceptionW(TEXT("Error: CreateVertexShader")); return false; }

			// Create the Pixel Shader from the buffer: (GET CODE ON "EXE")
			result = device11->CreatePixelShader(shaderManager[shaderType].blobPS, shaderManager[shaderType].sizePS, NULL, &m_pixelShader);
			if (FAILED(result)) { WomaFatalExceptionW(TEXT("Error: CreatePixelShader")); return false; }

			// GS
			if (bUseGS)
			{   // Create the Geometry Shader from the buffer.
				result = device11->CreateGeometryShader(shaderManager[shaderType].blobGS, shaderManager[shaderType].sizeGS, NULL, &m_geometryShader);
				if (FAILED(result)) { WomaFatalExceptionW(TEXT("Error: CreateGeometryShader")); return false; }
			}
			// GS
#endif

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// DX 12
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{

			//
			// Create a descriptor heap for the constant buffers.
			//
			D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

			switch (m_shaderType)
			{
			case SHADER_COLOR:
				// SHADER_COLOR:
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				srvHeapDesc.NumDescriptors = 1; // = Num. InitAsDescriptorTable: D3D12_DESCRIPTOR_RANGE_TYPE_CBV
				break;

			}

			srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;


			// IF Failed ?: | Root Signature		| Shader Registers	|   ---> dont match with HLSL code:
			result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap));
			if (FAILED(result))
			{
				WOMA::WomaMessageBox(TEXT("CreateDescriptorHeap"), TEXT("DX12 ERROR:"));
				ThrowIfFailed(result);
			}

#ifdef _DEBUG
			mSrvDescriptorHeap->SetName(L"VERTEX Constant Buffer View Descriptor Heap");
#endif

			m_CbvSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			// Create root Signature:
			// ----------------------------------------------------------------------------
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;

			switch (shaderType)
			{
			case SHADER_COLOR:
			{
				// ORIGINAL: D:\WoMAengine2014\EXTRA\DX12 Samples\DirectX-Graphics-Samples\Samples\01 D3D12HelloWorld\src\HelloTriangle

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
					D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

				rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
				break;
			}

			}

			ComPtr<ID3DBlob> error;
			ComPtr<ID3DBlob> signature;
			result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
			if (FAILED(result))
			{
				WOMA::WomaMessageBox("D3D12SerializeRootSignature", "DX12 ERROR:");
				WOMA_LOGManager_DebugMSGAUTO((char*)error->GetBufferPointer());
				//OutputDXError(error.Get());
				//ThrowIfFailed(result);
				return false;
			}
			result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
			if (FAILED(result))
			{
				WOMA::WomaMessageBox("CreateRootSignature", "DX12 ERROR:");
				char* compileErrors = (char*)(error->GetBufferPointer());
				WOMA::WomaMessageBox(compileErrors, "DX12 ERROR: CreateRootSignature");
				ThrowIfFailed(result);
			}

			if (!(polygonLayout && numElements && m_rootSignature))
			{
				WOMA::WomaMessageBox(TEXT("ASSERT(polygonLayout && numElements && m_rootSignature)"), TEXT("DX12 ERROR:"));
				ASSERT(polygonLayout && numElements && m_rootSignature);
			}

			//
			// LoadAssets()- PARTE 2 ----> bool DXshaderClass::InitializeShader()
			//
			// ------------------------------------------------------------------------------
			// Describe and create the graphics pipeline  state object (PSO).
			// ------------------------------------------------------------------------------
			D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc = {};

			opaquePsoDesc.InputLayout = { polygonLayout, numElements };
			opaquePsoDesc.pRootSignature = m_rootSignature.Get();

#if !defined USE_PRECOMPILED_SHADERS
	#ifdef _DEBUG
			// Enable better shader debugging with the graphics debugging tools.
			UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
	#else
			UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3; // 0;
	#endif

			ComPtr<ID3DBlob> vertexShader;
			ComPtr<ID3DBlob> pixelShader;
			ID3DBlob* errorMessage = NULL;

			std::stringstream sstm;
			sstm << DX_ENGINE_LEVEL;
			std::string lvl = sstm.str().c_str();
			D3D_SHADER_MACRO defines[] =
			{
				"ENGINE_LEVEL", lvl.c_str(),
				"DXAPI12", "1",
				NULL, NULL
			};

			// NOTE! The run time compiler support only Shader 5.0, for more use: USE_PRECOMPILED_SHADERS option 
			STRING vertVer = TEXT("vs_");
			vertVer.append(SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]->szShaderModel);  //TEXT("vs_5_0")
			vertVer[4] = '_';  //TEXT("vs_5_0")
			result = D3DCompileFromFile(vsFilename.c_str(), defines/*nullptr*/, nullptr, vertexHLSL.c_str(), TEXT("vs_5_0")/*vertVer.c_str()*/, compileFlags, 0, &vertexShader, &errorMessage);
			if (FAILED(result))
			{
				if (errorMessage)
					WOMA_LOGManager_DebugMSG("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				return false;
			}

			vertVer[0] = 'p';  //TEXT("ps_5_0")
			result = D3DCompileFromFile(psFilename.c_str(), defines/*nullptr*/, nullptr, pixelHLSL.c_str(), TEXT("ps_5_0")/*vertVer.c_str()*/, compileFlags, 0, &pixelShader, &errorMessage);
			if (FAILED(result))
			{
				if (errorMessage)
					WOMA_LOGManager_DebugMSG("SHADER Error description:\n%s", (LPCSTR)errorMessage->GetBufferPointer());
				return false;
			}

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
			//opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

			opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			opaquePsoDesc.DepthStencilState.DepthEnable = FALSE;
			opaquePsoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			opaquePsoDesc.DepthStencilState.StencilEnable = FALSE;
			opaquePsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

			opaquePsoDesc.SampleMask = UINT_MAX;
			if (PrimitiveTopology == LINELIST)
				opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			else
				opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

			opaquePsoDesc.NumRenderTargets = 1;
			opaquePsoDesc.RTVFormats[0] = driver->BUFFER_COLOR_FORMAT; // DXGI_FORMAT_B8G8R8A8_UNORM;
			opaquePsoDesc.SampleDesc.Count = driver->ReqMSAAmultiSampleCount; //opaquePsoDesc.SampleDesc.Count = 1;
			opaquePsoDesc.SampleDesc.Quality = SystemHandle->AppSettings->MSAA_ENABLED ? (driver->ReqMSAAquality - 1) : 0;
			opaquePsoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; //NOTE: This format (DXGI_FORMAT_D32_FLOAT) can be used with/without DSV, and this one DXGI_FORMAT_D24_UNORM_S8_UINT (CANT!)

			//------------------------------------------------------------------------------------------------------------------------------------------------------
			//
			// RasterizerStates
			//
			for (UINT zBufferMode = 0; zBufferMode < 2; zBufferMode++)
			{
				if (zBufferMode == 1)
					opaquePsoDesc.DepthStencilState.DepthEnable = TRUE;

				for (UINT cullMode = 0; cullMode < 3; cullMode++)			//0..2
				{
					for (UINT fillMode = 0; fillMode < 2; fillMode++)		//0..1
					{
						opaquePsoDesc.RasterizerState = driver->m_rasterState[cullMode][fillMode];
						//EQUAL TO:   device->CreateGraphicsPipelineState(&opaquePsoDesc, IID_ID3D12PipelineState, &m_pipelineState[cullMode][fillMode][TRANSPARENT_PIPELINE_STATES]);
						ThrowIfFailed(device->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&m_pipelineState[zBufferMode][cullMode][fillMode][SOLID_PIPELINE_STATES])));
					}
				}
			}

			//------------------------------------------------------------------------------------------------------------------------------------------------------


			// ------------------------------------------------------------------------------
			// Create the constant buffer(s).
			// ------------------------------------------------------------------------------
			{
				//
				// CreateConstantBufferView(): Describe and create a constant buffer view.
				//
				c_alignedVSConstantBufferSize = (sizeof(VSconstantBufferType) + 255) & ~255;

				// ALOCATE SPACE ON GPU RAM
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
				{
					D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc[1] = {};
					//[0]
					cbvDesc[0].BufferLocation = mVS_constantBuffer->GetGPUVirtualAddress();
					cbvDesc[0].SizeInBytes = c_alignedVSConstantBufferSize; 	// CB size is required to be 256-byte aligned.

					// CBV
					CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle0(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 0, 0);
					device->CreateConstantBufferView(&cbvDesc[0], cbvHandle0);
				}
				break;

				}

			}
		}
#endif

		// --------------------------------------------------------------------------------------------
		// Create/Setup Sampler State:
		// --------------------------------------------------------------------------------------------
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		{

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
			ASSERT(BufferDesc.ByteWidth <= D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT && (BufferDesc.ByteWidth % 16) == 0); // Validade Size

			result = device11->CreateBuffer(&BufferDesc, NULL, &m_VertexShaderBuffer11);
			IF_FAILED_RETURN_FALSE(result);
			// --------------------------------------------------------------------------------------------
			// CREATE Buffer(s) DATA for "Pixel Shader":
			// --------------------------------------------------------------------------------------------
		}

		return true;
	}

	// ----------------------------------------------------------------------------------------
	void DXshaderClass::SetShaderParameters(/*ID3D11DeviceContext*/ void* Device_Context,
		XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix,
		XMMATRIX* lightViewMatrix, XMMATRIX* ShadowProjectionMatrix)
		// ----------------------------------------------------------------------------------------
	{
		HRESULT result;
		ID3D11DeviceContext* deviceContext11 = ((ID3D11DeviceContext*)Device_Context);

		VSconstantBufferType* dataVSptr = NULL;	// Reset Pointer, only once:
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
			dataVSptr = &mVS_constantBufferData;	// Pointer to Static Buffer
#endif

		D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		{
			// VERTEX SHADER: Lock the constant buffer so it can be written to
			// ----------------------------------------------------------------------------
			result = deviceContext11->Map(m_VertexShaderBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result)) { throw woma_exception("deviceContext->Map()", __FILE__, __FUNCTION__, __LINE__); }

			dataVSptr = (VSconstantBufferType*)mappedResource.pData;	// Get a pointer to the data in the constant buffer.
		}

		//
		// BOTH: DX11 and DX12
		//

	//#if !defined USE_PRECOMPILED_SHADERS // ORIGINAL CODE: rastertek
	//	dataVSptr->world = XMMatrixTranspose (*worldMatrix);		// Copy the matrices into the constant buffer.
	//	dataVSptr->view = XMMatrixTranspose (*viewMatrix);		
	//	dataVSptr->projection = XMMatrixTranspose (*projectionMatrix);
	//#else
		// Copy the matrices into the Constant buffer:
		XMMATRIX WV = (*worldMatrix) * (*viewMatrix);
		dataVSptr->world = XMMatrixTranspose(*worldMatrix);
		dataVSptr->WV = XMMatrixTranspose(WV);						// Pre compute WV to reuse in all Vertices
		dataVSptr->WVP = XMMatrixTranspose(WV * (*projectionMatrix));	// Pre compute WVP to reuse in all Vertices
		//#endif

		// BLOCK: VS4

		// BLOCK: VS6

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			// Update the constant buffer resource:
			//CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
			memcpy(m_pMappedVSConstantBuffer, &mVS_constantBufferData, sizeof(mVS_constantBufferData));
		}
#endif


		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		{
			deviceContext11->Unmap(m_VertexShaderBuffer11, 0);						// Unlock the constant buffer.
			deviceContext11->VSSetConstantBuffers(0, 1, &m_VertexShaderBuffer11);	// Finanly set the "Constant" buffer in the vertex shader with the updated values.
		}

	}


	void DXshaderClass::RenderShader(/*ID3D11DeviceContext*/ void* Device_Context, int indexCount, int start)
	{
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		{
#define deviceContext ((ID3D11DeviceContext*)Device_Context)
			deviceContext->IASetInputLayout(m_layout11);					// Set the vertex input layout

			// Set CODE to Run on Shaders:
			deviceContext->VSSetShader(m_vertexShader11, NULL, 0);		// Set the vertex code that will be used to process vertices
			deviceContext->PSSetShader(m_pixelShader11, NULL, 0);			// Set the pixel code that will be used to process pixels

			// GS
			if (bUseGS)
				deviceContext->GSSetShader(m_geometryShader11, NULL, 0);
			else
			{
				// [VS] -> HS -> DS -> GS -> [PS]
				// Set PIPE: VS => PS
				deviceContext->HSSetShader(NULL, NULL, 0);
				deviceContext->DSSetShader(NULL, NULL, 0);
				deviceContext->GSSetShader(NULL, NULL, 0);
			}

				deviceContext->DrawIndexed(indexCount, start, 0);	// Render Indexed mesh
		}

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
#define driver m_driver

			// Set necessary state:
			driver->m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

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
				UINT cullMode = driver->m_CullMode;
				UINT fillMode = driver->m_fillMode;

				BOOL alphaBlend = SOLID_PIPELINE_STATES;
				BOOL zBuffer = driver->g_Zbuffer;

				driver->m_commandList->SetPipelineState(m_pipelineState[zBuffer][cullMode][fillMode][alphaBlend].Get());
			}

			switch (m_shaderType)
			{
			case SHADER_COLOR:
			{
				ID3D12DescriptorHeap* ppHeaps[] = { mSrvDescriptorHeap.Get() };
				driver->m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
				// SHADER_COLOR:
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|

				// CBV
				auto offSet = mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				driver->m_commandList->SetGraphicsRootDescriptorTable(0, offSet); //C0 Set "Constant Buffer View" 
			}
			break;

			}

			driver->m_commandList->DrawIndexedInstanced(indexCount, 1, start, 0, 0);	// Render Indexed mesh
		}
#endif
	}

	void DXshaderClass::Render(/*ID3D11DeviceContext*/ void* Device_Context, int indexCount, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix)
	{
		ASSERT(indexCount > 0);

		SetShaderParameters(Device_Context, worldMatrix, viewMatrix, projectionMatrix);	// Set the shader parameters that it will use for rendering
		RenderShader(Device_Context, indexCount);										// Now render the prepared buffers with the shader
	}

} // DirectX

