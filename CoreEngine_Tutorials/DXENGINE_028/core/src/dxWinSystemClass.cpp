// --------------------------------------------------------------------------------------------
// Filename: dxWinSystemClass.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#if defined DX_ENGINE

#include "mem_leak.h"
#if defined WINDOWS_PLATFORM
#include "dxWinSystemClass.h"
#include "womadriverclass.h"
#include "packManager.h"
#include "OSmain_dir.h"
#include "fileLoader.h"

  #if defined DX11 || defined DX9
	#include "Dx11Class.h"
  #endif
  #if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
	#include "Dx12Class.h"
  #endif
    #if (defined OPENGL3 || defined OPENGL40) 
	#include "womadriverclass.h"	//woma
	#include "GLmathClass.h"		//woma	
	#include "GLopenGLclass.h"		//woma
    #if defined WINDOWS_PLATFORM
	#include "wGLopenGLclass.h"		// Windows
    #endif
  #endif

	#include "xml_loader.h"

	#include <ShlObj_core.h>
	
#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
#include <wincodec.h>
#include "ScreenGrab.h"
#include "DirectXHelpers.h"
#endif

#if defined USE_SCENE_MANAGER
#include "SceneManager.h"
#endif

#if defined USE_LOADING_THREADS
HANDLE threadInitializeLoaderHandle = NULL;
unsigned long threadInitializeLoaderId = NULL;
#endif

dxWinSystemClass* DXsystemHandle = NULL;



//----------------------------------------------------------------------------------
dxWinSystemClass::dxWinSystemClass(WOMA::Settings* appSettings) : WinSystemClass() //	SystemClass::SystemClass() Will Run!
//----------------------------------------------------------------------------------
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;
	WOMA::AppSettings = appSettings;
	WinSystemClass::mMaximized = WOMA::AppSettings->FULL_SCREEN;

	mResizing = false;
	DXsystemHandle = this;

	if (!m_Camera) {
		m_Camera = NEW DXcameraClass; // DX Implementation
		IF_NOT_THROW_EXCEPTION(m_Camera);
}

#if defined USE_DIRECT_INPUT
	joyFlags = NULL;
	m_Input = NULL;
	useJoystick = false;
#endif

#if defined USE_WIN32_PLAY_MUSIC || defined INTRO_DEMO
	MusicID = -1;
	MusicSourceID = -1;
#endif

}

dxWinSystemClass::~dxWinSystemClass()
{
	Shutdown();
	CLASSDELETE();
}

//----------------------------------------------------------------------------
bool dxWinSystemClass::APPLICATION_INIT_SYSTEM() //LOAD ALL GRAPHICS
//----------------------------------------------------------------------------
{
	// UNPACK Resources for DEMO:
#if CORE_ENGINE_LEVEL >= 7 && defined RELEASE
	InitPackLibs();											
#endif

	// RESET:
	driverList.clear();
#if defined DX_ENGINE
	DX11windowsArray.clear();
	FSAA_possibleValues.clear();
#endif

	// SystemCheck
	// ClassRegister
	// ApplicationInitMainWindow
	// LoadXMLWorld
	// InitOsInput(m_OsInput and m_InputManager)
	// LoadAllDrivers
	// InitSelectedDriver("driver"->OnInit (...) and "driver"->Initialize (clearColor))
	// InitializeSystemScreen(SETUP SCREENs: F1,F2,F3,F4,F5,F6)
	// InitSceneManager
	// PlayIntroMovie
	// LoadAllGraphicAssets
	// StartTimer
	bool res = WinSystemClass::APPLICATION_INIT_SYSTEM();	

	// START-AUDIO: Start Background Music

#if defined WINDOWS_PLATFORM && defined INTRO_DEMO
	ShowCursor(FALSE);
	SetCursor(NULL);
#endif

	return res;
}

