// --------------------------------------------------------------------------------------------
// Filename: glTextureclass.h
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
// --------------------------------------------------------------------------------------------//WomaIntegrityCheck = 1234526920;
// WomaIntegrityCheck = 1234526920;

#pragma once
#ifndef gltextureclass_h
#define gltextureclass_h

#include "platform.h"
#include "ImageLoaderClass.h"

#if defined VULKAN
#include "vkClass.h"
#endif


//////////////
// INCLUDES //
//////////////
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: GLtextureClass
////////////////////////////////////////////////////////////////////////////////
class GLtextureClass
{
public:
	GLtextureClass();
	~GLtextureClass();

	bool Initialize(void* pContext, TCHAR* filename, unsigned int textureUnit, bool wrap);
	void Shutdown();
	bool loadImage(void* pContext, TCHAR* fileName, bool = true);

private:
	bool loaded = false;
	ImageLoaderClass ImageLoader;
	void AddTexture(unsigned char* image32b, int width, int height, bool wrap);
	
	bool AddTextureVulkan(void* pContext, unsigned char* image32b, int width, int height, bool wrap, UINT imageSize);
	
public:
	//Common:
	UINT width=0, height=0;		// Image Dimensions

	//OPENGL specific:
	unsigned int m_textureID;
	
#ifdef VULKAN
	//VULKAN specific:
	VkImage        m_vkImage = NULL;
	VkDeviceMemory m_vkImageMemory = NULL;
	VkImageView    m_vkImageView = NULL;
	VkBuffer	   m_vkStagingBuffer = NULL;
	VkDeviceMemory m_vkStagingMemory = NULL;

	bool m_wrap = true;    // true = Repeat, false = Clamp
#endif
};
#endif
