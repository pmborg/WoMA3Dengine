// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: Systemclass.cpp
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
//
// PURPOSE: Define APIs for systemclass.cpp which is the common OS API
//
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "SystemClass.h"
#include "winSystemClass.h"
#include "default_settings_xml.h"
#include "OSengine.h"
#include "woma_macros.h"

#include "language.h"
#include "mem_leak.h"
#include "OSmain_dir.h"

#include "stateMachine.h"


SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567890;

	AppSettings = NULL;

	if (WOMA::game_state < ENGINE_RESTART)
		WOMA::game_state = GAME_LOADING;

	TCHAR COMPILER[MAX_STR_LEN];
#if defined(__BORLANDC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("Borland C %d.%d\n"), __BORLANDC__ / 100, __BORLANDC__ % 100);
#elif defined(__WATCOMC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("WATCOMC %d.%d\n"), __WATCOMC__ / 100, __WATCOMC__ % 100);
#elif defined(__MINGW32__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("MinGW32 %d.%02d"), __MINGW32_VERSION_MAJOR, __MINGW32_VERSION_MINOR);
#elif defined(__MINGW64__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("MinGW64 %d.%02d"), __MINGW64_VERSION_MAJOR, __MINGW64_VERSION_MINOR);
#elif defined (__INTEL_COMPILER)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("Intel C++  %d.%02d"), __INTEL_COMPILER / 100, __INTEL_COMPILER % 100);
#elif defined  (_MSC_VER)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("Visual C++ %d.%02d"), _MSC_VER / 100, _MSC_VER % 100);
#elif defined  (__GNUC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("GCC %s.%s.%s"), GET_VERSION(__GNUC__), GET_VERSION(__GNUC_MINOR__), GET_VERSION(__GNUC_PATCHLEVEL__));
#endif	

	// APP_NAME with Binary Info:
	// -------------------------------------------------------------------------------------------
	TCHAR Wbuffer[MAX_STR_LEN] = { 0 };
	atow(Wbuffer, VER_PRODUCTVERSION_STR, MAX_STR_LEN); /*VER_PRODUCTVERSION_STRING_FOUR_PARTS*/

	StringCchPrintf(WOMA::APP_NAME, MAX_STR_LEN,
		TEXT("%s v%c%c%c%c.%c%c.%c%c COMPILED:%s OS:%s %s BUILD:%s BIN:%dbit %s CHAR:%s coreLvl:0000"),
		WOMA::APP_PROJECT_NAME,
		BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
		BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1,
		//1
		COMPILER,
		//2
#if defined WIN_XP
		TEXT("WinXP")
#elif defined WIN10
		TEXT("Win10")
#else
		TEXT("Windows")
#endif
		,
		// https://en.wikipedia.org/wiki/Ver_(command)
		//VER_PRODUCTBUILD
		Wbuffer
		,
		//3
#ifdef _DEBUG
		TEXT("Debug")
#else
		TEXT("Release")
#endif
		,
		//4
#ifdef X64
		64
#else
		32
#endif
		,
		//5
#ifdef __AVX2__
		TEXT("AVX2")	/*64bits*/
#elif defined ( __AVX__ )
		TEXT("AVX")	/*64bits*/
#elif (defined(_M_AMD64) || defined(_M_X64))
		TEXT("SSE2")	/*64bits*/
#elif _M_IX86_FP == 2
		TEXT("SSE2")	/*32bits*/
#elif _M_IX86_FP == 1
		TEXT("SSE")	/*32bits*/
#else
		TEXT("IA32")	/*32bits*/
#endif
		,
		//6
#ifdef UNICODE
		TEXT("Unicode")
#else
		TEXT("Ansi")
#endif
		//,
		//TEXT(__DATE__)
	);

	// Get ENGINE LEVEL
	ItoA(CORE_ENGINE_LEVEL, (WOMA::APP_NAME + (int)((STRING)WOMA::APP_NAME).find(TEXT("0000"))), 10); /* Base: Decimal Numbers (base 10) Set the number of "Engine Level" on title*/\

		// Log Title:
		StringCchPrintf(WOMA::APP_FULLNAME, sizeof(WOMA::APP_FULLNAME), TEXT("%s"), WOMA::APP_NAME);

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)WOMA::APP_FULLNAME);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("\n-------------------------------------------------------------------------------\n"));

}


SystemClass::~SystemClass() { CLASSDELETE(); }

void SystemClass::Shutdown()
{
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("SystemClass::Shutdown()\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));

	AppSettings = NULL;				// Pointer to Static object, no need to free.
}

void SystemClass::FrameUpdate()
{

}


