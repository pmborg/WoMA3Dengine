// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Control all Application main cycles.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "platform.h"
#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

#include "OSengine.h"
#include "main.h"
#include "ApplicationClass.h"
#include "OSmain_dir.h"
#include "mem_leak.h"

#include "vkClass.h"

#if defined WINDOWS_PLATFORM
#include "DXmodelClass.h"
#endif

#if (defined OPENGL3 || defined OPENGL40) 
#include "GLmodelClass.h"
#endif

#if defined VULKAN 
#include "vkModelClass.h"
#include "vkRenderTextureClass.h"
#endif


#if defined DX_ENGINE
DXcameraClass* DX_Camera = NULL;
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
DXcameraClass* DX_CameraSKY = NULL;
#endif
#endif

#if defined USE_SCENE_MANAGER
#include "SceneManager.h"
#include "xml_loader.h"

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
extern RApplicationClass* r_Application;
#endif
#if defined LINUX_PLATFORM
extern MyWin Win;
#endif

#if TUTORIAL_CHAP >= 60 // BILLBOARD: EQ: DX_ENGINE_LEVEL = 70
#include "BillClass.h"
#endif

#if defined MAIN_RENDER_CURVED_REAL_SKY_PLANE
#include "realSkyPlaneClass.h"
extern RealSkyPlaneClass realSkyPlaneClass;
#endif

#if defined (SCENE_COMPOUND)
#include "compound.h"
void CompoundReadFunction(WomaDriverClass* Driver);
#endif

#include <inttypes.h>

#include "commonfunctionsInterfaces.h"




void ApplicationClass::PositionUpdate(int playerId, float positionX, float positionZ)
{
	// Set the size of the mini-map: HARDCODED!
	float m_mapSizeX = 150.0f;
	float m_mapSizeY = 150.0f;

	float percentX, percentY;
	float percentMapX, percentMapY;

#ifdef MAIN_RENDER_TERRAIN
	#define m_terrainWidth	loadedTerrain[2]->m_terrainWidth
	#define m_terrainHeight loadedTerrain[2]->m_terrainHeight
#else
	#define m_terrainWidth	512
	#define m_terrainHeight 512
#endif

	#define g_ScreenWidth	WOMA::AppSettings->WINDOW_WIDTH
	#define g_ScreenHeight	WOMA::AppSettings->WINDOW_HEIGHT

	// Ensure the point does not leave the minimap borders even if the camera goes past the terrain borders.
	if (positionX <= 0) {
		positionX = 0;
		return;
	}

	if (positionZ <= 0) {
		positionZ = 0;
		return;
	}

	if (positionX > m_terrainWidth)
		positionX = (float)m_terrainWidth;

	if (positionZ > m_terrainHeight)
		positionZ = (float)m_terrainHeight;

	// Calculate the position of the camera on the minimap in terms of percentage.
	percentX = 0.5f;
	percentY = 0.5f;

	// MINI-MAP:
	//m_mapLocationX = WOMA::AppSettings->WINDOW_WIDTH - (200 * rescale);
	//m_mapLocationY = 100 * rescale;
	m_mapLocationX = WOMA::AppSettings->WINDOW_WIDTH - 200;
	m_mapLocationY = 75;

	// Determine the pixel location of the point on the Mini-Map:
	m_pointLocationX[playerId] = m_mapLocationX + (int)(percentX * m_mapSizeX); // Dont need rescale. always 150x150
	m_pointLocationY[playerId] = m_mapLocationY + (int)(percentY * m_mapSizeY);

	// Subtract one from the location to center the point on the mini-map according to the 3x3 point pixel image size.
	m_pointLocationX[playerId] -= 1;
	m_pointLocationY[playerId] -= 1;

	// MAIN-MAP:
	float m_positionX = sort_cameraX;
	float m_positionZ = sort_cameraZ;

	int Qx = (int)(m_positionX / ((m_terrainWidth ) / 2));
	int Qz = (int)(m_positionZ / ((m_terrainHeight ) / 2));
	percentMapX = (positionX - Qx * (m_terrainWidth) / 2) / (m_terrainWidth) * 2;
	percentMapY = 1.0f - ((positionZ - Qz * ((m_terrainHeight) / 2)) / (m_terrainHeight) * 2); //Note: *2 because we are viewing a 256 of a big 512 map!

	// Determine the pixel location of the point on the Main-Map:
	m_pointMapLocationX[playerId] = (int)((g_ScreenWidth - scaleX * 1000/*m_MapBitmap->m_bitmapWidth*/) / 2 + (int)(percentMapX * scaleX * 1000/*m_mainMapSizeX*/));   // Re-Scale Map.
	m_pointMapLocationY[playerId] = (int)((g_ScreenHeight - scaleY * 1000/*m_MapBitmap->m_bitmapHeight*/) / 2 + (int)(percentMapY * scaleY * 1000/*m_mainMapSizeY*/));
    if (WOMA::AppSettings->FULL_SCREEN) 
    {
        //m_pointMapLocationX[playerId] -= 20;
        m_pointMapLocationY[playerId] -= 30;
    }
    else
    {
        m_pointMapLocationX[playerId] += 20;
        m_pointMapLocationY[playerId] += 30;
    }

	#undef m_terrainWidth	
	#undef m_terrainHeight 
	#undef g_ScreenWidth	
	#undef g_ScreenHeight	
}

