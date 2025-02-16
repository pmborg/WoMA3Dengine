// ----------------------------------------------------------------------------------------------
// Filename: minidumperClass.h
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
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// PURPOSE: Intercept a Fatal Exception and get the "file".dmp
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;
#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////

#include "platform.h"
#if defined USE_MINIDUMPER && defined WINDOWS_PLATFORM

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
	UINT WomaIntegrityCheck = 1234567142;
	MiniDumper();
	~MiniDumper();
};

#endif
