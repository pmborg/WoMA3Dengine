// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: SystemManager_5LaunchEngine.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------

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

	return true;
}

