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
#pragma warning(disable : 4065) // warning C4065: switch statement contains 'default' but no 'case' labels

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

int __cdecl BillSortCB(const VOID* arg1, const VOID* arg2);

#include "C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Engine\\Source\\Resources\\Texture.h"
#include "LowLevel/MeshApplication.h"
#include "MyDemo.h"
#include "womamesh.h"

#if defined USE_MINIMAP_REDENRING_THREAD
#include <thread>
#include <mutex>
#include <condition_variable>

std::thread shadowWorker, minimapWorker;

std::mutex mtx;
std::condition_variable cv;

bool minimapWork = false;
bool minimapThreadDone = false;
bool stopThreads = false;

ID3D11DeviceContext* minimapCtx = nullptr;
ID3D11CommandList* minimapCmdList = nullptr;

#if defined USE_SHADOW_THREAD
bool shadowWork = false;
bool shadowThreadDone = false;
ID3D11DeviceContext* shadowCtx = nullptr;
ID3D11CommandList* shadowCmdList = nullptr;
#endif

DirectX::DX11Class* driver11Ref = nullptr;
WomaDriverClass* driverRef = nullptr;
UINT monitorRef = 0;
float fadeLightRef = 0;
#endif

extern void InitMeshDemo(ID3D11DeviceContext* pContext, ApplicationClass* app, MeshApplication* demoapp, MyDemo* demo);
extern void LoadAllMeshModels(UINT this_level, ApplicationClass* app, MeshApplication* demoapp, MyDemo* demo);
extern void RenderAllMeshModels(ID3D11DeviceContext* pContext, float fadeLight=1);

size_t AtlasobjModel_outVertsCount = 0, AtlasobjModel_outIdxCount = 0;

#if defined MAIN_RENDER_CURVED_REAL_SKY_PLANE
#include "realSkyPlaneClass.h"
extern RealSkyPlaneClass realSkyPlaneClass;
#endif

#if defined MAIN_RENDER_WATER_FALL 
#include "ParticlesystemClass.h"
extern ParticlesystemClass particlesystemClass;
#include "SmokeEmitterClass.h"
extern SmokeEmitterClass smokeEmitterClass;
#endif

//----------------------------------------------------------------------------------------------------
void ApplicationClass::RenderScene(void* mainCtx, UINT monitorIndex, WomaDriverClass* driver) // RENDER A FULL FRAME!
//----------------------------------------------------------------------------------------------------
{
	SystemHandle->TotalVertexCounter = 0;

	// UPDATE DYN. LIGHT RAY:
	// --------------------------------------------------------------------------------------------

#if DX_ENGINE_LEVEL >= 98 && defined USE_DAY_AND_NIGHT
	if(RENDER_PAGE >= 98)
		dayLightFade = 0.4f;
	else
#endif
		dayLightFade = 1; //levels < 98

#if DX_ENGINE_LEVEL >= 36 && (defined USE_MINIMAP_REDENRING_THREAD || defined USE_SHADOW_MAP || defined USE_MAIN_MAP)
																	//IF_RENDER_PAGE(RENDER_PAGE >= 29)  NOTE: (we need it before 20 at INTRO)
	AppPreRender(monitorIndex, driver, dayLightFade, mainCtx);		// [1] Launch shadow & mini-map async work, (do not wait for render on level>=91)
																	// Re-Start aux threads on this frame:
#endif

	AppRender(monitorIndex, RENDER_PAGE, dayLightFade, mainCtx);	// [2] 3D Render main scene while workers run in parallel

																	//IF_RENDER_PAGE(RENDER_PAGE >= 29) NOTE: (we need it before 20 at INTRO)
	AppPosRender(monitorIndex, RENDER_PAGE, dayLightFade, mainCtx);	// [3] 2D: Render TRANSPARENT Parts of 3D OBJs(like: "Glass windows", "Billboards", etc...)

#if DX_ENGINE_LEVEL >= 91 && defined USE_MINIMAP_REDENRING_THREAD
	// [2.5] Wait here (after main scene) before using shadows/mini-map results
	IF_RENDER_PAGE(RENDER_PAGE >= 91)
	WaitForPreRenderTasks(mainCtx);
#endif
}

void ApplicationClass::SortOutWhatNeedToBeRendered(void* pContext, WomaDriverClass* driver)
{
	totalRendered = 0;
	AtlasobjModel_outVertsCount = 0;
	AtlasobjModel_outIdxCount = 0;

	// [1] SET A SPECIFIC CAMERA POSITION FOR BILLBOARD SORT:
	// --------------------------------------------------------------------------------------------
#if defined USE_DIRECT_INPUT && DX_ENGINE_LEVEL >= 63
	const float SORT_OFFSET = 5.0f; // 5 METERS BEHIND CAMERA
	sort_cameraX -= FAST_sin(SystemHandle->m_Application->m_Position[g_NetID]->m_rotationY) * SORT_OFFSET;
	sort_cameraZ -= FAST_cos(SystemHandle->m_Application->m_Position[g_NetID]->m_rotationY) * SORT_OFFSET;
#endif

	// [2] SCENEMANAGER: PROCESS/FILTER AND CREATE LISTS/TREES OF OBJECTS TO RENDER FROM: WORLD.XML
	// --------------------------------------------------------------------------------------------
#if defined USE_SCENE_MANAGER && (defined DX_ENGINE)
	if (RENDER_PAGE >= 30)
	{
	WOMA::sceneManager->visibleModelList.clear();			//Reset list of 3D objects
	WOMA::sceneManager->visibleBillboardList.clear();		//Reset list of Billboard objects

	SystemHandle->m_Application->billboardRrenderCount = 0;
	WOMA::sceneManager->CreateLists(RENDER_PAGE);						//CREATE LISTS: for all objects to render (from WORLD.XML) and more

	world_main_size = WOMA::sceneManager->visibleModelList.size();
	}
#endif

	RunBillboardSortDemo(RENDER_PAGE, m_Trees);

	// [4] RESTORE DEFAULT CAMERA POSITION:
	// --------------------------------------------------------------------------------------------
#if defined USE_DIRECT_INPUT && DX_ENGINE_LEVEL >= 63
	sort_cameraX = SystemHandle->m_Application->m_Position[g_NetID]->m_positionX;
	sort_cameraY = SystemHandle->m_Application->m_Position[g_NetID]->m_positionY;
	sort_cameraZ = SystemHandle->m_Application->m_Position[g_NetID]->m_positionZ;
#endif

	// [5] ROTATE BILLS:
	// --------------------------------------------------------------------------------------------
	if (RENDER_PAGE >= 93)
	{
#if defined GENERATE_ATLAS_INTEGRATION_DDS
	for (UINT i = 0; i < m_Trees.size(); ++i)
	{
		const float dx = m_Trees[i].vPos.x - sort_cameraX;
		const float dz = m_Trees[i].vPos.z - sort_cameraZ;
		billangle = atan2(dx, dz) * (180 / PI);
		float ry = -billangle / (180 / PI);
		if (m_Trees[i].type == 12)       // for X cross bills to show as X and not as +
			ry -= PI * 0.25f;

		const UINT xmlid = m_Trees[i].ID + SystemHandle->m_Application->initial_world_xml_objs;
	#if defined  NO3DBILL
		if (m_Trees[i].type <= 12)  //Use all from 0..12
	#else
		if (m_Trees[i].type <= 10)
	#endif
		{
	#if _DEBUG
			if (xmlid < SystemHandle->xml_loader.theWorldXML.size())
	#endif
	#if !defined  NO3DBILL
				if (!SystemHandle->xml_loader.theWorldXML[xmlid].Bill)
	#else
				if (SystemHandle->xml_loader.theWorldXML[xmlid].Bill)
	#endif
				{
					const float dist2 = dx * dx + dz * dz;
					if (dist2 >= 9.0f || SystemHandle->xml_loader.theWorldXML[xmlid].type == 200)// >= 3m
					{
						m_Trees[i].rotY = ry;
					}
				}
	#if !defined  NO3DBILL
			m_Trees[i].rotY = ry;												// Rotate Bills 2D
	#endif
		}
		
	}

	// [6] POPULATE BILLS TO RENDER:
	//----------------------------------------------------------
		BuildBillboardAtlasMesh_FromTrees(outVerts, outIdx, m_Trees, AtlasobjModel_outVertsCount, AtlasobjModel_outIdxCount);

	if (AtlasobjModel==0) //EQ: if (m_Driver->RenderfirstTime)
	{
		CREATE_MODEL_IF_NOT_EXCEPTION(AtlasobjModel, I_AM_2D, false, false);

		if (WOMA::AppSettings->DRIVER != DRIVER_GL3)
		{
			bool resb = ((DXmodelClass*)AtlasobjModel)->LoadBillboardAtlas(
				pContext, TEXT("m_1stSquare3DColorModel"),
				driver, SHADER_BILLBOARD_ATLAS_FAST,
				NULL, //Auto
				&outVerts, &outIdx);

			ASSERT_DEBUG(resb);

			//Populate: 
			// m_vertexBuffer11
			// m_indexBuffer11
			//and assign atlas as the texture to be used

			gBillVB = ((DXmodelClass*)AtlasobjModel)->m_vertexBuffer11;
			gBillIB = ((DXmodelClass*)AtlasobjModel)->m_indexBuffer11;
			((DXmodelClass*)AtlasobjModel)->m_worldMatrix = XMMatrixIdentity();

			if (RENDER_PAGE >= 94)
				((DXmodelClass*)AtlasobjModel)->m_Shader11->VSshaderType = 127;
		}
	}

		UpdateBills((ID3D11DeviceContext*)pContext, outVerts, outIdx, AtlasobjModel_outVertsCount, AtlasobjModel_outIdxCount);

	((DXmodelClass*)AtlasobjModel)->ready = true;
#endif
	}
}

