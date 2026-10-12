// --------------------------------------------------------------------------------------------
// Filename: dxWinSystemClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234526920;

#include "OSengine.h"

#include "mem_leak.h"
#if defined WINDOWS_PLATFORM
#include "dxWinSystemClass.h"
#include "womadriverclass.h"
#include "packManager.h"
#include "OSmain_dir.h"
#include "fileLoader.h"

#include "DXmodelClass.h"

//
// Declare Drivers:
//
#include "womadriverclass.h"	//woma

#if defined DX11 || defined DX9
#include "Dx11Class.h"
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif

#if (defined OPENGL3 || defined OPENGL4)
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif

#if defined VULKAN
#include "vkClass.h"
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
	WOMA::AppSettings = appSettings;
	WinSystemClass::mMaximized = WOMA::AppSettings->FULL_SCREEN;

	mResizing = false;
	DXsystemHandle = this;

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

// ---------------------------------------------------------------------------
// APPLICATION_INIT_SYSTEM()
//
// Creates the complete runtime environment required by the engine.
//
// Responsibilities:
//
//  • System validation
//  • Window creation
//  • Input initialization
//  • Graphics driver creation
//  • Screen initialization
//  • Scene manager creation
//  • Asset loading
//  • Timer startup
//  • Audio startup
//
// After this function returns successfully,
// the application has created all mandatory runtime resources required
// to render the first frame.
// 
// Additional assets may continue loading asynchronously in the background.
//
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
#if defined WINDOWS_PLATFORM && (defined DX11 || defined DX9)
	DX11windowsToRenderArray.clear();
	FSAA_possibleValues.clear();
#endif

	//
	//	Runtime initialization sequence: APPLICATION_INIT_SYSTEM()
	//
	//	├── System validation        → SystemCheck()
	//	├── Register classes         → ClassRegister()
	//	├── Create main window       → ApplicationInitMainWindow()
	//	├── Load World.xml           → LoadXMLWorld()
	//	├── Initialize input         → InitOsInput()
	//	├── Enumerate drivers        → LoadAllDrivers()
	//	├── Create graphics driver   → InitSelectedDriver()
	//	├── Create screens           → InitializeSystemScreen()
	//	├── Create scene manager     → InitSceneManager()
	//	├── Play intro               → PlayIntroMovie()
	//	├── Load graphic assets      → LoadAllGraphicAssets()
	//	└── Start timer              → StartTimer()
	//
	bool res = WinSystemClass::APPLICATION_INIT_SYSTEM();	

	// START-AUDIO: Start Background Music
#if DX_ENGINE_LEVEL >= 29 && defined USE_WIN32_SOUND_MANAGER	
	IF_NOT_RETURN_FALSE(StartSoundManager());	 // (NOTE: After the INIT "rendering-device")
#endif

#if defined INTRO_DEMO //LEVEL29
	ShowCursor(FALSE);
	SetCursor(NULL);
#endif

	return res;
}

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
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

		womalog(TEXT("DX_Camera->m_viewMatrix"));
		DX_LOG_MATRIX(&DX_Camera->m_viewMatrix);
	}
#endif
}

//PRIVATE:
//----------------------------------------------------------------------------------
void dxWinSystemClass::Shutdown()
//----------------------------------------------------------------------------------
{
	//WinSystemClass::Shutdown();

#if defined USE_INTRO_VIDEO_DEMO
	if (g_DShowPlayer) {
		g_DShowPlayer->TearDownGraph();
		SAFE_DELETE(g_DShowPlayer);
	}
#endif

#if defined USE_SCENE_MANAGER
	SAFE_SHUTDOWN(WOMA::sceneManager);
#endif

#if DX_ENGINE_LEVEL >= 29 && (defined USE_WIN32_SOUND_MANAGER || defined USE_WIN32_PLAY_MUSIC)
	SAFE_SHUTDOWN(audio);
#endif

	// WinSystemClass Shutdown:
	womalogauto((TCHAR*)TEXT("WinSystemClass::Shutdown()\n"));
}

