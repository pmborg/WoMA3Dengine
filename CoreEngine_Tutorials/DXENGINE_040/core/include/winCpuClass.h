// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: winCpuClass.h
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
#pragma once

#include "platform.h"
#if CORE_ENGINE_LEVEL >= 6 && defined WINDOWS_PLATFORM && !defined WIN_XP

//We use the pdh library to query the cpu usage.

/////////////
// LINKING //
/////////////
#pragma comment(lib, "pdh.lib")

//////////////
// INCLUDES //
//////////////
#include <pdh.h>

///////////////////////////////////////////////////////////////////////////////
// Class name: WinCpuClass
///////////////////////////////////////////////////////////////////////////////
class WinCpuClass
{
public:
    UINT WomaIntegrityCheck = 1234567831;
    WinCpuClass();
    ~WinCpuClass();

    void SetProcessorAffinity(int cpuNumber);

    void Initialize();
    void Shutdown();
    void Frame();
    int GetCpuPercentage();

private:
    bool m_canReadCpu;
    PDH_HQUERY m_queryHandle;
    HCOUNTER m_counterHandle;
	long m_cpuUsage;

	#ifdef X64
	ULONGLONG m_lastSampleTime;
	#else
    unsigned long m_lastSampleTime;
	#endif
};
#endif
