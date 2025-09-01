// --------------------------------------------------------------------------------------------
// Filename: initApplication_Basics.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "main.h"
#include "ApplicationClass.h"
#include "OSengine.h"
#include "Math3D.h"
#include "mem_leak.h"
#include "log.h"
#include <cinttypes>
#include <string.h>
#include "fileLoader.h"

#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'
#pragma warning(disable : 4244) // warning C4244: '=': conversion from 'int' to 'float', possible

#if (defined OPENGL3 || defined OPENGL40) 
	#include "GLopenGLclass.h"
	#include "GLmodelClass.h"
#endif

#if defined DX_ENGINE
	#include "DXmodelClass.h"
#endif

#if defined USE_CUBE // Cubes
	#include "WomaCube.h"
#endif

#if defined SCENE_COLOR
// ----------------------------------------------------------------------------
void ApplicationClass::initColorDemo(void* pContext)
// ----------------------------------------------------------------------------
{
	//if (RENDER_PAGE == 21 || RENDER_PAGE == 22 || RENDER_PAGE == 23 || RENDER_PAGE == 24 || FORCE_RENDER_ALL)
	{
		//DEMO-1:
		// Step 1: Prepare Vertex(s)
		float X = 4, Y = 4, Z = 0;
		ModelColorVertexType vertex = {0};
		CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(SquareColorVertexVector, X,Y,Z);			// Step 1: Populate SquareColorVertexVector with, all vertices positions: X, Y, Z

		float color;
		float Start = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1));	// Float between 0..1
		float End = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1));		// Float between 0..1

		for (UINT i = 0; i < SquareColorVertexVector.size(); i++)	// Step 2: Add a color to all vertices
		{															
			if ( i == 0 || i == 3 /*|| i == 4*/ )					//  |\            \|
				color = End;										//  1  v0		  1  v0 v3
			else                                                    // -1  v2 v1	 -1     v1
				color = Start;										//  x: -1  1	  x: -1  1

			SquareColorVertexVector[i].r = color;// Red
			SquareColorVertexVector[i].g = color;// Green
			SquareColorVertexVector[i].b = 0.6f;	// Blue
		}

		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stSquare3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Allocate the MODEL
		ASSERT (m_1stSquare3DColorModel->LoadColor(pContext, TEXT("m_1stSquare3DColorModel"), m_Driver, SHADER_COLOR, &SquareColorVertexVector, &IndexSquarList));	// LOAD the Model //UINT IndexSquarList[] = {0,1,2, 0,3,1};
	}
	//--------------------------------------------------------------------------------------------------------------------------
	//if (RENDER_PAGE >= 21 || FORCE_RENDER_ALL)
	{
		//DEMO-2:
		// Step 1: Prepare Vertex(s)
		float X = 1, Y = 1, Z = 1;
		ModelColorVertexType vertex = { 0 };
		CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(TriangleColorVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		for (UINT i = 0; i < TriangleColorVertexVector.size(); i++)	// Step 2: Add a color to all vertices
		{
			TriangleColorVertexVector[i].r = 0.0f;
			TriangleColorVertexVector[i].b = 0.0f;
			TriangleColorVertexVector[i].g = 1.0f; 	// Green
		}
		// Step 2: Create a model: NEW GLmodelClass; || NEW DXmodelClass;
		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stTriangle3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		ASSERT(m_1stTriangle3DColorModel->LoadColor(pContext, TEXT("m_1stTriangle3DColorModel"), m_Driver, SHADER_COLOR, &TriangleColorVertexVector, &IndexTriangleList));	// LOAD the Model //UINT IndexTriangleList[] = {0,1,2};
	}
}
//END: initColorDemo
#endif

