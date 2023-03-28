// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXshaderClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------

#pragma once

//////////////
// INCLUDES //
//////////////
#include "platform.h"

#include "DXbasicTypes.h"
#include "womadriverclass.h"
#include "virtualModelClass.h"

  #if defined DX9sdk
	#include "Dx9Class.h"
  #endif
	#include "Dx11Class.h"
  #if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
	#include "Dx12Class.h"
  #endif
	#include "womadriverclass.h"	//woma
	#include "GLmathClass.h"		//woma	
	#include "GLopenGLclass.h"		//woma
	#include "wGLopenGLclass.h"		// Windows

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
// DX12 includes
#include <dxgi1_4.h>	// Always 1st!	(Select Driver)
#include <d3d12.h>		// DX12			(Select Device)
#include <D3Dcompiler.h>// Use Compiler
#include <DirectXMath.h>// Use Math

// Windows STL includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <conio.h>
#include <vector>
#include <string>
#include <array>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;
#endif

#include <fstream>
using namespace std;

namespace DirectX 
{
#define 	SOLID_PIPELINE_STATES		0

	#define MAXNUM_PIPELINE_STATES		1

////////////////////////////////////////////////////////////////////////////////
// Class name: DXshaderClass
////////////////////////////////////////////////////////////////////////////////
class DXshaderClass
{
private:
	// NOTE: DONT USE "bool" USE "BOOL"

    // VERTEX CBUFFER:
	struct VSconstantBufferType
	{
		// BLOCK: VS1
		XMMATRIX world;           // [64]: world
		XMMATRIX WV;              // [64]: world * view 
		XMMATRIX WVP;             // [64]: world * view * projection matrix

		// 23 BLOCK: VS2
		BOOL	VShasLight;
		BOOL	VShasSpecular;
		BOOL	VShasNormMap;
		BOOL	VShasFog;

		// 23 BLOCK: VS3
		XMFLOAT4/*XMVECTOR*/	VSlightDirection;	// LIGHT (XMVECTOR = XMFLOAT4)
		//float		VSPad1;
		XMFLOAT4	VSambientColor;		// LIGHT: Ka
		XMFLOAT4	VSdiffuseColor;		// LIGHT: Kd
		XMFLOAT4	VSemissiveColor;	// LIGHT: Ke

		// 31 BLOCK: VS4
		float		VSfogStart;			// FOG: Future
		float		VSfogEnd;			// FOG: Future
		BOOL		VShasShadowMap; 
		float		VSpad2;

		// 45 BLOCK: VS5
		XMMATRIX ViewToLightProj;
	};

	// PIXEL CBUFFER:


//PIXEL SKY:
struct ConstantBufferSkyType
{
};

	// FUNCTIONS:
	// ---------------------------------------------------------------------
public:
	UINT WomaIntegrityCheck = 1234567890;
	DXshaderClass(UINT ShaderVersionH, UINT ShaderVersionL, bool shader_3D);
	~DXshaderClass();
	void Shutdown();

	bool Initialize(TCHAR* objectName, SHADER_TYPE shaderType, /*ID3D11Device*/ void*, HWND, PRIMITIVE_TOPOLOGY PrimitiveTopology, bool useGS = false);
	void Render(/*ID3D11DeviceContext*/ void*, int, XMMATRIX*, XMMATRIX*, XMMATRIX*);
	void SetShaderParameters(	/*ID3D11DeviceContext*/ void* deviceContext, 
								XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix,
								XMMATRIX* lightViewMatrix=NULL, XMMATRIX* ShadowProjectionMatrix=NULL);

	void RenderShader(/*ID3D11DeviceContext*/ void*, int, int start = 0);

private:
	bool InitializeShader(SHADER_TYPE shaderType, /*ID3D11Device*/ void*, HWND, PRIMITIVE_TOPOLOGY PrimitiveTopology);


	void OutputShaderErrorMessage(ID3D10Blob*, HWND, TCHAR*);

	// VARS:
	// ----------------------------------------------------------------------
	STRING MODEL_NAME;

#if defined DX9sdk
	DirectX::DX9Class* m_driver9=NULL;
#endif
	DirectX::DX11Class* m_driver11 = NULL;
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	DirectX::DX12Class* m_driver = NULL;
#endif
private:
	UINT ShaderVersionH, ShaderVersionL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC transparentPsoDesc = {};

	ID3D11InputLayout*		m_layout11 = NULL;
	ID3D11Buffer*			m_VertexShaderBuffer11 = NULL; /*m_matrixBuffer*/

	// Shader CODE:
	ID3D11VertexShader*		m_vertexShader11 = NULL;
	ID3D11PixelShader*		m_pixelShader11 = NULL;
	ID3D11GeometryShader*	m_geometryShader11 = NULL;	// GS

public:

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	ComPtr<ID3D12RootSignature>		m_rootSignature = NULL;
	ComPtr<ID3D12PipelineState>		m_pipelineState[2][3][2][MAXNUM_PIPELINE_STATES] = {0};
	
	UINT m_CbvSrvDescriptorSize;
	ComPtr<ID3D12DescriptorHeap>	mSrvDescriptorHeap = NULL;			// for Descriptor buffer(s)

	// First Constant Buffer
	VSconstantBufferType	mVS_constantBufferData = {};
	ComPtr<ID3D12Resource>	mVS_constantBuffer = NULL;
	UINT					c_alignedVSConstantBufferSize = 0;
	UINT8*					m_pMappedVSConstantBuffer = NULL;

	// Second Constant Buffer

#endif

	SHADER_TYPE		m_shaderType;
	bool			shader2D;

    bool bUseGS;	// GS
};

}

