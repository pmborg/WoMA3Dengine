// ----------------------------------------------------------------------------------------------
// Filename: minidumperClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2026
//-------------------------------------------------------------------------------------------
// code adapted by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// PURPOSE: Intercept a Fatal Exception and get the "file".dmp
//
//  Original Source Code: 
//  http://www.codeproject.com/Articles/1934/Post-Mortem-Debugging-Your-Application-with-Minidu
//
/*
Original Code:
Copyright (C) 2002  Andy Pennell
   22.3.2007 - minor modifications by Jaroslav Libak
   20.8.2013 - minor modifications by Pedro Borges

   You can use code snippets and source code downloads in your applications as long as

   * You keep all copyright notices in the code intact.
   * You do not sell or republish the code or it's associated article without the author's written agreement
   * You agree the code is provided as-is without any implied or express warranty. 
*/
//
//  Added UNICODE support by Pedro Borges
//  Added Upload DUMP file to FTP site by Pedro Borges
//
//  IMPORTANT NOTE: Compile with "Security Check": NO
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "OSengine.h"
#if defined USE_WOMA_EXCEPTION //USE_MINIDUMPER
#include "mem_leak.h"
#include "minidumperClass.h"
#include <stdlib.h>

#ifdef UNICODE
#define _tcsrchr wcsrchr
#define _tcscpy wcscpy
#define _tcscat wcscat
#define strdup wcsdup
#define LPCSTR LPCWSTR
#define sprintf swprintf
#endif

#include "stackTrace.h"

HMODULE hDll = NULL;
#pragma warning(disable: 4102)

LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	std::ofstream f;
	f.open("VectoredExceptionHandler.txt", std::ios::out | std::ios::trunc);
	f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	f.close();

	if (3221225477 == pExceptionInfo->ExceptionRecord->ExceptionCode)
		WomaMessageBox(TEXT("WOMA Need to restart!"), WOMA::strConsoleTitle, MB_OK);

	exit(ENGINE_RESTART);

	//return EXCEPTION_CONTINUE_SEARCH;
}



//NOTE: Before Convert to UNICODE due to Macros!
void WriteError(TCHAR* szDumpPath)
{
	womalogauto((TCHAR*)TEXT("\nFILE DUMP: %s"), szDumpPath);
}

// New FEATURE: Allow UNICODE: by Pedro Borges
#pragma warning( disable : 4996 ) // Disable warning C4996: '': This function or variable may be unsafe.
#pragma warning( disable : 4005 ) // Disable warning C4005: 'strdup' : macro redefinition

LONG TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	TCHAR szDumpPath[_MAX_PATH] = { 0 };
	LONG retval = EXCEPTION_CONTINUE_SEARCH;

	_tprintf(TEXT ("TopLevelFilter()\n")); //NOTE: cant use: womalog

	//Show extra runtime "Call Stack" frame Debug info on a Generic uncatched exception!
	stack_trace *sttrace = NEW stack_trace(NULL, 0);
	ASSERT (sttrace);

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)

	LPCTSTR szResult = NULL;

	TCHAR szDbgHelpPath[_MAX_PATH] = TEXT("ConsoleTools");

	GetModuleFileName( GetModuleHandle(NULL), szDbgHelpPath, _MAX_PATH );

	TCHAR *pSlash = _tcsrchr( szDbgHelpPath, '\\' ); 
	if (pSlash)
	{
		_tcscpy( pSlash+1, TEXT("DBGHELP.DLL") );
		hDll = ::LoadLibrary( szDbgHelpPath );
	}

	if (!hDll)
		hDll = ::LoadLibrary( TEXT("DBGHELP.DLL") );	// load any version we can
	// If there is any error show it:
	if (szResult) 
	{
		::MessageBox(NULL, szResult, WOMA::APP_FULLNAME, MB_OK);
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

#endif

