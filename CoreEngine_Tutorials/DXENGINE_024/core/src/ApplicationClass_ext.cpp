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

#if (defined DX_ENGINE)
#include "DXmodelClass.h"
#endif
#include "GLmodelClass.h"

#include "dxWinSystemClass.h"

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

	m_Light = NULL;

	// TERRAIN

	m_lightRayModel = NULL;



	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	m_1stTriangle3DColorModel = NULL;					// Model2

	m_2nd3DModel = NULL;						// Model

	m_bmp3DModel = NULL;						// Model
	m_jpg3DModel = NULL;						// Model
	m_png3DModel = NULL;						// Model
	m_tif3DModel = NULL;						// Model
	m_dds3DModel = NULL;						// Model
#if defined SUPPORT_TGA
	m_tga3DModel = NULL;						// Model
#endif

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

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_1stSquare3DColorModel);			//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_1stTriangle3DColorModel);		//DEMO2:
		SAFE_SHUTDOWN_MODELGL3(m_2nd3DModel);
		SAFE_SHUTDOWN_MODELGL3(m_bmp3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_jpg3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_png3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_tif3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_dds3DModel);		//DEMO1:
#if defined SUPPORT_TGA
		SAFE_SHUTDOWN_MODELGL3(m_tga3DModel);		//DEMO1:
#endif

		SAFE_SHUTDOWN_MODELGL3(m_1stTriangleTextureVertexModel);
	}
	else
	{
#if (defined DX_ENGINE)
		SAFE_SHUTDOWN_MODELDX(m_1stSquare3DColorModel);
		SAFE_SHUTDOWN_MODELDX(m_1stTriangle3DColorModel);
		SAFE_SHUTDOWN_MODELDX(m_2nd3DModel);
		SAFE_SHUTDOWN_MODELDX(m_bmp3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_jpg3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_png3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_tif3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_dds3DModel);	//DEMO1:
#if defined SUPPORT_TGA
		SAFE_SHUTDOWN_MODELDX(m_tga3DModel);	//DEMO1:
#endif

		SAFE_SHUTDOWN_MODELDX(m_1stTriangleTextureVertexModel);
		//#endif
#endif
	}


#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELDX(m_3th3DModel1);
		SAFE_SHUTDOWN_MODELDX(m_3th3DModel2);
	}
#endif

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_3th3DModel1);
		SAFE_SHUTDOWN_MODELGL3(m_3th3DModel2);
	}

	//2D:
	DEMO_WOMA_APPLICATION_Shutdown2D();


	SAFE_DELETE (m_Light);

}

//-----------------------------------------------------------------------------------------
void ApplicationClass::WOMA_APPLICATION_Shutdown()
//-----------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown()\n");

	SAFE_DELETE(initWorld);

	SAFE_DELETE(weatherClass);
	SAFE_DELETE(metarClass);

#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		SAFE_SHUTDOWN_MODELDX(m_lightRayModel);
#endif

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		SAFE_SHUTDOWN_MODELGL3(m_lightRayModel);
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

	// Create a basic list of 6 Index(s), for a simple Square: ==> UINT indexList[] = {0,1,2, 0,3,1};
	if (IndexSquarList.size() == 0)
	{
		IndexSquarList.push_back(0); IndexSquarList.push_back(1); IndexSquarList.push_back(2);
		IndexSquarList.push_back(0); IndexSquarList.push_back(3); IndexSquarList.push_back(1);
	}
	if (IndexTriangleList.size() == 0)
	{
		IndexTriangleList.push_back(0); IndexTriangleList.push_back(1); IndexTriangleList.push_back(2);
	}

	return true;
}

//-------------------------------------------------------------------------------------------
bool ApplicationClass::Initialize(WomaDriverClass* Driver)
//-------------------------------------------------------------------------------------------
{
	ASSERT(Driver);

	//(Light, LigthRay...) and SCENE MANAGER: QuadTree object Loader/Render
	IF_NOT_RETURN_FALSE(WOMA_APPLICATION_Initialize3D(Driver));	
	if (WOMA::game_state == GAME_STOP) return false;



	IF_NOT_RETURN_FALSE(DEMO_WOMA_APPLICATION_Initialize3D(Driver));//SKY + DEMO APPLICATION:21..26 + 28..29
	if (WOMA::game_state == GAME_STOP) return false;

	Driver->Finalize();

	return true;
}


//	-------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------
// INIT/LOAD 3D Objects
// --------------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver)
// --------------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Initialize3D()\n");

	//LIGHT ////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_Light = NEW LightClass;	// Create the light object
	IF_NOT_THROW_EXCEPTION(m_Light);
	if (RENDER_PAGE >= 34) {
		m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1);
		m_Light->SetDiffuseColor(1, 1, 1, 1.0f);
		m_Light->SetDirection(-0.535041273f, -0.634380400f, 0.557935774f);
	} else {
		if (RENDER_PAGE < 30) {
			m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1);
			m_Light->SetDiffuseColor(0.3f, 0.3f, 0.3f, 1.0f);
			m_Light->SetDirection(0, -1.0f, 0);
		} else {
			if (RENDER_PAGE >= 32) {
				m_Light->SetAmbientColor(0.4f, 0.4f, 0.4f, 1);
				m_Light->SetDiffuseColor(0.4f, 0.4f, 0.4f, 1.0f);
				m_Light->SetDirection(0, -1.0f, 0);
			} else {
				m_Light->SetAmbientColor(0.9f, 0.9f, 0.9f, 1);
				m_Light->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
				m_Light->SetDirection(0, -1.0f, 0);
			}
		}
	}
	if (WOMA::game_state == GAME_STOP) return false;

	//SKY ////////////////////////////////////////////////////////////////////////////////////////////////////////
	initLightRay(Driver);
	if (WOMA::game_state == GAME_STOP) return false;

	//SHADOWMAP //////////////////////////////////////////////////////////////////////////////////////////////////////

	//TERRAIN ////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined SAVEM3D
	WOMA::WomaMessageBox(TEXT("Conversion from OBJ to M3D, ended."), TEXT("SAVEM3D"));
	Publish_Quit_Message();
	return false;
#endif

	return true;
}


#pragma warning(pop)