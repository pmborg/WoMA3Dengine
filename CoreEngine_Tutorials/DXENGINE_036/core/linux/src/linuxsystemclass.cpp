// --------------------------------------------------------------------------------------------
// Filename: linuxSystemClass.h
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
//WomaIntegrityCheck = 1234525217;
// 
// Eclipse Installation of C++ development for cygwin
// http://www.badprog.com/c-eclipse-installation-of-c-c-development-tools-cdt-and-cygwin-for-windows

// MORE INFO: https://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_%28GLX%29

#include "OSengine.h" //#include "linuxsystemclass.h"
#if defined LINUX_PLATFORM
#include <gtk/gtkmain.h>
#include "OSmain_dir.h"

#define ENGINE_LEVEL DX_ENGINE_LEVEL

#if defined USE_IDEA_PACK
#include "packManager.h"
#endif
#if defined USE_TINYXML_LOADER
#include "xml_loader.h"
#endif

#include "fileLoader.h"

	#include "womadriverclass.h"

	#include <math.h>
	#include "GLmathClass.h"

	#include "GLopenGLclass.h"
	#include "glxOpenGLclass.h"	// Linux

extern MyWin Win;

#if CORE_ENGINE_LEVEL >= 7 && defined USE_ASTRO_CLASS
#include "initWorld.h"	//TIMER 2
#endif

#if defined USE_SCENE_MANAGER
#include "SceneManager.h"
#endif


#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>

extern int MainWindowPaint();
extern void mainLoop();

///////////////////
// Linux GLOBALS //
///////////////////

XFontStruct* fontinfo = NULL;

int	NUM_SAMPLES = 4;
Display		*display = NULL;
//XFontStruct *fontinfo = NULL;
XGCValues	gr_values;
GC			graphical_context;

void linux_audio_shutdown();
SoundClass* linux_play_wav(std::string filename);

//----------------------------------------------------------------------------

double elapsedMsec( const struct timeval &start, const struct timeval &stop )
{
  return ( ( stop.tv_sec  - start.tv_sec  ) * 1000.0 +
           ( stop.tv_usec - start.tv_usec ) / 1000.0 );
}
 
double elapsedUsec( const struct timeval &start, const struct timeval &stop )
{
  return ( ( stop.tv_sec  - start.tv_sec  ) * 1000000.0 +
           ( stop.tv_usec - start.tv_usec ) );
}

//----------------------------------------------------------------------------
LinuxSystemClass::LinuxSystemClass(WOMA::Settings* appSettings) : SystemClass()
//----------------------------------------------------------------------------
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();

	AppSettings = appSettings; // Super
	SystemHandle = this;

	//public:
	mResizing = false;

	mMaximized = SystemHandle->AppSettings->FULL_SCREEN;
	WOMA::game_state = WOMA::previous_game_state;
	//WOMA::previous_game_state = WOMA::game_state;

	//m_Driver = NULL;
	//g_contextDriver = NULL;	// Note: Used only at 20

#if defined CLIENT_SCENE_SETUP
	womaSetup = NULL;
#endif

	// --------------------------------------------------------------
	// Init:
	// --------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 19 && defined WINDOWS_PLATFORM
	#if !defined WIN_XP
	m_Timer.Initialize();
	#endif

	m_Fps.Initialize();		// Start FPS Counting, Using: timeGetTime();

#endif

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
	audio = NULL;
#endif
}

LinuxSystemClass::~LinuxSystemClass()
{
	SystemHandle = NULL;
	CLASSDELETE();
#if LEVEL >= 2
	if (Win.display && Win.window)
		XClearWindow(Win.display, Win.window);
#endif
	gtk_main_quit(); // /usr/include/gtk-3.0/gtk/gtkmain.h
}

