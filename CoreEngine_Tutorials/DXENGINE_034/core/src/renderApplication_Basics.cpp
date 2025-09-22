// --------------------------------------------------------------------------------------------
// Filename: renderApplication_Basics.cpp
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

#include "platform.h"
#pragma warning(disable : 4267) // warning C4267: 'initializing': conversion from 'size_t' to 'UINT', possible loss of data

#include "OSengine.h"
#include "OSmain_dir.h"
#include "ApplicationClass.h"
#include "fileLoader.h"
#include "mem_leak.h"

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
extern RApplicationClass* r_Application;
#endif

#if defined DX11 || defined DX9
#include "Dx11Class.h"
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif
#if (defined OPENGL3 || defined OPENGL4)
#include "womadriverclass.h"	//woma
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif

#include "SceneManager.h"
#include "DXmodelClass.h"

#if TUTORIAL_CHAP >= 60 // BILLBOARD
#include "BillClass.h"	//[ch60]
#endif

#if defined USE_DIRECT_INPUT || DX_ENGINE_LEVEL >= 63
float sort_cameraX=0, sort_cameraY=0, sort_cameraZ = 0;
#endif

void ApplicationClass::SortOutWhatNeedToBeRendered(void* pContext, WomaDriverClass* driver)
{
	totalRendered = 0;
	
	// SET A SPECIFIC CAMERA POSITION FOR BILLBOARD SORT:
#if defined USE_DIRECT_INPUT
	const float SORT_OFFSET = 5.0f; // 5 METERS BEHIND CAMERA
	sort_cameraX -= FAST_sin(SystemHandle->m_Application->m_Position[g_NetID]->m_rotationY) * SORT_OFFSET;
	sort_cameraZ -= FAST_cos(SystemHandle->m_Application->m_Position[g_NetID]->m_rotationY) * SORT_OFFSET;
#endif

	// SCENEMANAGER: PROCESS/FILTER AND CREATE LISTS/TREES OF OBJECTS TO RENDER FROM: WORLD.XML
	// --------------------------------------------------------------------------------------------
#if defined USE_SCENE_MANAGER && (defined DX_ENGINE)
	WOMA::sceneManager->visibleModelList.clear();			//Reset list of opac objects

	WOMA::sceneManager->CreateLists();						//CREATE LISTS: for all objects to render (from WORLD.XML) and more

	world_main_size = WOMA::sceneManager->visibleModelList.size();
#endif

	// RESTORE DEFAULT CAMERA POSITION:
#if defined USE_DIRECT_INPUT
	sort_cameraX = SystemHandle->m_Application->m_Position[g_NetID]->m_positionX;
	sort_cameraY = SystemHandle->m_Application->m_Position[g_NetID]->m_positionY;
	sort_cameraZ = SystemHandle->m_Application->m_Position[g_NetID]->m_positionZ;
#endif

	// Rotate Bills:

	// LIGHT RAY:
	// --------------------------------------------------------------------------------------------
#if defined USE_LIGHT_RAY
	if (RENDER_PAGE >= 23)
	{
		CalculateLightRayVertex(SunDistance);							// Calculate Light Source Position
		m_lightRayModel->UpdateDynamic(pContext, m_LightVertexVector);	// Update LightRay vertex(s)
		m_lightRayModel->Render(pContext, 0, 0, 0, NULL, NULL);			// Render LightRay
	}
#endif
}

//----------------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(UINT monitorIndex, WomaDriverClass* driver) // RENDER A FULL FRAME!
//----------------------------------------------------------------------------------------------------
{
#if _DEBUG
	SystemHandle->TotalVertexCounter = 0;
#endif
	void* mainCtx=NULL;
#if defined DX_ENGINE
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		mainCtx = ((DX11Class*)m_Driver)->GetDeviceContext();
#endif

	SortOutWhatNeedToBeRendered(mainCtx, driver);

	AppRender(monitorIndex, dayLightFade, mainCtx);				// [2] 3D Render main scene while workers run in parallel

	AppPosRender(monitorIndex, dayLightFade, mainCtx);			// [3] 2D: Render TRANSPARENT Parts of 3D OBJs(like: "Glass windows", "Billboards", etc...)

}

//
// RENDER TO TEXTURE
//
#if (defined DX_ENGINE) && (defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21 || defined USE_VIEW2D_SPRITES)
extern DXcameraClass m_CameraMINIMAP;
extern DXcameraClass m_CameraMAP;
#endif

