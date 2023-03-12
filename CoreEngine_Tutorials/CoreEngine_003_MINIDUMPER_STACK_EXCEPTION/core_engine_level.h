#pragma once

//------------------------------------------------------------------------------------------------------------
#define DX_ENGINE_LEVEL 0
#define CORE_ENGINE_LEVEL 3
#define DEMO_TITLE TEXT("Testing MINI FILE DUMPER: C:\\Users\\Public\\Documents\\<user>\\<core-level>\\<dx-level>\\_WoMAengine2023_<DATE>.dmp")
//------------------------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 1 && !defined GOTO_BASICS
	#define USE_LOG_MANAGER
#endif
#if CORE_ENGINE_LEVEL >= 2 && !defined GOTO_BASICS
	
#endif
#if CORE_ENGINE_LEVEL >= 3 && !defined GOTO_BASICS
	#define USE_PROCESS_OS_KEYS
	#define USE_MINIDUMPER
	#define USE_WOMA_EXCEPTION
#endif

#if _ALL_FILES_
├───CoreEngine_003
│   │   CLEAN_ALL.bat
│   │   CoreEngine_003.vcxproj
│   │   CoreEngine_003.vcxproj.filters
│   │   CoreEngine_003.vcxproj.user
│   │   core_engine_level.h
│   │   include_000.lnk
│   │   include_001.lnk
│   │   include_002.lnk
│   │   REPORT.txt
│   │   src_000.lnk
│   │   src_001.lnk
│   │   src_002.lnk
│   │   WoMA.ico
│   │
│   ├───include
│   │       dumpUploader.h
│   │       fileLoader.h
│   │       minidumperClass.h
│   │       stackTrace.h
│   │       woma_exception.h
│   │
│   └───src
│           dumpUploader.cpp
│           fileLoader.cpp
│           minidumperClass.cpp
│           stackTrace.cpp
│           woma_exception.cpp
#endif