namespace WOMA
{
SceneManager* sceneManager=NULL;
}
#endif


#if defined USE_DIRECT_INPUT
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


	// ---------------------------------------------------------------------
	// private:

#if defined INTRO_DEMO								// VIDEO+INTRO+DEMO START
	#if defined WINDOWS_PLATFORM
		RENDER_PAGE = INTRO_DEMO_INITIAL_PAGE;		// default is: 10
		SpriteScreenToShow = SPRITE_SCREEN_TO_SHOW;	// default is: -5;
	#else
		RENDER_PAGE = 20;	// FOR DEBUG ONLY| INTRO START ON:
		SpriteScreenToShow = 0;
	#endif
#else
		RENDER_PAGE = DX_ENGINE_LEVEL;

	womalog("RENDER_PAGE: %d\n", RENDER_PAGE);
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

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_REALEARTH_TERRAIN //24
	//autoGenUnderWaterTerrain		= NULL;
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	m_TextureWithShadows		= NULL;
#endif

#if defined SCENE_TERRAIN_QUAD_TREE
	TerrainQuadtree = NULL;
#endif

	app_Light = NULL;

#if defined MAIN_RENDER_LIGHT_RAY
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

#if defined USE_DX_DRIVER_FONT
	SAFE_DELETE(m_FontV2Shader);
#endif

	//#if defined USE_MINIMAP_REDENRING_THREAD
	StopRenderThreads();
	//#endif

	SAFE_SHUTDOWN_MODEL(m_1stSquare3DColorModel);
	SAFE_SHUTDOWN_MODEL(m_1stTriangle3DColorModel);

	SAFE_SHUTDOWN_MODEL(m_2nd3DModel);

#if (!defined NO_SCENE_IMAGE_LOAD) || defined INTRO_DEMO
	SAFE_SHUTDOWN_MODEL(m_bmp3DModel);
	SAFE_SHUTDOWN_MODEL(m_jpg3DModel);
	SAFE_SHUTDOWN_MODEL(m_png3DModel);
	SAFE_SHUTDOWN_MODEL(m_tif3DModel);
	SAFE_SHUTDOWN_MODEL(m_dds3DModel);

#if defined SUPPORT_TGA
	SAFE_SHUTDOWN_MODEL(m_tga3DModel);
#endif

#endif

	SAFE_SHUTDOWN_MODEL(m_1stTriangleTextureVertexModel);


	SAFE_SHUTDOWN_MODEL(m_3th3DModel1);
	SAFE_SHUTDOWN_MODEL(m_3th3DModel2);

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	SAFE_SHUTDOWN_MODEL(m_SkyModel);
#endif


#if defined USE_CUBE // Cubes
	SAFE_SHUTDOWN_MODEL(m_cube1Model);
	SAFE_SHUTDOWN_MODEL(m_cube2Model);
	SAFE_SHUTDOWN_MODEL(m_cube3Model);
#endif


#if defined USE_SPHERE && DX_ENGINE_LEVEL >= 26
	SAFE_SHUTDOWN_MODEL(m_SphereModel1);
	SAFE_SHUTDOWN_MODEL(m_SphereModel2);
#endif

	//2D:
#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
	DEMO_WOMA_APPLICATION_Shutdown2D();
