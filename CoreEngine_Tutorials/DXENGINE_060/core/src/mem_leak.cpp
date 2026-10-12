// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: mem_leak.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Handle properly all Out of Memory Exceptions
//			Enable run-time memory leaks check for all "new" memory allocations
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "OSengine.h"
#include "mem_leak.h"
#include "OSmain_dir.h"

#if defined DX_ENGINE
extern DXcameraClass* DX_Camera;
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
extern DXcameraClass* DX_CameraSKY;
#endif
#endif

namespace WOMA
{
	// Handle properly all Out of Memory Exceptions:
	// --------------------------------------------------------------------------------------------
	bool FreeMemory(size_t size)
	{
		// Free "size" resources:
		return false;	// FATAL: false, if we failed the attempt to reduce resources!
	}

	int handle_out_of_memory(size_t size)
	{
		if (!FreeMemory(size)) {								// If we cant manage to get "size" of memory...
			WomaMessageBox(TEXT("ERROR: Out of Memory!"), TEXT("FATAL ERROR"), MB_OK);
			WomaFatalException("Out of Memory!");
			return 0;											// Raise an Exception for it!
		}

		return 1;												// Retry again the memory allocation...
	}

#if defined(_DEBUG) && defined(WINDOWS_PLATFORM)
	static HANDLE gWarnLogFile = INVALID_HANDLE_VALUE;
	static HANDLE gAssertLogFile = INVALID_HANDLE_VALUE;
#endif

	void activate_mem_leak_detection() 
	{
		// Enable run-time memory leaks check for all "new" memory allocations
		// -------------------------------------------------------------------------------------------
#if defined _DEBUG
	#if defined WINDOWS_PLATFORM 
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF		// ON: Enable debug heap allocations and use of memory block type identifiers
			| _CRTDBG_LEAK_CHECK_DF);			// ON: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
		
	  #if false
			_CrtSetBreakAlloc(9554);
			_CrtSetBreakAlloc(9553);
			_CrtSetBreakAlloc(9552);
	  #endif

	  //FILE:	Warnlog.txt
	  #if false && defined _DEBUG
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
		 gWarnLogFile = CreateFile("Warnlog.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, gWarnLogFile);
	  #endif

	  //FILE:	Assertlog.txt
	  #if false && defined _DEBUG
		 _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
		 gAssertLogFile = CreateFile("Assertlog.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ASSERT, gAssertLogFile);
	  #endif
	#endif
#endif

		// START MEMORY MANAGER - To Handle: "out of Memory: Exceptions"
		// -------------------------------------------------------------------------------------------
	#if defined WINDOWS_PLATFORM
		_set_new_handler(handle_out_of_memory);		// Set the a handler for "new" out of Memory
		_set_new_mode(1);							// Sets the new handler mode for "malloc" also!
	#endif
	}

	void shutdown_mem_leak_detection()
	{
#if defined(_DEBUG) && defined(WINDOWS_PLATFORM)
		if (gWarnLogFile != INVALID_HANDLE_VALUE) {
			CloseHandle(gWarnLogFile);
			gWarnLogFile = INVALID_HANDLE_VALUE;
		}

		if (gAssertLogFile != INVALID_HANDLE_VALUE) {
			CloseHandle(gAssertLogFile);
			gAssertLogFile = INVALID_HANDLE_VALUE;
		}
#endif
	}

#if (defined(_DEBUG) && !defined(NDEBUG)) && defined WINDOWS_PLATFORM
	void* woma_malloc(size_t size, const char* file, int line, const char* func)
	{
		void* p = malloc(size); // Replace by NEW!

		CHAR fullText[256] = { 0 };
		StringCchPrintfA(fullText, 256, "[malloc] %s:%i, %s(), Size:[%li]\n", file, line, func, (int)size);
		womalog(fullText);

		return p;
	}
#endif

	MemoryLeakGuard::MemoryLeakGuard()
	{
		WOMA::setup_OSmain_dirs();									//1! Setup OS main dirs.
	#if CORE_ENGINE_LEVEL >= 1 && defined WINDOWS_PLATFORM
		WOMA::activate_mem_leak_detection();						//2! Activate mem leak detection!
	#endif

	// START LOG:
	#if defined USE_LOG_MANAGER
		WOMA::start_log_manager();									//3! NOTE: Keep this order!
	#endif

#if defined DX_ENGINE
		DX_Camera = NEW DXcameraClass;
	#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
		DX_CameraSKY = NEW DXcameraClass;
	#endif
#endif

		//_CrtSetBreakAlloc(193);
	}

	MemoryLeakGuard::~MemoryLeakGuard()
	{
		shutdown_mem_leak_detection();
	}

	// Instância única global.
	static MemoryLeakGuard gMemoryLeakGuard;
}