//----------------------------------------------------------------------------
int dxWinSystemClass::APPLICATION_MAIN_LOOP()		// [RUN] - MAIN "INFINITE" LOOP!
//----------------------------------------------------------------------------
{
	MSG msg = { 0 };						// Reset MSG

    if (WOMA::renderOnce)
        WOMA::woma_timer = 0;

	do
	{
        BOOL gResult = TRUE;
		if ((gResult = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) > 0)	// There is any OS messages to handle?
		{
			TranslateMessage(&msg); // TranslateMessage produces WM_CHAR messages only for keys that are mapped to ASCII characters by the keyboard driver.
			DispatchMessage(&msg);  // Process MSG:  (INVOKE: WinSystemClass::MessageHandler)
		}

		if (WOMA::game_state > GAME_MINIMIZED)
			ProcessFrame();	// RENDER ONE: APPLICATION FRAME!
		else
			Sleep(100);     // We are in background? slowdown!

		if (WOMA::main_loop_state < 0 || (WOMA::renderOnce && WOMA::woma_timer > 30))
        {
			WOMA::game_state = GAME_STOP;
			if (WOMA::renderOnce && WOMA::woma_timer > 30)
				return EXIT_SUCCESS;                // Controlled Exit for Automatic Tests.
			break;										   
		}
        if (WOMA::game_state == ENGINE_RESTART)
		{
            PostQuitMessage(WOMA::game_state);		//RESTART ENGINE: return WOMA::game_state;
			break;
		}

	} while (msg.message != WM_QUIT);

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		Sleep(100);
	}

    womalog("msg.wParam: %d\n", msg.wParam);
	if (WOMA::game_state == ENGINE_RESTART)
		return ENGINE_RESTART;
	else
		return (int)msg.wParam; //return the PostQuitMessage (message code)
}

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG || defined USE_DEMO99 && defined MAVERICK
bool take_shot = false;
#endif

//----------------------------------------------------------------------------
void dxWinSystemClass::ProcessFrame() //RENDER ALL GRAPHICS
//----------------------------------------------------------------------------
{
	// Process Input, Timer, FPS and Render all GRAPHICs!
	WinSystemClass::ProcessFrame(); 

	// Process Special: "PRINT SCREEN" key or F10, the "Back-Buffer" have 1 frame rendered, so now we can dump it:
#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
	if (take_shot || (WOMA::game_state > GAME_MINIMIZED) && (OS_KEY_DOWN(DIK_SYSRQ + 0x35) || OS_KEY_DOWN(DIK_F10 + 0x35)))
	{
		ASSERT(SaveScreenshot());
		take_shot = false;

		womalog(TEXT("DXsystemHandle->m_Camera->m_viewMatrix"));
		DX_LOG_MATRIX(&DXsystemHandle->m_Camera->m_viewMatrix);
	}
#endif
}

//PRIVATE:
//----------------------------------------------------------------------------------
void dxWinSystemClass::Shutdown()
//----------------------------------------------------------------------------------
{
	//WinSystemClass::Shutdown();

	if (m_Camera)
	{
		delete ((DXcameraClass*)m_Camera); 
		m_Camera = NULL;
	}
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	if (m_CameraSKY)
	{
		delete ((DXcameraClass*)m_CameraSKY);
		m_CameraSKY = NULL;
	}
#endif
#if defined USE_DIRECT_INPUT
	for (UINT i = 0; i < HowManyPlayers; i++)
	{
		free(m_player[i]);
	}
#endif

#if defined USE_SCENE_MANAGER
	SAFE_SHUTDOWN(WOMA::sceneManager);
#endif

	// WinSystemClass Shutdown:
	womalogauto((TCHAR*)TEXT("WinSystemClass::Shutdown()\n"));
}

