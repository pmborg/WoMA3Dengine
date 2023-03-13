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
