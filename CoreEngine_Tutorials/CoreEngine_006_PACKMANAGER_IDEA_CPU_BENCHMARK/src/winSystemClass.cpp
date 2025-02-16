// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: winSystemClass.cpp
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
// PURPOSE: Define APIs for winSystemClass.cpp which is the WINDOWS OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#include "OSengine.h"
#include "mem_leak.h"
#if defined DX_ENGINE
#include "DXengine.h"
#endif

#include "OSmain_dir.h"
#include "language.h"

#include "stateMachine.h"

#include "woma_exception.h"

#include "packManager.h"
#include "idea.h"

/////////////////////
// Windows GLOBALS //
/////////////////////

TCHAR MainDeviceName[MAX_STR_LEN];	// Monitor Name

//----------------------------------------------------------------------------------
WinSystemClass::WinSystemClass() : SystemClass() 
//----------------------------------------------------------------------------------
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567142;

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
	AppSettings		= appSettings; // Super
	SystemHandle	= this;
	WinSystemClass_init();
}

//----------------------------------------------------------------------------
void WinSystemClass::ProcessFrame()
//----------------------------------------------------------------------------
{
	SystemClass::FrameUpdate();	// Process: (INPUT + PerformanceStats) Only!
	if (WOMA::game_state == ENGINE_RESTART)
		return;

#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
	if (WOMA::game_state == GAME_SETUP)
	{
		// F6: Init WOMA Setup:
		if (!SystemHandle->womaSetup)
		{
			SystemHandle->womaSetup = NEW WomaSetupManager;
			SystemHandle->womaSetup->Initialize(NULL);
			OS_REDRAW_WINDOW;
		}
	}
#endif

}

