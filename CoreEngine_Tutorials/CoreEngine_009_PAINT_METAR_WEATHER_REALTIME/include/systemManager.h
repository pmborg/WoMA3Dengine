// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: systemManager.h
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
#pragma once

#ifndef systemManager_h_
#define systemManager_h_

#include "main.h"// TCHAR

#include "timerClass.h"
#include "ProcessorInfo.h"
#include "SystemFeatures.h"

#include "wmiUtilClass.h"

struct DriveList {
	WCHAR   drive;
	__int64 freeBytesAvailable;
};

#define BUFSIZE 256

extern int GETOS(void);

typedef struct  
{
	TCHAR GraphicCard			[MAX_STR_LEN];
	TCHAR AdapterDACType		[MAX_STR_LEN];
	TCHAR AdapterRAM			[MAX_STR_LEN];
	TCHAR DedicatedVideoMemory	[MAX_STR_LEN];
	TCHAR DedicatedSystemMemory	[MAX_STR_LEN];
	TCHAR SharedSystemMemory	[MAX_STR_LEN];
} GPU;

typedef struct  
{
	TCHAR szCountryNameBuffer	[MAX_STR_LEN]; // Will hold country

	TCHAR cCurrentPath			[FILENAME_MAX];

	// System
	TCHAR platform				[MAX_STR_LEN];
	TCHAR characterSet			[MAX_STR_LEN];
	TCHAR binaryArchitecture	[MAX_STR_LEN];
	TCHAR binaryCode			[MAX_STR_LEN];

	// OS
	TCHAR windowsVersion		[MAX_STR_LEN];
	TCHAR windowsBuildNumber	[MAX_STR_LEN];
	TCHAR osName				[MAX_STR_LEN];

	// Processor
    TCHAR	processorName			[MAX_STR_LEN];
    TCHAR	processorId				[MAX_STR_LEN];
	TCHAR	clockSpeed				[MAX_STR_LEN];

    TCHAR	processorPackageCount	[MAX_STR_LEN];
    TCHAR	NumCoreProcessors		[MAX_STR_LEN];
	TCHAR	logicalProcessorCount	[MAX_STR_LEN];

	TCHAR	ProcessorCacheL1D		[MAX_STR_LEN];
	TCHAR	ProcessorCacheL1I		[MAX_STR_LEN];
	TCHAR	ProcessorCacheL2		[MAX_STR_LEN];
	TCHAR	ProcessorCacheL3		[MAX_STR_LEN];

	// RAM
	TCHAR totalMemoryCapacity		[MAX_STR_LEN]; // in GB
	TCHAR freeMemory				[MAX_STR_LEN]; // in GB

	// BOARD/CPU:
	 std::vector<STRING>		cpuFeactures;
	 STRING systemFeatures;

	// FreeSpace
    std::vector<STRING> drives_List;

	// BenchMark MathSpeed
	 STRING benchMarkMathSpeed;

	// OS
	TCHAR cmdLine	[MAX_STR_LEN];

	//GPU
	std::vector<GPU>	GPUINFO;

} SystemSettings;

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemManager
////////////////////////////////////////////////////////////////////////////////
class SystemManager
{
public:
	UINT WomaIntegrityCheck = 1234567890;
	SystemManager();
	~SystemManager();

	bool CheckOS ();				// Detect Version & System Check

	bool checkCPU();

	float CPUSpeedMHz;
    ProcessorInfo processorInfo;

	wmiUtilClass wmiUtil;

	bool checkRAM();

	bool checkDiskFreeSpace();

	bool checkCPUFeatures ();

	bool checkBenchMarkSpeed(TimerClass* m_Timer);

	bool CheckIO ();

	bool UpdateOSifNeeded();		// Check if OS need Updates
	bool CheckSetup();				// Check if Setup is Installed / Download & Install
	bool CheckEngineUpdates();		// Check Engine for Updates (Woma Server) / Download & Install
	bool LaunchEngine();			// Launch WOMA Engine (32bits or 64bits)

    SystemFeatures systemFeatures;

private:
	//CheckOS:
	TCHAR* GetOSversionPlatform();
	bool CheckOSVersion ();
	bool CheckDXGIVersion (bool* REQUIRES_WINDOWS_VISTA_SP2, bool* REQUIRES_UPDATE_KB971644);

	float GetProcessorSpeed();
	float GetProcessorSpeed4Intel(TCHAR*);

	DWORDLONG getAvailSystemMemory();

    std::vector<DriveList> drivesList;
    UINT driveLetter;

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
