// --------------------------------------------------------------------------------------------
// Filename: vkShaderClass.h
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

#ifndef vkShaderClass_h_
#define vkShaderClass_h_

#include "platform.h"
#include <fstream>
using namespace std;

#include "GLmathClass.h"  // Generic math (OpenGL/Vulkan) --> TODO: rename to MathClass.h
#include "virtualShaderClass.h"
#include "virtualModelClass.h"
#include "vkClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: vkShaderClass
////////////////////////////////////////////////////////////////////////////////
class vkShaderClass : public virtualShaderClass
{
public:
	vkShaderClass();
	~vkShaderClass();

	SHADER_TYPE m_shaderType;

	bool Initialize(INT Id, TCHAR* objectName, SHADER_TYPE shaderType, void* DriverDevice, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology, UINT monitor = 0, bool useGS = false);
	void Shutdown();
	void createDescriptorPool();
	bool createDescriptorSet(VkDevice DriverDevice);
	bool SetShaderParameters(UINT pass, void* Device_Context, 
	mat4* worldMatrix, mat4* viewMatrix, mat4* projectionMatrix, 
	mat4* lightViewMatrix=NULL, mat4* ShadowProjectionMatrix = NULL, float m_particleAlpha=0);
	void SetupRenderPipe(UINT monitor);
	void Render(UINT pass, void* Device_Context, int indexCount, mat4* worldMatrix, mat4* viewMatrix, mat4* projectionMatrix, UINT monitor = 0);
	void RenderShader(UINT pass, void* Device_Context, int texture_index, int indexCount, int start = 0);

private:
	bool InitializeShader(SHADER_TYPE shaderType, void* DriverDevice, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology, UINT monitor = 0);
	bool LoadShaderBinaryFile(TCHAR* filename, std::vector<char>& code);

	void ShutdownShader();

private:
	VkDevice m_device = NULL;
	std::vector<char> vertexCode;
	std::vector<char> fragmentCode;
	vkClass* vkDriver = NULL;



public:
	VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

	// VARS:
	// ----------------------------------------------------------------------
	STRING vsFilename = TEXT("");
	STRING psFilename = vsFilename;

	// COLOR SHADER:
	VkDescriptorSetLayout m_descriptorColorSetLayout = VK_NULL_HANDLE;

	// TEXTURE SHADER:
	VkDescriptorSetLayout m_descriptorTextureSetLayout = VK_NULL_HANDLE;

	void AllocateSubsetDescriptors(UINT subsetCount, VkDevice DriverDevice);

	// LIGHT SHADER:
	VkDescriptorSetLayout m_descriptorTextureLightSetLayout = VK_NULL_HANDLE;

	// LIGHT + ALFA MAP SHADER:
	VkDescriptorSetLayout m_descriptorTextureLightAlphaMapSetLayout = VK_NULL_HANDLE;

	// BUMP SHADER:
	VkDescriptorSetLayout m_descriptorTextureBumpSetLayout = VK_NULL_HANDLE;

	// SHADOW PASS (shadow.vert/shadow.frag)
	VkDescriptorSetLayout m_descriptorShadowSetLayout = VK_NULL_HANDLE;

	// LIGHT + SHADOW PASS (lightShadow.vert/lightShadow.frag)
	VkDescriptorSetLayout m_descriptorTextureLightShadowSetLayout = VK_NULL_HANDLE;

#if DX_ENGINE_LEVEL >= 41 && defined USE_SHADOW_INSTANCES	//41: Aux. Shader (render in texture), but using Instances
	VkDescriptorSetLayout m_descriptorShadowInstancedSetLayout = VK_NULL_HANDLE;
#endif

	VkDescriptorSetLayout m_descriptorTerrain60SetLayout = VK_NULL_HANDLE;

	// COMMON:
	// ----------------------------------------------------------------------
	STRING MODEL_NAME;
	INT    m_ObjId = 0;

	mat4 WV;
	mat4 WVP;

	VkShaderModule	m_vertexShaderModule = VK_NULL_HANDLE;
	VkShaderModule	m_fragmentShaderModule = VK_NULL_HANDLE;

#if   DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	#define WOMA_NUM_RENDERPASS_PIPELINES 2
#else
	#define WOMA_NUM_RENDERPASS_PIPELINES 1
#endif


