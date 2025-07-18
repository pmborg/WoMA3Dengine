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
//WomaIntegrityCheck = 1234525256;

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
		STRING MonitorName=TEXT("");
		CHAR deviceName[MAX_STR_LEN] = {};
		UINT deviceIndex=0;
		// 2
		std::vector<resolutionType> ScreenResolution = {};
		// 3
		UINT width=0;
		UINT height=0;
		UINT refreshRate_Numerator=0;
		UINT refreshRate_Denominator=0;
		// 4
		HWND hWnd=0;
	};
}

extern LRESULT CALLBACK WOMA_PAINT_MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

////////////////////////////////////////////////////////////////////////////////
// Class name: WinSystemClass
////////////////////////////////////////////////////////////////////////////////
class WinSystemClass : public SystemClass
{
public:
	UINT WomaIntegrityCheck = 1234525256;
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
	#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
    void GetInputs();
	bool InitOsInput();
	#endif
	bool APPLICATION_INIT_MAIN_WINDOW();
	bool MyRegisterClass(HINSTANCE hInstance);
	HWND WomaCreateWindowEx(DWORD dwExStyle, TCHAR* lpClassName, TCHAR* lpWindowName, DWORD dwStyle,
		int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	bool CreateWin32MainWindow(UINT USE_MONITOR_, void*, int&, int&); //wGLopenGLclass
	bool ShowWindow(int windowLeft, int windowTop);
	LRESULT CALLBACK WOMA_SYSTEM_MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void ProcessFrame();

	// --------------------------------------------------------------
	void PAUSE();
	void UNPAUSE();
	void ONRESIZE();

	//VARS:
	// --------------------------------------------------------------
	// SUBSYSTEM:WINDOWS
	PSTR	Scmdline=0;
	int		Cmdshow=0;
	DWORD	windowStyle=0;
	bool	bReremoteDesktop = false;

#if defined USE_STATUSBAR
	HWND	statusbar=0;
#endif

	std::vector<WOMA::WindowDataContainer> windowsArray;
	std::vector<WOMA::WindowDataContainer> allWindowsArray;

	RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
	RECT              m_rcWindowClient;    // Saved client area size for mode switches

	ScreenArrayInfo info;
	MONITORINFOEX monitorArray[MAX_WIN32_MONITORS];

public:
	// VARS:
	// --------------------------------------------------------------
	HINSTANCE m_hinstance;
	bool mResizing;

	void StartTimer();

#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
	WomaSetupManager* womaSetup;
#endif

#if CORE_ENGINE_LEVEL >= 8 && defined USE_GPS_MAP
	HBITMAP bmpWorldMap;
	HBITMAP bmpTarget;
#endif

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
