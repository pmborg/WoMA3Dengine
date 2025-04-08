// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: SystemClass.cpp
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
// PURPOSE: Define APIs for systemclass.cpp which is the common OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "SystemClass.h"
#include "OSengine.h" //#include "WinSystemClass.h"
#include "default_settings_xml.h"
#include "woma_macros.h"
#include "mem_leak.h"

#include "OSmain_dir.h"

#if defined WINDOWS_PLATFORM
#include "language.h"
#endif

#include "stateMachine.h"

#if CORE_ENGINE_LEVEL >= 4 && defined USE_SYSTEM_CHECK
#include "systemManager.h"
#endif

#if CORE_ENGINE_LEVEL >= 5 && defined WINDOWS_PLATFORM && defined USE_TINYXML_LOADER
#include "xml_loader.h"
#endif

#if CORE_ENGINE_LEVEL >= 6 && defined USE_TIMER
#include "ApplicationClass.h"	// Calculate dT for animations
#include "fpsClass.h"			// Calculate FPS
#endif

#if DX_ENGINE_LEVEL >= 19 && !defined NewWomaEngine
#if defined DX9sdk
#include "Dx9Class.h"
#endif
#if defined DX11 || defined DX9
#include "Dx11Class.h"
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif
#if (defined OPENGL3 || defined OPENGL4)
#include "WomaDriverClass.h"	//woma
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif

#if D3D11_SPEC_DATE_YEAR == 2009
#include <D3dx9core.h>		//D3DX_SDK_VERSION (Checks for the existance of the correct D3DX library version)
#endif

#include "Math3D.h"
#endif

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

//----------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 19 && !defined NewWomaEngine
bool SystemClass::LoadAllGraphicAssets()
{
#if defined USE_LOADING_THREADS
	m_Cpu.SetProcessorAffinity(0);  //Use CPU N.0 to Load
#endif

	SystemHandle->m_Application->scaleX = SystemHandle->AppSettings->WINDOW_WIDTH / 1920.0f;
	SystemHandle->m_Application->scaleY = SystemHandle->AppSettings->WINDOW_HEIGHT / 1080.0f;
	SystemHandle->m_Application->rescale = min(SystemHandle->m_Application->scaleX, SystemHandle->m_Application->scaleY);

	//################################ LOAD ALL INITIAL 3D OBJECTS ##################################
	// Load all assets that will be rendered@ 1st Frame and START TIMER
	if (!m_Application->Initialize(m_Driver)) {
		WOMA::main_loop_state = -1; 
		WOMA::game_state = GAME_STOP;
		return false;
	}

	//Shutdown VIDEO PLAYER:
#if defined USE_INTRO_VIDEO_DEMO
	SAFE_DELETE(g_DShowPlayer);
#endif

	return true;
}
#endif

//----------------------------------------------------------------------------
SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567222;

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
#elif defined  (ANDROID_PLATFORM)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("TODO %s.%s"), GET_VERSION(__GNUC__), GET_VERSION(__GNUC_MINOR__));
#elif defined  (__GNUC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("GCC %s.%s.%s"), GET_VERSION(__GNUC__), GET_VERSION(__GNUC_MINOR__), GET_VERSION(__GNUC_PATCHLEVEL__));
#endif	

	// APP_NAME with Binary Info:
	// -------------------------------------------------------------------------------------------
	TCHAR Wbuffer[MAX_STR_LEN] = { 0 };
#if defined WINDOWS_PLATFORM
	atow(Wbuffer, VER_PRODUCTVERSION_STR, MAX_STR_LEN); /*VER_PRODUCTVERSION_STRING_FOUR_PARTS*/
#endif

	StringCchPrintf(WOMA::APP_NAME, MAX_STR_LEN,
		TEXT("%s v%c%c%c%c.%c%c.%c%c BIN:%s OS:%s %s BUILD:%s BIN:%dbit %s CHAR:%s Lvl: %d"),
		PROJECT_NAME,
		BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
		BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1,
		//1
		COMPILER,
		//2
#if defined WIN_XP
		TEXT("WinXP")
#elif defined WIN10
		TEXT("Win10/11") //SystemHandle->systemManager->pszOS
#elif WINVER == _WIN32_WINNT_WIN7
		TEXT("WIN7")
#elif defined WINDOWS_PLATFORM
		TEXT("Windows")
#elif defined CYGWIN_PLATFORM
		TEXT("Cygwin")
#elif defined LINUX_PLATFORM
		TEXT("Linux")
#elif defined ANDROID_PLATFORM
	#if defined(__ARM_ARCH_2__)
			TEXT("Android-ARM2")
	#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
			TEXT("Android-ARM3")
	#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
			TEXT("Android-ARM4T")
	#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
			TEXT("Android-ARM5"
	#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
			TEXT("Android-ARM6T2")
	#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
			TEXT("Android-ARM6")
	#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7")
	#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7A")
	#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7R")
	#elif defined(__ARM_ARCH_7M__)
			TEXT("Android-ARM7M")
	#elif defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7S")
	#elif defined(__aarch64__) || defined(_M_ARM64)
			TEXT("Android-ARM64")
	#endif
#endif
		,
#if defined WINDOWS_PLATFORM
		// https://en.wikipedia.org/wiki/Ver_(command)
		//VER_PRODUCTBUILD
		Wbuffer
#else
		TEXT("")
#endif
		,
		//3
#if defined _DEBUG || defined DEBUG
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
#if defined WINDOWS_PLATFORM
	#ifdef __AVX512F__
			TEXT("AVX512")	/*64bits*/
	#elif __AVX2__
			TEXT("AVX2")	/*64bits*/
	#elif defined ( __AVX__ )
			TEXT("AVX")		/*64bits*/
	#elif (defined(_M_AMD64) || defined(_M_X64))
			TEXT("SSE2")	/*64bits*/
	#elif _M_IX86_FP == 2
			TEXT("SSE2")	/*32bits*/
	#elif _M_IX86_FP == 1
			TEXT("SSE")		/*32bits*/
	#else
			TEXT("IA32")	/*32bits*/
	#endif
#else
		TEXT("")
#endif
		,
		//6
#ifdef UNICODE
		TEXT("Unicode")
#else
		TEXT("Ansi")
#endif
		,
		LEVEL
		//TEXT(__DATE__)
	);
	
	// Log Title:
	StringCchPrintf(WOMA::APP_FULLNAME, sizeof(WOMA::APP_FULLNAME), TEXT("%s"), WOMA::APP_NAME);

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)WOMA::APP_FULLNAME);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("\n-------------------------------------------------------------------------------\n"));

	// Reset Vars:
	// -------------------------------------------------------------------------------------------