void dxWinSystemClass::GPH_RESIZE(void* pContext)
{
	// Return if Application was not yet created:
	if (!SystemHandle->m_Application || driverList.size() == 0)
		return;

	//-------------------------------------
	// Update application scaling
	//-------------------------------------
	SystemHandle->m_Application->scaleX = WOMA::AppSettings->WINDOW_WIDTH / 1920.0f;
	SystemHandle->m_Application->scaleY = WOMA::AppSettings->WINDOW_HEIGHT / 1080.0f;
	SystemHandle->m_Application->rescale = min(SystemHandle->m_Application->scaleX, SystemHandle->m_Application->scaleY);

	if (m_Driver) 
	{
	//-------------------------------------
	// Driver resize
	//-------------------------------------
	UINT m = 0;
	if (WOMA::windowsToRenderArray.size() == WOMA::allWindowsHwArray.size())
		for (m = 0; m < WOMA::allWindowsHwArray.size(); m++)
		{
			m_Driver->Resize(m, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
				WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH,
				false, WOMA::AppSettings->BITSPERPEL);
		}
	else {
		m_Driver->Resize(m, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
			WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH,
			false, WOMA::AppSettings->BITSPERPEL);
	}

	womalog("AppSettings: %ux%u\n", WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT);

#ifdef VULKAN
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		auto& vkWindow = ((vkClass*)m_Driver)->VKwindowsToRenderArray[0];
		womalog("CurrentExtent: %ux%u\n",
			vkWindow.surfCapabilities.currentExtent.width,
			vkWindow.surfCapabilities.currentExtent.height);
	}
#endif

	m_Driver->Pending2DResize = true;
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
	womalog("dxWinSystemClass::PAUSE()\n");
#if defined USE_WIN32_PLAY_MUSIC || defined INTRO_DEMO
	if (WOMA::AppSettings->MUSIC_ENABLED)
		if (audio) 
			audio->pause(MusicSourceID);	
#endif

	WinSystemClass::PAUSE();
}

void dxWinSystemClass::UNPAUSE()
{
	womalog("dxWinSystemClass::UNPAUSE()\n");
#if defined USE_WIN32_PLAY_MUSIC || defined INTRO_DEMO
	if (WOMA::AppSettings->MUSIC_ENABLED)
		if (audio) 
			audio->play(MusicSourceID);
#endif

	WinSystemClass::UNPAUSE();
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
	HRESULT hr = DirectX::SaveWICTextureToFile(Driver->GetDeviceContext(), DX11windowsToRenderArray[0].m_backBuffer, GUID_ContainerFormatPng, path.c_str());		//op2
	if (hr == S_OK)
		return true;

	return result;
}
#endif

#if DX_ENGINE_LEVEL >= 29 && (defined USE_WIN32_SOUND_MANAGER || defined USE_WIN32_PLAY_MUSIC)
bool dxWinSystemClass::StartSoundManager()
//----------------------------------------------------------------------------
{
#if DX_ENGINE_LEVEL >= 29 && (defined USE_WIN32_SOUND_MANAGER || defined USE_WIN32_PLAY_MUSIC)
	audio = NEW AudioClass;
  #if defined USE_WIN32_PLAY_MUSIC || defined INTRO_DEMO
	if (WOMA::AppSettings->MUSIC_ENABLED)
	{
		if (audio) {
			MusicID = audio->addSound(WOMA::LoadFile(TEXT(AUDIO_FILE)), NULL);	// Load Sound
			if (MusicID < 0)
			{
				WomaFatalException("Error loading Demo Music"); return false;
			}
		
			MusicSourceID = audio->addSoundSource(MusicID, LOOPING);			// Add Sound to player
			audio->play(MusicSourceID);											// Play it, now.
		}
	}
  #endif
#endif

	return true;
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

