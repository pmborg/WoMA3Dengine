// --------------------------------------------------------------------------------------------
// Filename: renderApplication_Basics.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "platform.h"
#pragma warning(disable : 4267)		// warning C4267: 'initializing': conversion from 'size_t' to 'UINT', possible loss of data
#pragma warning( disable : 4065 )	//warning C4065: switch statement contains 'default' but no 'case' labels

#include "OSengine.h"
#include "OSmain_dir.h"
#include "ApplicationClass.h"
#include "fileLoader.h"
#include "mem_leak.h"
#include "default_settings_xml.h"

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
extern RApplicationClass* r_Application;
#endif

//
// Declare Drivers:
//
#include "womadriverclass.h"	//woma


#if defined DX11 || defined DX9
#include "Dx11Class.h"
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif

#if (defined OPENGL3 || defined OPENGL4)
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif

#if defined VULKAN
#include "vkClass.h"
#endif



#if TUTORIAL_CHAP >= 60 // BILLBOARD
#include "BillClass.h"	//[ch60]
#endif


void ApplicationClass::SortOutWhatNeedToBeRendered(void* pContext, WomaDriverClass* driver)
{
#if defined _DEBUG || defined LINUX_PLATFORM || defined ANDROID_PLATFORM
	womalogATfirstframe(TEXT("ApplicationClass::SortOutWhatNeedToBeRendered()\n"));
#endif

	totalRendered = 0;

	// [1] SET A SPECIFIC CAMERA POSITION FOR BILLBOARD SORT:
	// --------------------------------------------------------------------------------------------

	// [2] SCENEMANAGER: PROCESS/FILTER AND CREATE LISTS/TREES OF OBJECTS TO RENDER FROM: WORLD.XML
	// --------------------------------------------------------------------------------------------
#if defined USE_SCENE_MANAGER //&& (defined DX_ENGINE)
	if (RENDER_PAGE >= 30)
	{

	WOMA::sceneManager->CreateLists(RENDER_PAGE);			//CREATE LISTS: for all 3D&2D objects to render (from WORLD.XML) and more

	world_visibleModelList_size = WOMA::sceneManager->visibleModelList.size();
	}
#endif

	// [4] RESTORE DEFAULT CAMERA POSITION:
	// --------------------------------------------------------------------------------------------

	// [5] ROTATE BILLS:
	// --------------------------------------------------------------------------------------------
}


// Returns the graphics context associated with the active driver.
// DX9/DX11/DX12 : ID3D*DeviceContext*
// Vulkan        : VkCommandBuffer (or load context during GAME_LOADING)
// OpenGL        : OpenGL context handle
// Returns NULL if no valid rendering context exists.
void* getvoidcontext()
{

	if (driverList.size() > 0 && driverList[WOMA::AppSettings->DRIVER])
	{
		switch (WOMA::AppSettings->DRIVER)
		{
#if defined DX11 && defined DX_ENGINE
		case DRIVER_DX9:
		case DRIVER_DX11:
			return ((DX11Class*)m_Driver)->GetDeviceContext();
#endif
#if defined DX12 && defined DX_ENGINE
		case DRIVER_DX12:
			return ((DX12Class*)m_Driver)->GetDeviceContext();
#endif
#if defined OPENGL3 || defined OPENGL40
		case DRIVER_GL3:
			return g_contextDriver;
#endif
#if defined VULKAN
		case DRIVER_VULKAN:
			return NULL;
#endif
		default:
			womalog("getvoidcontext(): Unknown DRIVER=%d\n", WOMA::AppSettings->DRIVER);
			ASSERT_DEBUG(false);
		}
	}

	return NULL;
}

#define TERRAIN_SCALE 1

void ApplicationClass::RenderMainSky(UINT monitorWindow, float fadeLight, void* pContext)
{
#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending(pContext);
#endif

	// RENDER: SKY Sphere:
	//----------------------------------------------------------------------------------------------------------------------
#if (defined USE_SKY_CAMERA_DOME && defined USE_SKYSPHERE) && defined MAIN_RENDER_SKY	// MAIN-RENDER: "Sky": (0.0ms)
	if (RENDER_PAGE >= 28 && m_SkyModel)
	{
		if (m_Driver->RenderfirstTime)
		{
			m_SkyModel->translation(0, 0, 0);
			m_SkyModel->scale(20, 20, 20);
		}

		m_SkyModel->RenderSky(pContext, CAMERA_SKY, fadeLight); // Camera with fixed position: 0,0,0: (CAMERA_SKY)
	}
#endif

}

