// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: SystemClass.cpp
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
// PURPOSE: Define APIs for systemclass.cpp which is the common OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "OSengine.h"
#include "log.h"
#include "SystemClass.h"
#include "default_settings_xml.h"
#include "woma_macros.h"

#include "OSmain_dir.h"

#if defined WINDOWS_PLATFORM
#include "language.h"
#endif

#include "stateMachine.h"

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567311;

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
#elif defined  (ANDROID_PLATFORM)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("TODO %s.%s"), GET_VERSION(__GNUC__), GET_VERSION(__GNUC_MINOR__));
#elif defined  (__GNUC__)
	StringCchPrintf(COMPILER, MAX_STR_LEN, TEXT("GCC %s.%s.%s"), GET_VERSION(__GNUC__), GET_VERSION(__GNUC_MINOR__), GET_VERSION(__GNUC_PATCHLEVEL__));
#endif	

	// APP_NAME with Binary Info:
	// -------------------------------------------------------------------------------------------
	TCHAR Wbuffer[MAX_STR_LEN] = { 0 };
#if defined WINDOWS_PLATFORM
	atow(Wbuffer, VER_PRODUCTVERSION_STR, MAX_STR_LEN); /*VER_PRODUCTVERSION_STRING_FOUR_PARTS*/
#endif

	StringCchPrintf(WOMA::APP_NAME, MAX_STR_LEN,
		TEXT("%s v%c%c%c%c.%c%c.%c%c BIN:%s OS:%s %s BUILD:%s BIN:%dbit %s CHAR:%s Lvl: %d"),
		WOMA::APP_PROJECT_NAME,
		BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
		BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1,
		//1
		COMPILER,
		//2
#if defined WIN_XP
		TEXT("WinXP")
#elif defined WIN10
		TEXT("Win10/11") //SystemHandle->systemManager->pszOS
#elif WINVER == _WIN32_WINNT_WIN7
		TEXT("WIN7")
#elif defined WINDOWS_PLATFORM
		TEXT("Windows")
#elif defined CYGWIN_PLATFORM
		TEXT("Cygwin")
#elif defined LINUX_PLATFORM
		TEXT("Linux")
#elif defined ANDROID_PLATFORM
	#if defined(__ARM_ARCH_2__)
			TEXT("Android-ARM2")
	#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
			TEXT("Android-ARM3")
	#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
			TEXT("Android-ARM4T")
	#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
			TEXT("Android-ARM5"
	#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
			TEXT("Android-ARM6T2")
	#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
			TEXT("Android-ARM6")
	#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7")
	#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7A")
	#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7R")
	#elif defined(__ARM_ARCH_7M__)
			TEXT("Android-ARM7M")
	#elif defined(__ARM_ARCH_7S__)
			TEXT("Android-ARM7S")
	#elif defined(__aarch64__) || defined(_M_ARM64)
			TEXT("Android-ARM64")
	#endif
#endif
		,
#if defined WINDOWS_PLATFORM
		// https://en.wikipedia.org/wiki/Ver_(command)
		//VER_PRODUCTBUILD
		Wbuffer
#else
		TEXT("")
#endif
		,
		//3
#if defined _DEBUG || defined DEBUG
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
#if defined WINDOWS_PLATFORM
	#ifdef __AVX512F__
			TEXT("AVX512")	/*64bits*/
	#elif __AVX2__
			TEXT("AVX2")	/*64bits*/
	#elif defined ( __AVX__ )
			TEXT("AVX")		/*64bits*/
	#elif (defined(_M_AMD64) || defined(_M_X64))
			TEXT("SSE2")	/*64bits*/
	#elif _M_IX86_FP == 2
			TEXT("SSE2")	/*32bits*/
	#elif _M_IX86_FP == 1
			TEXT("SSE")		/*32bits*/
	#else
			TEXT("IA32")	/*32bits*/
	#endif
#else
		TEXT("")
#endif
		,
		//6
#ifdef UNICODE
		TEXT("Unicode")
#else
		TEXT("Ansi")
#endif
		,
		LEVEL
		//TEXT(__DATE__)
	);
	
	// Log Title:
	StringCchPrintf(WOMA::APP_FULLNAME, sizeof(WOMA::APP_FULLNAME), TEXT("%s"), WOMA::APP_NAME);

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)WOMA::APP_FULLNAME);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("\n-------------------------------------------------------------------------------\n"));

	// Reset Vars:
	// -------------------------------------------------------------------------------------------


#if defined USE_TIMER_CLASS
	fps = NULL;
	cpu = NULL;
#endif

}



SystemClass::~SystemClass() { CLASSDELETE(); }

void SystemClass::Shutdown()
{
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("SystemClass::Shutdown()\n"));
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("-------------------------------------------------------------------------------\n"));

	AppSettings = NULL;				// Pointer to Static object, no need to free.
}
#if defined ANDROID_PLATFORM
extern android_app* app;
#endif

