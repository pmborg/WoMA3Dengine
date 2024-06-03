// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: systemclass.h
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
// PURPOSE: Export APIs for systemclass.cpp which is the common OS API
//
// --------------------------------------------------------------------------------------------
#pragma once

#include "main.h"

#include "win32InputClass.h"

#include "TrigonometryMathClass.h"
#include "systemManager.h"
#include "fpsClass.h"
#define N_SCREEN_TEXT 4			// N. of total dif. Screens

#include "xml_loader.h"

#include "ApplicationClass.h"
#include "winCpuClass.h"

#include "WomaDriverClass.h"


struct resolutionType
{
	UINT Width;
	UINT Height;
	UINT RefreshRate_Numerator;
	UINT RefreshRate_Denominator;
};

typedef struct {
	STRING label;
	int x;
	int y;
} Woma_Label;

typedef struct {
	int			hVisibility;
	int			seaLevel;
	int			size;
	int			patchSize;
	std::string	skyDayTexture;		//Note: have to be char!
	std::string	skyNightTexture;	//Note: have to be char!
} World;

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	SystemClass();
	~SystemClass();

	void Shutdown();

	WOMA::Settings* AppSettings = NULL;

	resolutionType		resolution;
	void FrameUpdate();

	InputClass* m_OsInput = NULL;
	void ProcessOSInput();
	virtual void ProcessInput() = 0;

	SystemSettings	systemDefinitions;
	SystemManager*	systemManager = NULL;
	STRING			userName;
	STRING			ComputerName;
	bool			mMaximized;

	bool SystemCheck();

	DWORD lastButtons = 0;
	DWORD lastXpos = 0, lastYpos = 0, lastZpos = 0;
	DWORD lastRpos = 0, lastUpos = 0, lastVpos = 0;
	JOYINFOEX joyInfo;

	XMLloader		xml_loader;
	STRING			XML_SETTINGS_FILE;	// Note: Have to be "char" (No STRING)
	bool LoadXmlSettings();

  #if !defined WIN_XP

	TimerClass	m_Timer;
	int			cpu;
  #endif
	FpsClass	m_Fps;
	int			fps;

	std::vector<Woma_Label> TextToPrint[N_SCREEN_TEXT];
	void ProcessPerformanceStats();

	TrigonometryMathClass	m_math;	// Init Math Class
  #if defined WINDOWS_PLATFORM && !defined WIN_XP //&& ENGINE_LEVEL >= 19
	WinCpuClass	m_Cpu;
  #endif

	ApplicationClass*		m_Application = NULL;

public:

	std::vector<WomaDriverClass*> driverList;
	WomaDriverClass* m_Driver = NULL;
	WomaDriverClass* m_contextDriver = NULL;	// Note: Used only at 20  /*wGLopenGLclass*/
  #if defined USE_ASPECT_RATIO
	float	aspect_r;	//Global Used by: WndProc
  #endif

	void LoadAllDrivers();
	bool InitializeDrivers(int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL vsync, BOOL fullscreen, float* clarColor);
	bool LoadAllGraphics();

	STRING		XML_WORLD_FILE;
	World		world;
};
