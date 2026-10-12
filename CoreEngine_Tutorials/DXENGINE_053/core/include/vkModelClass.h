// --------------------------------------------------------------------------------------------
// Filename: vkModelClass.h
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

//////////////
// INCLUDES //
//////////////
#include "platform.h"

#if CORE_ENGINE_LEVEL >= 10 && defined VULKAN
#include "virtualModelClass.h"
#include "GLmathClass.h"		//GL & VULKAN
#include "vkClass.h"
#include "vkShaderClass.h"
#include "mem_leak.h"

#include "fileLoader.h"

#include "glTextureclass.h"

#include "modelClass.h"

//| Criação    | `Model3D` | `PrimitiveTopology`  | Significado                |
//| -------- - | --------: | ------------------ - | -------------------------- |
//| `I_AM_3D`  | `true`    | `TRIANGLELIST`       | objeto 3D                  |
//| `I_AM_2D`  | `false`   | `TRIANGLELIST`       | objeto 2D                  |
//| `I_AM_2D`  | `false`   | `LINELIST`           | **linha**, como o LightRay |


////////////////////////////////////////////////////////////////////////////////
// Class name: vkModelClass
////////////////////////////////////////////////////////////////////////////////
class vkModelClass : public VirtualModelClass
{
public:
	vkModelClass(bool model3d, PRIMITIVE_TOPOLOGY = TRIANGLELIST, bool computeNormals = false, bool modelHASshadow = false, bool modelRENDERshadow = false);
	~vkModelClass();
	void Shutdown();

	void RenderWithFade(void* pContext, float fadeLight = 1, bool FOG = false, UINT pass = 0, void* lightViewMatrix = NULL, void* lightProjectionMatrix = NULL);
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
	void RenderSky(void* pContext, UINT camera, float fadeLight = 1);
#endif
	void Render(void* pContext, UINT threadID = 0, UINT camera = 0, UINT projection = 0, UINT pass = 0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL, bool disableFog = false);

	bool LoadColor(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0);
	bool LoadTexture(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0);
	bool LoadLight(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureLightVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0);
	bool LoadBump(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelNormalBumpVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0);

	bool SaveW3D(STRING filename, void* indices, void* vertices, UINT sizeofMODELvertex);

#if defined LOADW3D
	bool LoadW3D(void* pContext, SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow = false, bool renderShadow = false, UINT instanceCount = 0);
	bool LoadW3DfromMEM(void* pContext, SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow = false, bool renderShadow = false, UINT instanceCount = 0);
#endif

	bool LoadTerrain(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureDouble_Color_TerrainType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0);
//#if DX_ENGINE_LEVEL >= 97
	bool LoadWaterFall(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureWaterfall>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0);
	bool LoadSmoke(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<SmokeVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount = 0);
	void SetAlpha(float a);
//#endif

#if defined USE_VIEW2D_SPRITES // Sprites:24
	bool RenderSprite(void* pContext, int positionX, int positionY, float scale = 1.0f, float fade = 1.0f);
	bool UpdateBuffersRotY(void* pContext, int positionX, int positionY);
	bool UpdateSpriteBuffersRotY(void* pContext, int positionX, int positionY);
#endif

#if defined MAIN_RENDER_LIGHT_RAY
	void UpdateLightRayVertices(void* pContext, std::vector<ModelColorVertexType>* lightVertexVector);
#endif

	ModelClass modelClass;
	void UpdateAlphaMapDescriptor(vkShaderClass* useShader, UINT frame, UINT descriptorIndex);
	void RenderSubMesh(void* pContext, WomaDriverClass* driver, mat4* m_world, mat4* viewRender, mat4* projectionMatrix, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);

