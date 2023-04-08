// --------------------------------------------------------------------------------------------
// Filename: platform.h
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
// PURPOSE: Define/Auto-detect all global "target-settings" to compile WOMA project ENGINE
//
// - ONLY DEFINE(s) HERE:
//		NO INCLUDE(s) (except APPLE API & ANDROID API)
//		NO GLOBAL VARIABLE(s)
//
// --------------------------------------------------------------------------------------------
#pragma once
#if !defined _PLATFORM_H_
#define _PLATFORM_H_

#pragma warning( disable : 4005 )	// Disable warning C4005: '' : macro redefinition
#pragma warning( disable : 4067 )	// Disable warning C4067: unexpected tokens following preprocessor directive - expected a newline

#define disable_bin_compile_message_for_WINDOWS_PLATFORM_X64

// -------------------------------------------------------------------------------------------
// CHECK: X86 vx X64:
// -------------------------------------------------------------------------------------------
#if defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL)
	#define CPU_X86 //CPU: 32bits!
#endif

// [WIN]: Check Windows, if we are compiling in 64bits:
#if defined(_WIN64) && !defined X64
	#define X64	//CPU: 64bits!
#endif

// [GCC]: Check Linux, if we are compiling in 64bits:
#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define X64 //CPU: 64bits!
	#endif
#endif

// -------------------------------------------------------------------------------------------
// ASSERT: X86 vx X64 VALID COMPILATION OPTIONS:
// -------------------------------------------------------------------------------------------
#if defined CPU_X86 && defined X64
	static_assert(false, "Can't be X86 and X64 at the same time!");
#endif
#if !defined CPU_X86 && !defined X64
	static_assert(false, "At least one X86 or X64 need to be defined!");
#endif

// -------------------------------------------------------------------------------------------
// CHECK: Intel Intrinsics: SSE, SSE2, AVX, AVX2, AVX-512, etc...
// -------------------------------------------------------------------------------------------
	//x86intrin.h	: x86 instructions
	//mmintrin.h	: MMX(Pentium MMX!)
	//mm3dnow.h		: 3dnow!(K6 - 2) (deprecated)
	//xmmintrin.h	: SSE + MMX(Pentium 3, Athlon XP)
	//emmintrin.h	: SSE2 + SSE + MMX(Pentiuem 4, Ahtlon 64)
	//pmmintrin.h	: SSE3 + SSE2 + SSE + MMX(Pentium 4 Prescott, Ahtlon 64 San Diego)
	//tmmintrin.h	: SSSE3 + SSE3 + SSE2 + SSE + MMX(Core 2, Bulldozer)
	//popcntintrin.h: POPCNT(Core i7, Phenom subset of SSE4.2 and SSE4A)
	//ammintrin.h	: SSE4A + SSE3 + SSE2 + SSE + MMX(Phenom)
	//smmintrin.h	: SSE4_1 + SSSE3 + SSE3 + SSE2 + SSE + MMX(Core i7, Bulldozer)
	//nmmintrin.h	: SSE4_2 + SSE4_1 + SSSE3 + SSE3 + SSE2 + SSE + MMX(Core i7, Bulldozer)
	//wmmintrin.h	: AES(Core i7 Westmere, Bulldozer)
	//immintrin.h	: AVX512, AVX2, AVX, SSE4_2 + SSE4_1 + SSSE3 + SSE3 + SSE2 + SSE + MMX(Core i7 Sandy Bridge, Bulldozer)

