// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: mem_leak.h
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
#pragma once

// Enable MS WINDOWS run-time memory leaks check, on DEBUG builds:
// ------------------------------------------------------------------------------------------------
#include <new.h>

#if defined(_DEBUG) & !defined(NDEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <cstdlib>
	#include <crtdbg.h>

	#define NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ ) // Note: Use "NEW" instead of "new", to trace memory leaks!
	#define MALLOC(X)  woma_malloc( X, __FILE__, __LINE__, __FUNCTION__)
#else
	#define NEW new
	#define MALLOC malloc
#endif

#define HEAPALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define HEAPFREE(x) HeapFree(GetProcessHeap(), 0, (x))

namespace WOMA
{
	extern void* woma_malloc(size_t size, const char* file, int line, const char* func);
	extern void activate_mem_leak_detection();
}