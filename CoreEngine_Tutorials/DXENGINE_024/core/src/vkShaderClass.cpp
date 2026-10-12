// --------------------------------------------------------------------------------------------
// Filename: vkShaderClass.cpp
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

// For Linux:	sudo apt update && sudo apt install vulkan-tools libvulkan-dev
//				sudo apt update && sudo apt install vulkan-validationlayers vulkan-validationlayers-dev

#include "platform.h"

#if CORE_ENGINE_LEVEL >= 10 && defined VULKAN
#include "OSengine.h"

namespace WOMA
{
	extern TCHAR* getCurrentDir();
}

#if defined WINDOWS_PLATFORM
#include <DirectXMath.h>
using namespace DirectX;
#else
#include "GLmathClass.h"
#endif

#include "vkShaderClass.h"
#if defined VULKAN
#include "vkModelClass.h"
#endif

#include "fileLoader.h"

#include "virtualCameraClass.h"

vkShaderClass::vkShaderClass()
{
	CLASSLOADER();

	// VERTEX CBUFFER:
	// --------------------------------------------------------------------------------------------
	// BLOCK: VS1
	//-----------
	//	worldMatrix;	//worldMatrix
	//	view;			//view
	//	projection;		//projection
	//  WV;				//worldMatrix+viewMatrix
	//  WVP;			//worldMatrix+viewMatrix+projectionMatrix

	// BLOCK2: VS2
	//------------
	hasLight = false;
	hasSpecular = false;
	hasNormMap = false;
	hasFog = false;

	// BLOCK: VS3
	//-----------
	//lightDirection;	// Not Initialized
	ambientColor = XMFLOAT4(0, 0, 0, 0);;	// LIGHT: Ka
	diffuseColor = XMFLOAT4(1, 1, 1, 0);;	// LIGHT: Kd
	emissiveColor = XMFLOAT4(0, 0, 0, 0);;	// LIGHT: Ke

	// BLOCK4:
	//-----------
	castShadow = false;

	// PIXEL CBUFFER:
	// --------------------------------------------------------------------------------------------
	// BLOCK: PS1

	// BLOCK1:
	pixelColor = XMFLOAT4(0, 0, 0, 0);

	// BLOCK2:
	hasTexture = false;
	//hasLight = false;		//repeated in VS
	//hasSpecular = false;	//repeated in VS
	isFontShader = false;

	// BLOCK3:
	//XMFLOAT4	ambientColor;	// LIGHT: Ka //repeated in VS
	//XMFLOAT4	diffuseColor;	// LIGHT: Kd //repeated in VS
	//XMFLOAT4	emissiveColor;	// LIGHT: Ke //repeated in VS
	//XMVECTOR	lightDirection;	// LIGHT	 //repeated in VS

	// BLOCK4:
	hasColorMap = false;		// 66
	lightType = 1;				// 29
	isDay = false;
	shaderTypeParameter = 0;	// Future

	// BLOCK5:
	hasAlfaColor = false;
	alfaColor = 1;
	PSfade = 1;		// Time since Beg.
#if defined INTRO_DEMO
	frameTime = 0;	//29: Fadeout / Fadein
#endif

	// BLOCK6:
	//BOOL		hasFog;		//repeated in VS
	isSky = false;
	hasAlfaMap = false;
	//BOOL		hasNormMap;	//repeated in VS

	// BLOCK7:
	//XMFLOAT3	cameraPosition;	//TODO: soon
	//BOOL		castShadow;		//repeated in VS
	specularColor = XMFLOAT3(0, 0, 0);
	nShininess = 0;

		// --------------------------------------------------------------------------------------------

	bUseGS = false;	// GS
	// --------------------------------------------------------------------------------------------
}

vkShaderClass::~vkShaderClass() 
{
	//Shutdown();
	CLASSDELETE(); 
}

void vkShaderClass::Shutdown()
{
	ShutdownShader();				// Shutdown the vertex and pixel shaders as well as the related objects.
}

bool vkShaderClass::Initialize(INT Id, TCHAR* objectName, SHADER_TYPE shaderType, void* device, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology, UINT monitor, bool useGS)
{
	bool result = false;

	vkDriver = (vkClass*)m_Driver;

	m_ObjId = Id;
	m_shaderType = shaderType;
	MODEL_NAME = objectName;
	bUseGS = useGS;

	hasTexture = (shaderType != SHADER_COLOR && 
				  shaderType != SHADER_TEXTURE_LIGHT_SAVESHADOW)
				  ;

	if (shaderType == SHADER_COLOR
		|| shaderType == SHADER_TEXTURE
		)
	{
		{
			singleDescriptorSubset = true;						// Single UBO: VS
			AllocateSubsetDescriptors(1, vkDriver->m_device);
		}
	}
	else
	{
		singleDescriptorSubset = false;						// Multi UBO: VS / PS
	}

	return InitializeShader(shaderType, device, hwnd, PrimitiveTopology, monitor);
}


void vkShaderClass::createDescriptorPool()
{
	m_device = vkDriver->m_device;
	VkDescriptorPoolCreateInfo createInfo = {};
	VkDescriptorPoolSize typeCount = {};

	// Shared by SHADER_TEXTURE, SHADER_TEXTURE_LIGHT and SHADER_TEXTURE_FONT
	VkDescriptorPoolSize poolSizes[2] = {};

	switch (m_shaderType)
	{
		// ------------------------------------------------------------------
		// VS CONSTANT BUFFER
		// ------------------------------------------------------------------
		// layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
		case SHADER_COLOR:										//21
		{
			// Level 21: only UBO
			typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			typeCount.descriptorCount = MAX_FRAMES_IN_FLIGHT;

			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.poolSizeCount = 1;
			createInfo.pPoolSizes = &typeCount;
			createInfo.maxSets = 64;
			break;
		}

		// ------------------------------------------------------------------
		// VS CONSTANT BUFFER
		// ------------------------------------------------------------------
		// layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
		// PS:
		// layout(set = 0, binding = 1) uniform sampler2D shaderTexture;
		case SHADER_TEXTURE:
		{
			poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSizes[0].descriptorCount = 64;

			poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSizes[1].descriptorCount = 64;

			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.poolSizeCount = 2;
			createInfo.pPoolSizes = poolSizes;
			createInfo.maxSets = 64;
			break;
		}

		//0 VS UBO
		//  layout(binding = 1) uniform sampler2D shaderTexture;
		//2 PS UBO

		case SHADER_TEXTURE_LIGHT:
		{
			// -------------------------------------------------------------------------------------------- 
			// TEXTURE DESCRIPTORS
			// --------------------------------------------------------------------------------------------
			//				   0  VS UBO
			//layout(binding = 1) uniform sampler2D shaderTexture;
			//				   2  PS UBO

			poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSizes[0].descriptorCount = 128; // 2*64 UBOs per set

			poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSizes[1].descriptorCount = 64;  // 1 x texture

			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.poolSizeCount = 2;
			createInfo.pPoolSizes = poolSizes;
			createInfo.maxSets = 64;
			break;
		}

		default:
			womalog("Shader not supported yet (shaderType=%d)\n", m_shaderType);
			womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
			ASSERT(false);
	}

	if (vkCreateDescriptorPool(m_device, &createInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		womalog("Failed to create descriptor pool\n");
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false);
	}
#if defined _DEBUG
	else {
		womalog("Created descriptor pool\n");
	}
#endif
}

