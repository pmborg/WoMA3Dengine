// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: OSengine.h
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
// PURPOSE: Add OS engine interfaces
//
// --------------------------------------------------------------------------------------------
#pragma once

#include "main.h"
extern int Command;

#include "winsystemclass.h"			// Are we a Windows Instance?
#define SYSTEM WinSystemClass	

#include <Shellapi.h>	// CommandLineToArgv()
#include <comdef.h>		// CoInitializeEx()

void OS_ENGINE_START(int argc, char* argv[]);
void OS_ENGINE_START(LPSTR pScmdline, int iCmdshow);
int ENGINE_RUN();
int START_3D_ENGINE();
void OS_ENGINE_STOP();

extern void ENGINE_STOP();
extern void START_OS_SYSTEM_KERNEL(int argc, char* argv[]);
extern void ENGINE_STOP_ALL();