// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: woma_exception.h
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
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#pragma once

#include "platform.h"

using namespace std;//endl
#include <sstream>	//wstring
#include <fstream>	//wifstream/wofstream

// --------------------------------------------------------------------------------------------
// LINKS:
// --------------------------------------------------------------------------------------------

#if UNICODE
	#ifdef X64
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "x64/WDebug/UltimateTCPIP_LIBX64_d.lib" )		//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			//#pragma comment( lib, "x64/WRelease/UltimateTCPIP_LIBX64.lib" )		//RELEASE
			#pragma comment( lib, "x64/Release/UltimateTCPIP_LIBX64.lib" )		//RELEASE
		#else
			#pragma comment( lib, "x64/WDebug/UltimateTCPIP_LIBX64_d.lib" )		//DBGREL
		#endif
		#else
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "Win32/WDebug/UltimateTCPIP_LIB_d.lib" )		//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			#pragma comment( lib, "Win32/WRelease/UltimateTCPIP_LIB.lib" )		//RELEASE
		#else
			#pragma comment( lib, "Win32/WDebug/UltimateTCPIP_LIB_d.lib" )		//DBGREL
		#endif
	#endif
#else
	#ifdef X64
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "x64/Debug/UltimateTCPIP_LIBX64_d.lib" )		//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			#pragma comment( lib, "x64/Release/UltimateTCPIP_LIBX64.lib" )		//RELEASE
		#else
			#pragma comment( lib, "x64/Debug/UltimateTCPIP_LIBX64_d.lib" )		//DBGREL
		#endif
		#else
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "Win32/Debug/UltimateTCPIP_LIB_d.lib" )		//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			#pragma comment( lib, "Win32/Release/UltimateTCPIP_LIB.lib" )		//RELEASE
		#else
			#pragma comment( lib, "Win32/Debug/UltimateTCPIP_LIB_d.lib" )		//DBGREL
		#endif
	#endif
#endif

// --------------------------------------------------------------------------------------------
// CLASS:
// --------------------------------------------------------------------------------------------
	class woma_exception : public runtime_error
{
	public:
	woma_exception(const std::string &arg, const char *file, const char *function, int line);
	~woma_exception() throw();
	};