#endif

	for (int i = 0; i < MAX_TERRAINS; i++)
	{
		SAFE_SHUTDOWN_MODEL(m_TerrainModel[i]);
	}


	for (int i = 0; i < objModel.size(); i++) {
	//#if (defined OPENGL3 || defined OPENGL4)
	//	if (WOMA::AppSettings->DRIVER == DRIVER_GL3 || WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	//	{
	//		SAFE_SHUTDOWN_MODELGL3(objModel[i]);
	//	} 
	//	else 
	//#endif
	//	{
			if (objModel[i] && !SystemHandle->xml_loader.theWorldXML[i].CLONE)
			{
				SAFE_SHUTDOWN_MODEL(objModel[i]);
			}
			else
			{
				if (objModel[i]) { 
					objModel[i] = NULL; 
				}
			}
	//	}
	}

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
#ifdef VULKAN
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		VKrenderTextureClass* tex = (VKrenderTextureClass*)m_TextureWithShadows;
		SAFE_SHUTDOWN(tex);
	}
#endif
	//TODO: GL3+!
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		//SAFE_DELETE((GLrenderTextureClass*)m_TextureWithShadows);
	}
#ifdef DX_ENGINE
	if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
	{
		DXrendertextureclass* tex = (DXrendertextureclass*)m_TextureWithShadows;
		SAFE_SHUTDOWN(tex);
	}
#endif
	m_TextureWithShadows = NULL;
#endif

#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
#if defined VULKAN
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		VKrenderTextureClass* texture = (VKrenderTextureClass*)m_RenderMapTexture;
		SAFE_SHUTDOWN(texture);
		m_RenderMapTexture = NULL;
	} 
#endif
#if defined DX_ENGINE
	if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
	{
		DXrendertextureclass* texture = (DXrendertextureclass*)m_RenderMapTexture;
		SAFE_SHUTDOWN(texture);
		m_RenderMapTexture = NULL;
	}
#endif
#endif

#if DX_ENGINE_LEVEL >= 63 && defined USE_MINI_MAP
#if defined VULKAN
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		VKrenderTextureClass* texture = (VKrenderTextureClass*)m_MiniMapBitmapTexture;
		SAFE_SHUTDOWN(texture);
		m_MiniMapBitmapTexture = NULL;

		texture = (VKrenderTextureClass*)m_BorderTexture;
		SAFE_SHUTDOWN(texture);
		m_BorderTexture = NULL;

		texture = (VKrenderTextureClass*)m_PointTexture;
		SAFE_SHUTDOWN(texture);
		m_PointTexture = NULL;
	}
#endif
#if defined DX_ENGINE
	if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
	{
		DXrendertextureclass* texture = (DXrendertextureclass*)m_MiniMapBitmapTexture;
		SAFE_SHUTDOWN(texture);
		m_MiniMapBitmapTexture = NULL;

		texture = (DXrendertextureclass*)m_BorderTexture;
		SAFE_SHUTDOWN(texture);
		m_BorderTexture = NULL;

		texture = (DXrendertextureclass*)m_PointTexture;
		SAFE_SHUTDOWN(texture);
		m_PointTexture = NULL;
	}
#endif
#endif

	for (size_t i = 0; i < objModel_minimap.size(); i++)
	{
		SAFE_SHUTDOWN_MODEL(objModel_minimap[i]);
	}

#if defined SCENE_TERRAIN_QUAD_TREE
	SAFE_SHUTDOWN(TerrainQuadtree);
#endif

	for (UINT id = 0; id < MAX_TERRAINS; id++)
		SAFE_DELETE(loadedTerrain[id]);

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_REALEARTH_TERRAIN
	//SAFE_DELETE (autoGenUnderWaterTerrain);
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN
	//SAFE_DELETE (mainTerrain);
#endif

#if defined USE_DIRECT_INPUT
	for (UINT i = 0; i <m_Position.size(); i++) {
		SAFE_DELETE(m_Position[i]);
	}

	SAFE_DELETE (m_NextPosition);
#endif

	SAFE_DELETE (app_Light);

#if defined USE_RASTERTEK_TEXT_FONT //27
	SAFE_SHUTDOWN(AppTextClass);
#endif

#if TUTORIAL_CHAP >= 60 && defined SCENE_BILLBOARDS
	SAFE_SHUTDOWN(m_billTreeClass);
#endif

#if defined MAIN_RENDER_CURVED_REAL_SKY_PLANE
	realSkyPlaneClass.Shutdown();
#endif

