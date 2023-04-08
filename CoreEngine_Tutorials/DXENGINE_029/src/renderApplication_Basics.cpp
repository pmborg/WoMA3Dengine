// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: renderApplication_Basics.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567829;

#include "platform.h"
#include "dxWinSystemClass.h"
#include "ApplicationClass.h"

#if defined DX9sdk
#include "Dx9Class.h"
#endif
#include "Dx11Class.h"
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif
#include "womadriverclass.h"	//woma
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#include "wGLopenGLclass.h"		// Windows

#include "OSmain_dir.h"

void ApplicationClass::DemoRender()
{
	WomaDriverClass* m_Driver = DXsystemHandle->m_Driver;

	// RENDER: SKY SKY Sphere:
	// --------------------------------------------------------------------------------------------
	if (RENDER_PAGE >= 28 && m_SkyModel)
	{
		m_Driver->SetRasterizerState(CULL_NONE/*CULL_BACK*/, FILL_SOLID); // Render the Inside of Sphere
		m_SkyModel->translation(0, 0, 0);
		m_SkyModel->scale(10, 10, 10);
		m_SkyModel->RenderSky(m_Driver, CAMERA_SKY); // Cant Reach: (CAMERA_SKY)
	}

	//COLOR TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
	//DEMO-1: Squar
	if (RENDER_PAGE == 21 || FORCE_RENDER_ALL)
	{
		m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID); // Render the Inside of Sphere
		float rY = 0.0f;
		rY = dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!

		// Rotate the world matrix by the rotation value so that the Square will spin:
		m_1stSquar3DColorModel->translation(0, 0, 0);
		m_1stSquar3DColorModel->rotateY(rY);
		m_1stSquar3DColorModel->Render(m_Driver);
	}

	//DEMO-2: Triangle
	if (RENDER_PAGE <= 25 || FORCE_RENDER_ALL)
	{
		if (RENDER_PAGE == 21)
			m_1stTriangle3DColorModel->translation(0, 3.5, 1);
		else
			m_1stTriangle3DColorModel->translation(-7, 5.5, 1);

		m_1stTriangle3DColorModel->Render(m_Driver);
	}

	//TEXTURE TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
	//DEMO-1:
	if (RENDER_PAGE == 22 || FORCE_RENDER_ALL)
	{
		m_bmp3DModel->Render(m_Driver);
		m_jpg3DModel->Render(m_Driver);
		m_png3DModel->Render(m_Driver);
		m_tif3DModel->Render(m_Driver);
		m_dds3DModel->Render(m_Driver);
	#if defined SUPPORT_TGA
		m_tga3DModel->Render(m_Driver);
	#endif
	}

	//DEMO-2:
	if ((RENDER_PAGE >= 22 && RENDER_PAGE <= 25) || FORCE_RENDER_ALL)
	{

		if (RENDER_PAGE == 22)
			m_1stTriangleTextureVertexModel->translation(0, 4, 1);
		else
			m_1stTriangleTextureVertexModel->translation(-7, 3.5, 1);

		m_1stTriangleTextureVertexModel->Render(m_Driver);
	}

	//LIGHT TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
	//DEMO-1
	if (RENDER_PAGE == 23 || FORCE_RENDER_ALL)
	{
		float rY = 0.0f;
		rY = dt * (0.015f / 16.66f);		// MOVIMENT FORMULA!
		m_3th3DModel1->rotateX(rY);
		m_3th3DModel1->translation(0, 0, 1);
		m_3th3DModel1->Render(m_Driver);
	}

	//DEMO-2
	if ((RENDER_PAGE >= 23 && RENDER_PAGE <= 25) || FORCE_RENDER_ALL)
	{
		if (RENDER_PAGE == 23)
		{
			m_3th3DModel2->translation(0, 4.75f, 1);
			m_3th3DModel2->scale(1.75f, 1.75f, 1.75f);
		}
		else {
			m_3th3DModel2->translation(-7, 1.5f, 1);
			m_3th3DModel2->scale(1, 1, 1);
		}

		m_3th3DModel2->Render(m_Driver);
	}

	//CUBE TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
	//DEMO-1
	if ((RENDER_PAGE == 25 && m_cube1Model) || FORCE_RENDER_ALL)
	{
		float rY = 0.0f;
		rY = dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube1Model->rotateY(rY);
		m_cube1Model->translation(-2, -0.5f, 1);
		m_cube1Model->Render(m_Driver);
	}

	//DEMO-2
	if ((RENDER_PAGE == 25 && m_cube2Model) || FORCE_RENDER_ALL)
	{
		float rY = 0.0f;
		rY = dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube2Model->rotateY(rY);
		m_cube2Model->translation(1.5f, -0.5f, 1);
		m_cube2Model->Render(m_Driver);
	}

	//DEMO-3
	if ((RENDER_PAGE == 25 && m_cube3Model) || FORCE_RENDER_ALL)
	{
		m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube3Model->rotateY(rY);
		m_cube3Model->translation(5, -0.5f, 1);
		m_cube3Model->Render(m_Driver);
	}

	// SPHEREs
	// --------------------------------------------------------------------------------------------
	if ((RENDER_PAGE == 26 || RENDER_PAGE == 28) && m_SphereModel1)
	{
		m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_SphereModel1->rotateY(rY);
		m_SphereModel1->translation(-4, 2, 1);
		m_SphereModel1->Render(m_Driver);
	}
	if ((RENDER_PAGE == 26 || RENDER_PAGE == 28) && m_SphereModel1)
	{
		m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_SphereModel2->rotateY(rY);
		m_SphereModel2->translation(4, 2, 1);
		m_SphereModel2->Render(m_Driver);
	}

}

