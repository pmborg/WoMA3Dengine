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

#include "ApplicationClass.h"
#include "winCpuClass.h"

#include "WomaDriverClass.h"

typedef struct {

	int			hVisibility;
	int			seaLevel;
	int			size;
	int			patchSize;
	std::string	skyTexture;	//Note: have to be char!

} World;

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

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567829;
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



	bool onJoystickButton(int button, bool pressed);
	void onJoystickAxis(int axis, float value);
	void InitJoyStick();
	void joyStickFrame();

	DXInputClass	m_InputManager;
	bool			useJoystick=false;
	float			xScale = 0, xBias = 0;
	float			yScale = 0, yBias = 0;
	float 			zScale = 0, zBias = 0;
	float 			rScale = 0, rBias = 0;
	float 			uScale = 0, uBias = 0;
	float 			vScale = 0, vBias = 0;

	DWORD			joyFlags = 0;
	JOYCAPS			joyCaps = { 0 };
};