#define UPDATE_WATER_TIME(shader)\
{\
	shader->watertime += (float)dt * (0.0025f / 16.66f);\
	if (shader->watertime > 2 * PI)\
		shader->watertime -= (2 * PI);\
}

//#############################################################################################################
// [2/3] RENDER - 3D
//#############################################################################################################
void ApplicationClass::AppRender(UINT monitorIndex, UINT level, float fadeLight, void* pContext)
{
	//[1] Must be first:
#if (defined USE_SKY_CAMERA_DOME && defined USE_SKYSPHERE) && defined MAIN_RENDER_SKY
#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(pContext);
#endif
	RenderMainSky(monitorIndex, fadeLight, pContext);
#endif

#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);	//Set Default
#endif

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending(pContext);						// restore default blending
#endif

	// [2] RENDER LIGHT RAY:
	// --------------------------------------------------------------------------------------------

#if defined USE_ALPHA_BLENDING && defined INTRO_DEMO
	m_Driver->TurnOffAlphaBlending(pContext);
	m_Driver->ClearDepthBuffer(pContext);
#endif

	if ((RENDER_PAGE >= 19) && (RENDER_PAGE < 30) || RENDER_PAGE == 36)
		DemoRender(pContext);	// ALL Demos!: page [19, 20, 21, 22, 23 ... 49]

//
// SHADOWs Aux for DEBUG
//
#if defined render_debug_2nd3DModel
#endif

#if defined MAIN_RENDER_TERRAIN
	if (RENDER_PAGE >= 49) {
		MainTerrain(monitorIndex, fadeLight, pContext);
	}
#endif

	// 3D STATIC OPAC OBJECTS on WORLD.XML, that listed in: sceneManager->visibleModelList (in front of camera)
	//----------------------------------------------------------------------------------------------------------------------

#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);	//Set Default
#endif
	// -----------------------------------------------------------------------------------
	// Render PASS 0 & 1 Parts of 3D OBJs (like: glass window of (Space Compound), etc...)
	// -----------------------------------------------------------------------------------

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
	m_lightRayModel->UpdateLightRayVertices(pContext, &MyLightVertexVector);
	m_lightRayModel->Render(pContext);
#endif

}

#if DX_ENGINE_LEVEL >= 24 || defined USE_VIEW2D_SPRITES
//#############################################################################################################
// [3/3] POS-RENDER - 2D: Render TRANSPARENT Parts of 3D OBJs (like: "Glass windows", "Billboards", etc...)
//#############################################################################################################
void ApplicationClass::AppPosRender(UINT monitorIndex, UINT level, float dayLightFade, void* pContext)
{
#if defined _DEBUG || defined LINUX_PLATFORM //|| defined ANDROID_PLATFORM
	womalogATfirstframe("RenderfirstTime: ApplicationClass::AppPosRender() START\n");
#endif

    //=============================================================================================================
    // LIGHT: Get fade (real Sun Position): Show Debug Info

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(pContext);
#endif

#if (TUTORIAL_CHAP >= 60 && defined SCENE_BILLBOARDS && defined USE_SCENE_MANAGER && defined DX_ENGINE) && defined MAIN_RENDER_BILLBOARDS // MAIN-RENDER: BILLBOARD + FENCES + FIRE (11.4 ms)
        UINT obj_id;
		for (UINT tree_id = 0; tree_id < _countof(m_Trees); tree_id++)
        {
            obj_id = m_Trees[tree_id].ID + world_xml_objs;
            if (SystemHandle->xml_loader.theWorldXML[obj_id].render)											// TODO: use sceneManager
				RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, m_Driver, obj_id, PASS_BILL, NULL, NULL, dayLightFade);  // Render: "Billboards"
		}
#endif

#if (defined USE_MAIN_MAP || defined USE_MINI_MAP) && defined MAIN_RENDER_MINIMAP //MAIN-RENDER: MINI-MAP (0.4)
	if (RENDER_PAGE >= 62)
		if (ShouldDrawUI(monitorIndex))
			RenderMainMapMiniMap3Dview(pContext);
