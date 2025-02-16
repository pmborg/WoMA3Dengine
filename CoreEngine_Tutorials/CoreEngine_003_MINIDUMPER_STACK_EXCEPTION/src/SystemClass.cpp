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
//WomaIntegrityCheck = 1234567142;

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

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567142;

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
		WOMA::APP_PROJECT_NAME,
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

	static bool first_time = true;

#if defined WINDOWS_PLATFORM

	// "ESC": DX Process Special: key is beeing pressed ? -> EXIT APPLICATION

	// "ESC" OS Process Special: key is beeing pressed ? -> EXIT APPLICATION
	if (m_OsInput->IsKeyDown(VK_ESCAPE) && WOMA::game_state == GAME_RUN)		// CHECK: if the user pressed 'escape' and wants to exit the application.
	{
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
		return;
	}

	if (m_OsInput->IsKeyDown(VK_ESCAPE) && WOMA::game_state >= GAME_SYSTEM_SETTINGS && WOMA::game_state <= GAME_SETUP) {		// CHECK: if the user pressed 'escape' and wants to exit the application.
		//WOMA::game_state = GAME_MENU;
		m_OsInput->m_keys[VK_ESCAPE] = false;
		WOMA::game_state = GAME_RUN;
	}

	//F4

	//F3
	//F2

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

	//F5

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

#if defined WINDOWS_PLATFORM
	IF_NOT_RETURN_FALSE(systemManager->checkCPUFeatures());
	//LEVELNORMAL();
#endif

	if (Command == 0) {
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

#if defined USE_SYSTEM_CHECK
	SAFE_DELETE(systemManager);
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

#if defined ANDROID_PLATFORM && !defined NewWomaEngine
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
	
}

#if CORE_ENGINE_LEVEL >= 2 && (defined USE_PROCESS_OS_KEYS || defined INTRO_DEMO)
bool SystemClass::InitOsInput()
{
  #if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	g_NetID = (UINT)0; // Just Us on the world? So WE will be the index: zero...
	m_Application->SetPlayerPosition(g_NetID);	// Step 6: SetPlayerPosition
  #endif
	return true;
}
#endif

#if defined USE_TINYXML_LOADER //5
bool SystemClass::LoadXmlSettings()
{
//#if CORE_ENGINE_LEVEL >= 5
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
//#endif

	// Load and Parse XML FILE:"world.xml" the Configuration file
	//----------------------------------------------------------------------------

	SystemHandle->LandScape = (SystemHandle->AppSettings->WINDOW_WIDTH >= SystemHandle->AppSettings->WINDOW_HEIGHT) ? true : false;

	//FORCE LANDSCAPE

	return true;
}
#endif

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