	//  [0]  WINDOW
	//	└── vkWindow.m_render_pass
	//	viewport Window
	//
	//	[1]  SHADOW
	//	└── m_TextureWithShadows->m_renderPass
	//	viewport Shadow
	//
	//	[2]  MAIN MAP
	//	└── m_RenderMapTexture->m_renderPass
	//	viewport 1000 x 1000
	//
	//	[3]  MINI MAP
	//	└── m_MiniMapBitmapTexture->m_renderPass
	//	viewport MiniMap
	//							[zBufferMode][cullMode][fillMode][pipelineState][Rpass]
	VkPipeline m_pipelineState	[2][3][2][MAXNUM_PIPELINE_STATES][WOMA_NUM_RENDERPASS_PIPELINES] = {};

	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

	// -------------------------------------------------------------------------
	// Legacy Descriptor Sets
	// Levels 21-36
	// -------------------------------------------------------------------------
	// Used by:
	//   SHADER_COLOR						//21...
	//   SHADER_TEXTURE						//22-29
	//   SHADER_TEXTURE_FONT				//27...
	//   SHADER_TEXTURE_LIGHT_SAVESHADOW	//36
	VkDescriptorSet m_descriptorSet30[MAX_FRAMES_IN_FLIGHT] = {};

	// -------------------------------------------------------------------------
	// Multi-subset Descriptor Sets
	// Introduced with the Level 23 lighting pipeline.
	// (Actually required from Level 31 onwards, but only exists where the
	// lighting shaders exist.)
	// -------------------------------------------------------------------------
	//SHADER_TEXTURE_LIGHT					//23
	//SHADER_TEXTURE_LIGHT_ALPHAMAP			//34
	//SHADER_NORMAL_BUMP					//35
	//SHADER_TEXTURE_LIGHT_RENDERSHADOW		//36
	std::vector<VkDescriptorSet> m_descriptorSet[MAX_FRAMES_IN_FLIGHT] = {};


	inline VkDescriptorSet& GetDescriptorSet(UINT frame, UINT subset = 0)
	{
		if (singleDescriptorSubset)
			return m_descriptorSet30[frame];

		return m_descriptorSet[frame][subset];
	}

	bool singleDescriptorSubset = false;

	UINT descriptorSubsetCount = 0;

	UINT currentDescriptorSubset = 0;

#pragma pack(push, 1)
	// ------------------------------------------------------------------------------------------------
	// BUFFERS:
	// ------------------------------------------------------------------------------------------------
	VkBuffer m_uniformBufferVS = VK_NULL_HANDLE;
	VkDeviceMemory m_uniformBufferVSMemory = VK_NULL_HANDLE;

	//
	// VS SHADER_COLOR | SHADER_TEXTURE | SHADER_TEXTURE_FONT | SHADER_TEXTURE_LIGHT_SAVESHADOW
	//
	// CBUFFER:
	struct vkUniformBufferObject
	{
		mat4	WVP;
		mat4	WV;
		BOOL	VShasFog=0;
		float	VSfogStart=0;
		float	VSfogEnd=0;
		BOOL	Pad1=0;
	};
	vkUniformBufferObject ubo;

	//
	// VS SHADER_TEXTURE_LIGHT_INSTANCED
	//
	// CBUFFER:
	struct vkUniformBufferObject40
	{
		mat4 worldMatrix;
		mat4 view;
		mat4 projection;
		//mat4 WV;
		mat4 WVP;

		float VSrotY;
		float padding0;
		float padding1;
		float padding2;
	};
	vkUniformBufferObject40 ubo40;

	VkBuffer m_uniformBufferPS = VK_NULL_HANDLE;
	VkDeviceMemory m_uniformBufferPSMemory = VK_NULL_HANDLE;


//
// VS 23..25
//

//
// PS 26..35
//

	struct vkVSconstantBufferType
	{
		// BLOCK VS1: 80 bytes
		mat4 world;
		mat4 view;
		mat4 projection;
		mat4 WV;
		mat4 WVP;

		// BLOCK VS2: 16 bytes
		BOOL VShasLight;
		BOOL VShasSpecular;
		BOOL VShasNormMap;
		BOOL VShasFog;

		// BLOCK VS3: 64 bytes
		XMFLOAT4 VSlightDirection;
		XMFLOAT4 VSambientColor;
		XMFLOAT4 VSdiffuseColor;
		XMFLOAT4 VSemissiveColor;

		// BLOCK VS4: 16 bytes
		float		VSfogStart;
		float		VSfogEnd;
		BOOL		VShasShadowMap;
		BOOL		VS_USE_WVP;

