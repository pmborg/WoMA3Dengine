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
#if _NOT
C:\WoMAengine2023>generate_ALL_WINDOWS_Tutorials.bat
C:\WoMAengine2023\_HTML>generate_HTML_WINDOWS_CoreEngine_000.bat

// --------------------------------------------------------------------------------------------
// 2002 Tetriz2003		 DirectX 8.1						Visual Studio .NET (2002)
// 2003 GameTetriZ3D	 DirectX 9.0 (DirectX 9.0 SDK)	Visual Studio .NET 2003 / Visual Studio 2005
// 2008 MaverickDemo	 DX9 / DX10 / OPENGL				Visual Studio 2008 / Visual Studio 2010
// 2012 Dx11Engine3D	 DX11
// 2012 WorldOfMiddleAge
// 2013 woma2013
// 2014 woma2014		 DX12
// 2015 woma
// 2016/2017 World of Middle Age  - 3D Multi-Platform ENGINE 2013/2017
// 2023 WoMAengine2023   HTML WEB TUTORIALS
// --------------------------------------------------------------------------------------------

USED/TESTED ON: WDEBUG OR WRELEASE OR WDBGREL

https://www.rastertek.com/tutdx11.html
https://www.braynzarsoft.net/viewtutorial/q16390-braynzar-soft-directx-11-tutorials
http://www.directxtutorial.com/LessonList.aspx?listid=11

C:\WoMAengine2014\EXTRA\_TO LEARN FREE\_BOOKS_\www.d3dcoder.net\*.pdf + source code
    http://richardssoftware.net/home/directx11tutorials
        https://github.com/ericrrichards/dx11/tree/master/DX11

https://github.com/microsoft/DirectX-Graphics-Samples/tree/master/TechniqueDemos/D3D12MemoryManagement
    https://learn.microsoft.com/en-us/samples/microsoft/directx-graphics-samples/d3d12-raytracing-samples-win32/

https://www.youtube.com/watch?v=gQIG77PfLgo&list=PLcacUGyBsOIBlGyQQWzp6D1Xn6ZENx9Y2

DX12:
	https://digitalerr0r.net/2015/08/19/quickstart-directx-12-programming/

BOOKS:	C:\Users\pedro\Dropbox\_3D_BOOKS_to_Read_
		https://github.com/yang-shuohao/Render-Books#%E4%B8%80%E4%B8%AA%E5%91%A8%E6%9C%AB%E7%9A%84%E5%85%89%E7%BA%BF%E8%BF%BD%E8%B8%AAray-tracing-in-one-weekend
			F:\git\Render-Books

DEBUG VERTEX: https://www.youtube.com/watch?v=3tb-w_X6-4Y
#endif

#include "OSengine.h"
int Command = 0;

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
#if defined WINDOWS_PLATFORM
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) WomaFatalException("CoInitializeEx Failed!");
#endif

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
    } while (Command == ENGINE_RESTART);	// New settings from user?, restart the Engine with new settings...

    OS_ENGINE_STOP();                       // ENGINE STOP: RELEASE Mini Dump, CLOSE Log File and DELETE Temp Dir. (cache) files
    return Command;                         // ENGINE RETURN: to OS
}

// -------------------------------------------------------------------------------------------------------------------------------------
int START_3D_ENGINE()
// -------------------------------------------------------------------------------------------------------------------------------------
{
#if defined WINDOWS_PLATFORM
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#endif
    SYSTEM Sys(&WOMA::settings);        // NEW DX/WinSystemClass() 4OS + NEW ApplicationClass() + MyRegisterClass

    if (!Sys.InitializeSystem())        // START Woma Engine + RUN SystemCheck + INIT GUI + START THREAD/Load all WoMA Engine "Objects"

    return Sys.ApplicationMainLoop();   // RUN:  WIN32 MAIN LOOP: PROCESS FRAME (UPDATE & RENDER) FRAMES!
}

#if _NOT
#INDEX:
CoreEngine_000(OSengine)
CoreEngine_001(LANG + LOG + MEM_Leak + OSmain_dir)
CoreEngine_002(MAIN_Window + processINPUT)
CoreEngine_003(EXCEPTION_trace + DUMP_upload)
CoreEngine_004(SystemManager + TIMER + FPS)
CoreEngine_005(PAINT + settings.xml)
CoreEngine_006(PACKmanager + IDEA + MATH + CPU BENCH)    engine.pck(encoded)
CoreEngine_007(PAINT RealTimeCelestial) + Loading a files from engine.pck  #if ENGINE_LEVEL >= 11 #define USE_ASTRO_CLASS
CoreEngine_008(PAINT RealTimeMapGPSPosition) + Loading a file "engine/index.txt" from "PCK"  #define USE_ALLOW_MAINWINDOW_RESIZE         #define USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE
CoreEngine_009(PAINT RealTimeWeather) + Loading a file "engine/index.txt" from "PCK"  #if ENGINE_LEVEL >= 13  #define USE_METARCLASS
CoreEngine_010(DX) + allowResize + USE_ALLOW_MAINWINDOW_RESIZE + USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE + USE_ASPECT_RATIO

C:\WoMAengine2023\DXEngine_020\Output\mysetup.exe
C:\Program Files (x86)\WoMAengine2023
% TEMP %
C:\Users\Public\Documents\Pmborg\<CORE_ENGINE_LEVEL>\<DX_ENGINE_LEVEL>
C:\Users\Public\Documents\Pmborg\10\21

OUTPUT: $(SolutionDir)$(Platform)\$(Configuration)\
OUTPUT: C:\Program Files (x86)\WoMAengine2023

#DEBUG:
 DEBUG    64 DX11 GL3  DX9*  DX12(WINSDK)    DX_ENGINE_LEVEL >= 19 SETUP  OK
 DEBUG    32 DX11 GL3  DX9*  DX12(WINSDK)    DX_ENGINE_LEVEL >= 19 SETUP  OK
 WDEBUG   64 DX11 GL3 -DX9   ----            (SDK_June_2010)DX_ENGINE_LEVEL >= 19 SETUP  OK
 WDEBUG   32 DX11 GL3 -DX9   ----            (SDK_June_2010)DX_ENGINE_LEVEL >= 19 SETUP  OK

#RELEASE:
 RELEASE  64 DX11 GL3 DX9*   DX12(WINSDK)    DX_ENGINE_LEVEL >= 20 INSTALLER + RESIZE
 RELEASE  32 DX11 GL3 DX9*   DX12(WINSDK)    DX_ENGINE_LEVEL >= 20 INSTALLER + RESIZE
 WRELEASE 64 DX11 GL3 DX9    ----            (SDK_June_2010)DX_ENGINE_LEVEL >= 20 INSTALLER + RESIZE
 WRELEASE 32 DX11 GL3 DX9    ----            (SDK_June_2010)DX_ENGINE_LEVEL >= 20 INSTALLER + RESIZE
#endif