#if !defined(_XM_NO_INTRINSICS_) && defined _MSC_VER
	// -------------------------------------------------------------------------------------------
	// Select Advanced MATH-LIB based on which CPU X86 Generation we are TARGETTING:
	// GET COMPILATION OPTION SELECTED - C++/Code Generation: SAMPLE: 
	// - Advanced Vector Extensions 512 (/arch:AVX512)
	// - Advanced Vector Extensions 2 (/arch:AVX2)
	// ...
	// -------------------------------------------------------------------------------------------
	#ifdef __AVX512F__ || defined __AVX512__	//AVX512 (Core i7/i9-7xxxX Skylake-X) (2017)  TARGET CODE: WIN10 64b+AVX512 API:DX12 (Level DX12.x)
		//To be compiled in: VS2017-VS2022
		#include <immintrin.h>
	#elif defined (__AVX2__)					//AVX2	 (i3/5/7 Haswell)	      = (Q2 2013) TARGET CODE: WIN10 64b+AVX2	API:DX12 (Level DX12.x)
		#define _XM_AVX2_INTRINSICS_
		#include <immintrin.h>					//#include <wmmintrin.h>
	#elif defined (__AVX__)						//AVX	 (i3/5/7 Sandy Bridge)    = (Q3 2011) TARGET CODE: WIN7  64b+AVX	API:DX11 (Level DX11)
		#define _XM_AVX_INTRINSICS_
		#include <immintrin.h>					//#include <wmmintrin.h>
	#elif defined (__AES__)						//AES-NI						  = (2008)
		#include <wmmintrin.h>					//#include <nmmintrin.h>
	#elif defined (__SSE4A__)					// AMD Only
		#include <ammintrin.h>					// AMD Only
	#elif defined (__SSE42__)					//SSE42 (for Intel(R) Core(TM) 2 Duo processor)	= (Nov 2008)
		#include <nmmintrin.h>					//#include <smmintrin.h>
	#elif defined (__SSE41__)					//SSE41 (for Intel(R) Core(TM) 2 Duo processor) = (2007/2008)
		#include <smmintrin.h>					//#include <tmmintrin.h>
	#elif defined (__SSE3__)					//SSE3 (Pentium 4 Prescott)		  = (April 2005)
		#include <tmmintrin.h>					//#include <pmmintrin.h>
	#elif defined (_M_X64) || defined (_M_AMD64)//SSE2 x64 (Pentium 4F)		      = (Fev 2005)TARGET CODE: WIN7  64b+SSE2 API:DX11 (Level DX9, DX10 or DX10.1)
		#define SSE2_ONLY
		#define _XM_SSE_INTRINSICS_
		#include <pmmintrin.h>					//#include <emmintrin.h>
	#elif defined (_M_IX86_FP) && _M_IX86_FP >= 2//SSE2 x32	(Pentium 4)			  = (Nov 2000) TARGET CODE: WINXP 32b+SSE2 API:DX9 / API:OPENGL 3.3+
		#define SSE2_ONLY
		#define _XM_SSE_INTRINSICS_
		#include <emmintrin.h>					//#include <xmmintrin.h>
	#elif defined _M_IX86_FP && _M_IX86_FP==1	//SSE x32	(Pentium - III Katmai)= (Fev 1999) Target: WINXP 32b+SSE
		// SSE
		#define _XM_SSE_INTRINSICS_
		#include <xmmintrin.h>					//#include <mmintrin.h>
		static_assert(false, "This Target is not valid for WOMA3D Engine");
	#elif defined __I86__ == 6					//Pentium - II					  = (May 1996) Target: Win98 x86+32b
		#include <mmintrin.h>					//MMX
		static_assert(false, "This Target is not valid for WOMA3D Engine");
	#elif defined __I86__ == 5					//Pentium - I					  = (Ago 1995) Target: MS-DOS 6.0 16b + Win95
		#include <mmintrin.h>					//MMX
		static_assert(false, "This Target is not valid for WOMA3D Engine");
	#elif defined __i486__						//IA-32		(CPU: 486)			  = (Jun 1991) Target: MS-DOS 5.0 16b + Windows 3.x
		static_assert(false, "This Target is not valid for WOMA3D Engine");
	#elif defined __i386__						//IA-32		(CPU: 386)			  = (Nov 1989) Target: MS-DOS 3.31 16b + DOS/4GW
		static_assert(false, "This Target is not valid for WOMA3D Engine");
	#else										//x86 - 286 16-bit				  = (Nov 1987) Target: MS-DOS 3.31 16b
		static_assert(false, "This Target is not valid for WOMA3D Engine");
	#endif
#endif

#ifdef _XM_NO_INTRINSICS_ // This is for 32bits Builds.
	#define m128_f32 vector4_f32
#endif

// -------------------------------------------------------------------------------------------
// ASSERT: VALID COMPILATION OPTIONS: Intel Intrinsics: SSE, SSE2, AVX, AVX2, AVX-512, etc...
// -------------------------------------------------------------------------------------------
#if defined _MSC_VER
  #ifdef X64
	#if defined SSE2_ONLY
		// Make sure that AVX Instruction is Used (Faster Code!)
		#if defined __AVX__ || defined __AVX2__
			static_assert(false, "AVX1/2 should be disabled!");
		#endif
	#else
		// Make sure that AVX Instruction is Used (Faster Code!)
		#if !defined __AVX2__
			static_assert(false, "AVX2 should be enabled!");
		#endif
	#endif
  #else 
	// Allow 32Bits Builds to Run on P4 CPU's Since: 2001+
	#if defined __AVX__ || defined __AVX2__
		static_assert(false, "AVX1/2 should be disabled!");
	#endif

		static_assert(_M_IX86_FP == 2, "SSE2 instructions should be enabled!");
  #endif
