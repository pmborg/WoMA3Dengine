// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: core_engine_level.h
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
// PURPOSE: Define/Activate techonology by levels
// --------------------------------------------------------------------------------------------
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
