// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: renderApplication_Basics.cpp
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
//WomaIntegrityCheck = 1234567222;

#include "OSengine.h"
#pragma warning(disable : 4267) // warning C4267: 'initializing': conversion from 'size_t' to 'UINT', possible loss of data
#include "OSmain_dir.h"
#include "ApplicationClass.h"
#include "SceneManager.h"

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
extern RApplicationClass* r_Application;
#endif

#if defined DX9sdk
#include "Dx9Class.h"
#endif
#if defined DX11 || defined DX9
#include "Dx11Class.h"
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif
#if (defined OPENGL3 || defined OPENGL4)
#include "WomaDriverClass.h"	//woma
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif

#include "DXmodelClass.h"

//-------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(UINT monitorWindow, WomaDriverClass* driver)
//-------------------------------------------------------------------------------------------
{
	// [1] Process INPUT, CAMERA & Animations:
	// --------------------------------------------------------------------------------------------

	// [2] SceneManager: Process/Filter and Create Lists/trees of objects to render from: WORLD.XML
	// --------------------------------------------------------------------------------------------
#if defined USE_SCENE_MANAGER && (defined DX_ENGINE)
	SceneManager::GetInstance()->opacModelList.clear();				//Reset list of opac objects
	SceneManager::GetInstance()->shadowModelList.clear();			//Reset list of cast shadow objects
	SceneManager::GetInstance()->Render();							//Create Lists of objects to render from WORLD.XML
#endif

	// [3] LIGHT RAY:
	// --------------------------------------------------------------------------------------------
#if defined USE_LIGHT_RAY
	if (RENDER_PAGE >= 23)
	{
		CalculateLightRayVertex(SunDistance);					// Calculate Light Source Position

#if defined USE_LIGHT_RAY
		m_lightRayModel->UpdateDynamic(m_LightVertexVector);	// Update LightRay vertex(s)
		m_lightRayModel->Render();								// Render LightRay
#endif
	}
#endif

	// [4] Render one Screen:
	// --------------------------------------------------------------------------------------------
	AppPreRender(monitorWindow, driver, dayLightFade);	// [1] Render: SHADOWS 1st...

	AppRender(monitorWindow, dayLightFade);				// [2] Render: All 3D!!!

	AppPosRender();										// [3] Render: All 2D (on TOPs)
}

void ApplicationClass::AppPreRender(UINT monitorWindow, WomaDriverClass* Driver, float fadeLight)
{
	UINT len = (UINT)SceneManager::GetInstance()->opacModelList.size();

#if defined USE_SHADOW_MAP	// LATER: List all objects in front of camera with SHADOWs!
	if (len > 0)
	{
		//RENDER SHADOWS TO TEXTURE:
		if (fadeLight > 0.1f)
		{
			m_RenderShadowTexture->SetRenderTarget(Driver);								// Set the render target to be the render to texture.
			m_RenderShadowTexture->ClearRenderTarget(Driver, 1.0f, 1.0f, 1.0f, 1.0f);	// Clear the render to texture!

	#if defined  USE_LIGHT_RAY && defined USE_SHADOW_MAP
			m_Light->GenerateViewMatrix(MyLightVertexVector[1].x / 100, MyLightVertexVector[1].y / 100, MyLightVertexVector[1].z / 100);
	#endif

			// RENDER SHADOWS for all these 3D STATIC OBJECTS:
			// --------------------------------------------------------------------------------------------
	#if defined USE_SCENE_MANAGER && (defined DX_ENGINE)
			// OPAC Parts:
			SHADER_TYPE shader_type = SHADER_AUTO;
			for (UINT id = 0; id < len; id++)
			{
				shader_type = objModel[id]->ModelShaderType;
				if (shader_type != SHADER_TEXTURE_LIGHT_RENDERSHADOW &&
					shader_type != SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED &&
					shader_type != SHADER_NORMAL_BUMP_INSTANCED)
					RenderModel(monitorWindow, Driver, id, (UINT)PASS_SHADOWS);
			}
	#endif
		}
	}
#endif

	//MANDATORY! Back to Normal: From now on RENDER TO main buffer SCREEN:
	((DirectX::DX11Class*)Driver)->SetBackBufferRenderTarget(monitorWindow);

	m_Driver->TurnOnAlphaBlending(); // Re-assume default
}

