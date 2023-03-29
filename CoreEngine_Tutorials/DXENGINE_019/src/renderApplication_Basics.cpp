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

#include "platform.h"
#include "dxWinSystemClass.h"

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

/////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(UINT monitorWindow)
//-------------------------------------------------------------------------------------------
{
	SystemHandle->m_Driver->BeginScene(monitorWindow);	// Clear the buffers to begin the scene (glClear|ClearRenderTargetView/ClearDepthStencilView)

	// Process INPUT & CAMERA Render:
	float dayLightFade = Update(monitorWindow, SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);

	// 45 Render: SHADOWS	Render one Application Frame, TODO: process these 2 in paralell!?
#if defined USE_SHADOW_MAP
	AppPreRender(dayLightFade);
#endif

	// RENDER: MAIN - 3D, Render one Application Frame
	if (RENDER_PAGE >= 15)
		AppRender(monitorWindow, dayLightFade);

	// RENDER: SPRITEs on TOP of 3D - 2D Render one Application Frame. 26 - (Need to be after 3D)
}

float ApplicationClass::Update(UINT monitorWindow, WomaDriverClass* m_Driver)
{
	float fadeLight = 1;

	// GET INPUT for CAMERA: Movement

	// Animate Camera (INTRO_DEMO)

	// SET CAMERA (for this monitor): Prepare to Take a Shot: Generate the view matrix based on the camera's position.

	// CONSTRUCT: FRUSTRUM
#if defined USE_FRUSTRUM
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		((DX_CLASS*)m_Driver)->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX_CLASS*)m_Driver)->m_projectionMatrix,
			&((DX_CLASS*)m_Driver)->m_Camera->m_viewMatrix);
#endif

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		((DX_CLASS*)m_Driver)->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&((DX_CLASS*)m_Driver)->m_projectionMatrix,
			&((DX_CLASS*)m_Driver)->m_Camera->m_viewMatrix);

#if defined OPENGL3 && defined NOT_IMPLEMENTED
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		((GLOpenGLClass*)m_Driver)->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
			&((GLOpenGLClass*)m_Driver)->m_projectionMatrix,
			&((GLOpenGLClass*)m_Driver)->m_Camera->m_viewMatrix);
#endif
#endif

	// CAMERA TEXT: Show Debug Info

	// TIME Control: Show Debug Info


	// LIGHT: Get fade (real Sun Position): Show Debug Info

	return fadeLight;
}


extern float SunDistance;

//#############################################################################################################
// RENDER - 3D
//#############################################################################################################
//#define ClearColor SceneManager::GetInstance()->RootNode->nodeState.ClearColor
//#define ClearColor SystemHandle->m_Application->ClearColor

void ApplicationClass::AppRender(UINT monitorWindow, float fadeLight)
{

	// DEBUG SPRITE: Shadows
	// --------------------------------------------------------------------------------------------
#if defined USE_SHADOW_MAP //&& defined _DEBUG
	DirectX::DXmodelClass* model = (DirectX::DXmodelClass*)m_2nd3DModel;
	if (RENDER_PAGE >= 45)
		model->meshSRV[0] = m_RenderTexture->m_shaderResourceView;
#endif

	//#############################################################################################################-
	// RENDER:
	//#############################################################################################################
	// RENDER: SKY
	// --------------------------------------------------------------------------------------------
#if defined USE_SKY2D
	if (RENDER_PAGE >= 27 && RENDER_PAGE < 30) // At: 27 | 28 | 29
		m_Sky2DModel->RenderSprite(m_Driver, (SystemHandle->AppSettings->WINDOW_WIDTH - m_Sky2DModel->SpriteTextureWidth) / 2,
			(SystemHandle->AppSettings->WINDOW_HEIGHT - m_Sky2DModel->SpriteTextureHeight) / 2);
	m_Driver->ClearDepthBuffer(); // Need to Be Right after: m_Sky2DModel->RenderSprite 
#else
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

	// BASICS: page 21: / 22 / 23
	// --------------------------------------------------------------------------------------------

	// 3D STATIC OBJECTS
	// --------------------------------------------------------------------------------------------

	// 3D WATER
	// --------------------------------------------------------------------------------------------
	// ...

	// BEFORE 2D: Render TRANSPARENT Parts of 3D OBJs (like: glass window, etc...)
	// --------------------------------------------------------------------------------------------

	// 3D MESH OBJECTS
	// ...
	// TODO procedure
}