#endif


// -------------------------------------------------------------------------------------------
//	CHECK for ARM version
// -------------------------------------------------------------------------------------------
#if defined(__arm__) || defined(__thumb__) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__)
	#define CPU_ARM32	//FOR: ANDROID_PLATFORM
#elseif defined(__LP64__) || defined(_LP64) || defined(__arm64) || defined(__arm64__) || defined(_M_ARM64) || defined(__aarch64__)
	#define CPU_ARM64	//FOR: ANDROID_PLATFORM
#endif

// -------------------------------------------------------------------------------------------
//	CHECK for LOW/BIG ENDIAN
// -------------------------------------------------------------------------------------------
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	#define CPU_BIGENDIAN
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
	#define CPU_LOWENDIAN
#elif defined(__sparc) || defined(__sparc__) || \
      defined(_POWER) || defined(__powerpc__) || \
      defined(__ppc__) || defined(__hppa) || \
      defined(__PPC__) || defined(__PPC64__) || \
      defined(_MIPSEB) || defined(__ARMEB__) || \
      defined(__s390__) || \
      (defined(__sh__) && defined(__BIG_ENDIAN__)) || \
      (defined(__ia64) && defined(__BIG_ENDIAN__))
	#define CPU_BIGENDIAN
#else
	#define CPU_LOWENDIAN
#endif

// -------------------------------------------------------------------------------------------
// ASSERT: VALID COMPILATION OPTIONS:
// -------------------------------------------------------------------------------------------
#if !defined (CPU_LOWENDIAN)
	static_assert(false, "This code is for a LOWENDIAN CPU");
#endif

// -------------------------------------------------------------------------------------------
//	Automatically Select Platform to Compile: (WINDOWS_PLATFORM / LINUX_PLATFORM / ANDROID_PLATFORM)
// -------------------------------------------------------------------------------------------
/* Windows, both 32-bit and 64-bit */
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	#define WINDOWS_PLATFORM
#endif

#if defined(_WIN32) /*MSVC*/ || defined (__WINDOWS__) /*Watcom C/C++*/ || defined (__WIN32__) /*BORLANDC ++*/
  #if defined _XBOX_ONE
	#define XBOX_ONE_PLATFORM	//BUILD_FOR_XBOX_ONE
	#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
  #elif defined _XBOX
	#if _XBOX_VER >= 200
		#define XENON_PLATFORM	//BUILD_FOR_XBOX360 (XENON)
		#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
	#else
		#define XBOX_PLATFORM	//BUILD_FOR_XBOX
		#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
	#endif	
  #endif
#elif defined(ANDROID) || defined(__ANDROID__)
	#define ANDROID_PLATFORM	//BUILD_FOR_ANDROID
#elif defined(_PS3) || defined(__PS3__) 
	#define PS3_PLATFORM		//BUILD_FOR_PS3
	#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
#elif defined(_PS4) || defined(__PS4__) 
	#define PS4_PLATFORM		//BUILD_FOR_PS4
	#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
#elif defined(_PS5) || defined(__PS5__) 
	#define PS5_PLATFORM		//BUILD_FOR_PS5
	#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
	#define CYGWIN_PLATFORM		// BUILD_FOR_CYGWIN
	#define LINUX_PLATFORM		// Note: Using until now same as LINUX
#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#define BSD_PLATFORM		//BUILD_FOR_BSD
	#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
#elif defined(__linux__) || defined(__unix__) || defined(__unix)
	#define LINUX_PLATFORM		//BUILD_FOR_LINUX
#endif
#if defined __APPLE__
	#include "TargetConditionals.h"

	#if defined TARGET_OS_IPHONE || defined TARGET_IPHONE_SIMULATOR
		#define OS_IPHONE_PLATFORM	//BUILD_FOR_IOS
		#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
	#elif defined(__MACH__) || defined(__DARWIN__)
		#define OSX_PLATFORM		//BUILD_FOR_MAC
		#error "WOMA COMPILATION ERROR: This platform is not Supported yet."
	#endif