#if defined (SCENE_COMPOUND)//TUTORIAL_CHAP >= 55 && 
	//for (UINT i = 0; i < N_COMPOUNDS; i++) {
	//	SAFE_SHUTDOWN(compound[i].objModel);

	#if TUTORIAL_CHAP >= 70 && defined SOUND3D
		SAFE_SHUTDOWN(compound[i].sound);
	#endif
	//}
#endif
        Shutdownauxcommonfunctions();
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

#if defined MAIN_RENDER_LIGHT_RAY

#if defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_DX11 ||
		WOMA::AppSettings->DRIVER == DRIVER_DX9 ||
		WOMA::AppSettings->DRIVER == DRIVER_DX12)
	{
		SAFE_SHUTDOWN_MODELDX(m_lightRayModel);
	}
#endif

#if defined OPENGL3 || defined OPENGL4
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_lightRayModel);
	}
#endif

#if defined VULKAN
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		SAFE_SHUTDOWN_MODELVK(m_lightRayModel);
	}
#endif

#endif

#if defined SCENE_MOVIN_SKIN
    SAFE_DELETE(demo_mesh1.m_character);
    SAFE_DELETE(demo_mesh1.m_characterPos);
#endif
}

#if defined SCENE_MOVIN_SKIN
MESH demo_mesh1;
#endif

#if defined USE_3RD_PERSON_CAMERA// || defined USE_DEMO99
MAIN_3RD_PERSON_CAMERA main3rdPcamera;
#endif

//-----------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_InitGUI()
//-----------------------------------------------------------------------------------------
{
	if (WOMA::game_state >= GAME_STOP)
		return true;

	womalog("WOMA_APPLICATION_InitGUI()\n");

	#ifdef LINUX_PLATFORM
		// Check User Settings: Auto-Detect Resolution?
		//----------------------------------------------------------------------------
		printf(" Check User Settings: Auto-Detect Resolution");

		// Set the Window width and height, for requested screen mode:
		//----------------------------------------------------------------------------
		if (WOMA::AppSettings->FULL_SCREEN) {
			WOMA::AppSettings->WINDOW_WIDTH = WOMA::AppSettings->SCREEN_RESOLUTION_WIDTH;
			WOMA::AppSettings->WINDOW_HEIGHT = WOMA::AppSettings->SCREEN_RESOLUTION_HEIGHT;
		}
	#endif

	//Used by windows: CreateFont()
	#if CORE_ENGINE_LEVEL >= 4 && defined WINDOWS_PLATFORM
	if (SystemHandle->LandScape)
	{
		SystemHandle->m_scaleX = MAX(1, WOMA::AppSettings->WINDOW_WIDTH  / 1920.0f);
		SystemHandle->m_scaleY = MAX(1, WOMA::AppSettings->WINDOW_HEIGHT / 1080.0f);
	}
	else
	{
		SystemHandle->m_scaleX = MAX(1, WOMA::AppSettings->WINDOW_HEIGHT / 1080.0f);
		SystemHandle->m_scaleY = MAX(1, WOMA::AppSettings->WINDOW_WIDTH / 1920.0f);
	}
	SystemHandle->fontSizeX = MIN(MIN_WIN32_FONT_X, 48 * SystemHandle->m_scaleX); //To use on win32 window not DX
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
	if (IndexSquarList2.size() == 0)
	{
		IndexSquarList2.push_back(0); IndexSquarList2.push_back(1); IndexSquarList2.push_back(2);
		IndexSquarList2.push_back(0); IndexSquarList2.push_back(2); IndexSquarList2.push_back(3);
	}
	if (IndexTriangleList.size() == 0)
	{
		IndexTriangleList.push_back(0); IndexTriangleList.push_back(1); IndexTriangleList.push_back(2);
	}

	for (int i = 0; i < MAX_TERRAINS; i++)
		m_TerrainModel[i] = NULL;

#if defined USE_RASTERTEK_TEXT_FONT
	AppTextClass = NULL;
#endif

	return true;
}