void ApplicationClass::RenderSprites()
{
  WomaDriverClass* m_Driver = DXsystemHandle->m_Driver;

  if (RENDER_PAGE < 21 && (SpriteScreenToShow >= 0 && SpriteScreenToShow < m_screenShots.size()))
  {
	  VirtualModelClass* screenShot = m_screenShots[SpriteScreenToShow];

	  screenShot->RenderSprite(m_Driver, 
		  (SystemHandle->AppSettings->WINDOW_WIDTH - screenShot->SpriteTextureWidth) / 2,
		  SystemHandle->AppSettings->WINDOW_HEIGHT / 2 - screenShot->SpriteTextureHeight * 2 + (SystemHandle->AppSettings->WINDOW_HEIGHT - screenShot->SpriteTextureHeight) / 2, 
		  1, fadeIntro);
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(UINT monitorWindow)
//-------------------------------------------------------------------------------------------
{
	if (RENDER_PAGE < 15)
	{
		Update(monitorWindow, SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);
		return;
	} 
	else
	{
		// [0] Process INPUT, CAMERA, INTRO animation etc...
		float dayLightFade = Update(monitorWindow, SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);

		// [1] Render: SHADOWS	Render one Application Frame
	
		// [2] RENDER: MAIN - 3D, Render one Application Frame
		AppRender(monitorWindow, dayLightFade);
		
		// [3] RENDER: SPRITEs on TOP of 3D - 2D Render one Application Frame. (Need to be after 3D)
		AppPosRender();
	}
}

//#############################################################################################################
// POS-RENDER - 2D: Render TRANSPARENT Parts of 3D OBJs (like: glass window, etc...)
//#############################################################################################################
void ApplicationClass::AppPosRender()
{
	WomaDriverClass* m_Driver = DXsystemHandle->m_Driver;
	m_Driver->TurnOnAlphaBlending();
	m_Driver->ClearDepthBuffer();		
	m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
	
	if (RENDER_PAGE >= 24 && m_titleModel)
	{
		m_titleModel->RenderSprite(DXsystemHandle->m_Driver, (SystemHandle->AppSettings->WINDOW_WIDTH - m_titleModel->SpriteTextureWidth) / 2, (SystemHandle->AppSettings->WINDOW_HEIGHT - m_titleModel->SpriteTextureHeight) / 2);
	}

	// Render all Rastertek TEXT:
#if _DEBUG
	if (RENDER_PAGE >= 27|| FORCE_LOAD_ALL)
#else
	if (RENDER_PAGE >= 27)
#endif
		AppTextClass->Render(); 

	// Render Native TEXT:

	m_Driver->TurnOffAlphaBlending();
	// BANNERs and Sprites - On Top of 3D Rendered:
	RenderSprites();
}

float ApplicationClass::Update(UINT monitorWindow, WomaDriverClass* m_Driver)
{
	float fadeLight = 1;

	// TIME Control: Show Debug Info
	INT64 passedTotalTime = (INT64)((SystemHandle->m_Timer.currentTime - SystemHandle->m_Timer.m_startEngineTime) / SystemHandle->m_Timer.m_ticksPerMs);	// To control events in time (DEMO)

	// 5 INTRO DEBUG TEXT: Show time, etc..
	if (RENDER_PAGE < 21)
		fadeIntro = WOMA_APPLICATION_IntroRender(passedTotalTime);
	else
		fadeIntro = 1;
	
	WOMA_APPLICATION_DemoRender(passedTotalTime);

	if (RENDER_PAGE < 15)
		return 0;

	AppTextClass->SetTime(passedTotalTime);
	AppTextClass->SetScreenToShow(SpriteScreenToShow, RENDER_PAGE);
	if (RENDER_PAGE <= 20)
		AppTextClass->SetFade(fadeIntro);


	// Animate Camera (INTRO_DEMO)
	DXsystemHandle->m_player[g_NetID]->p_player.IsDownPressed = true;

	// GET INPUT for CAMERA: Movement
	HandleUserInput(dt);

	// SET CAMERA (for this monitor): Prepare to Take a Shot: Generate the view matrix based on the camera's position.
	if (SystemHandle->windowsArray.size() == 3)
	{
		// TODO: settings.xml define: LEFT/RIGTH: Monitor
		// Monitors Index:
		// | 1 | 0 | 2 |
		if (monitorWindow == 1) DXsystemHandle->m_Camera->m_rotationY -= (90 / 3); //  90:3 = 30deg
		if (monitorWindow == 0) DXsystemHandle->m_Camera->m_rotationY -= 0;
		if (monitorWindow == 2) DXsystemHandle->m_Camera->m_rotationY += (90 / 3);	//  90:3 = 30deg
	}
	if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		if (DXsystemHandle->m_Camera)
			DXsystemHandle->m_Camera->Render();
	}
	else
	{
		GLopenGLclass* driver = (GLopenGLclass*)DXsystemHandle->driverList[SystemHandle->AppSettings->DRIVER];
		if (driver->gl_Camera)
			driver->gl_Camera->Render();
	}

	if (RENDER_PAGE >= 28) 
	{
		if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
			DXsystemHandle->m_CameraSKY->m_rotationX = DXsystemHandle->m_Camera->m_rotationX;
			DXsystemHandle->m_CameraSKY->m_rotationY = DXsystemHandle->m_Camera->m_rotationY;
			DXsystemHandle->m_CameraSKY->Render();
		}
		else
		{
			GLopenGLclass* driver = (GLopenGLclass*)DXsystemHandle->driverList[SystemHandle->AppSettings->DRIVER];

			driver->gl_CameraSKY->m_rotationX = driver->gl_Camera->m_rotationX;
			driver->gl_CameraSKY->m_rotationY = driver->gl_Camera->m_rotationY;
			driver->gl_CameraSKY->Render();
		}
	}

	// CONSTRUCT: FRUSTRUM

	// CAMERA TEXT: Show Debug Info
	AppTextClass->SetFps(SystemHandle->fps);						// Update the FPS "Value" in the text object.
	AppTextClass->SetCpu(SystemHandle->m_Cpu.GetCpuPercentage());	// Update the CPU usage "Value" in the text object.

	AppTextClass->SetCameraPosition(m_Position[g_NetID]->m_positionX,
		m_Position[g_NetID]->m_positionY,
		m_Position[g_NetID]->m_positionZ);

	AppTextClass->SetCameraRotation(m_Position[g_NetID]->m_rotationX,
		m_Position[g_NetID]->m_rotationY,
		m_Position[g_NetID]->m_rotationZ);
	AppTextClass->SetClockTime(astroClass->hour, astroClass->minute);

	AppTextClass->SetLightDirection(m_Light->m_lightDirection.x, m_Light->m_lightDirection.y , m_Light->m_lightDirection.z );

	// LIGHT: Get fade (real Sun Position): Show Debug Info

	return fadeLight;
}


