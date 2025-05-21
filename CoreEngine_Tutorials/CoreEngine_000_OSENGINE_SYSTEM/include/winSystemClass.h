// --------------------------------------------------------------------------------------------
// Filename: winSystemClass.h
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
// PURPOSE: Export APIs for winSystemClass.cpp which is the WINDOWS OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;

#pragma once

//////////////
// INCLUDES //
//////////////

#include "platform.h"

#if defined WINDOWS_PLATFORM
#include "SystemClass.h"

#if defined CLIENT_SCENE_SETUP
#include "WomaSetupManager.h"
#endif

#define WOMA_ENGINE_CLASS TEXT("WoMA3Dengine")

#define g_contextDriver NULL

#if defined USE_STATUSBAR
extern HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);
#endif


#define check(A)							{}

////////////////////////////////////////////////////////////////////////////////
// Class name: WinSystemClass
////////////////////////////////////////////////////////////////////////////////
class WinSystemClass : public SystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567155;
	WinSystemClass();
	WinSystemClass(WOMA::Settings* AppSettings);
	void WinSystemClass_init();
	~WinSystemClass();
	void Shutdown();

	bool APPLICATION_BEFORE_WINDOW();
    bool APPLICATION_AFTER_WINDOW();
	bool APPLICATION_INIT_SYSTEM();
	int	 APPLICATION_MAIN_LOOP();

	HWND	m_hWnd;

#if defined ALLOW_CBIND_PROGRESS_BAR
	HWND hwndPrgBar = NULL;
	HWND settingstext = NULL;
#endif

private:
	void ShutdownWindows();


};

/////////////
// GLOBALS //
/////////////
extern WinSystemClass* SystemHandle;
#endif
