// --------------------------------------------------------------------------------------------
// Filename: RFontClass.h
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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut14.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;

#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//////////////
// INCLUDES //
//////////////
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Rtextureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RFontClass
////////////////////////////////////////////////////////////////////////////////
class RFontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
        float x, y, z;
        float tu, tv;
	};

public:
	RFontClass();
	RFontClass(const RFontClass&);
	~RFontClass();

	bool Initialize(OpenGLClass*, int);
	void Shutdown();

	void BuildVertexArray(void*, char*, float, float);
	int GetSentencePixelLength(char*);
	int GetFontHeight();

	void SetTexture();

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(char*);
	void ReleaseTexture();

private:
    OpenGLClass* m_OpenGLPtr;
	FontType* m_Font;
	RTextureClass* m_Texture;
	float m_fontHeight;
	int m_spaceSize;
};

#endif
#endif