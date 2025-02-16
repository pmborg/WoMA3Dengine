// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: 2UpdateOSifNeeded.cpp
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
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

// Windows Vista update (KB 971644) add support for DXGI 1.1
// Check whether we need this option

#include "OSengine.h"
#include "systemManager.h"

//------------------------------------------------------------------
// PUBLIC FUNCTIONS:
//------------------------------------------------------------------
bool SystemManager::UpdateOSifNeeded () 
{
	// Windows Vista update (KB 971644) add support for DXGI 1.1
	// Check whether we need this option

	#if defined WINDOWS_PLATFORM	
	
	bool REQUIRES_UPDATE_KB971644 = false;
	bool REQUIRES_WINDOWS_VISTA_SP2 = false;

	// Check for DXGI Version 1.1?
	if (!CheckDXGIVersion (&REQUIRES_WINDOWS_VISTA_SP2, &REQUIRES_UPDATE_KB971644)) 
	{
		// Check update for: SP2?
		if (REQUIRES_WINDOWS_VISTA_SP2) 
		#ifdef X64
			{WomaMessageBox (TEXT("DOWNLOAD LINK Windows Vista - SP2:\n http://www.microsoft.com/en-US/download/details.aspx?id=718 \n"), TEXT("FATAL ERROR: Windows Vista - Service Pack 2, is needed"), false); return false;}
		#else
			{WomaMessageBox (TEXT("DOWNLOAD LINK Windows Vista - SP2:\n http://www.microsoft.com/en-US/download/details.aspx?id=15278 \n"), TEXT("FATAL ERROR: Windows Vista - Service Pack 2, is needed"), false); return false;}
		#endif
			
		// Check update for: DX 11 (support)?
		if (REQUIRES_UPDATE_KB971644)
			{WomaMessageBox (TEXT("WARNING: Windows Vista Need This Update: KB971644\n"), TEXT("FATAL ERROR: Windows Vista"), false); return false;}
	}
	
	#endif

	return true;
}

