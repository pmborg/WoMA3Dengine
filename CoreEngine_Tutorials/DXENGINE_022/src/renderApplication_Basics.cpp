// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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
		static float rY = 0.0f;
		rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!

		// Rotate the world matrix by the rotation value so that the Square will spin:
		m_1stSquar3DColorModel->translation(0, 0, 0);
		m_1stSquar3DColorModel->rotateY(rY);
		m_1stSquar3DColorModel->Render(SystemHandle->m_Driver);
	}

	//DEMO-2:
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
	if (RENDER_PAGE >= 22)
	{
		m_1stTriangleTextureVertexModel->Render(SystemHandle->m_Driver);
	}

//LIGHT TUTORIAL DEMO:

	//CUBE TUTORIAL DEMO:



}

void DemoApplicationClass::DemoPosRender()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(UINT monitorWindow)
//-------------------------------------------------------------------------------------------
{
	// Process INPUT & CAMERA Render:
	float dayLightFade = Update(monitorWindow, SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);

	// 45 Render: SHADOWS	Render one Application Frame, TODO: process these 2 in paralell!?
#if defined USE_SHADOW_MAP
	AppPreRender(dayLightFade);
#endif

	// RENDER: MAIN - 3D, Render one Application Frame
	if (RENDER_PAGE >= 15) //OLD:20 now 15 to allow FADE BANNERS on INTRO_DEMO
		AppRender(monitorWindow, dayLightFade);

	// RENDER: SPRITEs on TOP of 3D - 2D Render one Application Frame. 26 - (Need to be after 3D)
}



float ApplicationClass::Update(UINT monitorWindow, WomaDriverClass* m_Driver)
{
	float fadeLight = 1;

	// GET INPUT for CAMERA: Movement

	// Animate Camera (INTRO_DEMO) before???: HandleUserInput

	// SET CAMERA (for this monitor): Prepare to Take a Shot: Generate the view matrix based on the camera's position.
	if (SystemHandle->windowsArray.size() == 3)
	{
		// TODO: settings.xml define: LEFT/RIGTH: Monitor
		// Monitors Index:
		// | 1 | 0 | 2 |

		switch (SystemHandle->AppSettings->DRIVER)
		{
		#if defined DX9sdk
			case DRIVER_DX9:
				// Give 90 Degress for all 3 Monitors:
				if (monitorWindow == 1) ((DirectX::DX9Class*)m_Driver)->m_Camera->m_rotationY -= (90 / 3); //  90:3 = 30deg
				if (monitorWindow == 0) ((DirectX::DX9Class*)m_Driver)->m_Camera->m_rotationY -= 0;
				if (monitorWindow == 2) ((DirectX::DX9Class*)m_Driver)->m_Camera->m_rotationY += (90 / 3);	//  90:3 = 30deg
			break;
		#endif
		#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
			case DRIVER_DX9:
				// Give 90 Degress for all 3 Monitors:
				if (monitorWindow == 1) ((DirectX::DX11Class*)m_Driver)->m_Camera->m_rotationY -= (90 / 3); //  90:3 = 30deg
				if (monitorWindow == 0) ((DirectX::DX11Class*)m_Driver)->m_Camera->m_rotationY -= 0;
				if (monitorWindow == 2) ((DirectX::DX11Class*)m_Driver)->m_Camera->m_rotationY += (90 / 3);	//  90:3 = 30deg
			break;
		#endif

			case DRIVER_DX11:
				// Give 90 Degress for all 3 Monitors:
				if (monitorWindow == 1) ((DirectX::DX11Class*)m_Driver)->m_Camera->m_rotationY -= (90 / 3); //  90:3 = 30deg
				if (monitorWindow == 0) ((DirectX::DX11Class*)m_Driver)->m_Camera->m_rotationY -= 0;
				if (monitorWindow == 2) ((DirectX::DX11Class*)m_Driver)->m_Camera->m_rotationY += (90 / 3);	//  90:3 = 30deg
			break;

		#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
			case DRIVER_DX12:
				// Give 90 Degress for all 3 Monitors:
				if (monitorWindow == 1) ((DirectX::DX12Class*)m_Driver)->m_Camera->m_rotationY -= (90 / 3); //  90:3 = 30deg
				if (monitorWindow == 0) ((DirectX::DX12Class*)m_Driver)->m_Camera->m_rotationY -= 0;
				if (monitorWindow == 2) ((DirectX::DX12Class*)m_Driver)->m_Camera->m_rotationY += (90 / 3);	//  90:3 = 30deg
			break;
		#endif
		}
	}

	//CAMERA_RENDER(m_Camera);	// ((GLOpenGLClass*)m_Driver)->m_Camera->Render(); || ((DX_CLASS*)m_Driver)->m_Camera->Render();
	switch (SystemHandle->AppSettings->DRIVER)
	{
	#if defined DX9sdk
		case DRIVER_DX9:
			if (((DirectX::DX9Class*)m_Driver)->m_Camera)
				((DirectX::DX9Class*)m_Driver)->m_Camera->Render();
		break;
	#endif
	#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX9:
			if (((DirectX::DX11Class*)m_Driver)->m_Camera)
				((DirectX::DX11Class*)m_Driver)->m_Camera->Render();
		break;
	#endif
		case DRIVER_DX11:
			if (((DirectX::DX11Class*)m_Driver)->m_Camera)
				((DirectX::DX11Class*)m_Driver)->m_Camera->Render();
		break;
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			if (((DirectX::DX12Class*)m_Driver)->m_Camera)
				((DirectX::DX12Class*)m_Driver)->m_Camera->Render();
		break;
	#endif
		case DRIVER_GL3:
			if (((GLopenGLclass*)m_Driver)->m_Camera)
				((GLopenGLclass*)m_Driver)->m_Camera->Render();
		break;
	}

	// CONSTRUCT: FRUSTRUM

	// CAMERA TEXT: Show Debug Info

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
	SystemHandle->m_Driver->BeginScene(monitorWindow);	// Clear the buffers to begin the scene (glClear|ClearRenderTargetView/ClearDepthStencilView)
	

	SystemHandle->m_Driver->SetRasterizerState(CULL_BACK, FILL_SOLID);
	//SystemHandle->m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);

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
	/*
		#if defined USE_LIGHT_RAY
		if (RENDER_PAGE == 23)
		{
			CalculateLightRayVertex(SunDistance);											// Calculate Light Source Position
			m_lightRayModel->UpdateDynamic(SystemHandle->m_Driver, m_LightVertexVector);	// Update LightRay vertex(s)
			m_lightRayModel->Render(SystemHandle->m_Driver);								// Render LightRay
		}
		#endif
*/

	// RENDER: CLOUDS
	// --------------------------------------------------------------------------------------------
	// ...

	// --------------------------------------------------------------------------------------------
	// [0] TERRAIN: UNDER WATER!
	// --------------------------------------------------------------------------------------------
	SystemHandle->m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
	//m_Driver->SetRasterizerState(CULL_NONE, FILL_WIRE);

	// [1] WATER:
	// --------------------------------------------------------------------------------------------

	// [2] Render MAIN Terrain Here
	// --------------------------------------------------------------------------------------------

	// BASICS: page 21: / 22 / 23
	// --------------------------------------------------------------------------------------------
	SystemHandle->m_Driver->SetRasterizerState(CULL_BACK, FILL_SOLID);
	//SystemHandle->m_Driver->SetRasterizerState(CULL_NONE, FILL_WIRE);

	SystemHandle->demoApplicationClass->DemoRender();

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