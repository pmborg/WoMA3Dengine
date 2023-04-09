// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: 5LaunchEngine.cpp
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567829;

#pragma warning( push )
#pragma warning( disable : 4996 ) // Disable warning C4996: '': This function or variable may be unsafe.
#include "main.h"

#include "OSmain_dir.h"
#include "systemManager.h"

#define Engine_NAME TEXT("SampleDemo")

#ifdef _DEBUG
#define Engine_SOLUTION TEXT("_d")
#else
#define Engine_SOLUTION TEXT("")
#endif

#ifdef WIN_PLATFORM
    #define Engine_EXE TEXT(".exe")
#else
	#define Engine_EXE TEXT("")
#endif

//------------------------------------------------------------------
// PUBLIC FUNCTIONS:
//------------------------------------------------------------------
bool SystemManager::LaunchEngine () 
{
	STARTUPINFO si;			
	PROCESS_INFORMATION pi;	
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	TCHAR LOCAL_EXE[MAX_STR_LEN] = {};
	#pragma warning( push )
	#pragma warning( disable : 4551 ) //warning C4551: function call missing argument list
	StringCchPrintf(LOCAL_EXE, MAX_STR_LEN, TEXT("%s%s%s%s"), Engine_NAME, (WOMA::is64) ? ( (systemFeatures.avx)? TEXT("AvX64"):TEXT("X64") ) : TEXT(""), Engine_SOLUTION, Engine_EXE);
	#pragma warning( pop )

	if( !CreateProcess( 
		LOCAL_EXE,  // No module name (use command line)
		LOCAL_EXE,	// Command line
		NULL,		// Process handle not inheritable
		NULL,		// Thread handle not inheritable
		FALSE,		// Set handle inheritance to FALSE
	#ifdef _DEBUG
		NORMAL_PRIORITY_CLASS,
	#else
		HIGH_PRIORITY_CLASS,
	#endif
		NULL,		// Use parent's environment block
		NULL,		// Use parent's starting directory 
		&si,		// Pointer to STARTUPINFO structure
		&pi )		// Pointer to PROCESS_INFORMATION structure
	) {
		WOMA::WomaMessageBox(TEXT("Cannot find Woma Executable!"), LOCAL_EXE);
		return false;
	}

	WaitForSingleObject(pi.hProcess, IGNORE);	//INFINITE

	// Get the exit code.
	// ----------------------------------------------------------------------------------------
	DWORD exitCode;
	BOOL result = GetExitCodeProcess(pi.hProcess, &exitCode);

	// Close the handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (!result) // Could not get exit code.
		WOMA::WomaMessageBox(TEXT("WARNING: Could not get exit code from WoMA"));

	// Delete TEMP files:
	#ifdef RELEASE
		WOMA::DeleteDirectory(WOMA::womaTempPATH.c_str());
	#endif

	return true;
}