void ApplicationClass::RenderModel(UINT monitorWindow, WomaDriverClass* driver, UINT modelID, UINT pass)
{
	VirtualModelClass* model = objModel[modelID];
	((DXmodelClass*)model)->m_worldMatrix = XMMatrixIdentity();

	{
		float rx = SystemHandle->xml_loader.theWorld[model->m_ObjId].rotX;
		if (rx == -1000) {
			static float rX = 0.0f;
			rX = (float)dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
			model->rotateX(rX);
		}
		else
			model->rotateX(rx);

		float ry = SystemHandle->xml_loader.theWorld[model->m_ObjId].rotY;
		if (ry == -1000) {
			static float rY = 0.0f;
			rY += (float)dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
			model->rotateY(rY);
		}
		else
			model->rotateY(ry);

		float rz = SystemHandle->xml_loader.theWorld[model->m_ObjId].rotZ;
		if (rz == -1000) {
			static float rZ = 0.0f;
			rZ = (float)dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
			model->rotateZ(rZ);
		}
		else
			model->rotateZ(rz);
	}

	model->translation(0, 0, 0);

	float scale = SystemHandle->xml_loader.theWorld[model->m_ObjId].scale;
	model->scale(scale, scale, scale);

	model->translation(SystemHandle->xml_loader.theWorld[model->m_ObjId].posX,
		SystemHandle->xml_loader.theWorld[model->m_ObjId].translateY,
		SystemHandle->xml_loader.theWorld[model->m_ObjId].posZ);

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	model->Render(CAMERA_NORMAL, PROJECTION_PERSPECTIVE, pass, &(m_Light->m_viewMatrix), &(m_Light->m_ligth_orthoMatrix));// Pass 2 (Shadow));
#else
	model->Render(CAMERA_NORMAL, PROJECTION_PERSPECTIVE, pass);// Pass 2 (Shadow));
#endif
}

#define TERRAIN_SCALE 1
//#############################################################################################################
// [2/3] RENDER - 3D
//#############################################################################################################
void ApplicationClass::AppRender(UINT monitorWindow, float fadeLight)
{
	SystemHandle->TotalVertexCounter = 0;
	#if LEVEL >= 10 && LEVEL <= 21
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
	// --------------------------------------------------------------------------------------------
#if defined USE_SKY_CAMERA_DOME && defined USE_SKYSPHERE	// Render world "Sky": Centered SKY Camera:
	if (RENDER_PAGE >= 28 && m_SkyModel)
	{
		m_Driver->SetRasterizerState(CULL_NONE/*CULL_BACK*/, FILL_SOLID); // Render the Inside of Sphere
		m_SkyModel->translation(0, 0, 0);
		m_SkyModel->scale(20, 20, 20);
		m_SkyModel->RenderSky(CAMERA_SKY); // Cant Reach: (CAMERA_SKY)
	}
#endif

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending();
#endif

	if ((RENDER_PAGE >= 21) && (RENDER_PAGE < 30))
		DemoRender();	// ALL Demos!: page 21: / 22 / 23 / ... 49

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

	// --------------------------------------------------------------------------------------------
	// [0] TERRAIN: UNDER WATER!
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN || defined SCENE_MAIN_TERRAIN
#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
#endif

	if (RENDER_PAGE == 49)
		m_Model[0]->RenderWithFade();					// New function to replace these 2 line options.
#endif

	// [2] Render MAIN Terrain Here
	// --------------------------------------------------------------------------------------------
#if defined SCENE_MAIN_TOPO_TERRAIN && !defined USE_TERRAIN_ALFA_MAP
	static bool fog = (RENDER_PAGE == 51 || RENDER_PAGE >= 60) ? true : false;
	if (RENDER_PAGE >= 50)
	{
		if (m_Model[2])
			m_Model[2]->RenderWithFade(fadeLight, fog);	// New function to replace these 2 line options.
	}
#endif

	// 3D STATIC OPAC OBJECTS
	// --------------------------------------------------------------------------------------------
#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
#endif
#if defined USE_SCENE_MANAGER && (defined DX_ENGINE)
	UINT size = SceneManager::GetInstance()->opacModelList.size();
	for (UINT id = 0; id < size; id++)
		RenderModel(monitorWindow, m_Driver, id, PASS_OPAC); //eq: objModel[id]->Render(m_Driver, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_OPAC);
#endif

	//THE "OTHER" NETWORK PLAYERS
	//----------------------------------------------------------------------------------------------------------------------

	// TRANSPARENT:
	// --------------------------------------------------------------------------------------------
	m_Driver->TurnOnAlphaBlending();

	// [1] WATER:
	// --------------------------------------------------------------------------------------------

	// Render TRANSPARENT Parts of 3D OBJs (like: glass window, etc...) (last part)
	// --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 33 && defined USE_SCENE_MANAGER
	for (UINT id = 0; id < SceneManager::GetInstance()->opacModelList.size(); id++)
		objModel[id]->Render(CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_TRANSPARENT);
#endif
}//#############################################################################################################

