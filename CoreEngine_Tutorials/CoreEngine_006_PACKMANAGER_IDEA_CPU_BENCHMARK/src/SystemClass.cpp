// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: Systemclass.cpp
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
// PURPOSE: Define APIs for systemclass.cpp which is the common OS API
//
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "SystemClass.h"
#include "winSystemClass.h"
#include "default_settings_xml.h"
#include "OSengine.h"
#include "woma_macros.h"

#include "language.h"
#include "mem_leak.h"
#include "OSmain_dir.h"

#include "stateMachine.h"

#include "systemManager.h"

#include "xml_loader.h"

#include "ApplicationClass.h"
#include "fpsclass.h"

TCHAR* getComputerName()
{
	static TCHAR buf[MAX_STR_LEN];
	DWORD dwCompNameLen = MAX_STR_LEN;

	if (GetComputerName(buf, &dwCompNameLen))
		return buf;
	else
		return TEXT("");
}

TCHAR* getUserName()
{
	static TCHAR userName[256];

	//v1
	_tcscpy_s(userName, _tgetenv(TEXT("USERNAME")));

	//v2
	if (_tcslen(userName) == 0)
	{
		DWORD Size = sizeof(userName);
		GetUserName(userName, &Size);
	}

	return userName;
}


SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567890;

	AppSettings = NULL;

	if (WOMA::game_state < ENGINE_RESTART)
		WOMA::game_state = GAME_LOADING;

	TCHAR COMPILER[MAX_STR_LEN];
#if defined(__BORLANDC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("Borland C %d.%d\n"), __BORLANDC__ / 100, __BORLANDC__ % 100);
#elif defined(__WATCOMC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("WATCOMC %d.%d\n"), __WATCOMC__ / 100, __WATCOMC__ % 100);
#elif defined(__MINGW32__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("MinGW32 %d.%02d"), __MINGW32_VERSION_MAJOR, __MINGW32_VERSION_MINOR);
#elif defined(__MINGW64__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("MinGW64 %d.%02d"), __MINGW64_VERSION_MAJOR, __MINGW64_VERSION_MINOR);
#elif defined (__INTEL_COMPILER)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("Intel C++  %d.%02d"), __INTEL_COMPILER / 100, __INTEL_COMPILER % 100);
#elif defined  (_MSC_VER)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("Visual C++ %d.%02d"), _MSC_VER / 100, _MSC_VER % 100);
#elif defined  (__GNUC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("GCC %s.%s.%s"), GET_VERSION(__GNUC__), GET_VERSION(__GNUC_MINOR__), GET_VERSION(__GNUC_PATCHLEVEL__));
#endif	

	// APP_NAME with Binary Info:
	// -------------------------------------------------------------------------------------------
	TCHAR Wbuffer[MAX_STR_LEN] = { 0 };
	atow(Wbuffer, VER_PRODUCTVERSION_STR, MAX_STR_LEN); /*VER_PRODUCTVERSION_STRING_FOUR_PARTS*/

	StringCchPrintf(WOMA::APP_NAME, MAX_STR_LEN,
		TEXT("%s v%c%c%c%c.%c%c.%c%c COMPILED:%s OS:%s %s BUILD:%s BIN:%dbit %s CHAR:%s coreLvl:0000"),
		WOMA::APP_PROJECT_NAME,
		BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
		BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1,
		//1
		COMPILER,
		//2
#if defined WIN_XP
		TEXT("WinXP")
#elif defined WIN10
		TEXT("Win10")
#else
		TEXT("Windows")
#endif
		,
		// https://en.wikipedia.org/wiki/Ver_(command)
		//VER_PRODUCTBUILD
		Wbuffer
		,
		//3
#ifdef _DEBUG
		TEXT("Debug")
#else
		TEXT("Release")
#endif
		,
		//4
#ifdef X64
		64
#else
		32
#endif
		,
		//5
#ifdef __AVX2__
		TEXT("AVX2")	/*64bits*/
#elif defined ( __AVX__ )
		TEXT("AVX")	/*64bits*/
#elif (defined(_M_AMD64) || defined(_M_X64))
		TEXT("SSE2")	/*64bits*/
#elif _M_IX86_FP == 2
		TEXT("SSE2")	/*32bits*/
#elif _M_IX86_FP == 1
		TEXT("SSE")	/*32bits*/
#else
		TEXT("IA32")	/*32bits*/
#endif
		,
		//6
#ifdef UNICODE
		TEXT("Unicode")
#else
		TEXT("Ansi")
#endif
		//,
		//TEXT(__DATE__)
	);

	// Get ENGINE LEVEL
	ItoA(CORE_ENGINE_LEVEL, (WOMA::APP_NAME + (int)((STRING)WOMA::APP_NAME).find(TEXT("0000"))), 10); /* Base: Decimal Numbers (base 10) Set the number of "Engine Level" on title*/\

		// Log Title:
		StringCchPrintf(WOMA::APP_FULLNAME, sizeof(WOMA::APP_FULLNAME), TEXT("%s"), WOMA::APP_NAME);

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)WOMA::APP_FULLNAME);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("\n-------------------------------------------------------------------------------\n"));

	// Reset Vars:
	// -------------------------------------------------------------------------------------------
	m_OsInput = NULL;

	WOMA::filename = GetCommandLine();

	systemManager = NULL;
	userName = TEXT("");
	ComputerName = TEXT("");
	fps = NULL;
	cpu = NULL;

	XML_SETTINGS_FILE = TEXT("");

	m_Application = NEW ApplicationClass;	// Create the User Level Class
}

	#ifndef DIK_ESCAPE					// Will be defined @ ENGINE_LEVEL >= 24
	#define DIK_ESCAPE 0x01
	#endif