void ApplicationClass::RenderModel(void* pContext, UINT threadID, UINT monitorIndex, WomaDriverClass* driver, UINT ID, UINT pass, XMMATRIX* m_viewMatrix, XMMATRIX* m_projectionMatrix)
{
    
    UINT modelID = ID;
	if (threadID == 0)
	{
		if (pass == PASS_OPAC)
		{
			if (WOMA::sceneManager->visibleModelList.size() == 0)
				return;
			modelID = WOMA::sceneManager->visibleModelList[ID]->m_ObjId;
		}
		else {
			ASSERT(0); //we should never get here!
		}
	}


	DXmodelClass* model = NULL;
		model = (DXmodelClass*)objModel[modelID];

	//----------------------------------------------------------------------------------------------------------------------------------
    float positionX, positionY, positionZ;
    positionX = SystemHandle->xml_loader.theWorldXML[modelID].posX;
    positionY = SystemHandle->xml_loader.theWorldXML[modelID].translateY;
    positionZ = SystemHandle->xml_loader.theWorldXML[modelID].posZ;

	// === SET AUDIO DISTANCE (IF ITS THE CASE) ===											   
    // Set the initial position of the listener to be in the middle of the scene.

	// === RESET WORLD MATRIX ===
	// ------------------------------------------------------------------------------------------------------------------------------
    if (m_Driver->RenderfirstTime || (SystemHandle->xml_loader.theWorldXML[model->m_ObjId].rotY != 0 && modelID > world_xml_objs))
        ((DXmodelClass*)model)->m_worldMatrix = XMMatrixIdentity();

	// === RESET TRANSLATION ===
	// ------------------------------------------------------------------------------------------------------------------------------
    model->translation(0, 0, 0);

	// === SET SCALE ===
	// ------------------------------------------------------------------------------------------------------------------------------
    {
    if (m_Driver->RenderfirstTime)
    {
        float scale = SystemHandle->xml_loader.theWorldXML[modelID].scale;
        float scaleY = SystemHandle->xml_loader.theWorldXML[modelID].scaleY;
        if (scaleY != 1)
            model->scale(scale, scaleY, scale);
        else
            model->scale(scale, scale, scale);
    }
    }

	// === SET ROTATION IN X AXIS: ===
	// ------------------------------------------------------------------------------------------------------------------------------
    {
        float rx = SystemHandle->xml_loader.theWorldXML[modelID].rotX;
        if (rx == -1000) {
            static float rX = 0.0f;
            rX = (float)dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
            model->rotateX(rX);
        }
        else
            if (rx)
                model->rotateX(rx);

	// === SET ROTATION IN Y AXIS: ===
	// ------------------------------------------------------------------------------------------------------------------------------					  
    float ry = 0;
        {
            ry = SystemHandle->xml_loader.theWorldXML[model->m_ObjId].rotY;
        }
        if (ry == -1000) {
            static float rY = 0.0f;
            rY = (float)dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
            model->rotateY(rY);
        }
        else
            if (ry)
                model->rotateY(ry);

	// === SET ROTATION IN Z AXIS: ===
	// ------------------------------------------------------------------------------------------------------------------------------
        float rz = SystemHandle->xml_loader.theWorldXML[model->m_ObjId].rotZ;
        if (rz == -1000) {
            static float rZ = 0.0f;
            rZ = (float)dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
            model->rotateZ(rZ);
        }
        else
            if (rz)
                model->rotateZ(rz);
    }// non-Instancing

	// === Adjust current OBJ. height in MAP for WORLD POSITION: ===
	// ------------------------------------------------------------------------------------------------------------------------------
    model->translation(positionX, positionY, positionZ);

	// === RENDER OBJ.: ===					   
	model->Render(pContext, threadID, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, pass);

	totalRendered++; //One done, next...
}

#define TERRAIN_SCALE 1

void ApplicationClass::SkyAndDemos(UINT monitorWindow, float fadeLight, void* pContext)
{
#if DX_ENGINE_LEVEL >= 10 && LEVEL <= 21
	{
#define cor driverList[SystemHandle->AppSettings->DRIVER]->driver_ClearColor
		cor[2] += ((float)dt / 10000);
		cor[2] = cor[2] + 0.001f;
		if (cor[2] >= 1)
			cor[2] = 0;
#undef cor
	}
#endif

	// RENDER: SKY Sphere:
	//----------------------------------------------------------------------------------------------------------------------
#if (defined USE_SKY_CAMERA_DOME && defined USE_SKYSPHERE) && defined MAIN_RENDER_SKY	// MAIN-RENDER: "Sky": (0.0ms)
	if (RENDER_PAGE >= 28 && m_SkyModel)
	{
		m_Driver->SetRasterizerState(pContext, CULL_NONE/*CULL_BACK*/, FILL_SOLID); // Render the Inside of Sphere
		m_SkyModel->translation(0, 0, 0);
		m_SkyModel->scale(20, 20, 20);
		m_SkyModel->RenderSky(pContext, CAMERA_SKY, 1); // Camera with fixed position: 0,0,0: (CAMERA_SKY)
	}
#endif

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending(pContext);
#endif

	if ((RENDER_PAGE >= 21) && (RENDER_PAGE < 30) || RENDER_PAGE == 36)
		DemoRender(pContext);	// ALL Demos!: page 21: / 22 / 23 / ... 49

#if defined USE_SKYSPHERE && defined USE_SUN && defined USE_MOON
	if (RENDER_PAGE >= 28)				//30: SKY
		Render_SKY_SUN_MOON(fadeLight); //34: SUN_MOON
#endif

#if defined USE_SKY2D
	if (RENDER_PAGE >= 27 && RENDER_PAGE < 30) // At: 27 | 28 | 29
		m_Sky2DModel->RenderSprite(m_Driver, (SystemHandle->AppSettings->WINDOW_WIDTH - m_Sky2DModel->SpriteTextureWidth) / 2,
			(SystemHandle->AppSettings->WINDOW_HEIGHT - m_Sky2DModel->SpriteTextureHeight) / 2);
	m_Driver->ClearDepthBuffer(); // Need to Be Right after: m_Sky2DModel->RenderSprite 
#endif
}


void ApplicationClass::WaterTerrain(UINT monitorWindow, float fadeLight, void* pContext)
{
	//----------------------------------------------------------------------------------------------------------------------
	// TERRAIN[0]: UNDER WATER
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_REALEARTH_TERRAIN || defined SCENE_MAIN_TERRAIN
#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);
#endif
	if (RENDER_PAGE == 49)
		m_TerrainModel[UNDERWATER_TERRAIN_ID]->RenderWithFade(pContext);					// New function to replace these 2 line options.
#endif

	// TERRAIN[2]: Render MAIN Terrain
	//----------------------------------------------------------------------------------------------------------------------
#if (defined SCENE_MAIN_TOPO_TERRAIN && !defined USE_TERRAIN_ALFA_MAP) && defined MAIN_RENDER_TERRAIN //MAIN-RENDER TERRAIN (0.3 ms)
	static bool fog = (RENDER_PAGE == 51 || RENDER_PAGE >= 60) ? true : false;
	if (RENDER_PAGE >= 50)
	{
		if (m_TerrainModel[MAIN_TERRAIN_ID])
			m_TerrainModel[MAIN_TERRAIN_ID]->RenderWithFade(pContext, fadeLight, fog);	    // New function to replace these 2 line options.
	}
#endif
#if defined DEBUG_COLLISION_TERRAIN //For debug collision terrain only!
	if (m_TerrainModel[DEBUG_COLLISION_TERRAIN_ID])
		m_TerrainModel[DEBUG_COLLISION_TERRAIN_ID]->RenderWithFade(fadeLight, fog);	// New function to replace these 2 line options. 
#endif

	// TRANSPARENT and SEMI-TRANSPARENT:
	// --------------------------------------------------------------------------------------------
#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(pContext);
#endif

	// TERRAIN[1]: Render Mesh for WATER:
// --------------------------------------------------------------------------------------------
}