//#############################################################################################################
// RENDER - 3D
//#############################################################################################################

void ApplicationClass::AppRender(UINT monitorWindow, float fadeLight)
{
	WomaDriverClass* m_Driver = DXsystemHandle->m_Driver;
	// BASICS: page 21: / 22 / 23
	// --------------------------------------------------------------------------------------------
	m_Driver->SetRasterizerState(CULL_BACK, FILL_SOLID); //(CULL_NONE, FILL_WIRE);

	if (RENDER_PAGE >= 23 && RENDER_PAGE < 27)
	{
		CalculateLightRayVertex(SunDistance);							// Calculate Light Source Position
		m_lightRayModel->UpdateDynamic(m_Driver, m_LightVertexVector);	// Update LightRay vertex(s)
		m_lightRayModel->Render(m_Driver);								// Render LightRay
	}

	if (RENDER_PAGE >= 21)
		DemoRender();

	// 3D STATIC OBJECTS
	// --------------------------------------------------------------------------------------------

	// 3D WATER
	// --------------------------------------------------------------------------------------------
	// ...

	// BEFORE 2D: Render TRANSPARENT Parts of 3D OBJs (like: glass window, etc...)
	// --------------------------------------------------------------------------------------------

	// 3D MESH OBJECTS
	// ...
	//THE "OTHER" NETWORK PLAYERS
}


