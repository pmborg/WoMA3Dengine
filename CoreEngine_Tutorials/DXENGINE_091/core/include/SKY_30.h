// --------------------------------------------------------------------------------------------
// Filename: SKY_30.h
// ----------------------------------------------------------------------------------------------
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
//WomaIntegrityCheck = 1234525256;

#pragma once

#define SkySize (terrain_squares*2+512)
#define SunDistance	(SkySize-(SkySize/5))
#define MoonDistance (SkySize-(SkySize/5))

#include "AutoGenTerrain.h"

#if ENGINE_LEVEL >= 30 || defined _DEBUG
	extern float SunX, SunY,SunZ;
	extern float MoonX, MoonY, MoonZ;
#endif
