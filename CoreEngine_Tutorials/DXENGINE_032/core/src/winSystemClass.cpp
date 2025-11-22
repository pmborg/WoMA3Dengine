// --------------------------------------------------------------------------------------------
// Filename: winSystemClass.cpp
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
// PURPOSE: Define APIs for winSystemClass.cpp which is the WINDOWS OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#include "standard_platform.h"
#include "OSengine.h"
#include "mem_leak.h"
#if defined DX_ENGINE
#include "DXengine.h"
#endif

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"

#include "OSmain_dir.h"
#include "language.h"

#include "woma_exception.h"
#include "fileLoader.h"

#include "packManager.h"
#include "idea.h"

#include "dxWinSystemClass.h"
#if (defined OPENGL3 || defined OPENGL40) 
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif
#include "DX11Class.h"
#include "DXmodelClass.h"

#include "SystemClass.h"

/////////////////////
// Windows GLOBALS //
/////////////////////

TCHAR MainDeviceName[MAX_STR_LEN];	// Monitor Name

//----------------------------------------------------------------------------------
WinSystemClass::WinSystemClass() : SystemClass() 
//----------------------------------------------------------------------------------
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

	//public:
	SystemHandle = this;
	WinSystemClass_init();
}
//----------------------------------------------------------------------------------
WinSystemClass::WinSystemClass(WOMA::Settings* appSettings): SystemClass() //	SystemClass::SystemClass() Will Run!
//----------------------------------------------------------------------------------
{
	CLASSLOADER();

	//public:
	WOMA::AppSettings		= appSettings; // Super
	SystemHandle	= this;
	WinSystemClass_init();
}

//----------------------------------------------------------------------------
void WinSystemClass::ProcessFrame()
//----------------------------------------------------------------------------
{
	static void* mainCtx=NULL;
	if (m_Driver->RenderfirstTime)
		mainCtx = getvoidcontext();

	SystemClass::FrameUpdate();	// Process: (Fx keys |ESC and F1 to F6| & get FPS)

	if (WOMA::game_state == ENGINE_RESTART)
		return; //Restart? so, don't render, restart now.

	// Render Setup?
#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
	if (WOMA::game_state == GAME_SETUP)
	{
        return; //Process win32 Setup/pages so, don't render!
	}
#endif

	{
        m_Application->dayLightFade = m_Application->ProcessMovementInput_and_UpdateDemos();	//CalculateViewMatrix (for Sky) and check collision(s):part I

		#if defined INTRO_DEMO
		if (RENDER_PAGE < 15)
		#else
		if (RENDER_PAGE < 10)
		#endif
			return; // We are in first win32 Demo pages? So, don't render yet!

		// For each Monitor: Render one Application Frame
		//0–27	Primary monitor only Ignore extra outputs; no multi-monitor logic needed
		//28+	Full multi-monitor	Use adapter/output detection and per-monitor SWAPCHAINS
        int num_monitors = (int)windowsArray.size();
		for (int monIdx = 0; monIdx < num_monitors; monIdx++)
		{
			m_Driver->BeginScene(monIdx);										// RESET FRAME: ClearRenderTargetView + ClearDepthBuffer
        
			CalculateCameraViewAndFrustum(mainCtx);								// CALCULATE: CalculateViewMatrix (to render) and Frustum
			IF_RENDER_PAGE(RENDER_PAGE >= 30)
				m_Application->SortOutWhatNeedToBeRendered(mainCtx, m_Driver);	// 30+: SORT OBJECTS: Sort out what need to be rendered (filter only objects in front of camera)

			m_Application->RenderScene(mainCtx, monIdx, m_Driver);				// RENDER 1 FRAME!
                                                               
			(g_contextDriver ? g_contextDriver : m_Driver)->EndScene(monIdx);	// SHOW FRAME :)
		}
       
		m_Driver->RenderfirstTime = false;										// 1st frame rendered.
	}
}

void WinSystemClass::WinSystemClass_init()
{
// --------------------------------------------------------------
// Init Vars:
// --------------------------------------------------------------

#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
	m_OsInput = NULL;
#endif
	windowStyle = NULL;
	if (WOMA::AppSettings)
		mMaximized = WOMA::AppSettings->FULL_SCREEN;

	WOMA::game_state = WOMA::previous_game_state;
	m_hWnd = NULL;
#if defined USE_STATUSBAR
	statusbar = NULL;
#endif
#if defined USE_ASPECT_RATIO
	aspect_r = NULL;		//Global Used by:WndProc
#endif

	memset(&displayDevice, 0, sizeof(displayDevice));

#if defined USE_TIMER_CLASS
#if !defined WIN_XP
	m_Timer.Initialize();
#endif
	m_Fps.Initialize();		// Start FPS Counting, Using: timeGetTime();
#endif

#if !defined WIN_XP
	m_Cpu.Initialize();		// WINDOWS_PLATFORM: Initialize CPU Usage Counter
#endif

#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
	womaSetup = NULL;
#endif

#if CORE_ENGINE_LEVEL >= 8 && defined USE_GPS_MAP
	bmpWorldMap = NULL;
	bmpTarget = NULL;
#endif

#if CORE_ENGINE_LEVEL >= 2 && defined WINDOWS_PLATFORM
	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);
	ASSERT(m_hinstance);
