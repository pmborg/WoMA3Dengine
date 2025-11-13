// --------------------------------------------------------------------------------------------
// Filename: win32PaintEvents.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Paint the main window depending of engine state screen page.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#include "WinSystemClass.h"
#include "OSmain_dir.h"
#include "mem_leak.h"
#include "stateMachine.h"

#if CORE_ENGINE_LEVEL >= 2 && defined WINDOWS_PLATFORM 

#if defined ALLOW_LOADING_SPLASH && defined WINDOWS_PLATFORM  //ENGINE_LEVEL >= 10 && 
#include "fileLoader.h"
#endif

#if CORE_ENGINE_LEVEL >= 4 && defined USE_USER_SETUP
int		MainWindowPaint(UINT monitor);
#endif

#if defined ALLOW_LOADING_SPLASH
void	PaintSplashScreen(HDC hdc);
#endif


// ---------------------------------------------------------------------------------------------
LRESULT CALLBACK WOMA_PAINT_Message_event_handler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
// ---------------------------------------------------------------------------------------------
{
	switch (umessage)
	{
	case WM_CREATE: // LOAD BACKGROUND IMAGE: NOTE: "LoadBitmap" DONT load inside a LIB!
	{

		break;
	}


    #if defined USE_USER_SETUP
	case WM_PAINT:
	{
#if defined USE_INTRO_VIDEO_DEMO
		if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
#endif
		{
			for (UINT i = 0; i < SystemHandle->windowsArray.size(); i++)
				MainWindowPaint(i);
		}

		break;
	}
    #endif

	}
	return SystemHandle->WOMA_SYSTEM_MessageHandler(hwnd, umessage, wparam, lparam);
}

