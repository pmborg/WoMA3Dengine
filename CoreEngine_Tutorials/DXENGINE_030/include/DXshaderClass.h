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

//#if (defined DX_ENGINE)

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

	#include "DX12TextureClass.h"

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

	#define TRANSPARENT_PIPELINE_STATES 1
	#define MAXNUM_PIPELINE_STATES		2

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
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX WV;              // [64]: world * view 
		XMMATRIX WVP;             // [64]: world * view * projection matrix

		// 23 BLOCK: VS2
		BOOL	VShasLight;
		BOOL	VShasSpecular;
		BOOL	VShasNormMap;
		BOOL	VShasFog;

		// 23 BLOCK: VS3
		XMFLOAT4/*XMVECTOR*/	VSlightDirection;	// LIGHT (XMVECTOR = XMFLOAT4)
		XMFLOAT4	VSambientColor;		// LIGHT: Ka
		XMFLOAT4	VSdiffuseColor;		// LIGHT: Kd
		XMFLOAT4	VSemissiveColor;	// LIGHT: Ke

		// 31 BLOCK: VS4
		float		VSfogStart;			
		float		VSfogEnd;			
		BOOL		VShasShadowMap; 
		BOOL		VS_USE_WVP;

		// 36 BLOCK: VS5
		XMMATRIX ViewToLightProj;

//#if DX_ENGINE_LEVEL >= 42
		// 42 BLOCK: VS6
		float		VSrotX;
		float		VSrotY;
		float		VSrotZ;
		float		VS6PAD;
//#endif
	};

	// PIXEL CBUFFER:
	struct PSconstantBufferType
	{
		// BLOCK1:
		XMFLOAT4    pixelColor;		// ch06: 16: (difColor) 4xfloat

		// BLOCK2:
		BOOL        hasTexture;     // No? Use pixelColor, then.
		BOOL        hasLight;		
		BOOL        hasSpecular;	
		BOOL		isFont;

		// BLOCK3:
		XMFLOAT4	ambientColor;	// LIGHT: Ka
		XMFLOAT4	diffuseColor;	// LIGHT: Kd
		XMFLOAT4	emissiveColor;	// LIGHT: Ke
		XMFLOAT4	lightDirection;	// LIGHT (XMVECTOR = XMFLOAT4)

		// BLOCK4:
		BOOL		hasColorMap;		// 66
		float		lightType;			// 29
		float		shaderType;			
		float		shaderTypeParameter;

		// BLOCK5:
		BOOL		hasAlfaColor;
		float		alfaColor;
		float		fade;
		float		frameTime;

		// BLOCK6:
		BOOL		hasFog;
		BOOL		isSky;
		BOOL		hasAlfaMap;
		BOOL		hasNormMap;
	
		// BLOCK7:
		XMFLOAT3	cameraPosition;
		BOOL		castShadow;
		XMFLOAT3	specularColor;
		float		nShininess;
	};


//PIXEL SKY:
struct ConstantBufferSkyType
{
};

	// FUNCTIONS:
	// ---------------------------------------------------------------------
public:
	UINT WomaIntegrityCheck = 1234567831;
	DXshaderClass(UINT ShaderVersionH, UINT ShaderVersionL, bool shader_3D);
	~DXshaderClass();
	void Shutdown();

	bool Initialize(INT m_ObjId, TCHAR* objectName, SHADER_TYPE shaderType, /*ID3D11Device*/ void*, HWND, PRIMITIVE_TOPOLOGY PrimitiveTopology, bool useGS = false);
	void Render(UINT pass,/*ID3D11DeviceContext*/ void*, int, XMMATRIX*, XMMATRIX*, XMMATRIX*);
	void SetShaderParameters(UINT pass, /*ID3D11DeviceContext*/ void* deviceContext,
								XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix,
								XMMATRIX* lightViewMatrix=NULL, XMMATRIX* ShadowProjectionMatrix=NULL);

	void RenderShader(UINT pass, /*ID3D11DeviceContext*/ void*, int texture_index, int, int start = 0);

