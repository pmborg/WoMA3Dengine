// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: packManager.h
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// -------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"
#include "standard_platform.h"

#include "unzip.h"
#include "zip.h"

#if UNICODE
#ifdef X64
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "x64/WDebug/ZipUtils_LIBX64_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/WRelease/ZipUtils_LIBX64.lib" )		//RELEASE
	#else
		#pragma comment( lib, "x64/WRelease/ZipUtils_LIBX64.lib" )		//DBGREL
	#endif
#else
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "Win32/WDebug/ZipUtils_LIB_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/WRelease/ZipUtils_LIB.lib" )		//RELEASE
	#else
		#pragma comment( lib, "Win32/WRelease/ZipUtils_LIB.lib" )		//DBGREL
	#endif
#endif
#else
#ifdef X64
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "x64/Debug/ZipUtils_LIBX64_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/Release/ZipUtils_LIBX64.lib" )		//RELEASE
	#else
		#pragma comment( lib, "x64/Release/ZipUtils_LIBX64.lib" )		//DBGREL
	#endif
#else
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "Win32/Debug/ZipUtils_LIB_d.lib" )			//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/Release/ZipUtils_LIB.lib" )			//RELEASE
	#else
		#pragma comment( lib, "Win32/Release/ZipUtils_LIB.lib" )			//DBGREL
	#endif
#endif
#endif

extern BOOL PackDirectory(HZIP hz, const TCHAR* sPath);
extern void PackDir(STRING dir, STRING packName);

#if CHAP >= 7
#include "../../System/src/Global.h"
extern int numZipItems;
extern int zipIndx;
extern UINT packCounter, totalPackCounter;
#endif