void* getvoidcontext()
{
	switch (WOMA::AppSettings->DRIVER)
	{
#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	case DRIVER_DX11:
	case DRIVER_DX9:
		return ((DX11Class*)m_Driver)->GetDeviceContext();
		break;
#endif
	}

	return NULL;
}




//
// RENDER TO TEXTURE
//
#if (defined DX_ENGINE) && (defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21 || defined USE_VIEW2D_SPRITES)
extern DXcameraClass m_CameraMINIMAP;
extern DXcameraClass m_CameraMAP;
#endif

#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
void ApplicationClass::RenderMiniMapPass(UINT monitorWindow, WomaDriverClass* Driver, void* pContext, float fadeLight, UINT ThreadID)
{
	//---------------------------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
	if (!DXsystemHandle->m_CameraMAP && m_Driver->RenderfirstTime) //m_Driver->RenderfirstTime
	{
		//Set Camera Position - Render Camera:

		//"viewMatrix": SET Camera Rotation and Position to 2D Render: TEXT and SPRITES
		m_CameraMAP.SetRotation(+89.999f, 0, 0);
		/*       /
			   /     |
			/a       | loadedTerrain[2]->m_terrainHeight/2
			--- h--- |
		Note:
			angle a = 21.8f (half of our view frustum)
		*/

		// Check which Quadrant we are:
		float Qx = (float)((int)(m_CameraMAP.m_positionX / ((loadedTerrain[2]->m_terrainWidth + 1) / 2)));
		float Qz = (float)((int)(m_CameraMAP.m_positionZ / ((loadedTerrain[2]->m_terrainHeight + 1) / 2)));
		float h = (float)(((float)(loadedTerrain[2]->m_terrainHeight / 4)) / tan(21.8f * 0.0174532925f) /* + 20*/);

		m_CameraMAP.SetPosition((float)loadedTerrain[2]->m_terrainWidth / 4 + Qx * loadedTerrain[2]->m_terrainWidth / 2,
			h,
			(float)loadedTerrain[2]->m_terrainHeight / 4 + Qz * loadedTerrain[2]->m_terrainHeight / 2);            //Note: Work with 512x512: 4 x 256x256 

		//MACRO RENDER:
#if defined DX_ENGINE	
		if (WOMA::AppSettings->DRIVER != DRIVER_GL3)
			m_CameraMAP.CalculateViewMatrix();
#endif
#if (defined OPENGL3 || defined OPENGL4)
		if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
		{
			GLopenGLclass* driver = (GLopenGLclass*)driverList[WOMA::AppSettings->DRIVER];
			if (driver->gl_Camera)
				driver->gl_Camera->Render();
		}
#endif
		// Render Water/Terrain in: to texture: m_RenderMapTexture
		m_RenderMapTexture->SetRenderTarget(Driver, (ID3D11DeviceContext*)pContext);								// Set the render target to be the render to texture.
		m_RenderMapTexture->ClearRenderTarget(Driver, (ID3D11DeviceContext*)pContext, 0.30f, 0.30f, 0.30f, 1.0f);	// Clear the render to texture!
		TerrainRender(ThreadID, monitorWindow, Driver, fadeLight, &m_CameraMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix, pContext);
	}
#endif

	//G:\DRIVE_MY_SOURCE_CODE\Dx11Engine3D\Dx11Engine3Dx64\src\terrain\Minimapclass.cpp
	//ORI: bool MiniMapClass::RenderMiniMapToTexture(ID3D11DeviceContext* pContext)
	//---------------------------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 63 && defined USE_MINI_MAP
	{
		//"viewMatrix": SET Camera Rotation and Position to 2D Render: TEXT and SPRITES
		m_CameraMINIMAP.SetRotation(+89.999f, 0, 0);
		m_CameraMINIMAP.SetPosition(sort_cameraX, 100, sort_cameraZ); // 100 Magic number

		//MACRO RENDER:
#if defined DX_ENGINE	
		if (WOMA::AppSettings->DRIVER != DRIVER_GL3)
		{
			m_CameraMINIMAP.CalculateViewMatrix();
		}
#endif
#if defined USE_MINIMAP_EXPANSION
		for (UINT id = 0; id < SystemHandle->m_Application->initial_world_xml_objs /*world_main_size - 1*/; id++)  //TODO: use sceneManager
			//for (int id = world_main_size - 1; id >= 0; --id) 
		{
			m_MiniMapBitmapTexture->SetRenderTarget(Driver, (ID3D11DeviceContext*)pContext);					// Set the render target to be the render to texture: pContext->OMSetRenderTargets
			RenderModel(pContext, RENDER_PAGE, 1, monitorWindow, m_Driver, id, PASS_OPAC, &m_CameraMINIMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix, fadeLight);
			m_RenderMapTexture->SetRenderTarget(Driver, (ID3D11DeviceContext*)pContext);						// Set the render target to be the render to texture.
			RenderModel(pContext, RENDER_PAGE, 1, monitorWindow, m_Driver, id, PASS_OPAC, &m_CameraMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix, fadeLight);
		}
#endif
		
		// Render Water/Terrain in: to texture: m_MiniMapBitmapTexture
		m_MiniMapBitmapTexture->SetRenderTarget(Driver, (ID3D11DeviceContext*)pContext);					// Set the render target to be the render to texture: pContext->OMSetRenderTargets
		m_MiniMapBitmapTexture->ClearRenderTarget(Driver, (ID3D11DeviceContext*)pContext, 0.0f, 0.0f, 0.0f, 1.0f);  // Clear the render to texture!
		TerrainRender(ThreadID, monitorWindow, Driver, fadeLight, &m_CameraMINIMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix, pContext);
	}
#endif
}
#endif

