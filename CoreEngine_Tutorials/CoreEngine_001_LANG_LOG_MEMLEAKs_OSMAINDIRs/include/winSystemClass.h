// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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
//WomaIntegrityCheck = 1234567311;

#pragma once

//////////////
// INCLUDES //
//////////////
//#define _CRT_SECURE_NO_WARNINGS		// Ignore: warning C4996
#include "SystemClass.h"
#if defined WINDOWS_PLATFORM

#define WOMA_ENGINE_CLASS TEXT("WoMA3Dengine")

#define g_contextDriver NULL

extern HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);

#if defined WINDOWS_PLATFORM
#include <combaseapi.h>				// VC7: ships with updated headers: CoInitializeEx()

#define check(A)							{}

////////////////////////////////////////////////////////////////////////////////
// Class name: WinSystemClass
////////////////////////////////////////////////////////////////////////////////
class WinSystemClass : public SystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567311;
	WinSystemClass();
	WinSystemClass(WOMA::Settings* AppSettings);
	void WinSystemClass_init();
	~WinSystemClass();
	void Shutdown();

	bool APPLICATION_CORE_SYSTEM();
	bool APPLICATION_INIT_SYSTEM();
	int	 APPLICATION_MAIN_LOOP();

	HWND	m_hWnd = 0;

private:
	void ShutdownWindows();
};

/////////////
// GLOBALS //
/////////////
extern WinSystemClass* SystemHandle;

#endif

#endif
