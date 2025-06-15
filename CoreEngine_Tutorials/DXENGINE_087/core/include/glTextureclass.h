// --------------------------------------------------------------------------------------------
// Filename: glTextureclass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: 
// --------------------------------------------------------------------------------------------//WomaIntegrityCheck = 1234567155;

#pragma once

#include "main.h"

#include "ImageLoaderClass.h"

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
	UINT WomaIntegrityCheck = 1234567155;
	GLtextureClass();
	~GLtextureClass();

	bool Initialize(TCHAR*, UINT, bool);
	void Shutdown();
	bool loadImage(TCHAR* fileName, bool = true);

private:
	bool loaded;
	ImageLoaderClass ImageLoader;
	void AddTexture(unsigned char* image32b, int width, int height, bool wrap);
	
public:
	unsigned int m_textureID;
	UINT width, height;		// Image Dimensions
};
