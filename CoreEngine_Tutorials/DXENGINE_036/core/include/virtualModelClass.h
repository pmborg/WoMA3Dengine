// --------------------------------------------------------------------------------------------
// Filename: virtualModelClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once
#ifndef virtualmodelclass_h
#define virtualmodelclass_h

#include "platform.h"
#if defined WINDOWS_PLATFORM
#include <DirectXMath.h>
using namespace DirectX;
#else
#include "GLmathClass.h"
#endif

#include "womadriverclass.h"

//
// USAGE: D3D_PRIMITIVE_TOPOLOGY_LINELIST + PrimitiveTopology
//
enum PRIMITIVE_TOPOLOGY
{
	//MORE INFO: https://msdn.microsoft.com/en-us/library/windows/desktop/bb205124(v=vs.85).aspx
	NONNE = 0,
	POINTLIST,
	LINELIST,		// Sun Ray														2 Vertex = 1 LINE  --> DrawPrimitive( D3DPT_LINELIST, 0, 1 );
	LINESTRIP,		// ...not used...												6 Vertex = 5 LINEs --> DrawPrimitive( D3DPT_LINESTRIP, 0, 5 );
	TRIANGLELIST,	// DxModel default Defined in (CREATE_MODEL_IF_NOT_EXCEPTION)   1 Triang. = 3 Vert.--> DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
	TRIANGLESTRIP	// Used on Optimized Sky Mesh: Sky/Sun/Moon Terrain          	4 Triang. = 6 Vert.--> DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
};


// --------------------------
// "OBJ" MATERIAL FORMAT:
// --------------------------
#pragma pack(push, 1)
struct SurfaceMaterial 
{
	char matName[100] = {};				//size:100 100xchar
#ifdef WINDOWS_PLATFORM
	XMFLOAT4 diffuseColor = {};			//size:16
	XMFLOAT4 ambientColor = {};			//size:16
	XMFLOAT4 emissiveColor = {};		//size:16
#else
	Vector4 diffuseColor = {};			//size:16
	Vector4 ambientColor = {};			//size:16
	Vector4 emissiveColor = {};			//size:16
#endif

	int texArrayIndex = NULL;			//size:4
	bool hasTexture = false;			//size:1
	bool transparent;					//size:1	>= 33

#if defined DX11 ||	defined DX9			
	void* alfaMap11 = NULL;				//size:8	>= 33	DX11 & DX9(apidx11)
#endif
#if defined DX12 || defined VULKAN
	void* alfaMap = NULL;				//size:8	// >=33 DX12 & Vulkan
#endif

	bool bSpecular = false;				//size:1	>= 34: NEW SPECULAR + SHININESS:
#ifdef WINDOWS_PLATFORM
	XMFLOAT3 specularColor = {};		//size:12	>= 34: NEW SPECULAR + SHININESS:
#else
	Vector3 specularColor = {};		//size:12	>= 34: NEW SPECULAR + SHININESS:
#endif
	int nShininess = NULL;				//size:4	>= 34: NEW SPECULAR + SHININESS:

	bool hasNormMap = false;			//size:1	>= 37: NEW BUMP
	int normMapTexArrayIndex = NULL;	//size:4	>= 37: NEW BUMP
};
#pragma pack(pop)

struct ADVOBJ3D 
{
	STRING fileNameOnly = TEXT("");

	//Vertex definition indices
	std::vector<int> vertPosIndex = {};			//vertPos Indexs
	std::vector<int> vertTCIndex = {};			//vertTexCoord Indexs
	std::vector<int> vertNormIndex = {};		//vertNorm Indexs

	UINT	triangleCount = 0;					//Total Triangles
	UINT	meshTriangles = 0;

	std::vector<STRING>		meshMaterials = {};

	std::vector<XMFLOAT3>	vertPos = {};		//30
	std::vector<XMFLOAT2>	vertTexCoord = {};	//31
	std::vector<XMFLOAT3>	vertNorm = {};		//32

	//Make sure we have a default if no tex coords or normals are defined
	bool hasTexCoord = false;					//ch07
	bool hasNorm = false;						//ch12
	bool hasRenderShadow = false;				//ch35
	bool hasNormMap = false;					//ch51
	bool hasTransparent = false;				// has transparent sub-meshes?