void ApplicationClass::RenderShadowPass(UINT monitorIndex, WomaDriverClass* Driver, void* pContext, float fadeLight)
{
#if defined USE_SHADOW_MAP	// LATER: List all objects in front of camera with SHADOWs!
	if (world_main_size > 0)
	{
		//RENDER SHADOWS TO TEXTURE:
		if (fadeLight > 0.1f)
		{
			m_RenderShadowTexture->SetRenderTarget(Driver, (ID3D11DeviceContext*)pContext);								// Set the render target to be the render to texture.
			m_RenderShadowTexture->ClearRenderTarget(Driver, (ID3D11DeviceContext*)pContext, 1.0f, 1.0f, 1.0f, 1.0f);	// Clear the render to texture!

#if defined  MAIN_RENDER_LIGHT_RAY && defined USE_SHADOW_MAP
			app_Light->GenerateViewMatrix(MyLightVertexVector[1].x / 100, MyLightVertexVector[1].y / 100, MyLightVertexVector[1].z / 100);
#endif

			// RENDER SHADOWS for all these 3D STATIC OBJECTS, to texture
			// --------------------------------------------------------------------------------------------
#if defined USE_SCENE_MANAGER && (defined DX_ENGINE)
		// OPAC Parts:
			SHADER_TYPE shader_type = SHADER_AUTO;
			for (UINT id = 0; id < WOMA::sceneManager->visibleModelList.size(); id++)																
			{
				if (objModel[id])
				{
				shader_type = objModel[id]->ModelShaderType;
				if (shader_type != SHADER_TEXTURE_LIGHT_RENDERSHADOW &&
					shader_type != SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED &&
					shader_type != SHADER_NORMAL_BUMP_INSTANCED)
					if (objModel[id]->ModelCastShadow)
						RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, Driver, id, (UINT)PASS_SHADOWS, NULL, NULL); // Pre-Render Shadows
				}
			}
#endif
		}
	}
#endif
}

#if defined USE_MINIMAP_REDENRING_THREAD
#if defined USE_SHADOW_THREAD
void RenderShadowsDeferred(ID3D11DeviceContext* ctx, WomaDriverClass* driver, UINT monitorIndex, float fadeLight) 
{
	// Disable alpha before rendering shadows
	m_Driver->TurnOffAlphaBlending(ctx);

	ctx->ClearState();  // optional: reset state
	SystemHandle->m_Application->RenderShadowPass(monitorIndex, driver, ctx, fadeLight);  // custom function using ctx instead of mainContext

#if defined USE_MESH_THREAD
	if (!g_GOD_MODE)
		SystemHandle->m_Application->UpdateMeshAnimations();
#endif
}
#endif

void RenderMiniMapDeferred(ID3D11DeviceContext* ctx, WomaDriverClass* driver, UINT monitorIndex, float fadeLight) {
	//ctx->ClearState();
	SystemHandle->m_Application->RenderMiniMapPass(monitorIndex, driver, ctx, fadeLight, 1); // custom function using ctx
}

void ApplicationClass::WaitForPreRenderTasks(void* ctx)
{
	ID3D11DeviceContext* mainCtx = (ID3D11DeviceContext*)ctx;

	// Wait for both workers to finish recording
	std::unique_lock<std::mutex> lock(mtx);
#if defined USE_SHADOW_THREAD
	cv.wait(lock, [&] { return shadowThreadDone && minimapThreadDone; });
#else
	cv.wait(lock, [&] { return minimapThreadDone; });
#endif

#if defined USE_SHADOW_THREAD
	// Execute command lists
	if (shadowCmdList) {
		mainCtx->ExecuteCommandList(shadowCmdList, FALSE);
		SAFE_RELEASE (shadowCmdList);
	}
#endif
	if (minimapCmdList) {
		mainCtx->ExecuteCommandList(minimapCmdList, FALSE);
		SAFE_RELEASE(minimapCmdList);
	}
}

#if defined USE_SHADOW_THREAD
void ApplicationClass::ShadowWorkerFunc() {
	while (true) {
		std::unique_lock<std::mutex> lock(mtx);

		cv.wait(lock, [&] { return shadowWork || stopThreads; });
		if (stopThreads) break;
		lock.unlock();

		//shadowCtx->ClearState(); // optional
		RenderShadowsDeferred(shadowCtx, driverRef, monitorRef, fadeLightRef);
		shadowCtx->FinishCommandList(FALSE, &shadowCmdList);

		lock.lock();
		shadowThreadDone = true;
		shadowWork = false;
		cv.notify_all();
	}
}
#endif

void ApplicationClass::MinimapWorkerFunc() {
	while (true) {
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&] { return minimapWork || stopThreads; });
		if (stopThreads) break;
		lock.unlock();

		//minimapCtx->ClearState(); // optional
		RenderMiniMapDeferred(minimapCtx, driverRef, monitorRef, fadeLightRef);
		minimapCtx->FinishCommandList(FALSE, &minimapCmdList);

		lock.lock();
		minimapThreadDone = true;
		minimapWork = false;

#if defined MAIN_RENDER_WATER_FALL
		particlesystemClass.Update();
#endif

		cv.notify_all();
	}
}
#endif

void ApplicationClass::AppPreRender(UINT monitorIndex, WomaDriverClass* Driver, float fadeLight, void* mainCtx)
{
	DirectX::DX11Class* m_driver11 = (DirectX::DX11Class*)Driver;

#if defined USE_MINIMAP_REDENRING_THREAD
	//FIRE AND FORGET:
	monitorRef = monitorIndex;
	driverRef = Driver;
	fadeLightRef = fadeLight;

	// Start worker tasks (threads will pick up work)
	{
		std::unique_lock<std::mutex> lock(mtx);
		minimapWork = true;
		minimapThreadDone = false;

  #if defined USE_SHADOW_THREAD
		shadowWork = true;
		shadowThreadDone = false;
  #endif
	}

	cv.notify_all(); //Re-Start aux threads on this frame
  #if !defined USE_SHADOW_THREAD
	//WAIT FOR RENDER:
	RenderShadowPass(monitorIndex, Driver, mainCtx, fadeLight);

	((DirectX::DX11Class*)Driver)->SetBackBufferRenderTarget(mainCtx, monitorIndex);	//MANDATORY! Back to default back buffer

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(mainCtx);												// restore default blending
#endif
  #endif
#else
#if defined USE_SHADOW_MAP
	RenderShadowPass(monitorIndex, Driver, mainCtx, fadeLight);

	((DirectX::DX11Class*)Driver)->SetBackBufferRenderTarget(mainCtx, monitorIndex);	//MANDATORY! Back to default back buffer

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(mainCtx);												// restore default blending
#endif
#endif

	// === RENDER MAP and MINIMAP TO TEXTURE: ===										 
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP && defined MAIN_RENDER_TERRAIN // Render MAP and MINI-MAP, to texture
	RenderMiniMapPass(monitorIndex, Driver, mainCtx, fadeLight);

	((DirectX::DX11Class*)Driver)->SetBackBufferRenderTarget(mainCtx, monitorIndex);	//MANDATORY! Back to default back buffer

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(mainCtx);												// restore default blending
#endif
#endif
#endif
}

