// --------------------------------------------------------------------------------------------
// Filename: fpsClass.cpp
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
// PURPOSE: Get the time spent in ms (mili seconds) to render a frame
// --------------------------------------------------------------------------------------------
#include "main.h"

#include "fpsclass.h"

FpsClass::FpsClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567829;

	//private:
	m_startTime = NULL;
	m_fps = m_count = NULL;
}

FpsClass::~FpsClass() {CLASSDELETE();}

//The Initialize function sets all the counters to zero and starts the timer.
void FpsClass::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
}

//The Frame function must be called each frame so that it can increment the frame count by 1. 
//If it finds that one second has elapsed then it will store the frame count in the m_fps variable. 
//It then resets the count and starts the timer again.

void FpsClass::Frame()
{
	m_count++;

	DWORD dw = timeGetTime();
	if(dw >= (m_startTime + 1000))
	{
		m_fps = m_count; // Ticks in last second.
		m_count = 0;
		
		m_startTime = dw;
	}
}

//GetFps returns the frame per second speed for the last second that just passed. 
//This function should be constantly queried so the latest fps can be displayed to the screen.

int FpsClass::GetFps()
{
	return m_fps;
}
