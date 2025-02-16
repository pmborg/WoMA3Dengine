// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: TrigonometryMathClass.cpp
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
//  Use pre-calculated data to speed-up until 20x, trivial trigonometry Math Calculations.
//  Precision 2 decimal numbers, i.e. sin(0.12)
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#include "platform.h"
#if defined USE_TIMER_CLASS

#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include "main.h"

#include "Math3D.h"
#include "TrigonometryMathClass.h" //sim, cos table

#if defined ANDROID_PLATFORM
#include "math.h"
#endif

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
	WomaIntegrityCheck = 1234567142;

	Initialize();
}

TrigonometryMathClass::~TrigonometryMathClass() {CLASSDELETE();}

void TrigonometryMathClass::Initialize()
{
	printf("TrigonometryMathClass::Initialize() - START\n");
	for (UINT deg=0;deg<360*100;deg++) {
		tableSin[deg] = sin (((float)deg / 100.0f) * 0.0174532925f); //0.0174532925f (PI / 180.0f): Convert degrees to radians.
		tableCos[deg] = cos (((float)deg / 100.0f) * 0.0174532925f); //0.0174532925f (PI / 180.0f): Convert degrees to radians.
	}
	printf("TrigonometryMathClass::Initialize() - END\n");
}

// --------------------------------------------------------------------------------------------
// Perform Simple BenchMark
// --------------------------------------------------------------------------------------------

//#define DEFAULT_MATH_FUNCTIONS   // (UN-COMMENT) to test with Normal Math Functions

void TrigonometryMathClass::testMathSpeed(TimerClass* m_Timer, double &delta1, double &delta2)
    // --------------------------------------------------------------------------------------------

{
#if !defined ANDROID_PLATFORM
	INT64 currentTime=0, currentTime1=0, currentTime2=0;
#else
	struct timeval currentTime, currentTime1, currentTime2;
#endif

	{
		static float t = 0;
#if !defined ANDROID_PLATFORM
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime); // Measure the initial Time
#else
		::gettimeofday(&currentTime, NULL);
#endif
		for (UINT time = 0; time < 10000000; time++)
		{
			// Run these functions 10 Million times:
			t = sqrt((float)time);
			t = cos((float)(time % 360));
			t = sin((float)(time % 360));
		}
#if !defined ANDROID_PLATFORM
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime1);// Measure current Time
#else
		::gettimeofday(&currentTime1, NULL);
#endif
	}

#if !defined ANDROID_PLATFORM
	ASSERT(m_Timer->m_ticksPerUs > 0);
	delta1 = ((((double)currentTime1 - (double)currentTime) / (double)m_Timer->m_ticksPerUs) / (double)1000.0f);
#else

#endif

	//--------------------------------------------------------------------------------------
	{
		static float t = 0;

#if !defined ANDROID_PLATFORM
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime); // Measure the initial Time
#else
		::gettimeofday(&currentTime, NULL);
#endif
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

#if !defined ANDROID_PLATFORM
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime2);// Measure current Time
#else
		::gettimeofday(&currentTime2, NULL);
#endif
	}

#if !defined ANDROID_PLATFORM
    ASSERT (m_Timer->m_ticksPerUs > 0);
	delta2 = ((((double)currentTime2 - (double)currentTime) / (double)m_Timer->m_ticksPerUs) / (double)1000.0f);
#else

#endif

	// SAMPLE:
	#if defined NDEBUG
	WOMA_LOGManager_DebugMSG("sqrt (81): %f\n", sqrt(81.0f));// Windows Calculator: 9
	WOMA_LOGManager_DebugMSG("cos (PI): %f\n", cos(PI));		// Windows Calculator: 0.99849714986386383363576701381654
	WOMA_LOGManager_DebugMSG("sin (PI): %f\n", sin(PI));		// Windows Calculator: 0.05480366514878953088770264713332

	WOMA_LOGManager_DebugMSG("FAST_sqrt (81): %f\n", FAST_sqrt(81.0f));
	WOMA_LOGManager_DebugMSG("FAST_cos (PI): %f\n", FAST_cos(180));
	WOMA_LOGManager_DebugMSG("FAST_sin (PI): %f\n", FAST_sin(180));
	#endif

}

#endif
