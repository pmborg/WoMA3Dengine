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
// PURPOSE: Add OS engine interfaces
// 
// This file shows the full file version for:
//   WINDOWS_PLATFORM
//   LINUX_PLATFORM
//   ANDROID_PLATFORM
// --------------------------------------------------------------------------------------------
#pragma once

#if defined _NOT //NOTES
                  ----------------  ---------                       ----------------
                  |              |  |       |                       |              |
                  |  SYSTEM MEM  |  | AUDIO |                       |  VIDEO MEM   |
                  |              |  |       |                       |              |
                  ----------------  ---------                       ----------------
                                 |  |                                      |
 ----------                 -------------  DX/DXGI                    -------------
 |HD-Drive| --------------> |           |  ------------------------ > |           |
 ----------                 |    CPU    |  dshow(VIDEO)               |    GPU    |
 ----------                 |           |  ------------------------ > |           |
 |NETWORK | --------------> -------------                             -------------
 ----------                 |    |      |                                   |
                  ----------  -------   ----------                    -------------
                  |Keyboard|  |Mouse|   |JoyStick|                    |           |
                  ----------  -------   ----------                    |  MONITOR  |
                                                                      |           |
                                                                      -------------
#endif

#include "main.h"
extern int Command;

#if defined WINDOWS_PLATFORM
#if CORE_ENGINE_LEVEL < 10
#include "winsystemclass.h"			
#define SYSTEM WinSystemClass	        // Are we a Basic Windows Instance?
#else
#include "dxwinsystemclass.h"		
#define SYSTEM dxWinSystemClass         // Are we a DX Instance?
#endif
#endif

#ifdef LINUX_PLATFORM
#include "linuxsystemclass.h"		    
#define SYSTEM LinuxSystemClass         // Are we a Linux Instance?
#endif

#ifdef ANDROID_PLATFORM
#include "androidsystemclass.h"		    
#define SYSTEM AndroidSystemClass       // Are we a Android Instance?
#endif

#if defined WINDOWS_PLATFORM
#include <Shellapi.h>	// CommandLineToArgv()
#include <comdef.h>		// CoInitializeEx()
#endif

void OS_ENGINE_START(int argc, char* argv[]);
void OS_ENGINE_START(LPSTR pScmdline, int iCmdshow);
int ENGINE_RUN();
int START_3D_ENGINE();
void OS_ENGINE_STOP();

extern void ENGINE_STOP();
extern void START_OS_SYSTEM_KERNEL(int argc, char* argv[]);
extern void ENGINE_STOP_ALL();