void ApplicationClass::RenderModel(void* pContext, UINT level, UINT threadID, UINT monitorIndex, WomaDriverClass* driver, UINT ID,
	UINT pass, XMMATRIX* m_viewMatrix, XMMATRIX* m_projectionMatrix, float fadeLight, UINT op)
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
		else if (pass == PASS_SHADOWS) {
			if (WOMA::sceneManager->visibleModelList.size() == 0)
				return;
			modelID = WOMA::sceneManager->visibleModelList[ID]->m_ObjId;
		}
		else if (pass == PASS_BILL) {
			if (SystemHandle->m_Application->billboardRrenderCount+WOMA::sceneManager->visibleBillboardList.size() == 0)
				return;
			modelID = ID;
			pass = PASS_OPAC;
		}
		else {
			ASSERT(0); //we should never get here!
		}
	}


	DXmodelClass* model = NULL;
	if (threadID>0)
		model = (DXmodelClass*)objModel_minimap[modelID];
	else
		model = (DXmodelClass*)objModel[modelID];

	if (!model || !model->ready)
		return; // Model not ready to render (come back later!)

	//----------------------------------------------------------------------------------------------------------------------------------
    float positionX, positionY, positionZ;
    positionX = SystemHandle->xml_loader.theWorldXML[modelID].posX;
    positionY = SystemHandle->xml_loader.theWorldXML[modelID].translateY;
    positionZ = SystemHandle->xml_loader.theWorldXML[modelID].posZ;

	if (SystemHandle->xml_loader.theWorldXML[modelID].type == 401)	//STREET_LAMP
	{
		if (threadID == 1)
			return; //minimap
		if (op == 0)
		{
			SystemHandle->xml_loader.theWorldXML[modelID].rotY = 90*3;
		}
		if (op == 1)
		{
			positionX += 3;
			SystemHandle->xml_loader.theWorldXML[modelID].rotY = 90;
		}
	}

#if defined USE_AABB_COLISION_CHECK
    static const float padding = 0.1f;
    DXmodelClass* dxModel = (DXmodelClass*)model;
    dxModel->UpdateWorldAABB();

    const XMFLOAT3& min = dxModel->worldMinVertex;
    const XMFLOAT3& max = dxModel->worldMaxVertex;

    if ((dxModel->m_instanceCount == 0) &&
        !m_Driver->frustum->CheckAABB(
            min.x - padding, min.y - padding, min.z - padding,
            max.x + padding, max.y + padding, max.z + padding))
    {
        dxModel->visible = false;
        return;
    }
#else
#endif

   ((DXmodelClass*)model)->visible = true;

	// === SET AUDIO DISTANCE (IF ITS THE CASE) ===											   
    // Set the initial position of the listener to be in the middle of the scene.
#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D //SOUND3D
    SoundClass* audioEffect = SystemHandle->xml_loader.theWorldXML[modelID].audio;
    if (audioEffect)
        if (audioEffect->m_listener)
            audioEffect->m_listener->SetPosition(sort_cameraX,
                sort_cameraY,
                sort_cameraZ, DS3D_IMMEDIATE); //NOTE: All updates should be: DS3D_DEFERRED
#endif

	// === RESET WORLD MATRIX ===
	// ------------------------------------------------------------------------------------------------------------------------------
    if (m_Driver->RenderfirstTime || (SystemHandle->xml_loader.theWorldXML[model->m_ObjId].rotY != 0 && modelID > world_xml_objs))
        ((DXmodelClass*)model)->m_worldMatrix = XMMatrixIdentity();

#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D //SOUND3D
    if (SystemHandle->xml_loader.theWorldXML[modelID].depend == -1 ||
        SystemHandle->xml_loader.theWorldXML[modelID].meshSRV ||
        SystemHandle->xml_loader.theWorldXML[model->m_ObjId].Bill ||
        SystemHandle->xml_loader.theWorldXML[model->m_ObjId].type == 12) //cross bush
#else
    if (SystemHandle->xml_loader.theWorldXML[modelID].meshSRV)
#endif
        ((DXmodelClass*)model)->m_worldMatrix = XMMatrixIdentity();

	// === RESET TRANSLATION ===
	// ------------------------------------------------------------------------------------------------------------------------------
    model->translation(0, 0, 0);

	// === SET SCALE ===
	// ------------------------------------------------------------------------------------------------------------------------------
	
    if (SystemHandle->xml_loader.theWorldXML[model->m_ObjId].type == 300) //FENCE 3D
    { 
        model->scale(1, 2, 1);
    }
    else
    {
#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D //SOUND3D
    if ((m_Driver->RenderfirstTime) || SystemHandle->xml_loader.theWorldXML[modelID].depend == -1 || (SystemHandle->xml_loader.theWorldXML[modelID].meshSRV) || 
        SystemHandle->xml_loader.theWorldXML[model->m_ObjId].Bill)
#else
    if ((m_Driver->RenderfirstTime) || (SystemHandle->xml_loader.theWorldXML[modelID].meshSRV))
#endif
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
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Instancing
    if (((DXmodelClass*)model)->m_instanceCount == 0)
#endif
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
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D //SOUND3D
        if ((SystemHandle->xml_loader.theWorldXML[model->m_ObjId].meshSRV || 
            SystemHandle->xml_loader.theWorldXML[model->m_ObjId].Bill))
	#else
        if (SystemHandle->xml_loader.theWorldXML[model->m_ObjId].meshSRV)
	#endif
        {
            if (SystemHandle->xml_loader.theWorldXML[model->m_ObjId].Bill)
            {
                float cameraPositionX = sort_cameraX;
                float cameraPositionZ = sort_cameraZ;

                // Calculate distance in XZ plane
                float dx = positionX - cameraPositionX;
                float dz = positionZ - cameraPositionZ;
                float distance = FAST_sqrt(dx * dx + dz * dz);

                if (distance >= 3.0f) {
                    billangle = atan2(dx, dz) * (180 / PI);
                    ry = billangle / (180 / PI);
                    if (SystemHandle->xml_loader.theWorldXML[model->m_ObjId].type == 12)
                        ry -= PI / 4;
                    SystemHandle->xml_loader.theWorldXML[modelID].ry = ry; //save it!
                }
                else
                    ry = SystemHandle->xml_loader.theWorldXML[modelID].ry;
            } else
                ry = SystemHandle->xml_loader.theWorldXML[model->m_ObjId].rotY;
        }
        else
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
    }// non-Instancing END

	// === Adjust current OBJ. height in MAP for WORLD POSITION: ===
	// ------------------------------------------------------------------------------------------------------------------------------
	if (SystemHandle->xml_loader.theWorldXML[model->m_ObjId].moveUp)
	{ 
		positionY += (model->maxVertex.y - model->minVertex.y)/2* model->m_worldMatrix.r[1].m128_f32[1]; //#define _22 r[1].m128_f32[1]
	}
    model->translation(positionX, positionY, positionZ);

	// === RENDER OBJ.: ===					   
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	((DXmodelClass*)model)->m_Shader11->PSfade = fadeLight;
	if (m_viewMatrix == NULL && m_projectionMatrix == NULL)
		model->Render(pContext, threadID, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, pass, &(app_Light->m_viewMatrix), &(app_Light->m_ligth_orthoMatrix));	// RENDER
    else
		model->Render(pContext, threadID, CAMERA_NORMAL, PROJECTION_MINIMAP, pass, m_viewMatrix, m_projectionMatrix);									// RENDER MINI MAP
#else
	model->Render(pContext, threadID, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, pass);
#endif

	totalRendered++; //One done, next...
}

extern MeshApplication* demoapp_;
extern MyDemo* demo_;

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

void ApplicationClass::WaterTerrain(UINT monitorWindow, float fadeLight, void* pContext)
{
	//----------------------------------------------------------------------------------------------------------------------
	// TERRAIN[0]: UNDER WATER
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_REALEARTH_TERRAIN || defined SCENE_MAIN_TERRAIN
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
}

