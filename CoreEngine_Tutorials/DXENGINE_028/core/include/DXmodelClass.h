// --------------------------------------------------------------------------------------------
// Filename: DXmodelClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
#if defined DX_ENGINE
#if defined DX11 || defined DX9
#include "dx11Class.h"
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
#include "DX12Class.h"	//#include "GLopenGLclass.h"

// DX12 includes
#include <dxgi1_4.h>	// Always 1st!	(Select Driver)
#include <d3d12.h>		// DX12			(Select Device)
#include <D3Dcompiler.h>// Use Compiler
#include <DirectXMath.h>// Use Math
using namespace DirectX;

#include "DX12TextureClass.h"
#endif
#endif

#endif

#if defined DX_ENGINE
//////////////
// INCLUDES //
//////////////
#if defined DX9sdk
#include "DX9Class.h"
#endif

#include "DXshaderClass.h"
#include "virtualModelClass.h"
#include "DXbasicTypes.h"

namespace DirectX 
{


////////////////////////////////////////////////////////////////////////////////
// Class name: DXmodelClass
////////////////////////////////////////////////////////////////////////////////
class DXmodelClass : public VirtualModelClass
{
public:
	UINT WomaIntegrityCheck = 1234567222;

	DXmodelClass(bool model3d, PRIMITIVE_TOPOLOGY = TRIANGLELIST, bool computeNormals = false, bool modelHASshadow = false, bool modelRENDERshadow = false);
	~DXmodelClass();
	void Shutdown();
	void LOADDRIVER(void* driver);

	void RenderWithFade(float fadeLight = 1, bool FOG = false);
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
	void RenderSky(UINT camera, float fadeLight = 1);
#endif
	void Render(UINT camera = 0, UINT projection = 0, UINT pass = 0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL);

	// ----------------------------------------------------------------------
#if defined USE_VIEW2D_SPRITES // Sprites
	bool RenderSprite( int positionX, int positionY, float scale=1.0f, float fade = 1.0f);
	bool UpdateBuffersRotY( int positionX, int positionY);
	bool UpdateSpriteBuffersRotY( int positionX, int positionY);
#endif

	// BASIC LOAD:
	bool LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);
	
		// [PATTERN] Image loader:
	#if defined DX11 || defined DX9
		ID3D11ShaderResourceView* m_Texture11 = NULL;
	#endif
	#if defined DX9sdk
		LPDIRECT3DTEXTURE9 m_Texture9 = NULL;
	#endif
	#if defined DX12
		DX12TextureClass* m_Texture = NULL;
	#endif
	
	bool LoadTexture(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);
	bool LoadLight(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureLightVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);

	// MODEL LOAD:

	STRING MODEL_NAME=TEXT("");

	// Math Aux Functions //
	void Identity();
	void multiply (void* m);	//XMMATRIX* or mat4*

	void rotateX (float rad);
	void rotateY (float rad);
	void rotateZ (float rad);
	
	void scale(float x, float y, float z);
	void translation(float x, float y, float z);
	
public:
	// VARS:
	// ----------------------------------------------------------------------
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		std::vector<DX12TextureClass*>	meshSRV;
	#endif
	#if defined DX11 || defined DX9
		std::vector<ID3D11ShaderResourceView*> meshSRV11;	// vector with all pointer(s) to textures loaded
	#endif
	#ifdef DX9sdk
		std::vector<IDirect3DBaseTexture9*> meshSRV9;	// vector with all pointer(s) to textures loaded
	#endif

// DX Specific:
#if defined DX9sdk
	DXshaderClass*	m_Shader9;
	D3DXMATRIX		m_worldMatrix9;
#else
  #if defined DX11 || defined DX9
	DXshaderClass*	m_Shader11=NULL;
  #endif
  #if defined DX12
	DXshaderClass*	m_Shader = NULL;
  #endif

	XMMATRIX		m_worldMatrix;
#endif

	XMFLOAT4 objectCenterOffset = XMFLOAT4(0, 0, 0, 0);
	XMFLOAT3 minVertex = XMFLOAT3(0, 0, 0);
	XMFLOAT3 maxVertex = XMFLOAT3(0, 0, 0);

#if defined BOUNDINGVOLUMES
	std::vector<XMFLOAT3> boundingBoxVerts;
	std::vector<DWORD> boundingBoxIndex;
	void CreateBoundingVolumes(std::vector<XMFLOAT3>& vertPosArray);
#endif

	HRESULT LoadTextureImage(TCHAR* textureFilename);

// ----------------------------------------------------------------------
private:
// ----------------------------------------------------------------------

#if defined LOADW3D
	bool LoadW3D	(SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow=false, bool renderShadow=false, UINT instanceCount=0);
#endif

	DXshaderClass* CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType);
	bool InitializeDXbuffers(TCHAR* objectName, std::vector<STRING>* textureFile=NULL);
	bool CreateDXbuffers(UINT sizeofMODELvertex, /*ID3D11Device*/ void* device, void* indices, void* vertices);
	void SetGeometryBuffers(void* deviceContext);	//ID3D11DeviceContext

	#if defined USE_LIGHT_RAY
	void UpdateDynamic( std::vector<ModelColorVertexType>* lightVertexVector);
	#endif

	std::vector<ModelColorVertexType>* modelColorVertex = NULL;			// MODEL!
	std::vector<ModelColorVertexType> modelColorVertex_;				// LOAD W3D
	bool InitializeColorBuffers(void* g_driver, void* indices);
	std::vector<ModelTextureVertexType>* modelTextureVertex = NULL;		// MODEL!
	std::vector<ModelTextureVertexType> modelTextureVertex_;			// LOAD W3D
	bool InitializeTextureBuffers(void* g_driver, void* indices);
	std::vector<ModelTextureLightVertexType>* modelTextureLightVertex = NULL;	// MODEL!
	std::vector<ModelTextureLightVertexType> modelTextureLightVertex_;			// LOAD W3D
	bool InitializeTextureLightBuffers(void* g_driver, void* indices);

	// VARS:
	// ----------------------------------------------------------------------
	
#if defined DX9sdk
	DirectX::DX9Class* m_driver9=NULL;
#endif
#if defined DX11 || defined DX9
	DirectX::DX11Class* m_driver11 = NULL;
#endif
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	DirectX::DX12Class* m_driver = NULL;
#endif

	UINT sizeofMODELvertex=0;

#if defined USE_VIEW2D_SPRITES
	float model_fade = 1;
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	// App resources.
	ComPtr<ID3D12Resource>		m_vertexBuffer = NULL;
	ComPtr<ID3D12Resource>		m_indexBuffer = NULL;

	// futuro:
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView = { 0 };
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView = { 0 };

	ComPtr<ID3D12Resource>		vertexBufferUpload = NULL;
	ComPtr<ID3D12Resource>		indexBufferUpload = NULL;
#endif

#if defined DX11 || defined DX9
	ID3D11Buffer	*m_vertexBuffer11 = NULL;
	ID3D11Buffer	*m_indexBuffer11 = NULL;
#endif

#if defined DX9sdk
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer9;
	LPDIRECT3DINDEXBUFFER9  indexBuffer9;
#endif

	std::vector<UINT>* indexModelList;

#if defined USE_VIEW2D_SPRITES
	float	m_xTexture=0;
#endif

	

};

}

#endif
