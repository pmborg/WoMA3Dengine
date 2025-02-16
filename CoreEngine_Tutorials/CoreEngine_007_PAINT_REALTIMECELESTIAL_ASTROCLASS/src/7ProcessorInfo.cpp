// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: 7ProcessorInfo.cpp
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
//WomaIntegrityCheck = 1234567142;

#include "OSengine.h" //#include "WinSystemClass.h"
#include "OSmain_dir.h"
#include "standard_platform.h"
// --------------------------------------------------------------------------------------------
// Includes:
// --------------------------------------------------------------------------------------------
#include "processorInfo.h"
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
#if defined WINDOWS_PLATFORM
  //__cpuid((int *)regs, (int)i);

#elif defined LINUX_PLATFORM
  asm volatile
    ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
     : "a" (i), "c" (0));
  // ECX is set to zero for CPUID function 4

#endif
}

#if !defined WINDOWS_PLATFORM
int cpuInfo(int argc, char **argv)
{
	WOMA_LOGManager_DebugMSG ("cpuInfo\n");

	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
	char *arg = 0;
	size_t size = 0;
	while(getdelim(&arg, &size, 0, cpuinfo) != -1)
	{
		//puts(arg);
		WOMA_LOGManager_DebugMSGAUTO (TEXT("%s\n"), arg);
	}
	free(arg);
	fclose(cpuinfo);
	return 0;
}

int CountLogicalCores()
{
        FILE * fp;
        char res[128];
        fp = popen("/bin/cat /proc/cpuinfo |grep -c '^processor'","r");
        fread(res, 1, sizeof(res)-1, fp);
        fclose(fp);
        return atoi(res);
}

int CountPhysicalCores()
{
        FILE * fp;
		char res[128] = {0};
        fp = popen("/bin/cat /proc/cpuinfo | grep 'physical id' | sort | uniq | wc -l","r");
        fread(res, 1, sizeof(res)-1, fp);
        fclose(fp);
        return atoi(res);
}

static char res[128] = {0};
char* processorSetting(char* setting)
{
        FILE* fp = NULL;
		char szScratch[128] = {0};
		
		ZeroMemory(res, 128);

		sprintf(szScratch, "/bin/cat /proc/cpuinfo | grep '%s' | head -n 1", setting);
        fp = popen(szScratch,"r");
        fread(res, 1, sizeof(res)-1, fp);
        fclose(fp);

		STRING tst = res;
		int k = tst.find_last_of ("\n");
		if (k>0)
			res[k]=0;

		int j = tst.find_last_of (":")+2;
		if (j>0)
			return res+j;

        return res;
}
#endif

ProcessorInfo::ProcessorInfo()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567142;

    processorName[0] = 0;
    processorId[0] = 0;

#if defined WINDOWS_PLATFORM
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

#else
    // --------------------------------------------------------------------------------------------
    // Get the number of CPU Cores and Logical Processors:
    // --------------------------------------------------------------------------------------------

	iNumCoreProcessors = MAX(1, CountPhysicalCores());
	ilogicalProcessorCount = MAX(1, CountLogicalCores());

	// Copy to local vars: The common Buffer:

	STRING cpu_family= processorSetting("cpu family");
	STRING model = processorSetting("model");
	STRING stepping = processorSetting("stepping");
	STRING vendor_id = processorSetting("vendor_id");

	// processorId:
	int ff = sizeof(processorId);
    StringCchPrintf(processorId, MAX_STR_LEN, TEXT("Cpu Family %d Model %d Stepping %d, %s"),
														atoi(cpu_family.c_str()), 
														atoi(model.c_str()), 
														atoi(stepping.c_str()), 
														vendor_id.c_str());
	//WOMA_LOGManager_DebugMSG (processorId);

	// processorName:
	STRING model_name = processorSetting("model name");
	strcpy_s (processorName, MAX_PATH, model_name.c_str());
	//WOMA_LOGManager_DebugMSG (processorName);
#endif
}

ProcessorInfo::~ProcessorInfo() {CLASSDELETE();}

