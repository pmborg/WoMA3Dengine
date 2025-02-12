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
//WomaIntegrityCheck = 1234567311;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "SystemClass.h"
#include "OSengine.h" //#include "WinSystemClass.h"
#include "default_settings_xml.h"
#include "woma_macros.h"

#if defined WINDOWS_PLATFORM
#include "language.h"
#endif

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567311;

	AppSettings = NULL;

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

	WOMA_LOGManager_DebugMSG("\n");
	//#endif

	return true;
}
#endif


SystemClass::~SystemClass() { CLASSDELETE(); }

void SystemClass::Shutdown()
{
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

	#if defined USE_PROCESS_OS_KEYS && defined WINDOWS_PLATFORM
		ProcessOSInput();							// READ+PROCESS-OS-INPUT: Process Special: Function Keys |ESC and F1 to F6|
		if (WOMA::game_state == ENGINE_RESTART)
			return;
	#endif
	
}

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

