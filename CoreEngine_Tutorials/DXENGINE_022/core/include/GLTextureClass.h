// --------------------------------------------------------------------------------------------
// Filename: glTextureclass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2025
// -------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// ********************************************************************************************
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
