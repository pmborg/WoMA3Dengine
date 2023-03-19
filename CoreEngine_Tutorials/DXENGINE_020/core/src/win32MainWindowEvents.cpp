// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: win32MainWindowEvents.cpp
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
//
// --------------------------------------------------------------------------------------------
#pragma warning( disable : 4312 ) // warning C4312: 'type cast': conversion from 'int' to 'HMENU' of greater size
#include "platform.h"
#include "stateMachine.h"
#include "OSmain_dir.h"

#include "winsystemclass.h"

#include "xml_loader.h"

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

	DWORD windowStyle = ((SystemHandle->AppSettings->AllowResize) && (DX_ENGINE_LEVEL >= 20)) ?
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

	#define TIMER_TITLE 0

  // TIMERS:
	#define TIMER_ASTRO 1

// Start benchamark - TIMER / FPS / CPU Initialize: (Min. Req.: Windows Vista)
//	-------------------------------------------------------------------------------------------
void WinSystemClass::StartTimer()
{
	if (WOMA::game_state >= GAME_STOP)
		return;

	// Start Timer for Window Title
	#define KEYB_TIMES_PER_SECOND 1

	if ((!AppSettings->FULL_SCREEN) && (windowStyle != 0x96080000))			// Dont Update on: FullScreen or Full-windowed
		SetTimer(m_hWnd, TIMER_TITLE, 1000 / KEYB_TIMES_PER_SECOND, NULL);	// 1000ms = 1 second!

	SetTimer(m_hWnd, TIMER_ASTRO, 60 * 1000 / KEYB_TIMES_PER_SECOND, NULL);	// 1000ms = 1 second!
}

