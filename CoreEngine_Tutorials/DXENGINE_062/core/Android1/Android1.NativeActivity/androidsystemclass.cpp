// --------------------------------------------------------------------------------------------
// Filename: androidSystemClass.cpp
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
//WomaIntegrityCheck = 1234526920;

//ANDROID NATIVE APLICATION: http://developer.android.com/reference/android/app/NativeActivity.html

#include "OSengine.h"
#if defined ANDROID_PLATFORM
#include "SystemClass.h"
#include "AndroidSystemClass.h"
#include "AndroidEngine.h"
#include "OSmain_dir.h"
#include "mem_leak.h"
#include "stateMachine.h"
#include "vkClass.h"

AndroidSystemClass::AndroidSystemClass(WOMA::Settings* appSettings) : SystemClass()
{
    CLASSLOADER();
    WOMA::AppSettings = appSettings; // Super
    SystemHandle = this;

    //public:
    mResizing = false;

#if CORE_ENGINE_LEVEL >= 10
    mMaximized = WOMA::AppSettings->FULL_SCREEN;
    WOMA::previous_game_state = WOMA::game_state;
#endif

#if defined CLIENT_SCENE_SETUP
    womaSetup = NULL;
#endif

    // --------------------------------------------------------------
    // Init:
    // --------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 19
    #if !defined WIN_XP && defined USE_TIMER_CLASS
    m_Timer.Initialize();
    #endif

    m_Fps.Initialize();		// Start FPS Counting, Using: timeGetTime();

    #if defined WINDOWS_PLATFORM && !defined WIN_XP
    m_Cpu.Initialize();		// WINDOWS_PLATFORM: Initialize CPU Usage Counter
    #endif
#endif

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
    audio = NULL;
#endif
}

AndroidSystemClass::~AndroidSystemClass() 
{
    SystemHandle = NULL;
    CLASSDELETE();
}

void AndroidSystemClass::Shutdown()
{
    // AndroidSystemClass Shutdown:
    womalog("AndroidSystemClass::Stop()\n");

#if defined USE_SCENE_MANAGER
    //SceneManager* sceneManager = SceneManager::GetInstance();
    SAFE_SHUTDOWN(WOMA::sceneManager);
#endif

    SystemClass::Shutdown();

#if defined _DEBUG && defined WOMA_CONSOLE_APPLICATION
    //printf ("\npress ENTER to close, console window..."); getchar();
#endif	
}

#if defined USE_ALLOW_MAINWINDOW_RESIZE
void AndroidSystemClass::ONRESIZE(void* pContext)
{
	womalog("ONRESIZE()\n");
	womalog("ONRESIZE: %ux%u\n", WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT);

#if CORE_ENGINE_LEVEL >= 10
	if (pContext == NULL)
		pContext = getvoidcontext();
#endif

	if (SystemHandle)
	{
		if (SystemHandle->m_Application)
			SystemHandle->m_Application->WOMA_APPLICATION_InitGUI();
#if defined DX_ENGINE //OPENGL TODO
		if (DXsystemHandle) {
			DXsystemHandle->GPH_RESIZE(pContext);
		}
#endif
	}

#if defined USE_STATUSBAR
	if (!WOMA::AppSettings->FULL_SCREEN)
	{
		if (!SystemHandle->statusbar)
			SystemHandle->statusbar = DoCreateStatusBar(SystemHandle->m_hWnd, 0, m_hinstance, 1);

		SendMessage(SystemHandle->statusbar, SB_SETTEXT, 0, (LPARAM)DEMO_TITLE);
	}
#endif
}
#endif

void AndroidSystemClass::GPH_RESIZE(void* pContext)
{
#if CORE_ENGINE_LEVEL >= 6
	// Return if Application was not yet created:
	if (!SystemHandle->m_Application)
		return;

	//-------------------------------------
	// Update application scaling
	//-------------------------------------
	SystemHandle->m_Application->scaleX = WOMA::AppSettings->WINDOW_WIDTH / 1920.0f;
	SystemHandle->m_Application->scaleY = WOMA::AppSettings->WINDOW_HEIGHT / 1080.0f;
	SystemHandle->m_Application->rescale = min(SystemHandle->m_Application->scaleX, SystemHandle->m_Application->scaleY);
#endif

#if CORE_ENGINE_LEVEL >= 10
	if (m_Driver)
	{
		//-------------------------------------
		// Shutdown application 2D for scaling:
		//-------------------------------------
#if DX_ENGINE_LEVEL >= 22 || defined USE_VIEW2D_SPRITES
		SystemHandle->m_Application->DEMO_WOMA_APPLICATION_Shutdown2D();
#endif

		//-------------------------------------
		// Driver resize
		//-------------------------------------
		m_Driver->Resize(0, WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT,
			WOMA::AppSettings->SCREEN_NEAR, WOMA::AppSettings->SCREEN_DEPTH,
			false, WOMA::AppSettings->BITSPERPEL);


		//----------------------------------------
		// Recreate UI application 2D for scaling:
		//----------------------------------------
#if DX_ENGINE_LEVEL >= 24 && defined USE_VIEW2D_SPRITES
		SystemHandle->m_Application->DEMO_WOMA_APPLICATION_InitializeSprites2D(pContext);
#endif
	}
#endif
}

