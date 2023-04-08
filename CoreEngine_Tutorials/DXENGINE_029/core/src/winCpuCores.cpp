// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: winCpuCores.cpp
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
//	Original Source Code:
//	http://msdn.microsoft.com/en-us/library/windows/desktop/ms683194%28v=vs.85%29.aspx
//
// --------------------------------------------------------------------------------------------

#include "WinSystemClass.h"
#include "winCpuCores.h"
//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID

WinCpuCores::WinCpuCores()
{
	CLASSLOADER();
    WomaIntegrityCheck = 1234567829;

	numaNodeCount = NULL;
	processorPackageCount = NULL;
	processorCoreCount = NULL;
	logicalProcessorCount = NULL;
	
    processorL1CacheDataCount = NULL;
	processorL1CacheInstCount = NULL;
    processorL2CacheCount = NULL;
    processorL3CacheCount = NULL;	

	processorL1CacheDataSize = NULL;
	processorL1CacheInstSize = NULL;
	processorL2CacheSize = NULL;
	processorL3CacheSize = NULL;

	//GetProcessorInformation();
}

WinCpuCores::~WinCpuCores() {CLASSDELETE();}

//  Helper function to count bits in the processor mask
DWORD WinCpuCores::CountBits(ULONG_PTR bitMask)
{
	// 32 is more secure once we are using kernel32 anyway...
    DWORD LSHIFT = 32 - 1;//sizeof(ULONG_PTR) * 8 - 1; (Bug Fix for 32bits)
    DWORD bitSetCount = 0;
    DWORD bitTest = 1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
	}

    return bitSetCount;
}

// --------------------------------------------------------------------------------------------
// Get Number of CPU(s): #Cores, #Logical & Cache L1,L2 & L3
// --------------------------------------------------------------------------------------------
void WinCpuCores::GetProcessorInformation()
{
	WOMA_LOGManager_DebugMSG ("GetProcessorInformation()\n");

    //static DWORD processorCoreCount = 0;    // Return "0" on any failure.  That'll show them.
	if (processorCoreCount > 0)
		return;

    LPFN_GLPI Glpi = (LPFN_GLPI) GetProcAddress(GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation");
    if (NULL == Glpi)
        return;

    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    DWORD returnLength = 0;

    while (!done)
    {
        DWORD rc = Glpi(buffer, &returnLength);
        if (FALSE == rc)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)
                    free(buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION) malloc(returnLength);
                if (NULL == buffer)
                    return;	// Allocation failure\n
            }
            else
                return; // Unanticipated error
        }
        else done = TRUE;
    }

	PCACHE_DESCRIPTOR Cache;
    DWORD byteOffset = 0;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength) 
    {
        switch (ptr->Relationship) 
        {
        case RelationNumaNode:
            // Non-NUMA systems report a single record of this type.
            numaNodeCount++;
            break;

        case RelationProcessorCore:
            processorCoreCount++;

            // A hyperthreaded core supplies more than one logical processor.
            logicalProcessorCount += CountBits(ptr->ProcessorMask);
            break;

        case RelationCache:
            // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 

            Cache = &ptr->Cache;		
            if (Cache->Level == 1)
            {
				if (Cache->Type == CacheData)
				{
					processorL1CacheDataCount++;
					processorL1CacheDataSize += ptr->Cache.Size;
				}

				if (Cache->Type == CacheInstruction)
				{
					processorL1CacheInstCount++;
					processorL1CacheInstSize += ptr->Cache.Size;
				}
            }
            else if (Cache->Level == 2)
            {
                processorL2CacheCount++;
				processorL2CacheSize += ptr->Cache.Size;
            }
            else if (Cache->Level == 3)
            {
                processorL3CacheCount++;
				processorL3CacheSize += ptr->Cache.Size;
            }
            break;

        case RelationProcessorPackage:
            // Logical processors share a physical package.
            processorPackageCount++;
            break;

        default:
            WOMA_LOGManager_DebugMSG ("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n");
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }
    free(buffer);

    WOMA_LOGManager_DebugMSGAUTO(TEXT("Number of NUMA nodes: %d\n"),						numaNodeCount);
    WOMA_LOGManager_DebugMSGAUTO(TEXT("Number of physical processor packages: %d\n"),	processorPackageCount);
    WOMA_LOGManager_DebugMSGAUTO(TEXT("Number of processor cores: %d\n"),				processorCoreCount);
    WOMA_LOGManager_DebugMSGAUTO(TEXT("Number of logical processors: %d\n"),				logicalProcessorCount);

	// CPU Cache:
	UINT L1DataSize = (processorL1CacheDataSize/KBs)/processorL1CacheDataCount;
	UINT L1InstSize = (processorL1CacheInstSize/KBs)/processorL1CacheInstCount;
	UINT L2Size =  (processorL2CacheSize/KBs)/processorL2CacheCount;
	UINT L3Size = (processorL3CacheSize/KBs)/processorL3CacheCount;

	StringCchPrintf(SystemHandle->systemDefinitions.ProcessorCacheL1D,	MAX_STR_LEN, TEXT("Cache L1 Data: %dx%d %s"), processorL1CacheDataCount, (L1DataSize>1024)?L1DataSize/KBs:L1DataSize, (L1DataSize>1024)?TEXT("MBs"):TEXT("KBs"));
	StringCchPrintf(SystemHandle->systemDefinitions.ProcessorCacheL1I,	MAX_STR_LEN, TEXT("Cache L1 Inst.: %dx%d %s"), processorL1CacheInstCount, (L1InstSize>1024)?L1InstSize/KBs:L1InstSize, (L1InstSize>1024)?TEXT("MBs"):TEXT("KBs"));
	StringCchPrintf(SystemHandle->systemDefinitions.ProcessorCacheL2,	MAX_STR_LEN, TEXT("Cache L2: %dx%d %s"), processorL2CacheCount, (L2Size>1024)?L2Size/KBs:L2Size, (L2Size>1024)?TEXT("MBs"):TEXT("KBs"));
	StringCchPrintf(SystemHandle->systemDefinitions.ProcessorCacheL3,	MAX_STR_LEN, TEXT("Cache L3: %dx%d %s"), processorL3CacheCount, (L3Size>1024)?L3Size/KBs:L3Size, (L3Size>1024)?TEXT("MBs"):TEXT("KBs"));

	WOMA_LOGManager_DebugMSGAUTO(TEXT("Processor Cache:\nL1 Data: %dx%d %s\nL1 Inst.: %dx%d %s\nL2: %dx%d %s\nL3: %dx%d %s\n"),	
								processorL1CacheDataCount, (L1DataSize>1024)?L1DataSize/KBs:L1DataSize, (L1DataSize>1024)?TEXT("MBs"):TEXT("KBs"),
								processorL1CacheInstCount, (L1InstSize>1024)?L1InstSize/KBs:L1InstSize, (L1InstSize>1024)?TEXT("MBs"):TEXT("KBs"),
								processorL2CacheCount, (L2Size>1024)?L2Size/KBs:L2Size, (L2Size>1024)?TEXT("MBs"):TEXT("KBs"),
								processorL3CacheCount, (L3Size>1024)?L3Size/KBs:L3Size, (L3Size>1024)?TEXT("MBs"):TEXT("KBs"));
}