#endif

// -------------------------------------------------------------------------------------------
// ASSERT: VALID COMPILATION OPTIONS:
// -------------------------------------------------------------------------------------------
// LOG WINDOWS:
#if (defined CPU_X86 || defined X64) && defined WINDOWS_PLATFORM
	#if defined X64
		#ifndef disable_bin_compile_message_for_WINDOWS_PLATFORM_X64
		#pragma message( "TARGET: CPU_X64 on WINDOWS_PLATFORM" )  
		#endif
	#else
		#pragma message("TARGET: CPU_x86 on WINDOWS_PLATFORM")  
	#endif
#endif

// LOG LINUX:
#if (defined CPU_X86 || defined X64) && defined LINUX_PLATFORM
	#if defined X64
		#pragma message ("TARGET: CPU_X64 on LINUX_PLATFORM")
	#else
		#pragma message ("TARGET: CPU_x86 on LINUX_PLATFORM")
	#endif
#endif

// LOG ANDROID:
#if (defined CPU_X86 || defined X64) && defined ANDROID_PLATFORM
	#if defined X64
		#pragma message ("TARGET: CPU_X64 on ANDROID_PLATFORM")
	#else
		#pragma message ("TARGET: CPU_x86 on ANDROID_PLATFORM")
	#endif
#endif

#if defined CPU_ARM32 && defined ANDROID_PLATFORM
	#pragma message ("TARGET: CPU_ARM32 on ANDROID_PLATFORM")
#endif

#if defined CPU_ARM64 && defined ANDROID_PLATFORM
	#pragma message ("TARGET: CPU_ARM64 on ANDROID_PLATFORM")
#endif

// ASSERT one valid PLATFORM Selected:
#if (!defined (WINDOWS_PLATFORM)) && (!defined (LINUX_PLATFORM)) && (!defined (ANDROID_PLATFORM))
	#error "WOMA COMPILATION ERROR: Only 1, target Platform can be select to COMPILE!"
#endif

#if defined WINDOWS_PLATFORM
	#define WOMA_NEWLINE "\r\n"
#elif defined __APPLE__
	#define WOMA_NEWLINE "\r"
#else
	#define WOMA_NEWLINE "\n"
#endif

// -------------------------------------------------------------------------------------------
//	Select OS GUI API:
// -------------------------------------------------------------------------------------------
#if defined WINDOWS_PLATFORM
	#define WIN32_GUI
#endif

#ifdef LINUX_PLATFORM
	#define GTK_GUI
#endif

#ifdef ANDROID_PLATFORM
	#define SDK_GUI
#endif

// ASSERT: proper OS-GUI selected:
#if defined (WINDOWS_PLATFORM) && !defined (WIN32_GUI)
	#error "WOMA COMPILATION ERROR: WINDOWS PLATFORM ONLY SUPPORT WIN32_GUI"
#endif

#if defined (LINUX_PLATFORM) && !defined (GTK_GUI)
	#error "WOMA COMPILATION ERROR: LINUX PLATFORM ONLY SUPPORT GTK_GUI"
#endif

#if defined (ANDROID_PLATFORM) && !defined (SDK_GUI)
	#error "WOMA COMPILATION ERROR: ANDROID PLATFORM ONLY SUPPORT SDK_GUI"
#endif

// -------------------------------------------------------------------------------------------
//	Auto-select "Render-driver" to Compile: DX12 / DX11 (dx11,dx10.1,dx10 and dx9.3) / OPENGL3.3+ / DX9
// -------------------------------------------------------------------------------------------
#if defined WINDOWS_PLATFORM	
	// Should be Compiled with: VS2010(for DX11) or VS2012(for DX11) or VS2013(for DX11) or VS2015(for DX12)
	#if !defined VER_PRODUCTMAJORVERSION
		#include <ntverp.h>				// Populate: VER_PRODUCTMAJORVERSION
	#endif

	// 64 bits WIN10 SDK + AVX2, VS2015: In Target Platform Windows 10 -> (Compile for DX12)
	#if VER_PRODUCTMAJORVERSION >= 10 && _MSC_VER >= 1900
		#ifndef WIN10
			#define WIN10				// Target Windows: Window 10!
		#endif
	#endif

	// ASSERT corect and valid SDK for WIN10:
	#if VER_PRODUCTMAJORVERSION == 6 && defined WIN10 && defined X64
		#error "Please Install/Select Windows 10 SDK, in order to compile for WIN10 & DX12 target." 
		// STEP1: Go to VS2015 MENU: File New Project / Visual C++ Windows / Universal Platforms (Install)
		// STEP2:	On Project General Settings select: toolset: v140 or better...
		//			On Project General Settings select: target platform: 10.0.xxxxx.0
	#endif
