// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: dxWinSystemClass.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"

#include "dxWinSystemClass.h"
#include "womadriverclass.h"
#include "mem_leak.h"
#include "packManager.h"
#include "OSmain_dir.h"

#include <d3d11.h>

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

dxWinSystemClass* DXsystemHandle = NULL;

//----------------------------------------------------------------------------------
dxWinSystemClass::dxWinSystemClass(WOMA::Settings* appSettings) : WinSystemClass() //	SystemClass::SystemClass() Will Run!
//----------------------------------------------------------------------------------
{
	CLASSLOADER();
	WinSystemClass::AppSettings = appSettings;
	WinSystemClass::mMaximized = WinSystemClass::AppSettings->FULL_SCREEN;

	mResizing = false;
	m_Driver = NULL;
	m_contextDriver = NULL;	// Note: Used only at 20

	DXsystemHandle = this;

#ifdef _NOT_

#if defined USE_INTRO_VIDEO_DEMO
	g_DShowPlayer = NULL;
#endif

	ASSERT(GuiHWND.size() == 0);
	ASSERT(SelectedItemHWND.size() == 0);

	XML_WORLD_FILE = TEXT("");

//#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
//	audio			= NULL;
//#endif
#endif
}

dxWinSystemClass::~dxWinSystemClass()
{
	Shutdown();
	CLASSDELETE();
}