bool vkShaderClass::InitializeShader(SHADER_TYPE shaderType, void* device, HWND hwnd, PRIMITIVE_TOPOLOGY PrimitiveTopology, UINT monitor)
{
	bool result = false;


	VkResult vkResult;

	VkDevice DriverDevice = (VkDevice)device;

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	womalog("1 VERTEX INPUT\n");

	switch (shaderType)
	{
	case SHADER_COLOR:
	{
		// --------------------------------------------------------------------------------------------
		// VERTEX INPUT
		// --------------------------------------------------------------------------------------------
		//layout(location = 0) in vec3 inPosition;
		//layout(location = 1) in vec4 inColor;

		// 1 Describe Vertex Input:
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(ModelColorVertexType);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		// POSITION
		attributeDescriptions_VS_SAMPLER[0].binding = 0;
		attributeDescriptions_VS_SAMPLER[0].location = 0;
		attributeDescriptions_VS_SAMPLER[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions_VS_SAMPLER[0].offset = offsetof(ModelColorVertexType, x);

		// COLOR
		attributeDescriptions_VS_SAMPLER[1].binding = 0;
		attributeDescriptions_VS_SAMPLER[1].location = 1;
		attributeDescriptions_VS_SAMPLER[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions_VS_SAMPLER[1].offset = offsetof(ModelColorVertexType, r);

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

		vertexInputInfo.vertexAttributeDescriptionCount = 2;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions_VS_SAMPLER;
		break;
	}

	case SHADER_TEXTURE:
	{
		// --------------------------------------------------------------------------------------------
		// VERTEX INPUT
		// --------------------------------------------------------------------------------------------
		//layout(location = 0) in vec3 inPosition;
		//layout(location = 1) in vec2 inTexCoord;
		
		// 1 Describe Vertex Input
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(ModelTextureVertexType);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		// POSITION
		attributeDescriptions_VS_SAMPLER[0].binding = 0;
		attributeDescriptions_VS_SAMPLER[0].location = 0;
		attributeDescriptions_VS_SAMPLER[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions_VS_SAMPLER[0].offset = offsetof(ModelTextureVertexType, x);

		// TEXCOORD
		attributeDescriptions_VS_SAMPLER[1].binding = 0;
		attributeDescriptions_VS_SAMPLER[1].location = 1;
		attributeDescriptions_VS_SAMPLER[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions_VS_SAMPLER[1].offset = offsetof(ModelTextureVertexType, tu);

		//vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

		vertexInputInfo.vertexAttributeDescriptionCount = 2;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions_VS_SAMPLER;

		break;
	}

	case SHADER_TEXTURE_LIGHT:				//23
	case SHADER_TEXTURE_LIGHT_ALPHAMAP:		//34
	{
		// --------------------------------------------------------------------------------------------
		// VERTEX INPUT
		// --------------------------------------------------------------------------------------------
		//layout(location = 0) in vec3 inPosition;
		//layout(location = 1) in vec2 inTexCoords;
		//layout(location = 2) in vec3 inNormal;

		// 1. Describe Vertex Input
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(ModelTextureLightVertexType);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		// POSITION
		attributeDescriptions_VS_PS_SAMPLER[0].binding = 0;
		attributeDescriptions_VS_PS_SAMPLER[0].location = 0;
		attributeDescriptions_VS_PS_SAMPLER[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions_VS_PS_SAMPLER[0].offset = offsetof(ModelTextureLightVertexType, x);

		// TEXCOORD
		attributeDescriptions_VS_PS_SAMPLER[1].binding = 0;
		attributeDescriptions_VS_PS_SAMPLER[1].location = 1;
		attributeDescriptions_VS_PS_SAMPLER[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions_VS_PS_SAMPLER[1].offset = offsetof(ModelTextureLightVertexType, tu);

		// NORMAL
		attributeDescriptions_VS_PS_SAMPLER[2].binding = 0;
		attributeDescriptions_VS_PS_SAMPLER[2].location = 2;
		attributeDescriptions_VS_PS_SAMPLER[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions_VS_PS_SAMPLER[2].offset = offsetof(ModelTextureLightVertexType, nx);

		//vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

		vertexInputInfo.vertexAttributeDescriptionCount = 3;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions_VS_PS_SAMPLER;

		break;
	}

	default:
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false); //Shader not supported yet.
		return false;
	}

	womalog("2 Load shader code: SPIR-V (DEFINE SHADER CODE:)\n");
	womalog("getCurrentDir(): %s\n", WOMA::getCurrentDir());

	//
	// --> 2 Load shader code: SPIR-V (DEFINE SHADER CODE:)
	//
	switch (shaderType)
	{
	case SHADER_COLOR:
	{
	#if defined LINUX_PLATFORM
		vsFilename.append(TEXT("BIN/021color.vert.spv"));
		psFilename.append(TEXT("BIN/021color.frag.spv"));
	#else
		vsFilename.append(TEXT("VK/BIN/021color.vert.spv")); //21
		psFilename.append(TEXT("VK/BIN/021color.frag.spv")); //21
	#endif
		break;
	}
	case SHADER_TEXTURE:
#if defined LINUX_PLATFORM
		vsFilename.append(TEXT("BIN/022texture.vert.spv"));
		psFilename.append(TEXT("BIN/022texture.frag.spv"));
#else
		vsFilename.append(TEXT("VK/BIN/022texture.vert.spv")); //22
		{
		psFilename.append(TEXT("VK/BIN/022texture.frag.spv")); //22
		}
#endif
		break;

	case SHADER_TEXTURE_LIGHT:
#if defined LINUX_PLATFORM
		vsFilename.append(TEXT("BIN/023light.vert.spv"));
		psFilename.append(TEXT("BIN/023light.frag.spv"));
#else
		vsFilename.append(TEXT("VK/BIN/023light.vert.spv")); //23
		psFilename.append(TEXT("VK/BIN/023light.frag.spv")); //23
#endif
		break;

	default:
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false); //Shader not supported yet.
		return false;
	}

	if (!LoadShaderBinaryFile(WOMA::LoadFile((TCHAR*)vsFilename.c_str(), true), vertexCode))
	{
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false);
		return false;
	}

	if (!LoadShaderBinaryFile(WOMA::LoadFile((TCHAR*)psFilename.c_str(), true), fragmentCode))
	{
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false);
		return false;
	}

	womalog("3 VkShaderModule: m_vertexShaderModule\n");

	//
	// --> 3 VkShaderModule: m_vertexShaderModule
	//
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = vertexCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(vertexCode.data());

	vkResult = vkCreateShaderModule(DriverDevice, &createInfo, nullptr, &m_vertexShaderModule);
	ASSERT(vkResult == VK_SUCCESS);

	womalog("4 VkShaderModule: m_fragmentShaderModule\n");
	//
	// --> 4 VkShaderModule: m_fragmentShaderModule
	//
	createInfo.codeSize = fragmentCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(fragmentCode.data());

	vkResult = vkCreateShaderModule(DriverDevice, &createInfo, nullptr, &m_fragmentShaderModule);
	ASSERT(vkResult == VK_SUCCESS);

	womalog("5 Descriptor Set Layout\n");
	//
	// --> 5 Descriptor Set Layout
	//

	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

	switch (shaderType)
	{
	case SHADER_COLOR:	//21
	{
		//VS: layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
		//{
		//	mat4 WVP;
		//} cb;

		VkDescriptorSetLayoutBinding bindings_VS_PS_SAMPLER[1]{};

		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1; // One Constant Buffer VS
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		vkResult = vkCreateDescriptorSetLayout(DriverDevice, &layoutInfo, nullptr, &m_descriptorColorSetLayout);
		ASSERT(vkResult == VK_SUCCESS);

		break;
	}

	case SHADER_TEXTURE:	//22
	{
		//VS: layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
		//{
		//	mat4 WVP;
		//} cb;
		
		//PS: layout(set = 0, binding = 1) uniform sampler2D shaderTexture;

		VkDescriptorSetLayoutBinding bindings_VS_PS_SAMPLER[2]{};
		//
		// Binding 0: Uniform Buffer
		//
		bindings_VS_SAMPLER[0].binding = 0;
		bindings_VS_SAMPLER[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		bindings_VS_SAMPLER[0].descriptorCount = 1; // One Constant Buffer VS
		bindings_VS_SAMPLER[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindings_VS_SAMPLER[0].pImmutableSamplers = nullptr;

		//
		// Binding 1: Texture + Sampler
		//
		bindings_VS_SAMPLER[1].binding = 1;
		bindings_VS_SAMPLER[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		bindings_VS_SAMPLER[1].descriptorCount = 1; // 1 Sampler
		bindings_VS_SAMPLER[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings_VS_SAMPLER[1].pImmutableSamplers = nullptr;

		layoutInfo.bindingCount = 2;
		layoutInfo.pBindings = bindings_VS_SAMPLER;

		vkResult = vkCreateDescriptorSetLayout(DriverDevice, &layoutInfo, nullptr, &m_descriptorTextureSetLayout);
		ASSERT(vkResult == VK_SUCCESS);

		break;
	}

	case SHADER_TEXTURE_LIGHT:	//23
	{
		//VS: layout(set = 0, binding = 0) uniform VSShaderParametersBuffer
		//PS: layout(set = 0, binding = 1) uniform sampler2D shaderTexture;
		//PS: layout(set = 0, binding = 2) uniform PSShaderParametersBuffer;
		//PS:	layout(push_constant) uniform PushConstants

		VkDescriptorSetLayoutBinding bindings_VS_PS_SAMPLER[3]{};
		//
		// Binding 0: VS Uniform Buffer
		//
		bindings_VS_PS_SAMPLER[0].binding = 0;
		bindings_VS_PS_SAMPLER[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		bindings_VS_PS_SAMPLER[0].descriptorCount = 1; // One Constant Buffer VS
		bindings_VS_PS_SAMPLER[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindings_VS_PS_SAMPLER[0].pImmutableSamplers = nullptr;

		//
		// Binding 1: Texture + Sampler
		//
		bindings_VS_PS_SAMPLER[1].binding = 1;
		bindings_VS_PS_SAMPLER[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		bindings_VS_PS_SAMPLER[1].descriptorCount = 1; // 1 Sampler
		bindings_VS_PS_SAMPLER[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings_VS_PS_SAMPLER[1].pImmutableSamplers = nullptr;

		//
		// Binding 2: PS Uniform Buffer
		//
		bindings_VS_PS_SAMPLER[2].binding = 2;
		bindings_VS_PS_SAMPLER[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		bindings_VS_PS_SAMPLER[2].descriptorCount = 1; // One Constant Buffer PS
		bindings_VS_PS_SAMPLER[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings_VS_PS_SAMPLER[2].pImmutableSamplers = nullptr;
		
		layoutInfo.bindingCount = 3;
		
		layoutInfo.pBindings = bindings_VS_PS_SAMPLER;

		//if (shaderType == SHADER_TEXTURE_LIGHT)
		//	layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

		vkResult = vkCreateDescriptorSetLayout(DriverDevice, &layoutInfo, nullptr, &m_descriptorTextureLightSetLayout);
		ASSERT(vkResult == VK_SUCCESS);

		break;
	}

	default:
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false); //Shader not supported yet.
	}

	womalog("VS Constant Buffer:\n");

	//
	// VS Constant Buffer:
	//

	//VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	switch (shaderType)
	{
		case SHADER_COLOR:
		case SHADER_TEXTURE:
		case SHADER_TEXTURE_LIGHT_SAVESHADOW:
		{
			bufferInfo.size = sizeof(ubo);
			break;
		}

		default:
		{
			bufferInfo.size = sizeof(dataVSptr);
			break;
		}
	}

	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	vkCreateBuffer(DriverDevice, &bufferInfo, nullptr, &m_uniformBufferVS);

	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(DriverDevice, m_uniformBufferVS, &memReqs);

	//VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memReqs.size;
	allocInfo.memoryTypeIndex = vkDriver->FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkAllocateMemory(DriverDevice, &allocInfo, nullptr, &m_uniformBufferVSMemory);

	vkBindBufferMemory(DriverDevice, m_uniformBufferVS, m_uniformBufferVSMemory, 0);

	womalog("PS Constant Buffer:\n");
	//
	// PS Constant Buffer:
	//

	if (shaderType >= SHADER_TEXTURE_FONT && shaderType != SHADER_TEXTURE_LIGHT_SAVESHADOW)
	{
		//VkBufferCreateInfo psBufferInfo = {};
		psBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		psBufferInfo.size = sizeof(dataPSptr);
		psBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		vkCreateBuffer(DriverDevice, &psBufferInfo, nullptr, &m_uniformBufferPS);

		VkMemoryRequirements psMemReqs;
		vkGetBufferMemoryRequirements(DriverDevice, m_uniformBufferPS, &psMemReqs);

		//VkMemoryAllocateInfo psAllocInfo = {};
		psAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		psAllocInfo.allocationSize = psMemReqs.size;

		psAllocInfo.memoryTypeIndex =vkDriver->FindMemoryType( psMemReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		vkAllocateMemory(DriverDevice, &psAllocInfo, nullptr, &m_uniformBufferPSMemory);

		vkBindBufferMemory(DriverDevice, m_uniformBufferPS, m_uniformBufferPSMemory, 0);
	}


	// --------------------------------------------------------------------------------------------------------------------------------------------

	// 1. Pipeline Layout
	// 2. m_pipeline
	// 
	// 4. Input Assembly
	// 5. Viewport / Scissor
	// 6. Rasterizer
	// 7. Multisampling
	// 8. Color blending
	// 9. Depth / Stencil

	// 10. VkGraphicsPipelineCreateInfo

	womalog("------------------------------------------------------------------------------------------------------------------------------\n");
	womalog("1. Pipeline Layout\n");

	//
	// --> 1. Pipeline Layout
	//

	//VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	
	switch (shaderType)
	{
	case SHADER_COLOR:
		pipelineLayoutInfo.pSetLayouts = &m_descriptorColorSetLayout;
		break;

	case SHADER_TEXTURE:
		pipelineLayoutInfo.pSetLayouts = &m_descriptorTextureSetLayout;
		break;

	case SHADER_TEXTURE_LIGHT:
		pipelineLayoutInfo.pSetLayouts = &m_descriptorTextureLightSetLayout;
		break;

	default:
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false); //Shader not supported yet.
	}

	vkResult = vkCreatePipelineLayout(DriverDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	ASSERT(vkResult == VK_SUCCESS);

	womalog("createGraphicsPipeline()\n");
	//
	//createGraphicsPipeline();
	//

	// Set up shader stage info
	//VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderCreateInfo.module = m_vertexShaderModule;
	vertexShaderCreateInfo.pName = "main";

	//VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderCreateInfo.module = m_fragmentShaderModule;
	fragmentShaderCreateInfo.pName = "main";

	// Describe input assembly
	
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	switch (PrimitiveTopology)
	{
	case POINTLIST:
		inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		break;

	case LINELIST:
		inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		break;

	case LINESTRIP:
		inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		break;

	case TRIANGLELIST:
		inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		break;

	case TRIANGLESTRIP:
		inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		break;

	default:
		womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
		ASSERT(false);
		break;
	}

	womalog("Describe rasterization\n");

	


	// Describe rasterization
	// Note: depth bias and using polygon modes other than fill require changes to logical device creation (device features)
	//VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo = {};
	rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationCreateInfo.cullMode = VK_CULL_MODE_NONE; //VK_CULL_MODE_BACK_BIT;
	rasterizationCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizationCreateInfo.depthBiasClamp = 0.0f;
	rasterizationCreateInfo.depthBiasSlopeFactor = 0.0f;
	rasterizationCreateInfo.lineWidth = 1.0f;

	// Describe multisampling
	// Note: using multisampling also requires turning on device features
	//VkPipelineMultisampleStateCreateInfo multisampleCreateInfo = {};
	multisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleCreateInfo.minSampleShading = 1.0f;
	multisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleCreateInfo.alphaToOneEnable = VK_FALSE;

	// Describing color blending
	// Note: all paramaters except blendEnable and colorWriteMask are irrelevant here
	//VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
	colorBlendAttachmentState.blendEnable = VK_FALSE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	// Note: all attachments must have the same values unless a device feature is enabled
	//VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {};

	colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendCreateInfo.attachmentCount = 1;
	colorBlendCreateInfo.pAttachments = &colorBlendAttachmentState;
	colorBlendCreateInfo.blendConstants[0] = 0.0f;
	colorBlendCreateInfo.blendConstants[1] = 0.0f;
	colorBlendCreateInfo.blendConstants[2] = 0.0f;
	colorBlendCreateInfo.blendConstants[3] = 0.0f;

	VkPipelineColorBlendStateCreateInfo opaqueColorBlendState = colorBlendCreateInfo;

	transparentColorBlendState = colorBlendCreateInfo;
	transparentColorBlendState.pAttachments = &transparentBlendAttachment;

	transparentBlendAttachment.blendEnable = VK_TRUE;
#if defined INTRO_DEMO || DX_ENGINE_LEVEL >= 70
	transparentBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
#else
	transparentBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR; // VK_BLEND_FACTOR_SRC_ALPHA;
#endif
	transparentBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	transparentBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
#ifdef INTRO_DEMO
	transparentBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
#else
	transparentBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
#endif
	transparentBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	transparentBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	transparentBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;

	womalog("depthStencilState\n");
	//
	// depthStencilState
	//
	//VkPipelineDepthStencilStateCreateInfo depthStencilState{};
	depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

	depthStencilState.depthTestEnable = VK_TRUE;
	depthStencilState.depthWriteEnable = VK_TRUE;
	depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilState.depthBoundsTestEnable = VK_FALSE;
	depthStencilState.stencilTestEnable = VK_FALSE;


	womalog("createDescriptorPool();\n");
	//
	// createDescriptorPool
	//
	createDescriptorPool();

	womalog("createDescriptorSet();\n");
	//
	// createDescriptorSet
	//
	createDescriptorSet(DriverDevice);

	return true;
}


bool vkShaderClass::createDescriptorSet(VkDevice DriverDevice)
{
	SHADER_TYPE shaderType = m_shaderType;

	//womalog("createDescriptorSet() - PART I;\n");

	{
		// There needs to be one descriptor set per binding point in the shader
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_descriptorPool;
		allocInfo.descriptorSetCount = 1;

		// Update descriptor set with uniform binding

		descriptorBufferInfo.buffer = m_uniformBufferVS;
		descriptorBufferInfo.offset = 0;

		VkDescriptorSetLayout m_descriptor_SetLayout = VK_NULL_HANDLE;
		switch (shaderType)
		{
		case SHADER_COLOR:
			descriptorBufferInfo.range = sizeof(ubo);
			allocInfo.pSetLayouts = &m_descriptorColorSetLayout;
			m_descriptor_SetLayout = m_descriptorColorSetLayout;
			break;

		case SHADER_TEXTURE:

			descriptorBufferInfo.range = sizeof(ubo);
			allocInfo.pSetLayouts = &m_descriptorTextureSetLayout;
			m_descriptor_SetLayout = m_descriptorTextureSetLayout;
			break;

		case SHADER_TEXTURE_LIGHT:
		{
			descriptorBufferInfoVS.buffer = m_uniformBufferVS;
			descriptorBufferInfoVS.offset = 0;
			descriptorBufferInfoVS.range = sizeof(dataVSptr);

			descriptorBufferInfoPS.buffer = m_uniformBufferPS;
			descriptorBufferInfoPS.offset = 0;
			descriptorBufferInfoPS.range = sizeof(dataPSptr);

			allocInfo.pSetLayouts = &m_descriptorTextureLightSetLayout;
			m_descriptor_SetLayout = m_descriptorTextureLightSetLayout;
			break;
		}

		default:
			womalog("DescriptorSets: FATAL ERROR");
			womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
			ASSERT(false); //Shader not supported yet.
			return false;
		}

		//womalog("createDescriptorSet() - PART II;\n");
		if (singleDescriptorSubset)
		{
			VkDescriptorSetLayout layouts[MAX_FRAMES_IN_FLIGHT];
			for (UINT i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				layouts[i] = m_descriptor_SetLayout;

			allocInfo.pSetLayouts = layouts;
			allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
			VkResult res= VK_SUCCESS;
			for (UINT i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				if (m_descriptorSet30[i] == NULL)
					res = vkAllocateDescriptorSets(DriverDevice, &allocInfo, &m_descriptorSet30[i]); //monitor
				if (res != VK_SUCCESS)
				{
					womalog("DescriptorSets: FATAL ERROR");
					womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
					ASSERT(false);
				}
			}
		}
		else
		{
			//UINT subsetCount = singleDescriptorSubset ? descriptorSubsetCount : 1;
			UINT subsetCount = MAX(descriptorSubsetCount,1);

			VkDescriptorSetLayout layout = m_descriptor_SetLayout;
			allocInfo.pSetLayouts = &layout;
			allocInfo.descriptorSetCount = 1;

			for (UINT frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++)
			{
				m_descriptorSet[frame].resize(subsetCount);

				VkResult res = VK_SUCCESS;
				for (UINT subset = 0; subset < subsetCount; subset++)
				{
					if (m_descriptorSet[frame][subset] == NULL)
						res = vkAllocateDescriptorSets( DriverDevice, &allocInfo, &m_descriptorSet[frame][subset]);

					if (res != VK_SUCCESS)
					{
						womalog("DescriptorSets: FATAL ERROR (frame=%u subset=%u)", frame, subset);
						womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
						ASSERT(false);
					}
				}
			}

			descriptorSubsetCount = subsetCount;
		}

		//womalog("createDescriptorSet() - PART III;\n");
		switch (shaderType)
		{
		case SHADER_COLOR:
		case SHADER_TEXTURE:
		{
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.dstBinding = 0;
			writeDescriptorSet.descriptorCount = 1;
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;

			if (singleDescriptorSubset)
			{
				for (UINT i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				{
					writeDescriptorSet.dstSet = m_descriptorSet30[i]; //frame
					vkUpdateDescriptorSets(DriverDevice, 1, &writeDescriptorSet, 0, nullptr);
				}
			}
			else
			{
				for (UINT t = 0; t < descriptorSubsetCount; t++)
				{
					for (UINT i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
					{
						writeDescriptorSet.dstSet = GetDescriptorSet(i, t);
						vkUpdateDescriptorSets(DriverDevice, 1, &writeDescriptorSet, 0, nullptr);
					}
				}
			}
			break;
		}

		case SHADER_TEXTURE_LIGHT:
		{
			// -------
			// VS
			// -------
			writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSets[0].dstBinding = 0;
			writeDescriptorSets[0].descriptorCount = 1;
			writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeDescriptorSets[0].pBufferInfo = &descriptorBufferInfoVS;

			// -------
			// PS
			// -------
			writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSets[1].dstBinding = 2;
			writeDescriptorSets[1].descriptorCount = 1;
			writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeDescriptorSets[1].pBufferInfo = &descriptorBufferInfoPS;

			for (UINT t = 0; t < descriptorSubsetCount; t++)
			{
				for (UINT i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				{
					writeDescriptorSets[0].dstSet = GetDescriptorSet(i, t);
					writeDescriptorSets[1].dstSet = GetDescriptorSet(i, t);

					vkUpdateDescriptorSets(DriverDevice, 2, writeDescriptorSets, 0, nullptr);
				}
			}

			break;
		}


		default:
			womalog("FATAL ERROR: %s %s() %d\n", __FILE__, __FUNCTION__, __LINE__);
			ASSERT(false);
			return false;
		}
	}

	womalog("createDescriptorSet() - END;\n");

	return true;
}


bool vkShaderClass::LoadShaderBinaryFile(TCHAR* filename, std::vector<char>& code)
{

	womalog("vkShaderClass::LoadShaderBinaryFile() - LOADING FILE: %s\n", filename);

#if defined ANDROID_PLATFORM
	File file;

	if (!file.open(filename, TEXT("rb")))
	{
		womalog(TEXT("ERROR: Could not open Vulkan shader asset: %s\n"), filename);
		return false;
	}

	size_t fileSize = static_cast<size_t>(AAsset_getLength(file._A));

	code.resize(fileSize);

	if (file.read(code.data(), fileSize) != fileSize)
	{
		FCLOSE(file);
		return false;
	}

	FCLOSE(file);

	return true;

#else

	IFSTREAM file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		womalog(TEXT("ERROR: Could not open Vulkan shader file: %s\n"), filename);
		return false;
	}

	size_t fileSize = static_cast<size_t>(file.tellg());

	code.resize(fileSize);

	file.seekg(0);

	file.read(code.data(), fileSize);

	file.close();

	return true;

#endif
}



#define DEBUG_SETSHADERPARAMETERS

bool vkShaderClass::SetShaderParameters(
	UINT pass,
	void* Device_Context,
	mat4* worldMatrix,
	mat4* viewMatrix,
	mat4* projectionMatrix,
	mat4* lightViewMatrix, mat4* ShadowProjectionMatrix, float m_particleAlpha)

{
	VkDevice DriverDevice = vkDriver->m_device;
	VkResult result = VK_SUCCESS;
	void* dataVS = nullptr;

	LightClass* light = SystemHandle->m_Application->app_Light;

	// --------------------------------------------------------------------
	// Matrices
	// --------------------------------------------------------------------
	WV = (*worldMatrix) * (*viewMatrix);
	WVP = WV * (*projectionMatrix);

	void* data=NULL;

	if (m_shaderType <= SHADER_TEXTURE || pass == 2) // SHADER_COLOR + SHADER_TEXTURE
	{
		ubo.WVP = WVP;
		result = vkMapMemory(DriverDevice, m_uniformBufferVSMemory, 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(DriverDevice, m_uniformBufferVSMemory);

		goto END;
	}
	 
	if (m_shaderType >= SHADER_TEXTURE_FONT && m_shaderType != SHADER_TEXTURE_LIGHT_SAVESHADOW)
	{
		//------------------------------------------
		// VS
		// BLOCK VS1: 64 bytes
		//------------------------------------------
		dataVSptr.world = *worldMatrix;
		dataVSptr.view = *viewMatrix;
		dataVSptr.projection = *projectionMatrix;
		dataVSptr.WV = WV;
		dataVSptr.WVP = WVP;

		// BLOCK VS2: 16 bytes
		dataVSptr.VShasLight = hasLight;
		dataVSptr.VShasSpecular = hasSpecular;
		dataVSptr.VShasNormMap = hasNormMap;
		dataVSptr.VShasFog = hasFog;

		//------------------------------------------
		// BLOCK VS3: 16 bytes
	// BLOCK: VS3
		if (hasLight)
		{
			dataVSptr.VSlightDirection = light->m_lightDirection;

			if (m_shaderType < SHADER_TEXTURE_LIGHT_SAVESHADOW || m_shaderType >= SHADER_Double_Color_Terrain)
			{
				dataVSptr.VSambientColor = light->m_ambientColor;		// Use Global: Light
				dataVSptr.VSdiffuseColor = light->m_diffuseColor;
			}
			else {
				dataVSptr.VSambientColor = ambientColor;				// Use Material Light
				dataVSptr.VSdiffuseColor = diffuseColor;
			}

			dataVSptr.VSemissiveColor = emissiveColor;
		}

		//------------------------------------------
		// BLOCK VS4: 16 bytes
		//------------------------------------------
		// 36 BLOCK: VS5

	//if (m_shaderType == SHADER_TEXTURE_WATER) womalog("vk SHADER time=%f\n", watertime);

		//------------------------------------------
		// BLOCK: VS7

		// ====================================================================
		// Upload VS constant buffer
		// ====================================================================
		result = vkMapMemory(DriverDevice, m_uniformBufferVSMemory, 0, sizeof(dataVSptr), 0, &dataVS);
		ASSERT(result == VK_SUCCESS);

		memcpy(dataVS, &dataVSptr, sizeof(dataVSptr));

		vkUnmapMemory(DriverDevice, m_uniformBufferVSMemory);

		if (m_shaderType == SHADER_COLOR ||
			m_shaderType == SHADER_TEXTURE_LIGHT_FAST)
			return true;

		// --------------------------------------------------------------------
		// [ PS  ] - PSPushConstants + dataPS
		// BLOCK1 
		// --------------------------------------------------------------------
		dataPSptr.pixelColor = pixelColor;				// SET THE PIXEL COLOR IN PIXEL SHADER

		// --------------------------------------------------------------------
		// BLOCK2 - PSPushConstants
		dataPSptr.hasTexture = hasTexture;			// NO? use pixelColor
		dataPSptr.hasLight = hasLight;
		dataPSptr.hasSpecular = hasSpecular;
		dataPSptr.isFont = isFontShader;

		// --------------------------------------------------------------------
		// BLOCK3 - vkPSconstantBufferType
		if (hasLight)
		{
			dataPSptr.lightDirection = light->m_lightDirection;
			if (m_shaderType < SHADER_TEXTURE_LIGHT_SAVESHADOW || m_shaderType >= SHADER_Double_Color_Terrain)
			{
				dataPSptr.ambientColor = light->m_ambientColor; // Use Global Light
				dataPSptr.diffuseColor = light->m_diffuseColor;
			}
			else {
				dataPSptr.ambientColor = ambientColor;			// Use Material Light
				dataPSptr.diffuseColor = diffuseColor;
				dataPSptr.emissiveColor = emissiveColor;
			}
		}

		// --------------------------------------------------------------------
		// BLOCK4 - PSPushConstants

		// --------------------------------------------------------------------
		// BLOCK7:
		if(m_uniformBufferPSMemory) 
		{
			// ====================================================================
			// Upload PS constant buffer
			// ====================================================================
			void* dataPS = nullptr;

			result = vkMapMemory(DriverDevice, m_uniformBufferPSMemory, 0, sizeof(dataPSptr), 0, &dataPS);
			ASSERT(result == VK_SUCCESS);

			memcpy(dataPS, &dataPSptr, sizeof(dataPSptr));

			vkUnmapMemory(DriverDevice, m_uniformBufferPSMemory);
		}
	}

END:
#if defined _DEBUG
	if (m_Driver->RenderfirstTime)
		womalog("SetShaderParameters() Model: '%s' - SHADER: %d (%s %s)\n", (TCHAR*)MODEL_NAME.c_str(), m_shaderType, vsFilename.c_str(), psFilename.c_str());
#endif

	return true;
}
 
void vkShaderClass::SetupRenderPipe(UINT monitor)
{
	VkDevice vkDevice = vkDriver->m_device;

	vkDriver->UpdateViewportScissorState(monitor);

	// IMPORTANT:
	// shaderStages[] must remain local to this function.
	// vkCreateGraphicsPipelines() consumes it immediately.
	// Do not build it in InitializeShader(), otherwise pStages becomes invalid
	// after InitializeShader() returns.
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	//
	//VkRenderPass renderPass;
	//

	// Create the graphics pipeline
	auto& vkWindow = vkDriver->VKwindowsToRenderArray[monitor];
	//VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = 2;
	pipelineCreateInfo.pStages = shaderStages;
	pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	pipelineCreateInfo.pViewportState = &vkWindow.viewportCreateInfo;	//monitor
	pipelineCreateInfo.pRasterizationState = &rasterizationCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisampleCreateInfo;
	pipelineCreateInfo.layout = m_pipelineLayout;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;
	pipelineCreateInfo.pDepthStencilState = &depthStencilState;

#if defined USE_RASTERIZER_STATE
	for (UINT Rpass = 0; Rpass < WOMA_NUM_RENDERPASS_PIPELINES; Rpass++)
	{
		switch (Rpass)
		{
		case 0:
		{
			pipelineCreateInfo.renderPass = vkWindow.m_render_pass;
			pipelineCreateInfo.pViewportState = &vkWindow.viewportCreateInfo;
			break;
		}
		}
		for (UINT zBufferMode = 0; zBufferMode < 2; zBufferMode++)
		{
			// Z BUFFER
			depthStencilState.depthTestEnable = (zBufferMode == 1) ? VK_TRUE : VK_FALSE;

			for (UINT cullMode = 0; cullMode < 3; cullMode++)			//0..2
			{
				// CULL
				switch (cullMode)
				{
				case CULL_FRONT:
					rasterizationCreateInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
					break;

				case CULL_BACK:
					rasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
					break;

				case CULL_NONE:
					rasterizationCreateInfo.cullMode = VK_CULL_MODE_NONE;
					break;
				}

				for (UINT fillMode = 0; fillMode < 2; fillMode++)		//0..1
				{
					// FILL
					switch (fillMode)
					{
					case FILL_SOLID:
						rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
						break;

					case FILL_WIRE:
						rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_LINE;
						break;
					}

					for (UINT pipelineState = 0; pipelineState < MAXNUM_PIPELINE_STATES; pipelineState++)
					{
						// Blend OFF
						VkPipeline& pipeline = m_pipelineState[zBufferMode][cullMode][fillMode][pipelineState][Rpass];

						if (pipelineState == SOLID_PIPELINE_STATES)
						{
							pipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
						}
						else
						{
							pipelineCreateInfo.pColorBlendState = &transparentColorBlendState;
						}

						if (vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline) != VK_SUCCESS)
						{
							womalog("failed to create graphics pipeline");
							ASSERT(false);
						}

						ASSERT_DEBUG(m_pipelineState[zBufferMode][cullMode][fillMode][pipelineState][Rpass]);
					}
				}
			}
		}

	}
#endif

}

void vkShaderClass::Render(UINT pass, /*VkCommandBuffer*/void* Device_Context, int indexCount, mat4* worldMatrix, mat4* viewMatrix, mat4* projectionMatrix, UINT monitor)
{
	ASSERT_DEBUG(indexCount > 0);

	// --------------------------------------------------------------------
	// Vulkan pipelines are monitor/framebuffer dependent.
	// After resize, monitor changes or focus transitions, some pipeline
	// state combinations may not exist yet. Ensure the required pipeline
	// exists before issuing vkCmdBindPipeline().
	// --------------------------------------------------------------------
	BOOL zBuffer = m_Driver->g_Zbuffer;

	UINT cullMode = vkDriver->m_CullMode;
	UINT fillMode = vkDriver->m_fillMode;

#if defined USE_ALPHA_BLENDING
	BOOL alphaBlend = m_Driver->g_AlphaBlend;
#else
	BOOL alphaBlend = SOLID_PIPELINE_STATES;
#endif

	// Fill Shader: Constant Buffer
	SetShaderParameters(pass, Device_Context, worldMatrix, viewMatrix, projectionMatrix);
	RenderShader(pass, Device_Context, /*texture_index*/ 0, indexCount);
}

extern UINT vkShaderClass_RenderShader_Calls;

void vkShaderClass::RenderShader(UINT pass, /*VkCommandBuffer*/void* Device_Context, int texture_index, int indexCount, int start)
{
	VkCommandBuffer commandBuffer = (VkCommandBuffer)Device_Context;
	VkDevice vkDevice = vkDriver->m_device;
#if defined _DEBUG || defined LINUX_PLATFORM || defined ANDROID_PLATFORM
	vkShaderClass_RenderShader_Calls++;
#endif
	// --------------------------------------------------------------------
	// Render 3 - Set Pipeline
	// --------------------------------------------------------------------
	BOOL zBuffer = m_Driver->g_Zbuffer;

	UINT cullMode = vkDriver->m_CullMode;
	UINT fillMode = vkDriver->m_fillMode;

#if defined USE_ALPHA_BLENDING
	BOOL alphaBlend = m_Driver->g_AlphaBlend;
#else
	BOOL alphaBlend = SOLID_PIPELINE_STATES;
#endif

	//UINT passIndex = (pass == PASS_SHADOWS) ? 1 : 0;
	UINT passIndex = 0;

	switch (pass)
	{
	case PASS_SHADOWS:
		passIndex = 1;
		break;

	case PASS_MAINMAP:
		passIndex = 2;
		break;

	case PASS_MINIMAP:
		passIndex = 3;
		break;

	default:
		passIndex = 0;
		break;
	}

	if (m_pipelineState[zBuffer][cullMode][fillMode][alphaBlend][passIndex] == VK_NULL_HANDLE)
		SetupRenderPipe(0); //monitor

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineState[zBuffer][cullMode][fillMode][alphaBlend][passIndex]);

	// --------------------------------------------------------------------
	// Render 4 -  Set Descriptor Set ( Texture + UBO)
	// --------------------------------------------------------------------
	UINT frame =  vkDriver->VKwindowsToRenderArray[0].m_currentFrame; //vkDriver->m_frameIndex;  //monitor

	if (singleDescriptorSubset)
	{
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet30[frame], 0, nullptr); //monitor
	}
	else
	{
		VkDescriptorSet set = GetDescriptorSet(frame, currentDescriptorSubset);
		
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &set, 0, nullptr); //monitor
	}

	// --------------------------------------------------------------------
	// Render 5 -  Draw
	// --------------------------------------------------------------------

	vkCmdDrawIndexed(commandBuffer,
		indexCount,
		1,			// instanceCount
		start,		// firstIndex
		0,			// vertexOffset
		0);			// firstInstance

#if defined _DEBUG || defined LINUX_PLATFORM //|| defined ANDROID_PLATFORM
	SystemHandle->TotalVertexCounter += indexCount;
#endif
}

void vkShaderClass::ShutdownShader()
{
	womalog("vkShaderClass::ShutdownShader() ---> MODEL: %s\n", MODEL_NAME.c_str());

	if (!m_device)
	{
		womalog("WARNING: no m_device\n");
		return;
	}

	// No longer necessary
	vkDestroyShaderModule(m_device, m_vertexShaderModule, nullptr);
	vkDestroyShaderModule(m_device, m_fragmentShaderModule, nullptr);

	//womalog("vkShaderClass::ShutdownShader() 1. Pipelines\n");
	// ------------------------------------------------------------
	// 1. Pipelines
	// ------------------------------------------------------------

	for (UINT Rpass = 0; Rpass < WOMA_NUM_RENDERPASS_PIPELINES; Rpass++)
	{
		for (UINT zBuffer = 0; zBuffer < 2; zBuffer++)
		{
			for (UINT cullMode = 0; cullMode < 3; cullMode++)
			{
				for (UINT fillMode = 0; fillMode < 2; fillMode++)
				{
					for (UINT alphaBlend = 0; alphaBlend < MAXNUM_PIPELINE_STATES; alphaBlend++)
					{
						VkPipeline& pipeline = m_pipelineState[zBuffer][cullMode][fillMode][alphaBlend][Rpass];
						//womalog(
						//	"PIPELINE [%d %d %d %d %d] = %p\n",
						//	zBuffer,
						//	cullMode,
						//	fillMode,
						//	alphaBlend,
						//	Rpass,
						//	(void*)pipeline);
						if (pipeline) 
						{
							vkDestroyPipeline(m_device, pipeline, nullptr);
							pipeline = VK_NULL_HANDLE;
							//womalog("Run vkDestroyPipeline(): %d %d %d %d %d\n", zBuffer,cullMode,fillMode,alphaBlend,Rpass);
						}
					}
				}
			}
		}
	}

	//womalog("vkShaderClass::ShutdownShader() 2. Pipeline layout\n");
	// ------------------------------------------------------------
	// 2. Pipeline layout
	// ------------------------------------------------------------

	if (m_pipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
		m_pipelineLayout = VK_NULL_HANDLE;
	}

	//womalog("vkShaderClass::ShutdownShader() 3. Descriptor pool\n");
	// ------------------------------------------------------------
	// 3. Descriptor pool
	// ------------------------------------------------------------

	if (m_descriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
		m_descriptorPool = VK_NULL_HANDLE;
	}

	//womalog("vkShaderClass::ShutdownShader() 4. Descriptor set layouts\n");
	// ------------------------------------------------------------
	// 4. Descriptor set layouts
	// ------------------------------------------------------------

	if (m_descriptorColorSetLayout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(
			m_device,
			m_descriptorColorSetLayout,
			nullptr);

		m_descriptorColorSetLayout = VK_NULL_HANDLE;
	}

	if (m_descriptorTextureSetLayout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(
			m_device,
			m_descriptorTextureSetLayout,
			nullptr);

		m_descriptorTextureSetLayout = VK_NULL_HANDLE;
	}
	if (m_descriptorTextureLightSetLayout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(
			m_device,
			m_descriptorTextureLightSetLayout,
			nullptr);

		m_descriptorTextureLightSetLayout = VK_NULL_HANDLE;
	}

	//womalog("vkShaderClass::ShutdownShader() 5. VS uniform buffer\n");
	// ------------------------------------------------------------
	// 5. VS uniform buffer
	// ------------------------------------------------------------

	if (m_uniformBufferVS != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_device, m_uniformBufferVS, nullptr);
		m_uniformBufferVS = VK_NULL_HANDLE;
	}

	if (m_uniformBufferVSMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_device, m_uniformBufferVSMemory, nullptr);
		m_uniformBufferVSMemory = VK_NULL_HANDLE;
	}

	womalog("vkShaderClass::ShutdownShader() 6. PS uniform buffer\n");
	// ------------------------------------------------------------
	// 6. PS uniform buffer
	// ------------------------------------------------------------

	if (m_uniformBufferPS != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_device, m_uniformBufferPS, nullptr);
		m_uniformBufferPS = VK_NULL_HANDLE;
	}

	if (m_uniformBufferPSMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_device, m_uniformBufferPSMemory, nullptr);
		m_uniformBufferPSMemory = VK_NULL_HANDLE;
	}

	//womalog("ShutDown Model: %s vkShaderClass::ShutdownShader() END\n", MODEL_NAME.c_str());
}


void vkShaderClass::AllocateSubsetDescriptors(UINT subsetCount, VkDevice DriverDevice)
{
	descriptorSubsetCount = subsetCount;

	for (UINT f = 0; f < MAX_FRAMES_IN_FLIGHT; f++)
		m_descriptorSet[f].resize(subsetCount);

	//
	// createDescriptorSet
	//
	if (subsetCount>1)
		createDescriptorSet(DriverDevice);
}

#endif
