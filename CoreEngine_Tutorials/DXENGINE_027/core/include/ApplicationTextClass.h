// --------------------------------------------------------------------------------------------
// Filename: ApplicationTextClass.h
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
//WomaIntegrityCheck = 1234567222;

#pragma once

#include "main.h"
#include "virtualTextClass.h"

// Index position in the ARRAY:
#define TEXT_FPS			0
#define TEXT_CPU			1
#define TEXT_POS			2
#define TEXT_ROT			3
#define TEXT_TIME			4
#define TEXT_FADE			5
#define TEXT_ScreenToShow	6
#define TEXT_CLOCK			7
#define TEXT_LIGHT_DIR		8
#define TEXT_TERRAINRENDERCOUNT 9	// 60
#define TEXT_LATENCY			10	// 140

#define N_TEXT_MAX_SENTENCE 11 // LAST+1

namespace DirectX {

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationTextClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationTextClass
{
public:
	UINT WomaIntegrityCheck = 1234567222;
	ApplicationTextClass();
	~ApplicationTextClass();

	bool Initialize(void* Driver);
	bool Shutdown();
	void Render();

	void SetFps(int);
	void SetCpu(int);

	void SetCameraPosition(float, float, float);
	void SetCameraRotation(float, float, float);
#if defined EXTRA_INFO
	void SetClockTime(UINT, UINT);
	void SetLightDirection(float rotX, float rotY, float rotZ);
	#endif

	#if defined INTRO_DEMO
		void SetTime(INT64 dt);
		void SetFade(float fade);
		void SetScreenToShow(UINT screenToShow, UINT render_page);
	#endif//

    #if TUTORIAL_PRE_CHAP >= 12
    void SetCompoundPosition(WCHAR *file, float dist, UINT, float, float, float, float scale);
    #endif

	#if DX_ENGINE_LEVEL >= 30
    void SetRenderCount(int terrainCount, int compoundCount, UINT totalCompoundLoaded);
	#endif//

	#if TUTORIAL_PRE_CHAP >= 14
	void SetLoboRenderCount(int count);
	#endif//

	#if TUTORIAL_PRE_CHAP >= 45
	void SetBillRenderCount(int count);
	#endif//

	#if TUTORIAL_PRE_CHAP >= 63
	void SetXP(UINT XP, UINT maxXP, float percent);
	void SetGold(float gold);
	#endif//

	#if TUTORIAL_PRE_CHAP >= 90
	void SetLatency(int latency);
	#endif//

	#if TUTORIAL_PRE_CHAP >= 200
	//TextClass now has a new function for setting the data to print out.
	void SetVideoCardInfo(char*, int);
	void SetMousePosition(int mouseX, int mouseY, LONG mouseMovement);
	void SetVideoCardInfo (DXGI_ADAPTER_DESC* adapterDesc);
	#endif

	SentenceType* m_sentence[N_TEXT_MAX_SENTENCE] = { 0 }; //std::vector<SentenceType*>	m_sentence;	
	VirtualTextClass*	m_Text = NULL;
};

}
