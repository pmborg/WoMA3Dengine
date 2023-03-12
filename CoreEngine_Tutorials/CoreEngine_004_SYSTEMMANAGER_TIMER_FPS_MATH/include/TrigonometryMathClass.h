// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: TrigonometryMathClass.h
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------
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
	TrigonometryMathClass();
	~TrigonometryMathClass();

	void Initialize();
	STRING testMathSpeed(TimerClass* m_Timer);
	//STRING testMathSpeed();

private:

};
