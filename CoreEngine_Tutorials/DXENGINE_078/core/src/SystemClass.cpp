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
#include "mem_leak.h"

#include "OSmain_dir.h"

#if defined WINDOWS_PLATFORM
#include "language.h"
#endif

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
#include "womadriverclass.h"
#if defined DX11 || defined DX9
#include "Dx11Class.h"
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif
#if (defined OPENGL3 || defined OPENGL4)
#include "womadriverclass.h"	// WOMA
#include "GLmathClass.h"		// WOMA	
#include "GLopenGLclass.h"		// WOMA
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif

#if D3D11_SPEC_DATE_YEAR == 2009
#include <D3dx9core.h>		//D3DX_SDK_VERSION (Checks for the existence of the correct D3DX library version)
#endif

#include "Math3D.h"
#endif

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

//----------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 19 && !defined NewWomaEngine
bool SystemClass::LoadAllGraphicAssets(void* pContext)
{
	//To preserve later the aspect ratio:
	SystemHandle->m_Application->scaleX = WOMA::AppSettings->WINDOW_WIDTH / 1920.0f;
	SystemHandle->m_Application->scaleY = WOMA::AppSettings->WINDOW_HEIGHT / 1080.0f;
	SystemHandle->m_Application->rescale = min(SystemHandle->m_Application->scaleX, SystemHandle->m_Application->scaleY);
	
	//################################ LOAD ALL INITIAL 3D OBJECTS ##################################
	// Load all assets that will be rendered on 1ST FRAME
	if (!m_Application->Initialize(pContext, m_Driver))
	{
		womalog("m_Application->Initialize() FAILED!");
		WOMA::main_loop_state = -1; 
		WOMA::game_state = GAME_STOP;
		return false;
	}

	return true;
}
#endif

//----------------------------------------------------------------------------
SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();

	WOMA::AppSettings = NULL;

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
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("Visual C++ %s.%s"), GET_VERSION(__GNUC__), GET_VERSION(__GNUC_MINOR__));
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
		TEXT("Lvl: %d %s v%c%c%c%c.%c%c.%c%c BIN:%s OS:%s %s BUILD:%s BIN:%dbit %s CHAR:%s"),
		LEVEL, 
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
	#if defined ANDROID_PLATFORM
		#if defined X64	
			TEXT("Android-x64")
		#else
			TEXT("Android-x86")
		#endif
	#else
		TEXT("WIN7")
	#endif
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
		//TEXT(__DATE__)
	);
	
	// Log Title:
	StringCchPrintf(WOMA::APP_FULLNAME, sizeof(WOMA::APP_FULLNAME), TEXT("%s"), WOMA::APP_NAME);

	womalogauto((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	womalogauto((TCHAR*)WOMA::APP_FULLNAME);
	womalogauto((TCHAR*)TEXT("\n-------------------------------------------------------------------------------\n"));

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

	m_Application = NEW ApplicationClass;	// Create the User Level Class
}


#if CORE_ENGINE_LEVEL >= 4 && defined USE_SYSTEM_CHECK
void SystemClass::InitializeSystemScreenF1(int x, int y)
//-----------------------------------------------------------------------------------------
{
	womalog("InitializeSystemScreenF1...\n");

	//v1
	//float LINE = 24;
	//float LINE_SPACE=45;
	//v2
	float LINE = 16;
	float LINE_SPACE = 40;
	if (fontSizeY > 0) {
		LINE = MIN(LINE, 2 * fontSizeY);
		LINE_SPACE = MIN(LINE_SPACE, 3 * fontSizeY);
	}

	// Make sure that is Reset!
	while (!TextToPrint[0].empty())
		TextToPrint[0].pop_back();

	// ----------------------------------
	TCHAR szScratch[128] = { 0 };

	// System Language:
	Woma_Label text = { TEXT(""), x, y };
	StringCchPrintf(szScratch, sizeof(szScratch), TEXT("System Language: %s"), systemDefinitions.szCountryBuffer);
	text.label = szScratch;
	TextToPrint[0].push_back(text);

	// Username
	StringCchPrintf(szScratch, sizeof(szScratch), TEXT("User Name: %s"), systemDefinitions.userName.c_str());
	text.y += (int)LINE;
	text.label = szScratch;
	TextToPrint[0].push_back(text);

	// Computer name
	StringCchPrintf(szScratch, sizeof(szScratch), TEXT("Computer Name: %s"), systemDefinitions.ComputerName.c_str());
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
	text.y += (int)LINE_SPACE; 
	//text.label = systemDefinitions.platform;
	//TextToPrint[0].push_back(text);

	//text.y += (int)LINE; 
	text.label = systemDefinitions.characterSet;
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

	text.y += (int)LINE; text.label = systemDefinitions.ProcessorCacheL1D;
	TextToPrint[0].push_back(text);
	text.y += (int)LINE; text.label = systemDefinitions.ProcessorCacheL1I;
	TextToPrint[0].push_back(text);
	text.y += (int)LINE; text.label = systemDefinitions.ProcessorCacheL2;
	TextToPrint[0].push_back(text);
	text.y += (int)LINE; text.label = systemDefinitions.ProcessorCacheL3;
	TextToPrint[0].push_back(text);

	int HALF;
	// ----------------------------------
	// NEW PAGE
	// ----------------------------------
	// BOARD/CPU Features (RIGHT SIDE):
#if defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->WINDOW_WIDTH == 0)
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
				if (((deviceNum == WOMA::AppSettings->UI_MONITOR) && (WOMA::AppSettings->UseAllMonitors == false)) ||
					((deviceNum == MONITOR_NUM) && (WOMA::AppSettings->UseAllMonitors == true)))
				{
					HALF = (devMode.dmPelsWidth / 5) * 3;
					break;
				}
			}
		}
	}
	else
