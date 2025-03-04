// --------------------------------------------------------------------------------------------
// Filename: androidSystemClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : woma.no-ip.org
//
// PURPOSE:
//
// ********************************************************************************************

//ANDROID NATIVE APLICATION: http://developer.android.com/reference/android/app/NativeActivity.html

#include "OSengine.h"
#if defined ANDROID_PLATFORM
#include "SystemClass.h"
#include "AndroidSystemClass.h"
#include "AndroidEngine.h"
#include "OSmain_dir.h"
#include "mem_leak.h"
#include "stateMachine.h"

AndroidSystemClass::AndroidSystemClass(WOMA::Settings* appSettings) : SystemClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567222;
	AppSettings = appSettings; // Super
	SystemHandle = this;

	//public:
	mResizing = false;

#if CORE_ENGINE_LEVEL >= 10
	mMaximized = SystemHandle->AppSettings->FULL_SCREEN;
	WOMA::previous_game_state = WOMA::game_state;

	//m_OsInput = NULL;
#endif

	#if DX_ENGINE_LEVEL >= 19 // Initializing Engine
	//m_Driver = NULL;
	//m_contextDriver = NULL;	// Note: Used only at 20
	//m_eglOpenGL = 0;
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
	WOMA_LOGManager_DebugMSG("AndroidSystemClass::Stop()\n");

	// STOP	All: Loading/Worker Threads:
#if defined _NOT
	// STOP	All: Loading/Worker Threads:
	StopAllThreads();
#endif

#if defined USE_SCENE_MANAGER
	//SceneManager* sceneManager = SceneManager::GetInstance();
	SAFE_SHUTDOWN(sceneManager);
#endif

	SystemClass::Shutdown();

#if _NOT //DX_ENGINE_LEVEL >= 19 // Initializing Engine
	if (m_eglOpenGL)
	{
		m_eglOpenGL->Shutdown();// Release the OpenGL object.
		delete m_eglOpenGL;
		m_eglOpenGL = 0;
	}

	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
#endif

#if defined _DEBUG && defined WOMA_CONSOLE_APPLICATION
	//printf ("\npress ENTER to close, console window..."); getchar();
#endif	
}

#if defined _NOT
bool AndroidSystemClass::ApplicationCreateThreads()
{
#ifdef RELEASE // On DEBUG all are there already no need!
	#if ENGINE_LEVEL >= 10
		InitPackLibs();	// LOAD NOW: Urgent Resources (Used on Main Splash)!
	#endif

	// Load on Thread: The other Resources less urgent... (Used for Redering)!
	#if defined USE_LOADING_THREADS
		WOMA::num_running_THREADS++;
		threadLoadPacksHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartPackLibs, (void*) this, 0, &threadLoadPacksId);
		ASSERT(threadLoadPacksHandle);
	#endif
#endif

	return true;
}
#endif	