//#############################################################################################################
// [2/3] RENDER - 3D
//#############################################################################################################
void ApplicationClass::AppRender(UINT monitorIndex, UINT level, float fadeLight, void* pContext)
{
#if (defined USE_SKY_CAMERA_DOME && defined USE_SKYSPHERE) && defined MAIN_RENDER_SKY
	RenderMainSky(monitorIndex, fadeLight, pContext);
#endif
#if (DX_ENGINE_LEVEL >= 19 && DX_ENGINE_LEVEL < 50) || defined INTRO_DEMO
	if ((RENDER_PAGE >= 19) && (RENDER_PAGE < 30) || RENDER_PAGE == 36)
		DemoRender(pContext);	// ALL Demos!: page 21: / 22 / 23 / ... 49
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	if (RENDER_PAGE == 36 || RENDER_PAGE == 41 || RENDER_PAGE == 42 || RENDER_PAGE == 99) // Debug Shadow
		m_2nd3DModel->Render(pContext, 0, 0, 0, NULL, NULL);
#endif

#if defined MAIN_RENDER_TERRAIN
	if (RENDER_PAGE >= 49)
		WaterTerrain(monitorIndex, fadeLight, pContext);
#endif

	if (RENDER_PAGE >= 93)
	{
  #if defined GENERATE_ATLAS_INTEGRATION_DDS && defined MAIN_RENDER_MAIN_XML_OBJ
	#if defined USE_ALPHA_BLENDING
		m_Driver->TurnOffAlphaBlending(pContext);
	#endif
	if (RENDER_PAGE >= 93)
	{
		if (AtlasobjModel && ((DXmodelClass*)AtlasobjModel)->ready)
		{
			((DXmodelClass*)AtlasobjModel)->model_fade = fadeLight;
			((DXmodelClass*)AtlasobjModel)->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_BILL, &(app_Light->m_viewMatrix), &(app_Light->m_ligth_orthoMatrix));
		}
	}
  #endif
	}

	// 3D STATIC OPAC OBJECTS on WORLD.XML, that listed in: sceneManager->visibleModelList (in front of camera)
	//----------------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 73 && defined BILLBOARD_FOR_WINDY_GRASS && defined MAIN_RENDER_MAIN_XML_OBJ
  #if defined SCENE_BILLBOARDS
	static float lasttime = 0;
	IF_RENDER_PAGE(RENDER_PAGE >= 73)
	{
		shadergrassframeTime += (timeGetTime() - lasttime) / 200;
		if (shadergrassframeTime >= PI * 2)
			shadergrassframeTime = 0;
		lasttime = (float)timeGetTime();
	}
  #endif
#endif

#if DX_ENGINE_LEVEL == 34 || DX_ENGINE_LEVEL == 40 || DX_ENGINE_LEVEL == 41 || DX_ENGINE_LEVEL == 42 || defined INTRO_DEMO
	IF_RENDER_PAGE(RENDER_PAGE == 34 || RENDER_PAGE == 40 || RENDER_PAGE == 41 || RENDER_PAGE == 42)
		m_Driver->TurnOffAlphaBlending(pContext);
#endif
#if defined USE_RASTERIZER_STATE
	m_Driver->SetRasterizerState(pContext, CULL_NONE, FILL_SOLID);	//Set Default
#endif
	// Render TRANSPARENT Parts of 3D OBJs (like: glass window of (Space Compound), etc...) (last part)
	// --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER && defined MAIN_RENDER_MAIN_XML_OBJ //MAIN-RENDER: MAIN OBJs. (9 ms)
	IF_RENDER_PAGE(RENDER_PAGE >= 30)
	{
		for (UINT id = 0; id < WOMA::sceneManager->visibleModelList.size(); id++)
		{
			switch (SystemHandle->xml_loader.theWorldXML[WOMA::sceneManager->visibleModelList[id]->m_ObjId].type)
			{
			case 401:
				IF_RENDER_PAGE(RENDER_PAGE >= 98)
				{
					RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, m_Driver, id, PASS_OPAC, NULL, NULL, fadeLight, 0);
					RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, m_Driver, id, PASS_OPAC, NULL, NULL, fadeLight, 1);
				}
				break;
			default:
				if (objModel[id])
				{
					RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, m_Driver, id, PASS_OPAC, NULL, NULL, fadeLight, 0);
					if (id == 0 && ((DXmodelClass*)objModel[id])->obj3d.hasTransparent == true)
					{
						m_Driver->TurnOnAlphaBlending(pContext);
						IF_RENDER_PAGE(RENDER_PAGE >= 98)
							((DXmodelClass*)objModel[id])->m_Shader11->PSfade = fadeLight;
						objModel[id]->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_TRANSPARENT, NULL, NULL);
						m_Driver->TurnOffAlphaBlending(pContext);
					}
				}
				break;
			}
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
	m_lightRayModel->UpdateDynamic(pContext, &MyLightVertexVector);
	m_lightRayModel->Render(pContext);
#endif

	// Render Animated meshes:
	// -----------------------
#if defined MAIN_RENDER_ASSIMP
	if (ShouldDrawUI(monitorIndex) && !g_GOD_MODE)
	{
	
		if (RENDER_PAGE == 79 || RENDER_PAGE >= 82) //NO: IF_RENDER_PAGE
		{
			m_Driver->TurnOffAlphaBlending(pContext);
		#if !defined USE_MESH_THREAD
			UpdateMeshAnimations();
		#endif
			RenderAllMeshModels((ID3D11DeviceContext*)pContext, fadeLight);
		}
	}
#endif

	// TRANSPARENT and SEMI-TRANSPARENT:
	// --------------------------------------------------------------------------------------------
#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(pContext);
#endif

  #if defined MAIN_RENDER_WATER_FALL
	IF_RENDER_PAGE(RENDER_PAGE >= 97)
	{
		if (m_Driver->frustum->CheckSphere(142, 15, 57, MAX(1, model_waterfall->boundingSphere) * 4))
		{
			particlesystemClass.Render(pContext, fadeLight); //AQUI
			smokeEmitterClass.Update(dt / 1000.0f);
			smokeEmitterClass.Render(pContext, fadeLight);
		}
	}
  #endif

	// TERRAIN[1]: Render Mesh for WATER:
// --------------------------------------------------------------------------------------------
#if (DX_ENGINE_LEVEL >= 50 && defined SCENE_WATER_TERRAIN) && defined MAIN_RENDER_WATER //MAIN-RENDER: WATER (0.3 ms)
	IF_RENDER_PAGE(RENDER_PAGE >= 50)
	{
		DXshaderClass* shader11 = ((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->m_Shader11;
		shader11->watertime += (float)dt * (0.0025f / 16.66f);
		if (shader11->watertime > 2 * PI)
			shader11->watertime -= (2 * PI);

		m_TerrainModel[WATER_TERRAIN_ID]->translation(0, -0.75, 0);
		m_TerrainModel[WATER_TERRAIN_ID]->scale(5, 5, 5);
		m_TerrainModel[WATER_TERRAIN_ID]->RenderWithFade(pContext, dayLightFade, false);
	}
#endif


//	IN THE END:

	#if defined MAIN_RENDER_CURVED_REAL_SKY_PLANE
	IF_RENDER_PAGE(RENDER_PAGE >= 96)
	{
		((DX11Class*)driverList[WOMA::AppSettings->DRIVER])->EnableSecondBlendState();
		realSkyPlaneClass.Render(pContext);
	}
	#endif
}

//#############################################################################################################
// [3/3] POS-RENDER - 2D: Render TRANSPARENT Parts of 3D OBJs (like: "Glass windows", "Billboards", etc...)
//#############################################################################################################
void ApplicationClass::AppPosRender(UINT monitorIndex, UINT level, float dayLightFade, void* pContext)
{
    //=============================================================================================================
    // LIGHT: Get fade (real Sun Position): Show Debug Info

#if defined USE_ALPHA_BLENDING
	m_Driver->TurnOnAlphaBlending(pContext);
#endif

#if DX_ENGINE_LEVEL >= 92 && defined MAIN_RENDER_BILLBOARDS
	UINT obj_id;
	if (RENDER_PAGE >= 60)
	{
		for (UINT tree_id = 0; tree_id < WOMA::sceneManager->visibleBillboardList.size(); tree_id++)
		{
			#if DX_ENGINE_LEVEL >= 94 && defined USE_TREE_POINTER
			obj_id = WOMA::sceneManager->visibleBillboardList[tree_id]->ID + SystemHandle->m_Application->initial_world_xml_objs;
			if (SystemHandle->xml_loader.theWorldXML[obj_id].render)
				RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, m_Driver, obj_id, PASS_BILL, NULL, NULL, dayLightFade);    // Render: "Billboards"
			#else
			obj_id = WOMA::sceneManager->visibleBillboardList[tree_id].ID + SystemHandle->m_Application->initial_world_xml_objs;
			if (SystemHandle->xml_loader.theWorldXML[obj_id].render)
				RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, m_Driver, obj_id, PASS_BILL, NULL, NULL, dayLightFade);    // Render: "Billboards"
			#endif
		}
	}