#endif

	// BenchMark totalMemoryCapacity
	text.y += (int)LINE_SPACE; text.label = TEXT("MEMORY:");
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.totalMemoryCapacity;
	TextToPrint[0].push_back(text);

	text.y += (int)LINE; text.label = systemDefinitions.freeMemory;
	TextToPrint[0].push_back(text);

	HALF = (WOMA::AppSettings->WINDOW_WIDTH / 5) * 3;

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

		text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].DedicatedVideoMemory;
		TextToPrint[0].push_back(text);
		text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].DedicatedSystemMemory;
		TextToPrint[0].push_back(text);
		text.y += (int)LINE; text.label = systemDefinitions.GPUINFO[i].SharedSystemMemory;
		TextToPrint[0].push_back(text);
	}
#endif
	text.x = HALF;
	text.y = 10;
	// ----------------------------------

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
	StringCchPrintf(pstrFPS, 300, TEXT("%s"), WOMA::APP_FULLNAME);
#else
#if defined USE_ASTRO_CLASS
	#if _DEBUG
	StringCchPrintf(pstrFPS, 300, TEXT("FPS:%d(%4.1f ms) [%s] %s shader:%s state:%d - TOTAL VERTEX: %d"), SystemHandle->fps, (SystemHandle->fps==0)?1:1000.0f/SystemHandle->fps,
		m_Driver->driverName, WOMA::APP_FULLNAME,
		m_Driver->szShaderModel, WOMA::game_state, SystemHandle->TotalVertexCounter);
	#else
	StringCchPrintf(pstrFPS, 300, TEXT("FPS:%d(%4.1f ms) [%s] %s"), SystemHandle->fps, (SystemHandle->fps == 0) ? 1 : 1000.0f / SystemHandle->fps,
		m_Driver->driverName, WOMA::APP_FULLNAME);
	#endif
#else
	StringCchPrintf(pstrFPS, 300, TEXT("FPS:%d %s [%s] shader:%s state:%d - TOTAL VERTEX: %d"), SystemHandle->fps, WOMA::APP_FULLNAME, m_Driver->driverName, m_Driver->szShaderModel, WOMA::game_state, SystemHandle->TotalVertexCounter);
#endif
#endif

#if defined WINDOWS_PLATFORM && !defined ANDROID_PLATFORM
#if defined(X64) // Set the new "Window Title"
	PDWORD_PTR dwResult = 0;// In 64 Bits
	for (int i = 0; i < SystemHandle->windowsArray.size(); i++)
		if (ShouldDrawUI(i))
			SendMessageTimeout(SystemHandle->windowsArray[i].hWnd, WM_SETTEXT, 0, (LPARAM)pstrFPS, SMTO_ABORTIFHUNG, 1000, dwResult);
