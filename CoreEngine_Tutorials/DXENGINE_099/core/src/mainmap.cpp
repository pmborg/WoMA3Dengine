// --------------------------------------------------------------------------------------------
// Filename: mainmap.cpp
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

#include "OSengine.h"
#include "ApplicationClass.h"

#if defined WINDOWS_PLATFORM
#include "DXmodelClass.h"
#endif

#if defined VULKAN
#include "VKrenderTextureClass.h"
#include "vkModelClass.h"
#endif

#if DX_ENGINE_LEVEL >= 62 && defined DX_ENGINE
#if defined USE_MAIN_MAP
DXcameraClass m_CameraMAP_DX;		// DX Implementation
#endif
#if defined USE_MINI_MAP
DXcameraClass m_CameraMINIMAP_DX; // DX Implementation
#endif
#endif

#if DX_ENGINE_LEVEL >= 62 && defined VULKAN
#if defined USE_MAIN_MAP
GLcameraClass m_CameraMAP_VK;
#endif
#if defined USE_MINI_MAP
GLcameraClass m_CameraMINIMAP_VK;
#endif
#endif

void* void_projectionMiniMapMatrix = NULL;
void* void_viewMatrix = NULL;

#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
void ApplicationClass::RenderMiniMapPass(UINT monitorWindow, WomaDriverClass* Driver, void* pContext, float fadeLight, UINT ThreadID)
{
	//---------------------------------------------------------------------------------------------------------------------------------
		//Set Camera Position - Render Camera:

		/*       /
			   /     |
			/a       | loadedTerrain[2]->m_terrainHeight/2
			--- h--- |
		Note:
			angle a = 21.8f (half of our view frustum)
		*/

		// Check which Quadrant we are:
		float Qx = 0;
		float Qz = 0;
		float h = (float)(((float)(loadedTerrain[2]->m_terrainHeight / 4)) / tan(21.8f * 0.0174532925f) /* + 20*/);

#ifdef DX_ENGINE
		if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
		{
			m_CameraMAP_DX.SetRotation(+89.999f, 0, 0);

			// Check which Quadrant we are:
			Qx = (float)((int)(m_CameraMAP_DX.m_positionX / ((loadedTerrain[2]->m_terrainWidth + 1) / 2)));
			Qz = (float)((int)(m_CameraMAP_DX.m_positionZ / ((loadedTerrain[2]->m_terrainHeight + 1) / 2)));

			m_CameraMAP_DX.SetPosition((float)loadedTerrain[2]->m_terrainWidth / 4 + Qx * loadedTerrain[2]->m_terrainWidth / 2,
				h,
				(float)loadedTerrain[2]->m_terrainHeight / 4 + Qz * loadedTerrain[2]->m_terrainHeight / 2);            //Note: Work with 512x512: 4 x 256x256 

			//MACRO RENDER:
			m_CameraMAP_DX.CalculateViewMatrix();

			void_viewMatrix = &m_CameraMAP_DX.m_viewMatrix;
			void_projectionMiniMapMatrix = &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix;

			((DXrendertextureclass*)m_RenderMapTexture)->SetRenderTarget(Driver, (ID3D11DeviceContext*)pContext);								// Set the render target to be the render to texture.
			((DXrendertextureclass*)m_RenderMapTexture)->ClearRenderTarget(Driver, (ID3D11DeviceContext*)pContext, 0.30f, 0.30f, 0.30f, 1.0f);	// Clear the render to texture!
		}
#endif
#ifdef VULKAN
		if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
		{
			m_CameraMAP_VK.SetRotation(+89.999f, 0, 0);

			Qx = (float)((int)(m_CameraMAP_VK.m_positionX / ((loadedTerrain[2]->m_terrainWidth + 1) / 2)));
			Qz = (float)((int)(m_CameraMAP_VK.m_positionZ / ((loadedTerrain[2]->m_terrainHeight + 1) / 2)));

			m_CameraMAP_VK.SetPosition(
				(float)loadedTerrain[2]->m_terrainWidth / 4 + Qx * loadedTerrain[2]->m_terrainWidth / 2,
				h,
				(float)loadedTerrain[2]->m_terrainHeight / 4 + Qz * loadedTerrain[2]->m_terrainHeight / 2);

			m_CameraMAP_VK.CalculateViewMatrix();

			void_projectionMiniMapMatrix = &((vkClass*)Driver)->m_projectionMiniMapMatrix;
			void_viewMatrix = &m_CameraMAP_VK.m_viewMatrix;

			((VKrenderTextureClass*)m_RenderMapTexture)->SetRenderTarget(pContext);
		}
#endif

		//MAINMAP: Render Water + Terrain in: to texture: m_RenderMapTexture
		TerrainRender(ThreadID, monitorWindow, Driver, fadeLight, void_viewMatrix, void_projectionMiniMapMatrix, pContext, PASS_MAINMAP);

#if defined VULKAN
		if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
			vkCmdEndRenderPass(((vkClass*)Driver)->m_offscreenCommandBuffer);
#endif


	//USE_MINIMAP_EXPANSION: ----------------------------------------------------------------------------------------------------------------------------------
#if defined USE_ALPHA_BLENDING
		m_Driver->TurnOffAlphaBlending(pContext);
#endif
#if DX_ENGINE_LEVEL >= 85 && defined USE_MINIMAP_EXPANSION
		for (UINT id = 0; id < initial_world_xml_objs; id++)  // TODO: use sceneManager
		{
			RenderModel(pContext, RENDER_PAGE, 1, monitorWindow, m_Driver, id, PASS_MINIMAP, (XMMATRIX*)void_viewMatrix, (XMMATRIX*)void_projectionMiniMapMatrix, fadeLight);
		}
#endif



#if DX_ENGINE_LEVEL >= 63 && defined USE_MINI_MAP

		// Render Water/Terrain in: to texture: m_MiniMapBitmapTexture
#ifdef DX_ENGINE
		if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
		{
			//"viewMatrix": SET Camera Rotation and Position to 2D Render: TEXT and SPRITES
			m_CameraMINIMAP_DX.SetRotation(+89.999f, 0, 0);
			m_CameraMINIMAP_DX.SetPosition(sort_cameraX, 100, sort_cameraZ);
			m_CameraMINIMAP_DX.CalculateViewMatrix();

			void_viewMatrix = &m_CameraMINIMAP_DX.m_viewMatrix;

			((DXrendertextureclass*)m_MiniMapBitmapTexture)->SetRenderTarget(Driver, (ID3D11DeviceContext*)pContext);
			((DXrendertextureclass*)m_MiniMapBitmapTexture)->ClearRenderTarget(Driver, (ID3D11DeviceContext*)pContext, 0.0f, 0.0f, 0.0f, 1.0f);
		}
#endif

#ifdef VULKAN
		if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
		{
			//"viewMatrix": SET Camera Rotation and Position to 2D Render: TEXT and SPRITES
			m_CameraMINIMAP_VK.SetRotation(+89.999f, 0, 0);
			m_CameraMINIMAP_VK.SetPosition(sort_cameraX, 100, sort_cameraZ);
			m_CameraMINIMAP_VK.CalculateViewMatrix();

			void_viewMatrix = &m_CameraMINIMAP_VK.m_viewMatrix;

			((VKrenderTextureClass*)m_MiniMapBitmapTexture)->SetRenderTarget(pContext);
		}
#endif
		//MINIMAP: Render Water + Terrain in: to texture: m_MiniMapBitmapTexture
		TerrainRender(ThreadID, monitorWindow, Driver, fadeLight, void_viewMatrix, void_projectionMiniMapMatrix, pContext, PASS_MINIMAP);

#if defined USE_ALPHA_BLENDING
		m_Driver->TurnOffAlphaBlending(pContext);
#endif
#if DX_ENGINE_LEVEL >= 85 && defined USE_MINIMAP_EXPANSION
		for (UINT id = 0; id < initial_world_xml_objs; id++)  // TODO: use sceneManager
		{
			RenderModel(pContext, RENDER_PAGE, 1, monitorWindow, m_Driver, id, PASS_MINIMAP, (XMMATRIX*)void_viewMatrix, (XMMATRIX*)void_projectionMiniMapMatrix, fadeLight);
		}
#endif

#if defined VULKAN
		if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
			vkCmdEndRenderPass(((vkClass*)Driver)->m_offscreenCommandBuffer);
#endif

#endif
}
#endif


