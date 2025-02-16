// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass.cpp
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
// PURPOSE: 25+
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#include "platform.h"
#include "OSengine.h"
#include "main.h"
#include "ApplicationClass.h"
#include "OSmain_dir.h"
#include "mem_leak.h"

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
extern RApplicationClass* r_Application;
#endif
#if defined LINUX_PLATFORM
extern MyWin Win;
#endif
#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

#if defined (SCENE_COMPOUND) //TUTORIAL_CHAP >= 55
#include "compound.h"
void CompoundReadFunction(WomaDriverClass* Driver);
#endif

#include <inttypes.h>

bool FORCE_RENDER_ALL = false;

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
UINT g_NetID = 0;
#endif

ApplicationClass::ApplicationClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567142;

	// ---------------------------------------------------------------------
	// private:

		RENDER_PAGE = (int)CORE_ENGINE_LEVEL;
  #if _DEBUG
	WOMA_LOGManager_DebugMSG("RENDER_PAGE: %d\n", RENDER_PAGE);
  #endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	g_NetID = NULL;
	m_NextPosition = NULL;
#endif

	// ---------------------------------------------------------------------
	// public:
	dt = 0.0f;	// Delta time
	dayLightFade = false;

#if defined USE_ASTRO_CLASS
	initWorld =  NULL;    // Get Astro Positions
#endif

#if CORE_ENGINE_LEVEL >= 9 && defined USE_METARCLASS
	weatherClass = NULL;
	metarClass = NULL;
#endif

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN //24
	//autoGenUnderWaterTerrain		= NULL;
#endif

	// TERRAIN

#if defined USE_LIGHT_RAY // LightModel
	m_lightRayModel = NULL;
#endif

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
	WOMA_LOGManager_DebugMSG("ApplicationClass::Shutdown()\n");

	//3D:


#if defined USE_CUBE // Cubes
#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELDX(m_cube1Model);
		SAFE_SHUTDOWN_MODELDX(m_cube2Model);
		SAFE_SHUTDOWN_MODELDX(m_cube3Model);
	}
#endif

#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_cube1Model);
		SAFE_SHUTDOWN_MODELGL3(m_cube2Model);
		SAFE_SHUTDOWN_MODELGL3(m_cube3Model);

	}
#endif
#endif

#if defined USE_DX10DRIVER_FONTS
	SAFE_SHUTDOWN(m_FontV2Shader);
#endif

	//2D:
#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
	DEMO_WOMA_APPLICATION_Shutdown2D();
#endif


#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN
	//SAFE_DELETE (autoGenUnderWaterTerrain);
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN
	//SAFE_DELETE (mainTerrain);
#endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	for (UINT i = 0; i <m_Position.size(); i++) {
		SAFE_DELETE(m_Position[i]);
	}

	SAFE_DELETE (m_NextPosition);
#endif

#if defined USE_RASTERTEK_TEXT_FONT //27
	SAFE_SHUTDOWN(AppTextClass);
#endif

#if defined (SCENE_COMPOUND)//TUTORIAL_CHAP >= 55 && 
	//for (UINT i = 0; i < N_COMPOUNDS; i++) {
	//	SAFE_SHUTDOWN(compound[i].objModel);

	#if TUTORIAL_CHAP >= 70 && defined (SOUND3D)
		SAFE_SHUTDOWN(compound[i].sound);
	#endif//
	//}
#endif//

}

//-----------------------------------------------------------------------------------------
void ApplicationClass::WOMA_APPLICATION_Shutdown()
//-----------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown()\n");

#if defined USE_ASTRO_CLASS
	SAFE_DELETE(initWorld);
#endif

#if CORE_ENGINE_LEVEL >= 9 && defined USE_METARCLASS
	SAFE_DELETE(weatherClass);
	SAFE_DELETE(metarClass);
#endif

#if defined USE_LIGHT_RAY
#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		SAFE_SHUTDOWN_MODELDX(m_lightRayModel);
#endif

#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		SAFE_SHUTDOWN_MODELGL3(m_lightRayModel);
#endif
#endif
}

//-----------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_InitGUI()
//-----------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()\n");

	#ifdef LINUX_PLATFORM
		// Check User Settings: Auto-Detect Resolution?
		//----------------------------------------------------------------------------
		printf(" Check User Settings: Auto-Detect Resolution");

		// Set the Window width and height, for requested screen mode:
		//----------------------------------------------------------------------------
		if (SystemHandle->AppSettings->FULL_SCREEN) {
			SystemHandle->AppSettings->WINDOW_WIDTH = SystemHandle->AppSettings->SCREEN_RESOLUTION_WIDTH;
			SystemHandle->AppSettings->WINDOW_HEIGHT = SystemHandle->AppSettings->SCREEN_RESOLUTION_HEIGHT;
		}
	#endif

	//Used by windows: CreateFont()
	#if CORE_ENGINE_LEVEL >= 4 && defined WINDOWS_PLATFORM
	if (SystemHandle->LandScape)
	{
		SystemHandle->m_scaleX = MIN(1, SystemHandle->AppSettings->WINDOW_WIDTH / 1920.0f);
		SystemHandle->m_scaleY = MIN(1, SystemHandle->AppSettings->WINDOW_HEIGHT / 1080.0f);
	}
	else
	{
		SystemHandle->m_scaleX = MIN(1, SystemHandle->AppSettings->WINDOW_HEIGHT / 1080.0f);
		SystemHandle->m_scaleY = MIN(1, SystemHandle->AppSettings->WINDOW_WIDTH / 1920.0f);
	}

	if (SystemHandle->m_scaleY > 0.9f)
		SystemHandle->m_scaleY = 1;

	SystemHandle->fontSizeX = MIN(MIN_WIN32_FONT_X, 48 * SystemHandle->m_scaleX);	//To use on win32 window not DX
	SystemHandle->fontSizeY = MIN(MIN_WIN32_FONT_Y, 40 * SystemHandle->m_scaleY); //To use on win32 window not DX
	#endif

#if defined USE_ASTRO_CLASS	//LVL:7   11 = Screen Text: 1
	if (!initWorld)
	{
		initWorld = NEW InitWorld;
		WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()-initWorld created\n");
	}

	if (astroClass) {
		InitializeCelestialInfoScreen(10, 10);
		WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()-InitializeCelestialInfoScreen created\n");
	}
#endif

#if defined USE_METARCLASS && defined USE_ASTRO_CLASS //LVL:9
	if (astroClass)
	{
		if (!InitializeWeatherInfoScreen(10, 10))
		{
			WomaMessageBox(TEXT("InitializeWeatherInfoScreen"), TEXT("Error: "));
		}
		WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitGUI()-InitializeWeatherInfoScreen created\n");
	}
#endif

	return true;
}

//-------------------------------------------------------------------------------------------
bool ApplicationClass::Start()
//-------------------------------------------------------------------------------------------
{

#if defined USE_RASTERTEK_TEXT_FONT
	AppTextClass = NULL;
#endif

	return true;
}




#pragma warning(pop)

