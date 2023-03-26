// --------------------------------------------------------------------------------------------
// Filename: GlTextClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
// --------------------------------------------------------------------------------------------

#pragma once

//////////////
// INCLUDES //
//////////////
#include "platform.h"

/*
#include "dx11Class.h"
#include "dx12Class.h"
*/
#include "virtualTextClass.h"
#include "GLshaderClass.h"
#include "textFontClass.h"
// ----------------------------------------

#if TUTORIAL_PRE_CHAP >= 45 //60 // BILLBOARD
#define TEXT_BILLRENDERCOUNT 7
#endif
#if TUTORIAL_PRE_CHAP >= 45 && TUTORIAL_PRE_CHAP < 55// BILLBOARD
#define N_TEXT_MAX_SENTENCE 8
#endif

#if TUTORIAL_PRE_CHAP >= 63 // 83
#define TEXT_XP 9
#define TEXT_GOLD 10
#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: GlTextClass
////////////////////////////////////////////////////////////////////////////////
class GlTextClass : public VirtualTextClass
{
public:
	GlTextClass();
	~GlTextClass();

	void Shutdown();
	bool Initialize(void* g_driver);
	bool InitializeTexture(void* Driver);

	bool UpdateSentence(SentenceType*, TCHAR*, int, int, float, float, float);
	bool InitializeSentence(SentenceType**, int);

	void ReleaseSentence(SentenceType**);
	void RenderSentence(SentenceType*);

	GLshaderClass* m_spriteShader = NULL;

	mat4* m_baseViewMatrix = NULL;
	textFontClass* m_Font = NULL;
};

