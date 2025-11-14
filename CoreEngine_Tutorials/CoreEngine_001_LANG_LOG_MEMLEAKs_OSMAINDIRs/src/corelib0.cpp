// --------------------------------------------------------------------------------------------
// Filename: corelib0.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#include "OSengine.h"

#if defined WINDOWS_PLATFORM
// safe_release_seh.cpp
#include <windows.h>
#include <unknwn.h>   // IUnknown
#include <stdio.h>

// Export as C so name mangling won't cause issues
extern "C" void SafeRelease_SEH(void** pp)
{
	if (!pp || !*pp) return;

	// Defensive: protect driver faults during Release()
	__try {
		IUnknown* unk = (IUnknown*)(*pp);
		// If pointer is bogus this can still fault, that's what SEH catches
		unk->Release();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		// optional: log the exception via your logging method
		// e.g. womalogauto(TEXT("[SAFE_RELEASE_SEH] exception during Release ptr=%p\n"), *pp);
	}
	*pp = nullptr;
}
#endif

#if defined STATIC_BUILD && !defined(COMMONFUNCTIONS_DLL_EXPORTS)
#endif

#ifdef COMMONFUNCTIONS_DLL_EXPORTS
#pragma message("NOTE: Building commonfunctions DLL — self-link disabled.")
#endif
