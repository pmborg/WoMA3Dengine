#pragma once

//------------------------------------------------------------------------------------------------------------
#define DX_ENGINE_LEVEL 0
#define CORE_ENGINE_LEVEL 2
#define DEMO_TITLE TEXT("Adding the MAIN WINDOW! (basic static version)")
//------------------------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 1 && !defined GOTO_BASICS
	#define USE_LOG_MANAGER
#endif

#if CORE_ENGINE_LEVEL >= 2 && !defined GOTO_BASICS
	//#define USE_PROCESS_OS_KEYS
#endif

#if _ALL_FILES_
├───CoreEngine_002
│   │   CLEAN_ALL.bat
│   │   CoreEngine_002.vcxproj
│   │   CoreEngine_002.vcxproj.filters
│   │   CoreEngine_002.vcxproj.user
│   │   core_engine_level.h
│   │   include_000.lnk
│   │   include_001.lnk
│   │   REPORT.txt
│   │   src_000.lnk
│   │   src_001.lnk
│   │   WoMA.ico
│   │
│   ├───include
│   │       stateMachine.h
│   │
│   └───src
│           WindowEvents.cpp
│           winSystemCallback.cpp
#endif