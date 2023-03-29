// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: TrigonometryMathClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
//  Use pre-calculated data to speed-up until 20x, trivial trigonometry Math Calculations.
//  Precision 2 decimal numbers, i.e. sin(0.12)
//
// --------------------------------------------------------------------------------------------
#include "platform.h"

#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include "main.h"

#include "Math3D.h"
#include "TrigonometryMathClass.h" //sim, cos table

#define SQRT_MAGIC_F 0x5f3759df

float FAST_sqrt(float x)
{
	const float xhalf = 0.5f*x;

	union // get bits for floating value
	{
		float x;
		int i;
	} u;
	u.x = x;
	u.i = SQRT_MAGIC_F - (u.i >> 1);		// gives initial guess y0
	return x*u.x*(1.5f - xhalf*u.x*u.x);	// Newton step, repeating increases accuracy
}

float tableSin[360*100], tableCos[360*100];

TrigonometryMathClass::TrigonometryMathClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567890;

	Initialize();
}

TrigonometryMathClass::~TrigonometryMathClass() {CLASSDELETE();}

void TrigonometryMathClass::Initialize()
{
	for (UINT deg=0;deg<360*100;deg++) {
		tableSin[deg] = sin (((float)deg / 100.0f) * 0.0174532925f); //0.0174532925f (PI / 180.0f): Convert degrees to radians.
		tableCos[deg] = cos (((float)deg / 100.0f) * 0.0174532925f); //0.0174532925f (PI / 180.0f): Convert degrees to radians.
	}
}

// --------------------------------------------------------------------------------------------
// Perform Simple BenchMark
// --------------------------------------------------------------------------------------------

//#define DEFAULT_MATH_FUNCTIONS   // (UN-COMMENT) to test with Normal Math Functions

STRING TrigonometryMathClass::testMathSpeed(TimerClass* m_Timer)
    // --------------------------------------------------------------------------------------------

{
	double delta1 = 0;
	double delta2 = 0;
	INT64 currentTime=0, currentTime1=0, currentTime2=0;
	{
		static float t = 0;

		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime); // Measure the initial Time
		for (UINT time = 0; time < 10000000; time++)
		{
			// Run these functions 10 Million times:
			t = sqrt((float)time);
			t = cos((float)(time % 360));
			t = sin((float)(time % 360));
		}

		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime1);// Measure current Time
	}
	ASSERT(m_Timer->m_ticksPerUs > 0);
	delta1 = ((((double)currentTime1 - (double)currentTime) / (double)m_Timer->m_ticksPerUs) / (double)1000.0f);

	//--------------------------------------------------------------------------------------
	{
		static float t = 0;

		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime); // Measure the initial Time
		for (UINT time = 0; time < 10000000; time++)
		{
			// Run these functions 10 Million times:
		#ifdef DEFAULT_MATH_FUNCTIONS
			t = sqrt((float)time);
			t = cos((float)(time % 360));
			t = sin((float)(time % 360));
		#else
		// These ones are 20x faster...
		t = FAST_sqrt((float)time);
		t = FAST_cos((float)(time % 360));
		t = FAST_sin((float)(time % 360));
		#endif
		}

		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime2);// Measure current Time
	}
    ASSERT (m_Timer->m_ticksPerUs > 0);
    delta2 = ((((double) currentTime2 - (double) currentTime) / (double)m_Timer->m_ticksPerUs) / (double)1000.0f);

    TCHAR txt[MAX_STR_LEN];
	StringCchPrintf(txt, MAX_STR_LEN, TEXT("-----------------------------------------------------------\n"));
    StringCchPrintf(txt, MAX_STR_LEN, TEXT("Benchmark1 (default MATH) to Run 100M (sqrt/sin/cos): %f ms\n"), delta1);
	WOMA_LOGManager_DebugMSG(txt);

	StringCchPrintf(txt, MAX_STR_LEN, TEXT("Benchmark2 (WOMA MATH) to Run 100M (sqrt/sin/cos): %f ms\n"), delta2);
	WOMA_LOGManager_DebugMSG(txt);
	StringCchPrintf(txt, MAX_STR_LEN, TEXT("-----------------------------------------------------------\n"));

	// SAMPLE:
	#if defined NDEBUG
	WOMA_LOGManager_DebugMSG("sqrt (81): %f\n", sqrt(81.0f));// Windows Calculator: 9
	WOMA_LOGManager_DebugMSG("cos (PI): %f\n", cos(PI));		// Windows Calculator: 0.99849714986386383363576701381654
	WOMA_LOGManager_DebugMSG("sin (PI): %f\n", sin(PI));		// Windows Calculator: 0.05480366514878953088770264713332

	WOMA_LOGManager_DebugMSG("FAST_sqrt (81): %f\n", FAST_sqrt(81.0f));
	WOMA_LOGManager_DebugMSG("FAST_cos (PI): %f\n", FAST_cos(180));
	WOMA_LOGManager_DebugMSG("FAST_sin (PI): %f\n", FAST_sin(180));
	#endif

    return txt;
}

