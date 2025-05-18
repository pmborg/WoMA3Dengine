// --------------------------------------------------------------------------------------------
// Filename: ShadowMapPass.cpp
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
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG

#include "stdafx.h"
#include "ShadowMapPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ShadowMapPass::ShadowMapPass(Graphics& graphics)
{
    shader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\VertexShader.cso", L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\ShadowMap.ps.cso", graphics);
}

void ShadowMapPass::Render(Graphics& graphics, Scene& scene, Texture& shadowMapTexture, ShadowMapRenderDesc& desc)
{
    OrthographicCamera shadowMapCamera;
    shadowMapCamera.m_EyePosition = desc.m_EyePosition;
    shadowMapCamera.m_LookAt = desc.m_LookAt;
    shadowMapCamera.m_NearZ = desc.m_NearZ;
    shadowMapCamera.m_FarZ = desc.m_FarZ;
    shadowMapCamera.m_ViewHeight = desc.m_ViewHeight;
    shadowMapCamera.m_ViewWidth = desc.m_ViewWidth;

    auto deviceContext = graphics.m_DeviceContext;
    scene.UseCamera(graphics, shadowMapCamera);
    scene.UseModel(graphics); // not so good...
    shader->Use(deviceContext);
	for (size_t i = 0; i < scene.m_Node.size(); ++i)
	{
		auto sceneObj = scene.m_Node[i];

        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;
        scene.UpdateModel(graphics, sceneObj->m_Transform.GetModel());
        sceneObj->m_MeshRenderer.m_Mesh->Draw(deviceContext);
    }

    graphics.Clear(DirectX::Colors::Transparent, 1.0f, 0);
    //  graphics.UnbindRenderTargetView();
}