//#############################################################################################################
// [2/3] RENDER - 3D
//#############################################################################################################
void ApplicationClass::AppRender(UINT monitorIndex, float fadeLight, void* pContext)
{
	SkyAndDemos(monitorIndex, fadeLight, pContext);

	WaterTerrain(monitorIndex, fadeLight, pContext);

	// 3D STATIC OPAC OBJECTS on WORLD.XML, that listed in: sceneManager->visibleModelList (in front of camera)
	//----------------------------------------------------------------------------------------------------------------------
#if defined USE_RASTERIZER_STATE && (defined INTRO_DEMO || defined USE_ALPHA_BLENDING)
	m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);
#endif

#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending(pContext);
#endif

	// Render TRANSPARENT Parts of 3D OBJs (like: glass window of (Space Compound), etc...) (last part)
	// --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER && defined MAIN_RENDER_MAIN_OBJ //MAIN-RENDER: MAIN OBJs. (9 ms)
	for (UINT id = 0; id < WOMA::sceneManager->visibleModelList.size(); id++)
	{
		RenderModel(pContext, 0, monitorIndex, m_Driver, id, PASS_OPAC, NULL, NULL);
		if (id == 0 && ((DXmodelClass*)objModel[id])->obj3d.hasTransparent == true)
		{
			objModel[id]->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_TRANSPARENT, NULL, NULL);
		}
	}
#endif

#if defined USE_MAP_EDITOR // MAP EDITOR: Render "Red" src/target line:
    //SRC:
    MyLightVertexVector[0].r = 1; MyLightVertexVector[0].g = 0; MyLightVertexVector[0].b = 0; MyLightVertexVector[0].a = 1;
    MyLightVertexVector[1].r = 1; MyLightVertexVector[1].g = 0; MyLightVertexVector[1].b = 0; MyLightVertexVector[1].a = 1;

    MyLightVertexVector[0].x = prwsPos.m128_f32[0];
    MyLightVertexVector[0].y = prwsPos.m128_f32[1];
    MyLightVertexVector[0].z = prwsPos.m128_f32[2];
    //DEST:
    MyLightVertexVector[1].x = prwsPos.m128_f32[0] + prwsDir.m128_f32[0] * 100;
    MyLightVertexVector[1].y = prwsPos.m128_f32[1] + prwsDir.m128_f32[1] * 100;
    MyLightVertexVector[1].z = prwsPos.m128_f32[2] + prwsDir.m128_f32[2] * 100;
    m_lightRayModel->UpdateDynamic(pContext , &MyLightVertexVector);
    m_lightRayModel->Render(pContext);
#endif

	// Render Animated meshes:
	// -----------------------
	if (ShouldDrawUI(monitorIndex)) 
	{
	}

}

//#############################################################################################################
// [3/3] POS-RENDER - 2D: Render TRANSPARENT Parts of 3D OBJs (like: "Glass windows", "Billboards", etc...)
//#############################################################################################################
void ApplicationClass::AppPosRender(UINT monitorIndex, float dayLightFade, void* pContext)
{
    //=============================================================================================================
    // LIGHT: Get fade (real Sun Position): Show Debug Info

#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);
#endif
#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(pContext);
#endif

#if (TUTORIAL_CHAP >= 60 && defined SCENE_BILLBOARDS && defined USE_SCENE_MANAGER && defined DX_ENGINE) && defined MAIN_RENDER_BILLBOARDS // MAIN-RENDER: BILLBOARD + FENCES + FIRE (11.4 ms)
        UINT obj_id;
		for (UINT tree_id = 0; tree_id < _countof(m_Trees); tree_id++)
        {
            obj_id = m_Trees[tree_id].ID + world_xml_objs;
            if (SystemHandle->xml_loader.theWorldXML[obj_id].render)								// TODO: use sceneManager
				RenderModel(pContext, 0, monitorIndex, m_Driver, obj_id, PASS_BILL, NULL, NULL);    // Render: "Billboards"
		}
#endif

	if (ShouldDrawUI(monitorIndex))
		RenderHUD_Logo(pContext);

#if (defined USE_MAIN_MAP || defined USE_MINI_MAP) && defined MAIN_RENDER_MINIMAP //MAIN-RENDER: MINI-MAP (0.4)
	if (ShouldDrawUI(monitorIndex))
		RenderMainMapMiniMap(pContext);
#endif

	// === AppTextClass-Fill: ===
#if defined USE_RASTERTEK_TEXT_FONT

	if (ShouldDrawUI(monitorIndex) && AppTextClass)
	{
#if defined EXTRA_INFO2
		AppTextClass->SetInfoA(astroClass->hour, astroClass->minute);
		AppTextClass->SetInfoB(app_Light->m_lightDirection.x, app_Light->m_lightDirection.y, app_Light->m_lightDirection.z);
#endif
		AppTextClass->SetFps(SystemHandle->fps);						// Update the FPS "Value" in the text object.

#if !defined TEXT_TEST
#if defined WINDOWS_PLATFORM && !defined WIN_XP
		AppTextClass->SetCpu(SystemHandle->m_Cpu.GetCpuPercentage());	// Update the CPU usage "Value" in the text object.
#endif
#endif

#if defined USE_DIRECT_INPUT			// we will use DX input method
#if !defined TEXT_TEST
		AppTextClass->SetCameraPosition(m_Position[g_NetID]->m_positionX,
			m_Position[g_NetID]->m_positionY,
			m_Position[g_NetID]->m_positionZ);

		AppTextClass->SetCameraRotation(m_Position[g_NetID]->m_rotationX,
			m_Position[g_NetID]->m_rotationY,
			m_Position[g_NetID]->m_rotationZ);
#endif
#else
#if !defined TEXT_TEST
#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		{

			AppTextClass->SetCameraPosition(DXsystemHandle->m_Camera->m_positionX,
				DXsystemHandle->m_Camera->m_positionY,
				DXsystemHandle->m_Camera->m_positionZ);

			AppTextClass->SetCameraRotation(DXsystemHandle->m_Camera->m_rotationX,
				DXsystemHandle->m_Camera->m_rotationY,
				DXsystemHandle->m_Camera->m_rotationZ);

		}
#endif
#if defined DX12
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			AppTextClass->SetCameraPosition(DXsystemHandle->m_Camera->m_positionX,
				DXsystemHandle->m_Camera->m_positionY,
				DXsystemHandle->m_Camera->m_positionZ);

			AppTextClass->SetCameraRotation(DXsystemHandle->m_Camera->m_rotationX,
				DXsystemHandle->m_Camera->m_rotationY,
				DXsystemHandle->m_Camera->m_rotationZ);
		}
