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

#include "dx11Class.h"

#if D3D11_SPEC_DATE_YEAR > 2009
#define _11 r[0].m128_f32[0]
#define _12 r[0].m128_f32[1]
#define _13 r[0].m128_f32[2]
#define _14 r[0].m128_f32[3]

#define _21 r[1].m128_f32[0]
#define _22 r[1].m128_f32[1]
#define _23 r[1].m128_f32[2]
#define _24 r[1].m128_f32[3]

#define _31 r[2].m128_f32[0]
#define _32 r[2].m128_f32[1]
#define _33 r[2].m128_f32[2]
#define _34 r[2].m128_f32[3]

#define _41 r[3].m128_f32[0]
#define _42 r[3].m128_f32[1]
#define _43 r[3].m128_f32[2]
#define _44 r[3].m128_f32[3]
#endif

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
	ASSERT(model->LoadTexture(texture, SystemHandle->m_Driver, shader_type, &Textures, &vertexVector, &IndexList)); \
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



#include "DXshaderClass.h"
#include "virtualModelClass.h"
#include "DXbasicTypes.h"

#include "modelClass.h"


namespace DirectX 
{


////////////////////////////////////////////////////////////////////////////////
// Class name: DXmodelClass
////////////////////////////////////////////////////////////////////////////////
class DXmodelClass : public VirtualModelClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;

	DXmodelClass(bool model3d, PRIMITIVE_TOPOLOGY = TRIANGLELIST, bool computeNormals = false, bool modelHASshadow = false, bool modelRENDERshadow = false);
	~DXmodelClass();
	void Shutdown();
	void LOADDRIVER(void* driver);

	void RenderWithFade(WomaDriverClass* driver, float fadeLight = 1);
	void RenderSky(WomaDriverClass* driver, UINT camera, float fadeLight = 1);
	void Render(WomaDriverClass* driver, UINT camera = 0, UINT projection=0, UINT pass=0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL);

	// ----------------------------------------------------------------------
	bool RenderSprite(void* Driver , int positionX, int positionY, float scale=1.0f, float fade = 1.0f);
	bool UpdateBuffersRotY(void* Driver , int positionX, int positionY);

	// BASIC LOAD:
	bool LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);
	
		// [PATTERN] Image loader:
		ID3D11ShaderResourceView* m_Texture11 = NULL;
	#if defined DX9sdk
		LPDIRECT3DTEXTURE9 m_Texture9 = NULL;
	#endif
	
	bool LoadTexture(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);
	bool LoadLight(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureLightVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);

	// MODEL LOAD:
	OBJ3D obj3d;
	bool LoadModel	(TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow = false, bool renderShadow=false, UINT instanceCount=0);

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
		std::vector<ID3D11ShaderResourceView*> meshSRV11;	// vector with all pointer(s) to textures loaded
	#ifdef DX9sdk
		std::vector<IDirect3DBaseTexture9*> meshSRV9;	// vector with all pointer(s) to textures loaded
	#endif

// DX Specific:
#if defined DX9sdk
	DXshaderClass*	m_Shader9;
	D3DXMATRIX		m_worldMatrix9;
#else
	DXshaderClass*	m_Shader11=NULL;

	XMMATRIX		m_worldMatrix;
#endif

	XMFLOAT4 objectCenterOffset = XMFLOAT4(0, 0, 0, 0);
	XMFLOAT3 minVertex = XMFLOAT3(0, 0, 0);
	XMFLOAT3 maxVertex = XMFLOAT3(0, 0, 0);

	//std::vector <CD3DX12_CPU_DESCRIPTOR_HANDLE> cbvHandle;
	HRESULT LoadTextureImage(TCHAR* textureFilename);

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

	ModelClass modelClass;
	void RenderSubMesh(WomaDriverClass* driver, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix,
						UINT pass, XMMATRIX* lightViewMatrix = NULL, XMMATRIX* ShadowProjectionMatrix = NULL);

	// VARS:
	// ----------------------------------------------------------------------
	
#if defined DX9sdk
	DirectX::DX9Class* m_driver9=NULL;
#endif
	DirectX::DX11Class* m_driver11 = NULL;

	UINT sizeofMODELvertex;

	float model_fade = 1;

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
