// --------------------------------------------------------------------------------------------
// Filename: VkTextClass.cpp
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

#include "platform.h"
#if (defined USE_RASTERTEK_TEXT_FONT || defined USE_RASTERTEK_TEXT_FONTV2) && defined VULKAN
#include "vkTextClass.h"
#include "mem_leak.h"
#include "OSengine.h"

VkTextClass::VkTextClass() 
{
	CLASSLOADER();

	//private:
	m_Font = NULL;
	m_baseViewMatrix = NULL;

	m_spriteShader = NULL;

	m_driver = NULL;
	m_device = NULL;
}


VkTextClass::~VkTextClass() 
{ 
	Shutdown(); 
	CLASSDELETE(); 
}

void VkTextClass::Shutdown() 
{
	SAFE_SHUTDOWN(m_spriteShader);	// Release the font shader object.
	SAFE_SHUTDOWN(m_Font);			// Release the textFontClass object.
}

bool VkTextClass::Initialize(void* Driver, void* pContext) 
{
#ifdef DEBUG_TEXT_FONT
	womalog(TEXT("[VK TEXT] Initialize()\n"));
#endif

	m_driver = (vkClass*)Driver;
	m_device = m_driver->m_device;

	m_baseViewMatrix = &m_driver->gl_Camera->m_viewmatrix2D;

	// TextClass: Initialize the font object. PART1
	m_Font = NEW textFontClass;
	IF_NOT_THROW_EXCEPTION(m_Font); // Create the font object.
	IF_NOT_THROW_EXCEPTION(m_Font->Initialize(pContext, Driver, FONT_DATA_FILE, FONT_DATA_TEXTURE));
	
	m_spriteShader = NEW vkShaderClass;
	IF_NOT_THROW_EXCEPTION(m_spriteShader);
	IF_NOT_THROW_EXCEPTION(m_spriteShader->Initialize(-1, TEXT("VkTextClass"), SHADER_TEXTURE_FONT, m_device, NULL, TRIANGLELIST));

	// Used "Color" and no default "Texture" so load it:
	m_spriteShader->isFontShader = true;
	m_spriteShader->hasTexture = false; // Will use Pixel Color

	return true;
}

bool VkTextClass::InitializeTexture(void* Driver)
{
	GLtextureClass* fontTexture = m_Font->gl_Texture;

#ifdef DEBUG_TEXT_FONT
	womalog(TEXT("[VK TEXT] InitializeTexture() called\n"));
#endif

	ASSERT_DEBUG(fontTexture);

	VkDescriptorImageInfo textureDescriptor = {};
	textureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	textureDescriptor.imageView = fontTexture->m_vkImageView;
	textureDescriptor.sampler = ((vkClass*)Driver)->m_vkSampler[SAMPLER_WRAP_FALSE];

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstBinding = 1; // shaderTexture / t0
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &textureDescriptor;

	for (UINT i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		descriptorWrite.dstSet = m_spriteShader->m_descriptorSet30[i]; //frame
		vkUpdateDescriptorSets(m_device, 1, &descriptorWrite, 0, nullptr);
	}

	return true;
}

bool VkTextClass::UpdateSentence(SentenceType* sentence, TCHAR* text, int positionX, int positionY, float red, float green, float blue)
{
		textFontVertexType* vertices;
		float drawX, drawY;

		if (!sentence)
			{ WomaFatalException("Text: sentence = NULL"); }

		// Store the color of the sentence.
		sentence->red = red;
		sentence->green = green;
		sentence->blue = blue;

		// Get the number of letters in the sentence.
		sentence->numLetters = (int)_tcslen(text);

		// Check for possible buffer overflow.
		ASSERT_DEBUG(sentence->numLetters < textmaxLength);

		sentence->indexCount = sentence->numLetters * 6;

		// Create the vertex array.
		vertices = NEW textFontVertexType[sentence->indexCount /*sentence->vertexCount*/];
		IF_NOT_THROW_EXCEPTION(vertices);

		// Initialize vertex array to zeros.
		//memset(vertices, 0, sizeof(textFontVertexType) * sentence->indexCount /*sentence->vertexCount*/);

		// Calculate the X and Y pixel position.
		drawX = (float)(((WOMA::AppSettings->WINDOW_WIDTH / 2) * -1) + positionX);
		drawY = (float)((WOMA::AppSettings->WINDOW_HEIGHT / 2) - positionY);

		// Build the vertex array from the sentence text.
		m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

		void* mappedVertexMemory = nullptr;

		VkResult result = vkMapMemory(
			m_device,
			sentence->vkVertexBufferMemory,
			0,
			sizeof(textFontVertexType) * sentence->indexCount,
			0,
			&mappedVertexMemory);

		if (result != VK_SUCCESS)
		{
			SAFE_DELETE_ARRAY(vertices);
			return false;
		}

		memcpy(mappedVertexMemory, vertices, sizeof(textFontVertexType) * sentence->indexCount);

		if (m_Driver->RenderfirstTime)
		{
			textFontVertexType* debugVertices = static_cast<textFontVertexType*>(mappedVertexMemory);

#ifdef DEBUG_TEXT_FONT
			for (int i = 0; i < 6; i++)
			{
				womalog(TEXT("[VK FONT] VB[%d] Pos=(%f,%f,%f) UV=(%f,%f)\n"),
					i,
					debugVertices[i].position.x,
					debugVertices[i].position.y,
					debugVertices[i].position.z,
#if defined WINDOWS_PLATFORM
					debugVertices[i].texture.x,
					debugVertices[i].texture.y
#else
					debugVertices[i].texture.u,
					debugVertices[i].texture.v
#endif
				);
			}

#endif
		}

		vkUnmapMemory(m_device, sentence->vkVertexBufferMemory);

		SAFE_DELETE_ARRAY(vertices);

	return true;
}