#endif
#if (defined OPENGL3 || defined OPENGL4)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
			AppTextClass->SetCameraPosition(((GLopenGLclass*)m_Driver)->gl_Camera->m_positionX,
				((GLopenGLclass*)m_Driver)->gl_Camera->m_positionY,
				((GLopenGLclass*)m_Driver)->gl_Camera->m_positionZ);

			AppTextClass->SetCameraRotation(((GLopenGLclass*)m_Driver)->gl_Camera->m_rotationX,
				((GLopenGLclass*)m_Driver)->gl_Camera->m_rotationY,
				((GLopenGLclass*)m_Driver)->gl_Camera->m_rotationZ);
		}
#endif
#endif
#endif

#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER && defined _DEBUG && !defined TEXT_TEST
		AppTextClass->SetRenderCount(WOMA::sceneManager->quadTree.totalVertexRendered,
			SystemHandle->m_Application->totalRendered,
			(UINT)SystemHandle->xml_loader.theWorldXML.size());
#endif
#if TUTORIAL_CHAP >= 60 && !defined RELEASE // BILLBOARD
		AppTextClass->SetBillRenderCount(SystemHandle->m_Application->billboardRrenderCount, total_deltaTime);
#endif  
	}
#endif

	// RENDER RASTERTEK V1 FONT:
	// -------------------------
#if (defined USE_RASTERTEK_TEXT_FONT) && defined MAIN_RENDER_RASTERTEK_FONT //MAIN-RENDER: Raster FONT: v1 (0.4 ms)
#if !defined INTRO_DEMO //|DEMO|
	if (ShouldDrawUI(monitorIndex) && RENDER_PAGE >= 27)
		AppTextClass->Render();
#else
	if (ShouldDrawUI(monitorIndex) && RENDER_PAGE == 27)
		AppTextClass->Render();
#endif
#endif

	// RENDER RASTERTEK V2 FONT:
	// -------------------------
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
	// Do the frame processing for the application object.
	if (ShouldDrawUI(monitorIndex))
	if (!r_Application->Frame(NULL))
	{
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP;
	}
#endif

	// PRESENTATION SPRITE(s)  - On Top of 3D Rendered stuff:
	// ---------------------------------------------------------------
#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending(pContext); // Re assume default
#endif
#if defined INTRO_DEMO //RenderDemoIntroSprites
	if (ShouldDrawUI(monitorIndex))
		RenderDemoIntroSprites(pContext);
#endif

	// RENDER NATIVE TEXT:
	// -------------------
#if defined USE_DX_DRIVER_FONT && defined MAIN_RENDER_DRIVER_FONT //MAIN-RENDER: Driver Font (0.5 ms)
	if (ShouldDrawUI(monitorIndex)) {
	#if !defined INTRO_DEMO //|DEMO Force NATIVE TEXT|
	if ((RENDER_PAGE >= 22) && (m_Driver->m_sCapabilities.USE_DXDRIVER_FONTSBoolean) && (SystemHandle->AppSettings->DRIVER == DRIVER_DX11))
	#endif
	{
		if (RENDER_PAGE >= 21) {
		#if defined USE_DIRECT_INPUT
			if (RENDER_PAGE >= 49) {
				if (g_GOD_MODE)
					StringCchPrintf(DEMO_NAME_SHOW, MAX_STR_LEN, TEXT("%s GOD-MODE: %s"), DEMO_NAME[RENDER_PAGE - 21], TEXT("ON"));		//Note: Have to be "wchar"
				else
					StringCchPrintf(DEMO_NAME_SHOW, MAX_STR_LEN, TEXT("%s GOD-MODE: %s"), DEMO_NAME[RENDER_PAGE - 21], TEXT("OFF"));	//Note: Have to be "wchar"
				((DirectX::DX11Class*)m_Driver)->addText(10, SystemHandle->AppSettings->WINDOW_HEIGHT - 120, DEMO_NAME_SHOW, 1, 1, 1);
			} 
			else 
		#endif
			{
				((DirectX::DX11Class*)m_Driver)->addText(10, SystemHandle->AppSettings->WINDOW_HEIGHT - 120, DEMO_NAME[RENDER_PAGE - 21], 1, 1, 1);
			}
		}
	}

	m_Driver->RenderDriverText(pContext);
	}
#endif

}

