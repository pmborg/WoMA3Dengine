// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// Filename: win32MainWindowEvents.cpp
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
//WomaIntegrityCheck = 1234567311;

#pragma warning( disable : 4312 ) // warning C4312: 'type cast': conversion from 'int' to 'HMENU' of greater size
#include "stateMachine.h"
#include "OSmain_dir.h"

#if defined WINDOWS_PLATFORM
#include "OSengine.h"

#include "xml_loader.h"

#if CORE_ENGINE_LEVEL >= 2 && defined USE_STATUSBAR
#include "dxWinSystemClass.h"

#if defined USE_INTRO_VIDEO_DEMO
extern void CALLBACK OnGraphEvent(HWND hwnd, long evCode, LONG_PTR param1, LONG_PTR param2);
#endif

#pragma comment( lib, "comctl32.lib" )

// Description: 
//   Creates a status bar and divides it into the specified number of parts.
// Parameters:
//   hwndParent - parent window for the status bar.
//   idStatus - child window identifier of the status bar.
//   hinst - handle to the application instance.
//   cParts - number of parts into which to divide the status bar.
// Returns:
//   The handle to the status bar.
//
int statusBarHeigth = 0;

HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts)
{
	HWND hwndStatus;
	RECT rcClient;
	HLOCAL hloc;
	PINT paParts;
	int i, nWidth;

	// Ensure that the common control DLL is loaded.
	InitCommonControls();

	DWORD windowStyle = ((SystemHandle->AppSettings->AllowResize) && (LEVEL >= 20)) ?
		SBARS_SIZEGRIP |        // includes a sizing grip
		WS_CHILD | WS_VISIBLE	// creates a visible child window
		:
		WS_CHILD | WS_VISIBLE   // creates a visible child window		
		;

	// Create the status bar.
	hwndStatus = CreateWindowEx(
		0,                       // no extended styles
		STATUSCLASSNAME,         // name of status bar class
		(PCTSTR)NULL,           // no text when first created
		windowStyle,			// creates a visible child window
		0, 0, 0, 0,              // ignores size and position
		hwndParent,              // handle to parent window
		(HMENU)idStatus,       // child window identifier
		hinst,                   // handle to application instance
		NULL);                   // no window creation data

	// Get the coordinates of the parent window's client area.
	GetClientRect(hwndParent, &rcClient);

	// Allocate an array for holding the right edge coordinates.
	hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	paParts = (PINT)LocalLock(hloc);

	// Calculate the right edge coordinate for each part, and
	// copy the coordinates to the array.
	nWidth = rcClient.right / cParts;
	int rightEdge = nWidth;
	for (i = 0; i < cParts; i++) {
		paParts[i] = rightEdge;
		rightEdge += nWidth;
	}

	// Tell the status bar to create the window parts.
	SendMessage(hwndStatus, SB_SETPARTS, (WPARAM)cParts, (LPARAM)paParts);

	// Free the array, and return.
	LocalUnlock(hloc);
	LocalFree(hloc);

	RECT rect;
	statusBarHeigth = (hwndStatus && GetWindowRect(hwndStatus, &rect)) ? (rect.bottom - rect.top) : 0;

	return hwndStatus;
}
#endif

	#define TIMER_TITLE 0

  // TIMERS:

// Start benchamark - TIMER / FPS / CPU Initialize: (Min. Req.: Windows Vista)
//	-------------------------------------------------------------------------------------------
void WinSystemClass::StartTimer()
{
	if (WOMA::game_state >= GAME_STOP)
		return;

	// Start Timer for Window Title
	#define KEYB_TIMES_PER_SECOND 1

	// Dont Update on: FullScreen or Full-windowed
	if ((!AppSettings->FULL_SCREEN) && (windowStyle != 0x96080000)) 
	{
		SetTimer(m_hWnd, TIMER_TITLE, 2000 / KEYB_TIMES_PER_SECOND, NULL);	// 2000ms = 2 seconds! (1000ms = 1 second!)
		
	}

}

//extern void ImGuiShutdown();
//extern bool ImGuiDONE;  // Main loop

