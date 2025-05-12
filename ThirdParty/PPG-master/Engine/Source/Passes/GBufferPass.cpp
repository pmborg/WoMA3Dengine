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
    shader = std::make_unique<Shader>(	L"C:\\WoMAengine2023\\PPG-master\\Bin\\Debug\\VertexShader.cso", 
										L"C:\\WoMAengine2023\\PPG-master\\Bin\\Debug\\GBuffer.ps.cso", graphics);
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

    graphics.ClearRenderTargetView(m_Diffuse.GetRTV(), Colors::Transparent);
    graphics.ClearRenderTargetView(m_MetalRough.GetRTV(), Colors::Transparent);
    graphics.ClearRenderTargetView(m_Normals.GetRTV(), Colors::Transparent);
    graphics.ClearRenderTargetView(m_Emissive.GetRTV(), Colors::Transparent);

    #define m_Driver driverList[SystemHandle->AppSettings->DRIVER]
    if (m_Driver->RenderfirstTime) 
    {
        XMMATRIX static world = XMMatrixIdentity();
        //Scale:
        //_11
        //_22
        //_33
        world.r[0].m128_f32[0] = world.r[1].m128_f32[1] = world.r[2].m128_f32[2] = 0.2f;  

        XMMATRIX rotX = XMMatrixRotationX(PI / 2);
        world *= rotX;
        //XMMATRIX rotY = XMMatrixRotationY(PI / 2);
        //world *= rotY;
        //XMMATRIX rotZ = XMMatrixRotationZ(PI/2);
        //world *= rotZ;

        //Translate:
        world.r[3].m128_f32[0] = 39;    //_41: X
        world.r[3].m128_f32[1] = 0.4f;  //_42: Y 
        world.r[3].m128_f32[2] = 20;    //_43: Z

        scene.UpdateModel(graphics, world);
    }
    
    scene.UseCamera(graphics, scene.m_MainCamera); // VIEW / PROJ

    scene.lightManager.Use(deviceContext, 1);
    scene.UseModel(graphics);
    shader->Use(deviceContext);
    deviceContext->VSSetConstantBuffers(3, 1, &m_BoneBuffer);
    deviceContext->PSSetConstantBuffers(0, 1, &m_Buffer);

    Animator* currentAnimator = nullptr;
	for (size_t i = 0; i < scene.m_Node.size(); ++i)
	{
		auto sceneObj = scene.m_Node[i];
        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;

        MeshRenderer& meshRenderer = sceneObj->m_MeshRenderer;
        Animator* animator = meshRenderer.m_Animator;
        if (animator != nullptr && animator->m_IsEnabled)
        {
            if (animator != currentAnimator)
            {
				animator->m_FinalTransforms[127].r->m128_f32[0] = 1;
                graphics.UpdateBuffer(m_BoneBuffer, animator->m_FinalTransforms);
                currentAnimator = animator;
            }
        }

        PBRMaterial* mat = meshRenderer.m_Material;
        graphics.UpdateBuffer(m_Buffer, &(mat->m_MaterialInfo));
        if (mat->m_Albedo)
            mat->m_Albedo->UseSRV(deviceContext, 0);
        if (mat->m_Normal)
            mat->m_Normal->UseSRV(deviceContext, 1);
        if (mat->m_OccRoughMetal)
            mat->m_OccRoughMetal->UseSRV(deviceContext, 2);
        if (mat->m_AoMap)
            mat->m_AoMap->UseSRV(deviceContext, 3);
        if (mat->m_Emissive)
            mat->m_Emissive->UseSRV(deviceContext, 4);
        meshRenderer.m_Mesh->Draw(deviceContext);
    }

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindShaderResourceView(1);
    graphics.UnbindShaderResourceView(2);
    graphics.UnbindShaderResourceView(3);
    graphics.UnbindShaderResourceView(4);
    //AQUI graphics.UnbindRenderTargetView();
}
