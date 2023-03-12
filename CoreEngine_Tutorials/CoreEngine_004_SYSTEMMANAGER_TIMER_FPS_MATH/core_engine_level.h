//------------------------------------------------------------------------------------------------------------
#define DX_ENGINE_LEVEL 0
#define CORE_ENGINE_LEVEL 4
#define DEMO_TITLE TEXT("From now on: PRESS [F1] for: [SYSTEM MANAGER DISPLAY] and added FPS(title bar)")
//------------------------------------------------------------------------------------------------------------

#if defined _DEBUG
	//#define VERBOSE_MEMORY_DEBUG			// [ON/OFF]: used (woma_macros.h)
#endif

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

#if CORE_ENGINE_LEVEL >= 4 && !defined GOTO_BASICS
	#define USE_SYSTEM_CHECK
	#define USE_TIMER_CLASS
	#define USE_CHECK_IO		//Check Mouse & Keyboard
#endif

#if _ALL_FILES_
├───CoreEngine_004
│   │   CLEAN_ALL.bat
│   │   CoreEngine_004.vcxproj
│   │   CoreEngine_004.vcxproj.filters
│   │   CoreEngine_004.vcxproj.user
│   │   core_engine_level.h
│   │   include_000.lnk
│   │   include_001.lnk
│   │   include_002.lnk
│   │   include_003.lnk
│   │   REPORT.txt
│   │   src_000.lnk
│   │   src_001.lnk
│   │   src_002.lnk
│   │   src_003.lnk
│   │   WoMA.ico
│   │
│   ├───include
│   │       processorInfo.h
│   │       systemFeatures.h
│   │       systemManager.h
│   │       timerClass.h
│   │       winCpuCores.h
│   │       wmiUtilClass.h
│   │
│   └───src
│           0CheckOS.cpp
│           1CheckSYS.cpp
│           2UpdateOSifNeeded.cpp
│           3CheckSetup.cpp
│           4CheckEngineUpdates.cpp
│           5LaunchEngine.cpp
│           6OperatingSystem.cpp
│           7ProcessorInfo.cpp
│           8SystemFeatures.cpp
│           winCpuCores.cpp
│           winWMIutilClass.cpp
#endif