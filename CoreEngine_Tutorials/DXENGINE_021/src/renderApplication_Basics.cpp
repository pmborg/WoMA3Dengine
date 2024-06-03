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
//WomaIntegrityCheck = 1234567831;

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

	//COLOR TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------
	//DEMO-1: Square
	if (RENDER_PAGE == 21 || FORCE_RENDER_ALL)
	{
		m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID); // Render the Inside of Sphere
		float rY = 0.0f;
		rY = dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!

	#if defined ROTATE_SQUARE
		// Rotate the world matrix by the rotation value so that the Square will spin:
		m_1stSquare3DColorModel->translation(0, 0, 0);
		m_1stSquare3DColorModel->rotateY(rY);
	#endif
		m_1stSquare3DColorModel->Render(m_Driver);
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

	//LIGHT TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------

	//CUBE TUTORIAL DEMO:
	// --------------------------------------------------------------------------------------------

	// SPHEREs
	// --------------------------------------------------------------------------------------------

}

void ApplicationClass::RenderSprites()
{
  WomaDriverClass* m_Driver = DXsystemHandle->m_Driver;

}

// PRE-RENDER - Shadows
#if defined USE_SHADOW_MAP
void ApplicationClass::AppPreRender(UINT monitorWindow, WomaDriverClass* Driver, float fadeLight)
{
	// TODO: List all objects in front of camera with SHADOW!!!!!!!!!!!

	//RENDER TO TEXTURE:
	if (fadeLight > 0.1f) {
		m_RenderTexture->SetRenderTarget(/*Context*/Driver);							// Set the render target to be the render to texture.
		m_RenderTexture->ClearRenderTarget(/*Context*/Driver, 1.0f, 1.0f, 1.0f, 1.0f);	// Clear the render to texture!

		// Get the view and orthographic matrices from the light object.
		lightViewMatrix = &(m_Light->m_viewMatrix);
		ShadowProjectionMatrix = &(m_Light->m_orthoMatrix);

		// 3D STATIC OBJECTS SHADOWS: RENDER/PROCESS SHADOWS!
		// --------------------------------------------------------------------------------------------
	}

	//RENDER TO SCREEN:
	((DirectX::DX11Class*)Driver)->SetBackBufferRenderTarget(monitorWindow);
}
#endif

float ApplicationClass::Update(UINT monitorWindow, WomaDriverClass* m_Driver)
{
	float fadeLight = 1;

	// TIME Control: Show Debug Info
	INT64 passedTotalTime = (INT64)((SystemHandle->m_Timer.currentTime - SystemHandle->m_Timer.m_startEngineTime) / SystemHandle->m_Timer.m_ticksPerMs);	// To control events in time (DEMO)

	// GET INPUT for CAMERA: Movement

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

	// CONSTRUCT: FRUSTRUM

	// CAMERA TEXT: Show Debug Info

	// LIGHT: Get fade (real Sun Position): Show Debug Info

	return fadeLight;
}


// INTRO


/////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(UINT monitorWindow, WomaDriverClass* driver)
//-------------------------------------------------------------------------------------------
{
	if (RENDER_PAGE < 15)
	{
		Update(monitorWindow, driver);
		return;
	}
	else
	{
		// [0] Process INPUT, CAMERA, INTRO animation etc...
		float dayLightFade = Update(monitorWindow, driver);
		if (dayLightFade == -100) return;

		// [1] Render: SHADOWS - Render one Application Frame (Need to be before 3D)

		// [2] Render: MAIN - 3D, Render one Application Frame
		AppRender(monitorWindow, dayLightFade);

		// [3] Render: MAIN - 2D (SPRITEs on TOP of 3D) Render one Application Frame. (Need to be after 3D)
	}
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

	// DEBUG SPRITE: Shadows
	// --------------------------------------------------------------------------------------------

	if (RENDER_PAGE >= 21)
		DemoRender();

	//#############################################################################################################-
	// RENDER:
	//#############################################################################################################

	// RENDER: CLOUDS
	// --------------------------------------------------------------------------------------------
	// ...

	// --------------------------------------------------------------------------------------------
	// [0] TERRAIN: UNDER WATER!
	// --------------------------------------------------------------------------------------------

	// [1] WATER:
	// --------------------------------------------------------------------------------------------

	// [2] Render MAIN Terrain Here
	// --------------------------------------------------------------------------------------------

	m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);

	// 3D STATIC OPAC OBJECTS
	// --------------------------------------------------------------------------------------------

	// 3D MESH OBJECTS
	// ...

	//THE "OTHER" NETWORK PLAYERS

	// 3D WATER
	// --------------------------------------------------------------------------------------------
	// ...

	// BEFORE 2D: Render TRANSPARENT Parts of 3D OBJs (like: glass window, etc...)
	// --------------------------------------------------------------------------------------------
}