float ApplicationClass::ProcessInputUpdate()
{
	float fadeLight = 1;

#if defined USE_TIMER_CLASS
#if defined INTRO_DEMO
	// TIME Control: Show Debug Info
	UINT64 passedTotalTime = (UINT64)((SystemHandle->m_Timer.currentTime - SystemHandle->m_Timer.m_startEngineTime) / SystemHandle->m_Timer.m_ticksPerMs);	// To control events in time (DEMO)
    //if (m_Driver->RenderfirstTime)
    //{
    //    TCHAR tmp[MAX_STR_LEN]; _stprintf(tmp, TEXT("PASSED TOTAL TIME TO LOAD: %ju ms\n"), passedTotalTime); OutputDebugString(tmp);
    //}
#endif
#if defined INTRO_DEMO
	// 5 INTRO DEBUG TEXT: Show time, etc..
	if (RENDER_PAGE < 21) {
		fadeIntro = WOMA_APPLICATION_IntroRender(passedTotalTime);
	}
	else
		fadeIntro = 1;

	WOMA_APPLICATION_DemoRender(passedTotalTime);

	if (RENDER_PAGE < 15)
		return 0;

#if defined USE_DIRECT_INPUT && defined INTRO_DEMO // Animate Camera (INTRO_DEMO)
	SystemHandle->m_player[g_NetID]->p_player.IsUpPressed = true;
	SystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed = true;
#endif
#endif
#endif

	//--------------------------------------------------------------------------------------------
	// DETECT COLISIONS: Get the closest Compound object/(s):
	//--------------------------------------------------------------------------------------------
#if defined CHECK_OBJ_COLISION //CHECK_COMPOUND_COLISION //Get the closest Compound object/(s):

	float X = 0.0f, Z = 0.0f;
	float camX = m_Position[g_NetID]->m_positionX;
	float camZ = m_Position[g_NetID]->m_positionZ;

	for (UINT c = 0; c < world_main_size; c++)
	{
		int id = WOMA::sceneManager->visibleModelList[c]->xmlId;
		X = objModel[id]->PosX - camX; //compound[id].posX
		Z = objModel[id]->PosZ - camZ; //compound[id].posZ
		compoundLoadingOrder[c].order = (UINT)(X * X + Z * Z);
	}
	qsort(compoundLoadingOrder, world_main_size, sizeof(compoundTreeLoadOrder), CompoundSortCB);	// Order compound by distance:

	// [Collision 1] Check Collision with "10" COMPOUNDS near to us...:
	// ----------------------------------------------------------------
    XMVECTOR prwsPos = {}, prwsDir = {};
	/////////////////////////////////////////  IMPORTANT - Get the Collision Ray /////////////////////////////////////////
#if defined CHECK_OBJ_COLISION
	pickRayVector((float)SystemHandle->AppSettings->WINDOW_WIDTH / 2.0f, (float)SystemHandle->AppSettings->WINDOW_HEIGHT - 65, prwsPos, prwsDir);
#endif

    UINT	closestObjId = UINT_MAX;
	for (UINT c = 0; c < MIN (world_main_size, 5); c++)        // We don't need all, right?:)
	{
		int i = c;	// This is the compound[id] to check collisions...
		{
			closestObjDist = pick(prwsPos, prwsDir, objModel[i]->boundingBoxVerts,
                                                    objModel[i]->boundingBoxIndex,
                                                    ((DXmodelClass*)objModel[i])->m_worldMatrix, false);	// Use Bounding Boxes, Faster!
			if (closestObjDist < FLT_MAX)
			{
				closestObjId = i;	// Get the Closest Object ID!
				break;
			}
		}
	}

	// Calculate it with more accuracy if we are really close to an object:
	if (closestObjDist >= 0 && closestObjDist <= 3) 
    {
        closestObjDist = pick(prwsPos, prwsDir, objModel[closestObjId]->bottleVertPosArray,
                              ((DXmodelClass*)objModel[closestObjId])->obj3d.indices32,
                              ((DXmodelClass*)objModel[closestObjId])->m_worldMatrix, true);
	}

#endif

#if defined USE_DIRECT_INPUT					
	ProcessUserKeyboardInput(dt); //Keyboard keys
#endif

	// [2] CAMERA SKY: Update & Prepare to Take a Shot
	// ==============
#if defined USE_SKYSPHERE && defined USE_SKY_CAMERA_DOME	
	if (RENDER_PAGE >= 28)
	{
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
	#if defined DX_ENGINE
			
        #if defined USE_3RD_PERSON_CAMERA

            if (g_GOD_MODE) {
                DXsystemHandle->m_CameraSKY->m_rotationX = DXsystemHandle->m_Camera->m_rotationX;
                DXsystemHandle->m_CameraSKY->m_rotationY = DXsystemHandle->m_Camera->m_rotationY;
                DXsystemHandle->m_CameraSKY->CalculateViewMatrix();
            } else {
				#if defined USE_MULTI_MONITOR
				DXsystemHandle->m_CameraSKY->CalculateViewMatrix_3rd_PersonCamera(main3rdPcamera.m_camYaw, main3rdPcamera.m_camPitch, true);
				#else
                DXsystemHandle->m_CameraSKY->CalculateViewMatrix_3rd_PersonCamera(main3rdPcamera.m_camYaw, main3rdPcamera.m_camPitch, true);
				#endif
            }
        #else
            DXsystemHandle->m_CameraSKY->m_rotationX = DXsystemHandle->m_Camera->m_rotationX;
            DXsystemHandle->m_CameraSKY->m_rotationY = DXsystemHandle->m_Camera->m_rotationY;
            DXsystemHandle->m_CameraSKY->CalculateViewMatrix();
        #endif
	#endif
		}
	#if (defined OPENGL3 || defined OPENGL4)
		else
		{
			GLopenGLclass* driver = (GLopenGLclass*)driverList[SystemHandle->AppSettings->DRIVER];
			driver->gl_CameraSKY->m_rotationX = driver->gl_Camera->m_rotationX;
			driver->gl_CameraSKY->m_rotationY = driver->gl_Camera->m_rotationY;
			driver->gl_CameraSKY->CalculateViewMatrix();
		}
	#endif
	}
#endif

	//Update Sun and Moon position:
#if defined USE_ASTRO_CLASS
if (!astroClass) {
	WOMA_APPLICATION_InitGUI();
}
#endif

return fadeLight;
}


void ApplicationClass::RenderHUD_Logo(void* pContext)
{
#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(pContext);
#endif
	m_Driver->ClearDepthBuffer(pContext);
	// -------------------------
#if (defined USE_TITLE_BANNER && defined MAIN_RENDER) && defined MAIN_RENDER_TITLE	// MAIN-RENDER: TITLE (0.3 ms)
	if ((RENDER_PAGE >= 24 && m_titleModel) && (WOMA::game_state != GAME_MAP))		//Don't render title, on main map!
	{
		float rescale = 1;
		int X = ((SystemHandle->AppSettings->WINDOW_WIDTH - m_titleModel->SpriteTextureWidth) / 2);
		int Y = 10;
		m_titleModel->RenderSprite(pContext, X, Y, rescale, 1.0f);
	}
#endif
}

// INTRO
#if defined INTRO_DEMO //DEMO fade speed
#define fadeSpeed 0.00025f
float ApplicationClass::WOMA_APPLICATION_IntroRender(UINT64 passedTotalTime)
{
	static bool FadeIn = true;
	static float fade = 0.01f;

	if (FadeIn)
		fade = fade + fadeSpeed * (float)dt;
	else
		fade = fade - fadeSpeed * (float)dt;

	if (fade >= 1)		// Fade until Max? Now go Down
	{
		fade = 1;
		FadeIn = !FadeIn;
	}

	if (fade <= 0)		// Fade until Min? Now go Up
	{
		fade = 0;
		FadeIn = !FadeIn;
		SpriteScreenToShow++;
		womalog("SpriteScreenToShow: %d\n", SpriteScreenToShow);
	}

	return fade;
}


