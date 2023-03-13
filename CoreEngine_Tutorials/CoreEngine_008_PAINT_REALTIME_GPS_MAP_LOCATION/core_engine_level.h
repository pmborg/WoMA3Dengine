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
#pragma once

//------------------------------------------------------------------------------------------------------------
#define DX_ENGINE_LEVEL 0
#define CORE_ENGINE_LEVEL 8
#define DEMO_TITLE TEXT("From now on: PRESS [F3] for: [REAL TIME MAP] user location.")
//------------------------------------------------------------------------------------------------------------

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