#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
	m_OsInput = NULL;
#endif


#if defined WINDOWS_PLATFORM
	WOMA::filename = GetCommandLine();
#else
	WOMA::filename = TEXT("");
#endif

#if defined USE_SYSTEM_CHECK
	systemManager = NULL;
	userName = TEXT("");
	ComputerName = TEXT("");
#endif
#if defined USE_TIMER_CLASS
	fps = NULL;
	cpu = NULL;
#endif
#if defined USE_TINYXML_LOADER
	XML_SETTINGS_FILE = TEXT("");
#endif

	m_Application = NEW ApplicationClass;	// Create the User Level Class
}


#if CORE_ENGINE_LEVEL >= 4 && defined USE_SYSTEM_CHECK
void SystemClass::InitializeSystemScreen(int x, int y)
//-----------------------------------------------------------------------------------------
{
	WOMA::logManager->DEBUG_MSG("InitializeSystemScreen...\n");
	//v1
	//float LINE = 24;
	//float LINE_SPACE=45;
	//v2
	float LINE = 22;
	float LINE_SPACE = 40;
	if (fontSizeY > 0) {
		LINE = MIN(LINE, 2 * fontSizeY);
		LINE_SPACE = MIN(LINE_SPACE, 3 * fontSizeY);
	}

	// Make sure that is Reset!
	while (!TextToPrint[0].empty())
		TextToPrint[0].pop_back();

	// ----------------------------------
	// Language
	Woma_Label text = { systemDefinitions.szCountryNameBuffer, x, y };
	TextToPrint[0].push_back(text);

	// Username
	TCHAR szScratch[128] = { 0 };
	StringCchPrintf(szScratch, sizeof(szScratch), TEXT("User Name: %s"), userName.c_str());
	text.y += (int)LINE;
	text.label = szScratch;
	TextToPrint[0].push_back(text);

	// Computer name
	StringCchPrintf(szScratch, sizeof(szScratch), TEXT("Computer Name: %s"), ComputerName.c_str());
	text.y += (int)LINE;
	text.label = szScratch;
	TextToPrint[0].push_back(text);

	// ----------------------------------
	// OS
	text.y += (int)LINE_SPACE; text.label = systemDefinitions.osName;
	TextToPrint[0].push_back(text);

#ifdef WINDOWS_PLATFORM
	text.y += (int)LINE; text.label = systemDefinitions.windowsVersion;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.windowsBuildVersion;
	TextToPrint[0].push_back(text);
#endif

	// ----------------------------------
	// System:
	text.y += (int)LINE_SPACE; text.label = systemDefinitions.platform;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.characterSet;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.binaryArchitecture;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.binaryCode;
	TextToPrint[0].push_back(text);

	// ----------------------------------
	// Processor
	text.y += (int)LINE_SPACE; text.label = systemDefinitions.processorPackageCount;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.NumCoreProcessors;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.logicalProcessorCount;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.clockSpeed;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.processorName;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.processorId;
	TextToPrint[0].push_back(text);

	int HALF;
	// ----------------------------------
	// NEW PAGE
	// ----------------------------------
	// BOARD/CPU Feactures (RIGHT SIDE):
#if defined WINDOWS_PLATFORM
	if (AppSettings->WINDOW_WIDTH == 0)
	{
		// --------------------------------------------------------------------------------------------
		DEVMODE devMode = { 0 };
		DWORD deviceNum = 0;
		UINT MONITOR_NUM = 0;

		displayDevice.cb = sizeof(DISPLAY_DEVICE);
		while (EnumDisplayDevices(NULL, deviceNum, &displayDevice, 0))	// Get deviceNum
		{
			// Get our Screen name (on THIS monitor)
			if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
			{
				// Use the Monitor selected by user:
				if (((deviceNum == AppSettings->UI_MONITOR) && (AppSettings->UseAllMonitors == false)) ||
					((deviceNum == MONITOR_NUM) && (AppSettings->UseAllMonitors == true)))
				{
					HALF = (devMode.dmPelsWidth / 5) * 3;
					break;
				}
			}
		}
	}
	else
#endif
		HALF = (AppSettings->WINDOW_WIDTH / 5) * 3;

	// ----------------------------------
	// CPU FEATURES:
	text.y += (int)LINE_SPACE;
	int initial_y = text.y;
	text.label = TEXT("CPU FEATURES:");
	TextToPrint[0].push_back(text);

	for (UINT i = 0; i < systemDefinitions.cpuFeactures.size(); i++)
	{
		if (i == 10)
		{
			text.y = initial_y;
			text.x = HALF;
		}
		text.y += (int)LINE;
		text.label = systemDefinitions.cpuFeactures[i];
		TextToPrint[0].push_back(text);
	}

#if defined DX_ENGINE
	// GPU:
	text.x = x;
	for (UINT i = 0; i < systemDefinitions.GPUINFO.size(); i++)
	{
		text.y += (int)LINE_SPACE;
		text.label = systemDefinitions.GPUINFO[i].GraphicCard;
		TextToPrint[0].push_back(text);
		/*
		if (i + 1 < systemDefinitions.GPUINFO.size())
		{
			text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].AdapterDACType;
			TextToPrint[0].push_back(text);
			text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].AdapterRAM;
			TextToPrint[0].push_back(text);
		}
		*/
		text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].DedicatedVideoMemory;
		TextToPrint[0].push_back(text);
		text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].DedicatedSystemMemory;
		TextToPrint[0].push_back(text);
		text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].SharedSystemMemory;
		TextToPrint[0].push_back(text);
	}

	text.x = HALF;
	text.y = 10;
	// ----------------------------------
	// RAM
	text.label = systemDefinitions.totalMemoryCapacity;
	TextToPrint[0].push_back(text);
	text.y += (int)LINE; text.label = systemDefinitions.freeMemory;
	TextToPrint[0].push_back(text);

	// BenchMark MathSpeed
	text.y += (int)LINE; text.label = systemDefinitions.benchMarkMathSpeed1;
	TextToPrint[0].push_back(text);
	text.y += (int)LINE; text.label = systemDefinitions.benchMarkMathSpeed2;
	TextToPrint[0].push_back(text);

	// FreeSpace:
	text.y += (int)LINE_SPACE; text.label = TEXT("DISK FREE:");
	TextToPrint[0].push_back(text);

	for (UINT driveLetter = 0; driveLetter < systemDefinitions.drives_List.size(); driveLetter++)
	{
		text.y += (int)LINE; text.label = systemDefinitions.drives_List[driveLetter];
		TextToPrint[0].push_back(text);
	}