//-----------------------------------------------------------------------------------------
void SystemClass::ProcessOSInput() // This Function will be invoked several times per second
//-----------------------------------------------------------------------------------------
{

	static bool first_time =
		true;

	// Process Special: "ESC" key is beeing pressed ?
	if ((WOMA::game_state > GAME_MINIMIZED) && (OS_KEY_DOWN(DIK_ESCAPE + 0x35)))
		Publish_Quit_Message();			// -> EXIT APPLICATION
	// Windows OS way... for keys (until direct input)!
	if (m_OsInput->IsKeyDown(VK_ESCAPE) && WOMA::game_state == GAME_RUN)		// CHECK: if the user pressed 'escape' and wants to exit the application.
		Publish_Quit_Message();
	if (m_OsInput->IsKeyDown(VK_ESCAPE) && WOMA::game_state >= GAME_SYSTEM_SETTINGS && WOMA::game_state <= GAME_SETUP) {		// CHECK: if the user pressed 'escape' and wants to exit the application.
		WOMA::game_state = GAME_MENU;
		SAFE_SHUTDOWN(SystemHandle->womaSetup);
		m_OsInput->m_keys[VK_ESCAPE] = false;
		WOMA::game_state = GAME_RUN;
	}

	if (first_time || (OS_KEY_DOWN(VK_F6) && WOMA::game_state != GAME_SETUP))
	{
		WOMA::game_state = GAME_SETUP;
		OS_REDRAW_WINDOW;
		// Toggle the full screen/window mode

		if (SystemHandle->AppSettings->FULL_SCREEN)
		{
			//if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
			SystemHandle->AppSettings->FULL_SCREEN = false;
			CHAR str[MAX_STR_LEN] = { 0 }; wtoa(str, (TCHAR*)SystemHandle->XML_SETTINGS_FILE.c_str(), MAX_STR_LEN); // wchar ==> char
			saveConfigSettings(str);
			/*
			// activate the window
			SetActiveWindow(SystemHandle->m_hWnd);
			ushort action = (ushort)WM_SYSKEYDOWN; //ALT
			ushort key = (ushort)0xd; //ENTER
			uint lparam = (0x01 << 28);
			SendMessage(SystemHandle->m_hWnd, action, key, lparam);
			*/
			WOMA::previous_game_state = WOMA::game_state;
			WOMA::game_state = ENGINE_RESTART;
		}
	}

	first_time = false;

#if CORE_ENGINE_LEVEL >= 4 && defined GAME_SYSTEM_SETTINGS
	if (first_time || (OS_KEY_DOWN(VK_F1) && WOMA::game_state != GAME_SYSTEM_SETTINGS)) {
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
		WOMA::game_state = GAME_SYSTEM_SETTINGS;
		OS_REDRAW_WINDOW;
	}
	first_time = false;
#endif

	if (first_time || (OS_KEY_DOWN(VK_F5) && WOMA::game_state != GAME_RUN)) {
		WOMA::game_state = GAME_RUN;
		OS_REDRAW_WINDOW;
	}
	first_time = false;


}

