// --------------------------------------------------------------------------------------------
// Filename: GlTextClass.h
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
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525256;

#pragma once

//////////////
// INCLUDES //
//////////////
#include "platform.h"
#ifdef USE_RASTERTEK_TEXT_FONT

#if (defined OPENGL3 || defined OPENGL4)
#include "virtualTextClass.h"
#include "GLshaderClass.h"
#include "textFontClass.h"
// ----------------------------------------
#include "ApplicationTextClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GlTextClass
////////////////////////////////////////////////////////////////////////////////
class GlTextClass : public VirtualTextClass
{
public:
	//NOTE: need to be local! to not interfear with others!
/*
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
*/

#if defined DX_ENGINE 
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
#else
	#pragma pack(push, 1)
	struct VertexType
	{
		Vector3 position;
		Vector2 texture;
	};
	#pragma pack(pop)
#endif

	UINT WomaIntegrityCheck = 1234525256;
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

#endif
#endif
