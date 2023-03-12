// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: ProcessorInfo.h
// ----------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//  -----------------------------------------------------------
//  code by : Pedro Borges - pmborg@yahoo.com
//  Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// ----------------------------------------------------------------------------------------------

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

