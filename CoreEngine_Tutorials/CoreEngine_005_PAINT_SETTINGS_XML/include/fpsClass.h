// --------------------------------------------------------------------------------------------
// Filename: fpsClass.h
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
//WomaIntegrityCheck = 1234567311;
#pragma once

//////////////
// INCLUDES //
//////////////

#include "main.h"

#if defined WINDOWS_PLATFORM
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: FpsClass
////////////////////////////////////////////////////////////////////////////////
class FpsClass
{
public:
	UINT WomaIntegrityCheck = 1234567311;
	FpsClass();
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	unsigned long m_startTime;
	int m_fps, m_count;
};