//#############################################################################################################
// [3/3] POS-RENDER - 2D: Render TRANSPARENT Parts of 3D OBJs (like: glass window, etc...)
//#############################################################################################################
void ApplicationClass::AppPosRender()
{
	//WomaDriverClass* m_Driver = SystemHandle->m_Driver;
#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending();
#endif
	m_Driver->ClearDepthBuffer();

#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
#endif

	//float rescale = 1;

#if defined USE_TITLE_BANNER 
	if ((RENDER_PAGE >= 24 && m_titleModel) && (WOMA::game_state != GAME_MAP)) //Dont render on main map
	{
		int X = ((SystemHandle->AppSettings->WINDOW_WIDTH - m_titleModel->SpriteTextureWidth) / 2);
		int Y = 10;
		m_titleModel->RenderSprite(X, Y, rescale);
	}
#endif

#if defined USE_MAIN_MAP || defined USE_MINI_MAP
	RenderMainMapMiniMap();
#endif

#if defined USE_IMGUI
	if (RENDER_PAGE >= 24 && m_iconSettings)
	{
		float rescale = 1;
		int X = 2;
		int Y = 2;
		m_iconSettings->RenderSprite(X, Y, rescale);
	}
	m_Driver->ClearDepthBuffer();
#endif

	// RENDER RASTERTEK V1 FONT:
	// -------------------------
#if defined USE_RASTERTEK_TEXT_FONT
#if !defined INTRO_DEMO //|DEMO|
	if (RENDER_PAGE >= 27)
		AppTextClass->Render();
#else
	if (RENDER_PAGE == 27)
		AppTextClass->Render();
#endif
#endif

	// RENDER RASTERTEK V2 FONT:
	// -------------------------
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
	// Do the frame processing for the application object.
	if (!r_Application->Frame(NULL))
	{
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP;
	}
#endif

	// PRESENTATION SPRITE(s)  - On Top of 3D Rendered stuff:
	// ---------------------------------------------------------------
#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOffAlphaBlending(); // Re assume default
#endif
#if defined INTRO_DEMO //RenderDemoIntroSprites
	RenderDemoIntroSprites();
#endif

	// RENDER NATIVE TEXT:
	// -------------------
#if defined USE_DX10DRIVER_FONTS
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

	//m_Driver->ClearDepthBuffer();		// Force BANNER: On Top of 3D Rendered
	m_Driver->RenderDriverText();
#endif

}

