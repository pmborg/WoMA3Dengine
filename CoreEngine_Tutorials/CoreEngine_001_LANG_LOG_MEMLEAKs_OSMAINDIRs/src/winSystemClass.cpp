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

#include "OSmain_dir.h"
#include "language.h"

#include "SystemClass.h"

/////////////////////
// Windows GLOBALS //
/////////////////////

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

bool WinSystemClass::APPLICATION_BEFORE_WINDOW()
{
	womalog("WinSystemClass::APPLICATION_BEFORE_WINDOW()\n");

	int yes = IDYES;

    if (!WOMA::renderOnce)
        yes = WomaMessageBox(TEXT("Memory leak done on Purpose for this Tutorial!\nCheck Visual Studio Output Console log for more info!\ndo a double click on windows console."), TEXT("WOMA Tutorial 001:"), true/*YesOrNo*/);

	if (yes == IDYES)
		UINT* p = NEW UINT[1];
	//free(p);
    return false; //false means exit now and don't enter on main loop

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
	IF_NOT_RETURN_FALSE(APPLICATION_BEFORE_WINDOW());
#if defined USE_SYSTEM_CHECK                                // BEFORE: APPLICATION_INIT_MAIN_WINDOW()
	IF_NOT_RETURN_FALSE(SystemClass::SystemCheck());		// SYSTEM INFO: HW (OS, CPU, RAM, DiskFreeSpace, CPUFeatures) 
#endif
    IF_NOT_RETURN_FALSE(APPLICATION_AFTER_WINDOW());
#if defined USE_PROCESS_OS_KEYS
	IF_NOT_RETURN_FALSE(InitOsInput());						// INIT-INPUT Devices, NOTE: AFTER: APPLICATION_INIT_MAIN_WINDOW()
#endif

// ########################################### LOAD DRIVERS ###########################################
 // ######################################### INIT SELECTED DRIVER ###################################

#if defined USE_SYSTEM_CHECK
	InitializeSystemScreenF1(10, 10); // SETUP SCREENs: F1,F2,F3,F4,F5,F6 (RUNNING NOW ON: PaintSetup())
#endif

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

	//DEBUG:
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// There is any OS messages to handle?
		{
		#if !defined USE_DIRECT_INPUT
			TranslateMessage(&msg); // TranslateMessage produces WM_CHAR messages only for keys that are mapped to ASCII characters by the keyboard driver.
		#endif
			DispatchMessage(&msg);  // Process MSGs: INVOKE: WOMA_PAINT_Message_event_handler(...)
		}

	} while (msg.message != WM_QUIT && WOMA::main_loop_state >= 0);

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