		// 36 BLOCK: VS5
		mat4		ViewToLightProj;
		mat4		WorldInverseTranspose;
		XMFLOAT3	VSCameraPosition;
		BOOL		Pad1;

		// 42 BLOCK: VS6
		float		VSrotX;
		float		VSrotY;
		float		VSrotZ;
		float		time;				// Water animation: 54

		// 42 BLOCK: VS7
		float		VSshaderType;
		bool		vsIsSky = false;
		float		vsPAD3;
		float		vsPAD4;
	};
	vkVSconstantBufferType dataVSptr = {};

// ------------------------------------------------------------------------------------------------

	struct PSPushConstants
	{
		// -----------------------------------------------------------------
		// BLOCK 1 (16 bytes) - LEVEL 27
		// DX11: PixelShaderBuffer::pixelColor
		// -----------------------------------------------------------------
		XMFLOAT4 pixelColor;

		// -----------------------------------------------------------------
		// BLOCK 2 (16 bytes) - LEVEL 27
		// Per-draw shader state.
		// -----------------------------------------------------------------
		int hasTexture;
		int hasLight;
		int hasSpecular;
		int isFont;

		// BLOCK 3: 64 bytes (is on Constant BUFFER)
		//vec4 ambientColor;
		//vec4 diffuseColor;
		//vec4 emissiveColor;
		//vec4 lightDirection;

		// -----------------------------------------------------------------
		// BLOCK 4 (16 bytes) - LEVEL 36
		// Shadow / lighting state.
		// -----------------------------------------------------------------
		int		hasColorMap = 0;		// 66
		float	lightType = 0;			// Light Type
		int		isDay = 0;				// level98
		float	shaderTypeParameter = 0;// level62

		// -----------------------------------------------------------------
		// BLOCK 5 (16 bytes) - LEVEL 33
		// Alpha state.
		// -----------------------------------------------------------------
		int   hasAlfaColor;
		float alfaColor;
		float pad1;
		float pad2;

		// -----------------------------------------------------------------
		// BLOCK 6 (16 bytes) - LEVEL 34
		// Material/Fog state.
		// -----------------------------------------------------------------
		int hasFog;
		int isSky;
		int hasAlfaMap;
		int hasNormMap;
	};

	PSPushConstants dataPSPushConstantsPtr = {};

	struct vkPSconstantBufferType
	{
		// BLOCK1
//#ifdef WINDOWS_PLATFORM
//		XMFLOAT4 pixelColor;
//#else
//		float pixelColor[4];
//#endif
//
//		// BLOCK2
//		BOOL hasTexture;
//		BOOL hasLight;
//		BOOL hasSpecular;
//		BOOL isFont;

		// BLOCK3
//#ifdef WINDOWS_PLATFORM
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT4 emissiveColor;
		XMFLOAT4 lightDirection;
//#else
//		Vector4 ambientColor;
//		Vector4 diffuseColor;
//		Vector4 emissiveColor;
//		Vector4 lightDirection;
//#endif

//		// BLOCK4:
//		bool	hasColorMap = false;	// 66
//		float	lightType = 0;			// Light Type
//		bool	isDay = 0;				// level98
//		float	shaderTypeParameter = 0;// level62

		// BLOCK5:
		BOOL		pad1; //old: hasAlfaColor;
		float		pad2; //old: alfaColor;
		float		fade;
		float		frameTime;

//		// BLOCK6:
//		BOOL		hasFog;
//		BOOL		isSky = false;
//		BOOL		hasAlfaMap;
//		BOOL		hasNormMap;

		// BLOCK7:
	//#ifdef WINDOWS_PLATFORM
		XMFLOAT3	cameraPosition;
		BOOL		castShadow;
		XMFLOAT3	specularColor;
		float		nShininess;
	//#else
	//	Vector3		cameraPosition;
	//	BOOL		castShadow;
	//	Vector3		specularColor;
	//	float		nShininess;
	//#endif
	};

	vkPSconstantBufferType dataPSptr = {};
#pragma pack(pop)


	// --------------------------------------------------------------------------------------------
	// Internal Shader VARs to Copy to Buffers: VS/PS
	// --------------------------------------------------------------------------------------------
	// BLOCK1:
//#ifdef WINDOWS_PLATFORM
	XMFLOAT4	pixelColor = {};
//#else
//	Vector4		pixelColor = {};
//#endif