float ApplicationClass::Update()
{
	float fadeLight = 1;

#if defined USE_TIMER_CLASS
	// TIME Control: Show Debug Info
	UINT64 passedTotalTime = (UINT64)((SystemHandle->m_Timer.currentTime - SystemHandle->m_Timer.m_startEngineTime) / SystemHandle->m_Timer.m_ticksPerMs);	// To control events in time (DEMO)

	if (m_Driver->RenderfirstTime)
	{
		TCHAR tmp[MAX_STR_LEN]; _stprintf(tmp, TEXT("PASSED TOTAL TIME TO LOAD: %ju ms\n"), passedTotalTime); OutputDebugString(tmp);
#if defined SAVEW3D
		WomaMessageBox(TEXT("Conversion from OBJ to W3D, ended."), TEXT("SAVEW3D"));
		WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP;
		return -100;
#endif
	}
#endif

#if defined INTRO_DEMO //|DEMO  (RENDER_PAGE < 15)|
	// 5 INTRO DEBUG TEXT: Show time, etc..
	if (RENDER_PAGE < 21) {
		if (m_Driver->RenderfirstTime) {
			TCHAR tmp[MAX_STR_LEN]; _stprintf(tmp, TEXT("WOMA_APPLICATION_IntroRender(%ju)\n"), passedTotalTime); OutputDebugString(tmp);
		}
		fadeIntro = WOMA_APPLICATION_IntroRender(passedTotalTime);
	}
	else
		fadeIntro = 1;

	if (m_Driver->RenderfirstTime) {
		TCHAR tmp[MAX_STR_LEN]; _stprintf(tmp, TEXT("WOMA_APPLICATION_DemoRender(%ju)\n"), passedTotalTime); OutputDebugString(tmp);
	}
	WOMA_APPLICATION_DemoRender(passedTotalTime);

	if (RENDER_PAGE < 15)
		return 0;

#if defined USE_DIRECT_INPUT && defined INTRO_DEMO
	// Animate Camera (INTRO_DEMO)
	SystemHandle->m_player[g_NetID]->p_player.IsDownPressed = true;
#endif
#endif

	//--------------------------------------------------------------------------------------------
	// DETECT COLISIONS: Get the closest Compound object/(s):
	//--------------------------------------------------------------------------------------------
#if defined CHECK_COMPOUND_COLISION
	// and what about a MOUSE PICK ? (left key?):
	/////////////////////////////////////////////////////
	anyMouseClickToPick();

	float X = 0.0f, Z = 0.0f;
	float camX = m_Position[g_NetID]->m_positionX;
	float camZ = m_Position[g_NetID]->m_positionZ;
	UINT N_COMPOUNDS = SceneManager::GetInstance()->opacModelList.size();

	//for (UINT id = 0; id < SceneManager::GetInstance()->opacModelList.size(); id++)
	//	RenderModel(monitorWindow, m_Driver, id, PASS_OPAC); //objModel[id]->Render(m_Driver, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_OPAC);

	for (UINT c = 0; c < N_COMPOUNDS; c++)
	{
		UINT id = compoundTreeLoadingOrder[c].compoundTreeId;
		X = objModel[id]->PosX - camX; //compound[id].posX
		Z = objModel[id]->PosZ - camZ; //compound[id].posZ
		compoundTreeLoadingOrder[c].order = (UINT)(X * X + Z * Z);
	}

	qsort(compoundTreeLoadingOrder, N_COMPOUNDS, sizeof(compoundTreeLoadOrder), CompoundSortCB);	// Order compound by distance:

	// [Colision 1] Check Colison with with "10" COMPOUNDS near to us...:
	// --------------------------------------------------------
	static XMMATRIX bottleWorld;
	XMVECTOR prwsPos, prwsDir;

	/////////////////////////////////////////  IMPORTANT - Get the initial Ray /////////////////////////////////////////
	pickRayVector((float)SystemHandle->AppSettings->WINDOW_WIDTH / 2.0f, (float)SystemHandle->AppSettings->WINDOW_HEIGHT - 65, prwsPos, prwsDir);

	for (UINT c = 0; c < 10/*N_COMPOUNDS*/; c++) // We dont need all, right?:)
	{
		UINT i = compoundTreeLoadingOrder[c].compoundTreeId;	// This is the compound[id] to check colisions...

		if (compound[i].ready && compound[i].visibel) // Check compounds already loaded... 
		{
			closestObjDist = FLT_MAX;
			D3DX_TO_XM_MATRIX(bottleWorld, compound[i].objModel->m_world); // CONVERT MATRIXs....:/(

			closestObjDist = pick(prwsPos, prwsDir, compound[i].objModel->bottleBoundingBoxVertPosArray, compound[i].objModel->bottleBoundingBoxVertIndexArray, bottleWorld, !true);	// Use Bounding Boxes, Faster!
			if (closestObjDist < FLT_MAX)
			{
				closestObjId = i;	// Get the Closest Object ID!
				break;
			}
		}
	}

	// Calculate it with more accurance if we are really close to an object:
	if (closestObjDist >= 0 && closestObjDist <= 3) {
		closestObjDist = pick(prwsPos, prwsDir, compound[closestObjId].objModel->bottleVertPosArray, compound[closestObjId].objModel->indices, bottleWorld, true);
	}
#endif

	// [Colision 2] Billboards
	// --------------------------------------------------------

#if defined USE_DIRECT_INPUT					
	HandleUserInput(dt);	// GET INPUT for CAMERA: Movement
#endif

	// SET CAMERA (for this monitor): Prepare to Take a Shot: Generate the view matrix based on the camera's position.

#if defined DX_ENGINE	
	if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		if (DXsystemHandle->m_Camera)
			DXsystemHandle->m_Camera->CalculateViewMatrix();
	}
#endif

#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		GLopenGLclass* driver = (GLopenGLclass*)driverList[SystemHandle->AppSettings->DRIVER];
		if (driver->gl_Camera)
			driver->gl_Camera->CalculateViewMatrix();
	}
#endif

	// Render with Multi-Monitors: (setup camera)

	// [2] CAMERA SKY: Update & Prepare to Take a Shot