bool AndroidSystemClass::APPLICATION_CORE_SYSTEM()
{

#if CORE_ENGINE_LEVEL == 0
	WomaMessageBox(WOMA::strConsoleTitle, TEXT("WOMA Hello World!"), MB_OK);
	Sleep(3000);	//wait 3 seconds
	return !true;	//exit
#endif

#if CORE_ENGINE_LEVEL == 1	// WINDOWS Memory leak done on Purpose! (To test: Memory leak catch system)
	WomaMessageBox(WOMA::strConsoleTitle, TEXT("Memory leak done on Purpose for this Tutorial!\nCheck Visual Studio Output Console log for more info!\ndo a double click on windows console.\n"), true);
	UINT* p = NEW UINT[1];
	//free(p);
	Sleep(3000);	//wait 3 seconds
	return false;
#endif

#if CORE_ENGINE_LEVEL >= 2 && defined ANDROID_PLATFORM && !defined NewWomaEngine
	if (SystemHandle) 
	{
		WOMA::AppSettings->WINDOW_WIDTH = engine.width;
		WOMA::AppSettings->WINDOW_HEIGHT = engine.height;

		WOMA::WindowDataContainer used_screen;

		used_screen.MonitorName = TEXT("Android");

		used_screen.width = ANativeWindow_getWidth(engine.app->window);
		used_screen.height = ANativeWindow_getHeight(engine.app->window);

		used_screen.refreshRate_Numerator = 60;   // depois poderás obter o refresh real
		used_screen.refreshRate_Denominator = 1;

		// Android
		//used_screen.hWnd = engine.app->window;          // se usares hWnd como void*
		//used_screen.window = engine.app->window;        // ou apenas este, conforme a tua estrutura

		WOMA::windowsToRenderArray.push_back(used_screen);
		WOMA::allWindowsHwArray.push_back(used_screen);
	}
#endif
}

bool AndroidSystemClass::APPLICATION_INIT_SYSTEM()
{
    bool result = true;

    womalog ("AndroidSystemClass::APPLICATION_INIT_SYSTEM() START\n");

#if DX_ENGINE_LEVEL >= 29
#if defined USE_ANDROID_SOUND
	DownloadFiles(AUDIO_LINK, AUDIO_FILE);
	m_main_music_id = playAudio(AUDIO_FILE);
#endif
#endif

#if CORE_ENGINE_LEVEL >= 5 // Must be before: APPLICATION_INIT_MAIN_WINDOW()
#if defined USE_TINYXML_LOADER
	IF_NOT_RETURN_FALSE(SystemClass::LoadXmlSettings());	// XML: Load Application Settings: "settings.xml", pickup "Driver" to Use.
#endif
#endif

#if CORE_ENGINE_LEVEL >= 10
#ifdef INTRO_DEMO
    SystemHandle->m_Application->ClearColor[0] = 0;
    SystemHandle->m_Application->ClearColor[1] = 0;
    SystemHandle->m_Application->ClearColor[2] = 0;
    SystemHandle->m_Application->ClearColor[3] = 1;
#else
#if DX_ENGINE_LEVEL >= 62
    SystemHandle->m_Application->ClearColor[0] = world.clearColorR;
    SystemHandle->m_Application->ClearColor[1] = world.clearColorG;
    SystemHandle->m_Application->ClearColor[2] = world.clearColorB;
    SystemHandle->m_Application->ClearColor[3] = 1.0f;
#else
    SystemHandle->m_Application->ClearColor[0] = 0.5f;
    SystemHandle->m_Application->ClearColor[1] = 0.6f;
    SystemHandle->m_Application->ClearColor[2] = 0.8f;
    SystemHandle->m_Application->ClearColor[3] = 1.0f;
#endif
#endif
#endif

    // SYSTEM-CHECK:
#if CORE_ENGINE_LEVEL >= 4 && defined USE_SYSTEM_CHECK		// BEFORE: APPLICATION_INIT_MAIN_WINDOW()
    IF_NOT_RETURN_FALSE(SystemClass::SystemCheck());		// SYSTEM INFO: HW (OS, CPU, RAM, DiskFreeSpace, CPUFeatures) 
    womalog("==========================================================\n");
    womalogauto(TEXT("BEFORE SYSTEM START - CORE_ENGINE_LEVEL: %d\n"), CORE_ENGINE_LEVEL);
    womalog("==========================================================\n");
#endif

#if (defined USE_PROCESS_OS_KEYS || defined INTRO_DEMO)
    IF_NOT_RETURN_FALSE(SystemClass::InitOsInput());				// INIT-INPUT Devices, NOTE: After "Create MainWindow(s)"
#endif

#if defined USE_TINYXML_LOADER && DX_ENGINE_LEVEL >= 21
	IF_NOT_RETURN_FALSE(LoadXmlWorld());					// Load all static and semi-static from WORLD.XML and add Instances!
#endif

    //Create Context:
#if CORE_ENGINE_LEVEL >= 10 && (defined OPENGL3 || defined OPENGL4)	//Create NEW CONTEXT Class: m_contextDriver
    if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
        IF_NOT_RETURN_FALSE(newDriver());
#endif
#if CORE_ENGINE_LEVEL >= 10
    //----------------------------------------------------------------------------
    SystemClass::LoadAllDrivers();								// LOAD ALL DRIVERS: (DX9, DX11, DX12, OpenGL)

    if (!InitSelectedDriver())
        return false;

	// INIT SCENE-MANAGER:
#if defined USE_SCENE_MANAGER
	InitSceneManager();		// INIT QUAD TREE
#endif

#if DX_ENGINE_LEVEL >= 25
    m_Application->WOMA_APPLICATION_InitGUI();
#endif

    //----------------------------------------------------------------------------
	void* pContext = getvoidcontext();
	womalog("[CONTEXT] LOAD/INIT  pContext = %p\n", pContext);

	result = SystemClass::LoadAllGraphicAssets(pContext);	// Load all main Graphics, that will be rendered
	if (!result)
	{
		womalog("SystemClass::LoadAllGraphicAssets() failed\n");
		return false;
	}

    //----------------------------------------------------------------------------

    if (WOMA::game_state >= GAME_STOP)	// Something FATAL on loading "mandatory 2D/3D Stuff"?
        return false;					// (SAMPLE: misssing 3D/IMAGE/AUDIO file...)

#if !defined USE_LOADING_THREADS
    if (WOMA::game_state == GAME_LOADING)
        WOMA::game_state = GAME_RUN;
#endif
#endif

	womalog("AndroidSystemClass::APPLICATION_INIT_SYSTEM() END\n");

    return result;
}