#endif
}

WinSystemClass::~WinSystemClass()
{

	Shutdown();
	CLASSDELETE();
	SystemHandle = NULL;
}

bool WinSystemClass::APPLICATION_BEFORE_WINDOW()
{
	womalog("WinSystemClass::APPLICATION_BEFORE_WINDOW()\n");

	return true;
}

bool WinSystemClass::APPLICATION_AFTER_WINDOW()
{
    womalog("WinSystemClass::APPLICATION_CORE_INIT_DONE()\n");

    return true;
}

bool WinSystemClass::APPLICATION_INIT_SYSTEM()
//----------------------------------------------------------------------------
{

	// ################################# SYSTEM CHECK + CREATE WINDOW #################################
#if defined USE_SYSTEM_CHECK                                // BEFORE: APPLICATION_INIT_MAIN_WINDOW()
	IF_NOT_RETURN_FALSE(SystemClass::SystemCheck());		// SYSTEM INFO: HW (OS, CPU, RAM, DiskFreeSpace, CPUFeatures) 
#endif
	IF_NOT_RETURN_FALSE(APPLICATION_INIT_MAIN_WINDOW());	// RegisterClass and Create: MainWindow(s)
#if defined USE_TINYXML_LOADER && DX_ENGINE_LEVEL >= 21
	IF_NOT_RETURN_FALSE(LoadXmlWorld());					// Load all static and semi-static from WORLD.XML and add Instances!
#endif
#if defined USE_PROCESS_OS_KEYS
	IF_NOT_RETURN_FALSE(InitOsInput());						// INIT-INPUT Devices, NOTE: AFTER: APPLICATION_INIT_MAIN_WINDOW()
#endif

// ########################################### LOAD DRIVERS ###########################################
	#if CORE_ENGINE_LEVEL >= 10 && (defined OPENGL3 || defined OPENGL40)
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
		IF_NOT_RETURN_FALSE(newDriver());	//Create NEW CONTEXT Class: g_contextDriver
	#endif
	LoadAllDrivers();		        //NEW DirectX::DX11Class()	(NEW DX9, NEW DX11, NEW DX12, NEW OpenGL): push_back(NEW DirectX::*Class());
 // ######################################### INIT SELECTED DRIVER ###################################
	if (!InitSelectedDriver())	// "driver"->OnInit (...)
		return false;			// "driver"->Initialize (clearColor)

#if defined USE_SYSTEM_CHECK
	InitializeSystemScreenF1(10, 10); // SETUP SCREENs: F1,F2,F3,F4,F5,F6 (RUNNING NOW ON: PaintSetup())
#endif

  #if defined USE_SCENE_MANAGER				// INIT SCENE-MANAGER:
	DXsystemHandle->InitSceneManager();		// INIT QUAD TREE
  #endif

#if defined USE_INTRO_VIDEO_DEMO // WINDOWS START-VIDEO: Start DEMO INTRO (MP4): (Give Time to Unpack/Load Resources)
    DXsystemHandle->g_DShowPlayer = NEW DShowPlayer(m_hWnd);	//INTRO MOVIE: mpg player
    IF_FAILED_RETURN_FALSE(DXsystemHandle->PlayIntroMovie(WOMA::LoadFile(VIDEO_INTRO)));	// VIDEO DEMO
#endif

	//################################ LOAD LoadAllGraphicAssets STUFF ##################################
	void* pContext = getvoidcontext();
	IF_NOT_RETURN_FALSE(SystemClass::LoadAllGraphicAssets(pContext));			// Call: m_Application->Initialize(...)

	//---------------------------------------------------------------------------------------------------
	if (WOMA::game_state >= GAME_STOP)	// Something FATAL on loading for "mandatory 2D/3D Stuff"?
		return false;					// (For example: 3D/IMAGE/AUDIO file missing!?)

	// WAIT FOR THE END OF THE VIDEO:
	#if defined USE_INTRO_VIDEO_DEMO
	{
		MSG msg = { };
		while (DXsystemHandle->g_DShowPlayer && DXsystemHandle->g_DShowPlayer->m_state != STATE_NO_GRAPH && (DXsystemHandle->g_DShowPlayer->m_state != STATE_STOPPED && DXsystemHandle->g_DShowPlayer->m_state != STATE_PAUSED))
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Process OS Messages:
			{	
				TranslateMessage(&msg);
				DispatchMessage(&msg);
	        
				// Make Sure that we have acquired the FOCUS and INPUT:
				if (DXsystemHandle->m_Input->m_mouse && DXsystemHandle->m_Input->m_keyboard)				
				{
					IF_NOT_THROW_EXCEPTION(DXsystemHandle->m_Input->GetMouseKeyboardState());
				}
				else
					DXsystemHandle->m_Input->Initialize(SystemHandle->m_hinstance);
	
				if (SystemHandle->m_player[g_NetID]->p_player.IsEscapePressed) // End Video, when ESC key is pressed:
				{
					DXsystemHandle->g_DShowPlayer->Stop();
					break;
				}

				Sleep(1); //Give CPU to time to do loader threads.
			}
		}

		SAFE_DELETE(DXsystemHandle->g_DShowPlayer); //Shutdown VIDEO PLAYER:
	}
	#endif

		StartTimer();	// START WINDOWS TIMER: ("Window Title" refresh & Real-Time Weather refresh)

