// --------------------------------------------------------------------------------------------
// Filename: mainmap.cpp
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
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#include "ApplicationClass.h"
#include "DXmodelClass.h"

//#############################################################################################################
// PRE-RENDER - MAP
//#############################################################################################################
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
void ApplicationClass::TerrainRender(UINT monitorWindow, WomaDriverClass* Driver, float fadeLight, XMMATRIX* m_viewMatrix, XMMATRIX* m_projectionMatrix)
{
	//MACRO/FUNCTIONAQUI1
	m_Driver->TurnOffAlphaBlending(); // Re assume default

	//Water Render:
	float t = ((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->m_Shader11->time; //preserve animation time
	((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->m_Shader11->time = 0;
	((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->shaderTypeParameter = 1; // Render for Map projection
	m_TerrainModel[WATER_TERRAIN_ID]->Render(CAMERA_NORMAL, PROJECTION_MINIMAP, PASS_MINIMAP1, m_viewMatrix, m_projectionMatrix);
	((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->m_Shader11->time = t;

	//Terrain Render:
	((DirectX::DXmodelClass*)m_TerrainModel[MAIN_TERRAIN_ID])->shaderTypeParameter = 1; // Render for Map projection
	m_TerrainModel[MAIN_TERRAIN_ID]->Render(CAMERA_NORMAL, PROJECTION_MINIMAP, PASS_MINIMAP1, m_viewMatrix, m_projectionMatrix);

	((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->shaderTypeParameter = 
	((DirectX::DXmodelClass*)m_TerrainModel[MAIN_TERRAIN_ID])->shaderTypeParameter = 0; // Render in normal projection
}

DXcameraClass m_CameraMINIMAP; // DX Implementation
DXcameraClass m_CameraMAP; // DX Implementation

//
// RENDER TO TEXTURE
//
void ApplicationClass::AppPreRenderMainMapMiniMap(UINT monitorWindow, WomaDriverClass* Driver, float fadeLight)
{
	//---------------------------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
	if (!DXsystemHandle->m_CameraMAP && m_Driver->RenderfirstTime) //m_Driver->RenderfirstTime
	{
		//Set Camera Position - Render Camera:
		//"viewMatrix": SET Camera Roration and Position to 2D Render: TEXT and SPRITES
		m_CameraMAP.SetRotation(+89.999f, 0, 0);
		/*       /
			   /     |
			/a       | loadedTerrain[2]->m_terrainHeight/2
			--- h--- |
		Note:
			angle a = 21.8f (half of our view frustrum)
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
		if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
			m_CameraMAP.CalculateViewMatrix();
#endif
#if (defined OPENGL3 || defined OPENGL4)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
			GLopenGLclass* driver = (GLopenGLclass*)driverList[SystemHandle->AppSettings->DRIVER];
			if (driver->gl_Camera)
				driver->gl_Camera->Render();
		}
#endif
        // Render Water/Terrain in: to texture: m_RenderMapTexture
		m_RenderMapTexture->SetRenderTarget(Driver);								// Set the render target to be the render to texture.
		m_RenderMapTexture->ClearRenderTarget(Driver, 0.30f, 0.30f, 0.30f, 1.0f);	// Clear the render to texture!
		TerrainRender(monitorWindow, Driver, fadeLight, &m_CameraMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix);
#if defined USE_MINIMAP_EXPANSION
        for (UINT id = 0; id < world_main_size; id++)  //TODO: use sceneManager
            RenderModel(monitorWindow, m_Driver, id, PASS_OPAC, &m_CameraMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix);
#endif
	}
#endif

	//G:\DRIVE_MY_SOURCE_CODE\Dx11Engine3D\Dx11Engine3Dx64\src\terrain\Minimapclass.cpp
	//ORI: bool MiniMapClass::RenderMiniMapToTexture(ID3D11DeviceContext* pContext)
	//---------------------------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 63 && defined USE_MINI_MAP //&& !defined USE_MINIMAP_EXPANSION
	{
		//"viewMatrix": SET Camera Roration and Position to 2D Render: TEXT and SPRITES
		m_CameraMINIMAP.SetRotation(+89.999f, 0, 0);
		m_CameraMINIMAP.SetPosition(sort_cameraX, 100, sort_cameraZ); // 100 Magic number

		//MACRO RENDER:
	#if defined DX_ENGINE	
		if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
			m_CameraMINIMAP.CalculateViewMatrix();
		}
	#endif

        // Render Water/Terrain in: to texture: m_MiniMapBitmapTexture
		m_MiniMapBitmapTexture->SetRenderTarget(Driver);							// Set the render target to be the render to texture: pContext->OMSetRenderTargets
		m_MiniMapBitmapTexture->ClearRenderTarget(Driver, 0.0f, 0.0f, 0.0f, 1.0f);  // Clear the render to texture!
		TerrainRender(monitorWindow, Driver, fadeLight, &m_CameraMINIMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix);
#if defined USE_MINIMAP_EXPANSION
        for (UINT id = 0; id < world_main_size-1; id++)  //TODO: use sceneManager
            RenderModel(monitorWindow, m_Driver, id, PASS_OPAC, &m_CameraMINIMAP.m_viewMatrix, &((DirectX::DX11Class*)Driver)->m_projectionMiniMapMatrix);
#endif
	}
#endif
}

//
// RENDER
//
void ApplicationClass::RenderMainMapMiniMap() 
{
#if defined USE_MAIN_MAP 
	if ((RENDER_PAGE >= 62 && m_mainMapModel) && (WOMA::game_state == GAME_MAP))
	{
		int X = ((SystemHandle->AppSettings->WINDOW_WIDTH - m_mainMapModel->SpriteTextureWidth) / 2);
		int Y = ((SystemHandle->AppSettings->WINDOW_HEIGHT - m_mainMapModel->SpriteTextureHeight) / 2);

        //[1] Render background MAP.
		m_Driver->TurnOffAlphaBlending(); // Re assume default
		m_mainMapModel->RenderSprite(X, Y, 1/*rescale*/);

        //[2] map Frame:
		m_Driver->ClearDepthBuffer();
		m_Driver->TurnOnAlphaBlending(); // Re assume default
		m_mainMapFrameModel->RenderSprite(X, Y, 1/*rescale*/); //SHADER_TEXTURE

		//[3] Render the "arrow" bitmap for all players:
		for (UINT i = 0; i < MAX_CLIENTS; i++)
		{
			if (m_pointMapLocationX[i] != 0 || m_pointMapLocationY[i] != 0)
			{
				m_miniMapArrowModel->UpdateSpriteBuffersRotY(m_pointMapLocationX[i], m_pointMapLocationY[i]);

				XMMATRIX* worldMatrix = &((DXmodelClass*)m_miniMapArrowModel)->m_worldMatrix;

				*worldMatrix = XMMatrixIdentity();
				m_miniMapArrowModel->rotateZ(-m_pointRotation[i] * 0.0174532925f);

				#define _41 r[3].m128_f32[0]
				#define _42 r[3].m128_f32[1]
				#define _43 r[3].m128_f32[2]
				worldMatrix->_41 = (float)-SystemHandle->AppSettings->WINDOW_WIDTH / 2 + m_miniMapArrowModel->SpriteTextureWidth / 2 + m_pointMapLocationX[i];
				worldMatrix->_42 = (float)SystemHandle->AppSettings->WINDOW_HEIGHT / 2 - m_miniMapArrowModel->SpriteTextureHeight / 2 - m_pointMapLocationY[i];
				worldMatrix->_43 = -0.1f; //Make sure that arrow is on top of map
				#undef _41
				#undef _42
				#undef _43
				//[3] Put the "arrow" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
				m_Driver->ClearDepthBuffer();
				m_miniMapArrowModel->Render(CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&(DXsystemHandle->m_Camera->m_viewmatrix2D), NULL);
			}
		}
	}
#endif

#if defined USE_MINI_MAP
	if (RENDER_PAGE >= 63 && m_miniMapModel)
	{
        m_Driver->TurnOffAlphaBlending(); // Re assume default
		//[1] Put the "mini-map" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Driver->ClearDepthBuffer(); //ClearDepthStencilView
		m_miniMapModel->RenderSprite(m_mapLocationX, m_mapLocationY); //SHADER_TEXTURE

        m_Driver->TurnOnAlphaBlending(); // Re assume default
		//[2] Put the "border" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Driver->ClearDepthBuffer();
		m_miniMapBorderModel->RenderSprite(m_mapLocationX, m_mapLocationY, 1/*rescale*/); //SHADER_TEXTURE

		//[3] Render the "arrow" bitmap for all players:
		for (UINT i = 0; i < MAX_CLIENTS; i++)
		{
			if (m_pointLocationX[i] != 0 || m_pointLocationY[i] != 0)
			{
				m_miniMapArrowModel->UpdateSpriteBuffersRotY(m_pointLocationX[i], m_pointLocationY[i]);

				XMMATRIX* worldMatrix = &((DXmodelClass*)m_miniMapArrowModel)->m_worldMatrix;

				//D3DXMATRIXA16 worldMatrix = g_identMatrix;;
				*worldMatrix = XMMatrixIdentity();
				m_miniMapArrowModel->rotateZ(-m_pointRotation[i] * 0.0174532925f);

				#define _41 r[3].m128_f32[0]
				#define _42 r[3].m128_f32[1]
				#define _43 r[3].m128_f32[2]
				worldMatrix->_41 = (float)-SystemHandle->AppSettings->WINDOW_WIDTH / 2 + m_miniMapArrowModel->SpriteTextureWidth / 2 + m_pointLocationX[i];
				worldMatrix->_42 = (float)SystemHandle->AppSettings->WINDOW_HEIGHT / 2 - m_miniMapArrowModel->SpriteTextureHeight / 2 - m_pointLocationY[i];
				worldMatrix->_43 = -0.1f; //Make sure that arrow is on top of map
				#undef _41
				#undef _42
				#undef _43
				//[3] Put the "arrow" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
				m_Driver->ClearDepthBuffer();
				m_miniMapArrowModel->Render(CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&(DXsystemHandle->m_Camera->m_viewmatrix2D), NULL);
			}
		}
	}
#endif
}
#endif
