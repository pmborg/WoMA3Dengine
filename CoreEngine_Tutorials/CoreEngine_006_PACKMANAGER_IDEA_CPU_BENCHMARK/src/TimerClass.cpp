// ----------------------------------------------------------------------------------------------
// Filename: timerClass.cpp
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
// PURPOSE:  Measure the time passed between two TimerClass::Frame() Invocations. 
//  i.e. Allow to calc FPS (number of Frames Processed per Second)
// ----------------------------------------------------------------------------------------------

//NOTES:
//0,000 001 [ millionth ] 	microsecond [ µs ]
//0,001		[ thousandth ] 	millisecond [ ms ]

#include "platform.h"
#if defined USE_TIMER_CLASS

#include "timerClass.h"

TimerClass::TimerClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567890;

	//public:
	currentTime = m_startEngineTime = NULL;

	//private:
	m_frequency = NULL;
	m_startTime = NULL;
	m_frameTime = NULL;

	m_ticksPerMs = NULL;
	m_ticksPerUs = NULL;

	Initialize();
}

TimerClass::~TimerClass() {CLASSDELETE();}

//The Initialize function will first query the system to see if it supports high frequency timers. 
//If it returns a frequency then we use that value to determine how many counter ticks will occur each millisecond. 
//We can then use that value each frame to calculate the frame time. At the end of the Initialize function we query for the 
//start time of this frame to start the timing.
bool TimerClass::Initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);		//millisecond (ms)
	m_ticksPerUs = (float)(m_frequency / 1000000);	//microsecond (μs)

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	m_startEngineTime = m_startTime;

	return true;
}

//The Frame function is called for every single loop of execution by the main program. 
//This way we can calculate the difference of time between loops and determine the time it took to execute this frame. 
//We query, calculate, and then store the time for this frame into m_frameTime so that it can be used by any calling object for 
//synchronization. We then store the current time as the start of the next frame.
void TimerClass::Frame()
{
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs; //delta time in Mili Seconds since last time here...

	m_startTime = currentTime;					 //Save current time for next frame
}

//GetTime returns the most recent frame time that was calculated.
float TimerClass::GetTime()
{
	return m_frameTime;
}

#endif