#if CORE_ENGINE_LEVEL >= 4 && defined USE_USER_SETUP
void PaintSetup(HDC hdc, HDC hdcMem, HFONT font_title, HFONT font, int scr) 
{
	// Paint all text Fonts:
	// ---------------------------------------------------------------------------------------------
	if (WOMA::game_state == GAME_SETUP) 
	{
		SelectObject(hdcMem, font_title);		//Select the Font to Render

		const TCHAR SETUP[] = TEXT("S E T U P");
		TextOut(hdcMem, 25, 25, SETUP, (int)_tcslen(SETUP));

	//#if CORE_ENGINE_LEVEL == 5
		//BitBlt(hdc, 0, 0, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY);
	//#else
		BitBlt(hdc, 0, 0, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, SRCPAINT);
	//#endif
	} else {
		HGDIOBJ obj = SelectObject(hdcMem, font);		//Select the Font to Render

		if (scr >= 0)
		{
			int TextToPrintSize = (int)SystemHandle->TextToPrint[scr].size();

			#if CORE_ENGINE_LEVEL >= 4 && defined USE_SYSTEM_CHECK // BEFORE need to be: APPLICATION_INIT_MAIN_WINDOW() & AFTER need to be: InitSelectedDriver()
			if (TextToPrintSize == 0)
				SystemHandle->InitializeSystemScreenF1(10, 10);		// SETUP SCREEN: F1,F2,F3,F4,F5,F6
			#endif

			TextToPrintSize = (int)SystemHandle->TextToPrint[scr].size();
			for (size_t i = 0; i < TextToPrintSize; i++)
			{
				TextOut(hdcMem, SystemHandle->TextToPrint[scr][i].x, SystemHandle->TextToPrint[scr][i].y,
					SystemHandle->TextToPrint[scr][i].label.c_str(), (int)_tcslen(SystemHandle->TextToPrint[scr][i].label.c_str()));
				BitBlt(hdc, 0, 0, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, SRCPAINT);
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------
int MainWindowPaint(UINT monitor)
// ---------------------------------------------------------------------------------------------
{
	// int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	// DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision,
	// DWORD iQuality, DWORD iPitchAndFamily, _In_opt_ LPCSTR pszFaceName);

	/*due RESIZE: cant be static!!!*/
	HFONT font = CreateFont((int)SystemHandle->fontSizeX, 0, 0, 0, 
							(int)SystemHandle->fontSizeY, FALSE,
							FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							ANTIALIASED_QUALITY, DEFAULT_PITCH, TEXT("Calibri"));

	HFONT font_title = CreateFont(50, 0, 0, 0, 50, FALSE,
		FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH, TEXT("Calibri"));

	PAINTSTRUCT ps;

	HDC hdc;
	hdc = BeginPaint(SystemHandle->windowsArray[monitor].hWnd, &ps);

	// Which Screen we will use:
	int scr = -1;
	switch (WOMA::game_state)
	{
	case GAME_SYSTEM_SETTINGS:
		scr = 0;
		break;
#if defined USE_ASTRO_CLASS
	case GAME_CELESTIAL_INFO:
		scr = 1;
		break;
#endif
#if defined USE_METARCLASS
	case GAME_WEATHER_INFO:
		scr = 2;
		break;
#endif
	}

#if defined  (WINDOWS_PLATFORM)

	// Paint BackGround Image:
	// ---------------------------------------------------------------------------------------------
	#if defined ALLOW_LOADING_SPLASH
	if (WOMA::game_state <= GAME_MENU)
		PaintSplashScreen(hdc); // (Loading Splash Screen) Default Background
	#endif

	switch (WOMA::game_state)
	{

		case GAME_SETUP:			//F6
		#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
		if (!SystemHandle->womaSetup)
		{
			//
				SystemHandle->womaSetup = NEW WomaSetupManager;
				if (!SystemHandle->womaSetup->m_setupWnd)
					SystemHandle->womaSetup->Initialize(NULL);
		}
		#endif

			break;
	}//switch

	static COLORREF color = RGB(200, 200, 200);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT);
	HANDLE hbmOld = SelectObject(hdcMem, hBmp);
	SetBkColor(hdcMem, TRANSPARENT);
	SetTextColor(hdcMem, color);

	PaintSetup(hdc, hdcMem, font_title, font, scr);

#if (defined USE_LOADING_THREADS || DX_ENGINE_LEVEL >= 37) && !defined ALLOW_CBIND_PROGRESS_BAR
	if (WOMA::game_state == GAME_LOADING)
	{
		TCHAR printOnLoading[MAX_STR_LEN] = { 0 };
		if (WOMA::num_loading_objects < SystemHandle->xml_loader.theWorldXML.size())
	#if defined SAVEW3D
			StringCchPrintf(printOnLoading, MAX_STR_LEN, TEXT("Loading OBJ -> Saving W3D: [%d/%d] %s"), WOMA::num_loading_objects, SystemHandle->xml_loader.theWorldXML.size(), SystemHandle->xml_loader.theWorldXML[WOMA::num_loading_objects-1].filename);
	#else
			StringCchPrintf(printOnLoading, MAX_STR_LEN, TEXT("Loading OBJ: [%d/%d] %s"), WOMA::num_loading_objects, SystemHandle->xml_loader.theWorldXML.size(), SystemHandle->xml_loader.theWorldXML[WOMA::num_loading_objects - 1].filename);
	#endif

		Woma_Label TextToPrintOnLoading;
		TextToPrintOnLoading.label = printOnLoading;
		TextToPrintOnLoading.x = 25;
		TextToPrintOnLoading.y = 25;

		TextOut(hdcMem, TextToPrintOnLoading.x, TextToPrintOnLoading.y,
			TextToPrintOnLoading.label.c_str(), (int)_tcslen(TextToPrintOnLoading.label.c_str()));
		BitBlt(hdc, 0, 0, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, SRCPAINT);
	}
#endif

	// Restore the old bitmap
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

// ---------------------------------------------------------------------------------------------
	EndPaint(SystemHandle->m_hWnd, &ps);
#endif

	return scr;
}

#endif

#endif
