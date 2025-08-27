// --------------------------------------------------------------------------------------------
// Filename: Math3D.cpp
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

#include "Math3D.h"
#include "TrigonometryMathClass.h"

#ifndef INFINITY
#define INFINITY	1e+8f
#endif

WOMA::vec3 operator + (const WOMA::vec3 &u, const WOMA::vec3 &v){
	return WOMA::vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

WOMA::vec3 operator + (const WOMA::vec3 &v, const float s){
	return WOMA::vec3(v.x + s, v.y + s, v.z + s);
}

WOMA::vec3 operator + (const float s, const WOMA::vec3 &v){
	return WOMA::vec3(v.x + s, v.y + s, v.z + s);
}


#if defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21
// (V2 - V1) X (V3 - V1), where X is the cross product of the 2 vectors
#define V1 0
#define V2 1
#define V3 2

#endif