#endif
	//WOMA::logManager->DEBUG_MSG(" done\n");
}
#endif



//-----------------------------------------------------------------------------
void SystemClass::refreshTitle() // Run once per second.
{
#if CORE_ENGINE_LEVEL >= 10 && !defined NewWomaEngine
	if (driverList.size() == 0)
		return;
	if (!m_Driver)
		return;
#endif

#if DX_ENGINE_LEVEL < 19 || LEVEL <10 || defined NewWomaEngine
	StringCchPrintf(pstrFPS, 300, TEXT("FPS:%d %s "), SystemHandle->fps, WOMA::APP_FULLNAME);
#else
#if defined USE_ASTRO_CLASS
	//(astroClass == NULL) ? 0 : astroClass->hour, (astroClass == NULL) ? 0 : astroClass->minute,
	StringCchPrintf(pstrFPS, 300, TEXT("FPS:%d [%s] %s shader:%s state:%d PAGE: %d - TOTAL VERTEX: %d"), SystemHandle->fps,
		m_Driver->driverName, WOMA::APP_FULLNAME,
		m_Driver->szShaderModel, WOMA::game_state, RENDER_PAGE, SystemHandle->TotalVertexCounter);
#else
	StringCchPrintf(pstrFPS, 300, TEXT("FPS:%d PAGE: %d %s [%s] shader:%s state:%d - TOTAL VERTEX: %d"), 
		SystemHandle->fps, RENDER_PAGE, WOMA::APP_FULLNAME, m_Driver->driverName, m_Driver->szShaderModel, WOMA::game_state, SystemHandle->TotalVertexCounter);
#endif
#endif

#if defined WINDOWS_PLATFORM && !defined ANDROID_PLATFORM
#if defined(X64) // Set the new "Window Title"
	PDWORD_PTR dwResult = 0;// In 64 Bits
	for (int i = 0; i < SystemHandle->windowsArray.size(); i++)
		SendMessageTimeout(SystemHandle->windowsArray[i].hWnd, WM_SETTEXT, 0, (LPARAM)pstrFPS, SMTO_ABORTIFHUNG, 1000, dwResult);
#else				
	DWORD dwResult = 0;		// In 32 Bits
	for (int i = 0; i < SystemHandle->windowsArray.size(); i++)
		SendMessageTimeout(SystemHandle->windowsArray[i].hWnd, WM_SETTEXT, 0, (LPARAM)pstrFPS, SMTO_ABORTIFHUNG, 1000, &dwResult);
#endif
#endif
}

#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
	#ifndef DIK_ESCAPE					// Will be defined @ ENGINE_LEVEL >= 24
	#define DIK_ESCAPE 0x01
	#endif

