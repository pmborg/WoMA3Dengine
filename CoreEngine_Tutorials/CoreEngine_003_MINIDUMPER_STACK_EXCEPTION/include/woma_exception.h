// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: woma_exception.h
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2014
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------------------
// INCLUDES:
// --------------------------------------------------------------------------------------------
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
			#pragma comment( lib, "x64/WRelease/UltimateTCPIP_LIBX64.lib" )		//RELEASE
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