#endif

	// -------------------------
	// === AppTextClass-Fill: ===
	// -------------------------
#if defined USE_RASTERTEK_TEXT_FONT && defined MAIN_RENDER_RASTERTEK_FONT

#if !defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
#endif
	if (ShouldDrawUI(monitorIndex) && AppTextClass)
	{
		AppTextClass->SetFps(SystemHandle->fps);						// Update the FPS "Value" in the text object.
#if defined WINDOWS_PLATFORM && !defined WIN_XP && defined RENDER_CPU_TEST
		AppTextClass->SetCpu(SystemHandle->m_Cpu.GetCpuPercentage());	// Update the CPU usage "Value" in the text object.
#endif

#if defined _DEBUG
	switch (WOMA::AppSettings->DRIVER)
	{
#if defined VULKAN && !defined DEBUG_TEXT_A_ONLY
		case DRIVER_VULKAN:
		{
			vkClass* driver = (vkClass*)m_Driver;

			AppTextClass->SetCameraRotation(driver->gl_Camera->m_rotationX, driver->gl_Camera->m_rotationY, driver->gl_Camera->m_rotationZ);
			AppTextClass->SetCameraPosition(driver->gl_Camera->m_positionX, driver->gl_Camera->m_positionY, driver->gl_Camera->m_positionZ);
			break;
		}
#endif

#if defined WINDOWS_PLATFORM
		default:
		{
	#if defined USE_DIRECT_INPUT			// we will use DX input method
			AppTextClass->SetCameraRotation(m_Position[g_NetID]->m_rotationX, m_Position[g_NetID]->m_rotationY, m_Position[g_NetID]->m_rotationZ);
			AppTextClass->SetCameraPosition(m_Position[g_NetID]->m_positionX, m_Position[g_NetID]->m_positionY, m_Position[g_NetID]->m_positionZ);
	#else
			AppTextClass->SetCameraPosition(DX_Camera->m_positionX, DX_Camera->m_positionY, DX_Camera->m_positionZ);
			AppTextClass->SetCameraRotation(DX_Camera->m_rotationX, DX_Camera->m_rotationY, DX_Camera->m_rotationZ);
	#endif			
			break;
		}
#endif

#if defined OPENGL3 || defined OPENGL4
		case DRIVER_GL3:
		{
			AppTextClass->SetCameraRotation(((GLopenGLclass*)m_Driver)->gl_Camera->m_rotationX, ((GLopenGLclass*)m_Driver)->gl_Camera->m_rotationY, ((GLopenGLclass*)m_Driver)->gl_Camera->m_rotationZ);
			AppTextClass->SetCameraPosition(((GLopenGLclass*)m_Driver)->gl_Camera->m_positionX, ((GLopenGLclass*)m_Driver)->gl_Camera->m_positionY, ((GLopenGLclass*)m_Driver)->gl_Camera->m_positionZ);

			break;
		}
#endif
	}
#endif

	//
	// === Update Render Count: ===
	//
#if TUTORIAL_CHAP >= 60 && !defined RELEASE // BILLBOARD
		AppTextClass->SetBillRenderCount(billboardRrenderCount, total_deltaTime);
#endif  
	}
#endif

	// RENDER RASTERTEK V1 FONT:
	// -------------------------
#if (defined USE_RASTERTEK_TEXT_FONT) && defined MAIN_RENDER_RASTERTEK_FONT //MAIN-RENDER: Raster FONT: v1 (0.4 ms)
	#if !defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	#endif
	if (ShouldDrawUI(monitorIndex) && RENDER_PAGE > 20)
	{
		ASSERT_DEBUG(AppTextClass);
		AppTextClass->Render(pContext);
	}
#endif

	// RENDER RASTERTEK V2 FONT:
	// -------------------------
#if defined USE_RASTERTEK_TEXT_FONTV2
#if !defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
#endif
	if (ShouldDrawUI(monitorIndex))
	{
		ASSERT_DEBUG(r_Application);
		r_Application->Render();
	}
		
#endif

	m_Driver->TurnOffAlphaBlending(pContext); // Re assume default
