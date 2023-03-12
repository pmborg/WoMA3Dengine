// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: getCpuCores.h
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"

#include <malloc.h>

//--------------------------------------------------------------------------------------
// Helper functions for querying information about the processors in the current
// system.  ( Copied from the doc page for GetLogicalProcessorInformation() )
//--------------------------------------------------------------------------------------
typedef BOOL(WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

class WinCpuCores
{
public:
	WinCpuCores();
	~WinCpuCores();

	DWORD numaNodeCount;
	DWORD processorPackageCount;
	DWORD processorCoreCount;
	DWORD logicalProcessorCount;
	
    DWORD processorL1CacheDataCount;
	DWORD processorL1CacheInstCount;
    DWORD processorL2CacheCount;
    DWORD processorL3CacheCount;	

    DWORD processorL1CacheDataSize;
	DWORD processorL1CacheInstSize;
    DWORD processorL2CacheSize;
    DWORD processorL3CacheSize;	

	void GetProcessorInformation();

private:
	DWORD CountBits(ULONG_PTR bitMask);
};