float ApplicationClass::WOMA_APPLICATION_DemoRender(UINT64 passedTotalTime)
{
	static bool FadeIn = true;
	static float fade = 0;

	if (fade == 0)
	{
#if defined ANDROID_PLATFORM
		if (RENDER_PAGE >= 21)
		{
			ShowAlert(DEMO_NAME[RENDER_PAGE - 21]);
			Sleep(750);
		}
#endif 
		if (RENDER_PAGE == GAME_SYSTEM_SETTINGS)
		{
			WOMA::game_state = GAME_SYSTEM_SETTINGS; OS_REDRAW_WINDOW;
		}

		if (RENDER_PAGE == GAME_CELESTIAL_INFO)
		{
			WOMA::game_state = GAME_CELESTIAL_INFO; OS_REDRAW_WINDOW;
		}

		if (RENDER_PAGE == GAME_SHOW_POSITION)
		{
			WOMA::game_state = GAME_SHOW_POSITION; OS_REDRAW_WINDOW;
		}

		if (RENDER_PAGE == GAME_WEATHER_INFO)
		{
			WOMA::game_state = GAME_WEATHER_INFO; OS_REDRAW_WINDOW;
		}

		if (RENDER_PAGE >= 15) {
			WOMA::game_state = GAME_RUN;
		}
	}

	if (FadeIn)
		fade = fade + fadeSpeed * (float)dt;
	else
		fade = fade - fadeSpeed * (float)dt;

	if (fade >= 1)		// Fade until Max? Now go Down
	{
		fade = 1;
		FadeIn = !FadeIn;
	}

	if (fade <= 0)		// Fade until Min? Now go Up
	{
		fade = 0;
		FadeIn = !FadeIn;
		if (RENDER_PAGE < DX_ENGINE_LEVEL) {
			RENDER_PAGE++;
			womalog("RENDER_PAGE: %d\n", RENDER_PAGE);
#if defined ANDROID_PLATFORM
			ShowFPS(RENDER_PAGE);
#endif
#if defined USE_DIRECT_INPUT && defined INTRO_DEMO
			m_Position[g_NetID]->m_positionX = SystemHandle->AppSettings->INIT_CAMX;
			m_Position[g_NetID]->m_positionY = SystemHandle->AppSettings->INIT_CAMY;
			m_Position[g_NetID]->m_positionZ = SystemHandle->AppSettings->INIT_CAMZ;
#endif
			if (RENDER_PAGE == 28)
				FORCE_RENDER_ALL = true;
			else
				FORCE_RENDER_ALL = false;
		}
		else {
			womalog("STOP: WOMA_APPLICATION_DemoRender ()\n");
			WOMA::main_loop_state = -1;		//WINDOWS
			WOMA::game_state = GAME_STOP;	//LINUX
		}
	}

	return fade;
}
#endif

static float rY = 0.0f;

void ApplicationClass::DemoRender(void* pContext)
{
	rY = (float)(dt) * (0.005f / 16.66f);	// MOVIMENT FORMULA!

	//COLOR TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
#if defined SCENE_COLOR //DEMO-1: Square

#if defined INTRO_DEMO //force page 21
	if (RENDER_PAGE == 21 || FORCE_RENDER_ALL)
#else
	if (RENDER_PAGE == 21 || RENDER_PAGE == 22 || RENDER_PAGE == 23 || RENDER_PAGE == 24 || FORCE_RENDER_ALL)
#endif
	{
#if defined USE_RASTERIZER_STATE
		m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID); // Render the Inside of Sphere
#endif

		if (RENDER_PAGE < 28) {
#if defined ROTATE_SQUARE
			// Rotate the world matrix by the rotation value so that the Square will spin:
			m_1stSquare3DColorModel->translation(0, -3, 0);
			m_1stSquare3DColorModel->rotateY(rY);
#endif
			m_1stSquare3DColorModel->Render(pContext);
		}

		//DEMO-2: Triangle!!{
		m_1stTriangle3DColorModel->translation(0, 8.75, -5); //>22
		m_1stTriangle3DColorModel->Render(pContext);
	}
#endif

	//TEXTURE TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 22 && defined SCENE_TEXTURE
#if !defined  NO_SCENE_IMAGE_LOAD
	//DEMO-1:
	//if (RENDER_PAGE == 22 || RENDER_PAGE == 28 || FORCE_RENDER_ALL)
#if defined INTRO_DEMO //DEMO Img formats
	if (RENDER_PAGE == 22 || FORCE_RENDER_ALL)
#endif
	{
		//Line1:
#if defined USE_IMAGE_BMP
		m_bmp3DModel->Render(pContext);
#endif
#if defined USE_IMAGE_PNG
		m_png3DModel->Render(pContext);
#endif

		//Line2:
#if defined USE_IMAGE_JPG
		m_jpg3DModel->Render(pContext);
#endif
#if defined USE_IMAGE_TIFF
		m_tif3DModel->Render(pContext);
#endif

		//Line3:
#if defined USE_IMAGE_DDS
		m_dds3DModel->Render(pContext);
#endif
#if defined SUPPORT_TGA
		m_tga3DModel->Render(pContext);
#endif
	}
#endif

	//DEMO-2: Triangle!!
	//if ((RENDER_PAGE >= 22 && RENDER_PAGE < 24) || FORCE_RENDER_ALL)
#if defined INTRO_DEMO //force page 22
	if (RENDER_PAGE == 22 || FORCE_RENDER_ALL)
#endif
	{
		m_1stTriangleTextureVertexModel->translation(0, 6.5, -5); //=22
		m_1stTriangleTextureVertexModel->Render(pContext);
	}
#endif

	//LIGHT TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 23 && defined SCENE_TEXTURE_LIGHT

	//DEMO-2
	//if ((RENDER_PAGE >= 23 && RENDER_PAGE < 24) || FORCE_RENDER_ALL)
#if defined INTRO_DEMO //force page 23
	if (RENDER_PAGE == 23 || FORCE_RENDER_ALL)
