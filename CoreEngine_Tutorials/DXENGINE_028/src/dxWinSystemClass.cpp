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
int USE_THIS_GRAPHIC_CARD_ADAPTER = 0;

#include "dxWinSystemClass.h"
#include "womadriverclass.h"
#include "mem_leak.h"
#include "packManager.h"
#include "OSmain_dir.h"
#include "fileLoader.h"

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

	#include "xml_loader.h"

	#include <ShlObj_core.h>
	#include <dinput.h>

#include <wincodec.h>
#include "ScreenGrab.h"
#include "DirectXHelpers.h"

dxWinSystemClass* DXsystemHandle = NULL;

//----------------------------------------------------------------------------------
dxWinSystemClass::dxWinSystemClass(WOMA::Settings* appSettings) : WinSystemClass() //	SystemClass::SystemClass() Will Run!
//----------------------------------------------------------------------------------
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;
	WinSystemClass::AppSettings = appSettings;
	WinSystemClass::mMaximized = WinSystemClass::AppSettings->FULL_SCREEN;

	mResizing = false;
	m_Driver = NULL;
	m_contextDriver = NULL;	// Note: Used only at 20

	DXsystemHandle = this;

	if (!m_Camera) {
		m_Camera = NEW DirectX::DXcameraClass; // DX Implementation
		//IF_NOT_THROW_EXCEPTION(m_Camera);
}

	joyFlags = NULL;
	m_Input = NULL;
	useJoystick = false;

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

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// 1
///////////////////////////////////////////////////////////////////////////////////////
bool dxWinSystemClass::InitializeSystem()
{
	WinSystemClass::InitializeSystem();	//ClassRegister/LoadXmlSettings/InitializeSetupScreen/ApplicationInitMainWindow/InitOsInput/StartTimer

	LoadAllDrivers();					// LOAD ALL DRIVERS: (DX9, DX11, DX12, OpenGL)

	if (!InitSelectedDriver())
		return false;

	//SINGLE THREAD SCENE MANAGER: QuadTree object Loader/Render
	IF_NOT_RETURN_FALSE(SystemClass::LoadAllGraphics());	// Load all main Graphics, that will be rendered
	
	//SOUND MANAGER: Music and sound effects

	//INTRO MOVIE: mpg player
#if defined USE_INTRO_VIDEO_DEMO	// START-VIDEO: Start DEMO INTRO (MP4): (Give Time to Unpack/Load Resources)
	g_DShowPlayer = NEW DShowPlayer(m_hWnd);
	IF_FAILED_RETURN_FALSE(PlayIntroMovie(WOMA::LoadFile(TEXT("engine/data/Logo.mp4"))));	// VIDEO DEMO
#endif

	//START LOADING THREADS: For detailed objects in the QuadTree

	if (WOMA::game_state >= GAME_STOP)	// Something FATAL on loading "mandatory 2D/3D Stuff"?
		return false;					// (SAMPLE: misssing 3D/IMAGE/AUDIO file...)

	if (WOMA::game_state == GAME_LOADING)
		WOMA::game_state = GAME_RUN;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// 3
///////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
void dxWinSystemClass::ProcessFrame()
//----------------------------------------------------------------------------
{
	WomaDriverClass* driver = SystemHandle->driverList[SystemHandle->AppSettings->DRIVER];

	WinSystemClass::ProcessFrame(); // Process Input, Timer and FPS

	if ((WOMA::game_state >= GAME_RUN && WOMA::game_state < ENGINE_RESTART) && (WOMA::game_state > GAME_SETUP))
	{
		// For each Monitor: Render one Application Frame
		for (int mon = 0; mon < windowsArray.size(); mon++)
		{
			if (RENDER_PAGE < 15)							//OS CORE FRAME ONLY!
				m_Application->Update(mon, driver);
			else
			{
				driver->BeginScene(mon);					//RESET FRAME

				m_Application->RenderScene(mon, driver);	//RENDER FRAME

				if (!m_contextDriver)						//PRESENT FRAME
					driver->EndScene(mon);					//[DX]
				else
					m_contextDriver->EndScene(mon);			//[OPENGL]
			}
		}

		m_Driver->RenderfirstTime = false;
	}

	// Process Special: "PRINT SCREEN" key, the "Back-Buffer" have 1 frame rendered, so now we can dump it:
	if ((WOMA::game_state > GAME_MINIMIZED) && (OS_KEY_DOWN(DIK_SYSRQ + 0x35)))
		ASSERT(SaveScreenshot());
}

//PRIVATE:
//----------------------------------------------------------------------------------
void dxWinSystemClass::Shutdown()
//----------------------------------------------------------------------------------
{
	//WinSystemClass::Shutdown();

	if (m_CameraSKY)
	{
		delete ((DirectX::DXcameraClass*)m_CameraSKY); 
		m_CameraSKY = NULL;
	}
	if (m_Camera)
	{
		delete ((DirectX::DXcameraClass*)m_Camera); 
		m_Camera = NULL;
	}

	for (UINT i = 0; i < HowManyPlayers; i++)
	{
		free(m_player[i]);
	}
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
		SystemHandle->m_Application->DEMO_WOMA_APPLICATION_Shutdown2D();

	switch (AppSettings->DRIVER)
	{
	#if defined DX9sdk
		case DRIVER_DX9:
			((DirectX::DX9Class*)m_Driver)->Resize(SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT,
					SystemHandle->AppSettings->SCREEN_NEAR, SystemHandle->AppSettings->SCREEN_DEPTH,
					SystemHandle->AppSettings->FULL_SCREEN, SystemHandle->AppSettings->BITSPERPEL);
	#endif
		case DRIVER_DX9:
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

	SystemHandle->m_Application->DEMO_WOMA_APPLICATION_InitializeSprites2D();
	}

}


void dxWinSystemClass::ProcessOSInput() // This Function will be invoked several times per second
{
	womaSetup->Initialize(m_Driver);
}
bool dxWinSystemClass::ApplicationInitMainWindow()
{

	return true;
}

//----------------------------------------------------------------------------
void dxWinSystemClass::PAUSE()
{
}

void dxWinSystemClass::UNPAUSE()
{
}


bool dxWinSystemClass::SaveScreenshot()
{
	bool result = false;
	WCHAR path_desktop[MAX_STR_LEN] = {0};
	DirectX::DX11Class* Driver = (DirectX::DX11Class*)DXsystemHandle->driverList[SystemHandle->AppSettings->DRIVER];

	#if defined _NOTES
	#define CSIDL_DESKTOP                   0x0000         // <desktop>
	#define CSIDL_INTERNET                  0x0001         // Internet Explorer (icon on desktop)
	#define CSIDL_PROGRAMS                  0x0002         // Start Menu\Programs
	#define CSIDL_CONTROLS                  0x0003         // My Computer\Control Panel
	#define CSIDL_PRINTERS                  0x0004         // My Computer\Printers
	#define CSIDL_PERSONAL                  0x0005         // My Documents
	#define CSIDL_FAVORITES                 0x0006         // <user name>\Favorites
	#define CSIDL_STARTUP                   0x0007         // Start Menu\Programs\Startup
	#define CSIDL_RECENT                    0x0008         // <user name>\Recent
	#define CSIDL_SENDTO                    0x0009         // <user name>\SendTo
	#define CSIDL_BITBUCKET                 0x000a         // <desktop>\Recycle Bin
	#define CSIDL_STARTMENU                 0x000b         // <user name>\Start Menu
	#define CSIDL_MYDOCUMENTS               CSIDL_PERSONAL //  Personal was just a silly name for My Documents
	#define CSIDL_MYMUSIC                   0x000d         // "My Music" folder
	#define CSIDL_MYVIDEO                   0x000e         // "My Videos" folder
	#define CSIDL_DESKTOPDIRECTORY          0x0010         // <user name>\Desktop
	#define CSIDL_DRIVES                    0x0011         // My Computer
	#define CSIDL_NETWORK                   0x0012         // Network Neighborhood (My Network Places)
	#define CSIDL_NETHOOD                   0x0013         // <user name>\nethood
	#define CSIDL_FONTS                     0x0014         // windows\fonts
	#define CSIDL_TEMPLATES                 0x0015		   
	#define CSIDL_COMMON_STARTMENU          0x0016         // All Users\Start Menu
	#define CSIDL_COMMON_PROGRAMS           0X0017         // All Users\Start Menu\Programs
	#define CSIDL_COMMON_STARTUP            0x0018         // All Users\Startup
	#define CSIDL_COMMON_DESKTOPDIRECTORY   0x0019         // All Users\Desktop
	#define CSIDL_APPDATA                   0x001a         // <user name>\Application Data
	#define CSIDL_PRINTHOOD                 0x001b         // <user name>\PrintHood
	#endif

	// STEP 1: Get Automatic Filename
#if defined(_WIN32)
	SHGetSpecialFolderPathW(NULL, path_desktop, CSIDL_DESKTOPDIRECTORY, FALSE);
#elif defined(LINUX_PLATFORM) || defined(__APPLE__)
	strcpy(path, getenv("HOME"));
	strcat(path, "/Desktop");
#endif

	std::wstring path= path_desktop;
	size_t pos = path.length();
	//path.append(L"/Screenshot00.jpg");	//op1
	path.append(L"/Screenshot00.png");		//op2

	static int snapshot_counter = 1;
	path[pos+11] = '0' + (snapshot_counter / 10);
	path[pos + 12] = '0' + (snapshot_counter % 10);

	if (snapshot_counter++ >= 100)
		snapshot_counter = 1;
	/*
	GUID_ContainerFormatBmp
	GUID_ContainerFormatPng				//op2
	GUID_ContainerFormatIco
	GUID_ContainerFormatJpeg			//op1
	GUID_ContainerFormatTiff
	GUID_ContainerFormatGif
	GUID_ContainerFormatWmp
	GUID_ContainerFormatDds
	*/
	HRESULT hr = DirectX::SaveWICTextureToFile(Driver->m_deviceContext, Driver->DX11windowsArray[0].m_backBuffer, GUID_ContainerFormatPng, path.c_str());		//op2
	if (hr == S_OK)
		return true;

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////
// 2
///////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
int dxWinSystemClass::ApplicationMainLoop()		// [RUN] - MAIN "INFINITE" LOOP!
//----------------------------------------------------------------------------
{
	MSG msg = { 0 };						// Reset msg

	//MAIN THREAD RELEASE BUILD LOOP:
	//MAIN DEBUG BUILD LOOP:
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
				ProcessFrame();// Render ONE: Application Frame
			else {
				if (WOMA::game_state == ENGINE_RESTART)
					return ENGINE_RESTART;
				else
					Sleep(50); //we are minized or in background, slow down CPU & GPU.
			}
		}
	} while (msg.message != WM_QUIT);

	ASSERT(WOMA::game_state == GAME_STOP);

	return (int)msg.wParam;
}