//#############################################################################################################
// RENDER - WATER & TERRAIN (Used for main terrain and MINIMAP texture)
//#############################################################################################################
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
void ApplicationClass::TerrainRender(UINT ThreadID, UINT monitorWindow, WomaDriverClass* Driver, float fadeLight, void* /*XMMATRIX**/ m_viewMatrix, void* /*XMMATRIX**/ m_projectionMatrix, void* pContext, UINT pass)
{
	//MACRO/FUNCTION
	m_Driver->TurnOffAlphaBlending(pContext); // Re assume default

	//Water Render:
	//m_TerrainModel[WATER_TERRAIN_ID]->shaderTypeParameter = 1; // Render for Map projection
	m_TerrainModel[WATER_TERRAIN_ID]->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_MINIMAP, pass, m_viewMatrix, m_projectionMatrix, true/*Disable Fog*/);
	//m_TerrainModel[WATER_TERRAIN_ID]->shaderTypeParameter = 0; // Render for normal 3D projection

	// Minimap thread -> use old terrain 61 (no shadows)
	bool isMinimapThread = (ThreadID != 0);
	UINT terrainID = isMinimapThread ? MINIMAP_TERRAIN_ID : MAIN_TERRAIN_ID;

	//Terrain Render:
	//m_TerrainModel[terrainID]->shaderTypeParameter = 1; // Render for Map projection
	m_TerrainModel[terrainID]->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_MINIMAP, pass, m_viewMatrix, m_projectionMatrix, true/*Disable Fog*/);
	//m_TerrainModel[terrainID]->shaderTypeParameter = 0; // Render for normal 3D projection
}