//	-------------------------------------------------------------------------------------------
#if defined USE_DIRECT_INPUT
void ApplicationClass::SetPlayerPosition(UINT netID)
{
	if (HowManyPlayers == 0) 
	{
		// "m_player"
		PlayerClass* player = NEW PlayerClass (netID);
		IF_NOT_THROW_EXCEPTION (player);
		SystemHandle->m_player.push_back(player);
		player = NULL;
	}
	
	PositionClass* position = NEW PositionClass (netID);
	IF_NOT_THROW_EXCEPTION (position);
	m_Position.push_back(position);

    if (netID != g_NetID) {
       m_Position[netID]->SetPosition(0, 0, 0);		// All other players, at same Position when offline (Have to be before CH90)
       m_Position[netID]->SetRotation(0, 0, 0);
    } else {
		// This is for "US" (the position of our Player)
       m_Position[netID]->SetPosition(WOMA::AppSettings->INIT_CAMX, WOMA::AppSettings->INIT_CAMY, WOMA::AppSettings->INIT_CAMZ);
       m_Position[netID]->SetRotation(WOMA::AppSettings->INIT_ROTX, WOMA::AppSettings->INIT_ROTY, WOMA::AppSettings->INIT_ROTZ);
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
bool ApplicationClass::Initialize(void* pContext, WomaDriverClass* Driver)
//-------------------------------------------------------------------------------------------
{
#if !defined ANDROID_PLATFORM
	ASSERT_DEBUG(Driver);
#endif

	//IMGUI:

#if defined USE_DIRECT_INPUT
	m_NextPosition = NEW PositionClass(/*ID*/-1);
	if (WOMA::game_state == GAME_STOP) return false;
#endif

	//LVL29 - 1st RELEASE DEMO:
#if defined INTRO_DEMO
	initIntroCreditsDemo(pContext);
#endif

#if DX_ENGINE_LEVEL >= 86 && !defined USE_INTRO_VIDEO_DEMO
	Startauxcommonfunctions(WOMA::logManager, DX_ENGINE_LEVEL, SystemHandle->m_hWnd); //ignore result at this level
#endif

//########################################### 3D: STUFF ###########################################
	// (app_Light && xml_loader.theWorldXML) and SCENE MANAGER: QuadTree object Loader/Render
	womalog("[ASSETS] WOMA_APPLICATION_Initialize3D()\n");
	IF_NOT_RETURN_FALSE(WOMA_APPLICATION_Initialize3D(pContext, Driver, DX_ENGINE_LEVEL)); //Load All: 3D + Billboards

//########################################### 2D: STUFF ###########################################
#if DX_ENGINE_LEVEL >= 24 && defined USE_VIEW2D_SPRITES
	womalog("[ASSETS] DEMO_WOMA_APPLICATION_InitializeSprites2D()\n");
	DEMO_WOMA_APPLICATION_InitializeSprites2D(pContext);			//2D:TITLE + FONTS + 2D:MINI-MAP + 2D:MAIN-MAP
#endif

//##################################### 82: START MESH THREADS ####################################
#if DX_ENGINE_LEVEL >= 79 && defined MAIN_RENDER_ASSIMP
	StartMeshDemo((ID3D11DeviceContext*)pContext);
#endif

	womalog("[ASSETS] Driver->FinalizeInitialization()\n");
	Driver->FinalizeInitialization();

	if (WOMA::game_state == GAME_STOP) 
		return false;

	return true; //go-go-go Start Rendering! :)
}
#endif

//----------------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(void* /*unused*/, UINT monIdx, WomaDriverClass* driver) // RENDER A FULL FRAME!
//----------------------------------------------------------------------------------------------------
{
	void* mainCtx = NULL;

#if defined VULKAN 
	vkClass* vkDriver = ((vkClass*)m_Driver);
#endif

	womalogATfirstframe("firstframe: ApplicationClass::RenderScene()\n-------------------------------\n");

	SystemHandle->TotalVertexCounter = 0;	// Reset total vertex counter

#if defined VULKAN
	//
	// shadowCtx: Pre-req for: PASS: 2 (VULKAN only)
	//
	void* preRenderCtx = (WOMA::AppSettings->DRIVER == DRIVER_VULKAN) ? vkDriver->BeginOffscreenScene(monIdx) : NULL;	// Begin Offscreen Scene (ShadowMap) for this monitor (START: Render to Texture)
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		SystemHandle->CalculateCameraViewAndFrustum(preRenderCtx);	// PRE-REQ[1] AppPreRender: 21: CalculateViewMatrix (to render) and 30: Frustum
		CalculateLightRayVertex(SunDistance);						// PRE-REQ[2] AppPreRender: Calculate Light Source Position
		SortOutWhatNeedToBeRendered(preRenderCtx, m_Driver);		// PRE-REQ[3] AppPreRender: 30+: SORT OBJECTS: Sort out what need to be rendered (filter only objects in front of camera)
		AppPreRender(monIdx, m_Driver, 1.0f, preRenderCtx);			// PRE-RENDER: Render ShadowMap(s) & MiniMap & MAP

		vkDriver->EndOffscreenScene(monIdx);						// End Offscreen Scene (ShadowMap) for this monitor (END: Render to Texture)
	}
#endif
	//
	// mainCtx
	//
	m_Driver->BeginScene(monIdx);								// RESET FRAME: ClearRenderTargetView + ClearDepthBuffer

	mainCtx = getvoidcontext();									// GET FRAME CONTEXT: For this monitor (monIdx)

	//
	// 2D: - need review for Multi-Monitor and Vulkan
	// 
	if (m_Driver->Pending2DResize)
	{
#if defined USE_VIEW2D_SPRITES
		DEMO_WOMA_APPLICATION_Shutdown2D();						//24: shutdown 2D sprites (if any) after resize
#endif

#if defined VULKAN && DX_ENGINE_LEVEL >= 24 && defined USE_VIEW2D_SPRITES
		if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
			mainCtx = vkDriver->GetLoadContext();				// Get LOAD context
#endif

		DEMO_WOMA_APPLICATION_InitializeSprites2D(mainCtx);		// 24: reload 2D sprites (if any) after resize

#if defined VULKAN && DX_ENGINE_LEVEL >= 24 && defined USE_VIEW2D_SPRITES
		if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
			vkDriver->FinalizeInitialization();								// Finalize LOAD context
#endif

		m_Driver->Pending2DResize = false;
	}

	//
	// DX & OPENGL (NON VULKAN) after level 36
	//
#if DX_ENGINE_LEVEL >= 36 && (defined VULKAN && defined activate_render_in_shadowMap && defined USE_SHADOW_MAP)
	if (WOMA::AppSettings->DRIVER != DRIVER_VULKAN)
#endif
	{
		SystemHandle->CalculateCameraViewAndFrustum(mainCtx);								// 21: CalculateViewMatrix (to render) and 30: Frustum

		CalculateLightRayVertex(SunDistance);

		SortOutWhatNeedToBeRendered(mainCtx, m_Driver);		// 30+: SORT OBJECTS: Sort out what need to be rendered (filter only objects in front of camera)
	}


	//
	// PASS: 0 & 1
	//

	// UPDATE DYN. LIGHT RAY:
	// --------------------------------------------------------------------------------------------
#if defined MAIN_RENDER_LIGHT_RAY
	m_lightRayModel->UpdateLightRayVertices(mainCtx, m_LightVertexVector);	// Update LightRay vertex(s)
#endif

		dayLightFade = 1;

#if DX_ENGINE_LEVEL >= 36 && (defined USE_MINIMAP_REDENRING_THREAD || defined activate_render_in_shadowMap || defined USE_MAIN_MAP)
	if (WOMA::AppSettings->DRIVER != DRIVER_VULKAN)				// On Vulkan this was already done!
		AppPreRender(monIdx, driver, dayLightFade, mainCtx);	// lvl36: Render shadow sequence work.
		// lvl91: Launch shadow & mini-map async work, (do not wait for render on level>=91) Start aux threads on this frame
#endif

	AppRender(monIdx, RENDER_PAGE, dayLightFade, mainCtx);		// [2] 3D Render "main scene" while workers run in parallel!

	AppPosRender(monIdx, RENDER_PAGE, dayLightFade, mainCtx);	// [3] 2D: TITLE:24 + FONTS:27 + DEMOINTROSPRITES:29 + DXFONTS:26 + MINI-MAP:63 & MAIN-MAP:63 + 2D:SPRITES:24

	// [2.5] Wait here (after main scene) before using shadows/mini-map results
	IF_RENDER_PAGE(RENDER_PAGE >= 91)
		WaitForPreRenderTasks(mainCtx);

#if !defined ANDROID_PLATFORM
	(g_contextDriver ? g_contextDriver : m_Driver)->EndScene(monIdx);	// SHOW FRAME: For this monitor (monIdx)
#else
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		bool swapResult = eglSwapBuffers(engine.display, engine.surface);
		assert(swapResult == EGL_TRUE);
	}
	else if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		m_Driver->EndScene(0);
	}
#endif
#if defined _DEBUG || defined LINUX_PLATFORM //|| defined ANDROID_PLATFORM
	womalogATfirstframe(TEXT("TotalVertexCounter: %d\n"), SystemHandle->TotalVertexCounter);
#endif
}

#pragma warning(pop)

