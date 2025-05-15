// --------------------------------------------------------------------------------------------
// Filename: Rtextureclass.h
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

#ifndef _RTextureClass_H_
#define _RTextureClass_H_

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//////////////
// INCLUDES //
//////////////
#include <stdio.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Ropenglclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RTextureClass
////////////////////////////////////////////////////////////////////////////////
class RTextureClass
{
private:
    struct TargaHeader
	{
        unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
    RTextureClass();
    //RTextureClass(const RTextureClass&);
    ~RTextureClass();

    bool Initialize(OpenGLClass*, char*, unsigned int, bool);
    void Shutdown();

    void SetTexture(OpenGLClass*);

    int GetWidth();
    int GetHeight();

private:
    bool LoadTarga32Bit(OpenGLClass*, char*, bool);

private:
    unsigned int m_textureUnit, m_textureID;
    int m_width, m_height;
    bool m_loaded;
};

#endif
#endif