#if CORE_ENGINE_LEVEL >= 2 && defined WINDOWS_PLATFORM	
//----------------------------------------------------------------------------
LRESULT CALLBACK WinSystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lparam)
//----------------------------------------------------------------------------
{
	// Note: break;		--> Means call windows default handler also!
	// Note: return 0;	--> Means Done!

	int wmId=0, wmEvent = 0;

	switch (umsg)
	{
	case WM_COMMAND: // Process WoMA Start Button
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
			case CBN_SELCHANGE:
			{
				if (womaSetup) 
				{
					//BUTTON: OK
					if (HWND(lparam) == womaSetup->m_hBtnOK)
					{
						//WOMA::game_state = GAME_SETUP; // GAME_MENU;

						//GET FULL_SCREEN COMBOBOX: "Display Mode"
						int index = (int)SendMessage(womaSetup->hWndComboBox[0], CB_GETCURSEL, NULL, NULL);
						if (index == 0)
							SystemHandle->AppSettings->FULL_SCREEN = false;
						if (index == 1)
							SystemHandle->AppSettings->FULL_SCREEN = true;

						//GET VSYNC CheckBox:
						SystemHandle->AppSettings->VSYNC_ENABLED = (SendMessage(womaSetup->hWndCheckBox[0], BM_GETCHECK, 0, 0) == BST_CHECKED);

						//GET UI_MONITOR COMBOBOX:
						SystemHandle->AppSettings->UI_MONITOR = (int)(SendMessage(womaSetup->hWndComboBox[1], CB_GETCURSEL, NULL, NULL));

						//GET UI_MONITOR COMBOBOX:
						int		previous_DRIVER = SystemHandle->AppSettings->DRIVER;
						SystemHandle->AppSettings->DRIVER = (int)(SendMessage(womaSetup->hWndComboBox[7], CB_GETCURSEL, NULL, NULL));
						CHAR str[MAX_STR_LEN] = { 0 }; wtoa(str, (TCHAR*)SystemHandle->XML_SETTINGS_FILE.c_str(), MAX_STR_LEN); // wchar ==> char
						SystemHandle->xml_loader.saveConfigSettings(str);
						SystemHandle->AppSettings->DRIVER = previous_DRIVER;

						WOMA::game_state = GAME_SETUP;
						WOMA::previous_game_state = WOMA::game_state;
						WOMA::game_state = ENGINE_RESTART;
					}

					//BUTTON: Cancel
					if (HWND(lparam) == womaSetup->m_hBtnCancel)
					{
						WOMA::game_state = GAME_RUN;
						SAFE_SHUTDOWN(womaSetup);
					}
				}
			}
		}
		break;

	case WM_SETFOCUS:
		if (WOMA::game_state == GAME_PAUSED)
			UNPAUSE();
		break;

	case WM_KILLFOCUS:
	#if !defined _DEBUG
		if (WOMA::game_state >= GAME_RUN  && WOMA::game_state < ENGINE_RESTART )
			PAUSE();
	#endif
		break;

	// ----------------------------------------------------------------------------
	// WM_CLOSE -> WM_QUIT -> WM_DESTROY 
	// ----------------------------------------------------------------------------
	// Check if the window is being closed: (i.e.) 
	// MainWindow Close: in Task Bar OR Window [X] (top right corner), etc...
/*
	case WM_CLOSE:
		//KillTimer(hwnd, TIMER_TITLE);
		WOMA::main_loop_state = -1; 
		//::PostMessage(hwnd, WM_QUIT, 0, 0);
		break;// return 0;	// Cancel Close, do nothing.
*/

	case WM_CLOSE:	// During the shutdown process of the device, the WM_CLOSE message is broadcasted to the applications.

		WOMA::main_loop_state = -1;
		WOMA::game_state = GAME_STOP;
		//::PostMessage(hwnd, WM_QUIT, 0, 0);
		break;


	case WM_QUIT:
		ASSERT(SystemHandle);
		ASSERT(WOMA::game_state == GAME_STOP);
		break;

	case WM_DESTROY:	// The main application Window will be destroyed
		//PostQuitMessage(0); //AQUI

		return 0;

#if defined USE_INTRO_VIDEO_DEMO
	case WM_GRAPH_EVENT:
		if (SystemHandle->g_DShowPlayer) // NOTE: And error in codecs may lead on a g_DShowPlayer = NULL
			SystemHandle->g_DShowPlayer->HandleGraphEvent(OnGraphEvent);
		return 0;
#endif

	//----------------------------------------------------------------------------
	// With Direct Input (USE_DIRECT_INPUT) This Messages are not invoked anymore:
	//----------------------------------------------------------------------------

	// -----------------------------------------------------------------------------
	// REMOTE-DESKTOP: Update flag for "Remote" desktop connect/disconnect
	// -----------------------------------------------------------------------------
	case WM_WTSSESSION_CHANGE:
	{
		bReremoteDesktop = (GetSystemMetrics(SM_REMOTESESSION) != 0);
		break;
	}

	// -----------------------------------------------------------------------------
	// POWER: Handle Suspend Mode (Enter and Recover)
	// -----------------------------------------------------------------------------
	case WM_POWERBROADCAST:
	{
		switch (wParam)
		{
			// Reject Querys do Suspend or Standby:
		case PBT_APMQUERYSUSPEND:
		case PBT_APMQUERYSTANDBY:
			return BROADCAST_QUERY_DENY;

			// System is "Suspending" operation.
		case PBT_APMSUSPEND:
			// At this point, the app should save any data for open
			// network connections, files, etc., and prepare to go into
			// a suspended mode.
			// ...
			return TRUE;

			// Operation is "Resuming". This message is sent after PBT_APMRESUMEAUTOMATIC 
			// if the resume is triggered by user input, such as pressing a key.
		case PBT_APMRESUMESUSPEND:
			// At this point, the app should recover any data, network
			// connections, files, etc., and resume running from when
			// the app was suspended.
			// ...
			return TRUE;
		}

		break;
	}

	// POWER: Prevent a powersave mode of monitor or the screensaver
	case WM_SYSCOMMAND:
	{
		if ((wParam & 0xFFF0) == SC_SCREENSAVE || (wParam & 0xFFF0) == SC_MONITORPOWER)
			return 0;
	}


	// -----------------------------------------------------------------------------
	// TIMERS:
	// -----------------------------------------------------------------------------
	case WM_TIMER: // Once per second:
		if (WOMA::game_state < GAME_STOP)
		{
			switch (wParam)
			{
			case TIMER_TITLE:
				if (!SystemHandle->AppSettings->FULL_SCREEN)
					SystemHandle->refreshTitle();
				return 0;

			}
		}

	}

	return DefWindowProc(hwnd, umsg, wParam, lparam);
}
#endif

#endif
