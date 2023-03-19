// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: WindowPaintClass.cpp
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

#include "WinSystemClass.h"
#include "mem_leak.h"
#include "WindowPaintClass.h"

//	-------------------------------------------------------------------------------------------
	#include "initWorld.h"
	#include "astroClass.h"

	#include "fileLoader.h"
	HBITMAP bmpExercising = NULL;

#define initWorld SystemHandle->m_Application->initWorld

void InitializeCelestialInfoScreen(int x, int y)
//-----------------------------------------------------------------------------------------
{
	WOMA::logManager->DEBUG_MSG ("InitializeCelestialInfoScreen...");

	TCHAR str[50];

	float LINE = MIN (25, 2 * SystemHandle->fontSizeY);
	float LINE_SPACE = MIN (35, 3 * SystemHandle->fontSizeY);

	if (initWorld) // Needed because paint is called before create initWorld
	{
		// Make sure that is Reset!
		while (!SystemHandle->TextToPrint[1].empty())
			SystemHandle->TextToPrint[1].pop_back();

		// CelestialInfo:
		STRING infomode = TEXT("online");

		StringCchPrintf(str, sizeof(str), TEXT("CELESTIAL INFO: %s"), infomode.c_str());
		Woma_Label text = { str, x, y };
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Latitude: %f"), initWorld->latitude);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Longitude: %f"), initWorld->longitude);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		// Static Sun Values:
		// ------------------
		text.y += 30;
		StringCchPrintf(str, sizeof(str), TEXT("Sunrise: %d:%d"), (UINT)initWorld->h_sunrise, (UINT)initWorld->m_sunrise);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Sunset: %d:%d"), (UINT)initWorld->h_sunset, (UINT)initWorld->m_sunset);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		// Static Moon Values:
		// -------------------
		text.y += 30;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Distance: %f (km)"), initWorld->moonDistance);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Scale: %f / 1.0"), initWorld->moonScale);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		// Dynamic Sun Values:
		// -------------------
		text.y += 30;
		StringCchPrintf(str, sizeof(str), TEXT("Sun Geometric Elevation: %f"), initWorld->geometricElevation);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Elevation: %f (with refraction)"), (initWorld->geometricMoonElevation > -5) ? initWorld->moonElevation : NULL);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Azimuth: %f"), initWorld->MoonAz);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		WOMA::logManager->DEBUG_MSG(" done\n");
	}
}




void PaintSplashScreen(HDC hdc)
{
    if (!bmpExercising) {
        bmpExercising = (HBITMAP)::LoadImage(NULL, WOMA::LoadFile(TEXT("engine/data/logotipo_small_backgroundV2.bmp")), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
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
    // SCALE: Stretch the bits from the memory DC into the current dc
    StretchBlt(	hdc, 0, 0, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND | SRCCOPY);
	//printf("%d %d\n", SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT);
    // ---------------------------------------------------------------------------------------------

    // Restore the old bitmap
    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

	#if TUTORIAL_PRE_CHAP >= 7 // totalPackCounter --- 100%
	TCHAR str[100];			   // packCounter  --- X
	if (totalPackCounter > 0) 
	{
		StringCchPrintf(str, sizeof(str), TEXT("Loading: %d %%"), packCounter * 100 / totalPackCounter);
		TextOut(hdc, 10, yPos, str, (int) _tcslen(str)); yPos += LineHeigth;
	}
	#endif
}
