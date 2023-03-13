#define CORE_ENGINE_LEVEL 1

#if CORE_ENGINE_LEVEL >= 1 && !defined GOTO_BASICS
	#define USE_LOG_MANAGER
#endif
