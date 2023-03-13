// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: win32PaintEvents.cpp
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
// PURPOSE: Paint the main window depending of engine state screen page.
// --------------------------------------------------------------------------------------------

#include "WinSystemClass.h"
#include "OSmain_dir.h"
#include "mem_leak.h"

#include "fileLoader.h"

#include "initWorld.h"

int		MainWindowPaint(UINT monitor);

void	PaintSplashScreen(HDC hdc);

//#define LineHeigth 20

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

	case WM_PAINT:
	{
		for (UINT i = 0; i < SystemHandle->windowsArray.size(); i++)
			MainWindowPaint(i);
		break;
	}

	}
	return SystemHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}

void PaintSetup(HDC hdc, HDC hdcMem, HFONT font_title, HFONT font, int scr) 
{
	// Paint all text Fonts:
	// ---------------------------------------------------------------------------------------------
	if (WOMA::game_state == GAME_SETUP) {
		SelectObject(hdcMem, font_title);		//Select the Font to Render

		const TCHAR SETUP[] = TEXT("S E T U P");
		TextOut(hdcMem, 50, 50, SETUP, (int)_tcslen(SETUP));
		BitBlt(hdc, 0, 0, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, SRCPAINT);
	}
	else {
		SelectObject(hdcMem, font);		//Select the Font to Render

		if (scr >= 0)
		{
			int TextToPrintSize = (int)SystemHandle->TextToPrint[scr].size();
			for (size_t i = 0; i < TextToPrintSize; i++)
			{
				TextOut(hdcMem, SystemHandle->TextToPrint[scr][i].x, SystemHandle->TextToPrint[scr][i].y,
					SystemHandle->TextToPrint[scr][i].label.c_str(), (int)_tcslen(SystemHandle->TextToPrint[scr][i].label.c_str()));
				BitBlt(hdc, 0, 0, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT, hdcMem, 0, 0, SRCPAINT);
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
	HFONT font = CreateFont((int)SystemHandle->fontSizeX, 0, 0, 0, (int)SystemHandle->fontSizeY, FALSE,
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
	case GAME_CELESTIAL_INFO:
		scr = 1;
		break;
	}

	//if (WOMA::game_state != GAME_SETUP)
	//	return scr;

	// Paint BackGround Image:
	// ---------------------------------------------------------------------------------------------
	if (WOMA::game_state <= GAME_MENU)
		PaintSplashScreen(hdc); // (Loading Splash Screen) //PaintSetupScreen(hdc);	// (Default Background)

	switch (WOMA::game_state)
	{

		case GAME_SETUP:			//F6
		if (!SystemHandle->womaSetup)
			//
				SystemHandle->womaSetup = NEW WomaSetupManager;
				if (!SystemHandle->womaSetup->m_setupWnd)
					SystemHandle->womaSetup->Initialize(NULL);

			PaintSplashScreen(hdc); // Loading Splash Screen.
			break;
	}//switch

	static COLORREF color = RGB(200, 200, 200);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT);
	HANDLE hbmOld = SelectObject(hdcMem, hBmp);
	SetBkColor(hdcMem, TRANSPARENT);
	SetTextColor(hdcMem, color);

	PaintSetup(hdc, hdcMem, font_title, font, scr);

	// Restore the old bitmap
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
// ---------------------------------------------------------------------------------------------
	EndPaint(SystemHandle->m_hWnd, &ps);

	return scr;
}