#endif

#if defined _USING_V110_SDK71_	// 32 bits WINXP SDK + SSE2, VS2010 or VS2012 (update4) For Windows XP (Sample: v110_xp/v120_xp/v140_xp)" XP Toolset is used? (Compile for DX9)
	// DX9sdk can be selected	// (Use DX-APIs: for DX9) for Windows XP Builds
	#ifndef WIN_XP
		#define WIN_XP			// Target Windows: Window XP! (NOTE: Cant use "WINXP" string already used by MS )
	#endif
#endif
#if _MSC_VER >= 1600			// 64 bits WIN6 SDK + AVX, Compiled in VS2010 or VS2012 or VS2013 or VS2015 or better TO USE (DX11)
	//DX9 can be selected
	#ifndef WIN6x
		#define WIN6x			// Target Windows: Vista, 7, 8 or 8.1
		#define DX9 			//can be selected 
	#endif
#endif
#if VER_PRODUCTMAJORVERSION >= 10	// 64 bits WIN10 SDK + AVX2, VS2015: In Target Platform Windows 10 -> (Compile for DX12)
	#if _MSC_VER >= 1900
		// AVX2	 (i3/5/7 Haswell) = (Q2 2013) TARGET CODE: WIN10 64b+AVX2 API:DX12 (Level DX12.x)
		#if (defined __AVX512F__ || defined __AVX512__ || defined __AVX__ || defined __AVX2__)
			#define DX11	//can be selected DX11.x, DX10.x and DX9.x
			#define DX12	//can be selected DX12.x
		#else
			#define DX9
			#define DX11	//can be selected DX11.x, DX10.x and DX9.x
		#endif
	#endif
#endif

// Select OPENGL-APIs to COMPILE:
// -------------------------------------------------------------------------------------------
// (Windows: Vista, 7 & 8) or Linux? ---> Allow OpenGL 3.3+ & 4.0
#if defined WIN6x || defined LINUX_PLATFORM 
	#define OPENGL3  //OPENGL-API: 3.3+ ~ DX10 for Microsoft Windows / Mac OS X / Linux
#endif

#if defined WIN10 || defined LINUX_PLATFORM 
	#define OPENGL40 //OPENGL-API: 4.0  ~ DX11 for Microsoft Windows / Mac OS X / Linux	
#endif

// Android? ---> Allow GLES2
#if defined ANDROID_PLATFORM
	#include <android/api-level.h>

	// Location of the __ANDROID_API__ define
	// #include: /tmp/ndk-arm/sysroot/usr/include/android/api-level.h
	#if !defined (__ANDROID_API__)
		# error __ANDROID_API__ must be defined
	#endif

	#if defined __ANDROID_API__ && __ANDROID_API__ >= 21
		#define GLES31	//3.1 ~ DX11 - Android 5.0 (API level 21+) - for Android / BlackBerry / iOS / Symbian
	#elif defined __ANDROID_API__ && __ANDROID_API__ >= 18
		#define GLES3	//3.0 ~ DX11 - Android 4.3 (API level 18+) - for Android / BlackBerry / iOS / Symbian
	#elif defined __ANDROID_API__ && __ANDROID_API__ >= 8
		#define GLES2	//2.0 ~ DX11 - Android 2.2 (API level 8+) - for Android / BlackBerry / iOS / Symbian
	#else
			#error "MIN: __ANDROID_API__" is 9
	#endif
		
	// NOTE: Google's "Almost Native Graphics Layer Engine" (ANGLE) project provides a means to convert OpenGL ES 2.0 application calls to DirectX 
#endif

// ASSERT (Render-System API DRIVERs): Check at-least for one driver:
#if !defined (DX9) && !defined (DX11) && !defined (DX12) && \
	!defined (GLES2) && !defined (GLES3) && !defined (GLES31) && \
	!defined (OPENGL21) && !defined (OPENGL3) && !defined (OPENGL40)
	#error "WOMA COMPILATION ERROR: At least one Render-System have to be Selected to COMPILE!"
#endif

