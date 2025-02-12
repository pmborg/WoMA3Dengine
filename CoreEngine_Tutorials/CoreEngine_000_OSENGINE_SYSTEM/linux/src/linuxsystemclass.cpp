// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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

// Eclipse Installation of C++ development for cygwin
// http://www.badprog.com/c-eclipse-installation-of-c-c-development-tools-cdt-and-cygwin-for-windows

// MORE INFO: https://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_%28GLX%29

#include "OSengine.h" //#include "linuxsystemclass.h"
#if defined LINUX_PLATFORM
#include <gtk/gtkmain.h>

#define ENGINE_LEVEL DX_ENGINE_LEVEL

#if defined USE_IDEA_PACK
#include "packManager.h"
#endif
#if defined USE_TINYXML_LOADER
#include "xml_loader.h"
#endif

	#define g_contextDriver NULL

extern MyWin Win;

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

#if defined CLIENT_SCENE_SETUP
	womaSetup = NULL;
#endif

	// --------------------------------------------------------------
	// Init:
	// --------------------------------------------------------------

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
	audio = NULL;
#endif
}

LinuxSystemClass::~LinuxSystemClass()
{
	SystemHandle = NULL;
	CLASSDELETE();
#if LEVEL >= 2
	XClearWindow(Win.display, Win.window);
#endif
	gtk_main_quit(); // /usr/include/gtk-3.0/gtk/gtkmain.h
}

//Stop() --> Shutdown()
void LinuxSystemClass::Shutdown()
{
	// LinuxSystemClass Shutdown:
	WOMA_LOGManager_DebugMSG ("LinuxSystemClass::Stop()\n");

#if defined USE_SCENE_MANAGER
	//SceneManager* sceneManager = SceneManager::GetInstance();
	SAFE_SHUTDOWN (sceneManager);
#endif

	// Destroy Drivers:
	SystemClass::Shutdown();

#if defined _DEBUG && defined WOMA_CONSOLE_APPLICATION
	//printf ("\npress ENTER to close, console window..."); getchar();
#endif	
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
	WomaMessageBox(WOMA::strConsoleTitle, TEXT("WOMA Hello World!"), MB_OK);
	return false;

	return true;
}

// APP SYSTEM: START POINT
//----------------------------------------------------------------------------
bool LinuxSystemClass::APPLICATION_INIT_SYSTEM() // ApplicationInit()
{
	bool result = true;
	AppSettings->DRIVER = DRIVER_GL3; //Force OpenGL Driver

	WOMA_LOGManager_DebugMSG("LinuxSystemClass::APPLICATION_INIT_SYSTEM()\n");

	IF_NOT_RETURN_FALSE(APPLICATION_CORE_SYSTEM());

#if defined USE_TINYXML_LOADER // Must be before: ApplicationInitMainWindow()
	IF_NOT_RETURN_FALSE(SystemClass::LoadXmlSettings());		// XML: Load Application Settings: "settings.xml", pickup "Driver" to Use.
#endif

#if defined USE_PROCESS_OS_KEYS
	IF_NOT_RETURN_FALSE(InitOsInput());			// INIT-INPUT Devices, NOTE: After "Create MainWindow(s)"
#endif

	// Step 2: Create Context Driver
	//----------------------------------------------------------------------------

	return result;
}

#if defined USE_PROCESS_OS_KEYS
void LinuxSystemClass::ProcessInput()
{

}
#endif


// Frame() --> ProcessFrame();
void LinuxSystemClass::ProcessFrame() // EQUAL: BOTH OS?
//----------------------------------------------------------------------------
{

}

extern bool createWindow();

#endif

