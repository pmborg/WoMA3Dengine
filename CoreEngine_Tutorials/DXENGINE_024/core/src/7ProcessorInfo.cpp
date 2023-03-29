// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: 7ProcessorInfo.cpp
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

#include "WinSystemClass.h"
#include "OSmain_dir.h"
// --------------------------------------------------------------------------------------------
// Includes:
// --------------------------------------------------------------------------------------------
#include "ProcessorInfo.h"
void cpuID(unsigned regs[4], unsigned i) 
{
/*
 __asm 
  {
     mov    esi, regs
     mov    eax, i
     xor    ecx, ecx  
     cpuid  
     mov    dword ptr [esi +  0], eax
     mov    dword ptr [esi +  4], ebx  
     mov    dword ptr [esi +  8], ecx  
     mov    dword ptr [esi + 12], edx  
  }
*/
  //__cpuid((int *)regs, (int)i);

}

ProcessorInfo::ProcessorInfo()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567890;

    processorName[0] = 0;
    processorId[0] = 0;

    // --------------------------------------------------------------------------------------------
    //Get Processor Info (Name / Model):
    // --------------------------------------------------------------------------------------------
    HKEY        hKey;
    DWORD       error, type, valueLength;

    error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hKey);
    if (error == ERROR_SUCCESS) {
        valueLength = sizeof processorName;
        if (!RegQueryValueEx(hKey, TEXT("ProcessorNameString"), NULL, &type, (PBYTE) processorName, &valueLength) == ERROR_SUCCESS)
            { WomaFatalException("ProcessorNameString"); Publish_Quit_Message(); return; }

        RegCloseKey(hKey);
    }

    error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("System\\CurrentControlSet\\Control\\Session Manager\\Environment"), 0, KEY_READ, &hKey);
    if (error == ERROR_SUCCESS) {
        valueLength = sizeof processorId;
        if (!RegQueryValueEx(hKey, TEXT("processor_identifier"), NULL, &type, (PBYTE) processorId, &valueLength) == ERROR_SUCCESS)
            { WomaFatalException("processor_identifier"); Publish_Quit_Message(); return; }

        RegCloseKey(hKey);
    }

}

ProcessorInfo::~ProcessorInfo() {CLASSDELETE();}

