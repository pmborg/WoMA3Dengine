// --------------------------------------------------------------------------------------------
// Filename: RTextClass.h
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
//WomaIntegrityCheck = 1234525217;

#ifndef _RTextClass_H_
#define _RTextClass_H_

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//////////////
// INCLUDES //
//////////////
#include "Rfontclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RTextClass
////////////////////////////////////////////////////////////////////////////////
class RTextClass
{
private:
	struct VertexType
	{
		float x, y, z;
        float tu, tv;
	};

public:
	RTextClass();
	RTextClass(const RTextClass&);
	~RTextClass();

	bool Initialize(OpenGLClass*, int, int, int, RFontClass*, char*, int, int, float, float, float);
	void Shutdown();
	void Render();

	bool UpdateText(RFontClass*, char*, int, int, float, float, float);
    void GetPixelColor(float*);

private:
	bool InitializeBuffers(RFontClass*, char*, int, int, float, float, float);
	void ShutdownBuffers();
	void RenderBuffers();

private:
    OpenGLClass* m_OpenGLPtr;
    int m_screenWidth, m_screenHeight, m_maxLength, m_vertexCount, m_indexCount;
    unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
    float m_pixelColor[4];
};

#endif
#endif