#if !defined USE_LOADING_THREADS
	if (WOMA::game_state == GAME_LOADING)
		WOMA::game_state = GAME_RUN;	// Let it run!
#endif

	if (WOMA::game_state == ENGINE_RESTART)
		return false;

	return true; // GREEN LIGHT: To Start Rendering! :)
}

#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
//----------------------------------------------------------------------------------------------------------
void WinSystemClass::GetInputs()
{
#if defined USE_DIRECT_INPUT						// Read the User Input
	if (DXsystemHandle->m_Input->m_mouse && DXsystemHandle->m_Input->m_keyboard)	// Make Sure that we have acquired the FOCUS and INPUT:
	{
		ASSERT(DXsystemHandle->m_Input->GetMouseKeyboardState()); // Update "Keyboard State": Process the changes in the Mouse and Keyboard.
	}
	else
		DXsystemHandle->m_Input->Initialize(SystemHandle->m_hinstance); //re-gain input if necessary.

	#if defined USE_JOY && defined USE_DIRECT_INPUT
	if (joyFlags)
		SystemHandle->joyStickFrame();		// Update "JOY State"
	#endif
#endif
}
#endif

//----------------------------------------------------------------------------
int WinSystemClass::APPLICATION_MAIN_LOOP()		// [RUN] - MAIN "INFINITE" LOOP!
//----------------------------------------------------------------------------
{
	MSG msg = { 0 };							// Reset msg

	return S_OK;
}

void WinSystemClass::Shutdown()
//----------------------------------------------------------------------------------
{
	// WinSystemClass Shutdown:
	womalogauto ((TCHAR*)TEXT("WinSystemClass::Shutdown()\n"));

	// Destroy Drivers:
	SystemClass::Shutdown();
	
#if defined CLIENT_SCENE_SETUP
	SAFE_SHUTDOWN(womaSetup);
#endif
	
	SAFE_DELETE(g_contextDriver);

#if CORE_ENGINE_LEVEL >= 8 && defined USE_GPS_MAP //8
	DeleteObject(bmpWorldMap);
	DeleteObject(bmpTarget);
#endif

	ShutdownWindows();				// Shutdown the Main Window.
}


void WinSystemClass::ShutdownWindows()
//----------------------------------------------------------------------------
{
	// Show the mouse cursor.
	ShowCursor(true);

	// For each Monitor:
	for (int i = 0; i < windowsArray.size(); ++i)
	{
		// Close & Destroy: this window
		DestroyWindow(windowsArray[i].hWnd);
		windowsArray[i].hWnd = NULL;
	}

	// Remove the application instance.
	if (!UnregisterClass(WOMA_ENGINE_CLASS, m_hinstance))
	{
		int errcode = GetLastError();
		TCHAR   lpBuffer[256] = _T("?");
		if (errcode != 0)									// Don't want to see a "operation done successfully" error ;-)
			::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,		// It's a system error
				NULL,										// No string to be formatted needed
				errcode,									// Hey Windows: Please explain this error!
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// Do it in the standard language
				lpBuffer,									// OUT: Put the message here
				sizeof(lpBuffer) - 1,						// OUT: Number of bytes to store the message
				NULL);
		#if false_for_now
		if (Command != ENGINE_RESTART)
			WomaMessageBox(lpBuffer, (TCHAR*)TEXT("Error: UnregisterClass"), false);
		#endif
	}

	m_hinstance = NULL;

	// Fix the display settings, if leaving from "fullscreen" mode.
	if (WOMA::AppSettings) {
		if (WOMA::AppSettings->FULL_SCREEN) {
			ChangeDisplaySettingsEx(MainDeviceName, NULL, NULL, 0, NULL);	//ChangeDisplaySettings(NULL, 0); // Used only for 1 Monitor applications
		}
	}
}

namespace WOMA
{
	extern TCHAR* getCurrentDir();
}

