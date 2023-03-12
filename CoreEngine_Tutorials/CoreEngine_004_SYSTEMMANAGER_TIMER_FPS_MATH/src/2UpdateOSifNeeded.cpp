// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: SystemManager_2UpdateOSifNeeded.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------

#include "main.h"
#include "systemManager.h"

//------------------------------------------------------------------
// PUBLIC FUNCTIONS:
//------------------------------------------------------------------
bool SystemManager::UpdateOSifNeeded () 
{
	// Windows Vista update (KB 971644) add support for DXGI 1.1
	// Check whether we need this option

	bool REQUIRES_UPDATE_KB971644 = false;
	bool REQUIRES_WINDOWS_VISTA_SP2 = false;

	// Check for DXGI Version 1.1?
	if (!CheckDXGIVersion (&REQUIRES_WINDOWS_VISTA_SP2, &REQUIRES_UPDATE_KB971644)) 
	{
		// Check update for: SP2?
		if (REQUIRES_WINDOWS_VISTA_SP2) 
		#ifdef X64
			{WOMA::WomaMessageBox (TEXT("DOWNLOAD LINK Windows Vista - SP2:\n http://www.microsoft.com/en-US/download/details.aspx?id=718 \n"), TEXT("FATAL ERROR: Windows Vista - Service Pack 2, is needed")); return false;}
		#else
			{WOMA::WomaMessageBox (TEXT("DOWNLOAD LINK Windows Vista - SP2:\n http://www.microsoft.com/en-US/download/details.aspx?id=15278 \n"), TEXT("FATAL ERROR: Windows Vista - Service Pack 2, is needed")); return false;}
		#endif
			
		// Check update for: DX 11 (support)?
		if (REQUIRES_UPDATE_KB971644)
			{WOMA::WomaMessageBox (TEXT("WARNING: Windows Vista Need This Update: KB971644\n"), TEXT("FATAL ERROR: Windows Vista")); return false;}
	}
	
	return true;
}