	vkShaderClass* CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType, UINT monitor = 0);
	bool InitializeVkBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile = NULL);
	void SetGeometryBuffers(void* pContext, UINT pass);

	// Math Aux Functions //
	void Identity();
	void multiply(void* m);	//XMMATRIX* or mat4*

	void rotateX(float rad);
	void rotateY(float rad);
	void rotateZ(float rad);

	void scale(float x, float y, float z);
	void translation(float x, float y, float z);

	mat4 m_worldMatrix;
	mat4 m_viewMatrix;
	mat4 projectionMatrix;

	//STRING MODEL_NAME = TEXT("");
	std::vector<UINT>* indexModelList;

	GLtextureClass* m_Texture = NULL;		// Image loader
	std::vector<GLtextureClass*> meshSRV;	// meshSRV

	std::vector<ModelColorVertexType>* modelColorVertex = NULL;			// MODEL!
	std::vector<ModelColorVertexType> modelColorVertex_;				// 39: LOAD W3D

	bool InitializeColorBuffers(void* pContext, TCHAR* objectName, void* indices);
	bool CreateVKbuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile, SHADER_TYPE shaderType = SHADER_AUTO);
	VkDeviceSize indexBufferSize = 0;
	VkDeviceSize vertexBufferSize = 0;
	VkDeviceSize currentVertexBufferSize = 0;
	bool FRAME[MAX_FRAMES_IN_FLIGHT] = { false };

	std::vector<ModelTextureVertexType>* modelTextureVertex = NULL;		// MODEL!
	std::vector<ModelTextureVertexType> modelTextureVertex_;			// 39: LOAD W3D

	bool InitializeTextureBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile);
	std::vector<ModelTextureLightVertexType>* modelTextureLightVertex = NULL;	// MODEL!
	std::vector<ModelTextureLightVertexType> modelTextureLightVertex_;			// 39: LOAD W3D
	bool InitializeTextureLightBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile);

	float	model_fade = 1;
	float	m_xTexture = 0;

	//ADVOBJ3D obj3d;
	bool LoadModel(void* pContext, TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow = false, bool renderShadow = false, UINT instanceCount = 0);

	VkDescriptorImageInfo alfatextureDescriptor = {};
	VkWriteDescriptorSet alfatextureWrite = {};

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	UINT m_currentpass=0;
	vkShaderClass* m_ShaderShadowMap = NULL;
	std::vector<ModelShadowMapVertexType>* modelShadowMapVertex = NULL;	// MODEL!
	std::vector<ModelShadowMapVertexType> modelShadowMapVertex_;		// LOAD W3D
	bool InitializeShadowMapBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile);
#endif

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES
	VkBuffer       m_instanceBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_instanceBufferMemory = VK_NULL_HANDLE;
#endif

	bool InitializeTextureDouble_Color_Terrain(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile);

	// ----------------------------------------------------------------------
	UINT* indices = NULL;
	void GetIndices();
	vkShaderClass* m_Shader = NULL;

	vkClass* m_driver = NULL;

private:
	GLmathClass	mathClass;

	std::vector<VkBuffer>       vertexBuffers;
	std::vector<VkDeviceMemory> vertexMemories;

	VkBuffer       m_vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;


	VkBuffer       m_indexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;


	uint32_t m_vertexCount = 0;
	uint32_t m_indexCount = 0;

	// Per-model descriptor cache.
	// Each vkModelClass supports up to MAX_VULKAN_TEXTURES_MODEL texture slots
	// (texArrayIndex = 0 .. MAX_VULKAN_TEXTURES_MODEL - 1).
	VkDescriptorImageInfo textureDescriptor[MAX_VULKAN_TEXTURES_MODEL] = {};
	VkWriteDescriptorSet  textureWrite[MAX_VULKAN_TEXTURES_MODEL] = {};

	std::vector<ModelNormalBumpVertexType>* modelNormalBumpVertex = NULL;	// MODEL!
	std::vector<ModelNormalBumpVertexType> modelNormalBumpVertex_;			// LOAD W3D
	bool InitializeTextureNormalBumpBuffers(void* pContext, TCHAR* objectName, std::vector<STRING>* textureFile);

	VkDescriptorImageInfo bumptextureDescriptor = {};
	VkWriteDescriptorSet bumptextureWrite = {};
	GLtextureClass* currentNormalMap = NULL;

	VkBuffer       m_vertexBuffer_4Shadows = VK_NULL_HANDLE;
	VkDeviceMemory m_vertexBufferMemory_4Shadows = VK_NULL_HANDLE;
	VkDescriptorImageInfo shadowtextureDescriptor = {};
	VkWriteDescriptorSet shadowtextureWrite = {};
	GLtextureClass* currentShadowMap = NULL;

	std::vector<ModelTextureDouble_Color_TerrainType>* ModelTextureDouble_Color_TerrainTypeVertex=NULL;

};

#endif
