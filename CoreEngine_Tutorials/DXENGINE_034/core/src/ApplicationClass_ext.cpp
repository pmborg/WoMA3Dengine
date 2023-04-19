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
#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

#include "winSystemClass.h"

#if (defined DX_ENGINE)
#include "DXmodelClass.h"
#endif
#include "GLmodelClass.h"

#include "dxWinSystemClass.h"

#include "SceneManager.h"
#include "xml_loader.h"

namespace WOMA
{
SceneManager* sceneManager;
}

UINT RENDER_PAGE=0;
bool FORCE_RENDER_ALL = false;
UINT g_NetID = 0;

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

	m_SkyModel = NULL;

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
		SAFE_SHUTDOWN_MODELGL3(m_1stSquar3DColorModel);			//DEMO1:
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
		SAFE_SHUTDOWN_MODELDX(m_1stSquar3DColorModel);
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

#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELDX(m_cube1Model);
		SAFE_SHUTDOWN_MODELDX(m_cube2Model);
		SAFE_SHUTDOWN_MODELDX(m_cube3Model);
		SAFE_SHUTDOWN_MODELDX(m_SphereModel1);
		SAFE_SHUTDOWN_MODELDX(m_SphereModel2);
		SAFE_SHUTDOWN_MODELDX(m_SkyModel);
	}
#endif

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_cube1Model);
		SAFE_SHUTDOWN_MODELGL3(m_cube2Model);
		SAFE_SHUTDOWN_MODELGL3(m_cube3Model);
		SAFE_SHUTDOWN_MODELGL3(m_SphereModel1);
		SAFE_SHUTDOWN_MODELGL3(m_SphereModel2);
		SAFE_SHUTDOWN_MODELGL3(m_SkyModel);
	}

	//2D:
	DEMO_WOMA_APPLICATION_Shutdown2D();


	for (int i = 0; i < objModel.size(); i++) {
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) {
			SAFE_SHUTDOWN_MODELGL3(objModel[i]);
		} 
		else 
		{
			SAFE_SHUTDOWN_MODELDX(objModel[i]);
		}
	}

	for (UINT i = 0; i <m_Position.size(); i++) {
		SAFE_DELETE(m_Position[i]);
	}

	SAFE_DELETE (m_NextPosition);

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
bool ApplicationClass::Initialize(WomaDriverClass* Driver)
//-------------------------------------------------------------------------------------------
{
	ASSERT(Driver);

	m_NextPosition = NEW PositionClass(/*ID*/-1);

	initText(Driver);

	//(Light, LigthRay...) and SCENE MANAGER: QuadTree object Loader/Render
	IF_NOT_RETURN_FALSE(WOMA_APPLICATION_Initialize3D(Driver));	

	IF_NOT_RETURN_FALSE(DEMO_WOMA_APPLICATION_Initialize3D(Driver));//SKY + DEMO APPLICATION:21..26 + 28..29

	Driver->Finalize();

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

	SceneManager* sceneManager = SceneManager::GetInstance();

	//LIGHT ////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_Light = NEW LightClass;	// Create the light object
	IF_NOT_THROW_EXCEPTION(m_Light);
	if (RENDER_PAGE >= 34) {
		m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1);
		m_Light->SetDiffuseColor(1, 1, 1, 1.0f);
		m_Light->SetDirection(-0.535041273, -0.634380400, 0.557935774);
	}
	else {
		if (RENDER_PAGE < 30) {
			m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1);
			m_Light->SetDiffuseColor(0.3f, 0.3f, 0.3f, 1.0f);
			m_Light->SetDirection(0, -1.0f, 0);
		}
		else {
			if (RENDER_PAGE >= 32) {
				m_Light->SetAmbientColor(0.4f, 0.4f, 0.4f, 1);
				m_Light->SetDiffuseColor(0.4f, 0.4f, 0.4f, 1.0f);
				m_Light->SetDirection(0, -1.0f, 0);
			}
			else {
				m_Light->SetAmbientColor(0.9f, 0.9f, 0.9f, 1);
				m_Light->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
				m_Light->SetDirection(0, -1.0f, 0);
			}
		}
	}

	//SKY ////////////////////////////////////////////////////////////////////////////////////////////////////////
	initLightRay(Driver);

	//OBJECTS ////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load 3D Objects: convert XML "objects" -- Load OBJ or M3D --> VirtualModelClass:
	for (int i = 0; i < SystemHandle->xml_loader.theWorld.size(); i++)
	{
		objModel.push_back(NULL);
		CREATE_MODEL_IF_NOT_EXCEPTION(objModel[i], I_AM_3D, SystemHandle->xml_loader.theWorld[i].WOMA_object.castShadows, SystemHandle->xml_loader.theWorld[i].WOMA_object.renderShadows);

		objModel[i]->m_ObjId = i; //SYNC-ID: objModel[i] with: xml_loader.theWorld[i]

		if (!(objModel[i]->LoadModel(SystemHandle->xml_loader.theWorld[i].filename, Driver, (SHADER_TYPE)SystemHandle->xml_loader.theWorld[i].shader,
			SystemHandle->xml_loader.theWorld[i].filename, SystemHandle->xml_loader.theWorld[i].WOMA_object.castShadows,
			SystemHandle->xml_loader.theWorld[i].WOMA_object.renderShadows, SystemHandle->xml_loader.theWorld[i].WOMA_object.instances)))
		{
			WOMA::WomaMessageBox(SystemHandle->xml_loader.theWorld[i].filename, TEXT("Error Loading: "), FALSE); return false;
		}
		sceneManager->addModel(sceneManager->RootNode, objModel[i]);	// Add node to nodesList: RootNode
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

#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		SAFE_SHUTDOWN_MODELDX(m_lightRayModel);
#endif

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		SAFE_SHUTDOWN_MODELGL3(m_lightRayModel);
}

#pragma warning(pop)