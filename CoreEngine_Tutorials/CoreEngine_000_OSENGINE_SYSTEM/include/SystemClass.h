// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: systemclass.h
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
// PURPOSE: Export APIs for systemclass.cpp which is the common OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;
#pragma once
#ifndef __SYSTEMCLASS_H__

#include "main.h" 
#include "default_settings_xml.h"

#if (defined LINUX_PLATFORM) || (defined ANDROID_PLATFORM)
#include <sys/utsname.h>
#endif

struct resolutionType
{
	UINT Width;
	UINT Height;
	UINT RefreshRate_Numerator;
	UINT RefreshRate_Denominator;
};

typedef struct
{
	TCHAR GraphicCard[MAX_STR_LEN] = {};
	//TCHAR AdapterDACType[MAX_STR_LEN] = {};
	//TCHAR AdapterRAM[MAX_STR_LEN] = {};
	TCHAR DedicatedVideoMemory[MAX_STR_LEN] = {};
	TCHAR DedicatedSystemMemory[MAX_STR_LEN] = {};
	TCHAR SharedSystemMemory[MAX_STR_LEN] = {};
} GPU;

typedef struct
{
	TCHAR szCountryNameBuffer[MAX_STR_LEN] = {}; // Will hold country

	TCHAR cCurrentPath[FILENAME_MAX] = {};

	// System
	TCHAR platform[MAX_STR_LEN] = {};
	TCHAR characterSet[MAX_STR_LEN] = {};
	TCHAR binaryArchitecture[MAX_STR_LEN] = {};
	TCHAR binaryCode[MAX_STR_LEN] = {};

	// OS
#if defined WINDOWS_PLATFORM	
	TCHAR windowsVersion[MAX_STR_LEN] = {};
	TCHAR windowsBuildVersion[MAX_STR_LEN] = {};
#else
	struct utsname ver;
#endif
	TCHAR osName[MAX_STR_LEN] = {};

	// Processor
	TCHAR	processorName[MAX_STR_LEN] = {};
	TCHAR	processorId[MAX_STR_LEN] = {};
	TCHAR	clockSpeed[MAX_STR_LEN] = {};

	TCHAR	processorPackageCount[MAX_STR_LEN] = {};
	TCHAR	NumCoreProcessors[MAX_STR_LEN] = {};
	TCHAR	logicalProcessorCount[MAX_STR_LEN] = {};

	TCHAR	ProcessorCacheL1D[MAX_STR_LEN] = {};
	TCHAR	ProcessorCacheL1I[MAX_STR_LEN] = {};
	TCHAR	ProcessorCacheL2[MAX_STR_LEN] = {};
	TCHAR	ProcessorCacheL3[MAX_STR_LEN] = {};

	// RAM
	TCHAR totalMemoryCapacity[MAX_STR_LEN] = {}; // in GB
	TCHAR freeMemory[MAX_STR_LEN] = {}; // in GB

	// BOARD/CPU:
	std::vector<STRING>				cpuFeactures;// = {};
	STRING							systemFeatures;

	// FreeSpace
	std::vector<STRING>				drives_List; // = {};

	// BenchMark MathSpeed
	STRING							benchMarkMathSpeed1;
	STRING							benchMarkMathSpeed2;

	// OS
	TCHAR cmdLine[MAX_STR_LEN] = {};

	//GPU
	std::vector<GPU>				GPUINFO;

} SystemSettings;

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567311;
	SystemClass();
	~SystemClass();

	void Shutdown();
	WOMA::Settings* AppSettings = NULL;

	bool			mMaximized;
	SystemSettings	systemDefinitions;
	
	STRING			userName;
	STRING			ComputerName;

public:

};

extern bool InitSelectedDriver();
extern bool newDriver();
#endif
