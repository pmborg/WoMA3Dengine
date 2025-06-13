// --------------------------------------------------------------------------------------------
// Filename: GBufferPass.cpp
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
#include "Math3D.h"
#include "OSengine.h"

#include "GBufferPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Material/PBRMaterial.h"
#include "Scene/Light.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Scene/Model/Skeleton.h"

GBufferPass::GBufferPass(Graphics& graphics, Texture& diffuse, Texture& metalRough, Texture& normals, Texture& emissive) :
    m_Diffuse(diffuse),
    m_MetalRough(metalRough),
    m_Normals(normals),
    m_Emissive(emissive)
{
    shader = std::make_unique<Shader>(	L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\VertexShader.cso", 
										L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\GBuffer.ps.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(PBRMaterialInfo), D3D11_BIND_CONSTANT_BUFFER, nullptr);
    m_BoneBuffer = graphics.CreateBuffer(sizeof(XMMATRIX) * Skeleton::NUM_BONES, D3D11_BIND_CONSTANT_BUFFER, nullptr);
    m_RenderTargets[0] = m_Diffuse.GetRTV();
    m_RenderTargets[1] = m_MetalRough.GetRTV();
    m_RenderTargets[2] = m_Normals.GetRTV();
    m_RenderTargets[3] = m_Emissive.GetRTV();
}

GBufferPass::~GBufferPass()
{
    SAFE_RELEASE(m_Buffer);
    SAFE_RELEASE(m_BoneBuffer);
}

void GBufferPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    static XMMATRIX staticm_FinalTransforms[128] = {};

    shader->Use(deviceContext);

    scene.UseModel(graphics);                                 // 0   VERTEX Buffer: World
    scene.UseCamera(graphics, scene.m_MainCamera);            // 1,2 VERTEX Buffer: VIEW: & PROJ
    deviceContext->VSSetConstantBuffers(3, 1, &m_BoneBuffer); // 3   VERTEX Buffer: Bones

    deviceContext->PSSetConstantBuffers(0, 1, &m_Buffer);    // 0 Pixel Buffer: 
    scene.lightManager.Use(deviceContext, 1);                // 1 Pixel Buffer: Light

    Animator* currentAnimator = nullptr;

	for (size_t i = 0; i < scene.m_Node.size(); ++i)
	{
		auto sceneObj = scene.m_Node[i];
        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;

        MeshRenderer& meshRenderer = sceneObj->m_MeshRenderer;
        Animator* animator = meshRenderer.m_Animator;
        if (animator != nullptr && animator->m_IsEnabled /* && (i == 10)*/)
        {
            if (animator != currentAnimator)
            {
                graphics.UpdateBuffer(m_BoneBuffer, animator->m_FinalTransforms);
                currentAnimator = animator;
            }
            animator->m_FinalTransforms[127].r->m128_f32[0] = 127;    //AQUI-ANIM
        } 
        else {                                                        //AQUI-ANIM
            staticm_FinalTransforms[127].r->m128_f32[0] = 0;       
            graphics.UpdateBuffer(m_BoneBuffer, staticm_FinalTransforms);
        }
        
        PBRMaterial* mat = meshRenderer.m_Material;
        if (mat->m_Albedo)
            mat->m_Albedo->UseSRV(deviceContext, 0);
        meshRenderer.m_Mesh->Draw(deviceContext);
    }
}