#if defined USE_SKYSPHERE && defined USE_SKY_CAMERA_DOME	
	if (RENDER_PAGE >= 28)
	{
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
	#if defined DX_ENGINE
			DXsystemHandle->m_CameraSKY->m_rotationX = DXsystemHandle->m_Camera->m_rotationX;
			DXsystemHandle->m_CameraSKY->m_rotationY = DXsystemHandle->m_Camera->m_rotationY;
			DXsystemHandle->m_CameraSKY->CalculateViewMatrix();
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

// CONSTRUCT: FRUSTRUM
#if defined USE_FRUSTRUM
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
		&((DX12Class*)m_Driver)->m_projectionMatrix,
		&DXsystemHandle->m_Camera->m_viewMatrix);
	#endif

	#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	m_Driver->frustum->ConstructFrustum(SystemHandle->AppSettings->SCREEN_DEPTH / 2.5f,
		&((DX11Class*)m_Driver)->m_projectionMatrix, 
		&DXsystemHandle->m_Camera->m_viewMatrix);
	#endif

	#if (defined OPENGL3 || defined OPENGL4)
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
#endif

	//Update Sun and Moon position:
#if defined USE_ASTRO_CLASS
if (!astroClass) {
	WOMA_APPLICATION_InitGUI();
}
#endif

// CAMERA TEXT: Show Debug Info
#if defined USE_RASTERTEK_TEXT_FONT							

if (AppTextClass) {
#if defined EXTRA_INFO
	AppTextClass->SetClockTime(astroClass->hour, astroClass->minute);
	AppTextClass->SetLightDirection(m_Light->m_lightDirection.x, m_Light->m_lightDirection.y, m_Light->m_lightDirection.z);
#endif
	AppTextClass->SetFps(SystemHandle->fps);						// Update the FPS "Value" in the text object.

#if !defined TEXT_TEST
#if defined WINDOWS_PLATFORM && !defined WIN_XP
	AppTextClass->SetCpu(SystemHandle->m_Cpu.GetCpuPercentage());	// Update the CPU usage "Value" in the text object.
#endif
#endif

#if defined USE_DIRECT_INPUT			// we will use DX input method
	AppTextClass->SetCameraPosition(m_Position[g_NetID]->m_positionX,
		m_Position[g_NetID]->m_positionY,
		m_Position[g_NetID]->m_positionZ);

	AppTextClass->SetCameraRotation(m_Position[g_NetID]->m_rotationX,
		m_Position[g_NetID]->m_rotationY,
		m_Position[g_NetID]->m_rotationZ);
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

#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER
	AppTextClass->SetRenderCount(SceneManager::GetInstance()->quadTree.totalVertexRendered,
		SceneManager::GetInstance()->quadTree.totalRendered,
		(UINT)SystemHandle->xml_loader.theWorld.size());
#endif
}
#endif

// LIGHT: Get fade (real Sun Position): Show Debug Info

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
		WOMA_LOGManager_DebugMSG("SpriteScreenToShow: %d\n", SpriteScreenToShow);
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
			WOMA_LOGManager_DebugMSG("RENDER_PAGE: %d\n", RENDER_PAGE);
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
			WOMA_LOGManager_DebugMSG("STOP: WOMA_APPLICATION_DemoRender ()\n");
			WOMA::main_loop_state = -1;		//WINDOWS
			WOMA::game_state = GAME_STOP;	//LINUX
		}
	}

	return fade;
}
#endif

static float rY = 0.0f;

void ApplicationClass::DemoRender()
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
		m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID); // Render the Inside of Sphere
#endif

		if (RENDER_PAGE < 28) {
#if defined ROTATE_SQUARE
			// Rotate the world matrix by the rotation value so that the Square will spin:
			m_1stSquare3DColorModel->translation(0, -3, 0);
			m_1stSquare3DColorModel->rotateY(rY);
#endif
			m_1stSquare3DColorModel->Render();
		}

		//DEMO-2: Triangle!!{
		m_1stTriangle3DColorModel->translation(0, 8.75, -5); //>22
		m_1stTriangle3DColorModel->Render();
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
		m_bmp3DModel->Render();
#endif
#if defined USE_IMAGE_PNG
		m_png3DModel->Render();
#endif

		//Line2:
#if defined USE_IMAGE_JPG
		m_jpg3DModel->Render();
#endif
#if defined USE_IMAGE_TIFF
		m_tif3DModel->Render();
#endif

		//Line3:
#if defined USE_IMAGE_DDS
		m_dds3DModel->Render();
#endif
#if defined SUPPORT_TGA
		m_tga3DModel->Render();
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
		m_1stTriangleTextureVertexModel->Render();
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

		m_3th3DModel2->Render();
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
		m_cube1Model->Render();
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
		m_cube2Model->Render();
	}

	//DEMO-3
	//if ((RENDER_PAGE == 25 && m_cube3Model) || FORCE_RENDER_ALL)
#if defined INTRO_DEMO
	if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
#else
	if (RENDER_PAGE < 30)