//Stop() --> Shutdown()
void LinuxSystemClass::Shutdown()
{
	// LinuxSystemClass Shutdown:
	womalog ("LinuxSystemClass::Stop()\n");

#if defined USE_SCENE_MANAGER
	//SceneManager* sceneManager = SceneManager::GetInstance();
	SAFE_SHUTDOWN (sceneManager);
#endif

	// Destroy Drivers:
	SystemClass::Shutdown();
#if DX_ENGINE_LEVEL >= 19 && LEVEL >= 10
	if(g_contextDriver) { (g_contextDriver)->Shutdown(); delete ((glxOpenGLClass*)g_contextDriver); g_contextDriver=NULL; } 
#endif

#if defined _DEBUG && defined WOMA_CONSOLE_APPLICATION
	//printf ("\npress ENTER to close, console window..."); getchar();
#endif

	linux_audio_shutdown();
}

int LinuxSystemClass::APPLICATION_MAIN_LOOP()
{
	mainLoop();
	return WOMA::game_state;//0;
}

extern int new_main( int argc, char *argv[] );
// LINUX
extern int InitImGui(HWND hwnd_ = NULL);

bool LinuxSystemClass::APPLICATION_CORE_SYSTEM()
{

  #if defined LINUX_PLATFORM && DX_ENGINE_LEVEL >= 22
	STRING dir = WOMA::getCurrentDir();
	STRING newdir = dir;
	newdir.append("/../../../CoreEngine_Tutorials/DXENGINE_0");
	newdir.append(std::to_string(DX_ENGINE_LEVEL));
	int res = chdir(newdir.c_str());
  #endif

	return true;
}

// APP SYSTEM: START POINT
//----------------------------------------------------------------------------
bool LinuxSystemClass::APPLICATION_INIT_SYSTEM() // ApplicationInit()
{
	bool result = true;
	AppSettings->DRIVER = DRIVER_GL3; //Force OpenGL Driver

	womalog("LinuxSystemClass::APPLICATION_INIT_SYSTEM()\n");

	linux_play_wav(AUDIO_FILE);

	IF_NOT_RETURN_FALSE(APPLICATION_CORE_SYSTEM());

#if defined USE_TINYXML_LOADER // Must be before: APPLICATION_INIT_MAIN_WINDOW()
	IF_NOT_RETURN_FALSE(SystemClass::LoadXmlSettings());		// XML: Load Application Settings: "settings.xml", pickup "Driver" to Use.
#endif

#ifdef INTRO_DEMO
	SystemHandle->m_Application->ClearColor[0] = 0;
	SystemHandle->m_Application->ClearColor[1] = 0;
	SystemHandle->m_Application->ClearColor[2] = 0;
	SystemHandle->m_Application->ClearColor[3] = 1;
#else
	SystemHandle->m_Application->ClearColor[0] = 0.5f;
	SystemHandle->m_Application->ClearColor[1] = 0.6f;
	SystemHandle->m_Application->ClearColor[2] = 0.8f;
	SystemHandle->m_Application->ClearColor[3] = 1.0f;
#endif

#if CORE_ENGINE_LEVEL >= 7 && defined RELEASE
	InitPackLibs();	// LOAD NOW: Urgent Resources: windows.pck
#endif

#if CORE_ENGINE_LEVEL >= 4 && defined USE_SYSTEM_CHECK	// BEFORE: APPLICATION_INIT_MAIN_WINDOW()
	IF_NOT_RETURN_FALSE(SystemClass::SystemCheck());		// SYSTEM INFO: HW (OS, CPU, RAM, DiskFreeSpace, CPUFeatures) 
#endif

	IF_NOT_RETURN_FALSE(m_Application->WOMA_APPLICATION_InitGUI());
	womalog("WOMA_APPLICATION_InitGUI() - DONE\n");

	// WINDOWS with CONTEXT:
	IF_NOT_RETURN_FALSE(APPLICATION_INIT_MAIN_WINDOW(g_contextDriver));	// Create the window the application will be using and also initialize OpenGL.

	InitializeSystemScreen(10, 10);			// SETUP SCREEN: F1,F2,F3,F4

#if defined USE_PROCESS_OS_KEYS
	IF_NOT_RETURN_FALSE(InitOsInput());			// INIT-INPUT Devices, NOTE: After "Create MainWindow(s)"
#endif

#if CORE_ENGINE_LEVEL >= 10 && defined OPENGL3	//Create NEW CONTEXT Class: g_contextDriver
	if (AppSettings->DRIVER == DRIVER_GL3)
		IF_NOT_RETURN_FALSE(newDriver());
#endif

	// Step 2: Create Context Driver
	//----------------------------------------------------------------------------
	// Create the OpenGL object.
	g_contextDriver = NEW glxOpenGLClass;
	IF_NOT_THROW_EXCEPTION (g_contextDriver);

	//----------------------------------------------------------------------------
	LoadAllDrivers();					// LOAD ALL DRIVERS: (DX9, DX11, DX12, OpenGL)

	if (!InitSelectedDriver())
		return false;

	// INIT SCENE-MANAGER:
#if defined USE_SCENE_MANAGER
	ApplicationInitSceneManager();
#endif

	// START-AUDIO: Start Background Music (NOTE: After the INIT "rendering-device")
#if defined USE_SOUND_MANAGER
	IF_NOT_RETURN_FALSE(StartSoundManager());
#endif

	//################################ MAIN STUFF ################################ 
#if DX_ENGINE_LEVEL >= 20 && defined USE_LOADING_THREADS
	IF_NOT_RETURN_FALSE(ApplicationMandatoryLoad());		// START-THREAD LOAD-ALL: "mandatory 2D/3D Stuff", before "start rendering":
#else
	IF_NOT_RETURN_FALSE(SystemClass::LoadAllGraphicAssets());	// Load all main Graphics, that will be rendered
#endif

	if (WOMA::game_state >= GAME_STOP)	// Something FATAL on loading "mandatory 2D/3D Stuff"?
		return false;					// (SAMPLE: misssing 3D/IMAGE/AUDIO file...)

#if !defined USE_LOADING_THREADS
	if (WOMA::game_state == GAME_LOADING)
		WOMA::game_state = GAME_RUN;
#endif

	return result;
}

