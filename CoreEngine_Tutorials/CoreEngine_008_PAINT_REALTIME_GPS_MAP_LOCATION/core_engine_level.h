#pragma once

//------------------------------------------------------------------------------------------------------------
#define DX_ENGINE_LEVEL 0
#define CORE_ENGINE_LEVEL 8
#define DEMO_TITLE TEXT("From now on: PRESS [F3] for: [REAL TIME MAP] user location.")
//------------------------------------------------------------------------------------------------------------
#if defined RELEASE && defined _NOT
	#define USE_MAIN_THREAD		// ON/OFF (Note: Dont work on openGL, NOTE!!! This is buggy with maximize!!! Dont use for now
	// Solution? https://msdn.microsoft.com/en-us/library/windows/desktop/ee417025.aspx
#endif

#if defined _DEBUG
	//#define VERBOSE_MEMORY_DEBUG			// [ON/OFF]: used (woma_macros.h)
#endif

//------------------------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 1 && !defined GOTO_BASICS
	#define USE_LOG_MANAGER		//1
#endif

#if CORE_ENGINE_LEVEL >= 2 && !defined GOTO_BASICS
	
#endif

#if CORE_ENGINE_LEVEL >= 3 && !defined GOTO_BASICS
	#define USE_PROCESS_OS_KEYS	//2
	#define USE_MINIDUMPER		//3
	#define USE_WOMA_EXCEPTION	//3
#endif

#if CORE_ENGINE_LEVEL >= 4 && !defined GOTO_BASICS
	#define USE_SYSTEM_CHECK	//4
	#define USE_TIMER_CLASS		//4
	#define ALLOW_LOADING_SPLASH//4
	#define USE_CHECK_IO		//4
#endif

#if CORE_ENGINE_LEVEL >= 5 && !defined GOTO_BASICS
	#define USE_TINYXML_LOADER	//5
	#define CLIENT_SCENE_SETUP	//5
#endif

#if CORE_ENGINE_LEVEL >= 6 && !defined GOTO_BASICS
	#define USE_IDEA_PACK		//6
#endif

#if CORE_ENGINE_LEVEL >= 7
	#define USE_ASTRO_CLASS     //7
	#define USE_NETWORK			//7
#endif

#if CORE_ENGINE_LEVEL >= 8
	#define USE_GPS_MAP			//8
#endif