	UINT meshSubsets = 0;						// Num. of sub-meshes

	std::vector<UINT> indices32 = {};			// DX >= 9.2
	std::vector<WORD> indices16 = {};			// DX >= 9.0 Shader 2.0

	std::vector<int> meshSubsetIndexStart = {};	// Start Index of each subset
	std::vector<int> subsetMaterialArray = {};	// Index of material to use in each subset

	std::vector<SurfaceMaterial> material = {};
	std::vector<STRING> textureNameArray = {};	// filename of all textures loaded

	//---------------------------------------------------------------------
	UINT m_vertexCount = 0, m_indexCount = 0;			//virtualModelClass.h
	bool	ModelHASNormals = false;
};

////////////////////////////////////////////////////////////////////////////////	
// Class name: virtualModelClass
////////////////////////////////////////////////////////////////////////////////
class VirtualModelClass
{
public:
	virtual void Shutdown() = 0;

	virtual void RenderWithFade(void* pContext, float fadeLight = 1, bool FOG = false, UINT pass = 0, void* lightViewMatrix = NULL, void* lightProjectionMatrix = NULL) = 0;
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
	virtual void RenderSky(void* pContext, UINT camera, float fadeLight = 1) = 0;
#endif
	virtual void Render(void* pContext, UINT threadID=0, UINT camera = 0, UINT projection = 0, UINT pass = 0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL, bool disableFog = false) = 0;

	virtual bool LoadColor(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0) = 0;
	virtual bool LoadTexture(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0) = 0;
	virtual bool LoadLight(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureLightVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0) = 0;
	virtual bool LoadBump(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelNormalBumpVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0) = 0;

#if defined USE_VIEW2D_SPRITES // Sprites
	virtual bool RenderSprite(void* pContext, int positionX, int positionY, float scale = 1.0f, float fade = 1.0f) = 0;
	virtual bool UpdateBuffersRotY(void* pContext, int positionX, int positionY) = 0;
	virtual bool UpdateSpriteBuffersRotY(void* pContext, int positionX, int positionY) = 0;
#endif
#if defined MAIN_RENDER_LIGHT_RAY
	virtual void UpdateLightRayVertices(void* pContext, std::vector<ModelColorVertexType>* lightVertexVector) = 0;
#endif

	ADVOBJ3D obj3d;
	virtual bool LoadModel(void* pContext, TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow = false, bool renderShadow = false, UINT instanceCount = 0) = 0;

	virtual bool LoadWaterFall(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureWaterfall>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0) = 0;
	virtual bool LoadSmoke(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<SmokeVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount = 0) = 0;
	virtual void SetAlpha(float a) = 0;

	virtual void Identity() = 0;
	virtual void multiply(void* m) = 0;	//XMMATRIX* or mat4*

	virtual void rotateX(float rad) = 0;
	virtual void rotateY(float rad) = 0;
	virtual void rotateZ(float rad) = 0;
	virtual void scale(float x, float y, float z) = 0;
	virtual void translation(float x, float y, float z) = 0;

	//VARS:
	// ----------------------------------------------------------------------
	STRING MODEL_NAME = TEXT("");

	INT					m_ObjId = -1;
    INT					xmlId = -1;
	SHADER_TYPE			ModelShaderType = SHADER_AUTO;	//Used by shader.cpp
	PRIMITIVE_TOPOLOGY  PrimitiveTopology;
	UINT				m_vertexCount = 0, m_indexCount = 0;

	float				PosX = 0, PosY = 0, PosZ = 0;

	bool				Model3D = false;
	bool				ModelHASfog = false;
	bool				ModelHASlight = false;
	bool				ModelHASAlfaColor = false;
	bool				ModelHASColorMap = false;
	float				ModelAlfaColor = 1;

	int					shaderTypeParameter = 0;// level62

	bool				isBill = false;
	bool				isAnimatedBill = false;

	float				boundingSphere = 0;
	bool				ModelHASNormals = false;
	bool				ModelcomputeNormals = false;

	int					SpriteTextureWidth = 0;		//NOTE: Have to be "int"
	int					SpriteTextureHeight = 0;	//NOTE: Have to be "int"

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	bool				ModelCastShadow = false;
	bool				ModelRenderShadow = false;
#endif

	UINT			m_instanceCount = 0;
};

#endif
