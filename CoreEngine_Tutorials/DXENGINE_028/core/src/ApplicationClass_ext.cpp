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

#include "DXmodelClass.h"
#include "GLmodelClass.h"

#include "DemoApplicationClass.h"

#include "dxWinSystemClass.h"

ApplicationClass::ApplicationClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567891;

	// ---------------------------------------------------------------------
	// private:

		RENDER_PAGE = DX_ENGINE_LEVEL;
  #if _DEBUG
	WOMA_LOGManager_DebugMSG("RENDER_PAGE: %d\n", RENDER_PAGE);
  #endif

	g_NetID = NULL;
	m_NextPosition = NULL;

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

	for (UINT i = 0; i <m_Position.size(); i++) {
		SAFE_DELETE(m_Position[i]);
	}

	SAFE_DELETE (m_NextPosition);

	for (UINT i = 0; i < HowManyPlayers; i++) 
	{
		SAFE_DELETE(m_Position[i]);
	}

	SAFE_DELETE (m_Light);

	SAFE_SHUTDOWN(AppTextClass);
}

//-----------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_InitGUI()
//-----------------------------------------------------------------------------------------
{
	SystemHandle->m_scaleX = MIN(1, SystemHandle->AppSettings->WINDOW_WIDTH / 1920.0f);
	SystemHandle->m_scaleY = MIN(1, SystemHandle->AppSettings->WINDOW_HEIGHT / 1080.0f);
	if (SystemHandle->m_scaleY > 0.9f)
		SystemHandle->m_scaleY = 1;

	SystemHandle->fontSizeX = MIN(25, 48 * SystemHandle->m_scaleX);	//To use on win32 window not DX
	SystemHandle->fontSizeY = MIN(25, 40 * SystemHandle->m_scaleY); //To use on win32 window not DX

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

	AppTextClass = NULL;

	return true;
}

//-------------------------------------------------------------------------------------------
bool ApplicationClass::Initialize(/*WomaDriverClass*/ void* Driver)
//-------------------------------------------------------------------------------------------
{
	ASSERT(Driver);
	SystemHandle->demoApplicationClass = NEW DemoApplicationClass;
	initText(SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);
	m_NextPosition = NEW PositionClass(/*ID*/-1);

	// USER LOAD main 3D objects!
	IF_NOT_RETURN_FALSE(WOMA_APPLICATION_Initialize3D(SystemHandle->m_Driver));	

	//CALL DEMO APPLICATION:
	IF_NOT_RETURN_FALSE(SystemHandle->demoApplicationClass->WOMA_APPLICATION_Initialize3D(SystemHandle->m_Driver));

	SystemHandle->m_Driver->Finalize();

	return true;
}


//	-------------------------------------------------------------------------------------------
void ApplicationClass::SetPlayerPosition(UINT netID)
{
	if (HowManyPlayers == 0) 
	{
		// "m_player"
		PlayerClass* player = NEW PlayerClass (netID);
		IF_NOT_THROW_EXCEPTION (player);
		DXsystemHandle->m_player.push_back(player);
		player = NULL;
	}
	// "m_Position"
	PositionClass* position = NEW PositionClass (netID);
	IF_NOT_THROW_EXCEPTION (position);
	m_Position.push_back(position);

    if (netID != g_NetID) {
       m_Position[netID]->SetPosition(0, 0, 0);		// All other players, at same Position when offline (Have to be before CH90)
       m_Position[netID]->SetRotation(0, 0, 0);
    } else {
		// This is for "US" (the position of our Player)
       m_Position[netID]->SetPosition(SystemHandle->AppSettings->INIT_CAMX, SystemHandle->AppSettings->INIT_CAMY, SystemHandle->AppSettings->INIT_CAMZ);
       m_Position[netID]->SetRotation(SystemHandle->AppSettings->INIT_ROTX, SystemHandle->AppSettings->INIT_ROTY, SystemHandle->AppSettings->INIT_ROTZ);
    }
}



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
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1);
	m_Light->SetDiffuseColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_Light->SetDirection(0, -1.0f, 0);

	//SKY ////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (RENDER_PAGE >= 23  && RENDER_PAGE < 27)
	{
		initLightRay(Driver);
	}

	//SHADOWMAP //////////////////////////////////////////////////////////////////////////////////////////////////////

	//TERRAIN ////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

//-----------------------------------------------------------------------------------------
void ApplicationClass::WOMA_APPLICATION_Shutdown()
//-----------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown()\n");

	SAFE_DELETE(initWorld);

	SAFE_DELETE(weatherClass);
	SAFE_DELETE(metarClass);

	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		SAFE_SHUTDOWN_MODELDX(m_lightRayModel);

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		SAFE_SHUTDOWN_MODELGL3(m_lightRayModel);
}
