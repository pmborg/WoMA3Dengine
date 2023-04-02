// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: minidumperClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
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

#include "OSengine.h"

#include "minidumperClass.h"
#include "log.h"
#include "mem_leak.h"
#include "dumpUploader.h"
#include "fileLoader.h"
#include "OSmain_dir.h"

#include <stdlib.h>

#ifdef UNICODE
#define char WCHAR
#define _tcsrchr wcsrchr
#define _tcscpy wcscpy
#define _tcscat wcscat
#define strdup wcsdup
#define LPCSTR LPCWSTR
#define sprintf swprintf
#endif

#include "stackTrace.h"

HMODULE hDll = NULL;

MiniDumper::MiniDumper()
{
	SetUnhandledExceptionFilter( TopLevelFilter );

	CLASSLOADER();
	WomaIntegrityCheck = 1234567891;
}

MiniDumper::~MiniDumper() {CLASSDELETE();}

//NOTE: Before Convert to UNICODE due to Macros!
void WriteError(TCHAR* szDumpPath)
{
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("\nFILE DUMP: %s"), szDumpPath);
}

// New FEATURE: Allow UNICODE: by Pedro Borges
#pragma warning( disable : 4996 ) // Disable warning C4996: '': This function or variable may be unsafe.
#pragma warning( disable : 4005 ) // Disable warning C4005: 'strdup' : macro redefinition

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	TCHAR szDumpPath[_MAX_PATH] = { 0 };
	LONG retval = EXCEPTION_CONTINUE_SEARCH;

	_tprintf(TEXT ("TopLevelFilter()\n")); //NOTE: cant use: WOMA_LOGManager_DebugMSG

	//Show extra runtime "Call Stack" frame Debug info on a Generic un-catched exception!
	stack_trace *sttrace = NEW stack_trace(NULL, 0);
	ASSERT (sttrace);

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)

	LPCTSTR szResult = NULL;

	char szDbgHelpPath[_MAX_PATH] = TEXT("ConsoleTools");

	GetModuleFileName( GetModuleHandle(NULL), szDbgHelpPath, _MAX_PATH );

	TCHAR *pSlash = _tcsrchr( szDbgHelpPath, '\\' ); 
	if (pSlash)
	{
		_tcscpy( pSlash+1, TEXT("DBGHELP.DLL") );
		hDll = ::LoadLibrary( szDbgHelpPath );
	}

	if (!hDll)
		hDll = ::LoadLibrary( TEXT("DBGHELP.DLL") );	// load any version we can

	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pDump)
		{
			char szScratch [_MAX_PATH];

			SYSTEMTIME str_t;
			GetSystemTime(&str_t);

			// NOTE: Dont USE: WOMA::APP_NAME (due special characters)!!!
			StringCchPrintf(szDumpPath, _MAX_PATH, TEXT("%s_%s_%d-%d-%d_%d-%d-%d.%s"), WOMA::PUBLIC_DOCUMENTS, WOMA::APP_PROJECT_NAME, str_t.wYear, str_t.wMonth, str_t.wDay, str_t.wHour, str_t.wMinute, str_t.wSecond, TEXT("dmp"));

            // Write Exception Error to log file ON TEMP!
            WriteError( WOMA::LoadFile(szDumpPath) );

			// ask the user if they want to save a dump file
            if (::MessageBox(NULL, TEXT("A Fatal Exception ocurred in WoMA, would you like to send us this error?"), WOMA::APP_NAME, MB_YESNO) == IDYES)
			{
				// create the file
				HANDLE hFile = ::CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

				if (hFile!=INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

					ExInfo.ThreadId = ::GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;

					// Write the DUMP File
					BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
					if (bOK)
					{
                        sprintf(szScratch, TEXT("Saved file dump '%s' saved and uploaded.\nThanks for helping to make %s a better application!"), szDumpPath, WOMA::APP_NAME);
						szResult = szScratch;
						retval = EXCEPTION_EXECUTE_HANDLER;
					}
					else
					{
						sprintf( szScratch, TEXT("Failed to save dump to '%s' (error %d)"), szDumpPath, GetLastError() );
						szResult = szScratch;
					}
					::CloseHandle(hFile);
				}
				else
				{
					sprintf( szScratch, TEXT("Failed to create dump file '%s' (error %d)"), szDumpPath, GetLastError() );
					szResult = szScratch;
				}
			}
            retval = EXCEPTION_EXECUTE_HANDLER; // FIX: Handle the exception even if user dont want to send feedback, by Pedro Borges
		}
		else
		{
			szResult = TEXT("DBGHELP.DLL too old");
		}
	}
	else
	{
		szResult = TEXT("DBGHELP.DLL not found");
	}

	// If there is any error show it:
	if (szResult) 
	{
		::MessageBox(NULL, szResult, WOMA::APP_FULLNAME, MB_OK);
	}

    // PB NOTE, new FEATURE: Upload DUMP Result
	if (WOMA::dumpUploader(szDumpPath) == 0)
    {
        ::MessageBox(NULL, szResult, WOMA::APP_FULLNAME, MB_OK);
    }
    else {
        ::MessageBox(NULL, TEXT("WARNING: Error Sending the report!"), WOMA::APP_FULLNAME, MB_OK);
    }

	return retval;
}

