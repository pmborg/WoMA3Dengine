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
// PURPOSE: Control all Application main cycles.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#include "OSengine.h"
#include "main.h"
#include "ApplicationClass.h"
#include "OSmain_dir.h"
#include "mem_leak.h"
#if TUTORIAL_CHAP >= 60 // BILLBOARD
#include "BillClass.h"				//[ch60]
#endif

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
extern RApplicationClass* r_Application;
#endif
#if defined LINUX_PLATFORM
extern MyWin Win;
#endif
#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

#if (defined DX_ENGINE)
#include "DXmodelClass.h"
#endif

#if (defined OPENGL3 || defined OPENGL40) 
#include "GLmodelClass.h"
#endif

#if defined USE_SCENE_MANAGER
#include "SceneManager.h"
#include "xml_loader.h"

namespace WOMA
{
SceneManager* sceneManager=NULL;
}
#endif

#if defined (SCENE_COMPOUND) //TUTORIAL_CHAP >= 55
#include "compound.h"
void CompoundReadFunction(WomaDriverClass* Driver);
#endif

#include <inttypes.h>

#if defined INTRO_DEMO
bool FORCE_RENDER_ALL = true;
#else
bool FORCE_RENDER_ALL = false;
#endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
UINT g_NetID = 0;
#endif

#if defined INTRO_DEMO
int SpriteScreenToShow;
std::vector<VirtualModelClass*> m_screenShots;
float fadeIntro=1;
#endif

ApplicationClass::ApplicationClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

	// ---------------------------------------------------------------------
	// private:

#if defined INTRO_DEMO	// VIDEO+INTRO+DEMO
	#if defined WINDOWS_PLATFORM
		RENDER_PAGE = 10;	// INTRO_DEMO START!!!
		SpriteScreenToShow = -5;
	#else
		RENDER_PAGE = 20;	// FOR DEBUG ONLY| INTRO START ON:
		SpriteScreenToShow = 0;
	#endif
#else
		RENDER_PAGE = DX_ENGINE_LEVEL;
  #if _DEBUG
	womalog("RENDER_PAGE: %d\n", RENDER_PAGE);
  #endif
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

#if defined USE_3RD_PERSON_CAMERA
    m_camYaw = 0.0f;
    m_camPitch = 0.0f;
#endif

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN //24
	//autoGenUnderWaterTerrain		= NULL;
#endif

#if defined SCENE_TERRAIN_QUAD_TREE
	TerrainQuadtree = NULL;
#endif

	m_Light = NULL;

#if defined USE_LIGHT_RAY
	m_lightRayModel = NULL;
#endif

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

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
	m_SkyModel = NULL;
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
	womalog("ApplicationClass::Shutdown()\n");

	//3D:

#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_1stSquare3DColorModel);			//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_1stTriangle3DColorModel);		//DEMO2:
#if !defined NO_SCENE_IMAGE_LOAD && DX_ENGINE_LEVEL >= 22
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
#endif
	}
	else
#endif
	{
#if defined DX_ENGINE
		SAFE_SHUTDOWN_MODELDX(m_1stSquare3DColorModel);
		SAFE_SHUTDOWN_MODELDX(m_1stTriangle3DColorModel);
			SAFE_SHUTDOWN_MODELDX(m_2nd3DModel);
		#if !defined NO_SCENE_IMAGE_LOAD
			SAFE_SHUTDOWN_MODELDX(m_bmp3DModel);	//DEMO1:
			SAFE_SHUTDOWN_MODELDX(m_jpg3DModel);	//DEMO1:
			SAFE_SHUTDOWN_MODELDX(m_png3DModel);	//DEMO1:
			SAFE_SHUTDOWN_MODELDX(m_tif3DModel);	//DEMO1:
			SAFE_SHUTDOWN_MODELDX(m_dds3DModel);	//DEMO1:
		#if defined SUPPORT_TGA
			SAFE_SHUTDOWN_MODELDX(m_tga3DModel);	//DEMO1:
		#endif
		#endif
		SAFE_SHUTDOWN_MODELDX(m_1stTriangleTextureVertexModel);
#endif
	}