void WinSystemClass::WinSystemClass_init()
{
// --------------------------------------------------------------
// Init Vars:
// --------------------------------------------------------------

	m_hinstance = NULL;
#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
	m_OsInput = NULL;
#endif
	windowStyle = NULL;
	if (SystemHandle->AppSettings)
		mMaximized = SystemHandle->AppSettings->FULL_SCREEN;

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

// WINDOWS/LINUX
extern int InitImGui(HWND hwnd = NULL);

bool WinSystemClass::APPLICATION_CORE_SYSTEM()
{
	WOMA_LOGManager_DebugMSG("WinSystemClass::APPLICATION_INIT_SYSTEM()\n");

	//FILE1: windows.pack (zip)
	PackDir(TEXT("\\WoMA3Dengine\\windows_engine"), TEXT("windows.pack"));
#if defined USE_IDEA_PACK
	//Test1:
	IF_NOT_THROW_EXCEPTION(EncodeIDEA("\\WoMA3Dengine\\windows_engine\\windows.pack", ENCODE));	// windows.pack (zip) --> windows.pck (encoded)
#endif
	WomaMessageBox(TEXT("This diretory: \\WoMA3Dengine\\windows_engine\nwas packed(zip) into: \\WoMA3Dengine\\windows.pack\nand then Encoded into: \\WoMA3Dengine\\windows.pck\nPress OK, to close app."), TEXT("Tutorial: 006"));

	// 
	//FILE2: woma.pack (zip)
	PackDir(TEXT("\\WoMA3Dengine\\woma_engine"), TEXT("woma.pack"));
#if defined USE_IDEA_PACK
	//Test1:
	IF_NOT_THROW_EXCEPTION(EncodeIDEA("\\WoMA3Dengine\\woma_engine\\woma.pack", ENCODE));	// windows.pack (zip) --> windows.pck (encoded)
#endif
	WomaMessageBox(TEXT("This diretory: \\WoMA3Dengine\\woma_engine\nwas packed(zip) into: \\WoMA3Dengine\\woma.pack\nand then Encoded into: \\WoMA3Dengine\\woma.pck\nPress OK, to close app."), TEXT("Tutorial: 006"));

	return false;

	if (WOMA::game_state == GAME_LOADING)
		WOMA::game_state = GAME_MENU;	// Force first frame to read key

	return true;
}

bool WinSystemClass::APPLICATION_INIT_SYSTEM()
//----------------------------------------------------------------------------
{
	//  NOTE: Constructors run, First!
	//  SystemClass::SystemClass()				Run: 1st - OS common    - WOMA::APP_NAME
	//	ApplicationClass::ApplicationClass()	Run: 2nd - User: level  - ApplicationClass::Start()
	//	WinSystemClass::WinSystemClass()		Run: 3th - Start Timers - WinSystemClass::WinSystemClass_init();

	IF_NOT_RETURN_FALSE(APPLICATION_CORE_SYSTEM()); // MyRegisterClass()

#if defined USE_TINYXML_LOADER // Must be before: ApplicationInitMainWindow()
	IF_NOT_RETURN_FALSE(LoadXmlSettings());	// XML: Load Application Settings: "settings.xml", pickup "Driver" to Use.
#endif
#if defined USE_SYSTEM_CHECK // BEFORE: ApplicationInitMainWindow()
	IF_NOT_RETURN_FALSE(SystemClass::SystemCheck());		// SYSTEM INFO: HW (OS, CPU, RAM, DiskFreeSpace, CPUFeatures) 
#endif
	IF_NOT_RETURN_FALSE(ApplicationInitMainWindow());		// CREATE: The/all "MainWindow(s) + INIT DX/GL "rendering-device"
#if defined USE_PROCESS_OS_KEYS
	IF_NOT_RETURN_FALSE(InitOsInput());						// INIT-INPUT Devices, NOTE: AFTER: ApplicationInitMainWindow()
#endif
#if defined USE_TIMER_CLASS									// WINDOWS AFTER: ApplicationInitMainWindow()
	StartTimer();											// START-TIMERS: ("Window Title" refresh & Real-Time Weather refresh)
#endif

// ########################################### LOAD DRIVERS ###########################################
	
#if CORE_ENGINE_LEVEL < 10 && defined USE_SYSTEM_CHECK // BEFORE need to be: ApplicationInitMainWindow() & AFTER need to be: InitSelectedDriver()
	InitializeSystemScreen(10, 10); // SETUP SCREEN: F1,F2,F3,F4,F5,F6 (RUNNING NOW ON: PaintSetup())
#endif

 // ################################################# INIT DRIVERS ###################################
	
	return true;						// GREEN LIGHT: to Start Rendering! :)
}

#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
//----------------------------------------------------------------------------------------------------------
void WinSystemClass::GetInputs()
{
#if defined USE_DIRECT_INPUT						// Read the User Input
	if (DXsystemHandle->m_Input->m_mouse && DXsystemHandle->m_Input->m_keyboard)	// Make Sure that we have aquired the FOCUS and INPUT:
	{
		ASSERT(DXsystemHandle->m_Input->Frame()); // Update "Keyboard State": Process the changes in the Mouse and Keyboard.
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
	MSG msg = { 0 };						// Reset msg

	//DEBUG:
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// There is any OS messages to handle?
		{
			TranslateMessage(&msg); // TranslateMessage produces WM_CHAR messages only for keys that are mapped to ASCII characters by the keyboard driver.
			DispatchMessage(&msg);  // Process Msg:  (INVOKE: WinSystemClass::MessageHandler)
		}
		else
		{	// Active?
			if (WOMA::game_state > GAME_MINIMIZED && WOMA::game_state <= GAME_RUN)
				ProcessFrame();			// <----- Render ONE: Application Frame
			else {
				if (WOMA::game_state == ENGINE_RESTART)
					return ENGINE_RESTART;
				else
					Sleep(50);
			}
		}
	} while (msg.message != WM_QUIT && WOMA::main_loop_state >= 0);

	return S_OK;
}

void WinSystemClass::Shutdown()
//----------------------------------------------------------------------------------
{
	// WinSystemClass Shutdown:
	WOMA_LOGManager_DebugMSGAUTO ((TCHAR*)TEXT("WinSystemClass::Shutdown()\n"));

	// Destroy Drivers:
	SystemClass::Shutdown();
	
#if defined CLIENT_SCENE_SETUP
	SAFE_SHUTDOWN(womaSetup);
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
	if (AppSettings) {
		if (AppSettings->FULL_SCREEN) {
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
bool WinSystemClass::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);

	// ALLOW WIN32 SYSTEM PAINT: (Causes the entire window to redraw if a movement or a size adjustment changes the height of the client area: CS_HREDRAW | CS_VREDRAW)
	wcex.style = (AppSettings->DRIVER == DRIVER_GL3) ? CS_OWNDC : CS_HREDRAW | CS_VREDRAW; // NOTE: CS_OWNDC is need by OPEN GL: https://www.opengl.org/wiki/Platform_specifics:_Windows
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;

	//
	// To Use External Icon: "*.png" -> "*.ico" Converter: http://converticon.com/
	// NOTE: I am Avoding to use Resource here: //wcex.hIcon = wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//
	// More info WNDCLASSEX: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577%28v=vs.85%29.aspx
	//
	IF_NOT_RETURN_FALSE(WOMA::getCurrentDir());
	bool gotIconFile = true;
	if (!WOMA::fileExists(WOMA::APP_ICO))
	{
		STRING err = TEXT("File not found: "); err += WOMA::APP_ICO;
		//DONT USE WomaMessageBox!
		MessageBox(NULL, err.c_str(), TEXT("WARNING: MyRegisterClass()"), MB_ICONWARNING);
		gotIconFile = false;
	}

	if (gotIconFile) {
		wcex.hIcon = (HICON)LoadImage(hInstance, WOMA::APP_ICO, IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
		wcex.hIconSm = (HICON)LoadImage(hInstance, WOMA::APP_ICO, IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
	}

	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //IDC_CROSS

	//wcex.hbrBackground = GetSysColorBrush(COLOR_3DFACE);			//TO USE THIS COLOR: GRAY
	//wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	//TO USE THIS COLOR: WHITE
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//TO USE THIS COLOR: BLACK
	wcex.lpszClassName  = WOMA_ENGINE_CLASS;
	wcex.cbSize = sizeof(WNDCLASSEX);


	IF_NOT_RETURN_FALSE (RegisterClassEx(&wcex));

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
	SystemClass::InitOsInput();

	// INIT OS Keyboard (WIN32: This object will be used to handle reading the input from the user)
	WOMA_LOGManager_DebugMSG("===============================================================================\n");
	WOMA_LOGManager_DebugMSG("INIT OS BASIC INPUT\n");

	m_OsInput = NEW InputClass;
	IF_NOT_THROW_EXCEPTION(m_OsInput);

	m_OsInput->Initialize();

#if defined USE_DIRECT_INPUT
	// Set the Player Position Init Player Class
	WOMA_LOGManager_DebugMSG("===============================================================================\n");
	WOMA_LOGManager_DebugMSG("INIT OS ADVANCED DIRECT INPUT\n");
	WOMA_LOGManager_DebugMSG("===============================================================================\n");


	DXsystemHandle->m_Input = (DXInputClass*)&SystemHandle->m_InputManager;
#endif

	return true;
}
#endif

bool WinSystemClass::CreateMainWindow(	UINT MONITOR_NUM, /*WomaDriverClass*/ void* 
										/*OpenGL*/ driver, int& width, int& height)
//----------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("---------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("WinSystemClass::CreateMainWindow()\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("---------------------------------\n"));

	if (AppSettings->FULL_SCREEN)
		AppSettings->AllowResize = true;					// Force: "Allow" User to resize to FullScreen.

	// --------------------------------------------------------------------------------------------
	// PURPOSE: Check all Monitors available
	// --------------------------------------------------------------------------------------------
	DEVMODE devMode = { 0 };
	DWORD deviceNum = 0;					// How Many Monitors we have?
	DWORD Current_Screen_WIDTH = 0;			// NOTE: GetSystemMetrics(SM_CXSCREEN) is not enouf, might not be our current Screen
	DWORD Current_Screen_HEIGHT = 0;

	TCHAR   DeviceNameToUseOnFullScreen[MAX_STR_LEN] = { 0 };

	// Iterate on all Monitors and get the below settings, for the target monitor defined: settings.xml (uiMonitor="2" )
	// --------------------------------------------------------------------------------------------
	// AppSettings->WINDOW_Xpos 
	// AppSettings->WINDOW_Ypos 
	// Current_Screen_WIDTH
	// Current_Screen_HEIGHT
	// AppSettings->BITSPERPEL

	if (windowsArray.size() == 0)
	{
		WOMA::WindowDataContainer screen;
		SystemHandle->windowsArray.push_back(screen);
	}

	displayDevice.cb = sizeof(DISPLAY_DEVICE);
	while (EnumDisplayDevices(NULL, deviceNum, &displayDevice, 0))	// Get deviceNum
	{
		// Get our Screen name (on THIS monitor)
		if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
		{
			if (((deviceNum == MONITOR_NUM) && (AppSettings->UseAllMonitors == true)) || (AppSettings->UseAllMonitors == false))
			{
				_tcscpy_s(MainDeviceName, 32, displayDevice.DeviceName);
				WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Monitor: %s\n"), displayDevice.DeviceName); //WCHAR
				WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Card: %s "), displayDevice.DeviceString);
			}

			// Use the Monitor selected by user:
			if (((deviceNum == AppSettings->UI_MONITOR) && (AppSettings->UseAllMonitors == false)) ||
				((deviceNum == MONITOR_NUM) && (AppSettings->UseAllMonitors == true)))
			{
				WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("[Using this Monitor!]\n")); //WCHAR
				_tcscpy_s(DeviceNameToUseOnFullScreen, 32, displayDevice.DeviceName);

				//AppSettings->WINDOW_Xpos	windowsArray[MONITOR_NUM].width		AppSettings->BITSPERPEL
				//AppSettings->WINDOW_Ypos	windowsArray[MONITOR_NUM].height
				//Mon0: 3840  -4			= 1920 1080							32
				//Mon1: 0      0			= 3840 2160							32
				//Mon2: -1920 16			= 1920 1080							32

				//Translate xpos to Multi-Screen Position virtual position:
				AppSettings->WINDOW_Xpos = devMode.dmPosition.x + AppSettings->WINDOW_Xpos;
				AppSettings->WINDOW_Ypos = devMode.dmPosition.y + AppSettings->WINDOW_Ypos;

				Current_Screen_WIDTH = devMode.dmPelsWidth;
				windowsArray[MONITOR_NUM].width = devMode.dmPelsWidth;

				Current_Screen_HEIGHT = devMode.dmPelsHeight;
				windowsArray[MONITOR_NUM].height = devMode.dmPelsHeight;

				AppSettings->BITSPERPEL = devMode.dmBitsPerPel;
			}
			else
				WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("\n")); //WCHAR

			if (((deviceNum == MONITOR_NUM) && (AppSettings->UseAllMonitors == true)) || (AppSettings->UseAllMonitors == false))
			{
				WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("X0, Y0 = %d, %d\n"), devMode.dmPosition.x, devMode.dmPosition.y);			// Get X0, Y0 position (of THIS monitor):
				WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Current Resolution: %d x %d\n"), devMode.dmPelsWidth, devMode.dmPelsHeight);// Current Resolution (on THIS monitor):
				WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Bits Supported: %d Bits\n\n"), devMode.dmBitsPerPel);							// Get num. bits per pixel Supported (on THIS monitor):
			}
		}
		else
			break;

		++deviceNum;
	}

	// Check User Settings: (settings.xml) Want to Change for Best more Available? (width="0" height = "0") ?
	//----------------------------------------------------------------------------
	if ((AppSettings->WINDOW_WIDTH == 0) && (AppSettings->WINDOW_HEIGHT == 0)) // Auto-Detect if Current Size: (0,0)
	{
		AppSettings->WINDOW_WIDTH = Current_Screen_WIDTH;
		AppSettings->WINDOW_HEIGHT = Current_Screen_HEIGHT;
	}

	//  We want Full-Screen? ok, Change the current "Display Resolution" (settings.xml) (fullScreen="false"/"true")
	// ----------------------------------------------------------------
	if (AppSettings->FULL_SCREEN)
	{
		WOMA_LOGManager_DebugMSG((TCHAR*)TEXT("Go to FullScreen Mode\n"));

		// Do we need to update Display Settings, i.e. user is already on this "resolution mode" (AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT) ?
		{
			DEVMODE dmScreenSettings = { 0 }; //memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)AppSettings->WINDOW_WIDTH;
			dmScreenSettings.dmPelsHeight = (unsigned long)AppSettings->WINDOW_HEIGHT;
			dmScreenSettings.dmBitsPerPel = AppSettings->BITSPERPEL;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Change the current "Display Settings" for "Full-Screen" Mode (in the monitor Selected by User)
			WOMA_LOGManager_DebugMSG((TCHAR*)TEXT("Monitor: %s  --- going ---> to FULLSCREEN: %dx%d %d\n"), DeviceNameToUseOnFullScreen, dmScreenSettings.dmPelsWidth, dmScreenSettings.dmPelsHeight, dmScreenSettings.dmBitsPerPel);
			if (ChangeDisplaySettingsEx(DeviceNameToUseOnFullScreen, &dmScreenSettings, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
			{
				WOMA_LOGManager_DebugMSG((TCHAR*)TEXT("Switch to FULLSCREEN: Failed\n"));

				// Used to convert 1500x700 (sample of a "non available size" in Full-Screen) 
				// Last try - Use current resolution instead once we want Full-Screen Mode:
				AppSettings->WINDOW_WIDTH = Current_Screen_WIDTH;
				AppSettings->WINDOW_HEIGHT = Current_Screen_HEIGHT;

				// Last try!:
				dmScreenSettings.dmPelsWidth = (unsigned long)AppSettings->WINDOW_WIDTH;
				dmScreenSettings.dmPelsHeight = (unsigned long)AppSettings->WINDOW_HEIGHT;

				WOMA_LOGManager_DebugMSG((TCHAR*)TEXT("Monitor: %s  --- going ---> to FULLSCREEN: %dx%d %d\n"), DeviceNameToUseOnFullScreen, dmScreenSettings.dmPelsWidth, dmScreenSettings.dmPelsHeight, dmScreenSettings.dmBitsPerPel);
				if (ChangeDisplaySettingsEx(DeviceNameToUseOnFullScreen, &dmScreenSettings, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
					WomaFatalException("ERROR - Switch to FULLSCREEN: Can't use that resolution!");
			}
		}
	}
	else// Windowed: If we will not change the resolution, use current resolution (of THIS monitor)
	{	// ---------------------------------------------------------------------------------------
		WOMA_LOGManager_DebugMSG((TCHAR*)TEXT("Go to Windowed Mode\n"));
		AppSettings->SCREEN_RESOLUTION_WIDTH = Current_Screen_WIDTH;	//GetSystemMetrics(SM_CXSCREEN);
		AppSettings->SCREEN_RESOLUTION_HEIGHT = Current_Screen_HEIGHT;	//GetSystemMetrics(SM_CYSCREEN);
	}

	// -------------------------------------------------------------------------------------------
	if (AppSettings->FULL_SCREEN)
		windowStyle = WS_POPUP | WS_VISIBLE;		// FULLSCREEN
	else
	{
		if (AppSettings->AllowResize)
		{
			// This Style is needed to allow Lancher to have the browser refreshed:
			//if (AppSettings->WINDOW_WIDTH == 1920)
			//	windowStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP; //Means: NO TOP BAR!
			//else
				windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS & ~WS_THICKFRAME;		// Normal WINDOW "Classic" (-[]X)

			// Define Window Size and Position:
			RECT R = { 0, 0, width, height };
			// [*] Define Window Style:
			AdjustWindowRect(&R, windowStyle, false);	// Compute "window rectangle dimensions" based on "requested client area" dimensions, fot this "style"!

			// Determine the real / resolution of the Window on this "style":
			AppSettings->WINDOW_WIDTH = (AppSettings->FULL_SCREEN) ? AppSettings->WINDOW_WIDTH : (R.right - R.left);
			AppSettings->WINDOW_HEIGHT = (AppSettings->FULL_SCREEN) ? AppSettings->WINDOW_HEIGHT : (R.bottom - R.top);
		}
		else
		{
			// WINDOW: With CLOSE option only! & Allow Child Windows on top! 
			// NOTE: Title Var				WS_CAPTION
			//		 [X]					WS_SYSMENU
			//		 Remove "sizing border" & ~WS_THICKFRAME 

			windowStyle = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			// Allow full-Screen on a Windowed:
			if ((AppSettings->WINDOW_WIDTH == AppSettings->SCREEN_RESOLUTION_WIDTH) && (AppSettings->WINDOW_HEIGHT == AppSettings->SCREEN_RESOLUTION_HEIGHT))
				windowStyle = windowStyle & (~WS_CAPTION);

			// Define Window Size and Position:
			RECT R = { 0, 0, width, height };
			// [*] Define Window Style:
			AdjustWindowRect(&R, windowStyle, false);
		}
	}

	#if defined USE_ASPECT_RATIO
	if (AppSettings->AllowResize)
		aspect_r = (float)AppSettings->WINDOW_WIDTH / (float)AppSettings->WINDOW_HEIGHT;// Aspect Ratio: To keep it, on "Resize Window"
	#endif

	// In Full-Screen, select the Window Position of that MONITOR:
	int windowLeft=0, windowTop=0;
	if (AppSettings->FULL_SCREEN)
	{
		windowLeft = AppSettings->WINDOW_Xpos;
		windowTop = AppSettings->WINDOW_Ypos;
	}
	else
	{
		windowLeft = AppSettings->WINDOW_Xpos - 8;
		windowTop = AppSettings->WINDOW_Ypos;
	}

	DWORD dwExStyle = (AppSettings->FULL_SCREEN) ? WS_EX_TOPMOST : WS_EX_APPWINDOW;

	if ((AppSettings->WINDOW_WIDTH == 0) && (AppSettings->WINDOW_HEIGHT == 0))
		WomaFatalException("FATAL ERROR: Monitor Settings Invalid");



	// ------------------------------------------------------------------------------------------
	// Create: WIN OS Main Window 
	// ------------------------------------------------------------------------------------------
#if normal_window
	HWND hWnd = CreateWindow(WOMA_ENGINE_CLASS, WOMA::APP_FULLNAME, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT,
		nullptr, nullptr, m_hinstance, nullptr);
#else
	int TaskBarHeigth = 0;
#if defined _DEBUG && defined USE_STATUSBAR
	TaskBarHeigth = WOMA::getTaskBarHeight();
	if (!AppSettings->FULL_SCREEN)
		AppSettings->WINDOW_HEIGHT = AppSettings->WINDOW_HEIGHT - TaskBarHeigth - 32/*statusBarHeigth*/;
#endif

	// [*] Create the window and return the handle to it:
	HWND hWnd = WomaCreateWindowEx(dwExStyle, (TCHAR*)WOMA_ENGINE_CLASS, "Loading...", windowStyle,
		windowLeft, windowTop,
		AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT,
		NULL,	// We have no parent window
		NULL,	// We aren't using menus
		m_hinstance,
		NULL);	// We aren't using multiple windows
#endif

	ASSERT(hWnd);
#if defined USE_STATUSBAR
	SystemHandle->statusbar = DoCreateStatusBar(hWnd, 0/*idStatus*/, m_hinstance, 1/*cParts*/);
	SendMessage(SystemHandle->statusbar, SB_SETTEXT, 0, (LPARAM)DEMO_TITLE);
#endif
	// Save window for Main Monitor
	m_hWnd = hWnd;

	// WindowDataContainer
	windowsArray[MONITOR_NUM].hWnd = hWnd;

	ShowWindow(windowLeft, windowTop);

	return true;
}

bool WinSystemClass::ShowWindow(int windowLeft, int windowTop) {
	//1
#if defined USE_STATUSBAR
	if (AppSettings->FULL_SCREEN)
		::ShowWindow(SystemHandle->statusbar, SW_HIDE);
#endif
	if (!AppSettings->FULL_SCREEN)
		::ShowWindow(m_hWnd, WOMA::Cmdshow);	// Use from Command line option! NOTE: Don't hardcode:	(default: SW_SHOWDEFAULT) SW_SHOW / SW_SHOWMINIMIZED

	//2
	SetForegroundWindow(m_hWnd);    // Slightly "Higher Priority"
	SetFocus(m_hWnd);               // Force "Focus" to our Window
	UpdateWindow(m_hWnd);           // 1st Window WIN32/"Paint"  NOW!

	//Useful if not "fullscreen" or "fullscreen windowed":
	//MoveWindow(m_hWnd, windowLeft, windowTop, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, TRUE);	// Adjust to Correct Real/Render size "Depend of the Window Sytle"

	// Save window properties
	GetWindowRect(m_hWnd, &m_rcWindowBounds);	//{top=0 bottom=1057 left=1920	right=3840}
	GetClientRect(m_hWnd, &m_rcWindowClient);	//{top=0 bottom=1018 left=0		right=1904}	

	return true;
}

BOOL CALLBACK MyInfoEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFOEX iMonitor;
	iMonitor.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &iMonitor);

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

		if ((info->Count == SystemHandle->AppSettings->UI_MONITOR) || (SystemHandle->AppSettings->UseAllMonitors == true))
			SystemHandle->windowsArray.push_back(screen);

		info->Count++;
		return true;
	};
}

bool WinSystemClass::ApplicationInitMainWindow()
//----------------------------------------------------------------------------
{
#if true //defined RELEASE
	// PURPOSE: Registers the Window Application Class, but first check if we are running!
	if (FindWindow(WOMA_ENGINE_CLASS, NULL))
	{
		WomaMessageBox((TCHAR*)TEXT("Another Process is already Running..."), (TCHAR*)TEXT("FATAL ERROR:"));
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
		return false;
	}
	else
#endif
	{
		if (!MyRegisterClass(m_hinstance)) {// Try to Register WOMA Engine WINDOW CLASS
			WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
			return false;
		}
	}

	//Populate Monitor List: (for Game Setup)
	info.Array = (ScreenArrayInfo*)&monitorArray;
	info.Count = 0;
	info.MaxCount = 15;
	EnumDisplayMonitors(NULL, NULL, &MyInfoEnumProc, reinterpret_cast<LPARAM>(&info));

	/*******************************************************************
	// [CREATE the Main WINDOW]:
	*******************************************************************/
#if !defined WOMA_SKIP_CREATE_MAIN_WINDOW 
	if (AppSettings->UseAllMonitors == false) // Are we using a specific Monitor?
	{
		//0 (FIXED) Means the Select Monitor: can be 0, 1 ,2...
		IF_NOT_RETURN_FALSE(CreateMainWindow(0 /*Just one monitor?*/, g_contextDriver, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT));
	}
	else
	{
		// For each Monitor:
		for (int i = 0; i < windowsArray.size(); ++i)
			IF_NOT_RETURN_FALSE(CreateMainWindow(i /* use all these monitors*/, g_contextDriver, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT));
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
void WinSystemClass::ONRESIZE()
{
	if (SystemHandle) {
		WOMA_LOGManager_DebugMSG("ONRESIZE()\n");
		if (SystemHandle->m_Application)
			SystemHandle->m_Application->WOMA_APPLICATION_InitGUI();
		#if defined DX_ENGINE //OPENGL TODO
		if (DXsystemHandle)
			DXsystemHandle->GPH_RESIZE();
		#endif
	}
}
#endif

