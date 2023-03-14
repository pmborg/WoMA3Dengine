// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: winSystemClass.h
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
//
// PURPOSE: Export APIs for winSystemClass.cpp which is the WINDOWS OS API
//
// --------------------------------------------------------------------------------------------
#pragma once

//////////////
// INCLUDES //
//////////////
#define _CRT_SECURE_NO_WARNINGS		// Ignore: warning C4996
#include "platform.h"
#include "SystemClass.h"

#include "WomaSetupManager.h"

#include <combaseapi.h>				// VC7: ships with updated headers: CoInitializeEx()

#define WOMA_ENGINE_CLASS TEXT("WoMA3Dengine")

extern HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);

struct ScreenArrayInfo
{
	ScreenArrayInfo* Array;
	int Count;
	int MaxCount;
};

namespace WOMA
{
	struct WindowDataContainer
	{
		// 1
		STRING MonitorName;
		CHAR deviceName[MAX_STR_LEN];
		UINT deviceIndex;
		// 2
		std::vector<resolutionType> ScreenResolution;
		// 3
		UINT width;
		UINT height;
		UINT refreshRate_Numerator;
		UINT refreshRate_Denominator;
		// 4
		HWND hWnd;
	};
}

extern LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

////////////////////////////////////////////////////////////////////////////////
// Class name: WinSystemClass
////////////////////////////////////////////////////////////////////////////////
class WinSystemClass : public SystemClass
{
public:
	WinSystemClass();
	WinSystemClass(WOMA::Settings* AppSettings);
	void WinSystemClass_init();
	~WinSystemClass();
	void Shutdown();

	bool InitializeSystem();
	int	 ApplicationMainLoop();

	void ProcessInput();
	bool InitOsInput();
	bool ApplicationInitMainWindow();
	bool MyRegisterClass(HINSTANCE hInstance);
	HWND WomaCreateWindowEx(DWORD dwExStyle, TCHAR* lpClassName, TCHAR* lpWindowName, DWORD dwStyle,
		int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	bool CreateMainWindow(UINT USE_MONITOR_, /*WomaDriverClass*/ void*, int&, int&, bool allowResize = false); /*wGLopenGLclass*/
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void ProcessFrame();

	// --------------------------------------------------------------
	void PAUSE();
	void UNPAUSE();
	void ONRESIZE();
	bool newDriver();
	//VARS:
	// --------------------------------------------------------------
	// SUBSYSTEM:WINDOWS
	PSTR	Scmdline;
	int		Cmdshow;
	DWORD	windowStyle;

	HBITMAP bmpBackGround;
	HWND	m_hWnd;
	HWND	statusbar;
	std::vector<WOMA::WindowDataContainer> windowsArray;
	std::vector<WOMA::WindowDataContainer> allWindowsArray;

	RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
	RECT              m_rcWindowClient;    // Saved client area size for mode switches

	DISPLAY_DEVICE displayDevice;
	ScreenArrayInfo info;
	MONITORINFOEX monitorArray[15];

public:
	// VARS:
	// --------------------------------------------------------------
	HINSTANCE m_hinstance;
	bool mResizing;

	// SCALE:
	float fontSizeX, fontSizeY;
	float m_scaleX, m_scaleY;
	TCHAR			pstrFPS[300];

	void refreshTitle();
	void StartTimer();

	void InitializeSetupScreen(int x, int y);

	WomaSetupManager* womaSetup;

	HBITMAP bmpWorldMap;
	HBITMAP bmpTarget;

private:
	void ShutdownWindows();
};

/////////////
// GLOBALS //
/////////////
extern WinSystemClass* SystemHandle;

