// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: mem_leak.h
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
// PURPOSE: 
//
// NOTES: 
// new										malloc()
//
// calls constructor							does not calls constructors
// It is an operator							It is a function
// Returns exact data type					Returns void*
// on failure, Throws bad_alloc exception    On failure, returns NULL
// size is calculated by compiler			size is calculated manually
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#pragma once
#include "platform.h"

// Enable MS WINDOWS run-time memory leaks check, on DEBUG builds:
// ------------------------------------------------------------------------------------------------
#if defined  WINDOWS_PLATFORM
	#include <new.h>

  #if defined _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <cstdlib>
  #endif

	#define HEAPALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
	#define HEAPFREE(x) HeapFree(GetProcessHeap(), 0, (x))

	namespace WOMA
	{
	extern void* woma_malloc(size_t size, const char* file, int line, const char* func);
	extern void activate_mem_leak_detection();
	}
#endif

#if (defined(_DEBUG) && !defined(NDEBUG)) && defined WINDOWS_PLATFORM
#include <crtdbg.h>
	#define NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ ) // Note: Use "NEW" instead of "new", to trace memory leaks!
	#define MALLOC(X)  woma_malloc( X, __FILE__, __LINE__, __FUNCTION__)
#else
	#define NEW new
	#define MALLOC malloc
#endif

