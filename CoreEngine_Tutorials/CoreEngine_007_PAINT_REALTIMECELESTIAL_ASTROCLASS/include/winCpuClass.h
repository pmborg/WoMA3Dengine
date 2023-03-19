// --------------------------------------------------------------------------------------------
// Filename: winCpuClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//---------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// ********************************************************************************************
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
    WinCpuClass();
    ~WinCpuClass();

	#if defined USE_LOADING_THREADS //ENGINE_LEVEL >= 25
    void SetProcessorAffinity(int cpuNumber);
	#endif

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