#if defined USE_PROCESS_OS_KEYS
void LinuxSystemClass::ProcessInput()
{

}
#endif

// Frame() --> ProcessFrame();
void LinuxSystemClass::ProcessFrame() // EQUAL to: WinSystemClass::ProcessFrame()
//-------------------------------------------------------------------------------
{
	SystemClass::FrameUpdate();	// Process: (INPUT + PerformanceStats) Only!

	if (WOMA::game_state == ENGINE_RESTART)
		return;

	#define mon 0

	#if !defined INTRO_DEMO
	if ((WOMA::game_state >= GAME_RUN && WOMA::game_state < ENGINE_RESTART) || (WOMA::game_state == GAME_SETUP))
	#endif
	{
		m_Application->dayLightFade = m_Application->ProcessInputUpdate(); //OS CORE ONLY!  F1, F2, ...

	#if defined INTRO_DEMO
		if (RENDER_PAGE < 15)
	#else
		if (RENDER_PAGE < 10)
	#endif
			return;

		{
		m_Driver->BeginScene(mon);					//RESET FRAME: glClear

		m_Application->RenderScene(mon, m_Driver);	//RENDER ONE FRAME: 100% is done here!

		if (!g_contextDriver)						//PRESENT FRAME
			m_Driver->EndScene(mon);				//[DX]
		else
			g_contextDriver->EndScene(mon);			//[OPENGL/linux]: glXSwapBuffers( Win.display, Win.window);
		}

		m_Driver->RenderfirstTime = false;
	}
}

extern bool createWindow();

//----------------------------------------------------------------------------
// Source: http://www.opengl.org/discussion_boards/archive/index.php/t-177999.html
// D:\WoMAengine2014\woma_developer\SAMPLES\Sample020_StartEngine\Src\original_sample_code.cxx
// C:\WoMAengine2023\LinuxWoma\LinuxWoma\main_linux.cppcreateWindow();
//----------------------------------------------------------------------------
bool LinuxSystemClass::APPLICATION_INIT_MAIN_WINDOW(void* OpenGL)
{
	bool res = createWindow();

	return res;
}

#endif