#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELDX(m_3th3DModel1);
		SAFE_SHUTDOWN_MODELDX(m_3th3DModel2);
	}
#endif

#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_3th3DModel1);
		SAFE_SHUTDOWN_MODELGL3(m_3th3DModel2);
	}
#endif

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
	#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined DX_ENGINE
		SAFE_SHUTDOWN_MODELDX(m_SkyModel);
	#endif
	} else {
	#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && (defined OPENGL3 || defined OPENGL4)
	SAFE_SHUTDOWN_MODELGL3(m_SkyModel);
	#endif
	}
#endif

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

#if defined USE_SPHERE && DX_ENGINE_LEVEL >= 26
#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
	#if defined USE_SPHERE && DX_ENGINE_LEVEL >= 26
		SAFE_SHUTDOWN_MODELDX(m_SphereModel1);
		SAFE_SHUTDOWN_MODELDX(m_SphereModel2);
	#endif
	}
#endif

#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
	#if defined USE_SPHERE && DX_ENGINE_LEVEL >= 26
		SAFE_SHUTDOWN_MODELGL3(m_SphereModel1);
		SAFE_SHUTDOWN_MODELGL3(m_SphereModel2);
	#endif

	}
#endif
#endif

#if defined USE_DX_DRIVER_FONT
	SAFE_SHUTDOWN(m_FontV2Shader);
#endif

	//2D:
#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
	DEMO_WOMA_APPLICATION_Shutdown2D();
#endif


#if defined SCENE_TERRAIN_QUAD_TREE
	SAFE_SHUTDOWN(TerrainQuadtree);
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

	SAFE_DELETE (m_Light);

#if defined USE_RASTERTEK_TEXT_FONT //27
	SAFE_SHUTDOWN(AppTextClass);
#endif

#if TUTORIAL_CHAP >= 60 && defined SCENE_BILLBOARDS
	SAFE_SHUTDOWN(m_billTreeClass);
#endif

#if defined (SCENE_COMPOUND)//TUTORIAL_CHAP >= 55 && 
	//for (UINT i = 0; i < N_COMPOUNDS; i++) {
	//	SAFE_SHUTDOWN(compound[i].objModel);

	#if TUTORIAL_CHAP >= 70 && defined SOUND3D
		SAFE_SHUTDOWN(compound[i].sound);
	#endif
	//}
#endif
}

//-----------------------------------------------------------------------------------------
void ApplicationClass::WOMA_APPLICATION_Shutdown()
//-----------------------------------------------------------------------------------------
{
	womalog("WOMA_APPLICATION_Shutdown()\n");

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
	womalog("WOMA_APPLICATION_InitGUI()\n");

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
		womalog("WOMA_APPLICATION_InitGUI()-initWorld created\n");
	}

	if (astroClass) {
		InitializeCelestialInfoScreen(10, 10);
		womalog("WOMA_APPLICATION_InitGUI()-InitializeCelestialInfoScreen created\n");
	}
#endif

#if defined USE_METARCLASS && defined USE_ASTRO_CLASS //LVL:9
	if (astroClass)
	{
		if (!InitializeWeatherInfoScreen(10, 10))
		{
			WomaMessageBox(TEXT("InitializeWeatherInfoScreen"), TEXT("Error: "));
		}
		womalog("WOMA_APPLICATION_InitGUI()-InitializeWeatherInfoScreen created\n");
	}
#endif

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

#if defined USE_RASTERTEK_TEXT_FONT
	AppTextClass = NULL;
#endif

	return true;
}