#else
#if (TUTORIAL_CHAP >= 60 && defined SCENE_BILLBOARDS && defined USE_SCENE_MANAGER && defined DX_ENGINE) && defined MAIN_RENDER_BILLBOARDS // MAIN-RENDER: BILLBOARD + FENCES + FIRE (11.4 ms)
        UINT obj_id;
        for (UINT tree_id = 0; tree_id < m_Trees.size(); tree_id++)
        {
            obj_id = m_Trees[tree_id].ID + world_xml_objs;
            if (SystemHandle->xml_loader.theWorldXML[obj_id].render)											// TODO: use sceneManager
				RenderModel(pContext, RENDER_PAGE, 0, monitorIndex, m_Driver, obj_id, PASS_BILL, NULL, NULL, dayLightFade);  // Render: "Billboards"
		}
#endif
#endif

#if DX_ENGINE_LEVEL >= 24 && defined MAIN_RENDER_TITLE
		if (ShouldDrawUI(monitorIndex))
			RenderHUD_Logo(pContext);
#endif

#if (defined USE_MAIN_MAP || defined USE_MINI_MAP) && defined MAIN_RENDER_MINIMAP //MAIN-RENDER: MINI-MAP (0.4)
	if (RENDER_PAGE >= 62)
		if (ShouldDrawUI(monitorIndex))
		RenderMainMapMiniMap(pContext);
#endif

	// === AppTextClass-Fill: ===
#if defined USE_RASTERTEK_TEXT_FONT && defined MAIN_RENDER_RASTERTEK_FONT

	if (ShouldDrawUI(monitorIndex) && AppTextClass)
	{
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
		if (WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX9)
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
		if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
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
		if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
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
	if (ShouldDrawUI(monitorIndex) && RENDER_PAGE >= 27)
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
	if ((RENDER_PAGE >= 22) && (m_Driver->m_sCapabilities.USE_DXDRIVER_FONTSBoolean) && (WOMA::AppSettings->DRIVER == DRIVER_DX11))
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
	}

	m_Driver->RenderDriverText(pContext);
	}
#endif

}

// =============================================================================================
// FUNCTION: DetectCollisions()
// Purpose : Isolated collision detection for compounds and optional map editor picking.
// =============================================================================================
#if DX_ENGINE_LEVEL >= 78 && defined CHECK_OBJ_COLISION
void ApplicationClass::DetectCollisions()
{
	//--------------------------------------------------------------------------------------------
	// DETECT COLISIONS: Get the closest Compound object/(s):
	//--------------------------------------------------------------------------------------------

#if DX_ENGINE_LEVEL >= 89 && defined USE_MAP_EDITOR // EDIT Compound...
	// and what about a MOUSE PICK ? (left key?):
	/////////////////////////////////////////////////////
	IF_RENDER_PAGE(RENDER_PAGE >= 89)
		anyMouseClickToPick();
#endif

	float X = 0.0f, Z = 0.0f;
	float camX = m_Position[g_NetID]->m_positionX;
	float camZ = m_Position[g_NetID]->m_positionZ;

	for (UINT c = 0; c < world_main_size; c++)
	{
		int id = WOMA::sceneManager->visibleModelList[c]->xmlId;

		if (objModel[id])
		{
			X = objModel[id]->PosX - camX; //compound[id].posX
			Z = objModel[id]->PosZ - camZ; //compound[id].posZ
		}
		else
		{
			X = 0.0f;
			Z = 0.0f;
		}
		compoundLoadingOrder[c].order = (UINT)(X * X + Z * Z);
	}

	qsort(compoundLoadingOrder, world_main_size, sizeof(compoundTreeLoadOrder), CompoundSortCB);	  // Order compound by distance:

	// [Collision 1] Check Collision with "10" COMPOUNDS near to us...:
	// ----------------------------------------------------------------
	XMVECTOR prwsPos = {}, prwsDir = {};

#if defined CHECK_OBJ_COLISION
	pickRayVector((float)WOMA::AppSettings->WINDOW_WIDTH / 2.0f,
		(float)WOMA::AppSettings->WINDOW_HEIGHT - 65,
		prwsPos, prwsDir);
#endif

	UINT closestObjId = UINT_MAX;
	for (UINT c = 0; c < MIN(world_main_size, 5); c++) // We don't need all, right? :)
	{
		int i = c;	// This is the compound[id] to check collisions...

		{
			if (!objModel[i])
				continue;

			closestObjDist = pick(prwsPos, prwsDir,
				objModel[i]->boundingBoxVerts,
				objModel[i]->boundingBoxIndex,
				((DXmodelClass*)objModel[i])->m_worldMatrix,
				false);	// Use Bounding Boxes, Faster!

			if (closestObjDist < FLT_MAX)
			{
				closestObjId = i; // Get the Closest Object ID!
				break;
			}
		}
	}

	// Calculate it with more accuracy if we are really close to an object:
	if (closestObjDist >= 0 && closestObjDist <= 3)
	{
		closestObjDist = pick(prwsPos, prwsDir,
			objModel[closestObjId]->bottleVertPosArray,
			((DXmodelClass*)objModel[closestObjId])->obj3d.indices32,
			((DXmodelClass*)objModel[closestObjId])->m_worldMatrix,
			true);
	}

}
#endif



