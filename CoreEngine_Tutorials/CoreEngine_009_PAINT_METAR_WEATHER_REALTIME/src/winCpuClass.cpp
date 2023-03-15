// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: winCpuClass.cpp
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
//   Get processor benchmark info
//   Allocate logical processors to different threads
// --------------------------------------------------------------------------------------------
#include "main.h"

#if CORE_ENGINE_LEVEL >= 6 && !defined WIN_XP // Initializing Engine
#include "winCpuClass.h"
#include <math.h>
#include <Pdh.h>

#ifdef X64
#define GET_TICK_COUNT GetTickCount64
#else
#define GET_TICK_COUNT GetTickCount
#endif

WinCpuClass::WinCpuClass()
{
	CLASSLOADER();

	//private:
    m_canReadCpu=false;
    m_queryHandle = NULL;
    m_counterHandle = NULL;
	m_cpuUsage = NULL;
	m_lastSampleTime = NULL;

    Initialize();
}

WinCpuClass::~WinCpuClass() { Shutdown(); CLASSDELETE(); }

// More Info:
// http://technet.microsoft.com/en-us/library/cc750967.aspx

//The Initialize function will setup the handle for querying the cpu on its usage. 
//The query setup here will combine the usage of all the cpus in the system and gives us back a total instead of each 
//individual cpu's usage. If it can't get a query handle or poll the cpu usage for whatever reason it will set the m_canReadCpu flag 
//to false and just keep the cpu usage at zero percent. Some cpus and operating systems privilege levels can cause this to fail. 
//We also start the timer so we only sample the cpu usage once a second.

void WinCpuClass::Initialize()
{
    PDH_STATUS status;
    CONST TCHAR* COUNTER_PATH = TEXT("\\Processor(_Total)\\% processor time");                //C:\Windows\system32>typeperf -q Processor
    //CONST PWSTR COUNTER_PATH_PT = L"\\processador(_Total)\\% de tempo do processador";  //C:\Windows\system32>typeperf -q processador

    // Initialize the flag indicating whether this object can read the system cpu usage or not.
    m_canReadCpu = true;

    // Create a query object to poll cpu usage.
    status = PdhOpenQuery(NULL, 0, &m_queryHandle);
    if (status != ERROR_SUCCESS)
        m_canReadCpu = false;

    // Set query object to poll all cpus in the system.
    status = PdhAddEnglishCounter(m_queryHandle, COUNTER_PATH, 0, &m_counterHandle);// Req. Windows VISTA! Dont Work on XP
    //status = PdhAddCounter(m_queryHandle, COUNTER_PATH_PT, 0, &m_counterHandle);// Win7 PT (64 bits only!)

    if (status != ERROR_SUCCESS)
        m_canReadCpu = false;

    m_lastSampleTime = GET_TICK_COUNT();

    m_cpuUsage = 0;
}

//The Shutdown function releases the handle we used to query the cpu usage.

void WinCpuClass::Shutdown()
{
    if (m_canReadCpu)
    {
        PdhRemoveCounter(m_counterHandle);
        PdhCloseQuery(m_queryHandle);
    }
}

//Just like the FpsClass we have to call the Frame function each frame. But to reduce the amount of querying we use a m_lastSampleTime 
//variable to ensure we only sample once a second. So each second we ask the cpu for its usage and save that value in m_cpuUsage. 
//More than this is not necessary.

void WinCpuClass::Frame()
{
    PDH_FMT_COUNTERVALUE value;

    if (m_canReadCpu)
    {
        if ((m_lastSampleTime + 1000) < GET_TICK_COUNT())
        {
            m_lastSampleTime = GET_TICK_COUNT();

            PdhCollectQueryData(m_queryHandle);
            PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

            m_cpuUsage = value.longValue;
        }
    }
}

//The GetCpuPercentage function returns the value of the current cpu usage to any calling function. 
//Once again if it couldn't read the cpu for whatever reason we just set the usage to zero.

int WinCpuClass::GetCpuPercentage()
{
    return (m_canReadCpu) ? m_cpuUsage : 0;
}

#endif

