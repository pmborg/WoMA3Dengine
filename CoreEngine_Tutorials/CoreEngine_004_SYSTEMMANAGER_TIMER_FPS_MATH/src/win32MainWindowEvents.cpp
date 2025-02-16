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
//WomaIntegrityCheck = 1234567142;

#pragma warning( disable : 4312 ) // warning C4312: 'type cast': conversion from 'int' to 'HMENU' of greater size
#include "stateMachine.h"
#include "OSmain_dir.h"

#if defined WINDOWS_PLATFORM
#include "winsystemclass.h"

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

#if defined USE_DIRECT_INPUT
		// SOURCE: http://stackoverflow.com/questions/21799069/mouse-wheel-only-can-scroll-either-up-or-down
	case WM_MOUSEWHEEL:
		SystemHandle->m_InputManager.mouseWheelIn(wParam);
		return 0;
#endif

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
#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
  #if !defined USE_DIRECT_INPUT	//USE OS INPUT:
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		SystemHandle->m_OsInput->KeyDown((unsigned int)wParam);
		return 0; //break;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		SystemHandle->m_OsInput->KeyUp((unsigned int)wParam);
		return 0; //break;
	}
  #elif defined USE_DIRECT_INPUT	//USE DX INPUT:

	// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			if (SystemHandle->m_OsInput)
				SystemHandle->m_OsInput->KeyDown((UINT)lparam, (UINT)wParam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		if (SystemHandle->m_OsInput)
			SystemHandle->m_OsInput->KeyUp((UINT)lparam, (UINT)wParam);
		return 0;
	}

	// ON Activate windows: Activate our Direct Keyborad System
	// -----------------------------------------------------------------------------
	case WM_ACTIVATEAPP:
	{
		if (!(int)wParam == 0)
			if (SystemHandle)
				SystemHandle->m_InputManager.Initialize(SystemHandle->m_hinstance);	//hWnd and hInst are just the global

		return 0;
	}
  #endif
#endif

#if defined USE_ALLOW_MAINWINDOW_RESIZE
	// -----------------------------------------------------------------------------
	// RE-SIZE: is sent when the user resizes the window.  
	// -----------------------------------------------------------------------------

	case WM_SIZE:
	{
		// Use windows settings!
		// Save the new client area dimensions.
		//g_ScreenWidth = LOWORD(lparam);
		//g_ScreenHeight = HIWORD(lparam);

		// Save the new Real/Client area dimensions (because Window Frame)
		if (SystemHandle)
		{
			// -[][X]
			if (wParam == SIZE_MINIMIZED) // -
			{
				mMaximized = false;
				WOMA::game_state = GAME_MINIMIZED;

			}
			else if (wParam == SIZE_MAXIMIZED)	// [] (go from default to maximize!)
			{
				SystemHandle->AppSettings->WINDOW_WIDTH = LOWORD(lparam);	// New Usefull Size
				SystemHandle->AppSettings->WINDOW_HEIGHT = HIWORD(lparam);	// New Usefull Size
				mMaximized = true;
				if (WOMA::game_state == GAME_MINIMIZED)
					UNPAUSE();	//Restore State
			#if defined USE_STATUSBAR
				if (SystemHandle->statusbar)
					DestroyWindow(SystemHandle->statusbar);
			#endif
				if (SystemHandle->m_hWnd) 
					{ ONRESIZE(); }
			}
			else if (wParam == SIZE_RESTORED)	// Restore
			{
				// Restoring from minimized state?
				if (WOMA::game_state == GAME_MINIMIZED)
				{
					UNPAUSE();	//Restore State
					if (SystemHandle->m_hWnd) 
						{ ONRESIZE(); }
				}

				// Restoring default, from maximized state?
				else if (mMaximized)
				{
					mMaximized = false;
				#if defined USE_STATUSBAR
					if (SystemHandle->statusbar)
						DestroyWindow(SystemHandle->statusbar);
				#endif
					if (SystemHandle->m_hWnd) 
						{ ONRESIZE(); }
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
					SystemHandle->AppSettings->WINDOW_WIDTH = LOWORD(lparam);	// New Usefull Size
					SystemHandle->AppSettings->WINDOW_HEIGHT = HIWORD(lparam);	// New Usefull Size
					if (SystemHandle->m_hWnd) 
						{ ONRESIZE(); }
				}

				#if defined USE_STATUSBAR //#if defined _DEBUG
					if (SystemHandle->m_hWnd) {
						if (SystemHandle->statusbar)
							DestroyWindow(SystemHandle->statusbar);
						//DoCreateStatusBar(SystemHandle->m_hWnd, 0 idStatus, m_hinstance, 1 cParts);
						SystemHandle->statusbar = DoCreateStatusBar(SystemHandle->m_hWnd, 0, m_hinstance, 1);
						SendMessage(SystemHandle->statusbar, SB_SETTEXT, 0, (LPARAM)DEMO_TITLE);
						if (AppSettings->FULL_SCREEN)
							::ShowWindow(SystemHandle->statusbar, SW_HIDE);
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
			if (SystemHandle->m_hWnd) 
				{ ONRESIZE(); } // Do the Window, "Buffers" & Textures Re-size

		mResizing = false;
		return 0;
	}

#endif

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
