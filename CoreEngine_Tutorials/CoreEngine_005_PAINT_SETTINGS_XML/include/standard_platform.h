// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: standard_platform.h
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
// PURPOSE: DEFINE OS SPECIFIC HEADERS
// --------------------------------------------------------------------------------------------

#pragma once
#include "platform.h"

// Include: MS WINDOWS SPECIFIC HEADERS
// ------------------------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN			// Exclude APIs such as Cryptography, DDE, RPC, Shell, and "Windows Sockets"!
#define NOCOMM						// Excludes the serial communication API.
#define _CRT_SECURE_NO_WARNINGS		// Ignore: warning C4996
#pragma warning( disable : 4098 )	// warning LNK4098: defaultlib 'MSVCRT' conflicts with use of other libs; use /NODEFAULTLIB:library
#pragma warning( disable : 4297 )	// warning C4297: 'WinMain' : function assumed not to throw an exception but do (ONCE WE USE MINI DUMPER WE DONT NEED THIS!)
#pragma warning( disable : 4091 )	// warning C4091 : 'typedef ' : ignored on left of '' when no variable is declared
#pragma warning( disable : 4995 )	//warning C4995: 'gets': name was marked as #pragma deprecated

// NOTE: Have to be on this order (1,2,3)!
#include <windows.h>// 1: Windows-specific header file for the C/C++ programming language which contains declarations for all of the functions in the Windows API
#include <tchar.h>	// 2: TCHAR (Windows header) to Support: ANSI & UNICODE
#include <strsafe.h>// 3: StringCchPrintf Secure Function to Replace: sprintf

// Provided by Microsoft Windows, which contains functions for manipulating file system directories
// -----------------------------------------------------------------------------------------------
#include <direct.h> // _chdir: 
#include <CommCtrl.h>
#include <io.h>			//_taccess()
// NOTE: Out of Memory raise exception: std::bad_alloc at memory location

// -------------------------------------------------------------------------------------------
// Include: COMMON c++ Libs:
// -------------------------------------------------------------------------------------------
// Advanced "String" Support + Advanced "file I/O" Support + Advanced "Vector" Support:
#include <sstream>	//wstring
#include <fstream>	//wifstream/wofstream
#include <iostream>
#include <vector>	//std::vector<int> Sample;
using namespace std;

// --------------------------------------------------------------------------------------------
// WOMA DEFINE: Strings and Files I/O
// --------------------------------------------------------------------------------------------
#ifdef UNICODE
#define OSTRINGSTREAM	std::wostringstream
#define STRINGSTREAM	std::wstringstream
#define STRING			std::wstring
#define IFSTREAM		std::wifstream
#define OFSTREAM		std::wofstream
#define FSTREAM			std::wfstream
#else
#define OSTRINGSTREAM	std::ostringstream
#define STRINGSTREAM	std::stringstream	// Use this way: (TCHAR*)ss.str().c_str()
#define STRING			std::string			// String str.c_str()
#define IFSTREAM		std::ifstream		// Input
#define OFSTREAM		std::ofstream		// Output
#define FSTREAM			std::fstream
#endif

// --------------------------------------------------------------------------------------------
// STANDARD DEFINE: Usefull Basic Interger Format:
// --------------------------------------------------------------------------------------------
#define u8	uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define s8	int8_t
#define s16 int16_t
#define s32 int32_t

#define _HAS_STD_BYTE 0