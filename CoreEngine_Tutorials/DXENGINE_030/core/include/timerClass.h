// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: timerClass.h
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
//
// ----------------------------------------------------------------------------------------------
#pragma once

//The TimerClass is a high precision timer that measures the exact time between frames of execution. 
//Its primary use is for synchronizing objects that require a standard time frame for movement. 
//In this tutorial we won't have a use for it but we will implement it in the code so you can see how to apply it to your projects. 
//The most common usage of the TimerClass is to use the frame time to figure out what percentage of a second has passed in the 
//current frame and then move the objects by that percentage.

//////////////
// INCLUDES //
//////////////
#include "main.h" // INT64

////////////////////////////////////////////////////////////////////////////////
// Class name: TimerClass
////////////////////////////////////////////////////////////////////////////////
class TimerClass
{
public:
	UINT WomaIntegrityCheck = 1234567830;
	TimerClass();
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

public:
	INT64 currentTime, m_startEngineTime;
	float m_ticksPerMs; //Used at: lvl 34
	float m_ticksPerUs;

private:
	INT64 m_frequency;
	INT64 m_startTime;
	float m_frameTime;
};