#else
	TCHAR		title[MAX_STR_LEN];
	DWORD dwResult = 0;		// In 32 Bits
	for (int i = 0; i < SystemHandle->windowsArray.size(); i++)
		if (ShouldDrawUI(i)) 
			SendMessageTimeout(SystemHandle->windowsArray[i].hWnd, WM_SETTEXT, 0, (LPARAM)pstrFPS, SMTO_ABORTIFHUNG, 1000, &dwResult);
		else
		{
			StringCchPrintf(title, 300, TEXT("WOMA ENGINE - SCR: %d"), i);
			SendMessageTimeout(SystemHandle->windowsArray[i].hWnd, WM_SETTEXT, 0, (LPARAM)title, SMTO_ABORTIFHUNG, 1000, &dwResult);
		}
#endif
#endif
}

#if defined USE_PROCESS_OS_KEYS //CORE_ENGINE_LEVEL >= 3
	#ifndef DIK_ESCAPE					// Will be defined @ ENGINE_LEVEL >= 24
	#define DIK_ESCAPE 0x01
	#endif

void SystemClass::CalculateCameraViewAndFrustum(void* pContext)
{
#if defined USE_WOMA_ENGINE_ONE_CBUFFER
	womalogATfirstframe(TEXT("[DEBUG] BuildGlobalVPArray check: DX_ENGINE_LEVEL=%d  RenderPage=%u\n"), DX_ENGINE_LEVEL, RENDER_PAGE);
#endif

	// SET CAMERA (for this monitor): Prepare to Take a Shot: Generate the view matrix based on the camera's position.

  #if defined DX_ENGINE
	if (WOMA::AppSettings->DRIVER != DRIVER_GL3)
	{
		auto* camera = DXsystemHandle->m_Camera;

		if (camera) 
		{
			// --------------------------------------------------------------------------------------
			// LEVEL 88+ : 3rd-person camera (default for gameplay or intro demo)
			// --------------------------------------------------------------------------------------
			#if defined USE_3RD_PERSON_CAMERA || defined USE_DEMO99
				IF_RENDER_PAGE(RENDER_PAGE >= 88)
				{
					if (g_GOD_MODE)
						camera->CalculateViewMatrix();
					else
						camera->CalculateViewMatrix_3rd_PersonCamera(main3rdPcamera.m_camYaw + XMConvertToRadians(camera->offsetDeg), main3rdPcamera.m_camPitch);
				}
			#endif
			// --------------------------------------------------------------------------------------
			// LEGACY LEVELS (<88) : simple first-person camera
			// --------------------------------------------------------------------------------------
			#if !defined USE_3RD_PERSON_CAMERA || defined INTRO_DEMO
				IF_RENDER_PAGE(RENDER_PAGE < 88)
					camera->CalculateViewMatrix();
			#endif
		}
	}
  #endif

  #if (defined OPENGL3 || defined OPENGL4)
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		GLopenGLclass* driver = (GLopenGLclass*)driverList[WOMA::AppSettings->DRIVER];
		if (driver->gl_Camera)
			driver->gl_Camera->CalculateViewMatrix();
	}
  #endif

	// ----------------------------------------------------------------------------
	// CONSTRUCT VIEW FRUSTUM (for visibility testing)
	// ----------------------------------------------------------------------------
#if defined USE_FRUSTUM
	IF_RENDER_PAGE(RENDER_PAGE >= 30)
	{
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
		m_Driver->frustum->ConstructFrustum(WOMA::AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX12Class*)m_Driver)->m_projectionMatrix,
			&DXsystemHandle->m_Camera->m_viewMatrix);
	#endif

	#if defined DX11 || defined DX9
	if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
		m_Driver->frustum->ConstructFrustum(WOMA::AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX11Class*)m_Driver)->m_projectionMatrix,
			&DXsystemHandle->m_Camera->m_viewMatrix);
	#endif

	#if (defined OPENGL3 || defined OPENGL4)
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3) {

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

		m_Driver->frustum->ConstructFrustum(WOMA::AppSettings->SCREEN_DEPTH / 2.5f,
			&m_projectionMatrix,
			&m_viewMatrix);
	}
	#endif
	}
#endif

	// ----------------------------------------------------------------------------
	// AFTER CAMERA CALCULATION – build global VP buffer, log matrices if needed
	// ----------------------------------------------------------------------------
}

