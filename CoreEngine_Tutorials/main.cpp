// --------------------------------------------------------------------------------------------
// Filename: main.cpp
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
//
// The WoMA tutorials are generated automatically from a single master codebase using compile-time feature levels (CORE_ENGINE_LEVEL and DX_ENGINE_LEVEL). 
// They are not maintained as independent projects. 
// Each tutorial is therefore a consistent snapshot of the engine at a particular stage of its evolution. 
// Bug fixes and architectural improvements made to the shared code automatically propagate to every affected tutorial, 
// ensuring consistency while eliminating code duplication.
// 
// --------------------------------------------------------------------------------------------
// PURPOSE: 
// 
//		Unified entry point for every WoMA application.
//		All platform-specific entry points (WinMain, main and android_main)
//		delegate to ENGINE_MAIN, making the engine startup,
//		execution and shutdown identical on every supported platform.

/*
ARCHITECTURE:

	The engine is responsible for HOW the application runs.
	The application is responsible for WHAT is updated and rendered.

	The engine creates and owns the runtime environment.
	The application only initializes and uses it.

	The WoMA Engine provides all platform-specific services
	(windowing, rendering, input, timing, resources, etc.),
	while the Application layer defines what is updated and
	rendered each frame.
	
	The same Application source code runs unchanged on
	Windows, Linux and Android.

ENGINE DIAGRAM:

	Operating System
	│
	▼
	WinMain / main / android_main
	│
	▼
	ENGINE_MAIN()
	│
	▼
	PROCESS_BOOTSTRAP()
		Process Bootstrap:
		• Logging
		• COM / GTK
		• Directories
		• Exception Handler
		• Memory Diagnostics
		• MiniDumper
	│
	▼
	┌───────────────────────────────────────┐
	│           SYSTEM(Engine)              │
	│                                       │
	│ Parse settings						│
	│		├── Defaults					│
	│		├── settings.xml				│
	│		└── Command Line                │       
	│                                       │
	│ APPLICATION_INIT_SYSTEM()             │
	│    ├── Window                         │
	│    ├── Input                          │
	│    ├── Graphics Driver                │
	│    ├── Scene Manager                  │
	│    ├── LoadAllGraphicAssets()         │
	│    │      │                           │
	│    │      └── Application::Initialize │
	│    ├── Timer                          │
	│    └── Audio                          │
	│                                       │
	│ APPLICATION_MAIN_LOOP()               │
	│    ├── Update()                       │
	│    └── RenderScene()                  │
	└───────────────────────────────────────┘
	▲
	│
	└── ENGINE_RESTART
	│
	▼
	Destroy SYSTEM (Window, Driver, Input, Scene Manager...)
	│
	▼
	PROCESS_SHUTDOWN()
	│
	▼
	Return to OS


 KEY FEATURES:

 • PLATFORM ABSTRACTION :
      The file uses pre-processor directives to select the appropriate entry point(WinMain for Windows GUI, main for console / Linux, and android_main for Android), ensuring compatibility across different operating systems.
 
 • CENTRALIZED APPLICATION LOGIC :
      All platform-specific entry points delegate to a single function, ENGINE_MAIN, which encapsulates the core engine startup, main loop, and shutdown logic.
 
 • COMMAND - LINE ARGUMENT HANDLING :
      Command-line arguments are parsed and passed to the engine, allowing for flexible configuration and runtime overrides.
 
 • ENGINE LIFECYCLE MANAGEMENT :
      The engine is initialized via PROCESS_BOOTSTRAP, runs the main loop(with support for engine restarts), and is properly shut down with PROCESS_SHUTDOWN, ensuring resources are managed and released correctly.
 
 • ERROR HANDLING :
      The design anticipates exceptions during startup, allowing for centralized error reporting and graceful application termination.
 
 • EXTENSIBILITY :
      The structure supports integration with external libraries(such as ASSIMP for model loading) and can be easily extended for new platforms or features.
*/
// Generation integrity marker:
// WomaIntegrityCheck = 1234526920;


#if NOTES
#include "notes.h"
#endif
#pragma warning(disable : 6387)
#pragma warning(disable : 6262)
#include "OSengine.h"

