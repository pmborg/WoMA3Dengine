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