void dxWinSystemClass::GPH_RESIZE(void* pContext)
{
	if (!SystemHandle->m_Application)
		return;



	// Used for OS: ...
	//Recalculated AT WOMA_APPLICATION_InitGUI()
	//SystemHandle->m_scaleX = MIN(1, WOMA::AppSettings->WINDOW_WIDTH / 1920.0f);
	//SystemHandle->m_scaleY = MIN(1, WOMA::AppSettings->WINDOW_HEIGHT / 1080.0f);

	// Used for DX: ...
	SystemHandle->m_Application->scaleX = WOMA::AppSettings->WINDOW_WIDTH / 1920.0f;
	//SystemHandle->m_Application->scaleY = WOMA::AppSettings->WINDOW_HEIGHT / (1080.0f - 65);
	SystemHandle->m_Application->scaleY = WOMA::AppSettings->WINDOW_HEIGHT / 1080.0f;
	SystemHandle->m_Application->rescale = min(SystemHandle->m_Application->scaleX, SystemHandle->m_Application->scaleY);

	if (driverList.size() < 4)
		return;

	if (m_Driver) 
	{
		SystemHandle->m_Application->DEMO_WOMA_APPLICATION_Shutdown2D();

	switch (WOMA::AppSettings->DRIVER)
	{
	#if defined DX11 || defined DX9
		case DRIVER_DX9:
		case DRIVER_DX11:
			{
				UINT m = 0;
				if (SystemHandle->windowsArray.size() == SystemHandle->allWindowsArray.size())
					for (m = 0; m < SystemHandle->allWindowsArray.size(); m++)
					{
						BOOL full_screen;
						DX11windowsArray[0].m_swapChain1->GetFullscreenState(&full_screen, nullptr);
						((DirectX::DX11Class*)m_Driver)->Resize(m, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
							WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH,
							full_screen, WOMA::AppSettings->BITSPERPEL);
					}
				else {
					BOOL full_screen;
					DX11windowsArray[0].m_swapChain1->GetFullscreenState(&full_screen, nullptr);
					((DirectX::DX11Class*)m_Driver)->Resize(m, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
						WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH,
						full_screen, WOMA::AppSettings->BITSPERPEL);
				}
			}
			break;
	#endif
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			for (UINT m = 0; m < SystemHandle->allWindowsArray.size(); m++)
			((DirectX::DX12Class*)m_Driver)->Resize(m,WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
				WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH,
				WOMA::AppSettings->FULL_SCREEN, WOMA::AppSettings->BITSPERPEL);
			break;
	#endif
	}

  #if DX_ENGINE_LEVEL >= 24 && defined USE_VIEW2D_SPRITES
	SystemHandle->m_Application->DEMO_WOMA_APPLICATION_InitializeSprites2D(pContext);
  #endif
	}

}


void dxWinSystemClass::ProcessOSInput() // This Function will be invoked several times per second
{
#if CORE_ENGINE_LEVEL >= 5 && defined CLIENT_SCENE_SETUP
	womaSetup->Initialize(m_Driver);
#endif
}

bool dxWinSystemClass::ApplicationInitMainWindow()
{

	return true;
}

//----------------------------------------------------------------------------
void dxWinSystemClass::PAUSE()
{
#if defined USE_WIN32_PLAY_MUSIC || defined INTRO_DEMO
	if (WOMA::AppSettings->MUSIC_ENABLED)
		if (audio) 
			audio->pause(MusicSourceID);	
#endif
}

void dxWinSystemClass::UNPAUSE()
{
#if defined USE_WIN32_PLAY_MUSIC || defined INTRO_DEMO
	if (WOMA::AppSettings->MUSIC_ENABLED)
		if (audio) 
			audio->play(MusicSourceID);
#endif
}


#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
bool dxWinSystemClass::SaveScreenshot()
{
	bool result = false;
	WCHAR path_desktop[MAX_STR_LEN] = {0};
	DirectX::DX11Class* Driver = (DirectX::DX11Class*)m_Driver;

	#if false /*_NOTES*/
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
	HRESULT hr = DirectX::SaveWICTextureToFile(Driver->GetDeviceContext(), DX11windowsArray[0].m_backBuffer, GUID_ContainerFormatPng, path.c_str());		//op2
	if (hr == S_OK)
		return true;

	return result;
}
#endif

#if defined USE_SCENE_MANAGER
void dxWinSystemClass::InitSceneManager()
//----------------------------------------------------------------------------
{
	//  QUADTREE:
	// ||256|256||
	// -----------
	// ||256|256||

	set_world_size(512);
	set_world_patchSize(256);

	// SCENE MANAGER: Create SceneManager Engine: (Driver will use Frustum to filter)
#if defined USE_SCENE_MANAGER
    if (WOMA::sceneManager == NULL)
        WOMA::sceneManager = SceneManager::GetInstance();

	WOMA::sceneManager->CreateRootNode( get_world_size(), m_Application->ClearColor); // 256 x 8 --> //-512,-512, 512, 512
    WOMA::sceneManager->quadTree.Initialize(WOMA::sceneManager->RootNode);
#endif
}
#endif