#if DX_ENGINE_LEVEL >= 28 && defined WINDOWS_PLATFORM
void SystemClass::LogAllMatrix() 
{
	ASSERT_DEBUG(WOMA::logManager);
	{
		// Log all active camera & projection matrices
		DirectX::DX11Class* m_driver11 = (DirectX::DX11Class*)m_Driver;

	#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
		XMMATRIX* camNormalView = &DXsystemHandle->m_Camera->m_viewMatrix;
		XMMATRIX* camSkyView = &DXsystemHandle->m_CameraSKY->m_viewMatrix;
	#else
		XMMATRIX* camNormalView = &DXsystemHandle->m_Camera->m_viewMatrix;
		XMMATRIX* camSkyView = nullptr;
	#endif

	#if defined USE_MINIMAP_CAMERA && DX_ENGINE_LEVEL >= 63
		XMMATRIX* camMiniView = &DXsystemHandle->m_CameraMINIMAP->m_viewMatrix;
	#else
		XMMATRIX* camMiniView = nullptr;
	#endif

		womalog(TEXT("[View Matrices before BuildGlobalVPArray]\n"));
		womalog(TEXT("CAMERA_NORMAL View:\n"));

		DX_LOG_MATRIX(camNormalView);

		if (camSkyView)
		{
			womalog(TEXT("CAMERA_SKY View:\n"));
			DX_LOG_MATRIX(camSkyView);
		}

		if (camMiniView)
		{
			womalog(TEXT("CAMERA_MINIMAP View:\n"));
			DX_LOG_MATRIX(camMiniView);
		}

		womalog(TEXT("Projection Matrices:\n"));
		DX_LOG_MATRIX(&m_driver11->m_projectionMatrix);
		DX_LOG_MATRIX(&m_driver11->m_projectionMatrix_sky);
	#if DX_ENGINE_LEVEL >= 63 && defined USE_MINIMAP_CAMERA
		DX_LOG_MATRIX(&m_driver11->m_orthoMatrix);
	#endif
	}
}
#endif