private:
	bool InitializeShader(SHADER_TYPE shaderType, /*ID3D11Device*/ void*, HWND, PRIMITIVE_TOPOLOGY PrimitiveTopology);


	void OutputShaderErrorMessage(ID3D10Blob*, HWND, TCHAR*);

	// VARS:
	// ----------------------------------------------------------------------
	STRING MODEL_NAME;
	INT    m_ObjId = 0;

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
	ComPtr<ID3D12DescriptorHeap>	DX12mSrvDescriptorHeap = NULL;			// for Descriptor buffer(s)

	// First Constant Buffer
	VSconstantBufferType	mVS_constantBufferData = {};
	ComPtr<ID3D12Resource>	mVS_constantBuffer = NULL;
	UINT					c_alignedVSConstantBufferSize = 0;
	UINT8*					m_pMappedVSConstantBuffer = NULL;

	PSconstantBufferType	mPS_constantBufferData = {};
	// Second Constant Buffer
	
	ComPtr<ID3D12Resource>	mPS_constantBuffer = NULL;
	UINT					c_alignedPSConstantBufferSize = 0;
	UINT8*					m_pMappedPSConstantBuffer = NULL;	// Pointer to RAM (virtual to GPU ConstantBufferView)

#endif

	SHADER_TYPE		m_shaderType;
	bool			shader2D;

	bool VS_USE_WVP = false; //by default use already multiplied matrix*matrix*matrix

			ID3D11Buffer*		m_PixelShaderBuffer11 = NULL;
			ID3D11SamplerState* m_sampleState11 = NULL;	// Resource: "Textures" States

			ID3D11SamplerState* m_sampleStateClamp11 = NULL;

			ID3D11ShaderResourceView*	texture11 = NULL;	// 21
			ID3D11ShaderResourceView*	texture11_2 = NULL;	// 43: Alfa Map
		#if defined DX9sdk
			LPDIRECT3DTEXTURE9 texture9 = NULL;
			LPDIRECT3DTEXTURE9 texture9_2 = NULL;
		#endif
			DX12TextureClass* texture = NULL;
			DX12TextureClass* texture2 = NULL;

		// --------------------------------------------------------------------------------------------
		// Internal Shader VARs to Copy to Buffers: VS/PS
		// --------------------------------------------------------------------------------------------
		// BLOCK1:
		XMFLOAT4	pixelColor;		

		// BLOCK2:
		BOOL		hasTexture = false;
		BOOL		hasLight = false;
		BOOL		hasSpecular = false;
		BOOL		isFontShader = false;

		// BLOCK3:
		XMFLOAT4	ambientColor;	// LIGHT: Ka
		XMFLOAT4	diffuseColor;	// LIGHT: Kd
		XMFLOAT4	emissiveColor;	// LIGHT: Ke
		//			lightDirection (AUTO)

		// BLOCK4:
		bool		hasColorMap;			// 66
		float		lightType = 0;			// Light Type
		float		shaderType = 0;			// Future
		float		shaderTypeParameter = 0;// Future

		// BLOCK5:
		bool		hasAlfaColor = 0;
		float		alfaColor = 0;
		float		PSfade = 0;			// Fade from 0 to 1

		// BLOCK6:
		BOOL		hasFog=false;
		BOOL		isSky = false;
		BOOL		hasAlfaMap = false;	// 43
		BOOL		hasNormMap = false;

		// BLOCK7:
		// cameraPosition (AUTO)
		BOOL		castShadow;
		XMFLOAT3	specularColor;	// 44:
		float		nShininess = 0;		// 44:

		// --------------------------------------------------------------------------------------------
		// Internal Shader VARs to Copy to Buffers: VS
		// --------------------------------------------------------------------------------------------
		float		fogStart;
		float		fogEnd;

		//Sky: 1

		//Sky: 2

    bool bUseGS;	// GS
};

}

	#include "lightClass.h"
