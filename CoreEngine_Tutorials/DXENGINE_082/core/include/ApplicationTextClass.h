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
//WomaIntegrityCheck = 1234525217;

#pragma once

#include "main.h"
#include "virtualTextClass.h"

// Index position in the ARRAY:
#define TEXT_FPS			    0
#define TEXT_CPU			    1
#define TEXT_POS			    2
#define TEXT_ROT			    3
#define TEXT_TIME			    4
#define TEXT_FADE			    5
#define TEXT_ScreenToShow	    6
//#define TEXT_CLOCK		    7
#define TEXT_BILLRENDERCOUNT    7
#define TEXT_LIGHT_DIR		    8
#define TEXT_TERRAINRENDERCOUNT 9	// 60
#define TEXT_BILLRENDERCOUNT    10  // 89
#define TEXT_LATENCY			11	// 140

#if DX_ENGINE_LEVEL >= 89 && defined USE_MAP_EDITOR // #if TUTORIAL_PRE_CHAP >= 12
#define TEXT_COMPOUND_ID    12
#define TEXT_COMPOUND       13 //LAST!
#endif

#define N_TEXT_MAX_SENTENCE 14 // LAST+1



#if TUTORIAL_PRE_CHAP >= 45 && TUTORIAL_PRE_CHAP < 55// BILLBOARD
#define N_TEXT_MAX_SENTENCE 8
#endif

#if TUTORIAL_PRE_CHAP >= 55
#define TEXT_TIME 8
#endif
#if TUTORIAL_PRE_CHAP >= 55 && TUTORIAL_PRE_CHAP < 63
#define N_TEXT_MAX_SENTENCE 9
#endif

#if TUTORIAL_PRE_CHAP >= 63 // 83
#define TEXT_XP 9
#define TEXT_GOLD 10
#endif
#if TUTORIAL_PRE_CHAP >= 63 && TUTORIAL_PRE_CHAP < 90
#define N_TEXT_MAX_SENTENCE 11
#endif
#if defined _NOT
// Not used yet...
// ----------------------------------------
/*
#if TUTORIAL_PRE_CHAP >= 50
#define TEXT_WATERRENDERCOUNT 9
#endif
#if TUTORIAL_PRE_CHAP >= 50 && TUTORIAL_PRE_CHAP < 60
#define N_TEXT_MAX_SENTENCE 10
#endif
*/
#endif

namespace DirectX {

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationTextClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationTextClass
{
public:
	UINT WomaIntegrityCheck = 1234525217;
	ApplicationTextClass();
	~ApplicationTextClass();

	bool Initialize(void* Driver);
	bool Shutdown();
	void Render();

	void SetFps(int);
	void SetCpu(int);

	void SetCameraPosition(float, float, float);
	void SetCameraRotation(float, float, float);
#if defined EXTRA_INFO2
	void SetInfoA(UINT, UINT);
	void SetInfoB(float rotX, float rotY, float rotZ);
	#endif

	#if defined INTRO_DEMO
		void SetTime(INT64 dt);
		void SetFade(float fade);
		void SetScreenToShow(UINT screenToShow, UINT render_page);
	#endif

    #if DX_ENGINE_LEVEL >= 89 && defined USE_MAP_EDITOR // #if TUTORIAL_PRE_CHAP >= 12 
    void SetCompoundPosition(CHAR* file, float dist, UINT, float, float, float, float scale, float rotY);
    #endif

	#if DX_ENGINE_LEVEL >= 30 && _DEBUG
    void SetRenderCount(int terrainCount, int compoundCount, UINT totalCompoundLoaded);
	#endif

	#if TUTORIAL_PRE_CHAP >= 14 && _DEBUG
	void SetLoboRenderCount(int count);
	#endif

#if TUTORIAL_CHAP >= 60 && !defined RELEASE // BILLBOARD
	void SetBillRenderCount(int count, UINT deltaTime);
	#endif

	#if TUTORIAL_PRE_CHAP >= 63 && _DEBUG
	void SetXP(UINT XP, UINT maxXP, float percent);
	void SetGold(float gold);
	#endif

	#if TUTORIAL_PRE_CHAP >= 90 && _DEBUG
	void SetLatency(int latency);
	#endif

	#if TUTORIAL_PRE_CHAP >= 200 && _DEBUG
	//TextClass now has a new function for setting the data to print out.
	void SetVideoCardInfo(char*, int);
	void SetMousePosition(int mouseX, int mouseY, LONG mouseMovement);
	void SetVideoCardInfo (DXGI_ADAPTER_DESC* adapterDesc);
	#endif

	SentenceType* m_sentence[N_TEXT_MAX_SENTENCE] = { 0 };
	VirtualTextClass*	m_Text = NULL;
};

}
