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

	// COMMON:
	// ----------------------------------------------------------------------
	STRING MODEL_NAME;
	INT    m_ObjId = 0;

	mat4 WV;
	mat4 WVP;

	VkShaderModule	m_vertexShaderModule = VK_NULL_HANDLE;
	VkShaderModule	m_fragmentShaderModule = VK_NULL_HANDLE;

	#define WOMA_NUM_RENDERPASS_PIPELINES 1


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
	};
	vkUniformBufferObject ubo;

	//
	// VS SHADER_TEXTURE_LIGHT_INSTANCED
	//


//
// VS 23..25
//

//
// PS 26..35
//
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

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	VkVertexInputBindingDescription bindingDescription = {};

	VkVertexInputAttributeDescription attributeDescriptions_VS_SAMPLER[2] = {}; //LEVEL22

	VkVertexInputAttributeDescription attributeDescriptions_VS_PS_SAMPLER[3] = {}; //LEVEL23

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	VkDescriptorSetLayoutCreateInfo layoutInfo{};

	VkDescriptorBufferInfo descriptorBufferInfo = {};

	VkWriteDescriptorSet writeDescriptorSet = {};

	VkWriteDescriptorSet writeDescriptorSets[2] = {};


};

#endif
