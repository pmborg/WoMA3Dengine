// --------------------------------------------------------------------------------------------
// Filename: systemManager.h
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#pragma once
#pragma warning( disable : 5208 ) // warning C5208: unnamed class used in typedef name cannot declare members other than non-static data members, member enumerations, or member classes

#ifndef systemManager_h_
#define systemManager_h_

#include "platform.h"// TCHAR

#if defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#include <sys/utsname.h>
#endif

#include "timerClass.h"
#if CORE_ENGINE_LEVEL >= 4
#include "ProcessorInfo.h"
#include "SystemFeatures.h"

#if defined WINDOWS_PLATFORM
#include "wmiUtilClass.h"

struct DriveList {
	WCHAR   drive = 0;
	__int64 freeBytesAvailable = 0;
};
#endif

#define BUFSIZE 256

#if CORE_ENGINE_LEVEL >= 4 && defined WINDOWS_PLATFORM
extern int GETOS(void);
#endif



////////////////////////////////////////////////////////////////////////////////
// Class name: SystemManager
////////////////////////////////////////////////////////////////////////////////
class SystemManager
{
public:
	UINT WomaIntegrityCheck = 1234567311;
	SystemManager();
	~SystemManager();

#if defined USE_SYSTEM_CHECK
	bool CheckOS();				// Detect Version & System Check
	bool checkCPU();
	bool checkRAM();
	bool checkDiskFreeSpace();
#endif

	float CPUSpeedMHz;
	ProcessorInfo processorInfo;

#if defined WINDOWS_PLATFORM
	wmiUtilClass wmiUtil;
	bool checkCPUFeatures();
#endif

#if defined USE_TIMER_CLASS
	bool checkBenchMarkSpeed(TimerClass* m_Timer);
#endif

	bool CheckIO();

#if defined WINDOWS_PLATFORM
	bool UpdateOSifNeeded();		// Check if OS need Updates
	bool CheckSetup();				// Check if Setup is Installed / Download & Install
	bool CheckEngineUpdates();		// Check Engine for Updates (Woma Server) / Download & Install
	bool LaunchEngine();			// Launch WOMA Engine (32bits or 64bits)

	SystemFeatures systemFeatures;
#endif

private:
	//CheckOS:
	TCHAR* GetOSversionPlatform();
#if defined USE_SYSTEM_CHECK
	bool CheckOSVersion();
#endif
	bool CheckDXGIVersion(bool* REQUIRES_WINDOWS_VISTA_SP2, bool* REQUIRES_UPDATE_KB971644);

	float GetProcessorSpeed();
	float GetProcessorSpeed4Intel(TCHAR*);

#if defined WINDOWS_PLATFORM
	TCHAR* GetOsVersion();
	DWORDLONG getAvailSystemMemory();
	std::vector<DriveList> drivesList;
	UINT driveLetter;
#endif

	// VARS:
public:
	//CheckOS:
	TCHAR	pszOS[BUFSIZE];
	UINT	MajorVersion, MinorVersion, BuildVersion;

private:
	TCHAR	osPlatform[BUFSIZE];
	UINT	DXGI_H, DXGI_L;
};

#endif
#endif