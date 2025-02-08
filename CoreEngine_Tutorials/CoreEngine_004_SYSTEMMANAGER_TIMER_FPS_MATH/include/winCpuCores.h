// --------------------------------------------------------------------------------------------
// Filename: winCpuCores.h
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

#include "platform.h"
#if defined WINDOWS_PLATFORM
#include <malloc.h>

//--------------------------------------------------------------------------------------
// Helper functions for querying information about the processors in the current
// system.  ( Copied from the doc page for GetLogicalProcessorInformation() )
//--------------------------------------------------------------------------------------
typedef BOOL(WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

class WinCpuCores
{
public:
	UINT WomaIntegrityCheck = 1234567311;
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

#endif