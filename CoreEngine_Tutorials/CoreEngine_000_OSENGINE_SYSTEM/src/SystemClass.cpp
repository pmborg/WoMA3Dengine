// --------------------------------------------------------------------------------------------
// Filename: SystemClass.cpp
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
// PURPOSE: Define APIs for systemclass.cpp which is the common OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "SystemClass.h"
#include "OSengine.h" //#include "WinSystemClass.h"
#include "default_settings_xml.h"
#include "woma_macros.h"

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();

	AppSettings = NULL;

	// Reset Vars:
	// -------------------------------------------------------------------------------------------
#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
	m_OsInput = NULL;
#endif


#if defined USE_SYSTEM_CHECK
	systemManager = NULL;
	//userName = TEXT("");
	//ComputerName = TEXT("");
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

void SystemClass::CalculateCameraViewAndFrustum(void* pContext)
{
	// SET CAMERA (for this monitor): Prepare to Take a Shot: Generate the view matrix based on the camera's position.

	// CONSTRUCT: FRUSTUM
#if defined USE_FRUSTUM
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX12Class*)m_Driver)->m_projectionMatrix,
			&DXsystemHandle->m_Camera->m_viewMatrix);
#endif

#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX11Class*)m_Driver)->m_projectionMatrix,
			&DXsystemHandle->m_Camera->m_viewMatrix);
#endif

#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) {

		mat4 glPrjMatrix = ((GLopenGLclass*)m_Driver)->m_projectionMatrix;
		XMMATRIX m_projectionMatrix = XMMatrixSet
		(
			glPrjMatrix.m[0], glPrjMatrix.m[1], glPrjMatrix.m[2], glPrjMatrix.m[3],
			glPrjMatrix.m[4], glPrjMatrix.m[5], glPrjMatrix.m[6], glPrjMatrix.m[7],
			glPrjMatrix.m[8], glPrjMatrix.m[9], glPrjMatrix.m[10], glPrjMatrix.m[11],
			glPrjMatrix.m[12], glPrjMatrix.m[13], glPrjMatrix.m[14], glPrjMatrix.m[15]
		);

		mat4 glvMatrix = ((GLopenGLclass*)m_Driver)->gl_Camera->m_viewMatrix;
		XMMATRIX m_viewMatrix = XMMatrixSet
		(
			glvMatrix.m[0], glvMatrix.m[1], glvMatrix.m[2], glvMatrix.m[3],
			glvMatrix.m[4], glvMatrix.m[5], glvMatrix.m[6], glvMatrix.m[7],
			glvMatrix.m[8], glvMatrix.m[9], glvMatrix.m[10], glvMatrix.m[11],
			glvMatrix.m[12], glvMatrix.m[13], glvMatrix.m[14], glvMatrix.m[15]
		);

		m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&m_projectionMatrix,
			&m_viewMatrix);
	}
#endif
#endif

	//AFTER: CalculateCameraViewAndFrustum:

}

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

	// "ESC": DX Process Special: key is being pressed ? -> EXIT APPLICATION

	// "ESC" OS Process Special: key is being pressed ? -> EXIT APPLICATION

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
			SystemHandle->xml_loader.saveXMLsettingsFile(str);
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

	// [10] Check ENDIAN = LITTLE_ENDIAN or BIG_ENDIAN  (Used in some libs)
	// -------------------------------------------------------------------------------------------
	WOMA::settings.Endian = WOMA::endian();
	if (WOMA::settings.Endian == LITTLE_ENDIAN)
		womalogauto(TEXT("The machine is Little ENDIAN\n"));	//8008, 8080, 8085, 8086, ...
	else
		womalogauto(TEXT("The machine is Big ENDIAN\n"));		//Motorola 68000

	womalogauto(TEXT("\n"));

	LEVELHIGHLIGHT(4);
	// INIT SYSTEM SETTINGS:
	womalog("------------------------------------SYSTEM CHECK SETTINGS: --------------------------------\n");
	LEVELNORMAL();
	womalogauto(TEXT("ENGINE_LEVEL: %d [Function Loader] get_current_dir()\n"), WOMA::ENGINE_LEVEL_USED);

	systemDefinitions.userName = getUserName(); // Note: Save for later use!
	womalogauto(TEXT("User Name: %s\n"), systemDefinitions.userName.c_str());

	systemDefinitions.ComputerName = getComputerName();
	womalogauto(TEXT("Computer Name: %s\n"), systemDefinitions.ComputerName.c_str());

	IF_NOT_RETURN_FALSE(WOMA::getCurrentDir());
	//LEVELNORMAL();

	systemManager = NEW SystemManager();
	IF_NOT_THROW_EXCEPTION(systemManager);

	// [2] CheckOS: Detect OS Version & DO System Check: DONE
	//----------------------------------------------------------------------------
	//LEVELHIGHLIGHT(4);
	womalog("-------------------------------------------------------------------------------\n");
	womalogauto(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->CheckOS()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->CheckOS()); // Mandatory line for all ENGINE_LEVELs

