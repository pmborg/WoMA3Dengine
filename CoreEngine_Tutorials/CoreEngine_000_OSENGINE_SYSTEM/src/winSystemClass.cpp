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
#if defined DX_ENGINE
#include "DXengine.h"
#endif

/////////////////////
// Windows GLOBALS //
/////////////////////

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

void WinSystemClass::WinSystemClass_init()
{
// --------------------------------------------------------------
// Init Vars:
// --------------------------------------------------------------

#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
	m_OsInput = NULL;
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

	WomaMessageBox(WOMA::strConsoleTitle, TEXT("WOMA Hello World!"), MB_OK);
	return false;

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
}

namespace WOMA
{
	extern TCHAR* getCurrentDir();
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