float ApplicationClass::ProcessMovementInput_and_UpdateDemos()
{
	float fadeLight = 1;

#if defined USE_TIMER_CLASS
  #if defined INTRO_DEMO
	// TIME Control: Show Debug Info
	UINT64 passedTotalTime = (UINT64)((SystemHandle->m_Timer.currentTime - SystemHandle->m_Timer.m_startEngineTime) / SystemHandle->m_Timer.m_ticksPerMs);	// To control events in time (DEMO)

	// 5 INTRO CREDITS TEXT: Show time, etc..
	if (RENDER_PAGE < 21) {
		fadeIntro = WOMA_APPLICATION_IntroRender(passedTotalTime);
	}
	else
		fadeIntro = 1;

	WOMA_APPLICATION_DemoRender(passedTotalTime);

	if (RENDER_PAGE < 15)
		return 0;

  #endif
#endif

	// ========================================================================================
	// [2] DETECT COLLISIONS (active from DX_ENGINE_LEVEL >= 78)
	// ========================================================================================
#if defined CHECK_OBJ_COLISION
	IF_RENDER_PAGE(RENDER_PAGE >= 78)
		DetectCollisions();
#endif

	// ========================================================================================
	// [3] USER INPUT (keyboard)
	// ========================================================================================
#if defined USE_DIRECT_INPUT					
	IF_RENDER_PAGE(RENDER_PAGE >= 28)
	ProcessUserKeyboardInputAndTerrainCollision(dt); //Keyboard keys
#endif

	// ========================================================================================
	// [4] CAMERA SKY UPDATE (third-person camera available from DX_ENGINE_LEVEL >= 88)
	// ========================================================================================
#if defined USE_SKYSPHERE && defined USE_SKY_CAMERA_DOME	
	IF_RENDER_PAGE(RENDER_PAGE >= 28) //if (RENDER_PAGE >= 28)
	{
		if (WOMA::AppSettings->DRIVER != DRIVER_GL3)
		{
	#if defined DX_ENGINE

        #if defined USE_3RD_PERSON_CAMERA || defined INTRO_DEMO
			IF_RENDER_PAGE(RENDER_PAGE >= 88)
			{
				if (g_GOD_MODE) {
					DXsystemHandle->m_CameraSKY->m_rotationX = DXsystemHandle->m_Camera->m_rotationX;
					DXsystemHandle->m_CameraSKY->m_rotationY = DXsystemHandle->m_Camera->m_rotationY;
					DXsystemHandle->m_CameraSKY->CalculateViewMatrix();
				} else {
					DXsystemHandle->m_CameraSKY->CalculateViewMatrix_3rd_PersonCamera(main3rdPcamera.m_camYaw, main3rdPcamera.m_camPitch, true);
				}
			}
		#endif
		#if DX_ENGINE_LEVEL < 88  || defined INTRO_DEMO
		#if !defined USE_3RD_PERSON_CAMERA || defined INTRO_DEMO
			IF_RENDER_PAGE(RENDER_PAGE < 88)
			{
				DXsystemHandle->m_CameraSKY->m_rotationX = DXsystemHandle->m_Camera->m_rotationX;
				DXsystemHandle->m_CameraSKY->m_rotationY = DXsystemHandle->m_Camera->m_rotationY;
				DXsystemHandle->m_CameraSKY->CalculateViewMatrix();
			}
        #endif
		#endif
	#endif
		}
	#if (defined OPENGL3 || defined OPENGL4)
		else
		{
			GLopenGLclass* driver = (GLopenGLclass*)driverList[WOMA::AppSettings->DRIVER];
			driver->gl_CameraSKY->m_rotationX = driver->gl_Camera->m_rotationX;
			driver->gl_CameraSKY->m_rotationY = driver->gl_Camera->m_rotationY;
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
		int X = ((WOMA::AppSettings->WINDOW_WIDTH - m_titleModel->SpriteTextureWidth) / 2);
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
		if (RENDER_PAGE < DX_ENGINE_LEVEL) 
		{
			RENDER_PAGE++;

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
	rY = (float)(dt) * (0.005f / 16.66f);	// MOVIMENT FORMULA!

	if (RENDER_PAGE == 28)
		FORCE_RENDER_ALL = true;
	else
		FORCE_RENDER_ALL = false;

#if (DX_ENGINE_LEVEL >= 10 && DX_ENGINE_LEVEL <= 23) || defined INTRO_DEMO
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

		#undef cor
	}
#endif

	if (RENDER_PAGE == 28) {
		FORCE_RENDER_ALL = true;
	}
	else
		FORCE_RENDER_ALL = false;

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
#if DX_ENGINE_LEVEL >= 23 && defined SCENE_TEXTURE_LIGHT

	//DEMO-2
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
		screenShot->RenderSprite(pContext, (WOMA::AppSettings->WINDOW_WIDTH - screenShot->SpriteTextureWidth) / 2, 
			(WOMA::settings.WINDOW_WIDTH > (1920 + 100))? WOMA::AppSettings->WINDOW_HEIGHT / 3: WOMA::AppSettings->WINDOW_HEIGHT / 4, 1, fadeIntro);
	}
}
#endif


#if defined CHECK_OBJ_COLISION
// Calculate the world space pick ray from the 2D coordinates
// ==================================================================================================================================
void ApplicationClass::pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir)
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
		//XMVECTOR faceNormalClosestObject = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		U = tri1V2 - tri1V1;
		V = tri1V3 - tri1V1;

		//Compute face normal by crossing U, V
		faceNormalClosestObject = XMVector3Cross(U, V);
		faceNormalClosestObject = XMVector3Normalize(faceNormalClosestObject);

		//Calculate a point on the triangle for the plane equation
		XMVECTOR triPoint = tri1V1;

		//Get plane equation ("Ax + By + Cz + D = 0") Variables
		float tri1A = XMVectorGetX(faceNormalClosestObject);
		float tri1B = XMVectorGetY(faceNormalClosestObject);
		float tri1C = XMVectorGetZ(faceNormalClosestObject);
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

#if DX_ENGINE_LEVEL >= 89 && defined USE_MAP_EDITOR // #if defined CHECK_COMPOUND_COLISION //anyMouseClickToPick()
// Check if the "LEFT Mouse Button" was pressed and if there is anything to pick
// ==================================================================================================================================
void ApplicationClass::anyMouseClickToPick()
// ==================================================================================================================================
{
	static int pickWhat = 1;
	static bool isShoot = false;
	static XMMATRIX targetWorld;

	float tempDist=0;
	
	int hitIndex = -1;
	float closestDist = FLT_MAX;

	if (DXsystemHandle->m_Input->m_mouseState.rgbButtons[MOUSE_LEFT] & 0x80)
	{
		if (isShoot == false)
		{
			POINT mousePos;

			GetCursorPos(&mousePos);
			//ScreenToClient(SystemHandle->m_hWnd, &mousePos);

			int mousex = mousePos.x;
			int mousey = mousePos.y;
			pickRayVector((float)mousex, (float)mousey, prwsPos, prwsDir); // when we click LEFT mouse button

			// Check all COMPOUNDS Objects:
			// ----------------------------
			for (UINT i = 0; i < world_main_size; i++)
			{
                ((DXmodelClass*)objModel[i])->bottleHit = 0; //reset it
                if ((DXmodelClass*)objModel[i]->ready && (DXmodelClass*)objModel[i]->visible) // Check compounds already loaded... 
                {
                    tempDist = FLT_MAX;

                    //D3DX_TO_XM_MATRIX(bottleWorld, compound[i].objModel->m_world);
                    targetWorld = ((DXmodelClass*)objModel[i])->m_worldMatrix;

                    //("Bounding Sphere")
                    //-------------------
                    if (pickWhat == 0)
                    {
                        float pRToPointDist = 0.0f; // Closest distance from the pick ray to the objects center

                        XMVECTOR targetPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
                        XMVECTOR pOnLineNearBottle = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

                        // For the Bounding Sphere to work correctly, we need to make sure we are testing
                        // the distance from the objects "actual" center and the pick ray. We have stored
                        // the distance from (0, 0, 0) in the objects model space to the object "actual"
                        // center in bottleCenterOffset. So now we just need to add that difference to
                        // the bottles world space position, this way the bounding sphere will be centered
                        // on the object real center.
                        XMVECTOR bottleCenterOffset = XMVectorSet(objModel[i]->bottleCenterOffset.x,
                            objModel[i]->bottleCenterOffset.y,
                            objModel[i]->bottleCenterOffset.z,
                            objModel[i]->bottleCenterOffset.w);

                        targetPos = XMVector3TransformCoord(targetPos, targetWorld) + bottleCenterOffset;
                        // This equation gets the point on the pick ray which is closest to bottlePos
                        pOnLineNearBottle = prwsPos + XMVector3Dot((targetPos - prwsPos), prwsDir) / XMVector3Dot(prwsDir, prwsDir) * prwsDir;

                        // Now we get the distance between bottlePos and pOnLineNearBottle
                        // This line is slightly less accurate, but it offers a performance increase by
                        // estimating the distance using XMVector3LengthEst()
                        pRToPointDist = XMVectorGetX(XMVector3LengthEst(pOnLineNearBottle - targetPos));				
                        //v2:
                        //pRToPointDist = XMVectorGetX(XMVector3Length(pOnLineNearBottle - targetPos));

                        // If the distance between the closest point on the pick ray (pOnLineNearBottle) to bottlePos
                        // is less than the bottles bounding sphere (represented by a float called bottleBoundingSphere)
                        // then we know the pick ray has intersected with the bottles bounding sphere, and we can move on
                        // to testing if the pick ray has actually intersected with the bottle itself.
                        if (pRToPointDist < objModel[i]->bottleBoundingSphere)
                        {
                            // This line is the distance to the pick ray intersection with the sphere
                            tempDist = XMVectorGetX(XMVector3Length(pOnLineNearBottle - prwsPos));
                            //v2:
                            // Check for picking with the actual model now
                            //tempDist = pick(prwsPos, prwsDir, objModel[i]->bottleVertPosArray, objModel[i]->boundingBoxIndex, targetWorld);
                        }
                    }

                    // ("Bounding Box")
                    //-----------------
                    if (pickWhat == 1)
                        tempDist = pick(prwsPos, prwsDir, objModel[i]->boundingBoxVerts, objModel[i]->boundingBoxIndex, targetWorld);
	
                    // ("Model") Check for picking directly with the real model: 
                    //----------------------------------------------------------
                    if(pickWhat == 2) {
                        //tempDist = pick(prwsPos, prwsDir, objModel[i]->bottleVertPosArray, objModel[i]->indices, targetWorld);
                        tempDist = pick(prwsPos, prwsDir, objModel[i]->bottleVertPosArray,
                            objModel[i]->boundingBoxIndex,
                            ((DXmodelClass*)objModel[i])->m_worldMatrix, false);	// Use Bounding Boxes, Faster!
                    }

					if (tempDist < closestDist)
					{
						closestDist = tempDist;
						hitIndex = i; // Compund = hitIndex
					}
				}
			}

			//------------------------------------------------------------------------------------------------------
			// To avoid pick more than 1 compound:
			if (closestDist < FLT_MAX)
			{
				if (hitIndex >= 0) {
                    ((DXmodelClass*)objModel[hitIndex])->bottleHit = 1; //warning C4701: potentially uninitialized local variable 'hitIndex' used
					CHG_COMPOUND = hitIndex;
				}
				//pickedDist = closestDist;
			}

			isShoot = true;

		}//allow press mouse?

	}//if mouse pressed...

	if (!(DXsystemHandle->m_Input->m_mouseState.rgbButtons[MOUSE_LEFT] & 0x80))
	{
		isShoot = false;
	}

    CheckEditor(hitIndex, CHG_COMPOUND);
}
#endif
#endif