bool dxWinSystemClass::InitSelectedDriver()
{
	/*******************************************************************
	// [INIT CONTEXT Driver]: ("m_contextDriver->OnInit()") Create: Rendering Context DRIVER! DirectX 11 or OPEN GL3+
	*******************************************************************/
	switch (AppSettings->DRIVER)
	{
	case DRIVER_DX11:
		ASSERT(((DirectX::DX11Class*)(driverList[DRIVER_DX11]))->OnInit(AppSettings->UI_MONITOR, m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_ENABLED, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
	case DRIVER_GL3:
		ASSERT(((GLopenGLclass*)(driverList[DRIVER_GL3]))->OnInit(AppSettings->UI_MONITOR, m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_ENABLED, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		ASSERT(m_contextDriver->OnInit(AppSettings->UI_MONITOR, m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_ENABLED, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#if defined DX9sdk //[2]
	case DRIVER_DX9:
		ASSERT(((DirectX::DX9Class*)(driverList[DRIVER_DX9]))->OnInit(AppSettings->UI_MONITOR, m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_ENABLED, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#endif
#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009 //[2]
	case DRIVER_DX9:
		ASSERT(((DirectX::DX11Class*)(driverList[DRIVER_DX9]))->OnInit(AppSettings->UI_MONITOR, m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_ENABLED, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //[3]
	case DRIVER_DX12:
		ASSERT(((DirectX::DX12Class*)(driverList[DRIVER_DX12]))->OnInit(AppSettings->UI_MONITOR, m_hWnd, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, 24 /*BufferDeep*/,
			AppSettings->SCREEN_DEPTH, AppSettings->SCREEN_NEAR, AppSettings->MSAA_ENABLED, AppSettings->VSYNC_ENABLED,
			AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize));
		break;
#endif
	};

	// [INIT MAIN Driver] "DX11" / "OpenGL": (Like: Select default color): "m_Driver->Initialize(clearColor)" [DX12] DXSample::Run OnInit();  LoadPipeline();
	if (!SystemClass::InitializeDrivers(AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, AppSettings->SCREEN_NEAR, AppSettings->SCREEN_DEPTH,
		AppSettings->VSYNC_ENABLED, AppSettings->FULL_SCREEN, SystemHandle->m_Application->ClearColor))
		return false;

	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("Active Render Systems:\n"));
	WOMA_LOGManager_DebugMSG("-------------------------------------------------------------------------------\n");

	WOMA_LOGManager_DebugMSGAUTO(TEXT("DX 9 Support: %s\n"), SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]->m_sCapabilities.CapDX9 ? TEXT("true") : TEXT("false"));		//Allow DX11, scale down to DX9, if needed?

#if defined DX9sdk
	if (driverList[DRIVER_DX9] && AppSettings->DRIVER == DRIVER_DX9)
		_tcscpy_s(m_Driver->driverName, sizeof(m_Driver->driverName), TEXT("DX9"));
#endif
	//#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
	//if (driverList[DRIVER_DX9] && AppSettings->DRIVER == DRIVER_DX9)
	//	_tcscpy_s(m_Driver->driverName, sizeof(m_Driver->driverName), TEXT("DX9*"));
	//#endif

	if (driverList[DRIVER_DX11]) //Driver 11 will give backward compatibility:
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX10 Support: %s\n"), SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]->m_sCapabilities.CapDX10_11 ? TEXT("true") : TEXT("false"));	//Allow DX11, scale down to DX10, if needed?		
	else
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX10 Support: false\n"));

	if (driverList[DRIVER_DX11])
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX11 Support: %s\n"), (((DirectX::DX11Class*)(driverList[DRIVER_DX11]))->ShaderVersionH >= 5) ? TEXT("true") : TEXT("false"));
	else
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX11 Support: false\n"));
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (driverList[DRIVER_DX12])
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX12 Support: %s\n"), (((DirectX::DX12Class*)(driverList[DRIVER_DX12]))->ShaderVersionH >= 6) ? TEXT("true") : TEXT("false"));
	else
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DX12 Support: false\n"));
#endif
}

bool dxWinSystemClass::InitializeSystem()
{
	WinSystemClass::InitializeSystem();
	
	LoadAllDrivers();					// LOAD ALL DRIVERS: Populate the List of ALL Graphics Drivers (DX9, DX11, DX12, OpenGL...)

	if (!InitSelectedDriver())
		return false;

	// MAIN LOADING THREAD:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	SystemClass::LoadAllGraphics();	//ApplicationClass::WOMA_APPLICATION_InitializeSprites2D()

	if (WOMA::game_state >= GAME_STOP) // Something FATAL on loading "mandatory 2D/3D Stuff"?
		return false;			 // (SAMPLE: misssing 3D/IMAGE/AUDIO file...)

	if (WOMA::game_state == GAME_LOADING)
		WOMA::game_state = GAME_RUN;

	return true;
}

//----------------------------------------------------------------------------
int dxWinSystemClass::ApplicationMainLoop()		// [RUN] - MAIN "INFINITE" LOOP!
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
	} while (msg.message != WM_QUIT);

	ASSERT(WOMA::game_state == GAME_STOP);

	return (int)msg.wParam;
}

//PRIVATE:
//----------------------------------------------------------------------------------
void dxWinSystemClass::Shutdown()
//----------------------------------------------------------------------------------
{
	//CHAMAR FILHO...
	//WinSystemClass::Shutdown();

#if defined USE_INTRO_VIDEO_DEMO
	if (g_DShowPlayer) {
		g_DShowPlayer->TearDownGraph();
		SAFE_DELETE(g_DShowPlayer);
	}
#endif



	// WinSystemClass Shutdown:
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("WinSystemClass::Shutdown()\n"));
}

void dxWinSystemClass::GPH_RESIZE()
{

	if (m_Driver) {

	switch (AppSettings->DRIVER)
	{
	#if defined DX9sdk
		case DRIVER_DX9:
			((DirectX::DX9Class*)m_Driver)->Resize(SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT,
					SystemHandle->AppSettings->SCREEN_NEAR, SystemHandle->AppSettings->SCREEN_DEPTH,
					SystemHandle->AppSettings->FULL_SCREEN, SystemHandle->AppSettings->BITSPERPEL);
	#endif
		case DRIVER_DX11:
			((DirectX::DX11Class*)m_Driver)->Resize(SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT,
				SystemHandle->AppSettings->SCREEN_NEAR, SystemHandle->AppSettings->SCREEN_DEPTH,
				SystemHandle->AppSettings->FULL_SCREEN, SystemHandle->AppSettings->BITSPERPEL);
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			((DirectX::DX12Class*)m_Driver)->Resize(SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT,
				SystemHandle->AppSettings->SCREEN_NEAR, SystemHandle->AppSettings->SCREEN_DEPTH,
				SystemHandle->AppSettings->FULL_SCREEN, SystemHandle->AppSettings->BITSPERPEL);
	#endif
	}

	}

}


void dxWinSystemClass::ProcessOSInput() // This Function will be invoked several times per second
{
	//CHAMAR FILHO...
	womaSetup->Initialize(m_Driver);
}

bool dxWinSystemClass::LoadXmlSettings()
{
	//CHAMAR FILHO...
	
	// Load and Parse XML [world.xml] the Configuration file
//----------------------------------------------------------------------------

	return true;
}

bool dxWinSystemClass::ApplicationInitMainWindow()
{
	//CHAMAR FILHO...


	return true;
}

//----------------------------------------------------------------------------
void dxWinSystemClass::PAUSE()
{
	//CHAMAR FILHO...

}

void dxWinSystemClass::UNPAUSE()
{
	//CHAMAR FILHO...

}

//----------------------------------------------------------------------------
void dxWinSystemClass::ProcessFrame()
//----------------------------------------------------------------------------
{
	//CHAMAR FILHO...
	WinSystemClass::ProcessFrame();

	// Process Special: "PRINT SCREEN" key, the "Back-Buffer" have 1 frame rendered, now we can dump it:
#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	if ((WOMA::game_state > GAME_MINIMIZED) && (OS_KEY_DOWN(DIK_SYSRQ + 0x35)))
		ASSERT(SaveScreenshot());
#endif

	if (WOMA::game_state == GAME_SETUP)
	{
		// Init WOMA Setup:
		if (!SystemHandle->womaSetup)
		{
			SystemHandle->womaSetup = NEW WomaSetupManager;
			SystemHandle->womaSetup->Initialize(NULL);
			OS_REDRAW_WINDOW;
		}
	}

	if (WOMA::game_state >= GAME_RUN && WOMA::game_state < ENGINE_RESTART)
	{
		// For each Monitor: Render one Application Frame
		for (int i = 0; i < windowsArray.size(); i++)
		{
			if (WOMA::game_state >= GAME_SYSTEM_SETTINGS)
			{
				if (m_Application->RENDER_PAGE >= 15)//OLD:20 now 15 to allow FADE BANNERS on INTRO_DEMO
				{
					m_Application->RenderScene(i);	// SystemHandle->m_Driver->BeginScene(monitorWindow);
					m_contextDriver->EndScene(i);	// SHOW: Present the FRAME successfully Rendered!
				}
			}
		}

		m_Driver->RenderfirstTime = false;	// NOTE: (After) FrameRender()
	}
}

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
// Source: HUMUS
bool dxWinSystemClass::SaveScreenshot()
{
	TCHAR path[MAX_STR_LEN];

	// STEP 1: Get Automatic Filename
#if defined(_WIN32)
	SHGetSpecialFolderPath(NULL, path, CSIDL_DESKTOPDIRECTORY, FALSE);
#elif defined(LINUX_PLATFORM) || defined(__APPLE__)
	strcpy(path, getenv("HOME"));
	strcat(path, "/Desktop");
#endif

	//FILE *file;
	size_t pos = strlen(path);

	strcpy_s(path + pos, MAX_STR_LEN, TEXT("/Screenshot00.png"));
	pos += 11;

	static int snapshot_counter = 1;
	path[pos] = '0' + (snapshot_counter / 10);
	path[pos + 1] = '0' + (snapshot_counter % 10);

	if (snapshot_counter++ >= 100)
		snapshot_counter = 1;

	// STEP 2: Capture Image from Current Driver
	ImageLoaderClass* img = NULL;
	img = m_Driver->CaptureScreenShot(AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT);

	// STEP 3: Save to File
	bool result = false;

	if (img)
		result = img->savePNG(path);

	SAFE_DELETE(img);

	return result;
}
#endif



#ifdef _NOT_

// ENGINE_LEVEL >= 1
#define _CRT_SECURE_NO_WARNINGS
#include "SystemPlatform.h"		// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "osDirectories.h"
#include "logManager.h"
//#include "../../SAMPLES/Include/svn.h"

#include "../../WomaUtils/include/TrigonometryMathClass.h" //sim, cos table

#define m_contextDriver NULL

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
#include "AudioClass.h"
#endif

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
#include "../../IMAGING/ImageLoaderClass.h"
#endif

#include <shlobj.h>

// Cant BE Inside Class:

#if defined LINUX_PLATFORM || defined WINDOWS_PLATFORM && !defined WOMA_WIN32_APPLICATION
int		ARGc;
char** ARGv;
#endif

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

#include "sdkddkver.h"


//-------------------------------------------------------------------------------------------
bool SystemClass::AFTER_SYSTEM_CHECK()
{
	WOMA_LOGManager_DebugMSG("==========================================================\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("BEFORE SYSTEM START - ENGINE_LEVEL: %d\n"), ENGINE_LEVEL);
	WOMA_LOGManager_DebugMSG("==========================================================\n");

	return true;	// NOTE: for a basic program without woma engine, return false.
}




void Init_Console_Chapters()
{

}










void SystemClass::StopAllThreads()
{
	// [STOP] - Wait for all threads to Stop:
	// ------------------------------------------------------------------------------------------
	WOMA_LOGManager_DebugMSG("WinSystemClass::Stop()\n");

	WOMA::game_state = GAME_STOP;

	int tries = 0;
#define WAIT_MS 100
	while (WOMA::game_state == GAME_STOP)
	{
		if (WOMA::num_running_THREADS == 0)
		{
			WOMA::game_state = GAME_EXIT; // All Threads are Stopped, we can now Stop Safelly and end the Show!
			WOMA_LOGManager_DebugMSG("===============================================================================\n");
			WOMA_LOGManager_DebugMSGAUTO(TEXT("SHUTDOWN - Waiting for thread(s) stop, %d (ms) \n"), tries * WAIT_MS);
			WOMA_LOGManager_DebugMSG("===============================================================================\n");
			break;
		}

		Sleep(WAIT_MS);			// Zzzz: Idle main thread.
		if (tries++ > 5000 / WAIT_MS) // Wait MAX: 5 seconds! (20 Cycle Tries)
		{
			WomaFatalException("PROBLEM! Waiting for thread(s) stop, exceed 1 second.");
			break;
		}
	}
}


#endif



#ifdef _NOT_
#include "packManager.h"
#include "xml_loader.h"

#include "SystemPlatform.h"		// To define OS [SystemHandle] Pointer (System Class) & define WomaSYSTEM for: WINDOWS, LINUX & ANDROID
#include "winsystemclass.h"

//----------------------------------------------------------------------------

#ifdef _NOT_

#if defined USE_INTRO_VIDEO_DEMO
void CALLBACK OnGraphEvent(HWND hwnd, long evCode, LONG_PTR param1, LONG_PTR param2)
{
	switch (evCode)
	{
	case EC_COMPLETE:
	case EC_USERABORT:
		SystemHandle->g_DShowPlayer->Pause(); //Stop();
		break;

	case EC_ERRORABORT:
		WomaFatalException("VIDEO: Playback error");
		SystemHandle->g_DShowPlayer->Stop();
		break;
	}
}
#endif

#endif //_NOT_

#if defined USE_INTRO_VIDEO_DEMO

HRESULT WinSystemClass::PlayIntroMovie(TCHAR* movie)
//----------------------------------------------------------------------------
{
	HRESULT hr = g_DShowPlayer->OpenFile(movie);
	IF_FAILED_RETURN_FALSE(hr);

	InvalidateRect(m_hWnd, NULL, FALSE);
	g_DShowPlayer->Play();

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	g_DShowPlayer->UpdateVideoWindow(&rc);

	MSG msg = { };
	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	// Process OS Messages
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if ((!threadLoadPacksAlive) && (g_DShowPlayer->State() != STATE_RUNNING))		// Video Ended? get out of here
			break;
	}

	return hr;
}
#endif


#endif //_NOT_


