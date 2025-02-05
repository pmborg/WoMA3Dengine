// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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
//WomaIntegrityCheck = 1234567311;

#include "OSengine.h"
#include "WinSystemClass.h"
#include "OSmain_dir.h"


#if CORE_ENGINE_LEVEL >= 2 && defined WINDOWS_PLATFORM 


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
