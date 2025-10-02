// --------------------------------------------------------------------------------------------
// Filename: mainmap.cpp
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

#include "OSengine.h"
#include "ApplicationClass.h"
#include "DXmodelClass.h"

DXcameraClass m_CameraMINIMAP; // DX Implementation
DXcameraClass m_CameraMAP; // DX Implementation

//#############################################################################################################
// PRE-RENDER - MAP
//#############################################################################################################
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP
void ApplicationClass::TerrainRender(UINT ThreadID, UINT monitorWindow, WomaDriverClass* Driver, float fadeLight, XMMATRIX* m_viewMatrix, XMMATRIX* m_projectionMatrix, void* pContext)
{
	//MACRO/FUNCTIONAQUI1
	m_Driver->TurnOffAlphaBlending(pContext); // Re assume default

	//Water Render:
	 ((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->shaderTypeParameter = 1; // Render for Map projection
	 m_TerrainModel[WATER_TERRAIN_ID]->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_MINIMAP, PASS_MINIMAP1, m_viewMatrix, m_projectionMatrix);

	//Terrain Render:
	((DirectX::DXmodelClass*)m_TerrainModel[MAIN_TERRAIN_ID])->shaderTypeParameter = 1; // Render for Map projection
	m_TerrainModel[MAIN_TERRAIN_ID]->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_MINIMAP, PASS_MINIMAP1, m_viewMatrix, m_projectionMatrix);

	//RESET:
	//((DirectX::DXmodelClass*)m_TerrainModel[WATER_TERRAIN_ID])->shaderTypeParameter = 
	//((DirectX::DXmodelClass*)m_TerrainModel[MAIN_TERRAIN_ID])->shaderTypeParameter = 0; // Render in normal projection
}



//
// RENDER
//
void ApplicationClass::RenderMainMapMiniMap(void* pContext) 
{
#if defined USE_MAIN_MAP 
	if ((RENDER_PAGE >= 62 && m_mainMapModel) && (WOMA::game_state == GAME_MAP))
	{
		int X = ((SystemHandle->AppSettings->WINDOW_WIDTH - m_mainMapModel->SpriteTextureWidth) / 2);
		int Y = ((SystemHandle->AppSettings->WINDOW_HEIGHT - m_mainMapModel->SpriteTextureHeight) / 2);

        //[1] Render background MAP.
		m_Driver->TurnOffAlphaBlending(pContext); // Re assume default
		m_mainMapModel->RenderSprite(pContext, X, Y, 1/*rescale*/, 1.0f);

        //[2] map Frame:
		m_Driver->ClearDepthBuffer(pContext);
		m_Driver->TurnOnAlphaBlending(pContext); // Re assume default
		m_mainMapFrameModel->RenderSprite(pContext, X, Y, 1/*rescale*/, 1.0f); //SHADER_TEXTURE

		//[3] Render the "arrow" bitmap for all players:
		for (UINT i = 0; i < MAX_CLIENTS; i++)
		{
			if (m_pointMapLocationX[i] != 0 || m_pointMapLocationY[i] != 0)
			{
				m_miniMapArrowModel->UpdateSpriteBuffersRotY(pContext, m_pointMapLocationX[i], m_pointMapLocationY[i]);

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
				m_Driver->ClearDepthBuffer(pContext);
				m_miniMapArrowModel->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&(DXsystemHandle->m_Camera->m_viewmatrix2D), NULL);
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
		//[2] Put the "border" bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Driver->ClearDepthBuffer(pContext);
		m_miniMapBorderModel->RenderSprite(pContext, m_mapLocationX, m_mapLocationY, 1/*rescale*/, 1.0f); //SHADER_TEXTURE

		//[3] Render the "arrow" bitmap for all players:
		for (UINT i = 0; i < MAX_CLIENTS; i++)
		{
			if (m_pointLocationX[i] != 0 || m_pointLocationY[i] != 0)
			{
				m_miniMapArrowModel->UpdateSpriteBuffersRotY(pContext, m_pointLocationX[i], m_pointLocationY[i]);

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
				m_Driver->ClearDepthBuffer(pContext);
				m_miniMapArrowModel->Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH, PASS_OPAC, (void*)&(DXsystemHandle->m_Camera->m_viewmatrix2D), NULL);
			}
		}
	}
#endif
}
#endif
