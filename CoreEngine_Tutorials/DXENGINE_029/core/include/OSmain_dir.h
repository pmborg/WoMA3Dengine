// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: OSmain_dir.h
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
// PURPOSE: 
//  - Log Current PATH
//  - Check if PATH if NETWORK PATH (Release)
//  - Init: PROGRAM_FILES "C:\ProgramFiles(x86)\Pmborg\Woma2017\"
//  - Init: APPDATA "C:\Users\"username"\AppData\Local\Pmborg\Woma2017\"
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567829;

#pragma once

#include "main.h"

// [ON DEBUG] APPDATA & PROGRAM_FILES are defined here:
#define INITPATH TEXT("./")

// --------------------------------------------------------------------------------------------
// Globals:
// --------------------------------------------------------------------------------------------
namespace WOMA
{
	// State Vars:
	//---------------------------------------------------
	extern int				game_state;
	extern int				previous_game_state;

	// Directory Vars:
	//---------------------------------------------------
	extern TCHAR PUBLIC_DOCUMENTS0[MAX_STR_LEN];
	extern TCHAR PUBLIC_DOCUMENTS1[MAX_STR_LEN];
	extern TCHAR PUBLIC_DOCUMENTS[MAX_STR_LEN];
	extern TCHAR PROGRAM_FILES[MAX_STR_LEN];
	extern TCHAR APPDATA1[MAX_STR_LEN];
	extern TCHAR APPDATA[MAX_STR_LEN];
	extern STRING womaTempPATH; //Use now: APPDATA

	// Functions:
	extern TCHAR* getCurrentDir();
	extern TCHAR* getTemp();
	extern TCHAR* getHome();
	extern bool logDirs(BOOL WINXP_FLAG = FALSE, BOOL bIsWow64 = FALSE);
	extern bool init_os_main_dirs(TCHAR* currentdir = NULL);
	//extern bool init_os_main_dirs(BOOL WINXP_FLAG = FALSE, BOOL bIsWow64 = FALSE, CHAR* currentdir = NULL);

	// Defined: in win32Plat.cpp
	extern BOOL bIsWow64;
	extern bool is64();
	extern BOOL isWow64();
	extern bool isXP();

	extern void setup_OSmain_dirs();

}