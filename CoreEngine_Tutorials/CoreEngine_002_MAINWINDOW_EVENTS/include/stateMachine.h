// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: stateMachine.h
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: Define the diferent states/pages on the engine
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#pragma once
#include "platform.h"

// Globals GAME STATE: [NO RENDEING]
//-----------------------------------------------------------------------------
#ifndef GAME_LOADING
#define GAME_LOADING		0 // the game is in the:    "Initialization / Load Mode"
#endif
#define GAME_PAUSED         1 // the game is in the:    "Pause Mode" 
#define GAME_MINIMIZED      2 // the game is in the:    "Minimized/Pause Mode"
//-----------------------------------------------------------------------------

// [RENDERING]
//-----------------------------------------------------------------------------

#define GAME_SETUP           15	// F6
#define GAME_MENU			 16 // Used with GAME_SETUP

#define GAME_IMGUI			 25 

#define GAME_RUN             50	// F5: the game is in the:    "Running Mode"

#if TUTORIAL_PRE_CHAP >= 91
#define GAME_LAUNCHER        60					// the game is in the:    "Running Mode"
#define GAME_LOGINMENU       23 // the game is in the:    "Login Menu Mode"
#define GAME_SETTINGSMENU    24 // the game is in the:    "Settings Menu Mode"
#define GAME_MAINMENU        25 // the game is in the:    "Main Menu Mode"
#define GAME_RESET           26 // the game is in the:    "Reset Window / DX with new settings"
#define GAME_CREATEACCOUNT   27 // the game is in the:    "Create an Account on Server"
#define GAME_LOGIN           28 // the game is in the:    "Validate Loggin with Server"
#define GAME_CREATECHAR      29 // the game is in the:    "Create a Character"
#endif

//#if CORE_ENGINE_LEVEL >= 62 //TUTORIAL_PRE_CHAP >= 26 (G:\DRIVE_MY_SOURCE_CODE\WorldOfMiddleAge\WoMA_PartII)
#define GAME_MAP             80 // the game is in the:    "Running Mode + Map Mode"
//#endif

// GAME STATE: [NO RENDEING]
//-----------------------------------------------------------------------------
#ifndef ENGINE_RESTART
	#define ENGINE_RESTART       100				// the game is in the:    "Re-start" the "SystemClass"
#endif
#ifndef GAME_STOP
	#define GAME_STOP            ENGINE_RESTART+1	// the game is in the:    "Stop Threads and Free Resources Mode": This the correct mode to: Stop the "Application"
#endif
#ifndef GAME_EXIT
	#define GAME_EXIT            ENGINE_RESTART+2	// the game is in the:    "Exit Mode (Only SystemClass should use this), After all Threads Have Completed
#endif