bool AndroidSystemClass::APPLICATION_INIT_SYSTEM()
{
	bool result = true;
	AppSettings->DRIVER = DRIVER_GL3; //Force OpenGL Driver

	WOMA_LOGManager_DebugMSG ("AndroidSystemClass::APPLICATION_INIT_SYSTEM()\n");

#if defined USE_ANDROID_SOUND
	m_main_music_id = playAudio(AUDIO_FILE);
#endif

#if defined ANDROID_PLATFORM && !defined NewWomaEngine
	if (SystemHandle) {
		SystemHandle->AppSettings->WINDOW_WIDTH = engine.width;
		SystemHandle->AppSettings->WINDOW_HEIGHT = engine.height;
}
#endif

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

#if CORE_ENGINE_LEVEL >= 5 // Must be before: ApplicationInitMainWindow()
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
	SystemHandle->m_Application->ClearColor[0] = SystemHandle->world.clearColorR;
	SystemHandle->m_Application->ClearColor[1] = SystemHandle->world.clearColorG;
	SystemHandle->m_Application->ClearColor[2] = SystemHandle->world.clearColorB;
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
#if CORE_ENGINE_LEVEL >= 4 && defined USE_SYSTEM_CHECK	// BEFORE: ApplicationInitMainWindow()
	IF_NOT_RETURN_FALSE(SystemClass::SystemCheck());		// SYSTEM INFO: HW (OS, CPU, RAM, DiskFreeSpace, CPUFeatures) 
	WOMA_LOGManager_DebugMSG("==========================================================\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("BEFORE SYSTEM START - CORE_ENGINE_LEVEL: %d\n"), CORE_ENGINE_LEVEL);
	WOMA_LOGManager_DebugMSG("==========================================================\n");
#endif

#if (defined USE_PROCESS_OS_KEYS || defined INTRO_DEMO)
	IF_NOT_RETURN_FALSE(SystemClass::InitOsInput());			// INIT-INPUT Devices, NOTE: After "Create MainWindow(s)"
#endif

	//Create Context:
#if CORE_ENGINE_LEVEL >= 10 && (defined OPENGL3 || defined OPENGL4)	//Create NEW CONTEXT Class: m_contextDriver
	if (AppSettings->DRIVER == DRIVER_GL3)
		IF_NOT_RETURN_FALSE(newDriver());
#endif
#if CORE_ENGINE_LEVEL >= 10
	//----------------------------------------------------------------------------
	SystemClass::LoadAllDrivers();					// LOAD ALL DRIVERS: (DX9, DX11, DX12, OpenGL)

	if (!InitSelectedDriver())
		return false;

#if DX_ENGINE_LEVEL >= 25
	m_Application->WOMA_APPLICATION_InitGUI();
#endif

	//----------------------------------------------------------------------------
	IF_NOT_RETURN_FALSE(SystemClass::LoadAllGraphicAssets());	// Load all main Graphics, that will be rendered

	//----------------------------------------------------------------------------

	if (WOMA::game_state >= GAME_STOP)	// Something FATAL on loading "mandatory 2D/3D Stuff"?
		return false;					// (SAMPLE: misssing 3D/IMAGE/AUDIO file...)

#if !defined USE_LOADING_THREADS
	if (WOMA::game_state == GAME_LOADING)
		WOMA::game_state = GAME_RUN;
#endif
#endif

	return result;
}

#if CORE_ENGINE_LEVEL >= 2
void AndroidSystemClass::ProcessFrame()
{
	// Process Input:
	SystemClass::FrameUpdate(); // Process: (INPUT + PerformanceStats) Only!

  #if CORE_ENGINE_LEVEL >= 10
  #if defined INTRO_DEMO
	if (RENDER_PAGE < 15)
  #else
	if (RENDER_PAGE < 10)
  #endif
		m_Application->Update();				//OS CORE ONLY!  F1, F2, ...
	else
	{
		// -----------------------
		//m_Driver->BeginScene(0);				//RESET FRAME
		// -----------------------
		#define cor driverList[SystemHandle->AppSettings->DRIVER]->driver_ClearColor
		glClearColor(cor[0], cor[1], cor[2], cor[3]);
		#undef cor
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		m_Application->RenderScene(0, m_Driver);//RENDER ONE FRAME: 100% is done here!

		// ------------------------------
		//if (!m_contextDriver)															//PRESENT FRAME
		//	m_Driver->EndScene(0);														//[DX]
		//else
		//	m_contextDriver->EndScene(0);												//[OPENGL]
		// ------------------------------
		bool swapResult = eglSwapBuffers(engine.display, engine.surface);	//[EGL]
		assert(swapResult == EGL_TRUE);
	}

	if (m_Driver->RenderfirstTime) {
		WOMA_LOGManager_DebugMSG("END:  AndroidSystemClass::ProcessFrame()\n");
		m_Driver->RenderfirstTime = false;
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

//struct engine* engine;

#if _NOT //DX_ENGINE_LEVEL >= 19
bool AndroidSystemClass::InitializeWindows(/*eGLopenGLclass*/ void* OpenGL, int& screenWidth, int& screenHeight)
{
const int   WIN_XPOS    = 50;
const int   WIN_YPOS    = 50;

	screenWidth = 640;
	screenHeight = 480;

	// Create Window
	// -----------------------------------------------------------------------------------------------
	// Android Basic Steps: http://jiggawatt.org/badc0de/android/index.html

    // Retrieves a display connection and initializes it.
    WOMA_LOGManager_DebugMSG("Connecting to the display.");

#if _NOT
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
		WomaMessageBox(TEXT("Error in eglGetDisplay"));
		return false;
	}


	// Init
	// -----------------------------------------------------------------------------------------------
	EGLint majorVersion, minorVersion;
    if (!eglInitialize((display, &majorVersion, &minorVersion)) 
		return false;
	WOMA_LOGManager_DebugMSGAUTO (TEXT("GL version: %d.%d\n"), majorVersion, minorVersion);
	WOMA_LOGManager_DebugMSGAUTO (TEXT("Window specs: %d*%d format=%d\n"), display->width, display->height, display->format);
#endif

	// Window surface that covers the entire screen, from libui.
	//displayWindow = android_createDisplaySurface();
	//ANativeWindow* displayWindow = ANativeWindow_fromSurface(env, javaSurface); //JNI
	//m_eglOpenGL->displayWindow = displayWindow;

#if _NOT
	// Create Context
	// -----------------------------------------------------------------------------------------------
	#if DX_ENGINE_LEVEL >= 19 // Initializing Engine
	// Initialize OpenGL now that the window has been created.
	if(!m_eglOpenGL->createContext(/*struct engine* engine*/ engine, AppSettings->WINDOW_WIDTH, AppSettings->WINDOW_HEIGHT, depthBits, SCREEN_DEPTH, SCREEN_NEAR, 
		VSYNC_ENABLED, AppSettings->FULL_SCREEN, AppSettings->UseDoubleBuffering, AppSettings->AllowResize))
	{
		WomaMessageBox(TEXT("Could not initialize createContext"));
		return false;
	}
	#endif
#endif
 
  return true;
}
#endif

#endif