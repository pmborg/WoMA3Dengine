// ----------------------------------------------------------------------------------------------
// Filename: dxWinSystemClass.h
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

#pragma warning( disable : 4065 )	// warning C4065: switch statement contains 'default' but no 'case' labels
#define _CRT_SECURE_NO_WARNINGS

#include "DXcameraClass.h"

#include "WinSystemClass.h"

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
#include "AudioClass.h"
#endif

#define castShadows_false	false
#define renderShadows_false false
#define modelHASlight_false	false

#define castShadows_true	true
#define renderShadows_true	true
#define modelHASlight_true	true

#define obj_instances_0		0
#define obj_instances_1		1
#define obj_instances_2		2
#define obj_instances_4		4
#define obj_instances_8		8


////////////////////////////////////////////////////////////////////////////////
// Class name: WinSystemClass
////////////////////////////////////////////////////////////////////////////////
class dxWinSystemClass : public WinSystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	dxWinSystemClass(WOMA::Settings* AppSettings);
	~dxWinSystemClass();

	bool InitSelectedDriver();
	bool InitializeSystem();
	int	 ApplicationMainLoop();
	void ProcessFrame();

	void ProcessOSInput();

	bool LoadWorldXmlSettings();

	bool ApplicationInitMainWindow();

	void PAUSE();
	void UNPAUSE();
	void GPH_RESIZE();

	DXcameraClass* m_Camera = NULL;
	DXcameraClass* m_CameraSKY = NULL;

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
	bool SaveScreenshot();
#endif

	// Movement
	DWORD						joyFlags = 0;
	DXInputClass* m_Input = NULL;
	std::vector<PlayerClass*>	m_player;

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
	AudioClass* audio=NULL;
	bool StartSoundManager();
#endif
#if defined USE_PLAY_MUSIC || defined INTRO_DEMO
	SoundID MusicID=NULL;
	SoundSourceID MusicSourceID = NULL;
#endif

	void ApplicationInitSceneManager();

	//private:
	void Shutdown();
};

extern dxWinSystemClass* DXsystemHandle;

#define HowManyPlayers DXsystemHandle->m_player.size()