#if CORE_ENGINE_LEVEL >= 10
#if defined _DEBUG || defined LINUX_PLATFORM || defined ANDROID_PLATFORM
extern UINT vkShaderClass_RenderShader_Calls;
#endif
#endif

#if CORE_ENGINE_LEVEL >= 2
void AndroidSystemClass::ProcessFrame()
{
	if (m_Driver->RenderfirstTime)
		womalog("AndroidSystemClass::ProcessFrame()\n");

	SystemClass::FrameUpdate();	// Process: (Fx keys |ESC and F1 to F6| & get FPS)

#if CORE_ENGINE_LEVEL >= 2
	if (WOMA::game_state == ENGINE_RESTART)
		return; //Restart? so, don't render, restart now.
#endif

#if CORE_ENGINE_LEVEL >= 10
	{
		m_Application->dayLightFade = m_Application->ProcessMovementInput_and_UpdateDemos();					//OS CORE ONLY!  F1, F2, ...

		#if defined INTRO_DEMO
		if (RENDER_PAGE < 15) 
		#else
		if (RENDER_PAGE < 10)
		#endif
			return;

		m_Application->RenderScene(NULL, 0, m_Driver);

	#if defined _DEBUG || defined LINUX_PLATFORM || defined ANDROID_PLATFORM
		if (m_Driver->RenderfirstTime)
		{

			womalog("Render calls: %u\n", vkShaderClass_RenderShader_Calls);

			vkShaderClass_RenderShader_Calls = 0;
			womalog("1st Frame ENDED OK: WinSystemClass::ProcessFrame()\n");		// 1st frame rendered!
			womalog("--------------------------------------------------\n");
			m_Driver->RenderfirstTime = false;
		}
	#endif
	}
#endif
}

#if CORE_ENGINE_LEVEL >= 3
void AndroidSystemClass::ProcessInput()
{
}
#endif
#endif

extern int InitImGui(HWND hwnd_ = NULL);
extern void ImGui_Stop();
int AndroidSystemClass::APPLICATION_MAIN_LOOP()
{
	womalog("AndroidSystemClass::APPLICATION_MAIN_LOOP()\n");
	womalog("WOMA::strConsoleTitle: %s\n", WOMA::strConsoleTitle);
//#if DX_ENGINE_LEVEL < 30
	WomaMessageBox(WOMA::strConsoleTitle, DEMO_TITLE, MB_OK);
	Sleep(3000);	//wait 3 seconds
//#endif

#if !defined NewWomaEngine
    while (WOMA::game_state < GAME_STOP)
    {
        process_events(&engine, engine.app);

        if (engine.has_focus_)
        {
            updateRenderArea(&engine);
            #if CORE_ENGINE_LEVEL >= 10 && defined USE_IMGUI && !RUN_ASMAIN
            if (WOMA::previous_game_state == GAME_IMGUI) {
                InitImGui();
            }
            else
            #endif
            #if CORE_ENGINE_LEVEL >= 2
            {
                SystemHandle->ProcessFrame();
            }
            #endif
        }
    }
#endif

#if CORE_ENGINE_LEVEL >= 10 && defined USE_IMGUI
    ImGui_Stop();
#endif

#if CORE_ENGINE_LEVEL >= 2
    ASSERT(WOMA::game_state == GAME_STOP);
#endif

    return S_OK;
}

#endif