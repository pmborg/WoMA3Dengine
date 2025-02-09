// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: TrigonometryMathClass.h
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

#include "platform.h"
#include "timerClass.h" // To benchMark

using namespace std;	// endl
#include <sstream>		// wstring

#define FAST_sin(x) tableSin[(int)( (x < 0) ? (x+360)*100 : (x > 360) ? (x-360)*100 : x *100)]
#define FAST_cos(x) tableCos[(int)( (x < 0) ? (x+360)*100 : (x > 360) ? (x-360)*100 : x *100)]

extern float tableSin[360*100], tableCos[360*100];

////////////////////////////////////////////////////////////////////////////////
// Class name: TrigonometryMathClass
////////////////////////////////////////////////////////////////////////////////
class TrigonometryMathClass
{
public:
	UINT WomaIntegrityCheck = 1234567311;
	TrigonometryMathClass();
	~TrigonometryMathClass();

	void Initialize();
	void testMathSpeed(TimerClass* m_Timer, double& delta1, double& delta2);

private:

};
