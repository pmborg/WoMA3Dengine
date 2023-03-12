// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: minidumperClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code adapted by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
//
// PURPOSE: Intercept a Fatal Exception and get the "file".dmp
//
// --------------------------------------------------------------------------------------------
#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#include "platform.h"

#include <dbghelp.h>	// VC7: ships with updated headers
#include <assert.h>		// For UniCode

// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
									CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
									CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
									CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

///////////////////////////////////////////////////////////////////////////////
// Class name: MiniDumper
///////////////////////////////////////////////////////////////////////////////
class MiniDumper
{
private:
	static LONG WINAPI TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo );

public:
	MiniDumper();
	~MiniDumper();
};

