// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: ProcessorInfo.h
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
#pragma once

#include "main.h" // TCHAR

#include "winCpuCores.h"

extern void cpuID(unsigned regs[4], unsigned i);

class ProcessorInfo
{
public:
    UINT WomaIntegrityCheck = 1234567890;
    ProcessorInfo();
    ~ProcessorInfo();

	WinCpuCores cpuCores;

    TCHAR processorName[MAX_PATH];
    TCHAR processorId[MAX_PATH];

private:

};