//-----------------------------------------------------------------------------------------
void SystemClass::ProcessOS_Fx_Keys_Input() // This Function will be invoked several times per second
//-----------------------------------------------------------------------------------------
{
	//LEVEL 4 System
	//LEVEL 5 Setup
	//LEVEL 7 Astro
	//LEVEL 8 Map
	//LEVEL 9 Weather

	static bool first_time = false;

#if defined WINDOWS_PLATFORM

	// "ESC": DX Process Special: key is being pressed ? -> EXIT APPLICATION
#if CORE_ENGINE_LEVEL >= 10 && defined USE_DIRECT_INPUT
	if ((WOMA::game_state > GAME_MINIMIZED && WOMA::game_state < GAME_MAP) && (OS_KEY_DOWN(DIK_ESCAPE + 0x35)))
	{
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
		return;
	}
#endif

	// "ESC" OS Process Special: key is being pressed ? -> EXIT APPLICATION
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
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12 && !first_time)
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
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12 && !first_time)
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
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12 && !first_time)
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
		if (WOMA::AppSettings->FULL_SCREEN)
		{
			WOMA::AppSettings->FULL_SCREEN = false;
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
#if CORE_ENGINE_LEVEL >= 4 && defined GAME_SYSTEM_SETTINGS
	if (first_time || (OS_KEY_DOWN(VK_F1) && WOMA::game_state != GAME_SYSTEM_SETTINGS)) {
	  #if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
		if (SystemHandle->womaSetup)
			SAFE_SHUTDOWN(SystemHandle->womaSetup);
	  #endif
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12 && !first_time)
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

#if CORE_ENGINE_LEVEL >= 4 && defined WINDOWS_PLATFORM
	GETOS();
#endif

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
	womalogauto((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	womalogauto((TCHAR*)TEXT("SystemClass::Shutdown()\n"));
	womalogauto((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
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
#if (defined OPENGL3 || defined OPENGL40) 
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
	}
#endif

}
#if defined ANDROID_PLATFORM
extern android_app* app;
#endif

void SystemClass::FrameUpdate()
{

	#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	  #if !defined ANDROID_PLATFORM
		DXsystemHandle->GetInputs();					// READ-INPUT: WinSystemClass::ProcessInput() + DXInputClass::Frame()
	  #endif
	  #if defined DX_ENGINE
		DXsystemHandle->m_Input->GetDirectInputKeys();	// Process Keyboard keys / (DXInputClass)
	  #endif
	#endif

	#if defined USE_PROCESS_OS_KEYS && defined WINDOWS_PLATFORM
		ProcessOS_Fx_Keys_Input();						// Process Special function keys |ESC and F1 to F6|
		if (WOMA::game_state == ENGINE_RESTART)
			return;
	#endif
	
	#if CORE_ENGINE_LEVEL >= 4 && defined USE_TIMER_CLASS
		ProcessPerformanceStats();					    // Run every: FPS: m_Timer.Frame(); m_Fps.Frame(); m_Cpu.Frame();
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

// Used by: runtest_WindowsEngine_002-009-Core.bat
// Used by: runtest_WindowsEngine_020-028.bat
// Used by: runtest_WindowsEngine_030-038.bat
// Used by: runtest_WindowsEngine_039-053.bat
// Used by: runtest_WindowsEngine_054-065.bat
// Used by: runtest_WindowsEngine_070-078.bat
// Used by: runtest_WindowsEngine_082-90.bat
// Used by: runtest_WindowsEngine_090-98.bat
void SystemClass::ParseCommandLineAndApplySettings(int argc, char* argv[])
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

	//FORCE VSYNC on short demos:
	if (WOMA::renderOnce)
	{
		WOMA::AppSettings->VSYNC_ENABLED = true;
		WOMA::AppSettings->UseAllMonitors = false;
	}
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

#if DX_ENGINE_LEVEL == 29 || defined INTRO_DEMO // Force Full Screen
	#ifdef RELEASE
		WOMA::AppSettings->FULL_SCREEN = true;
	#endif
	WOMA::AppSettings->INIT_CAMZ = -20;
	WOMA::AppSettings->NETWORK_ENABLED = true;
#endif

	SystemHandle->LandScape = (WOMA::AppSettings->WINDOW_WIDTH >= WOMA::AppSettings->WINDOW_HEIGHT) ? true : false;

	//FORCE LANDSCAPE:

	return true;
}

bool SystemClass::LoadXmlWorld()
{
	womalog("===============================================================================\n");
	womalogauto(TEXT("LOAD XML WORLD: %s\n"), (TCHAR*)XML_WORLD_FILE.c_str());
	womalog("===============================================================================\n");

	// ----------------------------------------------------------------------------------------
	// 1️st ADD SPECIAL COLOR LINE (used for Sun Direction visualization)
	// ----------------------------------------------------------------------------------------

	// Load and Parse XML FILE:"world.xml" the WORLD file
	//----------------------------------------------------------------------------

  #if defined MAIN_RENDER_MAIN_XML_OBJ
	XML_WORLD_FILE = WOMA::PUBLIC_DOCUMENTS;
	XML_WORLD_FILE += WORLD_XML;
	if (!SystemHandle->xml_loader.InitWorldLoader(DX_ENGINE_LEVEL, (TCHAR*)XML_WORLD_FILE.c_str()))
	{
		STRING err = TEXT("LoadXmlSettings::World File not found/Invalid: "); err += XML_WORLD_FILE;
		WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "));
		return false;
	}
  #endif

	SystemHandle->m_Application->AddObjsWithInstancesToXML();	//77 || 90 || 98 (add lamps)

		//Create WOMA_OBJECTs for all these loaded XML objects:
		for (size_t i = 0; i < SystemHandle->xml_loader.theWorldXML.size(); i++)
		{
			SHADER_TYPE shader = (SHADER_TYPE)SystemHandle->xml_loader.theWorldXML[i].shader;
			SystemHandle->xml_loader.theWorldXML[i].WOMA_object = WOMA_OBJECT(shader, castShadows_false, renderShadows_false, modelHASlight_true, SystemHandle->xml_loader.theWorldXML[i].instances);
		}


	return true;
}

#endif

#if CORE_ENGINE_LEVEL >= 4 && defined USE_TIMER_CLASS
void SystemClass::ProcessPerformanceStats()
{
	// Update the system stats: (BEFORE: HandleUserInput)
	m_Timer.Frame();		// Calculate dT for animations (Measure last frame time)
	m_Fps.Frame();			// Increase the frame counter, calculate FPS once per second
	fps = m_Fps.GetFps();	// Get current FPS (updated by "m_Fps.Frame()" every second)

	m_Application->dt = (float)m_Timer.GetTime();		// Calculate dT for animations & camera movements (in Mili Seconds)
	//womalog("dt: %.2f:\n", m_Application->dt);

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
	womalog("LoadAllDrivers - DriverList:\n");
	UINT size = (UINT)driverList.size();
	if (size >= 1)
		if (driverList[0])
			womalog("driverList[0] = %s \n", driverList[0]->driverName);
	if (size >= 2)
		if (driverList[1])
			womalog("driverList[1] = %s \n", driverList[1]->driverName);
	if (size >= 3)
		if (driverList[2])
			womalog("driverList[2] = %s \n", driverList[2]->driverName);
	if (size >= 4)
		if (driverList[3])
			womalog("driverList[3] = %s \n", driverList[3]->driverName);

	womalog("XML Conf - Selected Driver [%d]\n", WOMA::AppSettings->DRIVER);

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
	womalog("SystemClass::LoadAllDrivers()\n");

#if	!defined NewWomaEngine
	// -------------------------------------------------------------------------------------------
	// [0] DX11 (or DX11 with Downgrade: DX10)
	// -------------------------------------------------------------------------------------------
#if defined ( DX11 )
	{
		womalog("LoadDriver[0]: DX11Class\n");//driver = g_contextDriver;	// Re-Use the same driver ( Context Driver )
		driverList.push_back(NEW DirectX::DX11Class());
}
#else
	driverList.push_back(NULL);
#endif

	// -------------------------------------------------------------------------------------------
	// [1] GL3+
	// -------------------------------------------------------------------------------------------
#if (defined OPENGL3 || defined OPENGL4) //Driver
	womalog("LoadDriver[1]: GLopenGLclass\n");
	driverList.push_back((WomaDriverClass*)NEW GLopenGLclass);
#else
	driverList.push_back(NULL);
#endif

	// -------------------------------------------------------------------------------------------
	// [2] DX 9 (or DX11 with Downgrade: DX9)
	// -------------------------------------------------------------------------------------------
	driverList.push_back(NULL);

	// -------------------------------------------------------------------------------------------
	// [3] DX 12
	// -------------------------------------------------------------------------------------------
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	{
		driverList.push_back(NEW DirectX::DX12Class());
		//driver = g_contextDriver;	// Re-Use the same driver ( Context Driver )
		womalog("LoadDriver[3]: DX12Class\n");
		}
#else
	driverList.push_back(NULL);
#endif
#endif

} 

//END: LoadAllDrivers()

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

#if DX_ENGINE_LEVEL >= 19 && LEVEL >= 10// || (LEVEL < 10 && defined ANDROID_PLATFORM)
bool InitSelectedDriver()
{
#ifdef INTRO_DEMO
    SystemHandle->m_Application->ClearColor[0] = 0;
    SystemHandle->m_Application->ClearColor[1] = 0;
    SystemHandle->m_Application->ClearColor[2] = 0;
    SystemHandle->m_Application->ClearColor[3] = 1;
#else
    SystemHandle->m_Application->ClearColor[0] = get_world_clearColorR();
    SystemHandle->m_Application->ClearColor[1] = get_world_clearColorG();
    SystemHandle->m_Application->ClearColor[2] = get_world_clearColorB();
    SystemHandle->m_Application->ClearColor[3] = 1.0f;
#endif

	/*******************************************************************
	// [INIT CONTEXT Driver]: ("g_contextDriver->OnInit()") Create: Rendering Context DRIVER! DirectX 11 or OPEN GL3+
	*******************************************************************/
	WOMA::Settings* AppSettings = WOMA::AppSettings;
#if !defined NewWomaEngine
	switch (WOMA::AppSettings->DRIVER)
	{
#if defined DX11 // [0] Pure DX11
	case DRIVER_DX11:
		IF_NOT_RETURN_FALSE(((DirectX::DX11Class*)(driverList[DRIVER_DX11]))->OnInit(WOMA::AppSettings->UI_MONITOR, SystemHandle->m_hWnd,
			WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, WOMA::AppSettings->DEPTH_BITS,
			WOMA::AppSettings->SCREEN_DEPTH, WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->MSAA_Anisotropic, WOMA::AppSettings->VSYNC_ENABLED,
			WOMA::AppSettings->FULL_SCREEN, WOMA::AppSettings->UseDoubleBuffering, WOMA::AppSettings->AllowResize));
		break;
#endif
#if (defined OPENGL3 || defined OPENGL4) //[1]
	case DRIVER_GL3:
        ASSERT(((GLopenGLclass*)(driverList[DRIVER_GL3]))->OnInit(WOMA::AppSettings->UI_MONITOR, SystemHandle->m_hWnd, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/, WOMA::AppSettings->SCREEN_DEPTH,
            WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->MSAA_Anisotropic, WOMA::AppSettings->VSYNC_ENABLED, WOMA::AppSettings->FULL_SCREEN,
            WOMA::AppSettings->UseDoubleBuffering, WOMA::AppSettings->AllowResize));
#if !defined ANDROID_PLATFORM
        ASSERT(g_contextDriver->OnInit(WOMA::AppSettings->UI_MONITOR, SystemHandle->m_hWnd, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/, WOMA::AppSettings->SCREEN_DEPTH, WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->MSAA_Anisotropic,
            WOMA::AppSettings->VSYNC_ENABLED, WOMA::AppSettings->FULL_SCREEN, WOMA::AppSettings->UseDoubleBuffering, WOMA::AppSettings->AllowResize));
#endif
		break;
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //[3]
	case DRIVER_DX12:
        ASSERT(((DirectX::DX12Class*)(driverList[DRIVER_DX12]))->OnInit(WOMA::AppSettings->UI_MONITOR, SystemHandle->m_hWnd, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/, WOMA::AppSettings->SCREEN_DEPTH,
            WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->MSAA_Anisotropic, WOMA::AppSettings->VSYNC_ENABLED, WOMA::AppSettings->FULL_SCREEN,
            WOMA::AppSettings->UseDoubleBuffering, WOMA::AppSettings->AllowResize));
		break;
#endif
};

	// [INIT MAIN Driver] "DX11" / "OpenGL": (Like: Select default color): "m_Driver->Initialize(clearColor)" [DX12] DXSample::Run OnInit();  LoadPipeline();
	if (!SystemHandle->InitializeDrivers(WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT, WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH,
		WOMA::AppSettings->VSYNC_ENABLED, WOMA::AppSettings->FULL_SCREEN, SystemHandle->m_Application->ClearColor))
		return false;

	womalog("-------------------------------------------------------------------------------\n");
	womalogauto(TEXT("Active Render Systems:\n"));
	womalog("-------------------------------------------------------------------------------\n");

#if defined DX_ENGINE
	womalogauto(TEXT("DX 9 Support: %s\n"), driverList[WOMA::AppSettings->DRIVER]->m_sCapabilities.CapDX9 ? TEXT("true") : TEXT("false"));		//Allow DX11, scale down to DX9, if needed?

#if defined DX11 // Pure DX11
	if (driverList[DRIVER_DX11]) //Driver 11 will give backward compatibility:
		womalogauto(TEXT("DX10 Support: %s\n"), driverList[WOMA::AppSettings->DRIVER]->m_sCapabilities.CapDX10_11 ? TEXT("true") : TEXT("false"));	//Allow DX11, scale down to DX10, if needed?		
	else
		womalogauto(TEXT("DX10 Support: false\n"));

	if (driverList[DRIVER_DX11])
		womalogauto(TEXT("DX11 Support: %s\n"), (((DirectX::DX11Class*)(driverList[DRIVER_DX11]))->ShaderVersionH >= 5) ? TEXT("true") : TEXT("false"));
	else
		womalogauto(TEXT("DX11 Support: false\n"));
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (driverList[DRIVER_DX12])
		womalogauto(TEXT("DX12 Support: %s\n"), (((DirectX::DX12Class*)(driverList[DRIVER_DX12]))->ShaderVersionH >= 6) ? TEXT("true") : TEXT("false"));
	else
		womalogauto(TEXT("DX12 Support: false\n"));
#endif
#endif

#if (defined OPENGL3 || defined OPENGL4)
	womalogauto(TEXT("OPENGL3 Support: true\n"));
#endif
#endif

	return true;
}

bool newDriver()
{
	WOMA::Settings* AppSettings = WOMA::AppSettings;

	womalog("===============================================================================\n");
	womalog("CREATE WINDOWS CONTEXT DRIVER\n");

	switch (WOMA::AppSettings->DRIVER)
	{
	case -1:
		WomaFatalException("AppSettings DRIVER not defined!");
		break;

	case DRIVER_GL3:
#if (defined OPENGL3 || defined OPENGL40) 
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