#if defined DX11 && !defined WIN10
	// FOR WINDOWS VISTA:
	womalog("-------------------------------------------------------------------------------\n");
	womalogauto(TEXT("[Function Loader] systemManager->UpdateOSifNeeded()\n"));
	IF_NOT_RETURN_FALSE(systemManager->UpdateOSifNeeded());	// TODO: UpdateOSifNeeded: Check if OS need Updates: DONE

	womalogauto(TEXT("[Function Loader] systemManager->CheckSetup()\n"));
	IF_NOT_RETURN_FALSE(systemManager->CheckSetup());		// TODO: CheckSetup: Check if "Setup.exe" is Installed or needed / Download & Install: DONE
#endif // NOTE: WINDOWS10/DX12: Don't need this checks
	//LEVELNORMAL();

	//LEVELHIGHLIGHT(4);
	womalog("-------------------------------------------------------------------------------\n");
	womalogauto(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkCPU()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->checkCPU());
	//LEVELNORMAL();

	//LEVELHIGHLIGHT(5);
	womalog("-------------------------------------------------------------------------------\n");
	womalogauto(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkRAM()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->checkRAM());
	//LEVELNORMAL();

	//LEVELHIGHLIGHT(6);
	womalog("-------------------------------------------------------------------------------\n");
	womalogauto(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkDiskFreeSpace()\n"), WOMA::ENGINE_LEVEL_USED);
	IF_NOT_RETURN_FALSE(systemManager->checkDiskFreeSpace());
	//LEVELNORMAL();

	//LEVELHIGHLIGHT(7);
	womalog("-------------------------------------------------------------------------------\n");
	womalogauto(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkCPUFeatures()\n"), WOMA::ENGINE_LEVEL_USED);

#if defined WINDOWS_PLATFORM
	IF_NOT_RETURN_FALSE(systemManager->checkCPUFeatures());
	//LEVELNORMAL();
#endif

	if (Command == 0) {
#if ((defined USE_TIMER_CLASS && CORE_ENGINE_LEVEL >= 6) || (defined NDEBUG || defined INTRO_DEMO) || CORE_ENGINE_LEVEL == 4) && defined WINDOWS_PLATFORM
		//LEVELHIGHLIGHT(8);
		womalog("-------------------------------------------------------------------------------\n");
		womalogauto(TEXT("ENGINE_LEVEL: %d [Function Loader] systemManager->checkBenchMarkSpeed()\n"), WOMA::ENGINE_LEVEL_USED);
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

	womalog("\n");
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

	#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	  #if !defined ANDROID_PLATFORM
		DXsystemHandle->GetInputs();				    // READ-INPUT: WinSystemClass::ProcessInput() + DXInputClass::Frame()
	  #endif
	  #if defined DX_ENGINE
		DXsystemHandle->m_Input->ProcessInputKeys();	// Process Keyboard keys / (DXInputClass)
	  #endif
	#endif

	#if defined USE_PROCESS_OS_KEYS && defined WINDOWS_PLATFORM
		ProcessOSInput();							    // Process Special function keys |ESC and F1 to F6|
		if (WOMA::game_state == ENGINE_RESTART)
			return;
	#endif
	
}


void SystemClass::ParseCommandLineArgs(int argc, char* argv[])
{
#if defined USE_TINYXML_LOADER			        // This settings.xml can be override by command line options!
    IF_NOT_THROW_EXCEPTION(LoadXmlSettings()); //  XML: Load Application Settings: "settings.xml", pickup "Driver" to Use (override default: WOMA::settings)
#endif

#if defined UNICODE
    for (int i = 1; i < argc; ++i)
    {
        CHAR* parameter = argv[i];
        TCHAR* wparameter = NULL;
        atow(wparameter, parameter, (int)_tcslen(wparameter)); //VER_PRODUCTVERSION_STRING_FOUR_PARTS

        if (_tcsnicmp(wparameter, TEXT("-warp"), _tcslen(wparameter)) == 0 ||
            _tcsnicmp(wparameter, TEXT("/warp"), _tcslen(wparameter)) == 0)
        {
            WOMA::UseWarpDevice = true;
        }
    }
#else
    for (int i = 1; i < argc; ++i)
    {
        if (strlen(argv[i]) == 0)
            continue;
        if (_tcsnicmp(argv[i], "-warp", _tcslen(argv[i])) == 0 ||
            _tcsnicmp(argv[i], "/warp", _tcslen(argv[i])) == 0)
        {
            WOMA::UseWarpDevice = true;
        }
        if (_tcsnicmp(argv[i], "-renderOnce", _tcslen(argv[i])) == 0 ||
            _tcsnicmp(argv[i], "/renderOnce", _tcslen(argv[i])) == 0)
        {
            WOMA::renderOnce = true;
            WOMA::settings.FULLSCREEN_ON_WINDOWED = false;
            WOMA::settings.FULL_SCREEN = false;
#if defined WINDOWS_PLATFORM
            HWND hWnd = GetConsoleWindow();
            if (hWnd != NULL) {
                ShowWindow(hWnd, SW_MINIMIZE);
            }
#endif
        }
        if (_tcsnicmp(argv[i], "-Xpos", _tcslen(argv[i])) == 0 ||
            _tcsnicmp(argv[i], "/Xpos", _tcslen(argv[i])) == 0)
        {
            WOMA::settings.WINDOW_Xpos_ori = atoi(argv[i + 1]);
            i++;
        }
        if (_tcsnicmp(argv[i], "-Ypos", _tcslen(argv[i])) == 0 ||
            _tcsnicmp(argv[i], "/Ypos", _tcslen(argv[i])) == 0)
        {
            WOMA::settings.WINDOW_Ypos_ori = atoi(argv[i + 1]);
            i++;
        }
        if (_tcsnicmp(argv[i], "-WIDTH", _tcslen(argv[i])) == 0 ||
            _tcsnicmp(argv[i], "/WIDTH", _tcslen(argv[i])) == 0)
        {
            WOMA::settings.WINDOW_WIDTH_ori = atoi(argv[i + 1]);
            i++;
        }
        if (_tcsnicmp(argv[i], "-HEIGHT", _tcslen(argv[i])) == 0 ||
            _tcsnicmp(argv[i], "/HEIGHT", _tcslen(argv[i])) == 0)
        {
            WOMA::settings.WINDOW_HEIGHT_ori = atoi(argv[i + 1]);
            i++;
        }
    }
#endif
}


#if defined USE_TINYXML_LOADER //5
bool SystemClass::LoadXmlSettings()
{
	womalog("===============================================================================\n");
	womalogauto(TEXT("LOAD XML SETTINGS\n"));
	womalog("===============================================================================\n");

	// Load and Parse XML FILE:"settings.xml" the Configuration file
	//----------------------------------------------------------------------------
	XML_SETTINGS_FILE = WOMA::PUBLIC_DOCUMENTS;
	XML_SETTINGS_FILE.append(WOMA::APP_SETTINGS_FILE);

	womalogauto(TEXT("XML_SETTINGS_FILE: %s\n"), XML_SETTINGS_FILE.c_str());
	if (!SystemHandle->xml_loader.initAppicationSettings((TCHAR*)XML_SETTINGS_FILE.c_str()))
	{
		STRING err = TEXT("LoadXmlSettings::Settings File not found/Invalid: "); err += XML_SETTINGS_FILE;
		WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), false);
		return false;
	}

	SystemHandle->LandScape = (SystemHandle->AppSettings->WINDOW_WIDTH >= SystemHandle->AppSettings->WINDOW_HEIGHT) ? true : false;

	//FORCE LANDSCAPE:

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
		womalog("Joystick: Detected\n");
		womalog("Driver: %s\n", joyCaps.szPname);
		womalog("Manufacturer identifier: %X\n", joyCaps.wMid);
		womalog("Product identifier: %X\n", joyCaps.wPid);

		womalog("Num Buttons: %d\n", joyCaps.wNumButtons);
		womalog("Num Axes: %d\n", joyCaps.wNumAxes);
	}
	else {
		womalog("Joystick: Not Detected\n");
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
		womalogauto("lastXpos: %u lastYpos: %u lastZpos: %u lastRpos: %u lastUpos: %u lastVpos: %u\n");
	}
}

bool SystemClass::onJoystickButton(int button, bool pressed)
{
	womalog("button: %d is pressed: %d\n", button, pressed);
	return true;
}

void SystemClass::onJoystickAxis(int axis, float value)
{
#if defined _DEBUG
	womalog("axis: %d value: %f\n", axis, value);
#endif
}
#endif