#endif
	{
		{
			m_3th3DModel2->translation(0, 4.75f, 1);
			m_3th3DModel2->scale(1.25f, 1.25f, 1.25f);
		}

		m_3th3DModel2->Render(pContext);
	}
#endif

	//CUBE TUTORIAL DEMO:
#define Y_pos 1
// --------------------------------------------------------------------------------------------
#if defined USE_CUBE	
	//DEMO-1
	//if ((RENDER_PAGE == 25 && m_cube1Model) || FORCE_RENDER_ALL)
#if defined INTRO_DEMO //DEMO25 or 26
	if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
#else
	if (RENDER_PAGE < 30)
#endif
	{
		static float rY = 0.0f;
		rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube1Model->rotateY(rY);
		m_cube1Model->translation(-3.5, Y_pos, 1);
		m_cube1Model->Render(pContext);
	}

	//DEMO-2
	//if ((RENDER_PAGE == 25 && m_cube2Model) || FORCE_RENDER_ALL)
#if defined INTRO_DEMO //DEMO25 or 26
	if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
#else
	if (RENDER_PAGE < 30)
#endif
	{
		float rY = 0.0f;
		rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube2Model->rotateY(rY);
		m_cube2Model->translation(0, Y_pos, 1);
		m_cube2Model->Render(pContext);
	}

	//DEMO-3
	//if ((RENDER_PAGE == 25 && m_cube3Model) || FORCE_RENDER_ALL)
#if defined INTRO_DEMO
	if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
#else
	if (RENDER_PAGE < 30)
#endif
	{
		m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube3Model->rotateY(rY);
		m_cube3Model->translation(3.5, Y_pos, 1);
		m_cube3Model->Render(pContext);
	}
#endif

	// SPHEREs
	// --------------------------------------------------------------------------------------------
#if defined USE_SPHERE && DX_ENGINE_LEVEL >= 26
	if ((RENDER_PAGE == 26 || RENDER_PAGE == 28) && m_SphereModel1)
#if defined INTRO_DEMO
		if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
#else
		if (RENDER_PAGE < 30)
#endif
		{
			m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);
			float rY = 0.0f;
			rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
			m_SphereModel1->rotateY(rY);
			m_SphereModel1->translation(-3.2f, -4, 5.0f);
			m_SphereModel1->Render(pContext, 0, 0, 0, NULL, NULL);
		}

	if ((RENDER_PAGE == 26 || RENDER_PAGE == 28) && m_SphereModel1)
#if defined INTRO_DEMO
		if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
#else
		if (RENDER_PAGE < 30)
#endif
		{
			m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);
			float rY = 0.0f;
			rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
			m_SphereModel2->rotateY(rY);
			m_SphereModel2->translation(3.2f, -4, 5.0f);
			m_SphereModel2->Render(pContext, 0, 0, 0, NULL, NULL);
		}
#endif

}

#if DX_ENGINE_LEVEL >= 21 && defined INTRO_DEMO	//29  DEMO SPRITE TEXT
void ApplicationClass::RenderDemoIntroSprites(void* pContext)
{
	//USING: ModelShaderType = SHADER_TEXTURE_FONT(3)/
	if (RENDER_PAGE < 21 && (SpriteScreenToShow >= 0 && SpriteScreenToShow < m_screenShots.size()))
	{
		VirtualModelClass* screenShot = m_screenShots[SpriteScreenToShow];

		//WINDOWS:
		screenShot->RenderSprite(pContext, (SystemHandle->AppSettings->WINDOW_WIDTH - screenShot->SpriteTextureWidth) / 2, SystemHandle->AppSettings->WINDOW_HEIGHT / 2, 1, fadeIntro);
	}
}
#endif


#if defined CHECK_OBJ_COLISION
// Calculate the world space pick ray from the 2D coordinates
#if false
void ApplicationClass::pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir)
{
#define m_driver11 ((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])

    // [1] Get dimensions
    float width = static_cast<float>(SystemHandle->windowsArray[0].width);
    float height = static_cast<float>(SystemHandle->windowsArray[0].height);

    // [2] Convert mouse to Normalized Device Coordinates (NDC)
    float ndcX = (2.0f * mouseX / width) - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY / height); // invert Y

    // [3] View-space ray in clip space (Z=1 for far plane, W=1)
    XMVECTOR rayClip = XMVectorSet(ndcX, ndcY, 1.0f, 1.0f);

    // [4] Inverse projection transform (clip -> view space)
    XMMATRIX projMatrix = m_driver11->m_projectionMatrix;
    XMMATRIX invProj = XMMatrixInverse(nullptr, projMatrix);
    XMVECTOR rayEye = XMVector4Transform(rayClip, invProj);

    // THIS IS IMPORTANT: rayEye is homogeneous; convert to direction
    rayEye = XMVectorSet(rayEye.m128_f32[0], rayEye.m128_f32[1], rayEye.m128_f32[2], 0.0f);

    // [5] Inverse view transform (view -> world space)
    XMMATRIX viewMatrix = DXsystemHandle->m_Camera->m_viewMatrix;
    XMMATRIX invView = XMMatrixInverse(nullptr, viewMatrix);
    pickRayInWorldSpaceDir = XMVector3TransformNormal(rayEye, invView);
    pickRayInWorldSpaceDir = XMVector3Normalize(pickRayInWorldSpaceDir);

    // [6] Ray origin = camera position
    pickRayInWorldSpacePos = XMLoadFloat3(&DXsystemHandle->m_Camera->GetPosition());
}
#else
// ==================================================================================================================================
void ApplicationClass::pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir)
// ==================================================================================================================================
{
    #define m_driver11 ((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])

#define _11 r[0].m128_f32[0]
#define _12 r[0].m128_f32[1]
#define _13 r[0].m128_f32[2]
#define _14 r[0].m128_f32[3]

#define _21 r[1].m128_f32[0]
#define _22 r[1].m128_f32[1]
#define _23 r[1].m128_f32[2]
#define _24 r[1].m128_f32[3]

#define _31 r[2].m128_f32[0]
#define _32 r[2].m128_f32[1]
#define _33 r[2].m128_f32[2]
#define _34 r[2].m128_f32[3]

#define _41 r[3].m128_f32[0]
#define _42 r[3].m128_f32[1]
#define _43 r[3].m128_f32[2]
#define _44 r[3].m128_f32[3]

    int ClientWidth = SystemHandle->AppSettings->WINDOW_WIDTH;   
    int ClientHeight = SystemHandle->AppSettings->WINDOW_HEIGHT; 
    //float ClientWidth = static_cast<float>(SystemHandle->windowsArray[0].width);
    //float ClientHeight = static_cast<float>(SystemHandle->windowsArray[0].height);

    XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

    float PRVecX, PRVecY, PRVecZ;

	XMMATRIX m_projectionMatrix;
    m_driver11->GetProjectionMatrix(m_projectionMatrix);

	PRVecX = (((2.0f * mouseX) / ClientWidth) - 1) / m_projectionMatrix._11;
	PRVecY = -(((2.0f * mouseY) / ClientHeight) - 1) / m_projectionMatrix._22;
    PRVecZ = 1;

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;	//We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	// Convert D3DXMATRIX to XMMATRIX:
	XMMATRIX* camView = m_driver11->GetViewMatrix(CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_OPAC, NULL /*lightViewMatrix*/, NULL/*ShadowProjectionMatrix*/);

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, *camView);	//Inverse of View Space matrix is World space matrix
    
    pickRayInWorldSpacePos = XMLoadFloat3(&DXsystemHandle->m_Camera->GetPosition());
	pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
    pickRayInWorldSpaceDir = XMVector3Normalize(pickRayInWorldSpaceDir);