// --------------------------------------------------------------------------------------------
// [*] Register the Window Class.
// --------------------------------------------------------------------------------------------
bool WinSystemClass::WomaRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpszClassName = WOMA_ENGINE_CLASS;

	// ALLOW WIN32 SYSTEM PAINT: (Causes the entire window to redraw if a movement or a size adjustment changes the height of the client area: CS_HREDRAW | CS_VREDRAW)
	wcex.style = (WOMA::AppSettings->DRIVER == DRIVER_GL3) ? CS_OWNDC : CS_HREDRAW | CS_VREDRAW; // NOTE: CS_OWNDC is need by OPEN GL: https://www.opengl.org/wiki/Platform_specifics:_Windows
	wcex.style |= CS_DBLCLKS;
	wcex.lpfnWndProc = static_cast<WNDPROC>(WOMA_PAINT_Message_event_handler);
	wcex.hInstance = hInstance;

	//
	// To Use External Icon: "*.png" -> "*.ico" Converter: http://converticon.com/
	// NOTE: I am Avoiding to use Resource here: //wcex.hIcon = wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//
	// More info WNDCLASSEX: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577%28v=vs.85%29.aspx
	//
	//IF_NOT_RETURN_FALSE(WOMA::getCurrentDir());
	bool gotIconFile = true;
	if (!WOMA::fileExists(WOMA::APP_ICO))
	{
		STRING err = TEXT("File not found: "); err += WOMA::APP_ICO;
		//NOTE: DONT USE WomaMessageBox here! 
		MessageBox(NULL, err.c_str(), TEXT("WARNING: MyRegisterClass()"), MB_ICONWARNING);
		gotIconFile = false;
	}

	HICON bigicon = NULL, smallicon = NULL;
	if (gotIconFile) {
		bigicon = (HICON)LoadImage(hInstance, WOMA::APP_ICO, IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
		smallicon = (HICON)LoadImage(hInstance, WOMA::APP_ICO, IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
		wcex.hIcon = bigicon;
		wcex.hIconSm = smallicon;
	}

	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //IDC_CROSS

	wcex.hbrBackground = nullptr;
	//wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//TO USE THIS COLOR: BLACK

	IF_NOT_RETURN_FALSE (RegisterClassEx(&wcex));

	if (bigicon)   DestroyIcon(bigicon);
	if (smallicon) DestroyIcon(smallicon);

	return true;
}

HWND WinSystemClass::WomaCreateWindowEx(DWORD dwExStyle, TCHAR* lpClassName, TCHAR* lpWindowName, DWORD dwStyle, int X, int Y, 
										int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) 
{
	//MORE Win32 Error Codes: http://msdn.microsoft.com/en-us/library/cc231199.aspx
	HWND hwnd = CreateWindowEx(	dwExStyle, lpClassName, lpWindowName, dwStyle, 
								X, Y, nWidth, nHeight, 
								hWndParent,	// We have no parent window, NULL.
								hMenu,		// We aren't using menus, NULL.
								hInstance, 
								lpParam);	// We aren't using multiple windows, NULL.
	if (!hwnd) 
	{
		// Display the error message (in default OS language)
		DWORD error = GetLastError();

		LPTSTR lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		LPVOID lpDisplayBuf = (LPVOID)LocalAlloc(	LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + 40) * sizeof(TCHAR)); 
		if (lpDisplayBuf)
		{
			StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("CreateWindowEx Error (%d): %s"), error, lpMsgBuf);
			MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Create WindowEx Parameters Error!"), MB_OK);
		}
	}

	return hwnd;
}

#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
bool WinSystemClass::InitOsInput()
//----------------------------------------------------------------------------
{
	SystemClass::InitOsInput();	//m_Application->SetPlayerPosition(g_NetID);

	// INIT OS Keyboard (WIN32: This object will be used to handle reading the input from the user)
	womalog("===============================================================================\n");
	womalog("INIT OS BASIC INPUT\n");
	womalog("===============================================================================\n");

	m_OsInput = NEW InputClass;
	IF_NOT_THROW_EXCEPTION(m_OsInput);
	m_OsInput->Initialize();	//reset: m_keys[i]

#if defined USE_DIRECT_INPUT
	// Set the Player Position Init Player Class
	womalog("===============================================================================\n");
	womalog("INIT OS ADVANCED DIRECT INPUT\n");
	womalog("===============================================================================\n");

	DXsystemHandle->m_Input = (DXInputClass*)&SystemHandle->m_InputManager;
#endif

	return true;
}
#endif