bool VkTextClass::InitializeSentence(SentenceType** sentence, int maxLength)
{
#ifdef DEBUG_TEXT_FONT
	womalog(TEXT("VkTextClass::InitializeSentence()\n"));
#endif

	// Create a new sentence object.
	*sentence = NEW SentenceType;
	IF_NOT_THROW_EXCEPTION(*sentence);

	// Set the maximum length of the sentence.
	//(*sentence)->maxLength = maxLength;
	textmaxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * textmaxLength;
	//(*sentence)->vertexCount = 6 * (*sentence)->numLetters;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	(*sentence)->VKvertices = NEW textFontVertexType[(*sentence)->vertexCount];
	IF_NOT_THROW_EXCEPTION((*sentence)->VKvertices);

	// Create the index array.
	// NOTE Vulkan requires 32-bit indices here.
	//		Never use unsigned long: MSVC=4 bytes, GCC/Clang x64=8 bytes.
	(*sentence)->indices = NEW uint32_t[(*sentence)->indexCount];
	IF_NOT_THROW_EXCEPTION((*sentence)->indices);

	// Initialize vertex array to zeros.
	memset((*sentence)->VKvertices, 0, sizeof(textFontVertexType) * (*sentence)->vertexCount);

	// Initialize the index array.
	for (int i = 0; i < (*sentence)->indexCount; i++)
	{
		(*sentence)->indices[i] = i;
	}

	// ------------------------------------------------------------------------
	// VULKAN VERTEX BUFFER
	// ------------------------------------------------------------------------

	const VkDeviceSize vertexBufferSize = sizeof(textFontVertexType) * (*sentence)->vertexCount;

	VkBufferCreateInfo vertexBufferInfo{};
	vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vertexBufferInfo.size = vertexBufferSize;
	vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult vkResult = vkCreateBuffer(m_device, &vertexBufferInfo, nullptr, &(*sentence)->vkVertexBuffer);
	ASSERT(vkResult == VK_SUCCESS);

	// Get memory requirements.
	VkMemoryRequirements vertexMemRequirements{};
	vkGetBufferMemoryRequirements(m_device, (*sentence)->vkVertexBuffer, &vertexMemRequirements);

	// Allocate memory.
	VkMemoryAllocateInfo vertexAllocInfo{};
	vertexAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vertexAllocInfo.allocationSize = vertexMemRequirements.size;
	vertexAllocInfo.memoryTypeIndex = m_driver->FindMemoryType(vertexMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkResult = vkAllocateMemory(m_device, &vertexAllocInfo, nullptr, &(*sentence)->vkVertexBufferMemory);
	if (vkResult != VK_SUCCESS)
		ASSERT(vkResult == VK_SUCCESS);

	// Bind memory.
	vkResult = vkBindBufferMemory(m_device, (*sentence)->vkVertexBuffer, (*sentence)->vkVertexBufferMemory, 0);
	ASSERT(vkResult == VK_SUCCESS);

	// Upload initial vertex data.
	void* mappedVertexMemory = nullptr;

	vkResult = vkMapMemory(m_device, (*sentence)->vkVertexBufferMemory, 0, vertexBufferSize, 0, &mappedVertexMemory);
	ASSERT(vkResult == VK_SUCCESS);

	memcpy(mappedVertexMemory, (*sentence)->VKvertices, (size_t)vertexBufferSize);

	vkUnmapMemory(m_device, (*sentence)->vkVertexBufferMemory);

	// ------------------------------------------------------------------------
	// VULKAN INDEX BUFFER
	// ------------------------------------------------------------------------

	VkDeviceSize indexBufferSize = sizeof(uint32_t) * (*sentence)->indexCount;

	VkBufferCreateInfo indexBufferInfo{};
	indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	indexBufferInfo.size = indexBufferSize;
	indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkResult = vkCreateBuffer(m_device, &indexBufferInfo, nullptr, &(*sentence)->vkIndexBuffer);
	ASSERT(vkResult == VK_SUCCESS);

	// Get memory requirements.
	VkMemoryRequirements indexMemRequirements{};
	vkGetBufferMemoryRequirements(m_device, (*sentence)->vkIndexBuffer, &indexMemRequirements);

	// Allocate memory.
	VkMemoryAllocateInfo indexAllocInfo{};
	indexAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	indexAllocInfo.allocationSize = indexMemRequirements.size;
	indexAllocInfo.memoryTypeIndex =
		m_driver->FindMemoryType(
			indexMemRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkResult = vkAllocateMemory(m_device, &indexAllocInfo, nullptr, &(*sentence)->vkIndexBufferMemory);
	ASSERT(vkResult == VK_SUCCESS);

	// Bind memory.
	vkResult = vkBindBufferMemory(m_device, (*sentence)->vkIndexBuffer, (*sentence)->vkIndexBufferMemory, 0);
	ASSERT(vkResult == VK_SUCCESS);

	// Upload index data.
	void* mappedIndexMemory = nullptr;

	vkResult = vkMapMemory(m_device, (*sentence)->vkIndexBufferMemory, 0, indexBufferSize, 0, &mappedIndexMemory);
	ASSERT(vkResult == VK_SUCCESS);

	memcpy(mappedIndexMemory, (*sentence)->indices, (size_t)indexBufferSize);

	vkUnmapMemory(m_device, (*sentence)->vkIndexBufferMemory);

	// The GPU buffers now contain the initial data.
	SAFE_DELETE_ARRAY((*sentence)->VKvertices);
	SAFE_DELETE_ARRAY((*sentence)->indices);

	return true;
}

void VkTextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		SAFE_DELETE_ARRAY((*sentence)->VKvertices);
		SAFE_DELETE_ARRAY((*sentence)->indices);

		// Release the Vulkan sentence vertex buffer.
		if ((*sentence)->vkVertexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_device, (*sentence)->vkVertexBuffer, nullptr);
			(*sentence)->vkVertexBuffer = VK_NULL_HANDLE;
		}

		// Release the Vulkan sentence vertex buffer memory.
		if ((*sentence)->vkVertexBufferMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(m_device, (*sentence)->vkVertexBufferMemory, nullptr);
			(*sentence)->vkVertexBufferMemory = VK_NULL_HANDLE;
		}

		// Release the Vulkan sentence index buffer.
		if ((*sentence)->vkIndexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_device, (*sentence)->vkIndexBuffer, nullptr);
			(*sentence)->vkIndexBuffer = VK_NULL_HANDLE;
		}

		// Release the Vulkan sentence index buffer memory.
		if ((*sentence)->vkIndexBufferMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(m_device, (*sentence)->vkIndexBufferMemory, nullptr);
			(*sentence)->vkIndexBufferMemory = VK_NULL_HANDLE;
		}

		// Release the sentence.
		delete* sentence;
		*sentence = nullptr;
	}
}