//-----------------------------------------------------------------------------------------
void SystemClass::ProcessOSInput() // This Function will be invoked several times per second
//-----------------------------------------------------------------------------------------
{
	//LEVEL 4 System
	//LEVEL 5 Setup
	//LEVEL 7 Astro
	//LEVEL 8 Map
	//LEVEL 9 Weather

	static bool first_time = false;

#if defined WINDOWS_PLATFORM

	// "ESC": DX Process Special: key is beeing pressed ? -> EXIT APPLICATION
#if CORE_ENGINE_LEVEL >= 10 && defined USE_DIRECT_INPUT
	if ((WOMA::game_state > GAME_MINIMIZED && WOMA::game_state < GAME_MAP) && (OS_KEY_DOWN(DIK_ESCAPE + 0x35)))
	{
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
		return;
	}
#endif

	// "ESC" OS Process Special: key is beeing pressed ? -> EXIT APPLICATION
	if (m_OsInput->IsKeyDown(VK_ESCAPE) && WOMA::game_state == GAME_RUN)		// CHECK: if the user pressed 'escape' and wants to exit the application.
	{
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
		return;
	}

	if (m_OsInput->IsKeyDown(VK_ESCAPE) && WOMA::game_state >= GAME_SYSTEM_SETTINGS && WOMA::game_state <= GAME_SETUP) {		// CHECK: if the user pressed 'escape' and wants to exit the application.
		//WOMA::game_state = GAME_MENU;
		#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
		SAFE_SHUTDOWN(SystemHandle->womaSetup);
		#endif
		m_OsInput->m_keys[VK_ESCAPE] = false;
		WOMA::game_state = GAME_RUN;
	}

	//F4
#if CORE_ENGINE_LEVEL >= 9 && defined USE_METARCLASS
	if (first_time || (OS_KEY_DOWN(VK_F4) && WOMA::game_state != GAME_WEATHER_INFO)) {
	#if defined CLIENT_SCENE_SETUP
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
	#endif
		if (AppSettings->DRIVER == DRIVER_DX12 && !first_time)
		{
			WOMA::previous_game_state = GAME_WEATHER_INFO; //match*
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
		RENDER_PAGE = 9;
		WOMA::game_state = GAME_WEATHER_INFO; //match*
		OS_REDRAW_WINDOW;
	}
	first_time = false;
#endif

	//F3
#if CORE_ENGINE_LEVEL >= 8 && defined USE_GPS_MAP
	if (first_time || (OS_KEY_DOWN(VK_F3) && WOMA::game_state != GAME_SHOW_POSITION)) {
	#if defined CLIENT_SCENE_SETUP
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
	#endif
		if (AppSettings->DRIVER == DRIVER_DX12 && !first_time)
		{
			WOMA::previous_game_state = GAME_SHOW_POSITION; //match*
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
		RENDER_PAGE = 8;
		WOMA::game_state = GAME_SHOW_POSITION; //match*
		OS_REDRAW_WINDOW;
	}
	first_time = false;
#endif
	//F2
#if CORE_ENGINE_LEVEL >= 7 && defined USE_ASTRO_CLASS
	if (first_time || (OS_KEY_DOWN(VK_F2) && WOMA::game_state != GAME_CELESTIAL_INFO)) {
	#if defined CLIENT_SCENE_SETUP
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
	#endif
		if (AppSettings->DRIVER == DRIVER_DX12 && !first_time)
		{
			WOMA::previous_game_state = GAME_CELESTIAL_INFO; //match*
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
		RENDER_PAGE = 7;
		WOMA::game_state = GAME_CELESTIAL_INFO; //match*
		OS_REDRAW_WINDOW;
	}
	first_time = false;
#endif

	//F6
#if defined CLIENT_SCENE_SETUP
	if (first_time || (OS_KEY_DOWN(VK_F6) && WOMA::game_state != GAME_SETUP))
	{
		WOMA::game_state = GAME_SETUP;
		OS_REDRAW_WINDOW;
		// Toggle the full screen/window mode

		if (SystemHandle->AppSettings->FULL_SCREEN)
		{
			SystemHandle->AppSettings->FULL_SCREEN = false;
			CHAR str[MAX_STR_LEN] = { 0 }; wtoa(str, (TCHAR*)SystemHandle->XML_SETTINGS_FILE.c_str(), MAX_STR_LEN); // wchar ==> char
			#if defined CLIENT_SCENE_SETUP
			SystemHandle->xml_loader.saveConfigSettings(str);
			#endif

			RENDER_PAGE = 5;
			WOMA::previous_game_state = WOMA::game_state;
			WOMA::game_state = ENGINE_RESTART;
		}
	}

	first_time = false;
#endif

	//F1
#if CORE_ENGINE_LEVEL >= 4 && defined GAME_SYSTEM_SETTINGS
	if (first_time || (OS_KEY_DOWN(VK_F1) && WOMA::game_state != GAME_SYSTEM_SETTINGS)) {
	  #if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
	  #endif
		if (AppSettings->DRIVER == DRIVER_DX12 && !first_time)
		{
			WOMA::previous_game_state = GAME_SYSTEM_SETTINGS; //match*
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
		RENDER_PAGE = 4;
		WOMA::game_state = GAME_SYSTEM_SETTINGS; //match*
		OS_REDRAW_WINDOW;
		OS_REDRAW_WINDOW;
	}
	first_time = false;
#endif

	//F5
#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
	if (first_time || (OS_KEY_DOWN(VK_F5) && WOMA::game_state != GAME_RUN)) {
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
		if (OS_KEY_DOWN(VK_F5)) {
			RENDER_PAGE = LEVEL;
			WOMA::game_state = GAME_RUN;
			OS_REDRAW_WINDOW;
		}
	}
	first_time = false;
#endif

#endif
}
#endif

#if defined USE_SYSTEM_CHECK
bool SystemClass::SystemCheck()
{
	// [8] Get User Language/Country:
	// -------------------------------------------------------------------------------------------
#if defined WINDOWS_PLATFORM
	WOMA::settings.id = GetUserDefaultUILanguage();

	//TODO: Use this later on:
	WOMA::GetLangStringFromLangId(WOMA::settings.id);
#endif

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

#if CORE_ENGINE_LEVEL >= 4 && defined WINDOWS_PLATFORM
	GETOS();
#endif

	// [2] CheckOS: Detect OS Version & DO System Check: DONE
	//----------------------------------------------------------------------------
	//LEVELHIGHLIGHT(4);
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->CheckOS()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->CheckOS()); // Mandatory line for all ENGINE_LEVELs

#if defined DX11 && !defined WIN10
	// FOR WINDOWS VISTA:
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("[Function Loader] systemManager->UpdateOSifNeeded()\n"));
	IF_NOT_RETURN_FALSE(systemManager->UpdateOSifNeeded());	// TODO: UpdateOSifNeeded: Check if OS need Updates: DONE

	WOMA_LOGManager_DebugMSGAUTO(TEXT("[Function Loader] systemManager->CheckSetup()\n"));
	IF_NOT_RETURN_FALSE(systemManager->CheckSetup());		// TODO: CheckSetup: Check if "Setup.exe" is Installed or needed / Download & Install: DONE
#endif // NOTE: WINDOWS10/DX12: Dont need this checks
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

#if defined WINDOWS_PLATFORM
	IF_NOT_RETURN_FALSE(systemManager->checkCPUFeatures());
	//LEVELNORMAL();
#endif

	if (Command == 0) {
#if ((defined USE_TIMER_CLASS && CORE_ENGINE_LEVEL >= 6) || (defined RELEASE || defined INTRO_DEMO) || CORE_ENGINE_LEVEL == 4) && defined WINDOWS_PLATFORM
		//LEVELHIGHLIGHT(8);
		WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
		WOMA_LOGManager_DebugMSGAUTO(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkBenchMarkSpeed()\n"), WOMA::ENGINE_LEVEL_USED);
		IF_NOT_RETURN_FALSE(systemManager->checkBenchMarkSpeed(&m_Timer));
		//LEVELNORMAL();
#endif
	}

#if defined USE_CHECK_IO
	//LEVELHIGHLIGHT(9);
	IF_NOT_RETURN_FALSE(systemManager->CheckIO());
	//LEVELNORMAL();
#endif

#if defined USE_JOY && defined USE_DIRECT_INPUT
	InitJoyStick();
#endif

	WOMA_LOGManager_DebugMSG("\n");
	//#endif

	return true;
}
#endif


SystemClass::~SystemClass() { CLASSDELETE(); }

void SystemClass::Shutdown()
{
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("SystemClass::Shutdown()\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
#if defined USE_PROCESS_OS_KEYS
	SAFE_DELETE(m_OsInput);
#endif

	if (m_Application) {
		m_Application->WOMA_APPLICATION_Shutdown();
		m_Application->DEMO_WOMA_APPLICATION_Shutdown2D();
	}
	SAFE_SHUTDOWN(m_Application);					// Release ALL graphics object.

#if defined USE_SYSTEM_CHECK
	SAFE_DELETE(systemManager);
#endif

#if CORE_ENGINE_LEVEL >= 10 && !defined NewWomaEngine
	if (driverList.size() > 0) {
#ifdef OPENGL3
		if (driverList[DRIVER_GL3]) {
			delete ((GLopenGLclass*)driverList[DRIVER_GL3]);
			driverList[DRIVER_GL3] = NULL;
		}
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		if (driverList[DRIVER_DX12]) {
			delete ((DirectX::DX12Class*)driverList[DRIVER_DX12]);
			driverList[DRIVER_DX12] = NULL;
		}
#endif
#if defined DX11 //Pure DX11
		if (driverList[DRIVER_DX11]) {
			delete ((DirectX::DX11Class*)driverList[DRIVER_DX11]);
			driverList[DRIVER_DX11] = NULL;
		}
#endif
#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
		if (driverList[DRIVER_DX9]) {
			delete ((DirectX::DX11Class*)driverList[DRIVER_DX9]);
			driverList[DRIVER_DX9] = NULL;
		}
#elif defined DX9sdk
		if (driverList[DRIVER_DX9]) {
			delete ((DirectX::DX9Class*)driverList[DRIVER_DX9]);
			driverList[DRIVER_DX9] = NULL;
		}
#endif
	}
#endif

	AppSettings = NULL;				// Pointer to Static object, no need to free.
}
#if defined ANDROID_PLATFORM
extern android_app* app;
#endif

void SystemClass::FrameUpdate()
{
#if defined WINDOWS_PLATFORM && defined USE_DIRECT_INPUT
	if (DXsystemHandle->m_Input->m_mouseState.rgbButtons[MOUSE_LEFT] & 0x80)
	{
		POINT mousePos;

		GetCursorPos(&mousePos);
		ScreenToClient(SystemHandle->m_hWnd, &mousePos);

		#define mousex mousePos.x
		#define mousey mousePos.y
		//printf("mousex: %d mouseY: %d\n", mousex, mousey);
		if (mousex < 24 && mousey < 24)
		{
			RENDER_PAGE = 25;
			WOMA::previous_game_state = GAME_IMGUI;
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
	}
#endif

#if defined LINUX_PLATFORM
	if (WOMA::game_state == GAME_RUN)
	{
		/*
		int err = XGrabPointer(Win.display, Win.window,
								True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
								GrabModeAsync, GrabModeAsync,
								Win.window, None, CurrentTime);

		processXEvents(wm_protocols, wm_delete_window);
		XUngrabPointer(Win.display, CurrentTime);

		#define mousex event.xbutton.x_root 
		#define mousey event.xbutton.y_root
		//if (event.xbutton.button == Button1)
		//	_tprintf("mousex: %d mouseY: %d\n", (mousex) - WOMA::settings.WINDOW_Xpos, (mousey) -WOMA::settings.WINDOW_Ypos);
	
		if ((mousex < 100 && mousey < 100) && (mousex > 0 && mousey > 0))
		{
			RENDER_PAGE = 25;
			WOMA::previous_game_state = GAME_IMGUI;
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
		*/
	}
#endif

#if defined ANDROID_PLATFORM && !defined NewWomaEngine && defined USE_IMGUI
	if (WOMA::game_state == GAME_RUN)
	{
		struct womaengine* engine = (struct womaengine*)app->userData;

		#define mousex engine->state.x
		#define mousey engine->state.y
		//_tprintf("mousex: %d mouseY: %d\n", mousex, mousey);
		if ((mousex < 100 && mousey < 100) && (mousex > 0 && mousey > 0))
		{
			RENDER_PAGE = 25;
			WOMA::previous_game_state = GAME_IMGUI;
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
	}
#endif

	#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	  #if !defined ANDROID_PLATFORM
		DXsystemHandle->GetInputs();				// READ-INPUT: WinSystemClass::ProcessInput() + DXInputClass::Frame()
	  #endif
	  #if defined DX_ENGINE
		DXsystemHandle->m_Input->ProcessInput();	// PROCESS-INPUT/POSITION: (WINDOWS & DX) DXInputClass::ProcessInput()
	  #endif
	#endif

	#if defined USE_PROCESS_OS_KEYS && defined WINDOWS_PLATFORM
		ProcessOSInput();							// READ+PROCESS-OS-INPUT: Process Special: Function Keys |ESC and F1 to F6|
		if (WOMA::game_state == ENGINE_RESTART)
			return;
	#endif
	
	#if CORE_ENGINE_LEVEL >= 4 && defined USE_TIMER_CLASS
		ProcessPerformanceStats();					// ProcessPerformanceStats-FPS: m_Timer.Frame(); m_Fps.Frame(); m_Cpu.Frame();
	#endif
}

#if CORE_ENGINE_LEVEL >= 2 && (defined USE_PROCESS_OS_KEYS || defined INTRO_DEMO)
bool SystemClass::InitOsInput()
{
  #if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	g_NetID = (UINT)0; // Just Us on the world? So WE will be the index: zero...
	m_Application->SetPlayerPosition(g_NetID);
  #endif
	return true;
}
#endif

#if defined USE_TINYXML_LOADER //5
bool SystemClass::LoadXmlSettings()
{
	WOMA_LOGManager_DebugMSG("===============================================================================\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("LOAD XML SETTINGS\n"));
	WOMA_LOGManager_DebugMSG("===============================================================================\n");

	// Load and Parse XML FILE:"settings.xml" the Configuration file
	//----------------------------------------------------------------------------
	XML_SETTINGS_FILE = WOMA::PUBLIC_DOCUMENTS;
	XML_SETTINGS_FILE.append(WOMA::APP_SETTINGS_FILE);

	WOMA_LOGManager_DebugMSGAUTO(TEXT("XML_SETTINGS_FILE: %s\n"), XML_SETTINGS_FILE.c_str());
	if (!SystemHandle->xml_loader.initAppicationSettings((TCHAR*)XML_SETTINGS_FILE.c_str()))
	{
		STRING err = TEXT("LoadXmlSettings::Settings File not found/Invalid: "); err += XML_SETTINGS_FILE;
		WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), false);
		return false;
	}


	SystemHandle->LandScape = (SystemHandle->AppSettings->WINDOW_WIDTH >= SystemHandle->AppSettings->WINDOW_HEIGHT) ? true : false;

	//FORCE LANDSCAPE

	return true;
}

bool SystemClass::LoadXmlWorld()
{
	WOMA_LOGManager_DebugMSG("===============================================================================\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("LOAD XML WORLD\n"));
	WOMA_LOGManager_DebugMSG("===============================================================================\n");

	// Load and Parse XML FILE:"world.xml" the Configuration file
	//----------------------------------------------------------------------------
	XML_WORLD_FILE = WOMA::PUBLIC_DOCUMENTS;
	XML_WORLD_FILE += WORLD_XML;
	if (!SystemHandle->xml_loader.InitWorldLoader((TCHAR*)XML_WORLD_FILE.c_str()))
	{
		STRING err = TEXT("LoadXmlSettings::World File not found/Invalid: "); err += XML_WORLD_FILE;
		WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "));
		return false;
	}

	for (size_t i = 0; i < SystemHandle->xml_loader.theWorld.size(); i++)
	{
		SHADER_TYPE shader = (SHADER_TYPE)SystemHandle->xml_loader.theWorld[i].shader;
		SystemHandle->xml_loader.theWorld[i].WOMA_object = WOMA_OBJECT(shader, castShadows_false, renderShadows_false, modelHASlight_true, SystemHandle->xml_loader.theWorld[i].instances);
	}

	return true;
}

#endif

#if CORE_ENGINE_LEVEL >= 4 && defined USE_TIMER_CLASS
void SystemClass::ProcessPerformanceStats() // Run every frame
{
	// Update the system stats: (BEFORE: HandleUserInput)
	m_Timer.Frame();		// Calculate dT for animations (Measure last frame time)
	m_Fps.Frame();			// Increase the frame counter, calculate FPS once per second
	fps = m_Fps.GetFps();	// Get current FPS (updated by "m_Fps.Frame()" every second)

	m_Application->dt = m_Timer.GetTime();		// Calculate dT for animations & camera movements (in Mili Seconds)

  #if defined WINDOWS_PLATFORM && !defined WIN_XP
	m_Cpu.Frame();					// Collect CPU usage percentage, once per second!
	cpu = m_Cpu.GetCpuPercentage(); // Get current CPU use  (updated by "m_Cpu.Frame()" every second)
  #endif
}
#endif

bool SystemClass::InitializeDrivers(int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL vsync, BOOL fullscreen, float* clearColor)
{
	// INIT GRAPHIC CARD 3D:
	//----------------------------------------------------------------------------
	// Create/Initialize the "Main" Driver: "DX11" or "OpenGL" object.

#if	!defined NewWomaEngine
	WOMA_LOGManager_DebugMSG("LoadAllDrivers - DriverList:\n");
	UINT size = (UINT)driverList.size();
	if (size >= 1)
		if (driverList[0])
			WOMA_LOGManager_DebugMSG("driverList[0] = %s \n", driverList[0]->driverName);
	if (size >= 2)
		if (driverList[1])
			WOMA_LOGManager_DebugMSG("driverList[1] = %s \n", driverList[1]->driverName);
	if (size >= 3)
		if (driverList[2])
			WOMA_LOGManager_DebugMSG("driverList[2] = %s \n", driverList[2]->driverName);
	if (size >= 4)
		if (driverList[3])
			WOMA_LOGManager_DebugMSG("driverList[3] = %s \n", driverList[3]->driverName);

	WOMA_LOGManager_DebugMSG("XML Conf - Selected Driver [%d]\n", AppSettings->DRIVER);

	// Initialize: the Main "Driver"
	if (m_Driver)
		IF_NOT_RETURN_FALSE(m_Driver->Initialize(clearColor));
#endif

	return true;
}


// --------------------------------------------------------------------------------------------
void SystemClass::LoadAllDrivers()
// --------------------------------------------------------------------------------------------
{
	// Allocate all Drivers:
	WOMA_LOGManager_DebugMSG("SystemClass::LoadAllDrivers()\n");

#if	!defined NewWomaEngine
	// -------------------------------------------------------------------------------------------
	// [0] DX11 (or DX11 with Downgrade: DX10)
	// -------------------------------------------------------------------------------------------
#if defined ( DX11 )
	//if (WOMA::CapDX10_11)
	{
		WOMA_LOGManager_DebugMSG("LoadDriver[0]: DX11Class\n");//driver = g_contextDriver;	// Re-Use the same driver ( Context Driver )
		driverList.push_back(NEW DirectX::DX11Class());
}
#else
	driverList.push_back(NULL);
#endif

	// -------------------------------------------------------------------------------------------
	// [1] GL3+
	// -------------------------------------------------------------------------------------------
#if (defined OPENGL3 || defined OPENGL4) //Driver
	WOMA_LOGManager_DebugMSG("LoadDriver[1]: GLopenGLclass\n");
	driverList.push_back((WomaDriverClass*)NEW GLopenGLclass);
#else
	driverList.push_back(NULL);
#endif

	// -------------------------------------------------------------------------------------------
	// [2] DX 9 (or DX11 with Downgrade: DX9)
	// -------------------------------------------------------------------------------------------
#if defined DX9sdk
	//if (WOMA::CapDX9)
	{
		driverList.push_back(NEW DirectX::DX9Class());//driver = g_contextDriver;	// Re-Use the same driver ( Context Driver )
		WOMA_LOGManager_DebugMSG("LoadDriver[2]: DX9Class\n");
	}
#elif defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
	WOMA_LOGManager_DebugMSG("LoadDriver[2]: DX9(DX11)Class\n");
	driverList.push_back(NEW DirectX::DX11Class());
	((DirectX::DX11Class*)driverList[2])->dx11_force_dx9 = true;
	((DirectX::DX11Class*)driverList[2])->CheckAPIdriver(USE_THIS_GRAPHIC_CARD_ADAPTER); //Recalc the capabilities for DX9
#else
	driverList.push_back(NULL);
#endif

	// -------------------------------------------------------------------------------------------
	// [3] DX 12
	// -------------------------------------------------------------------------------------------
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	//if (WOMA::CapDX12)
	{
		driverList.push_back(NEW DirectX::DX12Class());
		//driver = g_contextDriver;	// Re-Use the same driver ( Context Driver )
		WOMA_LOGManager_DebugMSG("LoadDriver[3]: DX12Class\n");
		}
#else
	driverList.push_back(NULL);
#endif
#endif

} //LoadAllDrivers()

#if defined USE_JOY && defined USE_DIRECT_INPUT

void SystemClass::InitJoyStick()
{
	JOYINFO joyInfo;
	useJoystick = (joyGetPos(JOYSTICKID1, &joyInfo) == JOYERR_NOERROR);

	if (useJoystick)
	{
		if (joyGetDevCaps(JOYSTICKID1, &joyCaps, sizeof(joyCaps)) == JOYERR_NOERROR)
		{
			joyFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNBUTTONS;
			xScale = 2.0f / float(int(joyCaps.wXmax) - int(joyCaps.wXmin));
			xBias = 1.0f - joyCaps.wXmax * xScale;
			yScale = 2.0f / float(int(joyCaps.wYmax) - int(joyCaps.wYmin));
			yBias = 1.0f - joyCaps.wYmax * yScale;

			if (joyCaps.wCaps & JOYCAPS_HASZ) {
				joyFlags |= JOY_RETURNZ;
				zScale = 2.0f / float(int(joyCaps.wZmax) - int(joyCaps.wZmin));
				zBias = 1.0f - joyCaps.wZmax * zScale;
			}
			if (joyCaps.wCaps & JOYCAPS_HASR) {
				joyFlags |= JOY_RETURNR;
				rScale = 2.0f / float(int(joyCaps.wRmax) - int(joyCaps.wRmin));
				rBias = 1.0f - joyCaps.wRmax * rScale;
			}
			if (joyCaps.wCaps & JOYCAPS_HASU) {
				joyFlags |= JOY_RETURNU;
				uScale = 2.0f / float(int(joyCaps.wUmax) - int(joyCaps.wUmin));
				uBias = 1.0f - joyCaps.wUmax * uScale;
			}
			if (joyCaps.wCaps & JOYCAPS_HASV) {
				joyFlags |= JOY_RETURNV;
				vScale = 2.0f / float(int(joyCaps.wVmax) - int(joyCaps.wVmin));
				vBias = 1.0f - joyCaps.wVmax * vScale;
			}
		}

		// MORE INFO: https://technet.microsoft.com/pt-br/dd757103%28v=vs.71%29
		WOMA_LOGManager_DebugMSG("Joystick: Detected\n");
		WOMA_LOGManager_DebugMSG("Driver: %s\n", joyCaps.szPname);
		WOMA_LOGManager_DebugMSG("Manufacturer identifier: %X\n", joyCaps.wMid);
		WOMA_LOGManager_DebugMSG("Product identifier: %X\n", joyCaps.wPid);

		WOMA_LOGManager_DebugMSG("Num Buttons: %d\n", joyCaps.wNumButtons);
		WOMA_LOGManager_DebugMSG("Num Axes: %d\n", joyCaps.wNumAxes);
	}
	else {
		WOMA_LOGManager_DebugMSG("Joystick: Not Detected\n");
	}
}


void SystemClass::joyStickFrame()
{
	joyInfo.dwSize = sizeof(joyInfo);
	joyInfo.dwFlags = joyFlags;

	if (joyGetPosEx(JOYSTICKID1, &joyInfo) == JOYERR_NOERROR) {
		DWORD changed = lastButtons ^ joyInfo.dwButtons;
		if (changed) {
			for (UINT i = 0; i < joyCaps.wNumButtons; i++) {
				// Only call App for buttons that changed
				if (changed & 1) {
					onJoystickButton(i, ((joyInfo.dwButtons >> i) & 1) != 0);
				}
				changed >>= 1;
			}

			lastButtons = joyInfo.dwButtons;
		}
		if ((joyInfo.dwFlags & JOY_RETURNX) && joyInfo.dwXpos != lastXpos) {
			onJoystickAxis(0, joyInfo.dwXpos * xScale + xBias);
			lastXpos = joyInfo.dwXpos;
		}
		if ((joyInfo.dwFlags & JOY_RETURNY) && joyInfo.dwYpos != lastYpos) {
			onJoystickAxis(1, joyInfo.dwYpos * yScale + yBias);
			lastYpos = joyInfo.dwYpos;
		}
		if ((joyInfo.dwFlags & JOY_RETURNZ) && joyInfo.dwZpos != lastZpos) {
			onJoystickAxis(2, joyInfo.dwZpos * zScale + zBias);
			lastZpos = joyInfo.dwZpos;
		}
		if ((joyInfo.dwFlags & JOY_RETURNR) && joyInfo.dwRpos != lastRpos) {
			onJoystickAxis(3, joyInfo.dwRpos * rScale + rBias);
			lastRpos = joyInfo.dwRpos;
		}
		if ((joyInfo.dwFlags & JOY_RETURNU) && joyInfo.dwUpos != lastUpos) {
			onJoystickAxis(4, joyInfo.dwUpos * uScale + uBias);
			lastUpos = joyInfo.dwUpos;
		}
		if ((joyInfo.dwFlags & JOY_RETURNV) && joyInfo.dwVpos != lastVpos) {
			onJoystickAxis(5, joyInfo.dwVpos * vScale + vBias);
			lastVpos = joyInfo.dwVpos;
		}
		WOMA_LOGManager_DebugMSGAUTO("lastXpos: %u lastYpos: %u lastZpos: %u lastRpos: %u lastUpos: %u lastVpos: %u\n");
	}
}

bool SystemClass::onJoystickButton(int button, bool pressed)
{
	WOMA_LOGManager_DebugMSG("button: %d is pressed: %d\n", button, pressed);
	return true;
}

void SystemClass::onJoystickAxis(int axis, float value)
{
#if defined _DEBUG
	WOMA_LOGManager_DebugMSG("axis: %d value: %f\n", axis, value);
#endif
}
#endif

#if DX_ENGINE_LEVEL >= 19 && LEVEL >= 10// || (LEVEL < 10 && defined ANDROID_PLATFORM)
bool InitSelectedDriver()
{
	/*******************************************************************
	// [INIT CONTEXT Driver]: ("g_contextDriver->OnInit()") Create: Rendering Context DRIVER! DirectX 11 or OPEN GL3+
	*******************************************************************/
	WOMA::Settings* AppSettings = SystemHandle->AppSettings;
#if !defined NewWomaEngine
	switch (AppSettings->DRIVER)
	{
#if defined DX11 // [0] Pure DX11
	case DRIVER_DX11:
		ASSERT(((DirectX::DX11Class*)(driverList[DRIVER_DX11]))->OnInit(AppSettings->UI_MONITOR, SystemHandle->m_hWnd, 
			AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_Anisotropic, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#endif
#if (defined OPENGL3 || defined OPENGL4) //[1]
	case DRIVER_GL3:
		ASSERT(((GLopenGLclass*)(driverList[DRIVER_GL3]))->OnInit(AppSettings->UI_MONITOR, SystemHandle->m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_Anisotropic, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
#if !defined ANDROID_PLATFORM
		ASSERT(g_contextDriver->OnInit(AppSettings->UI_MONITOR, SystemHandle->m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_Anisotropic, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
#endif
		break;
#endif
#if defined DX9sdk //[2]
	case DRIVER_DX9:
		ASSERT(((DirectX::DX9Class*)(driverList[DRIVER_DX9]))->OnInit(AppSettings->UI_MONITOR, SystemHandle->m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_Anisotropic, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#endif
#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009 //[2]
	case DRIVER_DX9:
		ASSERT(((DirectX::DX11Class*)(driverList[DRIVER_DX9]))->OnInit(AppSettings->UI_MONITOR, SystemHandle->m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_Anisotropic, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //[3]
	case DRIVER_DX12:
		ASSERT(((DirectX::DX12Class*)(driverList[DRIVER_DX12]))->OnInit(AppSettings->UI_MONITOR, SystemHandle->m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_Anisotropic, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#endif
};

	// [INIT MAIN Driver] "DX11" / "OpenGL": (Like: Select default color): "m_Driver->Initialize(clearColor)" [DX12] DXSample::Run OnInit();  LoadPipeline();
	if (!SystemHandle->InitializeDrivers(AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, AppSettings->SCREEN_NEAR, AppSettings->SCREEN_DEPTH,
		AppSettings->VSYNC_ENABLED, AppSettings->FULL_SCREEN, SystemHandle->m_Application->ClearColor))
		return false;

	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("Active Render Systems:\n"));
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");

#if defined DX_ENGINE
	WOMA_LOGManager_DebugMSGAUTO(TEXT("DX 9 Support: %s\n"), driverList[SystemHandle->AppSettings->DRIVER]->m_sCapabilities.CapDX9 ? TEXT("true") : TEXT("false"));		//Allow DX11, scale down to DX9, if needed?

#if defined DX9sdk
	if (driverList[DRIVER_DX9] && AppSettings->DRIVER == DRIVER_DX9) {
		if (!WOMA::UseWarpDevice)
			_tcscpy_s(driverName, sizeof(driverName), TEXT("DX9sdk"));
		else
			_tcscpy_s(driverName, sizeof(driverName), TEXT("DX9sdk WARP"));
	}
#endif

#if defined DX11 // Pure DX11
	if (driverList[DRIVER_DX11]) //Driver 11 will give backward compatibility:
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX10 Support: %s\n"), driverList[SystemHandle->AppSettings->DRIVER]->m_sCapabilities.CapDX10_11 ? TEXT("true") : TEXT("false"));	//Allow DX11, scale down to DX10, if needed?		
	else
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX10 Support: false\n"));

	if (driverList[DRIVER_DX11])
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX11 Support: %s\n"), (((DirectX::DX11Class*)(driverList[DRIVER_DX11]))->ShaderVersionH >= 5) ? TEXT("true") : TEXT("false"));
	else
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX11 Support: false\n"));
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (driverList[DRIVER_DX12])
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX12 Support: %s\n"), (((DirectX::DX12Class*)(driverList[DRIVER_DX12]))->ShaderVersionH >= 6) ? TEXT("true") : TEXT("false"));
	else
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX12 Support: false\n"));
#endif
#endif

#if (defined OPENGL3 || defined OPENGL4)
	WOMA_LOGManager_DebugMSGAUTO(TEXT("OPENGL3 Support: true\n"));
#endif
#endif

	return true;
}
bool newDriver()
{
	WOMA::Settings* AppSettings = SystemHandle->AppSettings;

	WOMA_LOGManager_DebugMSG("===============================================================================\n");
	WOMA_LOGManager_DebugMSG("CREATE WINDOWS CONTEXT DRIVER\n");

	switch (AppSettings->DRIVER)
	{
	case -1:
		WomaFatalException("AppSettings DRIVER not defined!");
		break;

	case DRIVER_GL3:
#ifdef OPENGL3
	#if defined WINDOWS_PLATFORM
		g_contextDriver = NEW wGLopenGLclass;		// Create the OpenGL object for windows --> gl3wInit();
	#endif
#else
		WomaMessageBox(TEXT("The selected driver GL3 is not available at this build"), TEXT("FATAL"), MB_OK);
#endif
		break;
	}

	return true;
}
#endif
