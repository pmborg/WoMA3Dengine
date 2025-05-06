// --------------------------------------------------------------------------------------------
// Filename: win32PaintEvents.cpp
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
// PURPOSE: Paint the main window depending of engine state screen page.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#include "OSengine.h"
#include "WinSystemClass.h"
#include "OSmain_dir.h"
#include "mem_leak.h"
#include "stateMachine.h"

#if CORE_ENGINE_LEVEL >= 2 && defined WINDOWS_PLATFORM 

#if defined ALLOW_LOADING_SPLASH && defined WINDOWS_PLATFORM  //ENGINE_LEVEL >= 10 && 
#include "fileLoader.h"
#endif

#if defined ALLOW_LOADING_SPLASH
void	PaintSplashScreen(HDC hdc);
#endif


// ---------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
// ---------------------------------------------------------------------------------------------
{
	switch (umessage)
	{
	case WM_CREATE: // LOAD BACKGROUND IMAGE: NOTE: "LoadBitmap" DONT load inside a LIB!
	{

		break;
	}
#if defined ALLOW_CBIND_PROGRESS_BAR
	case WM_DRAWITEM: // DRAW TEXT FROM PROGRESS BAR
		// --------------------------------------------------------------------------------------------
	{
        if (WOMA::game_state <= GAME_RUN) {
		try {
			LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lparam;
			if (pDIS->hwndItem == SystemHandle->settingstext) {
				SetBkColor(pDIS->hDC, TRANSPARENT);
				SetTextColor(pDIS->hDC, RGB(200, 200, 200));
				TCHAR staticText[99];
				int len = (int)SendMessage(SystemHandle->settingstext, WM_GETTEXT, ARRAYSIZE(staticText), (LPARAM)staticText);

				TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
			}
		}
		catch (...) {
			// Log or handle the exception gracefully
		}
        }
		break;
	}
#endif

#ifdef _EXTRA_DEBUG
	default:
	{
		WOMA::logManager->DEBUG_MSG(TEXT("Msg: %04X \n"), umessage);
	}
#endif

	}
	return SystemHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}

#endif