#undef _11
#undef _12
#undef _13
#undef _14

#undef _21
#undef _22
#undef _23
#undef _24

#undef _31
#undef _32
#undef _33
#undef _34

#undef _41
#undef _42
#undef _43
#undef _44
}
#endif


// Calculates whether the object was picked or not | getPoligon = true (detect colision)
// ==================================================================================================================================
float ApplicationClass::pick(XMVECTOR pickRayInWorldSpacePos, XMVECTOR pickRayInWorldSpaceDir, std::vector<XMFLOAT3>& vertPosArray, 
                            std::vector<UINT/*DWORD*/>& indexPosArray, XMMATRIX& worldSpace, bool getPoligon)
{
	float closer = FLT_MAX;
	bool found = false;

	//Loop through each triangle in the object
	for (UINT i = 0; i < indexPosArray.size() / 3; i++)
	{
		//Triangle's vertices V1, V2, V3
		XMVECTOR tri1V1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tri1V2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tri1V3 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Temporary 3d floats for each vertex
		XMFLOAT3 tV1, tV2, tV3;

		//Get triangle 
		tV1 = vertPosArray[indexPosArray[(i * 3) + 0]];
		tV2 = vertPosArray[indexPosArray[(i * 3) + 1]];
		tV3 = vertPosArray[indexPosArray[(i * 3) + 2]];

		tri1V1 = XMVectorSet(tV1.x, tV1.y, tV1.z, 0.0f);
		tri1V2 = XMVectorSet(tV2.x, tV2.y, tV2.z, 0.0f);
		tri1V3 = XMVectorSet(tV3.x, tV3.y, tV3.z, 0.0f);

		//Transform the vertices to world space
		tri1V1 = XMVector3TransformCoord(tri1V1, worldSpace);
		tri1V2 = XMVector3TransformCoord(tri1V2, worldSpace);
		tri1V3 = XMVector3TransformCoord(tri1V3, worldSpace);

		//Find the normal using U, V coordinates (two edges)
		XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		U = tri1V2 - tri1V1;
		V = tri1V3 - tri1V1;

		//Compute face normal by crossing U, V
		faceNormal = XMVector3Cross(U, V);
		faceNormal = XMVector3Normalize(faceNormal);

		//Calculate a point on the triangle for the plane equation
		XMVECTOR triPoint = tri1V1;

		//Get plane equation ("Ax + By + Cz + D = 0") Variables
		float tri1A = XMVectorGetX(faceNormal);
		float tri1B = XMVectorGetY(faceNormal);
		float tri1C = XMVectorGetZ(faceNormal);
		float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

		//Now we find where (on the ray) the ray intersects with the triangles plane
		float ep1, ep2, t = 0.0f;
		float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
		XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		ep1 = (XMVectorGetX(pickRayInWorldSpacePos) * tri1A) + (XMVectorGetY(pickRayInWorldSpacePos) * tri1B) + (XMVectorGetZ(pickRayInWorldSpacePos) * tri1C);
		ep2 = (XMVectorGetX(pickRayInWorldSpaceDir) * tri1A) + (XMVectorGetY(pickRayInWorldSpaceDir) * tri1B) + (XMVectorGetZ(pickRayInWorldSpaceDir) * tri1C);

		//Make sure there are no divide-by-zeros
		if (ep2 != 0.0f)
			t = -(ep1 + tri1D) / (ep2);

		if (t > 0.0f || getPoligon)    //Make sure you don't pick objects behind the camera
		{
			//Get the point on the plane
			planeIntersectX = XMVectorGetX(pickRayInWorldSpacePos) + XMVectorGetX(pickRayInWorldSpaceDir) * t;
			planeIntersectY = XMVectorGetY(pickRayInWorldSpacePos) + XMVectorGetY(pickRayInWorldSpaceDir) * t;
			planeIntersectZ = XMVectorGetZ(pickRayInWorldSpacePos) + XMVectorGetZ(pickRayInWorldSpaceDir) * t;

			pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

			//Call function to check if point is in the triangle
			if (PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
			{
				//Return the distance to the hit, so you can check all the other pickable objects in your scene
				//and choose whichever object is closest to the camera
				if (getPoligon) {	//crashPoligonIndxIs = i;
					if (abs(t / 2.0f) < abs(closer))
						closer = t / 2.0f;
					found = true;
				}
				else
					return t / 2.0f;
			}
		}
	}

	if (!found)
		return FLT_MAX;	//return the max float value (near infinity) if an object was not picked
	else
		return closer;
}

// This function is called from the pick function, and it says if a point is inside a triangle (the point has to be on the triangles plane to know for sure).
// ==================================================================================================================================
bool ApplicationClass::PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
// ==================================================================================================================================
{
	//To find out if the point is inside the triangle, we will check to see if the point
	//is on the correct side of each of the triangles edges.
	XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
	XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
	{
		cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
		cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
			cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}

#endif


