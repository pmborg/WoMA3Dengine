//------------------------------------------------------------------------------------------------------------
#define DX_ENGINE_LEVEL 0
#define CORE_ENGINE_LEVEL 5
#define DEMO_TITLE TEXT("From now on: PRESS [F6] for: [GAME SETUP] and added FILE: C:\\Users\\Public\\Documents\\<user>\\<core-level>\\<dx-level>\\settings.xml")
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

  //#ifdef RELEASE
	#define USE_CHECK_IO
  //#endif
#endif

#if CORE_ENGINE_LEVEL >= 5 && !defined GOTO_BASICS
	#define USE_TINYXML_LOADER	
	#define CLIENT_SCENE_SETUP //USE_GAME_SYSTEM_SETTINGS
#endif