//
// RENDER: MAP & MINIMAP (on 3D view!)
//
void ApplicationClass::RenderMainMapMiniMap3Dview(void* pContext) 
{
#if defined USE_MAIN_MAP 
	if ((RENDER_PAGE >= 62 && m_mainMapModel) && (WOMA::game_state == GAME_MAP))
	{
		int X = ((WOMA::AppSettings->WINDOW_WIDTH - m_mainMapModel->SpriteTextureWidth) / 2);
		int Y = ((WOMA::AppSettings->WINDOW_HEIGHT - m_mainMapModel->SpriteTextureHeight) / 2);


		//[1] map Frame:
		m_Driver->TurnOffAlphaBlending(pContext); // Re assume default
		m_mainMapFrameModel->RenderSprite(pContext, X, Y-4, SystemHandle->m_scaleY/*rescale*/, 1.0f/*fade*/); //SHADER_TEXTURE: 051Texture.hlsl

		//[2] Render background MAP.
		m_Driver->ClearDepthBuffer(pContext);
		m_Driver->TurnOnAlphaBlending(pContext); // Re assume default
		if (m_Driver->RenderfirstTime)
		{
			m_mainMapModel->ModelHASAlfaColor = true;
			m_mainMapModel->ModelAlfaColor = 0.75f;
		}
		m_mainMapModel->RenderSprite(pContext, X, Y+28, SystemHandle->m_scaleY/*rescale*/, 1.0f/*fade*/);		//SHADER_TEXTURE: 051Texture.hlsl

		//
		//[3] Render the "ARROW" bitmap for all players: (ON MAIN MAP)
		//
		for (UINT i = 0; i < MAX_CLIENTS; i++)
		{
			//if (m_pointMapLocationX[i] != 0 || m_pointMapLocationY[i] != 0)
			{
				m_miniMapArrowModel->UpdateSpriteBuffersRotY(pContext, m_pointMapLocationX[i], m_pointMapLocationY[i]);

#ifdef VULKAN
				mat4* vkworldMatrix=NULL;
				if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
				{
					vkworldMatrix = &((vkModelClass*)m_miniMapArrowModel)->m_worldMatrix;
					((vkModelClass*)m_miniMapArrowModel)->m_worldMatrix.mat4identity();
				}
#endif
#ifdef DX_ENGINE
				XMMATRIX* dxworldMatrix=NULL;
				if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
				{
					dxworldMatrix = &((DXmodelClass*)m_miniMapArrowModel)->m_worldMatrix;
					*dxworldMatrix = XMMatrixIdentity();
				}
#endif
				m_miniMapArrowModel->rotateZ(-m_pointRotation[i] * 0.0174532925f);

#ifdef VULKAN
				if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
				{
					vkworldMatrix->m[12] = (float)-WOMA::AppSettings->WINDOW_WIDTH / 2 + m_miniMapArrowModel->SpriteTextureWidth / 2 + m_pointMapLocationX[i];
					vkworldMatrix->m[13] = (float)WOMA::AppSettings->WINDOW_HEIGHT / 2 - m_miniMapArrowModel->SpriteTextureHeight / 2 - m_pointMapLocationY[i];
					vkworldMatrix->m[14] = -0.1f;
				}
#endif
#ifdef DX_ENGINE
				if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
				{
				#define _41 r[3].m128_f32[0]
				#define _42 r[3].m128_f32[1]
				#define _43 r[3].m128_f32[2]
				dxworldMatrix->_41 = (float)-WOMA::AppSettings->WINDOW_WIDTH / 2 + m_miniMapArrowModel->SpriteTextureWidth / 2 + m_pointMapLocationX[i];
				dxworldMatrix->_42 = (float)WOMA::AppSettings->WINDOW_HEIGHT / 2 - m_miniMapArrowModel->SpriteTextureHeight / 2 - m_pointMapLocationY[i];
				dxworldMatrix->_43 = -0.1f; //Make sure that arrow is on top of map
				#undef _41
				#undef _42
				#undef _43
				}
#endif

				m_Driver->ClearDepthBuffer(pContext);

#ifdef VULKAN
				if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
					m_miniMapArrowModel->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&((vkClass*)m_Driver)->gl_Camera->m_viewmatrix2D, NULL);
#endif
#ifdef DX_ENGINE
				if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
					m_miniMapArrowModel->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&(DX_Camera->m_viewmatrix2D), NULL);
#endif
			}
		}
	}