#if defined USE_TITLE_BANNER	//24 DX_ENGINE_LEVEL >= 24
void ApplicationClass::initStatic2D(void* ctx)
{
#if defined DX_ENGINE
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;
#else
	#define pContext ctx
#endif

	// Step 1: Prepare Vertex(s)
	std::vector<ModelTextureVertexType> SpriteVertexVector;				// 1 Declare: the Vector with Vertex "TYPE"
	ModelTextureVertexType vertex = {};									// 2 Use this "VERTEX" on macro
	CREATE_VERTEXVECTOR_SQUAD_MODEL(SpriteVertexVector, 0, 0, 0);		// 3 Initialize Vertex ARRAY at world center at first
	std::vector<UINT> emptyIndexList;									// Empty index list.

	//--------------------------------------------------------------------------------
	//CreateDXbuffers for 2D:
	#if defined USE_TITLE_BANNER
		// # Title #
		initModelwithTexture2D(m_titleModel, DEMO_TITLE_TEXTURE, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE);
	#endif

}
#endif

#if defined USE_CUBE
bool ApplicationClass::initCubes3D(void* pContext)
{
	//DEMO1:
	CCube cube = CCube(0,0,0);
	{
		CREATE_MODEL_IF_NOT_EXCEPTION(m_cube1Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Allocate the MODEL
		ASSERT(m_cube1Model->LoadColor(pContext, TEXT("m_cube1Model"), m_Driver, SHADER_COLOR, &cube.VertexCubeColorModel, &cube.IndexCubeList));
	}

	//DEMO2:
	{
		initLoadTexture3D(m_cube2Model, LEVEL22_DEMO_TEXTURE, cube.VertexCubeTextureModel, cube.IndexCubeList, SHADER_TEXTURE);
		#if NOTES
		//#define initLoadTexture3D(model, texture, vertexVector, IndexList, shader_type)
		//{
		//	std::vector<STRING> Textures;
		//	Textures.push_back(TEXT("engine/data/seafloor.dds"));
		//	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(m_cube2Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
		//	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION(m_cube2Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
		//	ASSERT(m_cube2Model->LoadTexture(TEXT("engine/data/seafloor.dds"), SystemHandle->m_Driver, SHADER_TEXTURE, &Textures, &cube.VertexCube2, &cube.IndexCubeList));
		//}
		#endif
	}
	//DEMO3:
	{
		initLoadTextureLight3D(m_cube3Model, LEVEL22_DEMO_TEXTURE, cube.VertexCubeTextureLightModel, cube.IndexCubeList, SHADER_TEXTURE_LIGHT);
	}

	return true;
}
#endif

#if defined USE_VIEW2D_SPRITES
// INIT/LOAD 2D (SPRITE or TEXT) Objects
// --------------------------------------------------------------------------------------------
bool ApplicationClass::DEMO_WOMA_APPLICATION_InitializeSprites2D(void* pContext)
// --------------------------------------------------------------------------------------------
{
	womalog("DEMO_WOMA_APPLICATION_InitializeSprites2D()\n");

#if defined USE_TITLE_BANNER
	initStatic2D(pContext);			//TITLE + 2D:MAP + 2D:MINI-MAP
#endif

	return true;
}
#endif

#if DX_ENGINE_LEVEL >= 22 || defined USE_VIEW2D_SPRITES
// --------------------------------------------------------------------------------------------
void ApplicationClass::DEMO_WOMA_APPLICATION_Shutdown2D()
// --------------------------------------------------------------------------------------------
{
#if defined USE_DX_DRIVER_FONT
	if ((DirectX::DX11Class*)driverList.size() > 0)
		((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])->Shutdown2D();
#endif

	womalog("WOMA_APPLICATION_Shutdown2D()\n");

	#if (defined DX_ENGINE)
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
		#if defined USE_TITLE_BANNER // TITLE-SHUTDOWN
			SAFE_SHUTDOWN_MODELDX(m_titleModel);
		#endif
		#if defined USE_MAIN_MAP
			SAFE_SHUTDOWN_MODELDX(m_mainMapFrameModel);
			SAFE_SHUTDOWN_MODELDX(m_mainMapModel);
		#endif
		#if defined USE_MINI_MAP
			SAFE_SHUTDOWN_MODELDX(m_miniMapModel);
			SAFE_SHUTDOWN_MODELDX(m_miniMapBorderModel);
			SAFE_SHUTDOWN_MODELDX(m_miniMapArrowModel);
		#endif
		}
	#endif
	#if (defined OPENGL3 || defined OPENGL4)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
		#if defined USE_TITLE_BANNER // TITLE-SHUTDOWN
			SAFE_SHUTDOWN_MODELGL3(m_titleModel);
		#endif
		#if defined USE_MAIN_MAP
			SAFE_SHUTDOWN_MODELGL3(m_mainMapFrameModel);
			SAFE_SHUTDOWN_MODELGL3(m_mainMapModel);
		#endif
		#if defined USE_MINI_MAP
			SAFE_SHUTDOWN_MODELGL3(m_miniMapModel);
			SAFE_SHUTDOWN_MODELGL3(m_miniMapBorderModel);
			SAFE_SHUTDOWN_MODELGL3(m_miniMapArrowModel);
		#endif
		}
	#endif