void VkTextClass::RenderSentence(void* pContext, SentenceType* sentence)
{
	VkCommandBuffer m_Context = (VkCommandBuffer)pContext;

	VkDeviceSize offset = 0;

	// ------------------------------------------------------------------------
	// 1. Matrices
	// ------------------------------------------------------------------------

	static mat4 m_worldMatrix = m_worldMatrix.mat4identity();
	static mat4 m_viewMatrix = m_viewMatrix.mat4identity();

	m_viewMatrix.m[14] = 1;

	mat4 projectionMatrix = m_driver->m_orthoMatrix;

	// ------------------------------------------------------------------------
	// 2. Font color
	// ------------------------------------------------------------------------
	m_spriteShader->pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);


	// ------------------------------------------------------------------------
	// 3. Bind Vertex Buffer
	// ------------------------------------------------------------------------
	vkCmdBindVertexBuffers(m_Context, 0, 1, &sentence->vkVertexBuffer, &offset);

	// ------------------------------------------------------------------------
	// 4. Bind Index Buffer
	// ------------------------------------------------------------------------
	vkCmdBindIndexBuffer(m_Context, sentence->vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

	// ------------------------------------------------------------------------
	// 5. Render
	// ------------------------------------------------------------------------
#ifdef DEBUG_TEXT_FONT
	if (m_Driver->RenderfirstTime)
		womalog(
			TEXT("[VK TEXT] RenderSentence: VB=%p IB=%p vertices=%u indices=%u sizeofVertex=%zu\n"),
			sentence->vkVertexBuffer,
			sentence->vkIndexBuffer,
			sentence->vertexCount,
			sentence->indexCount,
			sizeof(textFontVertexType));
#endif

	m_spriteShader->Render(0, m_Context, sentence->indexCount, &m_worldMatrix, &m_viewMatrix, &projectionMatrix);
}

#endif