// -------------------------------------------------------------------------------------------------
// Platform entry points:
//
//		Each supported operating system has its own native application entry point
//		(WinMain, main or android_main). Their only responsibility is to perform the
//		minimum platform-specific initialization required by the OS and then delegate
//		execution to the common ENGINE_MAIN(), which represents the true
//		architectural entry point of the WoMA Engine.
// -------------------------------------------------------------------------------------------------

#if defined WINDOWS_PLATFORM
// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:WINDOWS                        //FOR: WOMA_WIN32_APPLICATION (WINDOWS for RELEASE)
// -------------------------------------------------------------------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
	
    WOMA::Scmdline = lpCmdLine;
    WOMA::Cmdshow = nShowCmd;

    return ENGINE_MAIN(__argc, __argv); // ENTRY-POINT: WINDOWS (RELEASE)!
}
#endif

#if !defined WOMA_WIN32_APPLICATION
// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:CONSOLE                         // FOR: WOMA_CONSOLE_APPLICATION (LINUX, or WINDOWS-Console for DEBUG)
// -------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    return ENGINE_MAIN(argc, argv);    // ENTRY-POINT: WINDOWS CONSOLE(DEBUG) or LINUX!
}
#endif

#if defined ANDROID_PLATFORM && !defined NewWomaEngine
android_app* app;
struct womaengine engine = { 0 };

#if defined WOMAENGINE
void android_main(android_app* state)       // ENTRY-POINT: ANDROID(DEBUG/RELEASE)!
{
    app = state;

    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;
    init_engine(app, &engine);

    //MANDATORY: Wait for JAVA window creation:
    while (!engine.has_focus_)             //Means: window created.
        process_events(&engine, app);

#if defined ANDROID_PLATFORM && DX_ENGINE_LEVEL >= 30 && !defined NewWomaEngine
	//AndroidSetLandscape(&engine);
#endif

    int argc = 0;
    char* argv[1] = { nullptr };
    ENGINE_MAIN(argc, argv);           // ENTRY-POINT: ANDROID(DEBUG/RELEASE)!

    engine.has_focus_ = false;
}
#endif
#endif

// Common architectural entry point (used by: WINDOWS / LINUX / ANDROID).
// All platform-specific entry points (WinMain, main and android_main)
// delegate execution to this function.
// -------------------------------------------------------------------------------------------------------------------------------------
int ENGINE_MAIN(int argc, char* argv[])
// -------------------------------------------------------------------------------------------------------------------------------------
{
    PROCESS_BOOTSTRAP(argc, argv);                        // PROCESS BOOTSTRAP: CoInitializeEx, OSmain_dirs, Memory leaks check, Log, Mini Dumper, etc...

    if (Command == EXIT_SUCCESS) {                        // If PROCESS_BOOTSTRAP throw an exception, Command will be: EXIT_FAILURE
        do {
            {
                SYSTEM engine(&WOMA::settings);                     // For Specific OS: (WinSystemClass, dxWinSystemClass, LinuxSystemClass or AndroidSystemClass), WOMA::APP_NAME, NEW ApplicationClass()"|
				engine.ParseCommandLineAndApplySettings(argc, argv);// LoadXmlSettings + Parse the command line parameters: -warp /warp, ... (that will override: settings.xml)

                if (engine.APPLICATION_INIT_SYSTEM())				// INIT Woma Engine: SOUND, Register, XML, Sys.Chk, Window, OS-Input, Timer, Drivers, Load Assets|
                    Command = engine.APPLICATION_MAIN_LOOP();		// RUN: OS MAIN LOOP -> PROCESS FRAMES: (UPDATE + RENDER)!
            }														// SYSTEM instance destroyed here. Window and OS resources are released.
            if (Command == ENGINE_RESTART)							// Engine Restart: requested after a runtime configuration change.
                Sleep(2000);										// Need to be 2secs to change resources between drivers
        } while (Command == ENGINE_RESTART);						// Try to restart the Engine with new settings then! (if fail! goto VectoredExceptionHandler())
    }

    PROCESS_SHUTDOWN();                                   // PROCESS SHUTDOWN: CoUninitialize, Free Mini Dumper, CLOSE Log, DELETE Temp files(RELEASE)
    return Command;                                       // ENGINE RETURN: to OS (Can be: 0 or "an error" code)
}