#if defined INTRO_DEMO //29
	for (int i = 0; i < m_screenShots.size(); i++) {
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
			#if defined DX_ENGINE
			SAFE_SHUTDOWN_MODELDX(m_screenShots[i]);
			#endif
		} 
		else
		#if (defined OPENGL3 || defined OPENGL4)
		{
			SAFE_SHUTDOWN_MODELGL3(m_screenShots[i]);
		}
		#else
		{
		}
		#endif
	}

	while (!m_screenShots.empty())
		m_screenShots.pop_back();
#endif
}
#endif

#ifdef USE_RASTERTEK_TEXT_FONT

bool ApplicationClass::initText(void* pContext)
{
	_tprintf(TEXT("[%d]: initText()\n"), gettid());

	AppTextClass = NEW DirectX::ApplicationTextClass();
	IF_NOT_THROW_EXCEPTION(AppTextClass);

	switch (SystemHandle->AppSettings->DRIVER)
	{
#if (defined OPENGL3 || defined OPENGL40) 
	case DRIVER_GL3:
		ASSERT(AppTextClass->Initialize((ID3D11DeviceContext*)pContext, (GLopenGLclass*)m_Driver));
		break;
#endif

#if defined DX11 || defined DX9
	case DRIVER_DX9:
	case DRIVER_DX11:
		ASSERT(AppTextClass->Initialize((ID3D11DeviceContext*)pContext, (DirectX::DX11Class*)m_Driver));
		break;
#endif

#ifdef DX12
	case DRIVER_DX12:
		ASSERT(AppTextClass->Initialize((ID3D11DeviceContext*)pContext, (DirectX::DX12Class*)m_Driver));
		break;
#endif

	default:
#if defined USE_WOMA_EXCEPTION
		throw woma_exception("Not Enough Memory!", __FILE__, __FUNCTION__, __LINE__);
#else
		WomaFatalException("Not Enough Memory!");
#endif
		break;
	}

	return true;
}

#endif

#ifdef INTRO_DEMO
void ApplicationClass::initIntroDemo(void* pContext)
{
	std::vector<STRING> INTRO_TEXT;

	INTRO_TEXT.push_back(DEMO1_BANNER1_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER2_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER3_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER4_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER5_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER6_TEXTURE);

	//1way
	for (int i = 0; i < INTRO_TEXT.size(); i++)
	{
		// Step 1: Prepare Vertex(s)
		std::vector<ModelTextureVertexType> SpriteVertexVector;				// 1 Declare: the Vector with Vertex "TYPE"
		ModelTextureVertexType vertex = { };								// 2 Use this "VERTEX" on macro
		CREATE_VERTEXVECTOR_SQUAD_MODEL(SpriteVertexVector, 0, 0, 0);		// 3 Initialize Vertex ARRAY at world center at first
		std::vector<UINT> emptyIndexList;
		VirtualModelClass* m_spriteModel = NULL;
		initModelwithTexture2D(m_spriteModel, (TCHAR*)INTRO_TEXT[i].c_str(), SpriteVertexVector, emptyIndexList, SHADER_TEXTURE_FONT);

		switch (SystemHandle->AppSettings->DRIVER)
		{

		#if defined DX11
		case DRIVER_DX11:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader11->isFontShader = false;
			break;
		#endif

		#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader->isFontShader = false;
			break;
		#endif

		#if (defined OPENGL3 || defined OPENGL4)
		case DRIVER_GL3:
			((GLmodelClass*)m_spriteModel)->m_Shader->isFont = false;
			break;
		#endif
		}
		
		m_screenShots.push_back(m_spriteModel);

#if defined DX12

#endif
	}

}
#endif

