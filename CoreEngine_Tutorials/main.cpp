// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: main.cpp
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
// PURPOSE: Entry point of all WoMA ENGINE Applications OS: WINDOWS
// --------------------------------------------------------------------------------------------

#include "OSengine.h"
int Command = 0; //old Restart

// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:CONSOLE
// -------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])    // FOR: WOMA_CONSOLE_APPLICATION (LINUX or ANDROID or WINDOWS Console)
{
    OS_ENGINE_START(argc, argv);    // ENGINE SETUP:(DEBUG) Memory leaks check + Mini Dump + Log + Process Command Line Arg(s)
    return ENGINE_RUN();            // ENGINE RETURN: to OS
}

// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:WINDOWS
// -------------------------------------------------------------------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)    //FOR: WOMA_WIN32_APPLICATION
{
    OS_ENGINE_START(lpCmdLine, nShowCmd);   // START Memory leaks check + START: Mini-dumpper + START: Log + Process: Command Line Arg(s)
    return ENGINE_RUN();                    // START Application
}

// -------------------------------------------------------------------------------------------------------------------------------------
void OS_ENGINE_START(int argc, char* argv[])        //FOR: WOMA_CONSOLE_APPLICATION
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) WomaFatalException("CoInitializeEx Failed!");

    START_OS_SYSTEM_KERNEL(argc, argv);
}


void OS_ENGINE_START(LPSTR pScmdline, int iCmdshow)	//FOR: WOMA_WIN32_APPLICATION
{
    WOMA::Scmdline = pScmdline;
    WOMA::Cmdshow = iCmdshow;

    char* argv[] = { NULL };
    OS_ENGINE_START(0, argv);
}

// -------------------------------------------------------------------------------------------------------------------------------------
int ENGINE_RUN()
// -------------------------------------------------------------------------------------------------------------------------------------
{
    do {
        Command = START_3D_ENGINE();	    // Run until the "GAME_EXIT" or do a "ENGINE_RESTART"
        if (Command == ENGINE_RESTART)
            Sleep(1000);
    } while (Command == ENGINE_RESTART);	// New Setings from user?, restart the Engine with new seetings...

    OS_ENGINE_STOP();                       // ENGINE STOP: RELEASE Mini Dump, CLOSE Log File & DELETE Temp Dir. (cache) files
    return Command;                         // ENGINE RETURN: to OS
}

// -------------------------------------------------------------------------------------------------------------------------------------
int START_3D_ENGINE()
// -------------------------------------------------------------------------------------------------------------------------------------
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    SYSTEM Sys(&WOMA::settings);        // NEW DX/WinSystemClass() 4OS + NEW ApplicationClass() + MyRegisterClass

    if (!Sys.InitializeSystem())        // START Woma Engine + RUN SystemCheck + INIT GUI + 
        return EXIT_FAILURE;            // START THREAD/Load all WoMA Engine "Objects"

    return Sys.ApplicationMainLoop();   // RUN:  WIN32 MAIN LOOP: PROCESS FRAME (UPDATE & RENDER) FRAMES!
}

