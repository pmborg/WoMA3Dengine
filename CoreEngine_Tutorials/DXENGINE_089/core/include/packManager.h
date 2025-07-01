// --------------------------------------------------------------------------------------------
// Filename: packManager.h
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
//WomaIntegrityCheck = 1234525256;

#pragma once

#include "standard_platform.h"
#include "unzip.h"
#include "zip.h"

#if UNICODE
#ifdef X64
	#if defined(_DEBUG)// && !defined(NDEBUG)
		#pragma comment( lib, "x64/WDebug/ZipUtils_LIBX64_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/Release/ZipUtils_LIBX64.lib" )		//RELEASE: C:\WoMAengine2023\x64\Release\ZipUtils_LIBX64.lib
	#else
		#pragma comment( lib, "x64/WRelease/ZipUtils_LIBX64.lib" )		//DBGREL
	#endif
#else
	#if defined(_DEBUG)// && !defined(NDEBUG)
		#pragma comment( lib, "Win32/WDebug/ZipUtils_LIB_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/WRelease/ZipUtils_LIB.lib" )		//RELEASE
	#else
		#pragma comment( lib, "Win32/WRelease/ZipUtils_LIB.lib" )		//DBGREL
	#endif
#endif
#else
#ifdef X64
	#if defined(_DEBUG)// && !defined(NDEBUG)
		#pragma comment( lib, "x64/Debug/ZipUtils_LIBX64_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/Release/ZipUtils_LIBX64.lib" )		//RELEASE
	#else
		#pragma comment( lib, "x64/Release/ZipUtils_LIBX64.lib" )		//DBGREL
	#endif
#else
	#if defined(_DEBUG)// && !defined(NDEBUG)
		#pragma comment( lib, "Win32/Debug/ZipUtils_LIB_d.lib" )			//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/Release/ZipUtils_LIB.lib" )			//RELEASE
	#else
		#pragma comment( lib, "Win32/Release/ZipUtils_LIB.lib" )			//DBGREL
	#endif
#endif
#endif


#if DX_ENGINE_LEVEL >= 86
#if UNICODE
#ifdef X64
#if defined(_DEBUG)// && !defined(NDEBUG)
#pragma comment( lib, "x64/WDebug/commonfunctionsX64_d.lib" )		//DEBUG
#elif !defined _DEBUG && defined NDEBUG
#pragma comment( lib, "x64/Release/commonfunctionsX64.lib" )		//RELEASE: C:\WoMAengine2023\x64\Release\commonfunctionsX64.lib
#else
#pragma comment( lib, "x64/WRelease/commonfunctionsX64.lib" )		//DBGREL
#endif
#else
#if defined(_DEBUG)// && !defined(NDEBUG)
#pragma comment( lib, "Win32/WDebug/commonfunctions_d.lib" )		//DEBUG
#elif !defined _DEBUG && defined NDEBUG
#pragma comment( lib, "Win32/WRelease/commonfunctions.lib" )		//RELEASE
#else
#pragma comment( lib, "Win32/WRelease/commonfunctions.lib" )		//DBGREL
#endif
#endif
#else
#ifdef X64
#if defined(_DEBUG)// && !defined(NDEBUG)
#pragma comment( lib, "x64/Debug/commonfunctionsX64_d.lib" )		//DEBUG
#elif !defined _DEBUG && defined NDEBUG
#pragma comment( lib, "x64/Release/commonfunctionsX64.lib" )		//RELEASE
#else
#pragma comment( lib, "x64/Release/commonfunctionsX64.lib" )		//DBGREL
#endif
#else
#if defined(_DEBUG)// && !defined(NDEBUG)
#pragma comment( lib, "Win32/Debug/commonfunctions_d.lib" )			//DEBUG
#elif !defined _DEBUG && defined NDEBUG
#pragma comment( lib, "Win32/Release/commonfunctions.lib" )			//RELEASE
#else
#pragma comment( lib, "Win32/Release/commonfunctions.lib" )			//DBGREL
#endif
#endif
#endif
#endif

#if CORE_ENGINE_LEVEL >= 6
extern BOOL PackDirectory(HZIP hz, const TCHAR* sPath);
extern void PackDir(STRING dir, STRING packName);
#endif

#if CORE_ENGINE_LEVEL >= 7 && defined RELEASE || DX_ENGINE_LEVEL >= 86
extern bool InitPackLibs();
extern bool StartPackLibs();
extern bool InitPackLib(char* packfilename);
#endif

#ifdef RELEASE
extern int numZipItems;
extern int zipIndx;
extern UINT packCounter, totalPackCounter;
#endif

#if defined  USE_CYPHER_PACK
#define ENCODE 1
#define DECODE 2
extern int EncodeIDEA90(char* filename, int whatTOdo);
#endif