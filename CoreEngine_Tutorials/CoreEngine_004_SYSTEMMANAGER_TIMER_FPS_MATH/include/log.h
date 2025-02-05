// --------------------------------------------------------------------------------------------
// Filename: log.h
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
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;
#pragma once

#include "platform.h"
#if defined USE_LOG_MANAGER

#if NOTES
// INTERFACE LIKE: http://stackoverflow.com/questions/17554515/hiding-private-members-of-c-library
#endif
namespace WOMA
{
////////////////////////////////////////////////////////////////////////////////
// Class name: LogManager
////////////////////////////////////////////////////////////////////////////////
class ILogManager
{
public:
	UINT WomaIntegrityCheck = 1234567311;
	static ILogManager* CreateInstance();
	static void ShutdownInstance();

	virtual TCHAR* getLogFileName() = 0;

#if defined WINDOWS_PLATFORM
	virtual void DEBUG_MSG(WCHAR* strMsg, ...) = 0;
#endif

	virtual void DEBUG_MSG(CHAR* strMsg, ...) = 0;
};

extern ILogManager* logManager;		// Global log manager
extern void start_log_manager();
extern std::string android_temp_folder(struct android_app* app);
}
#endif
