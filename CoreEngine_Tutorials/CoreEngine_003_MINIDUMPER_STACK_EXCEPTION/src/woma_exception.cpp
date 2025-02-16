// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: woma_exception.cpp
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
// --------------------------------------------------------------------------------------------
// PURPOSE: Allow us to send a Customized Exception with Debug Info including:
//  Filename, Function and Line
// 
//  NOTE: If a constructor throws an Exception ===> the object's destructor do not run. !!!
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#include "OSengine.h"
#include "log.h"
#include "mem_leak.h"
#if defined USE_WOMA_EXCEPTION

#if defined WINDOWS_PLATFORM
#include "stackTrace.h"
stack_trace* sttrace;

woma_exception::woma_exception(const std::string &arg, const char *file, const char *function, int line) : std::runtime_error(arg) 
#else
woma_exception::woma_exception(const std::string &arg, const char *file, const char *function, int line) : exception() 
#endif
{
	#if defined USE_WOMA_EXCEPTION
		std::string msg;

		#if defined WINDOWS_PLATFORM
		//Show extra runtime "Call Stack" frame Debug info on a "woma_exception":
		sttrace = NEW stack_trace(NULL, 0);
		
		if (sttrace)
		{
			CHAR str[MAX_STR_LEN] = { 0 }; wtoa(str, WOMA::APP_FULLNAME, MAX_STR_LEN); // wchar ==> char
			MessageBoxA(NULL, sttrace->to_string().c_str(), str, 0);
		}
		#endif
	#endif
}

woma_exception::~woma_exception() throw() 
{
}

#endif
