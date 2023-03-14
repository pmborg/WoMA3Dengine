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

#include "winSystemClass.h"

ApplicationClass::ApplicationClass()
{
	CLASSLOADER();

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

	//m_Driver = NULL;
	
	ClearColor[0] = 0.5f;
	ClearColor[1] = 0.6f;
	ClearColor[2] = 0.8f;
	ClearColor[3] = 1.0f;



#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN //24
	autoGenUnderWaterTerrain		= NULL;
#endif

#if defined SCENE_TERRAIN_QUAD_TREE //67
	TerrainQuadtree = NULL;
#endif

	// TERRAIN

//#if defined USE_TITLE_BANNER //26
//	m_titleModel = NULL;
//#endif

	Start();
}

ApplicationClass::~ApplicationClass() {CLASSDELETE();}

//	-------------------------------------------------------------------------------------------
void ApplicationClass::Shutdown()
{
	WOMA_LOGManager_DebugMSG ("ApplicationClass::Shutdown()\n");

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN
	SAFE_DELETE (autoGenUnderWaterTerrain);
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN
	SAFE_DELETE (mainTopoTerrain);
#endif
#if defined SCENE_MAIN_TERRAIN
	//SAFE_DELETE(mainTerrain);
#endif


}


//-----------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_InitGUI()
//-----------------------------------------------------------------------------------------
{
	SystemHandle->m_scaleX = MIN(1, SystemHandle->AppSettings->WINDOW_WIDTH / 1920.0f);
	SystemHandle->m_scaleY = MIN(1, SystemHandle->AppSettings->WINDOW_HEIGHT / 1080.0f);

	SystemHandle->fontSizeX = MIN(25, 48 * SystemHandle->m_scaleX);	//To use on win32 window not DX
	SystemHandle->fontSizeY = MIN(25, 40 * SystemHandle->m_scaleY); //To use on win32 window not DX

	//if (SystemHandle->m_Driver != NULL && initWorld == NULL)
	{
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
bool ApplicationClass::Initialize(/*WomaDriverClass*/ void* Driver)
//-------------------------------------------------------------------------------------------
{
	ASSERT(Driver);



	SystemHandle->m_Driver->Finalize();

	return true;
}




//	-------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------
void ApplicationClass::WOMA_APPLICATION_Shutdown()
//-----------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown()\n");

	SAFE_DELETE(initWorld);

	SAFE_DELETE(weatherClass);
	SAFE_DELETE(metarClass);

}
