#define CORE_ENGINE_LEVEL 1

#if CORE_ENGINE_LEVEL >= 1 && !defined GOTO_BASICS
	#define USE_LOG_MANAGER
#endif

#if _ALL_FILES_
├───CoreEngine_001
│   │   CLEAN_ALL.bat
│   │   CoreEngine_001.vcxproj
│   │   CoreEngine_001.vcxproj.filters
│   │   CoreEngine_001.vcxproj.user
│   │   core_engine_level.h
│   │   include_000.lnk
│   │   REPORT.txt
│   │   src_000.lnk
│   │
│   ├───include
│   │       language.h
│   │       log.h
│   │       mem_leak.h
│   │       OSmain_dir.h
│   │
│   └───src
│           language.cpp
│           log.cpp
│           mem_leak.cpp
│           OSmain_dir.cpp
#endif