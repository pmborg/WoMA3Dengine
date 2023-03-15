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

#if defined USE_SKYSPHERE	// [2] CAMERA SKY: Update & Prepare to Take a Shot
	if (RENDER_PAGE >= 30) {
		((DX_CLASS*)m_Driver)->m_CameraSKY->m_rotationX = ((DX_CLASS*)m_Driver)->m_Camera->m_rotationX;
		((DX_CLASS*)m_Driver)->m_CameraSKY->m_rotationY = ((DX_CLASS*)m_Driver)->m_Camera->m_rotationY;
		CAMERA_RENDER(m_CameraSKY);
	}
#endif

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
  #if defined USE_SKYSPHERE
		if (RENDER_PAGE >= 30)				//30: SKY
			Render_SKY_SUN_MOON(fadeLight); //34: SUN_MOON
  #else
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
  #endif
#endif

	// RENDER: CLOUDS
	// --------------------------------------------------------------------------------------------
	// ...

	// --------------------------------------------------------------------------------------------
	// [0] TERRAIN: UNDER WATER!
	// --------------------------------------------------------------------------------------------

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN || defined SCENE_MAIN_TERRAIN
	if (RENDER_PAGE >= 24)
	{
		//m_Model[0]->translation(-terrain_squares*2, 0, -terrain_squares*2); // Scale from 0,0 to 2048,2048 --> -1024,-1024 to 1024,1024
		m_Model[0]->RenderWithFade(m_Driver, fadeLight);					// New function to replace these 2 line options.
	}
#endif

	// [1] WATER:
	// --------------------------------------------------------------------------------------------
#if defined SCENE_WATER_TERRAIN //60
	if (RENDER_PAGE >= 60)
	{

		//m_Model[1]->translation(-terrain_squares * 2, 0, -terrain_squares * 2); // Scale from 0,0 to 2048,2048 --> -1024,-1024 to 1024,1024
		m_Model[1]->RenderWithFade(m_Driver, fadeLight);						// New function to replace these 2 line options.

	}
#endif

	// [2] Render MAIN Terrain Here
	// --------------------------------------------------------------------------------------------
#if defined SCENE_MAIN_TOPO_TERRAIN
	if (RENDER_PAGE >= 64)
	{
		//m_Model[2]->translation(-terrain_squares*2, 0, -terrain_squares*2); // Scale from 0,0 to 2048,2048 --> -1024,-1024 to 1024,1024
		m_Model[2]->RenderWithFade(m_Driver, fadeLight);					// New function to replace these 2 line options.
	}
#endif

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
