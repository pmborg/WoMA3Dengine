// --------------------------------------------------------------------------------------------
// Filename: VKrenderTextureClass.h
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

#ifndef _VKRENDERTEXTURECLASS_H_
#define _VKRENDERTEXTURECLASS_H_

#ifdef VULKAN

#include "vkClass.h"
#include "glTextureclass.h"
class VKrenderTextureClass
{
public:
	VKrenderTextureClass();
	~VKrenderTextureClass();

	bool Initialize(void* pContext, int textureWidth, int textureHeight,
		int viewportX, int viewportY,
		float screenDepth, float screenNear);

	void Shutdown();

	void SetRenderTarget(void* pContext);
	void ClearRenderTarget(void* pContext, float red, float green, float blue, float alpha);

public:
	//VkImageView m_shaderTextureImageView = VK_NULL_HANDLE;
	GLtextureClass* m_shaderTextureResourceView = VK_NULL_HANDLE;
	VkRenderPass    m_renderPass = VK_NULL_HANDLE;

	
	VkPipelineViewportStateCreateInfo m_viewportCreateInfo{};

private:
	vkClass*		m_driver;

	VkRect2D		m_TextureScissor{};
	VkViewport      m_TextureViewport{};

	VkImage         m_depthImage = VK_NULL_HANDLE;
	VkDeviceMemory  m_depthMemory = VK_NULL_HANDLE;
	VkImageView     m_depthView = VK_NULL_HANDLE;

	VkFramebuffer   m_framebuffer = VK_NULL_HANDLE;
};

#endif
#endif
