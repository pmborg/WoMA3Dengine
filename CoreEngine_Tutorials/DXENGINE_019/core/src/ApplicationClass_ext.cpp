// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass_ext.cpp
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
// PURPOSE: 25+
// --------------------------------------------------------------------------------------------
#include "main.h"
#include "ApplicationClass.h"
#include "mem_leak.h"
#include "OSmain_dir.h"

#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

#include "winSystemClass.h"

#include <inttypes.h>

bool FORCE_RENDER_ALL = false;

ApplicationClass::ApplicationClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

	// ---------------------------------------------------------------------
	// private:

		RENDER_PAGE = DX_ENGINE_LEVEL;
  #if _DEBUG
	WOMA_LOGManager_DebugMSG("RENDER_PAGE: %d\n", RENDER_PAGE);
  #endif

	// ---------------------------------------------------------------------
	// public:
	dt = 0.0f;	// Delta time

	initWorld =  NULL;    // Get Astro Positions

	weatherClass = NULL;
	metarClass = NULL;

	ClearColor[0] = 0.5f;
	ClearColor[1] = 0.6f;
	ClearColor[2] = 0.8f;
	ClearColor[3] = 1.0f;

	// TERRAIN



	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------

	Start();
}

ApplicationClass::~ApplicationClass() {
	Shutdown();
	CLASSDELETE();
}

//	-------------------------------------------------------------------------------------------
void ApplicationClass::Shutdown()
{
	WOMA_LOGManager_DebugMSG ("ApplicationClass::Shutdown()\n");

	//3D:


	//2D:


}

//-----------------------------------------------------------------------------------------
void ApplicationClass::WOMA_APPLICATION_Shutdown()
//-----------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown()\n");

	SAFE_DELETE(initWorld);

	SAFE_DELETE(weatherClass);
	SAFE_DELETE(metarClass);

}

//-----------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_InitGUI()
//-----------------------------------------------------------------------------------------
{
	//Used by windows: CreateFont()
	SystemHandle->m_scaleX = MIN(1, SystemHandle->AppSettings->WINDOW_WIDTH / 1920.0f);
	SystemHandle->m_scaleY = MIN(1, SystemHandle->AppSettings->WINDOW_HEIGHT / 1080.0f);
	if (SystemHandle->m_scaleY > 0.9f)
		SystemHandle->m_scaleY = 1;

	SystemHandle->fontSizeX = MIN(30, 48 * SystemHandle->m_scaleX);	//To use on win32 window not DX
	SystemHandle->fontSizeY = MIN(30, 40 * SystemHandle->m_scaleY); //To use on win32 window not DX

	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()\n");

	if (!initWorld)
	{
		initWorld = NEW InitWorld;
		WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()-initWorld created\n");
	}

	if (astroClass) {
		InitializeCelestialInfoScreen(10, 10);
		WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()-InitializeCelestialInfoScreen created\n");
	}

	if (astroClass)
	{
		if (!InitializeWeatherInfoScreen(10, 10))
		{
			WOMA::WomaMessageBox(TEXT("InitializeWeatherInfoScreen"), TEXT("Error: "));
		}
		WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()-InitializeWeatherInfoScreen created\n");
	}

	return true;
}



//-------------------------------------------------------------------------------------------
bool ApplicationClass::Start()
//-------------------------------------------------------------------------------------------
{

	return true;
}

//-------------------------------------------------------------------------------------------
bool ApplicationClass::Initialize(WomaDriverClass* Driver)
//-------------------------------------------------------------------------------------------
{
	ASSERT(Driver);



	Driver->Finalize();

	return true;
}


//	-------------------------------------------------------------------------------------------


#pragma warning(pop)