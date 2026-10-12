// --------------------------------------------------------------------------------------------
// Filename: GlTextClass.h
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

#define Y_INIT 40

extern TCHAR* g_SetCameraRotation(float rotX, float rotY, float rotZ, float* pixelColor, int* positionX, int* positionY);
extern TCHAR* g_SetFps(int fps, float* pixelColor, int* positionX, int* positionY);
extern TCHAR* g_SetCameraPosition(float posX, float posY, float posZ, float* pixelColor, int* positionX, int* positionY);

#if defined USE_RASTERTEK_TEXT_FONT || defined USE_RASTERTEK_TEXT_FONTV2
extern ApplicationTextClass* g_AppTextClass;
#endif

#ifdef USE_RASTERTEK_TEXT_FONT
//OPENGL for Windows Platform only

#if (defined OPENGL3 || defined OPENGL4)
#include "virtualTextClass.h"
#include "GLshaderClass.h"
#include "textFontClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GlTextClass
////////////////////////////////////////////////////////////////////////////////
class GlTextClass : public VirtualTextClass
{
public:
	//NOTE: need to be local! to not interfear with others!

#if defined WINDOWS_PLATFORM //DX_ENGINE 
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

	GlTextClass();
	~GlTextClass();

	void Shutdown();
	bool Initialize(void* g_driver, void* pContext);
	bool InitializeTexture(void* Driver);

	bool UpdateSentence(SentenceType*, TCHAR*, int, int, float, float, float);
	bool InitializeSentence(SentenceType**, int);

	void ReleaseSentence(SentenceType**);
	void RenderSentence(void* pContext, SentenceType*);

	GLshaderClass* m_spriteShader = NULL;

	mat4* m_baseViewMatrix = NULL;
	textFontClass* m_Font = NULL;
};

#endif
#endif