#if defined USE_INTRO_VIDEO_DEMO 

void CALLBACK OnGraphEvent(HWND hwnd, long evCode, LONG_PTR param1, LONG_PTR param2)
{
	switch (evCode)
	{
	case EC_COMPLETE:
	case EC_USERABORT:
		DXsystemHandle->g_DShowPlayer->Pause(); //Stop();
		break;

	case EC_ERRORABORT:
		WomaMessageBox(TEXT("VIDEO: Playback error"), TEXT("Error: "));
		DXsystemHandle->g_DShowPlayer->Stop();
		break;
	}
}

HRESULT dxWinSystemClass::PlayIntroMovie(TCHAR* movie)
//----------------------------------------------------------------------------
{
	HRESULT hr = S_OK;
	STRING movie_file = movie;

	{
		hr = g_DShowPlayer->OpenFile((TCHAR*)movie_file.c_str());
		IF_FAILED_RETURN_FALSE(hr);

		InvalidateRect(m_hWnd, NULL, FALSE);
		g_DShowPlayer->Play();

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		g_DShowPlayer->UpdateVideoWindow(&rc);
	}

	return hr;
}
#endif

#if defined USE_LOADING_THREADS
//-----------------------------------------------------------------------------------------	
void InitializeObjectsLoaderThreadFunction() // InitializeThread
//-----------------------------------------------------------------------------------------	
{
    SetUnhandledExceptionFilter(TopLevelFilter);

	womalog("CreateThread: InitializeObjectsLoaderThreadFunction\n");

	switch (SystemHandle->systemManager->processorInfo.cpuCores.logicalProcessorCount)
	{
	case 1:
		WomaMessageBox(TEXT("Your Processor just have 1 core, this application will run very slow!"), TEXT("WARNING:") );
		break;

		//case 2:
		//case 4:
		//case 8:
	default:
		SystemHandle->m_Cpu.SetProcessorAffinity(1);  // Use CPU N.1 for loader threads!
		break;
	}

	bool loading = SystemHandle->LoadAllGraphicAssets();	// Load all main Graphics Objects, that will be rendered

	// We were minimized ? While in the load thread ?
	if (WOMA::game_state == GAME_MINIMIZED)
		WOMA::previous_game_state = GAME_RUN;

	WOMA::num_running_THREADS--; //InitializeObjectsLoaderThreadFunction
#if defined _DEBUG
	womalog("WOMA::num_running_THREADS: %d %s %s %d\n", WOMA::num_running_THREADS, __FILE__, __FUNCTION__, __LINE__);
#endif

	if (loading)
	{
		// WE ARE DONE:
		if (WOMA::game_state == GAME_LOADING)
			WOMA::game_state = GAME_RUN;
	} else {
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
	}
}
#endif

#if DX_ENGINE_LEVEL >= 20 && defined USE_LOADING_THREADS
bool dxWinSystemClass::ApplicationMandatoryLoad()
//----------------------------------------------------------------------------
{
	{
		switch (AppSettings->DRIVER)
		{

#if defined USE_LOADING_THREADS
		case DRIVER_DX9:
		case DRIVER_DX11:
		case DRIVER_DX12:
			threadInitializeLoaderHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitializeObjectsLoaderThreadFunction, (void*)this, 0, &threadInitializeLoaderId);
			ASSERT(threadInitializeLoaderHandle);
			WOMA::num_running_THREADS++;
			break;
#endif

		default:
			IF_NOT_RETURN_FALSE(SystemClass::LoadAllGraphicAssets());;	// (DX9 & Before Thread Support): [DX12] DXSample::Run OnInit();  LoadAssets();
			break;
		}
	}

	return true;
}
#endif

#endif

#endif