//	-------------------------------------------------------------------------------------------
#if defined USE_DIRECT_INPUT
void ApplicationClass::SetPlayerPosition(UINT netID)
{
	//OutputDebugString("SetPlayerPosition(%d)\n", netID);
	if (HowManyPlayers == 0) 
	{
		// "m_player"
		PlayerClass* player = NEW PlayerClass (netID);
		IF_NOT_THROW_EXCEPTION (player);
		SystemHandle->m_player.push_back(player);
		player = NULL;
		//OutputDebugString("SystemHandle->m_player.size()=%d\n", SystemHandle->m_player.size());
	}
	
	// "m_Position"
	PositionClass* position = NEW PositionClass (netID);
	IF_NOT_THROW_EXCEPTION (position);
	m_Position.push_back(position);
	//OutputDebugString("m_Position.size()=%d\n", m_Position.size());

    if (netID != g_NetID) {
       m_Position[netID]->SetPosition(0, 0, 0);		// All other players, at same Position when offline (Have to be before CH90)
       m_Position[netID]->SetRotation(0, 0, 0);
    } else {
		// This is for "US" (the position of our Player)
       m_Position[netID]->SetPosition(SystemHandle->AppSettings->INIT_CAMX, SystemHandle->AppSettings->INIT_CAMY, SystemHandle->AppSettings->INIT_CAMZ);
       m_Position[netID]->SetRotation(SystemHandle->AppSettings->INIT_ROTX, SystemHandle->AppSettings->INIT_ROTY, SystemHandle->AppSettings->INIT_ROTZ);
    }
}
#endif



#if defined CHECK_OBJ_COLISION //CHECK_COMPOUND_COLISION
//-----------------------------------------------------------------------------   
// Name: CompoundSortCB()   
// Desc: Callback function for sorting Compounds in front-to-back order   
//-----------------------------------------------------------------------------   
int __cdecl CompoundSortCB(const VOID* arg1, const VOID* arg2)
{
	compoundTreeLoadOrder* p1 = (compoundTreeLoadOrder*)arg1;
	compoundTreeLoadOrder* p2 = (compoundTreeLoadOrder*)arg2;

	if (p1->order > p2->order)
		return +1;

	return -1;
}

#endif

#if CORE_ENGINE_LEVEL >= 10 && !defined NewWomaEngine
//-------------------------------------------------------------------------------------------
bool ApplicationClass::Initialize(WomaDriverClass* Driver)
//-------------------------------------------------------------------------------------------
{
#if !defined ANDROID_PLATFORM
    ASSERT_DEBUG(Driver);
#endif

	//imgGui:

#if defined USE_DIRECT_INPUT
	m_NextPosition = NEW PositionClass(/*ID*/-1);
	if (WOMA::game_state == GAME_STOP) return false;
#endif

	//DEMO:
#if defined INTRO_DEMO
	initIntroDemo();
#endif
//########################################### 3D: STUFF ###########################################
	// (m_Light && xml_loader.theWorld) and SCENE MANAGER: QuadTree object Loader/Render
	IF_NOT_RETURN_FALSE(WOMA_APPLICATION_Initialize3D(Driver));	
//########################################### 2D: STUFF ###########################################
 
#if DX_ENGINE_LEVEL >= 24 && defined USE_VIEW2D_SPRITES
	DEMO_WOMA_APPLICATION_InitializeSprites2D();				//2D:TITLE + 2D:MAP + 2D:MINI-MAP
#endif
	// 2D-FONTS: (Windows)
#if defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONT //27
	initText();
#endif
	// 2D-FONTS: (Android / Linux)
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
	r_Application = new RApplicationClass;
	_tprintf("r_Application->Initialize(m_videoDisplay, m_hwnd, screenWidth, screenHeight)\n");
	IF_NOT_RETURN_FALSE(r_Application->Initialize(SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT));
#endif

#if CORE_ENGINE_LEVEL >= 10 && !defined ANDROID_PLATFORM
	Driver->Finalize(); //Mostly for DX12
#endif

	if (WOMA::game_state == GAME_STOP) return false;

	return true;
}

#endif

#pragma warning(pop)

