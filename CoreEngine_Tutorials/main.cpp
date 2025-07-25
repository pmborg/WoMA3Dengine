// --------------------------------------------------------------------------------------------
// Filename: main.cpp
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
//
// --------------------------------------------------------------------------------------------
// PURPOSE: Entry point of all WoMA ENGINE Applications OS: WINDOWS / LINUX / ANDROID

// KEY FEATURES:
//
// • PLATFORM ABSTRACTION :
//      The file uses preprocessor directives to select the appropriate entry point(WinMain for Windows GUI, main for console / Linux, and android_main for Android), ensuring compatibility across different operating systems.
// 
// • CENTRALIZED APPLICATION LOGIC :
//      All platform - specific entry points delegate to a single function, APPLICATION_MAIN, which encapsulates the core engine startup, main loop, and shutdown logic.
// 
// • COMMAND - LINE ARGUMENT HANDLING :
//      Command - line arguments are parsed and passed to the engine, allowing for flexible configuration and runtime overrides.
// 
// • ENGINE LIFECYCLE MANAGEMENT :
//      The engine is initialized via APPLICATION_STARTUP, runs the main loop(with support for engine restarts), and is properly shut down with APPLICATION_STOP, ensuring resources are managed and released correctly.
// 
// • ERROR HANDLING :
//      The design anticipates exceptions during startup, allowing for centralized error reporting and graceful application termination.
// 
// • EXTENSIBILITY :
//      The structure supports integration with external libraries(such as Assimp for model loading) and can be easily extended for new platforms or features.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525256;

#if NOTES
#include "notes.h"
#endif
#include "OSengine.h"
#pragma warning(disable : 6387)				   

#if defined WINDOWS_PLATFORM
// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:WINDOWS                                    //FOR: WOMA_WIN32_APPLICATION (WINDOWS)
// -------------------------------------------------------------------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    int argc = 0;
    char* argv[MAX_PARAMS] = { };
    
    WOMA::Scmdline = lpCmdLine;
    WOMA::Cmdshow = nShowCmd;                        
    COMMANDLINE_TO_ARGC_ARGV();                 // POPULATE: argc & argv
    
    int res = APPLICATION_MAIN(argc, argv); // ENTRY POINT!

    for (int i = 0; i < MAX_PARAMS; i++)
        SAFE_DELETE(argv[i]);

    return res;
}
#endif

#if !defined WOMA_WIN32_APPLICATION
// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:CONSOLE                            // FOR: WOMA_CONSOLE_APPLICATION (LINUX, or WINDOWS-Console)
// -------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    return APPLICATION_MAIN(argc, argv);        // ENTRY-POINT: WINDOWS DEBUG or LINUX!
}
#endif

#if defined ANDROID_PLATFORM && !defined NewWomaEngine
android_app* app;
struct womaengine engine = { 0 };

#if defined WOMAENGINE
void android_main(android_app* state)           // ENTRY-POINT: ANDROID
{
    app = state;

    _tprintf("[%d]: android_main()\n", gettid());
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;
    init_engine(app, &engine);

    //MANDATORY: Wait for JAVA window creation:
    while (!engine.has_focus_)                  //Means: window created.
        process_events(&engine, app);

    int argc = 0;
    char* argv[1] = { nullptr };
    APPLICATION_MAIN(argc, argv);

    engine.has_focus_ = false;
}
#endif

#endif

int Command = EXIT_SUCCESS;

// Entry point of all WoMA ENGINE Applications all "main's" call this this one (used by: WINDOWS / LINUX / ANDROID)
// -------------------------------------------------------------------------------------------------------------------------------------
int APPLICATION_MAIN(int argc, char* argv[])
// -------------------------------------------------------------------------------------------------------------------------------------
{
    APPLICATION_STARTUP(argc, argv);                      // ENGINE SETUP: |CoInitializeEx|+|OSmain_dirs|+|Memory leaks check|+|Log|+|Mini Dumper|
    if (Command == EXIT_SUCCESS) {                        // If APPLICATION_STARTUP throw an exception, Command will be: EXIT_FAILURE
      do {
          {
              SYSTEM demo(&WOMA::settings);               // NEW |SystemClass()::WinSystemClass()::DxWinSystemClass() for Specific OS|+|WOMA::APP_NAME|+|NEW ApplicationClass()"|
              demo.ParseCommandLineArgs(argc, argv);      // LoadXmlSettings + Parse the command line parameters: -warp /warp, ... (that will override: settings.xml)
      
              if (demo.APPLICATION_INIT_SYSTEM())         // INIT Woma Engine: |SOUND|+|Register|+|XML|+|Sys.Chk|+|Window|+|OS-Input|+|Timer|+|Drivers|+|Load Assets|
                  Command = demo.APPLICATION_MAIN_LOOP(); // RUN: OS MAIN LOOP -> PROCESS FRAMES: (UPDATE + RENDER)!
          }                                               // DELETE SYSTEM demo: Close WINDOW
          if (Command == ENGINE_RESTART)                  // The User set new settings?
              Sleep(2000);                                // Need to be 2secs to change resources between drivers
      } while (Command == ENGINE_RESTART);                // Try to restart the Engine with new settings then! (if fail! goto VectoredExceptionHandler())
    }                                                     
    APPLICATION_STOP();                                   // ENGINE STOP: |CoUninitialize|+|Free Mini Dumper|+|CLOSE Log|+|DELETE Temp files(RELEASE)
    return Command;                                       // ENGINE RETURN: to OS (Can be: 0, ENGINE_RESTART or "an error" code)
}
 