bool WinSystemClass::CreateWin32MainWindow(	UINT MONITOR_NUM, /*WomaDriverClass*/ void* driver, int& width, int& height)
//----------------------------------------------------------------------------
{
	womalogauto((TCHAR*)TEXT("---------------------------------\n"));
	womalogauto((TCHAR*)TEXT("WinSystemClass::CreateMainWindow()\n"));
	womalogauto((TCHAR*)TEXT("---------------------------------\n"));

	// --------------------------------------------------------------------------------------------
	// PURPOSE: Check all Monitors available
	// --------------------------------------------------------------------------------------------
	DEVMODE devMode = { 0 };
	DWORD deviceNum = 0;					// How Many Monitors we have?
	DWORD Current_Screen_WIDTH = 0;			// NOTE: GetSystemMetrics(SM_CXSCREEN) is not enough, might not be our current Screen
	DWORD Current_Screen_HEIGHT = 0;

	TCHAR   DeviceNameToUseOnFullScreen[MAX_STR_LEN] = { 0 };

	// Iterate on all Monitors and get the below settings, for the target monitor defined: settings.xml (uiMonitor="2" )
	// --------------------------------------------------------------------------------------------
	// AppSettings->WINDOW_Xpos 
	// AppSettings->WINDOW_Ypos 
	// Current_Screen_WIDTH
	// Current_Screen_HEIGHT
	// AppSettings->BITSPERPEL

	if (WOMA::AppSettings->FULL_SCREEN)
		WOMA::AppSettings->AllowResize = true;					// Force: "Allow" User to resize to FullScreen.

	if (windowsArray.size() == 0)
	{
		WOMA::WindowDataContainer screen;
		SystemHandle->windowsArray.push_back(screen);
	}

    // If we have multiple monitors, extract info from the monitor that we will use:
	displayDevice.cb = sizeof(DISPLAY_DEVICE);
	while (EnumDisplayDevices(NULL, deviceNum, &displayDevice, 0))	// Get deviceNum
	{
		// Get our Screen name (on THIS monitor)
		if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
		{
			if (((deviceNum == MONITOR_NUM) && (WOMA::AppSettings->UseAllMonitors == true)) || (WOMA::AppSettings->UseAllMonitors == false))
			{
				_tcscpy_s(MainDeviceName, 32, displayDevice.DeviceName);
				womalogauto((TCHAR*)TEXT("Monitor: %s\n"), displayDevice.DeviceName); //WCHAR
				womalogauto((TCHAR*)TEXT("Card: %s "), displayDevice.DeviceString);
			}

			// Use the Monitor selected by user:
			if (((deviceNum == WOMA::AppSettings->UI_MONITOR) && (WOMA::AppSettings->UseAllMonitors == false)) ||
				((deviceNum == MONITOR_NUM) && (WOMA::AppSettings->UseAllMonitors == true)))
			{
				womalogauto((TCHAR*)TEXT("[Using this Monitor!]\n")); //WCHAR
				_tcscpy_s(DeviceNameToUseOnFullScreen, 32, displayDevice.DeviceName);

				//AppSettings->WINDOW_Xpos	windowsArray[MONITOR_NUM].width		AppSettings->BITSPERPEL
				//AppSettings->WINDOW_Ypos	windowsArray[MONITOR_NUM].height
				//Mon0: 3840  -4			= 1920 1080							32
				//Mon1: 0      0			= 3840 2160							32
				//Mon2: -1920 16			= 1920 1080							32

				//Translate xpos to Multi-Screen Position virtual position:
				WOMA::AppSettings->WINDOW_Xpos = devMode.dmPosition.x;
				WOMA::AppSettings->WINDOW_Ypos = devMode.dmPosition.y;

				Current_Screen_WIDTH = devMode.dmPelsWidth;
				windowsArray[MONITOR_NUM].width = devMode.dmPelsWidth;

				Current_Screen_HEIGHT = devMode.dmPelsHeight;
				windowsArray[MONITOR_NUM].height = devMode.dmPelsHeight;

				WOMA::AppSettings->BITSPERPEL = devMode.dmBitsPerPel;
			}
			else
				womalogauto((TCHAR*)TEXT("\n")); //WCHAR

			if (((deviceNum == MONITOR_NUM) && (WOMA::AppSettings->UseAllMonitors == true)) || (WOMA::AppSettings->UseAllMonitors == false))
			{
				womalogauto((TCHAR*)TEXT("X0, Y0 = %d, %d\n"), devMode.dmPosition.x, devMode.dmPosition.y);			// Get X0, Y0 position (of THIS monitor):
				womalogauto((TCHAR*)TEXT("Current Resolution: %d x %d\n"), devMode.dmPelsWidth, devMode.dmPelsHeight);// Current Resolution (on THIS monitor):
				womalogauto((TCHAR*)TEXT("Bits Supported: %d Bits\n\n"), devMode.dmBitsPerPel);							// Get num. bits per pixel Supported (on THIS monitor):
			}
		}
		else
			break;

		++deviceNum;
	}

	// Check User Settings: (settings.xml) Want to Change for Best more Available? (width="0" height = "0") ?
	//----------------------------------------------------------------------------
	if ((WOMA::AppSettings->WINDOW_WIDTH == 0) && (WOMA::AppSettings->WINDOW_HEIGHT == 0)) // Auto-Detect if Current Size: (0,0)
	{
		WOMA::AppSettings->WINDOW_WIDTH = Current_Screen_WIDTH;
		WOMA::AppSettings->WINDOW_HEIGHT = Current_Screen_HEIGHT;
	}

	//  We want Full-Screen? ok, Change the current "Display Resolution" (settings.xml) (fullScreen="false"/"true")
	// ----------------------------------------------------------------
	if (WOMA::AppSettings->FULL_SCREEN)
	{
		womalog((TCHAR*)TEXT("Go to FullScreen Mode\n"));

		// Do we need to update Display Settings, i.e. user is already on this "resolution mode" (AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT) ?
		{
			DEVMODE dmScreenSettings = { 0 }; //memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)WOMA::AppSettings->WINDOW_WIDTH;
			dmScreenSettings.dmPelsHeight = (unsigned long)WOMA::AppSettings->WINDOW_HEIGHT;
			dmScreenSettings.dmBitsPerPel = WOMA::AppSettings->BITSPERPEL;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Change the current "Display Settings" for "Full-Screen" Mode (in the monitor Selected by User)
			womalog((TCHAR*)TEXT("Monitor: %s  --- going ---> to FULLSCREEN: %dx%d %d\n"), DeviceNameToUseOnFullScreen, dmScreenSettings.dmPelsWidth, dmScreenSettings.dmPelsHeight, dmScreenSettings.dmBitsPerPel);
			if (ChangeDisplaySettingsEx(DeviceNameToUseOnFullScreen, &dmScreenSettings, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
			{
				womalog((TCHAR*)TEXT("Switch to FULLSCREEN: Failed\n"));

				// Used to convert 1500x700 (sample of a "non available size" in Full-Screen) 
				// Last try - Use current resolution instead once we want Full-Screen Mode:
				WOMA::AppSettings->WINDOW_WIDTH = Current_Screen_WIDTH;
				WOMA::AppSettings->WINDOW_HEIGHT = Current_Screen_HEIGHT;

				// Last try!:
				dmScreenSettings.dmPelsWidth = (unsigned long)WOMA::AppSettings->WINDOW_WIDTH;
				dmScreenSettings.dmPelsHeight = (unsigned long)WOMA::AppSettings->WINDOW_HEIGHT;

				womalog((TCHAR*)TEXT("Monitor: %s  --- going ---> to FULLSCREEN: %dx%d %d\n"), DeviceNameToUseOnFullScreen, dmScreenSettings.dmPelsWidth, dmScreenSettings.dmPelsHeight, dmScreenSettings.dmBitsPerPel);
				if (ChangeDisplaySettingsEx(DeviceNameToUseOnFullScreen, &dmScreenSettings, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
					WomaFatalException("ERROR - Switch to FULLSCREEN: Can't use that resolution!");
			}
		}
	}
	else// Windowed: If we will not change the resolution, use current resolution (of THIS monitor)
	{	// ---------------------------------------------------------------------------------------
		womalog((TCHAR*)TEXT("Go to Windowed Mode\n"));
		WOMA::AppSettings->SCREEN_RESOLUTION_WIDTH = Current_Screen_WIDTH;	//GetSystemMetrics(SM_CXSCREEN);
		WOMA::AppSettings->SCREEN_RESOLUTION_HEIGHT = Current_Screen_HEIGHT;	//GetSystemMetrics(SM_CYSCREEN);
	}

	// -------------------------------------------------------------------------------------------
    RECT R = { 0, 0, width, height };
    bool FULLSCREEN_ON_WINDOWED= WOMA::AppSettings->FULLSCREEN_ON_WINDOWED;
	if (WOMA::AppSettings->FULL_SCREEN)
		windowStyle = WS_POPUP | WS_VISIBLE;		// FULLSCREEN
	else
	{
        // ALLOW RESIZE:
		if (WOMA::AppSettings->AllowResize)
		{

#if defined USE_NORMAL_WINDOW
			windowStyle = WS_OVERLAPPEDWINDOW;													    // No TaskBar (bottom)
#else
			windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS & ~WS_THICKFRAME; // Allow TaskBar (bottom)
#endif

			// Define Window Size and Position:
			AdjustWindowRect(&R, windowStyle, false);	// Compute "window rectangle dimensions" based on "requested client area" dimensions, for this "style"!
		}
		else
        
		{
#if !defined USE_ALLOW_MAINWINDOW_RESIZE
			// DO NOT ALLOW RESIZE:
			windowStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) & ~WS_THICKFRAME & ~WS_BORDER;
#else
			// Allow full-Screen on a Windowed:
			if (((WOMA::AppSettings->WINDOW_WIDTH == WOMA::AppSettings->SCREEN_RESOLUTION_WIDTH) && (WOMA::AppSettings->WINDOW_HEIGHT == WOMA::AppSettings->SCREEN_RESOLUTION_HEIGHT)) || FULLSCREEN_ON_WINDOWED)
            { 
                windowStyle = WS_OVERLAPPED | WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS & ~WS_THICKFRAME;
                windowStyle = windowStyle & (~WS_CAPTION);
                FULLSCREEN_ON_WINDOWED = true;
            }
            else {
                windowStyle = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
            }
#endif
			windowStyle = windowStyle & (~WS_EX_TOPMOST);

			// Define Window Size and Position:
			AdjustWindowRect(&R, windowStyle, false);
		}
	}

    // Determine the real / resolution of the Window on this "style":
	WOMA::AppSettings->WINDOW_WIDTH = (WOMA::AppSettings->FULL_SCREEN) ? WOMA::AppSettings->WINDOW_WIDTH : (R.right - R.left);
	WOMA::AppSettings->WINDOW_HEIGHT = (WOMA::AppSettings->FULL_SCREEN) ? WOMA::AppSettings->WINDOW_HEIGHT : (R.bottom - R.top);

	#if defined USE_ASPECT_RATIO
	if (AppSettings->AllowResize)
		aspect_r = (float)AppSettings->WINDOW_WIDTH / (float)AppSettings->WINDOW_HEIGHT;// Aspect Ratio: To keep it, on "Resize Window"
	#endif

	// In Full-Screen, select the Window Position of that MONITOR:
	int windowLeft=0, windowTop=0;
	if (WOMA::AppSettings->FULL_SCREEN || FULLSCREEN_ON_WINDOWED)
	{
		windowLeft = WOMA::AppSettings->WINDOW_Xpos;
		windowTop = WOMA::AppSettings->WINDOW_Ypos;
	}
	else
	{
		windowLeft = WOMA::AppSettings->WINDOW_Xpos - 8;
		windowTop = WOMA::AppSettings->WINDOW_Ypos;
	}

	if (WOMA::renderOnce) //mini-demo windows from test*.bat files.
	{
		windowLeft += WOMA::settings.WINDOW_Xpos_ori;
		windowTop += WOMA::settings.WINDOW_Ypos_ori;
		WOMA::AppSettings->WINDOW_WIDTH = WOMA::settings.WINDOW_WIDTH_ori;
		WOMA::AppSettings->WINDOW_HEIGHT = WOMA::settings.WINDOW_HEIGHT_ori;
	}

    DWORD dwExStyle = 0;

	if ((WOMA::AppSettings->WINDOW_WIDTH == 0) && (WOMA::AppSettings->WINDOW_HEIGHT == 0))
		WomaFatalException("FATAL ERROR: Monitor Settings Invalid");

	// ------------------------------------------------------------------------------------------
	// Create: WIN32 - Main Window 
	// ------------------------------------------------------------------------------------------
#if defined USE_NORMAL_WINDOW
	HWND hWnd = CreateWindow(WOMA_ENGINE_CLASS, WOMA::APP_FULLNAME, WS_OVERLAPPEDWINDOW,
        windowLeft, windowTop, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
		nullptr, nullptr, m_hinstance, nullptr);
#else
	int TaskBarHeigth = 0;
#if defined _DEBUG && defined USE_STATUSBAR
    if (!WOMA::AppSettings->FULL_SCREEN)
    {
	    TaskBarHeigth = WOMA::getTaskBarHeight();
		WOMA::AppSettings->WINDOW_HEIGHT = WOMA::AppSettings->WINDOW_HEIGHT; // -TaskBarHeigth - TaskBarHeigth;
    }
#endif

	#define DEFAULT_TITLE TEXT("Loading...")


	womalog("WINDOW_WIDTH: %d, WINDOW_HEIGHT: %d1n", WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT);
	// [*] Create the window and return the handle to it:
	HWND hWnd = WomaCreateWindowEx( dwExStyle, (TCHAR*)WOMA_ENGINE_CLASS, DEFAULT_TITLE, windowStyle,
                            		windowLeft, windowTop,
                            		WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
                            		NULL,	    // We have no parent window
                            		NULL,	    // We aren't using menus
                            		m_hinstance,
                            		NULL);	    // We aren't using multiple windows
#endif

	ASSERT(hWnd);

    // Save window for Main Monitor
    m_hWnd = hWnd;

    // WindowDataContainer
    windowsArray[MONITOR_NUM].hWnd = hWnd;

    // ------------------------------------------------------------------------------------------
    // Create: WIN32 - Status bar Window 
    // ------------------------------------------------------------------------------------------
#if defined USE_STATUSBAR
    if (!WOMA::AppSettings->FULL_SCREEN)
    {
        SystemHandle->statusbar = DoCreateStatusBar(hWnd, 0/*idStatus*/, m_hinstance, 1/*cParts*/);
        SendMessage(SystemHandle->statusbar, SB_SETTEXT, 0, (LPARAM)DEMO_TITLE);
    }
#endif

	ShowWindow(MONITOR_NUM, windowLeft, windowTop);

	return true;
}

bool WinSystemClass::ShowWindow(UINT MONITOR_NUM, int windowLeft, int windowTop)
{
#if defined USE_STATUSBAR
	if (WOMA::AppSettings->FULL_SCREEN)
		::ShowWindow(SystemHandle->statusbar, SW_HIDE);
#endif

	int Cmdshow = WOMA::Cmdshow;
	if (!WOMA::AppSettings->FULL_SCREEN)
		Cmdshow = (WOMA::renderOnce == TRUE) ? WOMA::Cmdshow : SW_MAXIMIZE;
	if (WOMA::AppSettings->UseAllMonitors)
		Cmdshow = SW_MAXIMIZE;

	::ShowWindow(m_hWnd, Cmdshow);	// Use from Command line option! NOTE: Don't hardcode:	(default: SW_SHOWDEFAULT) SW_SHOW / SW_SHOWMINIMIZED

	SetForegroundWindow(m_hWnd);    // Slightly "Higher Priority"
	SetFocus(m_hWnd);               // Force "Focus" to our Window
	UpdateWindow(m_hWnd);           // 1st Window WIN32/"Paint"  NOW!

	// Save window properties
	GetWindowRect(m_hWnd, &windowsArray[MONITOR_NUM].m_rcWindowBounds);	//{m_rcWindowBounds = {LT(3832, -21) RB(5768, 1027)  [1936 x 1048]}}
	GetClientRect(m_hWnd, &windowsArray[MONITOR_NUM].m_rcWindowClient);	//{m_rcWindowClient = {LT(0, 0) RB(1920, 1009)  [1920 x 1009]}}	

	return true;
}


#if defined CLIENT_SCENE_SETUP
#pragma warning(push)
#pragma warning( disable : 4715 )
extern void ListDisplayModes(TCHAR* deviceName);
BOOL CALLBACK MyInfoEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    MONITORINFOEX iMonitor;
    iMonitor.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMonitor, &iMonitor);


    // Part II: get current settings:
    double horizontalScale = 0;
    double verticalScale = 0;
    double cxLogical = 0;
    double cyLogical = 0;
    double cxPhysical = 0;
    double cyPhysical = 0;
    {
        // Get the logical width and height of the monitor
        MONITORINFOEX monitorInfoEx;
        monitorInfoEx.cbSize = sizeof(monitorInfoEx);
        GetMonitorInfo(hMonitor, &monitorInfoEx);
        cxLogical = monitorInfoEx.rcMonitor.right - monitorInfoEx.rcMonitor.left;
        cyLogical = monitorInfoEx.rcMonitor.bottom - monitorInfoEx.rcMonitor.top;

        // Get the physical width and height of the monitor
        DEVMODE devMode;
        devMode.dmSize = sizeof(devMode);
        devMode.dmDriverExtra = 0;
        EnumDisplaySettings(monitorInfoEx.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
        cxPhysical = devMode.dmPelsWidth;
        cyPhysical = devMode.dmPelsHeight;

        // Calculate the scaling factor
        horizontalScale = ((double)cxPhysical / (double)cxLogical);
        verticalScale = ((double)cyPhysical / (double)cyLogical);
    }

    if (iMonitor.dwFlags == DISPLAY_DEVICE_MIRRORING_DRIVER)
    {
        return true;
    }
    else
    {
        ScreenArrayInfo* info = reinterpret_cast<ScreenArrayInfo*>(dwData);
        if (info->Count == info->MaxCount)
            return false;
        SystemHandle->monitorArray[info->Count] = iMonitor;

        //SETUP SIMILAR ARRAY as DX11DRIVER:
        WOMA::WindowDataContainer screen;
        screen.MonitorName = iMonitor.szDevice;
        screen.width = (UINT)cxPhysical;	// GetSystemMetrics(SM_CXSCREEN);
        screen.height = (UINT)cyPhysical; // GetSystemMetrics(SM_CYSCREEN);
        screen.refreshRate_Numerator = 60;  //Default
        screen.refreshRate_Denominator = 1; //Default

        if ((info->Count == WOMA::AppSettings->UI_MONITOR) || (WOMA::AppSettings->UseAllMonitors == true))
            SystemHandle->windowsArray.push_back(screen); //List of Monitors that will be used

        info->Count++;
        return true;
    };

}
#pragma warning(pop)
#endif

bool WinSystemClass::APPLICATION_INIT_MAIN_WINDOW()
//----------------------------------------------------------------------------
{
#if defined RELEASE
	// PURPOSE: Registers the Window Application Class, but first check if we are running!
	if (FindWindow(WOMA_ENGINE_CLASS, NULL))
	{
		WomaMessageBox((TCHAR*)TEXT("Another Process is already Running..."), (TCHAR*)TEXT("FATAL ERROR:"));
		WOMA::main_loop_state = -1;
		return false;
	}
	else
#endif
	{
		if (!WomaRegisterClass(m_hinstance)) {// Try to Register WOMA Engine WINDOW CLASS
			WOMA::main_loop_state = -1;		
			return false;
		}
	}

#if !defined WOMA_SKIP_CREATE_MAIN_WINDOW
    #if defined CLIENT_SCENE_SETUP
	//Populate Monitor List: (for Game Setup)
	info.Array = (ScreenArrayInfo*)&monitorArray;
	info.Count = 0;
	info.MaxCount = MAX_WIN32_MONITORS;
	EnumDisplayMonitors(NULL, NULL, &MyInfoEnumProc, reinterpret_cast<LPARAM>(&info));
    #endif

	/*******************************************************************
	// [CREATE the Main WINDOW]:
	*******************************************************************/
	if (WOMA::AppSettings->UseAllMonitors == false) // Are we using a specific Monitor?
	{
		//0 (FIXED) Means the Select Monitor: can be 0, 1 ,2...
		IF_NOT_RETURN_FALSE(CreateWin32MainWindow(0 /*Just one monitor?*/, g_contextDriver, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT));
	}
	else
	{
		// For each Monitor:
		for (int i = 0; i < windowsArray.size(); ++i)
			IF_NOT_RETURN_FALSE(CreateWin32MainWindow(i /* use all these monitors*/, g_contextDriver, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT));
	}
#endif

	return true;
}


//----------------------------------------------------------------------------
void WinSystemClass::PAUSE()
{
	if (WOMA::game_state != ENGINE_RESTART)
	{
		WOMA::previous_game_state = WOMA::game_state;
		WOMA::game_state = GAME_PAUSED;
	}
}

void WinSystemClass::UNPAUSE()
{
	if (WOMA::game_state != ENGINE_RESTART)
	{
		WOMA::game_state = WOMA::previous_game_state;
	}
}

#if defined USE_ALLOW_MAINWINDOW_RESIZE //CORE_ENGINE_LEVEL >= 10 // Initializing Engine
void WinSystemClass::ONRESIZE(void* pContext)
{
	if (SystemHandle) {
		womalog("ONRESIZE()\n");
		if (SystemHandle->m_Application)
			SystemHandle->m_Application->WOMA_APPLICATION_InitGUI();
		#if defined DX_ENGINE //OPENGL TODO
		if (DXsystemHandle)
			DXsystemHandle->GPH_RESIZE(pContext);
		#endif
	}
}
#endif