bool SystemClass::SystemCheck()
{
	// [8] Get User Language/Country:
	// -------------------------------------------------------------------------------------------
	WOMA::settings.id = GetUserDefaultUILanguage();

	//TODO: Use this later on:
	WOMA::GetLangStringFromLangId(WOMA::settings.id);

	// [10] Check Endian = LITTLE_ENDIAN or BIG_ENDIAN  (Used in some libs)
	// -------------------------------------------------------------------------------------------
	WOMA::settings.Endian = WOMA::endian();
	if (WOMA::settings.Endian == LITTLE_ENDIAN)
		WOMA_LOGManager_DebugMSGAUTO(TEXT("The machine is Little Endian\n"));	//8008, 8080, 8085, 8086, ...
	else
		WOMA_LOGManager_DebugMSGAUTO(TEXT("The machine is Big Endian\n"));		//Motorola 68000

	WOMA_LOGManager_DebugMSGAUTO(TEXT("\n"));

	LEVELHIGHLIGHT(4);
	// INIT SYSTEM SETTINGS:
	WOMA_LOGManager_DebugMSG("------------------------------------SYSTEM CHECK SETTINGS: --------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] get_current_dir()\n"), WOMA::ENGINE_LEVEL_USED);

	userName = getUserName(); // Note: Save for later use!
	WOMA_LOGManager_DebugMSGAUTO(TEXT("User Name: %s\n"), userName.c_str());

	ComputerName = getComputerName();
	WOMA_LOGManager_DebugMSGAUTO(TEXT("Computer Name: %s\n"), ComputerName.c_str());

	IF_NOT_RETURN_FALSE(WOMA::getCurrentDir());
	//LEVELNORMAL();

	systemManager = NEW SystemManager();
	IF_NOT_THROW_EXCEPTION(systemManager);

	GETOS();

	// [2] CheckOS: Detect OS Version & DO System Check: DONE
	//----------------------------------------------------------------------------
	//LEVELHIGHLIGHT(4);
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->CheckOS()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->CheckOS()); // Mandatory line for all ENGINE_LEVELs

	//LEVELNORMAL();

	//LEVELHIGHLIGHT(4);
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkCPU()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->checkCPU());
	//LEVELNORMAL();

	//LEVELHIGHLIGHT(5);
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkRAM()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->checkRAM());
	//LEVELNORMAL();

	//LEVELHIGHLIGHT(6);
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkDiskFreeSpace()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->checkDiskFreeSpace());
	//LEVELNORMAL();

	//LEVELHIGHLIGHT(7);
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkCPUFeatures()\n"), WOMA::ENGINE_LEVEL_USED);

	IF_NOT_RETURN_FALSE(systemManager->checkCPUFeatures());
	//LEVELNORMAL();

	if (Command == 0) {
		//LEVELHIGHLIGHT(8);
		WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
		WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkBenchMarkSpeed()\n"), WOMA::ENGINE_LEVEL_USED);
		IF_NOT_RETURN_FALSE(systemManager->checkBenchMarkSpeed(&m_Timer));
		//LEVELNORMAL();
	}

	WOMA_LOGManager_DebugMSG("\n");
	//#endif

	return true;
}


SystemClass::~SystemClass() { CLASSDELETE(); }

void SystemClass::Shutdown()
{
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("SystemClass::Shutdown()\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	SAFE_DELETE(m_OsInput);

	if (m_Application)
		m_Application->WOMA_APPLICATION_Shutdown();
	SAFE_SHUTDOWN(m_Application);					// Release ALL graphics object.

	SAFE_DELETE(systemManager);

	AppSettings = NULL;				// Pointer to Static object, no need to free.
}

void SystemClass::FrameUpdate()
{

	ProcessOSInput();

	ProcessPerformanceStats();	// ProcessPerformanceStats: m_Timer.Frame(); m_Fps.Frame(); m_Cpu.Frame();
}

bool SystemClass::LoadXmlSettings()
{
	WOMA_LOGManager_DebugMSG("===============================================================================\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("LOAD XML SETTINGS\n"));
	WOMA_LOGManager_DebugMSG("===============================================================================\n");

	// Load and Parse XML [settings.xml] the Configuration file
	//----------------------------------------------------------------------------
	XML_SETTINGS_FILE = WOMA::PUBLIC_DOCUMENTS;
	XML_SETTINGS_FILE.append(WOMA::APP_SETTINGS_FILE);

	WOMA_LOGManager_DebugMSGAUTO(TEXT("XML_SETTINGS_FILE: %s\n"), XML_SETTINGS_FILE.c_str());
	if (!initAppicationSettings((TCHAR*)XML_SETTINGS_FILE.c_str()))
	{
		STRING err = TEXT("File not found/Invalid: "); err += XML_SETTINGS_FILE;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "));
		return false;
	}

	return true;
}

void SystemClass::ProcessPerformanceStats() // Run every frame
{
	//if (!m_Application) return;

	// Update the system stats: (BEFORE: HandleUserInput)
	m_Timer.Frame();		// Calculate dT for animations (Measure last frame time)
	m_Fps.Frame();			// Increase the frame counter, calculate FPS once per second
	fps = m_Fps.GetFps();	// Get current FPS (updated by "m_Fps.Frame()" every second)

#if defined WINDOWS_PLATFORM && !defined WIN_XP
	m_Cpu.Frame();			// Collect CPU usage percentage once per second

	m_Application->dt = m_Timer.GetTime();		// Calculate dT for animations & camera movements (in Mili Seconds)
#endif

#if defined WINDOWS_PLATFORM && !defined WIN_XP
	cpu = m_Cpu.GetCpuPercentage(); // Get current CPU use  (updated by "m_Cpu.Frame()" every second)
#endif
}


