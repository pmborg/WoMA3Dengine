// --------------------------------------------------------------------------------------------
// Filename: stateMachine.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// ********************************************************************************************
#pragma once

// Globals GAME STATE: [NO RENDEING]
//-----------------------------------------------------------------------------
#define GAME_LOADING		0 // the game is in the:    "Initialization / Load Mode"
#define GAME_PAUSED         1 // the game is in the:    "Pause Mode" 
#define GAME_MINIMIZED      2 // the game is in the:    "Minimized/Pause Mode"
//-----------------------------------------------------------------------------

// [RENDERING]
//-----------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 3
	#define GAME_SYSTEM_SETTINGS 11 // OLD:ENGINE LEVEL 10
#endif
#if CORE_ENGINE_LEVEL >= 7
	#define GAME_CELESTIAL_INFO	 12 // OLD:ENGINE LEVEL 11 
#endif
#if CORE_ENGINE_LEVEL >= 8
	#define GAME_SHOW_POSITION   13 // OLD:ENGINE LEVEL 12
#endif
#if CORE_ENGINE_LEVEL >= 9
	#define GAME_WEATHER_INFO    14 // OLD:ENGINE LEVEL 13
#endif

	#define GAME_SETUP          15 // ENGINE LEVEL 20:    "System Setup"

// Engine Running: RENDER!!!
#define GAME_MENU			 45
#define GAME_RUN             50			// the game is in the:    "Running Mode"

// GAME STATE: [NO RENDEING]
//-----------------------------------------------------------------------------
#define GAME_LAUNCHER        60			// the game is in the:    "Running Mode"
#define ENGINE_RESTART       100// the game is in the:    "Re-start" the "SystemClass"
#define GAME_STOP            ENGINE_RESTART+1			// the game is in the:    "Stop Threads and Free Resources Mode": This the correct mode to: Stop the "Application"
#define GAME_EXIT            ENGINE_RESTART+2// the game is in the:    "Exit Mode (Only SystemClass should use this), After all Threads Have Completed
//-----------------------------------------------------------------------------