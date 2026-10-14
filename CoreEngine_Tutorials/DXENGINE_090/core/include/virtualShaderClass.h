// --------------------------------------------------------------------------------------------
// Filename: virtualShaderClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once

#ifndef virtualShaderClass_h_
#define virtualShaderClass_h_

#include "platform.h"
#include "main.h"

////////////////////////////////////////////////////////////////////////////////	
// Class name: virtualShaderClass
////////////////////////////////////////////////////////////////////////////////
class virtualShaderClass
{
public:
	// --------------------------------------------------------------------------------------------
	// Internal Shader VARs to Copy to Buffers: VS
	// --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
	UINT		m_instanceCount = 0;
#endif

	float		watertime = 0.1f;

	float		fogStart = 0;
	float		fogEnd = 0;

	bool bUseGS = false;	// GS

	char vsFilenameUsed[MAX_STR_LEN] = {};
	char psFilenameUsed[MAX_STR_LEN] = {};
private:

};

#endif