// ----------------------------------------------------------------------------
// WOMA_APPLICATION_FrameUpdateInstancesPositions
// ----------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// INIT/LOAD 3D Objects
// --------------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_Initialize3D(void* pContext, WomaDriverClass* Driver)
// --------------------------------------------------------------------------------------------
{
	womalogauto(TEXT("----------------------------------------------------------------------------------------\n"));
	womalogauto(TEXT("[%d]: WOMA_APPLICATION_Initialize3D()\n"), gettid());

	//-----------------------------------------------------------------------------------------------------------------
	// INIT ASTROs (Sun Moon) /////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
#if defined USE_ASTRO_CLASS
	initWorld->Calculate(); //Calculate Real position of Sun and Moon.
#endif

#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION
	Use3DSunMoonPosition();
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// INIT ALL BASIC DEMOS: //////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
#if defined SCENE_COLOR
	initColorDemo(pContext);

#endif

    //=================================================================================================================
	// INIT SKY ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //=================================================================================================================
	
//Sphere:

//Sky:

#if defined MAIN_RENDER_TERRAIN
    //=================================================================================================================
	// INIT TERRAINs //////////////////////////////////////////////////////////////////////////////////////////////////
    //=================================================================================================================
	//0
 #if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN		// UNDER WATER: Terrain
	loadedTerrain[0] = NEW CTerrain(TERRAIN);
	loadedTerrain[0]->initUnderWaterDemo(pContext, 0);			//UNDERWATER	(populate: modelVertexVector) 2022:LEVEL_ENGINE: 25
  #endif

	//1 WATER TERRAIN MESH: 6 vertex + 6 index
  #if defined SCENE_WATER_TERRAIN
	loadedTerrain[1] = NEW CTerrain(TERRAIN);
	loadedTerrain[1]->initTerrainWaterMeshDemo(pContext, 1);		//WATER			(populate: modelVertexVector)
  #endif

	//2 MAIN TERRAIN MESH: 4 vertex + 6 index
  #if defined SCENE_MAIN_TOPO_TERRAIN	&& !defined USE_TERRAIN_ALFA_MAP
	loadedTerrain[2] = NEW CTerrain(TERRAIN);
	loadedTerrain[2]->initMainTopoTerrainDemo(2, (ID3D11DeviceContext*)pContext);		//TERRAIN		(populate: modelVertexVector)
  #endif

	//3 TERRAIN:6 vertex + 6 index: TO BE USED BY COLLISION TERRAIN
  #if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX && defined SCENE_TERRAIN_COLLISION
	loadedTerrain[3] = NEW CTerrain(TERRAIN);
	loadedTerrain[3]->initMainTopoTerrainDemo(3, (ID3D11DeviceContext*)pContext);
  #endif
#endif

	//=================================================================================================================
	// Init MAIN 3D Scene       ///////////////////////////////////////////////////////////////////////////////////////
	//=================================================================================================================

	//-----------------------------------------------------------------------------------------------------------------
	// Add Instanced Billboards to World.xml
	//-----------------------------------------------------------------------------------------------------------------

    //-----------------------------------------------------------------------------------------------------------------
    // Log xml objects:

	//-----------------------------------------------------------------------------------------------------------------
	// Create Billboard populate Trees / Flowers (extra populate WORLD.XML)       /////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
#if TUTORIAL_CHAP >= 60 && defined (SCENE_MAIN_TOPO_TERRAIN) && defined (SCENE_BILLBOARDS) // BILLBOARD
	IF_NOT_RETURN_FALSE(m_billTreeClass = NEW BillClass);
	if (!m_billTreeClass->Initialize((ID3D11DeviceContext*)pContext, loadedTerrain[2]->m_terrainWidth / 2, loadedTerrain[2]->m_terrainHeight / 2, false))
	{
		WomaMessageBox(TEXT("Could not initialize the billboard Class"), TEXT("Create Billboard for Trees / Flowers"));
		return false;
	}
	womalogauto("Number of billboard objects added %d\n", SystemHandle->xml_loader.theWorldXML.size()- world_xml_objs);
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// PROGRESS BAR		///////////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
#if defined ALLOW_CBIND_PROGRESS_BAR
	// --- CREATE PROGRESS BAR:
#if defined USE_INTRO_VIDEO_DEMO
	if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
#endif
	{
		SystemHandle->hwndPrgBar = SystemHandle->WomaCreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 50, SystemHandle->AppSettings->WINDOW_HEIGHT - 100,
			SystemHandle->AppSettings->WINDOW_WIDTH - 100, 20, SystemHandle->m_hWnd, (HMENU)401, SystemHandle->m_hinstance, NULL);

		SendMessage(SystemHandle->hwndPrgBar, PBM_SETRANGE, 0, (LPARAM)MAKELPARAM(0, 100));
		SendMessage(SystemHandle->hwndPrgBar, PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
		SendMessage(SystemHandle->hwndPrgBar, PBM_SETBARCOLOR, 0, RGB(0, 0, 128));
		SendMessage(SystemHandle->hwndPrgBar, PBM_SETPOS, (WPARAM)(0), 0);

		::ShowWindow(SystemHandle->hwndPrgBar, 1);
	}

	// --- CREATE PROGRESS TEXT:
	SystemHandle->settingstext = SystemHandle->WomaCreateWindowEx(WS_EX_TRANSPARENT, TEXT("STATIC"), TEXT(""),
		WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER | SS_OWNERDRAW, 25, 25, 175, 22, SystemHandle->m_hWnd, 0, SystemHandle->m_hinstance, NULL);

#if defined USE_INTRO_VIDEO_DEMO
	if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
#endif
		::ShowWindow(SystemHandle->settingstext, 1);
#endif

	// Temporarily disable log file (on this loop) due performance:
	//-----------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------
	// [MAIN OBJ LOAD]: 3D-Load Scene: Create "model OBJECTS" from loaded "XML OBJECTS" in file WORLD.XML     /////////
	//-----------------------------------------------------------------------------------------------------------------

#if defined ALLOW_CBIND_PROGRESS_BAR
	#if defined USE_INTRO_VIDEO_DEMO
	if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
	#endif
	{
		::ShowWindow(SystemHandle->hwndPrgBar, SW_HIDE);
		::ShowWindow(SystemHandle->settingstext, SW_HIDE);
		RedrawWindow(SystemHandle->m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);	// Invoke: Window PAINT before end.
	}
#endif

	//Restore: Temp. Disable log file:
	//-----------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------
	// DEMO-29                  ///////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------
	// RENDER ASTROs //////////////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------
	// SHADOWMAP //////////////////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------

	// --------------------------------------------------------------------------------------------
	//Finally, launch dynamic Load Compound/OBJ Thread ////////////////////////////////////////////
	// --------------------------------------------------------------------------------------------
#if defined CHECK_OBJ_COLISION && defined MAIN_RENDER_MAIN_OBJ //CHECK_COMPOUND_COLISION
	for (UINT i = 0; i < WOMA::num_loading_objects; i++) {
		compoundTreeLoadingOrder[i].compoundTreeId = i;
		compoundTreeLoadingOrder[i].order = 0;
	}
#endif

#if defined SAVEW3D && DX_ENGINE_LEVEL < 89
	WomaMessageBox(TEXT("Conversion from OBJ to W3D, ended."), TEXT("SAVEW3D"));
	WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
	return false;
#endif

	// --------------------------------------------------------------------------------------------
	//INIT CLIENT/SERVER NETWORK COMS /////////////////////////////////////////////////////////////
	// --------------------------------------------------------------------------------------------

	return true;
}

#pragma warning(pop)