#endif

#if defined USE_MINI_MAP
	if (RENDER_PAGE >= 63 && m_miniMapModel)
	{
        m_Driver->TurnOffAlphaBlending(pContext); // Re assume default
		//[1] Put the "mini-map" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Driver->ClearDepthBuffer(pContext); //ClearDepthStencilView
		m_miniMapModel->RenderSprite(pContext, m_mapLocationX, m_mapLocationY, 1.0f, 1.0f); //SHADER_TEXTURE
        m_Driver->TurnOnAlphaBlending(pContext); // Re assume default

		//
		//[3] Render the "ARROW" bitmap for all players: (MINI MAP)
		//
		for (UINT i = 0; i < MAX_CLIENTS; i++)
		{
			if (m_pointLocationX[i] != 0 || m_pointLocationY[i] != 0)
			{
				m_miniMapArrowModel->UpdateSpriteBuffersRotY(pContext, m_pointLocationX[i], m_pointLocationY[i]);

#ifdef VULKAN
				mat4* vkworldMatrix = &((vkModelClass*)m_miniMapArrowModel)->m_worldMatrix;
				if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
				{
					((vkModelClass*)m_miniMapArrowModel)->m_worldMatrix.mat4identity();
				}
#endif
#ifdef DX_ENGINE
				XMMATRIX* dxworldMatrix = &((DXmodelClass*)m_miniMapArrowModel)->m_worldMatrix;
				if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
				{
					*dxworldMatrix = XMMatrixIdentity();
				}
#endif

				m_miniMapArrowModel->rotateZ(-m_pointRotation[i] * 0.0174532925f);

#ifdef VULKAN
				if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
				{
					vkworldMatrix->m[12] = (float)-WOMA::AppSettings->WINDOW_WIDTH / 2 + m_miniMapArrowModel->SpriteTextureWidth / 2 + m_pointMapLocationX[i];
					vkworldMatrix->m[13] = (float)WOMA::AppSettings->WINDOW_HEIGHT / 2 - m_miniMapArrowModel->SpriteTextureHeight / 2 - m_pointMapLocationY[i];
					vkworldMatrix->m[14] = -0.1f;
				}
#endif
#ifdef DX_ENGINE
				if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
				{
				#define _41 r[3].m128_f32[0]
				#define _42 r[3].m128_f32[1]
				#define _43 r[3].m128_f32[2]
				dxworldMatrix->_41 = (float)-WOMA::AppSettings->WINDOW_WIDTH / 2 + m_miniMapArrowModel->SpriteTextureWidth / 2 + m_pointLocationX[i];
				dxworldMatrix->_42 = (float)WOMA::AppSettings->WINDOW_HEIGHT / 2 - m_miniMapArrowModel->SpriteTextureHeight / 2 - m_pointLocationY[i];
				dxworldMatrix->_43 = -0.1f; //Make sure that arrow is on top of map
				#undef _41
				#undef _42
				#undef _43
				}
#endif

				//[3] Put the "arrow" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
				m_Driver->ClearDepthBuffer(pContext);
#ifdef VULKAN
				if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
					m_miniMapArrowModel->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&((vkClass*)m_Driver)->gl_Camera->m_viewmatrix2D, NULL);
#endif
#ifdef DX_ENGINE
				if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11 || WOMA::AppSettings->DRIVER == DRIVER_DX12)
					m_miniMapArrowModel->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&(DX_Camera->m_viewmatrix2D), NULL);
#endif
			}
		}

		//[2] Put the "border" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Driver->ClearDepthBuffer(pContext);
		m_miniMapBorderModel->RenderSprite(pContext, m_mapLocationX, m_mapLocationY, 1/*rescale*/, 1.0f); //SHADER_TEXTURE
	}
#endif
}
#endif

