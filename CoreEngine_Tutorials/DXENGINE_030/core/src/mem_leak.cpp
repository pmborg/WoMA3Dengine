// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: mem_leak.cpp
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
// PURPOSE: Handle properlly all Out of Memory Exceptions
//			Enable run-time memory leaks check for all "new" memory allocations
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567830;

#include "main.h"
#include "mem_leak.h"

namespace WOMA
{
	// Handle properlly all Out of Memory Exceptions:
	// --------------------------------------------------------------------------------------------
	bool FreeMemory(size_t size)
	{
		// ...Free "size" resources.
		return false;	// false, if we failed the attempt to reduze resources!
	}

	int handle_out_of_memory(size_t size)
	{
		if (!FreeMemory(size)) {								// If we cant manage to get "size" of memory...
			MessageBox(NULL, TEXT("ERROR: Out of Memory!"), TEXT("FATAL ERROR"), MB_OK);
			WomaFatalException("Out of Memory!");
			return 0;											// Raise an Exception for it!
		}

		return 1;												// Retry again the memory alocation...
	}

	void activate_mem_leak_detection() {

		// Enable run-time memory leaks check for all "new" memory allocations
		// -------------------------------------------------------------------------------------------
#if defined _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF		// ON: Enable debug heap allocations and use of memory block type identifiers
			| _CRTDBG_LEAK_CHECK_DF);			// ON: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

		// START MEMORY MANAGER - To Handle: "out of Memory: Exceptions"
		// -------------------------------------------------------------------------------------------
		_set_new_handler(handle_out_of_memory);		// Set the a handler for "new" out of Memory
		_set_new_mode(1);							// Sets the new handler mode for "malloc" also!
	}


#if defined _DEBUG
void* woma_malloc(size_t size, const char* file, int line, const char* func)
{
	void* p = malloc(size); // Replace by NEW!

	CHAR fullText[256] = { 0 };
	StringCchPrintfA(fullText, 256, "[malloc] %s:%i, %s(), Size:[%li]\n", file, line, func, (int)size);
	OutputDebugStringA(fullText);

	return p;
}
#endif
}