#endif
	{
		m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
		float rY = 0.0f;
		rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
		m_cube3Model->rotateY(rY);
		m_cube3Model->translation(3.5, Y_pos, 1);
		m_cube3Model->Render();
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
			m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
			float rY = 0.0f;
			rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
			m_SphereModel1->rotateY(rY);
			m_SphereModel1->translation(-3.2f, -4, 5.0f);
			m_SphereModel1->Render();
		}

	if ((RENDER_PAGE == 26 || RENDER_PAGE == 28) && m_SphereModel1)
#if defined INTRO_DEMO
		if (RENDER_PAGE == 26 || FORCE_RENDER_ALL)
#else
		if (RENDER_PAGE < 30)
#endif
		{
			m_Driver->SetRasterizerState(CULL_NONE, FILL_SOLID);
			float rY = 0.0f;
			rY = (float)dt * (0.005f / 16.66f);		// MOVIMENT FORMULA!
			m_SphereModel2->rotateY(rY);
			m_SphereModel2->translation(3.2f, -4, 5.0f);
			m_SphereModel2->Render();
		}
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	if (RENDER_PAGE == 36 || RENDER_PAGE == 41 || RENDER_PAGE == 42) // Debug Shadow
		m_2nd3DModel->Render();
#endif
}

#if DX_ENGINE_LEVEL >= 21 && defined INTRO_DEMO	//29  DEMO SPRITE TEXT
void ApplicationClass::RenderDemoIntroSprites()
{
	//USING: ModelShaderType = SHADER_TEXTURE_FONT(3)/
	if (RENDER_PAGE < 21 && (SpriteScreenToShow >= 0 && SpriteScreenToShow < m_screenShots.size()))
	{
		VirtualModelClass* screenShot = m_screenShots[SpriteScreenToShow];

		//WINDOWS:
		screenShot->RenderSprite((SystemHandle->AppSettings->WINDOW_WIDTH - screenShot->SpriteTextureWidth) / 2, SystemHandle->AppSettings->WINDOW_HEIGHT / 2, 1, fadeIntro);
	}
}
#endif


#if defined CHECK_COMPOUND_COLISION //DX_ENGINE_LEVEL >= 56
// Calculate the world space pick ray from the 2d coordinates of our mouse cursor.
// ==================================================================================================================================
void ApplicationClass::pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir/*, bool mouseClick*/)
// ==================================================================================================================================
{
	int ClientWidth = SystemHandle->AppSettings->WINDOW_WIDTH; //g_ScreenWidth;
	int ClientHeight = SystemHandle->AppSettings->WINDOW_HEIGHT; //g_ScreenHeight;

	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;

	//if (mouseClick) {
		//Transform 2D pick position on screen space to 3D ray in View space

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

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	DX12Class* driver = NULL;
#endif
#if defined DX11 || defined DX9
	DX11Class* driver11 = NULL;
#endif
#if defined DX9sdk
	DX9Class* driver9 = NULL;
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		driver = (DX12Class*)Driver;
	}
#endif
#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		driver11 = (DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER];
	}
#endif
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		driver9 = (DX9Class*)Driver;
#endif

	XMMATRIX* m_projectionMatrix = NULL;
	driver11->GetProjectionMatrix(*m_projectionMatrix/*Driver, camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix*/);

	PRVecX = (((2.0f * mouseX) / ClientWidth) - 1) / m_projectionMatrix->_11;
	PRVecY = -(((2.0f * mouseY) / ClientHeight) - 1) / m_projectionMatrix->_22;
	PRVecZ = 1.0f;	//View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

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


#undef _44

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);
	//} else {
		//to use the center of the screen (client area)
		//to be the point that creates the picking ray (eg. first person shooter)
		//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//}

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;	//We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	// Convert D3DXMATRIX to XMMATRIX:
	XMMATRIX* camView = driver11->GetViewMatrix(driver11, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_OPAC, NULL /*lightViewMatrix*/, NULL/*ShadowProjectionMatrix*/);

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, *camView);	//Inverse of View Space matrix is World space matrix
	pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
}

// Calculates whether the object was picked or not | getPoligon = true (detect colision)
// ==================================================================================================================================
float ApplicationClass::pick(XMVECTOR pickRayInWorldSpacePos, XMVECTOR pickRayInWorldSpaceDir,
	// ==================================================================================================================================
	std::vector<XMFLOAT3>& vertPosArray, std::vector<DWORD>& indexPosArray, XMMATRIX& worldSpace, bool getPoligon)
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