//----------------------------------------------------------------------------
LRESULT CALLBACK WinSystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
//----------------------------------------------------------------------------
{
	// Note: break;		--> Means call windows default handler also!
	// Note: return 0;	--> Means Done!

	bool bReremoteDesktop;
	static int wmId, wmEvent;

	switch (umsg)
	{
	case WM_COMMAND: // Process WoMA Start Button
		wmId = LOWORD(wparam);
		wmEvent = HIWORD(wparam);

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
						WOMA::game_state = GAME_MENU;

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
						saveConfigSettings(str);

						SystemHandle->AppSettings->DRIVER = previous_DRIVER;

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
		if (WOMA::game_state >= GAME_RUN  && WOMA::game_state < ENGINE_RESTART )
			PAUSE();
		break;

	// ----------------------------------------------------------------------------
	// WM_CLOSE -> WM_QUIT -> WM_DESTROY 
	// ----------------------------------------------------------------------------
	// Check if the window is being closed: (i.e.) 
	// MainWindow Close: in Task Bar OR Window [X] (top right corner), etc...

	case WM_CLOSE:	// During the shutdown process of the device, the WM_CLOSE message is broadcasted to the applications.
		{
			WOMA::game_state = GAME_STOP;
		}

		::PostMessage(hwnd, WM_QUIT, 0, 0);
		return 0;			

	case WM_QUIT:
		//if (audio) {}
		ASSERT(SystemHandle);
		WOMA::game_state = GAME_STOP;
		break;

	case WM_DESTROY:	// The main application Window will be destroyed
		KillTimer(hwnd, TIMER_TITLE);

		KillTimer(hwnd, TIMER_ASTRO);

		if ((m_contextDriver != driverList[DRIVER_DX9]) &&
			(m_contextDriver != driverList[DRIVER_DX11]) &&
			(m_contextDriver != driverList[DRIVER_DX12]))
			SAFE_DELETE(m_contextDriver);
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
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		SystemHandle->m_OsInput->KeyDown((unsigned int)wparam);
		return 0; //break;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		SystemHandle->m_OsInput->KeyUp((unsigned int)wparam);
		return 0; //break;
	}

	// -----------------------------------------------------------------------------
	// "ALT-ENTER" command: Toggle "g_FULL_SCREEN" Variable upon 
	// -----------------------------------------------------------------------------
	case WM_SYSKEYDOWN:
	{
		switch (wparam)
		{
			case VK_RETURN:					// key: ENTER
			{
				DWORD dwMask = (1 << 29);
				if ((lparam & dwMask) != 0) // key: ALT is down also
				{
					// Toggle the full screen/window mode
					SystemHandle->AppSettings->FULL_SCREEN = !SystemHandle->AppSettings->FULL_SCREEN;
					if (SystemHandle->AppSettings->FULL_SCREEN)
						ShowWindow(SystemHandle->statusbar, SW_HIDE);
					else
						ShowWindow(SystemHandle->statusbar, SW_SHOW);

					return 0;
				}
			}
		}

	break;
	}

	// -----------------------------------------------------------------------------
	// RE-SIZE: is sent when the user resizes the window.  
	// -----------------------------------------------------------------------------
	case WM_SIZE:
	{
		// Save the new Real/Client area dimensions (because Window Frame)
		if (SystemHandle)
		{
			// -[][X]
			if (wparam == SIZE_MINIMIZED) // -
			{
				mMaximized = false;
				WOMA::game_state = GAME_MINIMIZED;

			}
			else if (wparam == SIZE_MAXIMIZED)	// [] (go from default to maximize!)
			{
				mMaximized = true;
				if (WOMA::game_state == GAME_MINIMIZED)
					UNPAUSE();	//Restore State
				if (SystemHandle->statusbar)
					DestroyWindow(SystemHandle->statusbar);
				ONRESIZE();
			}
			else if (wparam == SIZE_RESTORED)	// Restore
			{
				// Restoring from minimized state?
				if (WOMA::game_state == GAME_MINIMIZED)
				{
					UNPAUSE();	//Restore State
					ONRESIZE();
				}

				// Restoring default, from maximized state?
				else if (mMaximized)
				{
					mMaximized = false;
					if (SystemHandle->statusbar)
						DestroyWindow(SystemHandle->statusbar);
					ONRESIZE();
				}
				else if (mResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.

					//mResizing = true;
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					//SystemHandle->AppSettings->WINDOW_WIDTH = LOWORD(lparam);	// New Usefull Size
					//SystemHandle->AppSettings->WINDOW_HEIGHT = HIWORD(lparam);	// New Usefull Size

					ONRESIZE();
				}

				#if defined _DEBUG
				if (SystemHandle->m_hWnd) {
					if (SystemHandle->statusbar)
						DestroyWindow(SystemHandle->statusbar);
					SystemHandle->statusbar = DoCreateStatusBar(SystemHandle->m_hWnd, 0/*idStatus*/, m_hinstance, 1/*cParts*/);
					SendMessage(SystemHandle->statusbar, SB_SETTEXT, 0, (LPARAM)DEMO_TITLE);
					if (AppSettings->FULL_SCREEN)
						ShowWindow(SystemHandle->statusbar, SW_HIDE);
				}
				#endif
			}
		}

		return 0;
	}
	#if defined USE_ASPECT_RATIO
	// Keep "Aspect Ratio" on Window Resize
	case WM_SIZING:
	{
		LPRECT r = LPRECT(lparam);

		switch (wparam)
		{
		case WMSZ_LEFT:
		case WMSZ_BOTTOMLEFT:
		case WMSZ_BOTTOMRIGHT:
		case WMSZ_RIGHT:
			r->bottom = r->top + (LONG)((float)(r->right - r->left) / SystemHandle->aspect_r);
			break;

		case WMSZ_TOPRIGHT:
		case WMSZ_TOP:
		case WMSZ_BOTTOM:
			r->right = r->left + (LONG)((float)(r->bottom - r->top)*SystemHandle->aspect_r);
			break;

		case WMSZ_TOPLEFT:r->left = r->right - (LONG)((float)(r->bottom - r->top)*SystemHandle->aspect_r);
			break;
		}
		return true;
	}
	#endif

	// -----------------------------------------------------------------------------
	// MIN-SIZE: Catch this message so to prevent the window from becoming too small.
	// -----------------------------------------------------------------------------

	// -----------------------------------------------------------------------------
	// MOVE: The mainwindow is being dragged...
	// -----------------------------------------------------------------------------
	case WM_MOVE:
	{
		SystemHandle->AppSettings->WINDOW_Xpos = (int)(short)LOWORD(lparam); //NOTE: (int)(short) --> Allow to receive negative numbers
		SystemHandle->AppSettings->WINDOW_Ypos = (int)(short)HIWORD(lparam);
		break;
	}
	// WM_EXITSIZEMOVE is sent when the user "drag" to resize window:
	case WM_ENTERSIZEMOVE:
	{
		mResizing = true;
		PAUSE();

		return 0;
	}

	// WM_EXITSIZEMOVE is sent when the user "releases" the resize bars:
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
	{
		UNPAUSE();		// Restore State: "Green" Light to Render Again (after: return 0)
		if (mResizing)
			ONRESIZE(); // Do the Window, "Buffers" & Textures Re-size

		mResizing = false;
		return 0;
	}


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
		switch (wparam)
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
		if ((wparam & 0xFFF0) == SC_SCREENSAVE || (wparam & 0xFFF0) == SC_MONITORPOWER)
			return 0;
	}


	// -----------------------------------------------------------------------------
	// TIMERS:
	// -----------------------------------------------------------------------------
	case WM_TIMER: // Once per second:
		if (WOMA::game_state < GAME_STOP)
		{
			switch (wparam)
			{
			case TIMER_TITLE:
				if (!SystemHandle->AppSettings->FULL_SCREEN)
					SystemHandle->refreshTitle();
				return 0;

			case TIMER_ASTRO:
				if (SystemHandle->m_Application) //SAFER: In shutdown/exception timer might be running...
				{
					SystemHandle->m_Application->initWorld->Frame(); // Recalculate astros every minute...
					#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION //#if ENGINE_LEVEL >= 33
						SystemHandle->m_Application->Calc3DSunMoonPosition();
					#endif
				}
				return 0;
			}
		}

	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

