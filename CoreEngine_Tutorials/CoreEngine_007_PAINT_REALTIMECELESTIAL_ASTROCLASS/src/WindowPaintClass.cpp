// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: WindowPaintClass.cpp
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
//WomaIntegrityCheck = 1234567142;

#include "OSengine.h"
#include "WindowPaintClass.h"

//	-------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 7 && defined USE_ASTRO_CLASS
	#include "initWorld.h"
	#include "astroClass.h"
#endif

#if defined ALLOW_LOADING_SPLASH && defined WINDOWS_PLATFORM
	#include "fileLoader.h"
	HBITMAP bmpExercising = NULL;
#endif

#define initWorld SystemHandle->m_Application->initWorld



#if defined ALLOW_LOADING_SPLASH && defined WINDOWS_PLATFORM
void PaintSplashScreen(HDC hdc)
{
    if (!bmpExercising) {
        bmpExercising = (HBITMAP)::LoadImage(NULL, WOMA::LoadFile(BACKGROUND_IMAGE), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        if (!bmpExercising)	//Might happend that unpack of Temp dir didnt happend yet: (so this is not an error...)
		{ 
			STRING err = TEXT("BMP File not found: "); 
			err += WOMA::lastfile; 
			MessageBox(NULL, err.c_str(), TEXT("WARNING: LoadingImage"), MB_ICONWARNING); //WomaFatalExceptionW((WCHAR*) err.c_str()); 
			return;
		}
	}

	BITMAP bm = { 0 };
    HDC hdcMem = CreateCompatibleDC(hdc);					// Create a memory device compatible with the above DC variable
    HGDIOBJ hbmOld = SelectObject(hdcMem, bmpExercising);	// Get old and Select the new bitmap

    // ---------------------------------------------------------------------------------------------
    GetObject(bmpExercising, sizeof(bm), &bm);

    // COPY: the bits from the memory DC into the current dc
    //BitBlt(hdc, (SystemHandle->AppSettings->WINDOW_WIDTH -bm.bmWidth)/2, (SystemHandle->AppSettings->WINDOW_HEIGHT -bm.bmHeight)/2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	// 
    // SCALE: Stretch the bits from the memory DC into the current dc
    StretchBlt(	hdc, 0, 0, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND | SRCCOPY);

	// ---------------------------------------------------------------------------------------------
    // Restore the old bitmap
	HGDIOBJ obj = SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

}
#endif
