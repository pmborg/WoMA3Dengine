// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: main.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2024
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
//WomaIntegrityCheck = 1234567311;
// 
// DEFINE ENGINE LEVEL:
// C:\[WoMAengine]\woma_engine_assets.h

// NOTE !!!!!!!!!! ON LINUX dont forget to OPEN a WSL console before compile !!!!!!!!!!
// C:\ProgramData\Microsoft\Windows\Start Menu\Programs\WSL

// --------------------------------------------------------------------------------------------
// #HOWTO:
// C:\[WoMAengine]\Android-howto\HowTo\HowToInstallVisualStudio2019CrossPlatformDevelopmentEnvironment\howto_download_and_install_VisualStudio2019CrossPlatformSetupEnvironment.txt
//
// #TOOLS / ANDROID:
// C:\ProgramData\Microsoft\AndroidSDK\25
// C:\Program Files (x86)\Android\AndroidNDK\android-ndk-r15c
// C:\Java\jdk1.8.0_202
// C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Apps\apache-ant-1.9.3
// 
// INCLUDE... C:\Program Files (x86)\Android\AndroidNDK64\android-ndk-r15c\platforms\android-26\arch-arm64\usr\include
// 
// 
// WINDOWS:
// **   C:\[WoMAengine]\DXEngine_055\GLengine
//      C:\[WoMAengine]\DXEngine_055\REPORT.txt
// 
// LINUX:
// C:\[WoMAengine]\LinuxWoma\LinuxWoma\bin\x64\Debug
//   ~/projects/LinuxWoma/bin/x64/Debug/GLengine
//----> \\wsl.localhost\Ubuntu\home\pedro\projects\LinuxWoma\bin\x64\Debug\GLengine
//      \\wsl.localhost\Ubuntu\home\pedro\projects\LinuxWoma\bin\x64\Debug\REPORT.txt
// 
// ANDROID:
// **C:\[WoMAengine]\Android-WomaEngine\Android2\Android2.Packaging\ARM64\Debug
//   ~/projects/LinuxWoma/Android-WomaEngine/Android2/Android2.NativeActivity/GLengine
//----> \\wsl.localhost\Ubuntu\home\pedro\projects\LinuxWoma\Android-WomaEngine\Android2\Android2.NativeActivity\GLengine
// 
// COMMON:
// C:\[WoMAengine]\DXEngine_055\engine\data\basics
// \\wsl.localhost\Ubuntu\home\pedro\projects\LinuxWoma\bin\x64\Debug\engine\data\basics
//----> C:\[WoMAengine]\Android-WomaEngine\Android2\Android2.Packaging\ARM64\Debug\Package\assets\engine\data\basics

// --------------------------------------------------------------------------------------------
// SETUP ENV: C:\[WoMAengine]\Android-howto\howto.txt
// --------------------------------------------------------------------------------------------
// PURPOSE: Entry point of all WoMA ENGINE Applications OS: WINDOWS
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "OSengine.h"


int Command = 0;

#if defined WINDOWS_PLATFORM && !defined ANDROID_PLATFORM
// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:WINDOWS                                    //FOR: WOMA_WIN32_APPLICATION (WINDOWS)
// -------------------------------------------------------------------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	int argc = 0;
	char* argv[MAX_PARAMS] = { NULL };
	COMMANDLINE_TO_ARGC_ARGV();             // POPULATE: argc & argv

	Command = APPLICATION_MAIN(argc, argv); // ENTRY POINT!

	for (int i = 0; i < MAX_PARAMS; i++)
		SAFE_DELETE(WOMA::ARGv[i]);

	return Command;
}
#endif

#if !defined WOMA_WIN32_APPLICATION && !defined ANDROID_PLATFORM
// -------------------------------------------------------------------------------------------------------------------------------------
// SUBSYSTEM:CONSOLE                                    // FOR: WOMA_CONSOLE_APPLICATION (LINUX, or WINDOWS-Console)
// -------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	return APPLICATION_MAIN(argc, argv);                // ENTRY POINT!
}
#endif

void ParseCommandLineArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (_tcsnicmp(argv[i], "-warp", _tcslen(argv[i])) == 0 ||
			_tcsnicmp(argv[i], "/warp", _tcslen(argv[i])) == 0)
		{
			WOMA::UseWarpDevice = true;
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
int APPLICATION_MAIN(int argc, char* argv[])
// -------------------------------------------------------------------------------------------------------------------------------------
{
	APPLICATION_STARTUP(argc, argv, Command);           // ENGINE SETUP: |CoInitializeEx|+|OSmain_dirs|+|Memory leaks check|+|Log|+|Mini Dumper|
	ParseCommandLineArgs(argc, argv);                   // [*] Parse the command line parameters: -warp /warp

	do {
		{
			SYSTEM demo(&WOMA::settings);               // NEW |SystemClass()::WinSystemClass()::DxWinSystemClass() for Specific OS|+|WOMA::APP_NAME|+|NEW ApplicationClass()"|

			if (demo.APPLICATION_INIT_SYSTEM())         // INIT Woma Engine: |SOUND|+|Register|+|XML|+|Sys.Chk|+|Window|+|OS-Input|+|Timer|+|Drivers|+|Load Assets|
				Command = demo.APPLICATION_MAIN_LOOP(); // RUN: OS MAIN LOOP -> PROCESS FRAMES: (UPDATE + RENDER)!

			if (Command == ENGINE_RESTART)              // The User set new settings?
				Sleep(2000);                            // Need to be 2secs to change resources between drivers
		}                                               // DELETE SYSTEM demo: Close WINDOW
	} while (Command == ENGINE_RESTART);	            // Try to restart the Engine with new settings then! (if fail! goto VectoredExceptionHandler())

	APPLICATION_STOP();                                 // ENGINE STOP: |CoUninitialize|+|Free Mini Dumper|+|CLOSE Log|+|DELETE Temp files(RELEASE)

	return Command;                                     // ENGINE RETURN: to OS (Can be: 0, ENGINE_RESTART or "an error" code)
}