#if defined (CHECK_COMPOUND_COLISION) //TUTORIAL_CHAP >= 96 && 
// Check if the "LEFT Mouse Button" was pressed and if there is anything to pick
// ==================================================================================================================================
void ApplicationClass::anyMouseClickToPick()
// ==================================================================================================================================
{
	static int pickWhat = 2;
	//static float pickedDist = 0.0f;
	static bool isShoot = false;
	static XMMATRIX bottleWorld;

	float tempDist;
	XMVECTOR prwsPos, prwsDir;
	int hitIndex = -1;
	float closestDist = FLT_MAX;

	if (m_Input->m_mouseState.rgbButtons[MOUSE_LEFT] & 0x80)
	{
		if (isShoot == false)
		{
			POINT mousePos;

			GetCursorPos(&mousePos);
			ScreenToClient(g_hwnd, &mousePos);

			int mousex = mousePos.x;
			int mousey = mousePos.y;
			pickRayVector((float)mousex, (float)mousey, prwsPos, prwsDir); // when we click LEFT mouse button

			// Check all COMPOUNDS Objects:
			// ----------------------------
			for (UINT i = 0; i < N_COMPOUNDS; i++)
			{
				compound[i].bottleHit = 0; //reset it
				if (compound[i].ready && compound[i].visibel) // Check compounds already loaded... 
				{
					tempDist = FLT_MAX;

					D3DX_TO_XM_MATRIX(bottleWorld, compound[i].objModel->m_world);
#ifdef zero
					if (pickWhat == 0)//(L"Bounding Sphere";)
					{
						float pRToPointDist = 0.0f; // Closest distance from the pick ray to the objects center

						XMVECTOR bottlePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
						XMVECTOR pOnLineNearBottle = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

						// For the Bounding Sphere to work correctly, we need to make sure we are testing
						// the distance from the objects "actual" center and the pick ray. We have stored
						// the distance from (0, 0, 0) in the objects model space to the object "actual"
						// center in bottleCenterOffset. So now we just need to add that difference to
						// the bottles world space position, this way the bounding sphere will be centered
						// on the object real center.
						XMVECTOR bottleCenterOffset = XMVectorSet(compound[i].objModel->bottleCenterOffset.x,
							compound[i].objModel->bottleCenterOffset.y,
							compound[i].objModel->bottleCenterOffset.z,
							compound[i].objModel->bottleCenterOffset.w);

						bottlePos = XMVector3TransformCoord(bottlePos, bottleWorld) + bottleCenterOffset;
						// This equation gets the point on the pick ray which is closest to bottlePos
						pOnLineNearBottle = prwsPos + XMVector3Dot((bottlePos - prwsPos), prwsDir) / XMVector3Dot(prwsDir, prwsDir) * prwsDir;

						// Now we get the distance between bottlePos and pOnLineNearBottle
						// This line is slightly less accurate, but it offers a performance increase by
						// estimating the distance using XMVector3LengthEst()
						//pRToPointDist = XMVectorGetX(XMVector3LengthEst(pOnLineNearBottle - bottlePos));				
						pRToPointDist = XMVectorGetX(XMVector3Length(pOnLineNearBottle - bottlePos));

						// If the distance between the closest point on the pick ray (pOnLineNearBottle) to bottlePos
						// is less than the bottles bounding sphere (represented by a float called bottleBoundingSphere)
						// then we know the pick ray has intersected with the bottles bounding sphere, and we can move on
						// to testing if the pick ray has actually intersected with the bottle itself.
						if (pRToPointDist < compound[i].objModel->bottleBoundingSphere)
						{
							// This line is the distance to the pick ray intersection with the sphere
							//tempDist = XMVectorGetX(XMVector3Length(pOnLineNearBottle - prwsPos));

							// Check for picking with the actual model now
							tempDist = pick(prwsPos, prwsDir, compound[i].objModel->bottleVertPosArray, compound[i].objModel->indices, bottleWorld);
						}
					}

					// Bounding Box picking test (L"Bounding Box";)
					if (pickWhat == 1)
						tempDist = pick(prwsPos, prwsDir, compound[i].objModel->bottleBoundingBoxVertPosArray, compound[i].objModel->bottleBoundingBoxVertIndexArray, bottleWorld);
#endif					
					// Check for picking directly with the model without bounding volumes testing first: (L"Model")
					//if(pickWhat == 2)
					tempDist = pick(prwsPos, prwsDir, compound[i].objModel->bottleVertPosArray, compound[i].objModel->indices, bottleWorld);

					if (tempDist < closestDist)
					{
						closestDist = tempDist;
						hitIndex = i; // Compund = hitIndex
					}
				}
			}

			// Check all Players, online:
			// ----------------------------
			selected_playerID = g_NetID;	// Init the other selected player as "us":

			for (UINT i = 0; i < HowManyPlayers; i++)
			{
				m_player[i]->p_player.bottleHit = false; //reset it
				if (m_player[i]->p_player.online && m_player[i]->p_player.visibel) // Check players online and visible at camera... 
				{
					tempDist = FLT_MAX;

					float posY = m_Terrain[active_terrain].getTerrainHeight(m_Position[i]->m_positionX, m_Position[i]->m_positionZ);
					m_player[i]->p_player.m_world._42 = posY;
					D3DX_TO_XM_MATRIX(bottleWorld, m_player[i]->p_player.m_world);

					// Check for picking directly with the model without bounding volumes testing first: (L"Model")
					//if(pickWhat == 2) 
					{
						BYTE meshtype = m_player[i]->p_player.meshType;
						tempDist = pick(prwsPos, prwsDir, robotMesh[meshtype].bottleVertPosArray, robotMesh[meshtype].indices, bottleWorld);
					}

					if (tempDist < closestDist)
					{
						closestDist = tempDist;
						selected_playerID = i;
						m_player[i]->p_player.bottleHit = true;
					}
				}
			}


			// Check all animals:
			// ----------------------------
			selected_animalTypeID = -1;
			selected_whatAnimalID = -1;

			for (UINT c = 0; c < animals.size(); c++) 		// For all kind of animals:
			{
				AnimalClass* animal = (AnimalClass*)animals[c];
				for (UINT i = 0; i < animal->N_LOBOS; i++) // For all animals of this type
				{
					if (animal->lobo[i].visible  /*&& !animal->lobo[i].alreadyLooted*/)
					{

						tempDist = FLT_MAX;

						D3DX_TO_XM_MATRIX(bottleWorld, animal->lobo[i].m_world);

						// Check for picking directly with the model without bounding volumes testing first: (L"Model")
						//if(pickWhat == 2) 
						{
							tempDist = pick(prwsPos, prwsDir, animal->m_objModel.bottleVertPosArray, animal->m_objModel.indices, bottleWorld);
						}

						if (tempDist < closestDist)
						{
							closestDist = tempDist;
							selected_animalTypeID = c;
							selected_whatAnimalID = i;
						}

					}
				}
			}

			// Check if we are picking up a respawnd obj:
			for (UINT i = 0; i < respawnObjectList.size(); i++)
			{
				if (!respawnObjectList[i].hide &&
					m_FrustumObj->CheckSphere(respawnObjectList[i].objModel->m_world._41, respawnObjectList[i].objModel->m_world._42, respawnObjectList[i].objModel->m_world._43,
						respawnObjectList[i].objModel->diameter * respawnObjectList[i].scale))
				{
					tempDist = FLT_MAX;
					D3DX_TO_XM_MATRIX(bottleWorld, respawnObjectList[i].objModel->m_world);

					// Check for picking directly with the model without bounding volumes testing first: (L"Model")
					tempDist = pick(prwsPos, prwsDir, respawnObjectList[i].objModel->bottleVertPosArray, respawnObjectList[i].objModel->indices, bottleWorld);

					if (tempDist < closestDist)
					{
						respawnObjectList[i].hide = true;
						closestDist = tempDist;
						selected_respawnObjectID = i;
						respawnObjectList[i].bottleHit = true;

						if (i == 0) {
							BYTE currentAMMO = m_player[g_NetID]->p_player.ammu[m_player[g_NetID]->p_player.currentWeapon];
							m_player[g_NetID]->p_player.ammu[m_player[g_NetID]->p_player.currentWeapon] = min(MAX_AMMU, max(255, currentAMMO + 50));											// Reduce 1 ammu
						}

						if (i == 1)
							m_player[g_NetID]->p_player.health = min(gameDemo.health_lvl[m_player[g_NetID]->p_player.playerlvl], m_player[g_NetID]->p_player.health + 20);											// Reduce 1 ammu

						lastTime[selected_respawnObjectID] = m_Timer.currentTime;
					}
				}
			}

			//------------------------------------------------------------------------------------------------------
			// To avoid pick more than 1 compound:
			if (closestDist < FLT_MAX)
			{
				if (hitIndex >= 0) {
					compound[hitIndex].bottleHit = 1; //warning C4701: potentially uninitialized local variable 'hitIndex' used
					CHG_COMPOUND = hitIndex;
				}
				//pickedDist = closestDist;
			}

			isShoot = true;

		}//allow press mouse?

	}//if mouse pressed...

	if (!(m_Input->m_mouseState.rgbButtons[MOUSE_LEFT] & 0x80))
	{
		isShoot = false;
	}
}
#endif
#endif
