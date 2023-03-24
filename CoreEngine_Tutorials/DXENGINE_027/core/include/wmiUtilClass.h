// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: wmiUtilClass.h
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
// 
// --------------------------------------------------------------------------------------------
#pragma once

#if defined CORE_ENGINE_LEVEL 4

#include "main.h"

#include <comdef.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

class wmiUtilClass
{
//	-------------------------------------------------------------------------------------------
public:
//	-------------------------------------------------------------------------------------------
    wmiUtilClass();
    ~wmiUtilClass();

	bool GetCpuTemperature();
	bool GetTotalPhysicalMemory();
	bool GetSystemInfo();
	STRING GetMonitorDescription(HMONITOR hMonitor);

	// DATA:
    std::wstring description;	// NO STRING
    int AdapterRAM;
    std::wstring AdapterDACType;// NO STRING

	float totalMemoryCapacity; // in GB

//	-------------------------------------------------------------------------------------------
private:
//	-------------------------------------------------------------------------------------------
    bool initWMI(STRING);
    bool GetVideoControllerInfoFromWMI();
    STRING GetMonitorDescriptonFromWMI(DWORD iMonitor);

	IWbemLocator *pLocator;
    IWbemServices *pServices;
};

#endif