#if defined INTRO_DEMO // PRESENTATION SPRITE(s)  - On Top of 3D Rendered stuff: RenderDemoIntroSprites
	if (ShouldDrawUI(monitorIndex))
	{
		RenderDemoIntroSprites(pContext);
	}
#endif

	// RENDER NATIVE TEXT:
	// -------------------
#if defined USE_DX_DRIVER_FONT && defined MAIN_RENDER_DRIVER_FONT //MAIN-RENDER: Driver Font (0.5 ms)
	if (ShouldDrawUI(monitorIndex) && (WOMA::AppSettings->DRIVER == DRIVER_DX11))
	{
		#if !defined INTRO_DEMO //|DEMO Force NATIVE TEXT|
		if ((RENDER_PAGE >= 22) && (m_Driver->m_sCapabilities.USE_DXDRIVER_FONTSBoolean))
		#endif
		{
			if (RENDER_PAGE >= 21) {
			#if defined USE_DIRECT_INPUT
				if (RENDER_PAGE >= 49) {
					if (g_GOD_MODE)
						StringCchPrintf(DEMO_NAME_SHOW, MAX_STR_LEN, TEXT("%s GOD-MODE: %s"), DEMO_NAME[RENDER_PAGE - 21], TEXT("ON"));		//Note: Have to be "wchar"
					else
						StringCchPrintf(DEMO_NAME_SHOW, MAX_STR_LEN, TEXT("%s GOD-MODE: %s"), DEMO_NAME[RENDER_PAGE - 21], TEXT("OFF"));	//Note: Have to be "wchar"
					((DirectX::DX11Class*)m_Driver)->addText(10, WOMA::AppSettings->WINDOW_HEIGHT - 120, DEMO_NAME_SHOW, 1, 1, 1);
				} 
				else 
			#endif
				{
					((DirectX::DX11Class*)m_Driver)->addText(10, WOMA::AppSettings->WINDOW_HEIGHT - 120, DEMO_NAME[RENDER_PAGE - 21], 1, 1, 1);
				}
			}

			m_Driver->RenderDriverText(pContext);
		}
	}
#endif
}
#endif

// =============================================================================================
// FUNCTION: DetectCollisions()
// Purpose : Isolated collision detection for compounds and optional map editor picking.
// =============================================================================================



