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

#include "DemoApplicationClass.h"

void DemoApplicationClass::DemoRender()
{
//COLOR TUTORIAL DEMO:
	//DEMO-1:
	if (RENDER_PAGE == 21)
	{
		float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!

		// Rotate the world matrix by the rotation value so that the Square will spin:
		m_1stSquar3DColorModel->translation(0, 0, 0);
		m_1stSquar3DColorModel->rotateY(rY);
		m_1stSquar3DColorModel->Render(SystemHandle->m_Driver);
	}

	//DEMO-2:
	if (RENDER_PAGE <= 25)
		m_1stTriangle3DColorModel->Render(SystemHandle->m_Driver);

//TEXTURE TUTORIAL DEMO:
	//DEMO-1:
	if (RENDER_PAGE == 22)
	{
		m_bmp3DModel->Render(SystemHandle->m_Driver);
		m_jpg3DModel->Render(SystemHandle->m_Driver);
		m_png3DModel->Render(SystemHandle->m_Driver);
		m_tif3DModel->Render(SystemHandle->m_Driver);
		m_dds3DModel->Render(SystemHandle->m_Driver);
	#if defined SUPPORT_TGA
		m_tga3DModel->Render(SystemHandle->m_Driver);
	#endif
	}

	//DEMO-2:
	if (RENDER_PAGE >= 22 && RENDER_PAGE <= 25)
	{
		m_1stTriangleTextureVertexModel->Render(SystemHandle->m_Driver);
	}

//LIGHT TUTORIAL DEMO:
	//DEMO-1
	if (RENDER_PAGE == 23)
	{
		float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.015f / 16.66f);		// MOVIMENT FORMULA!
		m_3th3DModel1->rotateX(rY);
		m_3th3DModel1->translation(0, 0, 1);
		m_3th3DModel1->Render(SystemHandle->m_Driver);
	}

	//DEMO-2
	if (RENDER_PAGE <= 25)
		m_3th3DModel2->Render(SystemHandle->m_Driver);

	//CUBE TUTORIAL DEMO:
	//DEMO-1
	if (RENDER_PAGE == 25 && m_cube1Model)
	{
		float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube1Model->rotateY(rY);
		m_cube1Model->translation(-2, -0.5f, 1);
		m_cube1Model->Render(SystemHandle->m_Driver);
	}

	//DEMO-2
	if (RENDER_PAGE == 25 && m_cube2Model)
	{
		float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube2Model->rotateY(rY);
		m_cube2Model->translation(1.5f, -0.5f, 1);
		m_cube2Model->Render(SystemHandle->m_Driver);
	}

	//DEMO-3
	if (RENDER_PAGE == 25 && m_cube3Model)
	{
		SystemHandle->m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube3Model->rotateY(rY);
		m_cube3Model->translation(5, -0.5f, 1);
		m_cube3Model->Render(SystemHandle->m_Driver);
	}
	if (RENDER_PAGE == 26 && m_SphereModel1)
	{
		SystemHandle->m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_SphereModel1->rotateY(rY);
		m_SphereModel1->translation(-4, 2, 1);
		m_SphereModel1->Render(SystemHandle->m_Driver);
	}
	if (RENDER_PAGE == 26 && m_SphereModel2)
	{
		SystemHandle->m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_SphereModel2->rotateY(rY);
		m_SphereModel2->translation(4, 2, 1);
		m_SphereModel2->Render(SystemHandle->m_Driver);
	}
}

