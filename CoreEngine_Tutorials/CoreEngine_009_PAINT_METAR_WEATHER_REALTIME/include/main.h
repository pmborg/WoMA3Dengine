// --------------------------------------------------------------------------------------------
// Filename: main.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
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
//WomaIntegrityCheck = 1234567142;

#pragma once

#define WOMAAUTHOR_ID		"Pedro Borges (pmborg@yahoo.com)"						// WoMA Engine: Author
#define WOMASERVER			"woma.servegame.com"									// Define: WOMA ENGINE HOME: (Server)

// -----------------------------------------------------------------------------------------
// DEFINE PLATFORM: Defaults
// -----------------------------------------------------------------------------------------
#pragma warning( disable : 4838 )
#pragma warning( disable : 5208 )	// 
#define _CRT_SECURE_NO_WARNINGS		// Ignore: warning C4996
#include "platform.h"				// WOMA PLATFORM	- (Check OS: WINDOWS / LINUX / ANDROID)
#include "standard_platform.h"		// WOMA INCLUDES	- Standard Platform: INCLUDES
#include "woma_macros.h"			// WOMA MACROS		- COMMON WOMA ENGINE: MACROS
#include "default_settings_xml.h"	// DEFINE:			- The "default" run-time Settings

// -------------------------------------------------------------------------------------------
// Include: COMMON TRASNVERSAL WoMA Util Libs:
// -------------------------------------------------------------------------------------------
#if defined USE_MINIDUMPER
#include "minidumperClass.h"
#endif
#if defined USE_WOMA_EXCEPTION
#include "woma_exception.h"
#endif
#if defined USE_LOG_MANAGER
#include "log.h"
#endif

//	-------------------------------------------------------------------------------------------
// Global WOMA RUN-TIME SETTINGS:
//	-------------------------------------------------------------------------------------------
extern UINT CLASS_LOAD_N;
extern UINT CLASS_DELETE_N;


#if defined USE_ASTRO_CLASS
extern void InitializeCelestialInfoScreen(int x, int y);
#endif

#if defined USE_METARCLASS
extern bool InitializeWeatherInfoScreen(int x, int y);
#endif
