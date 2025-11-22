// --------------------------------------------------------------------------------------------
// Filename: packManager.h
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
//WomaIntegrityCheck = 1234525217;

#pragma once

#include "standard_platform.h"
#include "unzip.h"
#include "zip.h"

#if CORE_ENGINE_LEVEL >= 6
extern BOOL PackDirectory(HZIP hz, const TCHAR* sPath);
extern void PackDir(STRING dir, STRING packName);
#endif

#if CORE_ENGINE_LEVEL >= 7 && defined RELEASE || DX_ENGINE_LEVEL >= 86
extern bool InitPackLibs();
extern bool StartPackLibs();

#if DX_ENGINE_LEVEL >= 86
// commonfunctions (Assimp/PBR side)
#include "commonfunctionsInterfaces.h"
#else
// Fallback for older CoreEngine levels that don’t have the header yet
extern bool InitPackLib(const TCHAR* packfilename);
#endif

#endif


