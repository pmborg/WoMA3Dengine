// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: initApplication_Basics.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------

#include "main.h"
#include "ApplicationClass.h"
#include "OSengine.h"
#include "Math3D.h"
#include "mem_leak.h"
#include <cinttypes>

#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

#ifdef OPENGL3
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
void ApplicationClass::initColorDemo()
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

		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stSquare3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		ASSERT (m_1stSquare3DColorModel->LoadColor(TEXT("m_1stSquare3DColorModel"), m_Driver, SHADER_COLOR, &SquareColorVertexVector, &IndexSquarList));	// LOAD the Model //UINT IndexSquarList[] = {0,1,2, 0,3,1};
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
		ASSERT(m_1stTriangle3DColorModel->LoadColor(TEXT("m_1stTriangle3DColorModel"), m_Driver, SHADER_COLOR, &TriangleColorVertexVector, &IndexTriangleList));	// LOAD the Model //UINT IndexTriangleList[] = {0,1,2};
	}
}
//END: initColorDemo
#endif

#if defined USE_CUBE
bool ApplicationClass::initCubes3D()
{
	//DEMO1:
	CCube cube = CCube(0,0,0);
	{
		CREATE_MODEL_IF_NOT_EXCEPTION(m_cube1Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		ASSERT(m_cube1Model->LoadColor(TEXT("m_cube1Model"), m_Driver, SHADER_COLOR, &cube.VertexCubeColorModel, &cube.IndexCubeList));
	}

	//DEMO2:
	{
		initLoadTexture3D(m_cube2Model, LEVEL22_DEMO_TEXTURE, cube.VertexCubeTextureModel, cube.IndexCubeList, SHADER_TEXTURE);
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
bool ApplicationClass::DEMO_WOMA_APPLICATION_InitializeSprites2D()
// --------------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("DEMO_WOMA_APPLICATION_InitializeSprites2D()\n");

#if defined USE_TITLE_BANNER
	initStatic2D();			//TITLE + MAP + MINI-MAP
#endif

	return true;
}
#endif

#if DX_ENGINE_LEVEL >= 22 || defined USE_VIEW2D_SPRITES
// --------------------------------------------------------------------------------------------
void ApplicationClass::DEMO_WOMA_APPLICATION_Shutdown2D()
// --------------------------------------------------------------------------------------------
{
#if defined USE_DX10DRIVER_FONTS
	if ((DirectX::DX11Class*)/*SystemHandle->*/driverList.size() > 0)
		((DirectX::DX11Class*)/*SystemHandle->*/driverList[SystemHandle->AppSettings->DRIVER])->Shutdown2D();
#endif

	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown2D()\n");

	#if (defined DX_ENGINE)
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
		#if defined USE_TITLE_BANNER // TITLE-SHUTDOWN
			SAFE_SHUTDOWN_MODELDX(m_titleModel);
		#endif
		#if defined USE_IMGUI
			SAFE_SHUTDOWN_MODELDX(m_iconSettings);
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
		#if defined USE_IMGUI
			SAFE_SHUTDOWN_MODELGL3(m_iconSettings);
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

bool ApplicationClass::initText()
{
	_tprintf("[%d]: initText()\n", gettid());

	AppTextClass = NEW DirectX::ApplicationTextClass();
	IF_NOT_THROW_EXCEPTION(AppTextClass);

	switch (SystemHandle->AppSettings->DRIVER)
	{
#ifdef OPENGL3
	case DRIVER_GL3:
		ASSERT(AppTextClass->Initialize((GLopenGLclass*)m_Driver));
		break;
#endif

#if defined DX11 || defined DX9
	case DRIVER_DX9:
	case DRIVER_DX11:
		ASSERT(AppTextClass->Initialize((DirectX::DX11Class*)m_Driver));
		break;
#endif

#ifdef DX12
	case DRIVER_DX12:
		ASSERT(AppTextClass->Initialize((DirectX::DX12Class*)m_Driver));
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
void ApplicationClass::initIntroDemo()
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
		#if defined DX9sdk
		case DRIVER_DX9:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader9->isFontShader = false;
			break;
		#endif
		#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX9:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader11->isFontShader = false;
			break;
		#endif

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
bool ApplicationClass::WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver)	//WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Initialize3D()\n");
// --------------------------------------------------------------------------------------------
{
	_tprintf("----------------------------------------------------------------------------------------\n");
	_tprintf("[%d]: WOMA_APPLICATION_Initialize3D()\n", gettid());

	//ASTRO ////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined USE_ASTRO_CLASS
	SystemHandle->m_Application->initWorld->Calculate();
#endif

#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION //#if ENGINE_LEVEL >= 33
	SystemHandle->m_Application->Calc3DSunMoonPosition();
	//if (WOMA::game_state == GAME_STOP) return false;
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// INIT ALL BASIC DEMOS:
	//-----------------------------------------------------------------------------------------------------------------
#if defined SCENE_COLOR
	initColorDemo();

#endif

	//Sphere+SKY:



	//-----------------------------------------------------------------------------------------------------------------
	// Create "model OBJECTS" from loaded "XML OBJECTS" in file WORLD.XML
	//-----------------------------------------------------------------------------------------------------------------

	//ASTRO ///////////////////////////////////////////////////////////////////////////////////////////////////////////

	//SHADOWMAP //////////////////////////////////////////////////////////////////////////////////////////////////////

	//TERRAIN ////////////////////////////////////////////////////////////////////////////////////////////////////////
//0
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN		// UNDER WATER: Terrain
	loadedTerrain[0] = NEW CTerrain(TERRAIN);
	loadedTerrain[0]->initUnderWaterDemo(0);			//UNDERWATER	(populate: modelVertexVector) 2022:LEVEL_ENGINE: 25
#endif

	//1 WATER TERRAIN MESH: 6 vertex + 6 index
#if defined SCENE_WATER_TERRAIN
	loadedTerrain[1] = NEW CTerrain(TERRAIN);
	loadedTerrain[1]->initTerrainWaterMeshDemo(1);		//WATER			(populate: modelVertexVector)
#endif

	//2 MAIN TERRAIN MESH: 4 vertex + 6 index
#if defined SCENE_MAIN_TOPO_TERRAIN	&& !defined USE_TERRAIN_ALFA_MAP
	loadedTerrain[2] = NEW CTerrain(TERRAIN);
	loadedTerrain[2]->initMainTopoTerrainDemo(2);		//TERRAIN		(populate: modelVertexVector)
#endif

	//3 TERRAIN:6 vertex + 6 index: TO BE USED BY COLLISION TERRAIN
#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX && defined SCENE_TERRAIN_COLLISION
	loadedTerrain[3] = NEW CTerrain(TERRAIN);
	loadedTerrain[3]->initMainTopoTerrainDemo(3);
#endif

#if defined SCENE_MAIN_TERRAIN									//66: MAIN: TERRAIN-V3
	initMainTerrainDemo(0);
#endif

	//NETWORK ////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined SAVEW3D
	WomaMessageBox(TEXT("Conversion from OBJ to W3D, ended."), TEXT("SAVEW3D"));
	WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
	return false;
#endif

#if defined (CHECK_COMPOUND_COLISION) && defined (SCENE_COMPOUND) //TUTORIAL_CHAP >= 55 && 
	for (UINT i = 0; i < N_COMPOUNDS; i++) {
		compoundTreeLoadingOrder[i].compoundTreeId = i;
		compoundTreeLoadingOrder[i].order = 0;
	}

	// [26] Finally, launch Load Compound/OBJ Thread:
	//-----------------------------------------------------------------------------------------	
#if TUTORIAL_CHAP < 95
	CompoundReadFunction(Driver);
#else
// Create a thread to load our compounds:
	threadCompoundLoaderAlive = true;
	threadCompoundLoaderHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CompoundReadFunction, (void*)this, 0, &threadCompoundLoaderId);
	if (!threadCompoundLoaderHandle) { return false; }
	if (!SetThreadPriority(threadCompoundLoaderHandle, THREAD_PRIORITY_IDLE/*THREAD_PRIORITY_LOWEST*//*THREAD_PRIORITY_BELOW_NORMAL*/)) { return false; }
#endif//
#endif

	return true;
}

#pragma warning(pop)