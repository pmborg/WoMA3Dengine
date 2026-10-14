// --------------------------------------------------------------------------------------------
// Filename: VKrenderTextureClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "OSengine.h"
#if DX_ENGINE_LEVEL >= 36 && defined VULKAN // ShadowMap
#include "VKrenderTextureClass.h"
#include "log.h"
#include "OSengine.h"
#include "mem_leak.h"

VKrenderTextureClass::VKrenderTextureClass()
{
	CLASSLOADER();

	// public:
	m_shaderTextureResourceView = VK_NULL_HANDLE;

	// private:
	m_depthImage          = VK_NULL_HANDLE;
	m_depthMemory         = VK_NULL_HANDLE;
	m_depthView           = VK_NULL_HANDLE;

	m_framebuffer         = VK_NULL_HANDLE;
	m_renderPass          = VK_NULL_HANDLE;

	ZeroMemory(&m_TextureViewport, sizeof(m_TextureViewport));
}

VKrenderTextureClass::~VKrenderTextureClass() 
{ 
	Shutdown(); 
	CLASSDELETE(); 
}

bool VKrenderTextureClass::Initialize(void* pContext, int textureWidth, int textureHeight,
									  int viewportX, int viewportY, float screenDepth, float screenNear)
{
	m_driver = (vkClass*)m_Driver;
	VkDevice vkDevice = m_driver->m_device;
	VkResult result;

	womalog("VKrenderTextureClass::Initialize() - Creating Offscreen Shadow RenderTexture (%d x %d)\n", textureWidth, textureHeight);

	m_shaderTextureResourceView = NEW GLtextureClass;

	// --------------------------------------------------------------------------------
	// 1. CREATE COLOR TEXTURE (Like in m_renderTextureTarget on DX11)
	// --------------------------------------------------------------------------------
	VkImageCreateInfo colorImageCI{};
	colorImageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	colorImageCI.imageType = VK_IMAGE_TYPE_2D;
	colorImageCI.format = VK_FORMAT_R8G8B8A8_UNORM; // VK_FORMAT_R8G8B8A8_UNORM; // Like: DXGI_FORMAT_R8G8B8A8_UNORM
	colorImageCI.extent.width = textureWidth;
	colorImageCI.extent.height = textureHeight;
	colorImageCI.extent.depth = 1;
	colorImageCI.mipLevels = 1;
	colorImageCI.arrayLayers = 1;
	colorImageCI.samples = VK_SAMPLE_COUNT_1_BIT; // Offscreen shadow maps 1:1
	colorImageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
	
	colorImageCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	colorImageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	colorImageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	result = vkCreateImage(vkDevice, &colorImageCI, nullptr, &m_shaderTextureResourceView->m_vkImage);
	ASSERT(result == VK_SUCCESS);

	VkMemoryRequirements colorMemReqs;
	vkGetImageMemoryRequirements(vkDevice, m_shaderTextureResourceView->m_vkImage, &colorMemReqs);

	VkMemoryAllocateInfo colorAllocInfo{};
	colorAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	colorAllocInfo.allocationSize = colorMemReqs.size;
	colorAllocInfo.memoryTypeIndex = m_driver->FindMemoryType(colorMemReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	result = vkAllocateMemory(vkDevice, &colorAllocInfo, nullptr, &m_shaderTextureResourceView->m_vkImageMemory);
	ASSERT(result == VK_SUCCESS);

	vkBindImageMemory(vkDevice, m_shaderTextureResourceView->m_vkImage, m_shaderTextureResourceView->m_vkImageMemory, 0);

	// Create internal view (For m_renderTextureView / RTV)
	VkImageViewCreateInfo colorViewCI{};
	colorViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	colorViewCI.image = m_shaderTextureResourceView->m_vkImage	;
	colorViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
	colorViewCI.format = colorImageCI.format;
	colorViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	colorViewCI.subresourceRange.levelCount = 1;
	colorViewCI.subresourceRange.layerCount = 1;
	colorViewCI.subresourceRange.baseMipLevel = 0;
	colorViewCI.subresourceRange.baseArrayLayer = 0;

	result = vkCreateImageView(vkDevice, &colorViewCI, nullptr, &m_shaderTextureResourceView->m_vkImageView);
	ASSERT(result == VK_SUCCESS);

	// --------------------------------------------------------------------------------
	// 2. CREATE DEPTH TEXTURE (Like in m_depthStencilTextureBuffer on DX11)
	// --------------------------------------------------------------------------------
	VkFormat depthFormat = m_driver->FindDepthFormat();

	VkImageCreateInfo depthImageCI{};
	depthImageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	depthImageCI.imageType = VK_IMAGE_TYPE_2D;
	depthImageCI.format = depthFormat;
	depthImageCI.extent.width = textureWidth;
	depthImageCI.extent.height = textureHeight;
	depthImageCI.extent.depth = 1;
	depthImageCI.mipLevels = 1;
	depthImageCI.arrayLayers = 1;
	depthImageCI.samples = VK_SAMPLE_COUNT_1_BIT;
	depthImageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
	depthImageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	depthImageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	depthImageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	result = vkCreateImage(vkDevice, &depthImageCI, nullptr, &m_depthImage);
	ASSERT(result == VK_SUCCESS);

	VkMemoryRequirements depthMemReqs;
	vkGetImageMemoryRequirements(vkDevice, m_depthImage, &depthMemReqs);

	VkMemoryAllocateInfo depthAllocInfo{};
	depthAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	depthAllocInfo.allocationSize = depthMemReqs.size;
	depthAllocInfo.memoryTypeIndex = m_driver->FindMemoryType(depthMemReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	result = vkAllocateMemory(vkDevice, &depthAllocInfo, nullptr, &m_depthMemory);
	ASSERT(result == VK_SUCCESS);

	vkBindImageMemory(vkDevice, m_depthImage, m_depthMemory, 0);

	VkImageViewCreateInfo depthViewCI{};
	depthViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	depthViewCI.image = m_depthImage;
	depthViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
	depthViewCI.format = depthFormat;
	depthViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	depthViewCI.subresourceRange.levelCount = 1;
	depthViewCI.subresourceRange.layerCount = 1;
	depthViewCI.subresourceRange.baseMipLevel = 0;
	depthViewCI.subresourceRange.baseArrayLayer = 0;

	result = vkCreateImageView(vkDevice, &depthViewCI, nullptr, &m_depthView);
	ASSERT(result == VK_SUCCESS);


	// --------------------------------------------------------------------------------
	// 3. RECREATE A DIDICATED RENDER PASS FOR OFFSCREEN RENDERING
	// --------------------------------------------------------------------------------
	VkAttachmentDescription attachments[2]{};
	
	// Color:
	attachments[0].format = colorImageCI.format;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // Ready to read.

	// Depth:
	attachments[1].format = depthFormat;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorRef{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentReference depthRef{ 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorRef;
	subpass.pDepthStencilAttachment = &depthRef;

	VkRenderPassCreateInfo rpCI{};
	rpCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpCI.attachmentCount = 2;
	rpCI.pAttachments = attachments;
	rpCI.subpassCount = 1;
	rpCI.pSubpasses = &subpass;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	rpCI.dependencyCount = 1;
	rpCI.pDependencies = &dependency;

	result = vkCreateRenderPass(vkDevice, &rpCI, nullptr, &m_renderPass);
	ASSERT(result == VK_SUCCESS);


	// --------------------------------------------------------------------------------
	// 4. CREATE THE PERSISTENT OFFSCREEN FRAMEBUFFER
	// --------------------------------------------------------------------------------
	VkImageView fbAttachments[2] = { m_shaderTextureResourceView->m_vkImageView, m_depthView };

	VkFramebufferCreateInfo fbCI{};
	fbCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fbCI.renderPass = m_renderPass;
	fbCI.attachmentCount = 2;
	fbCI.pAttachments = fbAttachments;
	fbCI.width = textureWidth;
	fbCI.height = textureHeight;
	fbCI.layers = 1;

	result = vkCreateFramebuffer(vkDevice, &fbCI, nullptr, &m_framebuffer);
	ASSERT(result == VK_SUCCESS);

	// --------------------------------------------------------------------------------
	// 5. CONFIGURATE A LOCAL VIEWPORT FOR PASSE
	// --------------------------------------------------------------------------------
	m_TextureViewport.x = 0.0f;
	m_TextureViewport.y = 0.0f;
	m_TextureViewport.width = (float)textureWidth;
	m_TextureViewport.height = (float)textureHeight;
	m_TextureViewport.minDepth = 0.0f;
	m_TextureViewport.maxDepth = 1.0f;

	// --------------------------------------------------------------------------------
	// 6. SETUP M_VIEWPORTCREATEINFO
	// --------------------------------------------------------------------------------
	m_TextureScissor.offset = { 0,0 };
	m_TextureScissor.extent.width = textureWidth;
	m_TextureScissor.extent.height = textureHeight;

	m_viewportCreateInfo = {};
	m_viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_viewportCreateInfo.viewportCount = 1;
	m_viewportCreateInfo.pViewports = &m_TextureViewport;
	m_viewportCreateInfo.scissorCount = 1;
	m_viewportCreateInfo.pScissors = &m_TextureScissor;

	return true;
}

void VKrenderTextureClass::Shutdown()
{
	if (!m_driver || !m_driver->m_device) return;

	VkDevice vkDevice = m_driver->m_device;

	if (m_framebuffer) { vkDestroyFramebuffer(vkDevice, m_framebuffer, nullptr); m_framebuffer = VK_NULL_HANDLE; }
	if (m_renderPass)  { vkDestroyRenderPass(vkDevice, m_renderPass, nullptr); m_renderPass = VK_NULL_HANDLE; }

	if (m_depthView) { vkDestroyImageView(vkDevice, m_depthView, nullptr); m_depthView = VK_NULL_HANDLE; }
	if (m_depthImage) { vkDestroyImage(vkDevice, m_depthImage, nullptr); m_depthImage = VK_NULL_HANDLE; }
	if (m_depthMemory) { vkFreeMemory(vkDevice, m_depthMemory, nullptr); m_depthMemory = VK_NULL_HANDLE; }
	if (m_shaderTextureResourceView) { m_shaderTextureResourceView->Shutdown(); SAFE_SHUTDOWN(m_shaderTextureResourceView); }
}

void VKrenderTextureClass::SetRenderTarget(void* pContext)
{
	VkCommandBuffer cmd = (VkCommandBuffer)pContext;

	ASSERT_DEBUG(m_framebuffer);

	// ---------------------------------------------------------
	// Shadow-map viewport/scissor
	// ---------------------------------------------------------
	m_TextureViewport.x = 0.0f;
	m_TextureViewport.y = 0.0f;
	m_TextureViewport.minDepth = 0.0f;
	m_TextureViewport.maxDepth = 1.0f;

	m_TextureScissor.offset = { 0, 0 };
	m_TextureScissor.extent = {
		(uint32_t)m_TextureViewport.width,
		(uint32_t)m_TextureViewport.height
	};

	m_viewportCreateInfo = {};
	m_viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_viewportCreateInfo.viewportCount = 1;
	m_viewportCreateInfo.pViewports = &m_TextureViewport;
	m_viewportCreateInfo.scissorCount = 1;
	m_viewportCreateInfo.pScissors = &m_TextureScissor;

	// ---------------------------------------------------------
	// Begin render pass
	// ---------------------------------------------------------
	VkRenderPassBeginInfo rpBegin{};
	rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpBegin.renderPass = m_renderPass;

	rpBegin.framebuffer = m_framebuffer;

	rpBegin.renderArea.offset = { 0, 0 };
	rpBegin.renderArea.extent = m_TextureScissor.extent;

	static VkClearValue clearValues[2];
	clearValues[0].color = { { 1, 1, 1, 1.0f } }; //white color + A=1
	clearValues[1].depthStencil = { 1.0f, 0 };

	rpBegin.clearValueCount = 2;
	rpBegin.pClearValues = clearValues;

	vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

	// ---------------------------------------------------------
	// Dynamic state
	// ---------------------------------------------------------
	vkCmdSetViewport(cmd, 0, 1, &m_TextureViewport);
	vkCmdSetScissor(cmd, 0, 1, &m_TextureScissor);
}

void VKrenderTextureClass::ClearRenderTarget(void* pContext, float red, float green, float blue, float alpha)
{
	// Not used in Vulkan, because we clear the render target in SetRenderTarget() with vkCmdBeginRenderPass()
}

#endif
