// ----------------------------------------------------------------------------------------------
// Filename: dxWinSystemClass.h
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
#pragma once

#include "platform.h"

#if defined DX_ENGINE

#include "WinSystemClass.h"

#include "DXcameraClass.h"

#if DX_ENGINE_LEVEL >= 29 && defined USE_WIN32_SOUND_MANAGER || defined USE_WIN32_PLAY_MUSIC
#include "AudioClass.h"
#endif

// Used to Print Labels (on win WM_PAINT):
// --------------------------------------------------------------------------------------------
#include "ApplicationClass.h"

#if defined USE_INTRO_VIDEO_DEMO
#include <dshow.h>
#include "playback.h"
#endif

extern PSTR Scmdline;

extern bool threadInitializeLoaderAlive;
extern HANDLE threadInitializeLoaderHandle;
extern unsigned long threadInitializeLoaderId;

#ifdef INTRO_DEMO
extern WCHAR* DEMO_Legends[];
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: WinSystemClass
////////////////////////////////////////////////////////////////////////////////
class dxWinSystemClass : public WinSystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567222;
	dxWinSystemClass(WOMA::Settings* AppSettings);
	~dxWinSystemClass();

	bool APPLICATION_INIT_SYSTEM();
	int	 APPLICATION_MAIN_LOOP();
	void ProcessFrame();

	void ProcessOSInput();

	bool LoadWorldXmlSettings();

	bool ApplicationInitMainWindow();

	void PAUSE();
	void UNPAUSE();
	void GPH_RESIZE();

#if DX_ENGINE_LEVEL >= 20 && defined USE_LOADING_THREADS
	bool ApplicationMandatoryLoad();
#endif

	DXcameraClass* m_Camera = NULL;
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	DXcameraClass* m_CameraSKY = NULL;
#endif
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
	DXcameraClass* m_CameraMAP = NULL;
#endif
#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
	bool SaveScreenshot();
#endif

#if defined USE_DIRECT_INPUT
	// Movement
	DWORD						joyFlags = 0;
	DXInputClass*				m_Input = NULL;
#endif

#if DX_ENGINE_LEVEL >= 29 && (defined USE_WIN32_SOUND_MANAGER || defined USE_WIN32_PLAY_MUSIC)
	AudioClass* audio=NULL;
	bool StartSoundManager();
#endif
#if defined USE_WIN32_PLAY_MUSIC || defined INTRO_DEMO
	SoundID MusicID=NULL;
	SoundSourceID MusicSourceID = NULL;
#endif

#if defined USE_SCENE_MANAGER
	void ApplicationInitSceneManager();
#endif

#if defined USE_INTRO_VIDEO_DEMO
	DShowPlayer* g_DShowPlayer;
#endif
#if defined USE_INTRO_VIDEO_DEMO	//35: VIDEO DEMO
	HRESULT PlayIntroMovie(TCHAR* movie);
#endif

	//private:
	void Shutdown();
};

extern dxWinSystemClass* DXsystemHandle;


