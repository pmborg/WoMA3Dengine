// --------------------------------------------------------------------------------------------
// Filename: AndroidSystemClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// PURPOSE:
//
// ********************************************************************************************
#pragma once

// ALL Includes to Android: http://mobilepearls.com/labs/native-android-api/

//////////////
// INCLUDES //
//////////////
#include "main.h"
#include "SystemClass.h"

#ifdef ANDROID_PLATFORM
//#include "logManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

//Android:
#if ENGINE_LEVEL >= 10
#include <jni.h>
#include <errno.h>
#include <android/log.h>

#endif

#if _NOT //DX_ENGINE_LEVEL >= 19
#include "eGLopenGLclass.h"	// Android
#endif

#if DX_ENGINE_LEVEL >= 21
	#include "ApplicationClass.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: AndroidSystemClass
////////////////////////////////////////////////////////////////////////////////
class AndroidSystemClass : public SystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567222;
	AndroidSystemClass(WOMA::Settings* AppSettings);
	~AndroidSystemClass();
	void Shutdown();

	//bool InitSelectedDriver();
	bool APPLICATION_INIT_SYSTEM();
	int	 APPLICATION_MAIN_LOOP();
#if CORE_ENGINE_LEVEL >= 2
	void ProcessFrame();
#endif
#if CORE_ENGINE_LEVEL >= 3
	void ProcessInput();
	bool InitOsInput();
#endif

	bool LoadWorldXmlSettings();

	//bool ApplicationInitMainWindow();

	void PAUSE();
	void UNPAUSE();
	void GPH_RESIZE();

	//int ApplicationRun();
	//bool ApplicationInit();
#if DX_ENGINE_LEVEL >= 19 && defined USE_LOADING_THREADS
	bool ApplicationMandatoryLoad();
  #if defined _NOT
	bool ApplicationCreateThreads();
  #endif
#endif

	#if _NOT //ENGINE_LEVEL >= 10
	bool InitializeWindows(/*eGLopenGLclass*/ void*, int&, int&);
	#endif

	#if DX_ENGINE_LEVEL >= 19 // Initializing Engine
	EGLDisplay display;
	//EGLDisplay mDisplay;
	//NativeWindowType displayWindow;
	//ANativeWindow displayWindow;

	//eGLopenGLclass* m_eglOpenGL;
	#endif

	//WOMA::Settings* AppSettings = NULL;

private:
	bool mResizing;

	//void ShutdownWindows();	

	// VARS
	// --------------------------------------------------------------
public:
	//SystemSettings	systemDefinitions;
#if DX_ENGINE_LEVEL >= 21
	World			world;
#endif
	void*			m_hWnd = NULL;
	//int				game_state;

	STRING			userName;
	STRING			ComputerName;

	//

#if CORE_ENGINE_LEVEL >= 4
	SystemManager*	systemManager;
#endif
#if CORE_ENGINE_LEVEL >= 5
	STRING			XML_SETTINGS_FILE;	// Note: Have to be "char" (No STRING)
#endif

#if defined USE_TIMER_CLASS
	TrigonometryMathClass	m_math;	// Init Math Class
#endif

	//ApplicationClass*		m_Application;	//OLD 20...

#if ENGINE_LEVEL >= 10
	InputClass*				m_OsInput;
	std::vector<Woma_Label> TextToPrint[N_SCREEN_TEXT];
#endif

#if DX_ENGINE_LEVEL >= 19
	FpsClass	m_Fps;
	//int			fps;
	int			cpu;
#if defined USE_TIMER_CLASS
	TimerClass	m_Timer;
#endif

#if defined WINDOWS_PLATFORM && !defined WIN_XP
	WinCpuClass	m_Cpu;
#endif

	STRING		XML_WORLD_FILE;

	//std::vector<WomaDriverClass*> driverList;
	resolutionType		resolution;
	//WomaDriverClass*	m_Driver;
#endif

#if DX_ENGINE_LEVEL >= 21
	bool				useJoystick;
#endif

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
	AudioClass* audio;
#endif
#if defined USE_PLAY_MUSIC
	SoundID MusicID;
	SoundSourceID MusicSourceID;
#endif
};

/////////////
// GLOBALS //
/////////////
extern AndroidSystemClass* SystemHandle;

#endif