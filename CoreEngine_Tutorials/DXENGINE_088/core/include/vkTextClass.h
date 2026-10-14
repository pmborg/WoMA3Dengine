// --------------------------------------------------------------------------------------------
// Filename: VkTextClass.h
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

#pragma once

//////////////
// INCLUDES //
//////////////
#include "platform.h"
#if (defined USE_RASTERTEK_TEXT_FONT || defined USE_RASTERTEK_TEXT_FONTV2) && defined VULKAN
#include "virtualTextClass.h"
#include "textFontClass.h"
#include "vkClass.h"
#include "vkShaderClass.h"

class VkTextClass : public VirtualTextClass
{
public:
	VkTextClass();
	~VkTextClass();

	void Shutdown();

	bool Initialize(void* Driver, void* pContext);
	bool InitializeTexture(void* Driver);

	bool UpdateSentence(SentenceType*, TCHAR*, int, int, float, float, float);

	bool InitializeSentence(SentenceType**, int);

	void ReleaseSentence(SentenceType**);
	void RenderSentence(void* pContext, SentenceType*);

private:
	//
	//  VARIABLES:
	//
	textFontClass* m_Font = NULL;

	VkPipeline m_pipeline = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

	VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;

	//------------------------------
	vkShaderClass* m_spriteShader = NULL;
	mat4* m_baseViewMatrix = NULL;

	VkDevice m_device = NULL;
	vkClass* m_driver = NULL;
};

#endif
