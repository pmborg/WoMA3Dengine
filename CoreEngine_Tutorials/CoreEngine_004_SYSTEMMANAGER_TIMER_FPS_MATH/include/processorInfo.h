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
#pragma once

#include "main.h" // TCHAR

#include "winCpuCores.h"

extern void cpuID(unsigned regs[4], unsigned i);

class ProcessorInfo
{
public:
    ProcessorInfo();
    ~ProcessorInfo();

	WinCpuCores cpuCores;

    TCHAR processorName[MAX_PATH];
    TCHAR processorId[MAX_PATH];

private:

};