	// BLOCK2:
	BOOL		hasTexture = false;
	BOOL		hasLight = false;
	BOOL		hasSpecular = false;
	BOOL		isFontShader = false;
	 
	// BLOCK3:
//#ifdef WINDOWS_PLATFORM
	XMFLOAT4	ambientColor = {};		// LIGHT: Ka
	XMFLOAT4	diffuseColor = {};		// LIGHT: Kd
	XMFLOAT4	emissiveColor = {};		// LIGHT: Ke
	//			lightDirection (AUTO)
//#else
//	Vector4		ambientColor = {};	// LIGHT: Ka
//	Vector4		diffuseColor = {};	// LIGHT: Kd
//	Vector4		emissiveColor = {};	// LIGHT: Ke
//#endif

	// BLOCK4:
	bool		hasColorMap=false;		// 66
	int			lightType=1;			// Future
	bool		isDay = false;
	float		shaderTypeParameter = 0;

	// BLOCK5:
	bool		hasAlfaColor=false;
	float		alfaColor=1;
	float		PSfade = 1;			// Fade from 0 to 1
#if defined INTRO_DEMO
	float		frameTime=0;			// For animations
#endif

	// BLOCK6:
	BOOL		hasFog=FALSE;
	BOOL		isSky = false;
	BOOL		hasAlfaMap = FALSE;		// 43
	BOOL		hasNormMap = FALSE;

	// BLOCK7:
	// cameraPosition (AUTO)
	BOOL		castShadow = FALSE;
//#ifdef WINDOWS_PLATFORM
	XMFLOAT3	specularColor = {};	// 34:
////#else
//	Vector3		specularColor = {};	// 34:
//#endif
	float		nShininess=0;		// 34:



	bool VS_USE_WVP = true; //by default use already multiplied matrix*matrix*matrix
		
	// --------------------------------------------------------------------------------------------
	//PS
#ifdef WINDOWS_PLATFORM
	XMFLOAT2 distortion1 = {}, distortion2 = {}, distortion3 = {};
#else
	float distortion1[2] = {}, distortion2[2] = {}, distortion3[2] = {};
#endif
	float distortionScale=0, distortionBias = 0;



	// --------------------------------------------------------------------------------------------

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
	VkShaderModuleCreateInfo createInfo{};
	VkBufferCreateInfo bufferInfo = {};
	VkMemoryAllocateInfo allocInfo = {};
	VkBufferCreateInfo psBufferInfo = {};
	VkMemoryAllocateInfo psAllocInfo = {};
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	//VkPipelineViewportStateCreateInfo viewportCreateInfo = {};
	VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo = {};
	VkPipelineMultisampleStateCreateInfo multisampleCreateInfo = {};
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};

	VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {};
	//VkPipelineColorBlendStateCreateInfo opaqueColorBlendState = colorBlendCreateInfo;

	//VkPipelineColorBlendStateCreateInfo transparentColorBlendState = colorBlendCreateInfo;

	VkDescriptorSetLayoutBinding layoutBinding = {};
	VkDescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo = {};
	VkPipelineDepthStencilStateCreateInfo depthStencilState{};
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};

	VkDescriptorSetLayoutBinding bindings_VS_SAMPLER[2]{};

	VkPipelineColorBlendStateCreateInfo transparentColorBlendState;
	VkPipelineColorBlendAttachmentState transparentBlendAttachment = {};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	VkVertexInputBindingDescription bindingDescription = {};

	VkVertexInputAttributeDescription attributeDescriptions_VS_SAMPLER[4] = {}; //LEVEL40

	VkVertexInputAttributeDescription attributeDescriptions_VS_PS_SAMPLER[4] = {}; //LEVEL40

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES
		VkVertexInputBindingDescription bindingInstanceDescriptions[2] = {};
#endif

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	VkDescriptorSetLayoutCreateInfo layoutInfo{};

	VkDescriptorBufferInfo descriptorBufferInfo = {};

	VkDescriptorBufferInfo descriptorBufferInfoVS = {};
	VkDescriptorBufferInfo descriptorBufferInfoPS = {};

	VkWriteDescriptorSet writeDescriptorSet = {};

	VkWriteDescriptorSet writeDescriptorSets[2] = {};

	VkWriteDescriptorSet writeDescriptorSets34[3] = {};


};

#include "lightClass.h"	

#endif