//#define fadeSpeed 0.0005f
  #define fadeSpeed 0.00025f

// INTRO
float ApplicationClass::WOMA_APPLICATION_IntroRender(INT64 passedTotalTime)
{
	static bool FadeIn = true;
	static float fade = 0.01f;

	if (FadeIn)
		fade = fade + fadeSpeed * dt;
	else
		fade = fade - fadeSpeed * dt;

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
		WOMA_LOGManager_DebugMSG("SpriteScreenToShow: %d\n", SpriteScreenToShow);
	}

	return fade;
}


float ApplicationClass::WOMA_APPLICATION_DemoRender(INT64 passedTotalTime)
{
	static bool FadeIn = true;
	static float fade = 0;

	if (fade == 0)
	{
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
		fade = fade + fadeSpeed * dt;
	else
		fade = fade - fadeSpeed * dt;

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
			m_Position[g_NetID]->m_positionX = SystemHandle->AppSettings->INIT_CAMX;
			m_Position[g_NetID]->m_positionY = SystemHandle->AppSettings->INIT_CAMY;
			m_Position[g_NetID]->m_positionZ = SystemHandle->AppSettings->INIT_CAMZ;

			if (RENDER_PAGE == 28)
				FORCE_RENDER_ALL = true;
			else
				FORCE_RENDER_ALL = false;
		}
		else {
			Publish_Quit_Message(); //END of DEMO!
		}

		WOMA_LOGManager_DebugMSG("RENDER_PAGE: %d\n", RENDER_PAGE);
	}

	return fade;
}
