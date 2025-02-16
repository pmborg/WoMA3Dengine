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

#if defined DX9sdk
#include "Dx9Class.h"
#endif
#include "Dx11Class.h"
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif
#include "womadriverclass.h"	//woma
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#include "wGLopenGLclass.h"		// Windows

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

#if D3D11_SPEC_DATE_YEAR == 2009
#include <D3dx9core.h>		//D3DX_SDK_VERSION (Checks for the existance of the correct D3DX library version)
#endif

#include "Math3D.h"

SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

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
		TEXT("Win10/11")
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

		static bool first_time = false;

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

	if (first_time || (OS_KEY_DOWN(VK_F4) && WOMA::game_state != GAME_WEATHER_INFO)) {
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
		WOMA::game_state = GAME_WEATHER_INFO;
		OS_REDRAW_WINDOW;
	}
	first_time = false;

	if (first_time || (OS_KEY_DOWN(VK_F3) && WOMA::game_state != GAME_SHOW_POSITION)) {
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
		WOMA::game_state = GAME_SHOW_POSITION;
		OS_REDRAW_WINDOW;
	}
	first_time = false;

	if (first_time || (OS_KEY_DOWN(VK_F2) && WOMA::game_state != GAME_CELESTIAL_INFO)) {
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
		WOMA::game_state = GAME_CELESTIAL_INFO;
		OS_REDRAW_WINDOW;
	}
	first_time = false;

	if (first_time || (OS_KEY_DOWN(VK_F6) && WOMA::game_state != GAME_SETUP))
	{
		WOMA::game_state = GAME_SETUP;
		OS_REDRAW_WINDOW;
		// Toggle the full screen/window mode

		if (SystemHandle->AppSettings->FULL_SCREEN)
		{
			SystemHandle->AppSettings->FULL_SCREEN = false;
			CHAR str[MAX_STR_LEN] = { 0 }; wtoa(str, (TCHAR*)SystemHandle->XML_SETTINGS_FILE.c_str(), MAX_STR_LEN); // wchar ==> char
			SystemHandle->xml_loader.saveConfigSettings(str);
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

	if (driverList[DRIVER_GL3]) {
		//delete ((wGLopenGLclass*)driverList[DRIVER_GL3]);
		delete ((GLopenGLclass*)driverList[DRIVER_GL3]);
		driverList[DRIVER_GL3] = NULL;
	}
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (driverList[DRIVER_DX12]) {
		delete ((DirectX::DX12Class*)driverList[DRIVER_DX12]);
		driverList[DRIVER_DX12] = NULL;
	}
#endif
	if (driverList[DRIVER_DX11]) {
		delete ((DirectX::DX11Class*)driverList[DRIVER_DX11]);
		driverList[DRIVER_DX11] = NULL;
	}
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

	AppSettings = NULL;				// Pointer to Static object, no need to free.
}

void SystemClass::FrameUpdate()
{
	ProcessOSInput();			// ProcessFrame: Process Special: Function Keys F1 to F6
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
	if (!SystemHandle->xml_loader.initAppicationSettings((TCHAR*)XML_SETTINGS_FILE.c_str()))
	{
		STRING err = TEXT("File not found/Invalid: "); err += XML_SETTINGS_FILE;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "));
		return false;
	}

	// Load and Parse XML [world.xml] the Configuration file
	//----------------------------------------------------------------------------

	return true;
}

void SystemClass::ProcessPerformanceStats() // Run every frame
{
	// Update the system stats: (BEFORE: HandleUserInput)
	m_Timer.Frame();		// Calculate dT for animations (Measure last frame time)
	m_Fps.Frame();			// Increase the frame counter, calculate FPS once per second
	fps = m_Fps.GetFps();	// Get current FPS (updated by "m_Fps.Frame()" every second)

#if defined WINDOWS_PLATFORM && !defined WIN_XP
	m_Cpu.Frame();			// Collect CPU usage percentage, once per second!

	m_Application->dt = m_Timer.GetTime();		// Calculate dT for animations & camera movements (in Mili Seconds)
#endif

#if defined WINDOWS_PLATFORM && !defined WIN_XP
	cpu = m_Cpu.GetCpuPercentage(); // Get current CPU use  (updated by "m_Cpu.Frame()" every second)
#endif
}

bool SystemClass::LoadAllGraphics()
{

	// LOAD ALL INITIAL 3D OBJECTS, that will be rendered@ 1st Frame & START TIMER
	//----------------------------------------------------------------------------
	if (!m_Application->Initialize(m_Driver))
		WOMA::game_state = GAME_STOP;

	//Shutdown VIDEO PLAYER:
#if defined USE_INTRO_VIDEO_DEMO
	SAFE_DELETE(g_DShowPlayer);
#endif

	return true;
}

bool SystemClass::InitializeDrivers(int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL vsync, BOOL fullscreen, float* clearColor)
{
	// INIT GRAPHIC CARD 3D:
	//----------------------------------------------------------------------------
	// Create/Initialize the "Main" Driver: "DX11" or "OpenGL" object.

	WOMA_LOGManager_DebugMSG("LoadAllDrivers - DriverList:\n");
	WOMA_LOGManager_DebugMSG("driverList[0] = %d \n", driverList[0]);
	WOMA_LOGManager_DebugMSG("driverList[1] = %d \n", driverList[1]);
	WOMA_LOGManager_DebugMSG("driverList[2] = %d \n", driverList[2]);
	WOMA_LOGManager_DebugMSG("driverList[3] = %d \n", driverList[3]);

	WOMA_LOGManager_DebugMSG("XML Conf - Selected Driver [%d]\n", AppSettings->DRIVER);

	// Pick The Driver to Use:
	switch (AppSettings->DRIVER)
	{
		// [0] DX11 (10)
	case DRIVER_DX11:
		//m_contextDriver = m_Driver = driverList[DRIVER_DX11];
		m_Driver = driverList[DRIVER_DX11];
		break;

		// [1] GL3+
	case DRIVER_GL3:
		m_Driver = driverList[DRIVER_GL3]; // NEW GLopenGLclass;
		//m_contextDriver is another one on OPENGL
		break;

		// [2] DX 9
	case DRIVER_DX9:
		//m_contextDriver = m_Driver = driverList[DRIVER_DX9];
		m_Driver = driverList[DRIVER_DX9];
		break;

		// [3] DX 12
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
	case DRIVER_DX12:
		//m_contextDriver = m_Driver = driverList[DRIVER_DX12];
		m_Driver = driverList[DRIVER_DX12];
		break;
	#endif
	}

	if (!m_Driver) {
		MessageBox(SystemHandle->m_hWnd, TEXT("No Driver Selected"), TEXT("FATAL ERROR"), MB_ICONERROR);
		return false;
	}

	// Initialize: the Main "Driver"
	IF_NOT_RETURN_FALSE(m_Driver->Initialize(clearColor));

	return true;
}

// --------------------------------------------------------------------------------------------
void SystemClass::LoadAllDrivers()
// --------------------------------------------------------------------------------------------
{
	// Allocate all Drivers:
	WOMA_LOGManager_DebugMSG("SystemClass::LoadAllDrivers()\n");

	// -------------------------------------------------------------------------------------------
	// [0] DX11 (or DX11 with Downgrade: DX10)
	// -------------------------------------------------------------------------------------------
	//if (WOMA::CapDX10_11)
	{
		WOMA_LOGManager_DebugMSG("LoadDriver[0]: DX11Class\n");//driver = m_contextDriver;	// Re-Use the same driver ( Context Driver )
		driverList.push_back(NEW DirectX::DX11Class());
	}

	// -------------------------------------------------------------------------------------------
	// [1] GL3+
	// -------------------------------------------------------------------------------------------
	WOMA_LOGManager_DebugMSG("LoadDriver[1]: GLopenGLclass\n");
	driverList.push_back((WomaDriverClass*)NEW GLopenGLclass);

	// -------------------------------------------------------------------------------------------
	// [2] DX 9 (or DX11 with Downgrade: DX9)
	// -------------------------------------------------------------------------------------------
#if defined DX9sdk
	//if (WOMA::CapDX9)
	{
		driverList.push_back(NEW DirectX::DX9Class());//driver = m_contextDriver;	// Re-Use the same driver ( Context Driver )
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
		//driver = m_contextDriver;	// Re-Use the same driver ( Context Driver )
		WOMA_LOGManager_DebugMSG("LoadDriver[3]: DX12Class\n");
	}
#else
	driverList.push_back(NULL);
#endif

	WomaDriverClass* m_GLcontextDriver = NULL;
}