float ApplicationClass::ProcessMovementInput_and_UpdateDemos()
{
	float fadeLight = 1;

#if defined USE_TIMER_CLASS
  #if defined INTRO_DEMO
	// TIME Control: Show Debug Info
	UINT64 passedTotalTime = (UINT64)((SystemHandle->m_Timer.currentTime - SystemHandle->m_Timer.m_startEngineTime) / SystemHandle->m_Timer.m_ticksPerMs);	// To control events in time (DEMO)

    if (m_Driver->RenderfirstTime)
    {
        TCHAR tmp[MAX_STR_LEN]; _stprintf(tmp, TEXT("PASSED TOTAL TIME TO LOAD: %ju ms\n"), passedTotalTime); OutputDebugString(tmp);
    }

	// 5 INTRO CREDITS TEXT: Show time, etc..
	if (RENDER_PAGE < 21) {
		fadeIntro = WOMA_APPLICATION_IntroRender(passedTotalTime);
	}
	else
		fadeIntro = 1;

	WOMA_APPLICATION_DemoRender(passedTotalTime);

	if (RENDER_PAGE < 15)
		return 0;

	#if defined USE_DIRECT_INPUT && defined INTRO_DEMO && !defined USE_DEMO99 // Animate Camera (INTRO_DEMO)
	SystemHandle->m_player[g_NetID]->p_player.IsUpPressed = true;
	//SystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed = true;
	#endif
  #endif
#endif

	// ========================================================================================
	// [2] DETECT COLLISIONS (active from DX_ENGINE_LEVEL >= 78)
	// ========================================================================================

	// ========================================================================================
	// [3] USER INPUT (keyboard)
	// ========================================================================================

	// ========================================================================================
	// [4] CAMERA SKY UPDATE (third-person camera available from DX_ENGINE_LEVEL >= 88)
	// ========================================================================================
#if defined USE_SKYSPHERE && defined USE_SKY_CAMERA_DOME	
	IF_RENDER_PAGE(RENDER_PAGE >= 28) //if (RENDER_PAGE >= 28)
	{
		if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
		{
	#if defined WINDOWS_PLATFORM

		#if (DX_ENGINE_LEVEL < 88  || defined INTRO_DEMO) && defined DX_ENGINE
		#if !defined USE_3RD_PERSON_CAMERA || defined INTRO_DEMO
			IF_RENDER_PAGE(RENDER_PAGE < 88)
			{
				DX_CameraSKY->m_rotationX = DX_Camera->m_rotationX;
				DX_CameraSKY->m_rotationY = DX_Camera->m_rotationY;
				DX_CameraSKY->CalculateViewMatrix();
			}
        #endif
		#endif
	#endif
		}

#if defined VULKAN
		if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
		{
			vkClass* driver = (vkClass*)driverList[WOMA::AppSettings->DRIVER];
	#if defined WINDOWS_PLATFORM
			driver->gl_CameraSKY->m_rotationX = m_Position[g_NetID]->m_rotationX;
			driver->gl_CameraSKY->m_rotationY = m_Position[g_NetID]->m_rotationY;
	#else
			driver->gl_CameraSKY->m_rotationX = driver->gl_Camera->m_rotationX;
			driver->gl_CameraSKY->m_rotationY = driver->gl_Camera->m_rotationY;
	#endif
			driver->gl_CameraSKY->CalculateViewMatrix();
		}
#endif

	#if (defined OPENGL3 || defined OPENGL4)
		if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
		{
			GLopenGLclass* driver = (GLopenGLclass*)driverList[WOMA::AppSettings->DRIVER];
#if defined  WINDOWS_PLATFORM
			driver->gl_CameraSKY->m_rotationX = m_Position[g_NetID]->m_rotationX;
			driver->gl_CameraSKY->m_rotationY = m_Position[g_NetID]->m_rotationY;
#else
			driver->gl_CameraSKY->m_rotationX = driver->gl_Camera->m_rotationX;
			driver->gl_CameraSKY->m_rotationY = driver->gl_Camera->m_rotationY;
#endif
			driver->gl_CameraSKY->CalculateViewMatrix();
		}
	#endif
	}
#endif

	//UPDATE ASTRO POSITION: SUN AND MOON
#if defined USE_ASTRO_CLASS
	if (!astroClass) {
		WOMA_APPLICATION_InitGUI();
	}
#endif

return fadeLight;
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
		womalog("SpriteScreenToShow: %d - ", SpriteScreenToShow);
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
		if (RENDER_PAGE < DX_ENGINE_LEVEL) 
		{
			RENDER_PAGE++;
#if defined INTRO_DEMO && defined DX_ENGINE
			womalog("RENDER_PAGE: %d - posX: %4.2f posY: %4.2f posZ: %4.2f\n", RENDER_PAGE, DX_Camera->m_positionX, DX_Camera->m_positionY, DX_Camera->m_positionZ);
#endif

			// ---------------------------------------------------------------------
			// RECENTER CAMERA when entering new demo page (after fade-out)
			// ---------------------------------------------------------------------

#if defined ANDROID_PLATFORM
			ShowFPS(RENDER_PAGE);
#endif

#if defined USE_DIRECT_INPUT && defined USE_DEMO29
			m_Position[g_NetID]->m_positionX = WOMA::AppSettings->INIT_CAMX;
			m_Position[g_NetID]->m_positionY = WOMA::AppSettings->INIT_CAMY;
			m_Position[g_NetID]->m_positionZ = WOMA::AppSettings->INIT_CAMZ;
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
#ifdef USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending(pContext);
#endif
	rY = (float)(dt) * (0.005f / 16.66f);	// MOVIMENT FORMULA!

	if (RENDER_PAGE == 28)
		FORCE_RENDER_ALL = true;
	else
		FORCE_RENDER_ALL = false;

	#if defined INTRO_DEMO
	IF_RENDER_PAGE(RENDER_PAGE >= 21 && RENDER_PAGE <= 23)
	#else
	IF_RENDER_PAGE(RENDER_PAGE >= 19 && RENDER_PAGE <= 23)
	#endif
	{
		#define cor driverList[WOMA::AppSettings->DRIVER]->driver_ClearColor

		static float totalTime = 0;
		totalTime += dt / 1000.0f;
#if defined ANDROID_PLATFORM
		float t = (sinf(totalTime * 0.5f) * 0.5f) + 0.5f; // oscillates 0..1
#else
		float t = (sin(totalTime * 0.5f) * 0.5f) + 0.5f; // oscillates 0..1
#endif
		cor[0] = 0.1f + 0.1f * t;  // R channel slightly modulates
		cor[1] = 0.2f + 0.2f * t;  // G channel softly moves
		cor[2] = 0.5f + 0.4f * t;  // B channel oscillates more

		cor[2] += ((float)dt / 10000);
		cor[2] = cor[2] + 0.001f;
		if (cor[2] >= 1)
			cor[2] = 0;

		if (m_Driver->RenderfirstTime) womalog("dt = %f\n", dt);

		#undef cor
	}

	//COLOR TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
#if defined SCENE_COLOR //DEMO-1: Square

  #if !defined INTRO_DEMO //force page 21
	if (RENDER_PAGE == 21 || FORCE_RENDER_ALL)
  #else
	if (RENDER_PAGE == 21 || FORCE_RENDER_ALL)
  #endif
	{
	#if defined ROTATE_SQUARE
		// Rotate the world matrix by the rotation value so that the Square will spin:
		m_1stSquare3DColorModel->translation(0, -3, 0);
		m_1stSquare3DColorModel->rotateY(rY);
	#endif
		m_1stSquare3DColorModel->Render(pContext);

		//DEMO-2: Triangle!!{
		m_1stTriangle3DColorModel->translation(0, 6, -3); //>22
		m_1stTriangle3DColorModel->Render(pContext);
	}
#endif

	//TEXTURE TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
#if (DX_ENGINE_LEVEL >= 22 && defined SCENE_TEXTURE) || defined INTRO_DEMO
#if !defined NO_SCENE_IMAGE_LOAD || defined INTRO_DEMO
	//DEMO-1:

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

	#if defined INTRO_DEMO
	if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
	#else
	if (RENDER_PAGE < 30)
	#endif
	{
		float rY = 0.0f;
		rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube3Model->rotateY(rY);
		m_cube3Model->translation(3.5, Y_pos, 1);
		m_cube3Model->Render(pContext);
	}
#endif

	// SPHEREs
	// --------------------------------------------------------------------------------------------

}


#if defined CHECK_OBJ_COLISION
// Calculate the world space pick ray from the 2D coordinates
// ==================================================================================================================================
void pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir)
// ==================================================================================================================================
{
    #define m_driver11 ((DirectX::DX11Class*)driverList[WOMA::AppSettings->DRIVER])

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

    int ClientWidth = WOMA::AppSettings->WINDOW_WIDTH;   
    int ClientHeight = WOMA::AppSettings->WINDOW_HEIGHT; 

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
    
    pickRayInWorldSpacePos = XMLoadFloat3(&DX_Camera->GetPosition());
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


// Calculates whether the object was picked or not | getPoligon = true (detect colision)
// ==================================================================================================================================
float pick(XMVECTOR pickRayInWorldSpacePos, XMVECTOR pickRayInWorldSpaceDir, std::vector<XMFLOAT3>& vertPosArray, 
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
		//XMVECTOR faceNormalClosestObject = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		U = tri1V2 - tri1V1;
		V = tri1V3 - tri1V1;

		//Compute face normal by crossing U, V
		XMVECTOR triangleNormal = XMVector3Cross(U, V);
		triangleNormal = XMVector3Normalize(triangleNormal);

		//Calculate a point on the triangle for the plane equation
		XMVECTOR triPoint = tri1V1;

		//Get plane equation ("Ax + By + Cz + D = 0") Variables
		float tri1A = XMVectorGetX(triangleNormal);
		float tri1B = XMVectorGetY(triangleNormal);
		float tri1C = XMVectorGetZ(triangleNormal);
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
					{
						closer = t / 2.0f;
						faceNormalClosestObject = triangleNormal;
					}
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
bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
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

//NOTE: LoadTextureFromPathFBX work on DLL (on level >= 86)


