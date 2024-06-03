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

#if defined USE_SKYSPHERE	// [2] CAMERA SKY: Update & Prepare to Take a Shot
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
#endif

	// CONSTRUCT: FRUSTRUM
#if defined USE_FRUSTRUM
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX12Class*)m_Driver)->m_projectionMatrix,
			&DXsystemHandle->m_Camera->m_viewMatrix);
#endif

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX11Class*)m_Driver)->m_projectionMatrix,
			&DXsystemHandle->m_Camera->m_viewMatrix);

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) {

		mat4 glPrjMatrix = ((GLopenGLclass*)m_Driver)->m_projectionMatrix;
		XMMATRIX m_projectionMatrix = XMMatrixSet
		(
			glPrjMatrix.m[0], glPrjMatrix.m[1], glPrjMatrix.m[2], glPrjMatrix.m[3],
			glPrjMatrix.m[4], glPrjMatrix.m[5], glPrjMatrix.m[6], glPrjMatrix.m[7],
			glPrjMatrix.m[8], glPrjMatrix.m[9], glPrjMatrix.m[10], glPrjMatrix.m[11],
			glPrjMatrix.m[12], glPrjMatrix.m[13], glPrjMatrix.m[14], glPrjMatrix.m[15]
		);

		mat4 glvMatrix = ((GLopenGLclass*)m_Driver)->gl_Camera->m_viewMatrix;
		XMMATRIX m_viewMatrix = XMMatrixSet
		(
			glvMatrix.m[0], glvMatrix.m[1], glvMatrix.m[2], glvMatrix.m[3],
			glvMatrix.m[4], glvMatrix.m[5], glvMatrix.m[6], glvMatrix.m[7],
			glvMatrix.m[8], glvMatrix.m[9], glvMatrix.m[10], glvMatrix.m[11],
			glvMatrix.m[12], glvMatrix.m[13], glvMatrix.m[14], glvMatrix.m[15]
		);

		m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&m_projectionMatrix,
			&m_viewMatrix);
	}
#endif

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

	// DEBUG SPRITE: Shadows
	// --------------------------------------------------------------------------------------------

	//#############################################################################################################-
	// RENDER:
	//#############################################################################################################

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