// -------------------------------------------------------------------------------------------
// ASSERT BIN Build Check:
// -------------------------------------------------------------------------------------------
// ASSERT: Only Allow WinXp in 32 bits:
#if defined X64 && defined WIN_XP
	static_assert(false, "WIN_XP: Must be compiled in 32bits");
#endif

#if defined WIN6x
	#if (defined X64 || defined WIN32)
		// Allow 32 and 64 bits: For WIN6x
	#else
		static_assert(false, "WIN6x: X64 or WIN32, have to be selected");
	#endif
#endif

#define BIG_ENDIAN 0
#define LITTLE_ENDIAN 1

//LET USER DECIDE ON: ###core_engine_level.h
#undef DX9sdk
#undef DX9
#undef DX10
#undef DX11
#undef DX12
#undef GLES2
#undef GLES3
#undef GLES31
#undef OPENGL21
#undef OPENGL3
#undef OPENGL40

// -------------------------------------------------------------------------------------------
// Windows Platforms Define (Internal-Version):
// -------------------------------------------------------------------------------------------
#define _WindowsXP32b_	5.1
#define _WindowsXP64b_	5.2
#define _Windows_Vista_ 6.0
#define _Windows_7_		6.1
#define _Windows_8_		6.2
#define _Windows_8_1_	6.3
#define _Windows_10_   10.0

// -------------------------------------------------------------------------------------------
// Define Compilation Time:
// -------------------------------------------------------------------------------------------
#define BUILD_YEAR_CH0 (__DATE__[ 7])
#define BUILD_YEAR_CH1 (__DATE__[ 8])
#define BUILD_YEAR_CH2 (__DATE__[ 9])
#define BUILD_YEAR_CH3 (__DATE__[10])

#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')

#define BUILD_MONTH_CH0 \
    ((BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC) ? '1' : '0')

#define BUILD_MONTH_CH1 \
    ( \
        (BUILD_MONTH_IS_JAN) ? '1' : \
        (BUILD_MONTH_IS_FEB) ? '2' : \
        (BUILD_MONTH_IS_MAR) ? '3' : \
        (BUILD_MONTH_IS_APR) ? '4' : \
        (BUILD_MONTH_IS_MAY) ? '5' : \
        (BUILD_MONTH_IS_JUN) ? '6' : \
        (BUILD_MONTH_IS_JUL) ? '7' : \
        (BUILD_MONTH_IS_AUG) ? '8' : \
        (BUILD_MONTH_IS_SEP) ? '9' : \
        (BUILD_MONTH_IS_OCT) ? '0' : \
        (BUILD_MONTH_IS_NOV) ? '1' : \
        (BUILD_MONTH_IS_DEC) ? '2' : \
        /* error default */    '?' \
    )

#define BUILD_DAY_CH0 ((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define BUILD_DAY_CH1 (__DATE__[ 5])

#if defined LINUX_PLATFORM
	#define WOMA_CONSOLE_APPLICATION	// Allow: OS "CMD Line Console" VS Native "WINDOWS" or "X-WINDOWS"
#endif

#define MAX_STR_LEN 256		//Used by TCHAR Arrays:
#define CONSOLE_LOG_WIDTH 800

// -------------------------------------------------------------------------------------------
#if defined NDEBUG
	#define RELEASE // ON/OFF Comercial Version (Check Serials) RELEASE = NDEBUG + "Real" Client Path(s) with Pack(s)
#endif

#if defined UNICODE
	//UNICODE
#else
	//MULTI-BYTE
#endif

// -------------------------------------------------------------------------------------------
// Define WOMA Project "Settings/Features" that will be COMPILED depending of "ENGINE_LEVEL"
// -------------------------------------------------------------------------------------------
#define FORCE_LOAD_ALL false
#include "core_engine_level.h"

#if DX_ENGINE_LEVEL >= 19 && CORE_ENGINE_LEVEL == 10	//DX9 DX11+DX10 DX12 OPENGL3
	extern int USE_THIS_GRAPHIC_CARD_ADAPTER;
#endif

#if CORE_ENGINE_LEVEL < 10	//AFTER: #include "core_engine_level.h"
	#define WOMA_CONSOLE_APPLICATION
	#define WomaDriverClass void
#else
	#if _DEBUG
		//#define RELEASE	//FORCE TO DEBUG A RELEASE bin!
		#define WOMA_CONSOLE_APPLICATION
	#else
		#define WOMA_WIN32_APPLICATION
	#endif
#endif

#endif