//NOTE: LoadTextureFromPathFBX work on DLL (on level >= 86)
#if (DX_ENGINE_LEVEL == 79 || DX_ENGINE_LEVEL >= 82) && LEVEL <= 85
Texture* LoadTextureFromPathFBX(UINT model_type, Graphics& graphics, LPCWSTR& texture) //LEVEL <= 85
{
    return NULL;
}
#endif


#if DX_ENGINE_LEVEL >= 89 && defined USE_MAP_EDITOR
void ApplicationClass::CheckEditor(int hitIndex, int c)
{

    // Process Movement Keys: + - (NUM PAD), LEFT, RIGHT, UP or DOWN (SHIFT: SLOW MOVE)
    float deltaMove = (DXsystemHandle->m_player[g_NetID]->p_player.IsShift) ? 0.1f : 1.0f; // Slow or Normal MOVE?

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

#if defined(USE_RASTERTEK_TEXT_FONT)
    static bool firstPick = true;
    if (c >= 0 && firstPick) // IF user clicked in a obj. dif. than default first one: "render text" he want the: "EDIT MODE"...
        firstPick = false;

    if (c >= 0 && ((DXmodelClass*)objModel[c])->ready && !firstPick)
    {
        AppTextClass->SetCompoundPosition((CHAR*)((DXmodelClass*)objModel[c])->MODEL_NAME.c_str(), closestObjDist, c,
            ((DXmodelClass*)objModel[c])->m_worldMatrix._41, //X
            ((DXmodelClass*)objModel[c])->m_worldMatrix._42, //Y
            ((DXmodelClass*)objModel[c])->m_worldMatrix._43, //Z
			SystemHandle->xml_loader.theWorldXML[c].scale /*((DXmodelClass*)objModel[c])->m_worldMatrix._11*/, //scale
            SystemHandle->xml_loader.theWorldXML[c].rotY        //Rot 
        );
    }
    if (c >= 0)  // An OBJ was selected so we are in EDIT MODE.
    {
        // -----------------------------------------------------------------
        // Arrow <- (X) //
        static bool cursorLeft = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORLeftPressed && cursorLeft == false)
        {
            cursorLeft = true;
            SystemHandle->xml_loader.theWorldXML[c].posX -= deltaMove;
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORLeftPressed && cursorLeft == true)
                cursorLeft = false;

        // Arrow -> (X)//
        static bool cursorRight = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORRightPressed && cursorRight == false)
        {
            cursorRight = true;
            SystemHandle->xml_loader.theWorldXML[c].posX += deltaMove;
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORRightPressed && cursorRight == true)
                cursorRight = false;

        // Arrow ^ (Z)//
        static bool cursorUp = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORUpPressed && cursorUp == false)
        {
            cursorUp = true;
            SystemHandle->xml_loader.theWorldXML[c].posZ -= deltaMove;
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORUpPressed && cursorUp == true)
                cursorUp = false;

        // Arrow v (Z)//
        static bool cursorDown = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORDownPressed && cursorDown == false)
        {
            cursorDown = true;
            SystemHandle->xml_loader.theWorldXML[c].posZ += deltaMove;
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORDownPressed && cursorDown == true)
                cursorDown = false;

        // -----------------------------------------------------------------
        // pagDOWN (Y)//
        static bool pgDown = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsPgDownPressed && pgDown == false)
        {
            pgDown = true;
            SystemHandle->xml_loader.theWorldXML[c].translateY -= deltaMove;
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsPgDownPressed && pgDown == true)
                pgDown = false;

        // pgUP (Y)//
        static bool pgUp = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsPgUpPressed && pgUp == false)
        {
            pgUp = true;
            SystemHandle->xml_loader.theWorldXML[c].translateY += deltaMove;
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsPgUpPressed && pgUp == true)
                pgUp = false;

        // -----------------------------------------------------------------
        // NUMPAD - is Scale//
        static bool numPadMinus = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsNumPadMinus && numPadMinus == false)
        {
            numPadMinus = true;
            SystemHandle->xml_loader.theWorldXML[c].scale -= (deltaMove / 2);
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsNumPadMinus && numPadMinus == true)
                numPadMinus = false;


        // NUMPAD + is Scale//
        static bool numPadPlus = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsNumPadPlus && numPadPlus == false)
        {
            numPadPlus = true;
            SystemHandle->xml_loader.theWorldXML[c].scale += (deltaMove / 2);
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsNumPadPlus && numPadPlus == true)
                numPadPlus = false;

        // -----------------------------------------------------------------
        // NUMPAD - is rotY//
        static bool numPad4Left = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORNumPad4 && numPad4Left == false)
        {
            numPad4Left = true;
            SystemHandle->xml_loader.theWorldXML[c].rotY -= (deltaMove / 100);
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORNumPad4 && numPad4Left == true)
                numPad4Left = false;


        // NUMPAD + is rotY//
        static bool numPad6Right = false;
        if (DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORNumPad6 && numPad6Right == false)
        {
            numPad6Right = true;
            SystemHandle->xml_loader.theWorldXML[c].rotY += (deltaMove / 100);
        }
        else
            if (!DXsystemHandle->m_player[g_NetID]->p_player.IsEDITORNumPad6 && numPad6Right == true)
                numPad6Right = false;
    }


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
#endif

}
#endif

#if defined USE_MINIMAP_REDENRING_THREAD
void ApplicationClass::CreateRenderThreads()
{
	driver11Ref = (DirectX::DX11Class*)m_Driver;

#if defined USE_SHADOW_THREAD
	shadowCtx = driver11Ref->shadowDeferredCtx;
	shadowWorker = std::thread(&ApplicationClass::ShadowWorkerFunc, this);
#endif

	minimapCtx = driver11Ref->minimapDeferredCtx;
	minimapWorker = std::thread(&ApplicationClass::MinimapWorkerFunc, this);
}

void ApplicationClass::StopRenderThreads()
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		stopThreads = true;
		cv.notify_all();
	}

	if (shadowWorker.joinable()) {
		shadowWorker.join();
	}

	if (minimapWorker.joinable()) {
		minimapWorker.join();
	}
}
#endif
