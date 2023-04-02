// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXmodelClass.h
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

#define initLoadTexture3D(model, texture, vertexVector, IndexList, shader_type)\
{\
	std::vector<STRING> Textures;\
	Textures.push_back(texture);\
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) {CREATE_MODELGL3_IF_NOT_EXCEPTION(model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);}\
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) {CREATE_MODELDX_IF_NOT_EXCEPTION(model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);}\
	if (shader_type == SHADER_TEXTURE) ASSERT(model->LoadTexture(texture, SystemHandle->m_Driver, shader_type, &Textures, &vertexVector, &IndexList));\
}

#define initLoadTexture2D(model, texture, vertexVector, IndexList, shader_type)\
{\
	std::vector<STRING> Textures; \
	Textures.push_back(texture); \
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(model, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }\
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION(model, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }\
	if (shader_type == SHADER_TEXTURE) ASSERT(model->LoadTexture(texture, SystemHandle->m_Driver, shader_type, &Textures, &vertexVector, &IndexList)); \
}

#define initLoadTextureLight3D(model, texture, vertexVector, IndexList, shader_type)\
{\
	std::vector<STRING> Textures;\
	Textures.push_back(texture);\
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) {CREATE_MODELGL3_IF_NOT_EXCEPTION(model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);}\
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) {CREATE_MODELDX_IF_NOT_EXCEPTION(model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);}\
	if (shader_type == SHADER_TEXTURE_LIGHT) ASSERT(model->LoadLight(texture, SystemHandle->m_Driver, shader_type, &Textures, &vertexVector, &IndexList));\
}

#define initLoadTextureLight2D(model, texture, vertexVector, IndexList, shader_type)\
{\
	std::vector<STRING> Textures; \
	Textures.push_back(texture); \
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(model, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }\
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION(model, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }\
	if (shader_type == SHADER_TEXTURE_LIGHT) ASSERT(model->LoadLight(texture, SystemHandle->m_Driver, shader_type, &Textures, &vertexVector, &IndexList)); \
}

#if defined DX_ENGINE
//////////////
// INCLUDES //
//////////////
#if defined DX9sdk
#include "DX9Class.h"
#endif

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

#include "DXshaderClass.h"
#include "virtualModelClass.h"
#include "DXbasicTypes.h"

namespace DirectX 
{
// --------------------------
// "OBJ" MATERIAL FORMAT:
// --------------------------
struct SurfaceMaterial
{
	char matName[100];					//size:100 100xchar

	XMFLOAT4 diffuseColor;				//size:16
	XMFLOAT4 ambientColor;				//size:16
	XMFLOAT4 emissiveColor;				//size:16

	int texArrayIndex;					//size:4
	bool hasTexture;					//size:1

	bool transparent;					//size:1	>= 43
    ID3D11ShaderResourceView* alfaMap11;//size:8	>= 43

    bool bSpecular;						//size:1	>= 44: NEW SPECULAR + SHININESS:
    XMFLOAT3 specularColor;				//size:12	>= 44: NEW SPECULAR + SHININESS:
    int nShininess;						//size:4	//>= 44: NEW SPECULAR + SHININESS:

    bool hasNormMap;					//size:1	>=47: NEW BUMP
    int normMapTexArrayIndex;			//size:4	>=47: NEW BUMP
	//							TOTAL		 184
};


////////////////////////////////////////////////////////////////////////////////
// Class name: DXmodelClass
////////////////////////////////////////////////////////////////////////////////
class DXmodelClass : public VirtualModelClass
{
public:
	UINT WomaIntegrityCheck = 1234567891;
	DXmodelClass(bool model3d, PRIMITIVE_TOPOLOGY = TRIANGLELIST, bool computeNormals = false, bool modelHASshadow = false, bool modelRENDERshadow = false);
	~DXmodelClass();
	void Shutdown();
	void LOADDRIVER(void* driver);

	void RenderWithFade(WomaDriverClass* driver, float fadeLight = 1);
	void RenderSky(WomaDriverClass* driver, UINT camera, float fadeLight = 1);
	void Render(WomaDriverClass* driver, UINT camera = 0, UINT projection=0, UINT pass=0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL);

	// ----------------------------------------------------------------------
	bool RenderSprite(void* Driver /*ID3D11DeviceContext* deviceContext*/, int positionX, int positionY, float scale=1.0f);
	bool UpdateBuffersRotY(void* Driver /*ID3D11DeviceContext* deviceContext*/, int positionX, int positionY);

	// BASIC LOAD:
	bool LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);
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
		std::vector<ID3D11ShaderResourceView*> meshSRV11;	// vector with all pointer(s) to textures loaded
	#ifdef DX9sdk
		std::vector<IDirect3DBaseTexture9*> meshSRV;	// vector with all pointer(s) to textures loaded
	#endif

// DX Specific:
#if defined DX9sdk
	DXshaderClass*	m_Shader9;
	D3DXMATRIX		m_worldMatrix9;
#else
	DXshaderClass*	m_Shader11=NULL;
	DXshaderClass*	m_Shader = NULL;

	XMMATRIX		m_worldMatrix;
#endif

	XMFLOAT4 objectCenterOffset;
	XMFLOAT3 minVertex;
	XMFLOAT3 maxVertex;

// ----------------------------------------------------------------------
private:
// ----------------------------------------------------------------------

	DXshaderClass* CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType);
	bool InitializeDXbuffers(TCHAR* objectName, std::vector<STRING>* textureFile=NULL);
	bool CreateDXbuffers(UINT sizeofMODELvertex, /*ID3D11Device*/ void* device, void* indices, void* vertices);
	void SetBuffers(void* deviceContext);	//ID3D11DeviceContext

	void UpdateDynamic(void* Driver, std::vector<ModelColorVertexType>* lightVertexVector);

	std::vector<ModelColorVertexType>* modelColorVertex;				// MODEL!
	std::vector<ModelColorVertexType> modelColorVertex_;				// LOAD M3D
	bool InitializeColorBuffers(void* g_driver, void* indices);
	std::vector<ModelTextureVertexType>* modelTextureVertex;			// MODEL!
	std::vector<ModelTextureVertexType> modelTextureVertex_;			// LOAD M3D
	bool InitializeTextureBuffers(void* g_driver, void* indices);
	std::vector<ModelTextureLightVertexType>* modelTextureLightVertex;	// MODEL!
	std::vector<ModelTextureLightVertexType> modelTextureLightVertex_;	// LOAD M3D
	bool InitializeTextureLightBuffers(void* g_driver, void* indices);

	// VARS:
	// ----------------------------------------------------------------------
	
#if defined DX9sdk
	DirectX::DX9Class* m_driver9=NULL;
#endif
	DirectX::DX11Class* m_driver11 = NULL;
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	DirectX::DX12Class* m_driver = NULL;
#endif

	UINT sizeofMODELvertex;

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

	ID3D11Buffer	*m_vertexBuffer11 = NULL;
	ID3D11Buffer	*m_indexBuffer11 = NULL;

#if defined DX9sdk
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer9;
	LPDIRECT3DINDEXBUFFER9  indexBuffer9;
#endif

	std::vector<UINT>* indexModelList;

	float	m_xTexture=0;

};

}

#endif