void DemoApplicationClass::DemoPosRender()
{
	SystemHandle->m_Driver->SetRasterizerState(CULL_BACK, FILL_SOLID); //(CULL_NONE, FILL_WIRE);

	SystemHandle->m_Driver->TurnOnAlphaBlending();	// BANNER: Have Transparent Alfa color, so use it!

	if (RENDER_PAGE >= 24 && m_titleModel)
		m_titleModel->RenderSprite(SystemHandle->m_Driver, (SystemHandle->AppSettings->WINDOW_WIDTH - m_titleModel->SpriteTextureWidth) / 2, (SystemHandle->AppSettings->WINDOW_HEIGHT - m_titleModel->SpriteTextureHeight) / 2);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(UINT monitorWindow)
//-------------------------------------------------------------------------------------------
{
	SystemHandle->m_Driver->BeginScene(monitorWindow);	// Clear the buffers to begin the scene (glClear|ClearRenderTargetView/ClearDepthStencilView)

	// Process INPUT & CAMERA Render:
	float dayLightFade = Update(monitorWindow, SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);

	// 45 Render: SHADOWS	Render one Application Frame, TODO: process these 2 in paralell!?

	// RENDER: MAIN - 3D, Render one Application Frame
	if (RENDER_PAGE >= 15)
		AppRender(monitorWindow, dayLightFade);

	// RENDER: SPRITEs on TOP of 3D - 2D Render one Application Frame. 26 - (Need to be after 3D)
	if (RENDER_PAGE >= 15)
		AppPosRender();
}

//#############################################################################################################
// POS-RENDER - 2D: Render TRANSPARENT Parts of 3D OBJs (like: glass window, etc...)
//#############################################################################################################
void ApplicationClass::AppPosRender()
{
	SystemHandle->m_Driver->ClearDepthBuffer();		// BANNER: On Top of 3D Rendered

	SystemHandle->demoApplicationClass->DemoPosRender();

	AppTextClass->Render();

}

float ApplicationClass::Update(UINT monitorWindow, WomaDriverClass* m_Driver)
{
	float fadeLight = 1;

	// GET INPUT for CAMERA: Movement

	// Animate Camera (INTRO_DEMO)

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
		if (DXsystemHandle->m_Camera)
			DXsystemHandle->m_Camera->Render();

	// CONSTRUCT: FRUSTRUM

	// CAMERA TEXT: Show Debug Info
	AppTextClass->SetFps(SystemHandle->fps);						// Update the FPS "Value" in the text object.
	AppTextClass->SetCpu(SystemHandle->m_Cpu.GetCpuPercentage());	// Update the CPU usage "Value" in the text object.

	AppTextClass->SetCameraPosition(DXsystemHandle->m_Camera->m_positionX,
		DXsystemHandle->m_Camera->m_positionY,
		DXsystemHandle->m_Camera->m_positionZ);

	AppTextClass->SetCameraRotation(DXsystemHandle->m_Camera->m_rotationX,
		DXsystemHandle->m_Camera->m_rotationY,
		DXsystemHandle->m_Camera->m_rotationZ);
	AppTextClass->SetClockTime(astroClass->hour, astroClass->minute);

	AppTextClass->SetLightDirection(m_Light->m_lightDirection.x, m_Light->m_lightDirection.y , m_Light->m_lightDirection.z );

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
	SystemHandle->m_Driver->SetRasterizerState(CULL_BACK, FILL_SOLID); //(CULL_NONE, FILL_SOLID);

	// DEBUG SPRITE: Shadows
	// --------------------------------------------------------------------------------------------

	//#############################################################################################################-
	// RENDER:
	//#############################################################################################################
	// RENDER: SKY
	// --------------------------------------------------------------------------------------------
	SystemHandle->m_Driver->TurnOffAlphaBlending();

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
	SystemHandle->m_Driver->SetRasterizerState(CULL_BACK, FILL_SOLID); //(CULL_NONE, FILL_WIRE);

	SystemHandle->demoApplicationClass->DemoRender();

	if (RENDER_PAGE >= 23 && RENDER_PAGE < 27)
	{
		CalculateLightRayVertex(SunDistance);											// Calculate Light Source Position
		m_lightRayModel->UpdateDynamic(SystemHandle->m_Driver, m_LightVertexVector);	// Update LightRay vertex(s)
		m_lightRayModel->Render(SystemHandle->m_Driver);								// Render LightRay
	}

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
