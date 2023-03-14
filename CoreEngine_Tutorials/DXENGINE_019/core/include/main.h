// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: main.h
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
//	PURPOSE: "main.h" include file for WoMA ENGINE.
//
//  DEFINE PLATFORM: Defaults
//  DEFINE UNIVERSAL C++: Code Conventions/Features for project
//  DEFINE GLOBAL - MACROS
//  DEFINE The Default Application Settings
// 	DEFINE woma_exception
// 	DEFINE minidumperClass
// 	DEFINE logManager
// --------------------------------------------------------------------------------------------
#pragma once

#define WOMAAUTHOR_ID		"Pedro Borges (pmborg@yahoo.com)"						// WoMA Engine: Author
#define WOMASERVER			"woma.servegame.com"									// Define: WOMA ENGINE HOME: (Server)

// -----------------------------------------------------------------------------------------
// DEFINE PLATFORM: Defaults
// -----------------------------------------------------------------------------------------
#pragma warning( disable : 4838 )
#define _CRT_SECURE_NO_WARNINGS		// Ignore: warning C4996
#include "platform.h"				// WOMA PLATFORM	- (Check OS: WINDOWS / LINUX / ANDROID)
#include "standard_platform.h"		// WOMA INCLUDES	- Standard Platform: INCLUDES
#include "woma_macros.h"			// WOMA MACROS		- COMMON WOMA ENGINE: MACROS
#include "default_settings_xml.h"	// DEFINE:			- The "default" run-time Settings

// -------------------------------------------------------------------------------------------
// Include: COMMON TRASNVERSAL WoMA Util Libs:
// -------------------------------------------------------------------------------------------
#include "minidumperClass.h"
#include "woma_exception.h"
#include "log.h"

//	-------------------------------------------------------------------------------------------
// Global WOMA RUN-TIME SETTINGS:
//	-------------------------------------------------------------------------------------------
extern UINT CLASS_LOAD_N;
extern UINT CLASS_DELETE_N;

namespace WOMA
{
	extern Settings settings;				// Main Settings: settings.h
	extern UINT		ENGINE_LEVEL_USED;

	// SUBSYSTEM:WINDOWS
	extern PSTR		Scmdline;
	extern int		Cmdshow;
	// SUBSYSTEM:CONSOLE
	extern int		ARGc;
	extern CHAR**	ARGv;

	extern TCHAR strConsoleTitle[MAX_STR_LEN];

	extern TCHAR	APP_COMPANY_NAME[];	// "Company" Directory Name: 1st lvl
	extern TCHAR	APP_PROJECT_NAME[];	// "Project" Directory Name: 2nd lvl

	extern TCHAR	APP_NAME[MAX_STR_LEN];	// "Aplication Name"

	extern int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption = APP_NAME, bool yesORno = false);
	extern bool dirExists(STRING& dirName_in);
	extern int getTaskBarHeight();

	extern TCHAR	APP_ICO[];					// "Icon" for this aplication
	extern TCHAR	APP_FULLNAME[MAX_STR_LEN];	// "Aplication FullName"
	extern bool fileExists(STRING Filename);

	extern MiniDumper* miniDumper;

	// Defined at: main_settings.cpp
	extern STRING	filename;			// CMD line: filename

	extern TCHAR	APP_SETTINGS_FILE[];
}

extern void InitializeCelestialInfoScreen(int x, int y);

extern bool InitializeWeatherInfoScreen(int x, int y);
