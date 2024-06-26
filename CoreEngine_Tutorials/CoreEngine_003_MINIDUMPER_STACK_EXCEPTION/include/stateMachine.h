// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: stateMachine.h
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
// PURPOSE: Define the diferent states/pages on the engine
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#pragma once

// Globals GAME STATE: [NO RENDEING]
//-----------------------------------------------------------------------------
#define GAME_LOADING		0 // the game is in the:    "Initialization / Load Mode"
#define GAME_PAUSED         1 // the game is in the:    "Pause Mode" 
#define GAME_MINIMIZED      2 // the game is in the:    "Minimized/Pause Mode"
//-----------------------------------------------------------------------------

// [RENDERING]
//-----------------------------------------------------------------------------
#define GAME_SYSTEM_SETTINGS 11

#define GAME_SETUP           15

// Engine Running: RENDER
#define GAME_MENU			 45
#define GAME_RUN             50	// the game is in the:    "Running Mode"

// GAME STATE: [NO RENDEING]
//-----------------------------------------------------------------------------
#define GAME_LAUNCHER        60					// the game is in the:    "Running Mode"
#define ENGINE_RESTART       100				// the game is in the:    "Re-start" the "SystemClass"
#define GAME_STOP            ENGINE_RESTART+1	// the game is in the:    "Stop Threads and Free Resources Mode": This the correct mode to: Stop the "Application"
#define GAME_EXIT            ENGINE_RESTART+2	// the game is in the:    "Exit Mode (Only SystemClass should use this), After all Threads Have Completed
