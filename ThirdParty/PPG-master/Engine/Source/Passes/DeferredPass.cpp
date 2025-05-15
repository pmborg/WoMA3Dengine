// --------------------------------------------------------------------------------------------
// Filename: DeferredPass.cpp
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
#include "DeferredPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Light.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Sampler.h"

_declspec(align(16))
struct DeferredPassCBuffer
{
    XMMATRIX m_InverseProjection;
    XMMATRIX m_InverseView;
    int m_UseAO = 0;
    int m_UseEnvMap = 0;
};

DeferredPass::DeferredPass(Graphics& graphics, Texture& renderTarget, Texture& diffuse, Texture& metalRough, Texture& normals, Texture& emissive) :
    m_Diffuse(diffuse),
    m_MetalRough(metalRough),
    m_Normals(normals),
    m_Emissive(emissive),
    m_RenderTarget(renderTarget)
{
    m_Shader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Fullscreen.vs.cso", 
                                        L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Deferred.ps.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(DeferredPassCBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr);
}


DeferredPass::~DeferredPass()
{
    SAFE_RELEASE(m_Buffer);
}

void DeferredPass::UseAmbientOcclusion(Texture& aoMap)
{
    m_AO = &aoMap;
    m_UseAO = true;
}

void DeferredPass::UseEnvMap(Texture* envMap, Texture* preFilter, Texture* brdfLut)
{
    m_EnvMap = envMap;
    m_PreFilter = preFilter;
    m_BrdfLut = brdfLut;
    m_UseEnvMap = true;
}

void DeferredPass::DisableAmbientOcclusion()
{
    m_AO = nullptr;
    m_UseAO = false;
}

void DeferredPass::Render(Graphics& graphics, Scene& scene)
{
    graphics.ClearRenderTargetView(m_RenderTarget.GetRTV(), Colors::Transparent);
    //AQUI graphics.SetRenderTarget(m_RenderTarget, false);

    auto deviceContext = graphics.m_DeviceContext;
    scene.lightManager.Use(deviceContext, 1);
    auto& lightManager = scene.lightManager;
    if (lightManager.hasLightWithShadows)
    {
        deviceContext->PSSetConstantBuffers(2, 1, &(lightManager.m_OneShadowMapCBuffer));
        lightManager.m_OneShadowMapTexture->UseSRV(deviceContext, 4);
    }

    auto& camera = scene.m_MainCamera;
    DeferredPassCBuffer deferredCBuf;
    deferredCBuf.m_InverseProjection = XMMatrixInverse(NULL, camera.CalculateProjection());
    deferredCBuf.m_InverseView = XMMatrixInverse(NULL, camera.CalculateView());
    deferredCBuf.m_UseAO = m_UseAO;
    deferredCBuf.m_UseEnvMap = m_UseEnvMap;
    deviceContext->PSSetConstantBuffers(3, 1, &m_Buffer);
    graphics.UpdateBuffer(m_Buffer, &(deferredCBuf));

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    graphics.m_DepthStencilBuffer->UseSRV(deviceContext, 0);
    //deviceContext->PSSetShaderResources(0, 1, &m_TextureSRV);

    m_Diffuse.UseSRV(deviceContext, 1);
    m_MetalRough.UseSRV(deviceContext, 2);
    m_Normals.UseSRV(deviceContext, 3);
    m_Emissive.UseSRV(deviceContext, 9);
    if (m_UseAO)
    {
        m_AO->UseSRV(deviceContext, 5);
    }
    if (m_UseEnvMap)
    {
        m_EnvMap->UseSRV(deviceContext, 6);
        m_PreFilter->UseSRV(deviceContext, 7);
        m_BrdfLut->UseSRV(deviceContext, 8);
    }
    m_Shader->Use(deviceContext);
    deviceContext->Draw(4, 0);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindShaderResourceView(1);
    graphics.UnbindShaderResourceView(2);
    graphics.UnbindShaderResourceView(3);
    graphics.UnbindShaderResourceView(4);
    graphics.UnbindShaderResourceView(5);
    graphics.UnbindShaderResourceView(6);
    graphics.UnbindShaderResourceView(7);
    graphics.UnbindShaderResourceView(8);
    graphics.UnbindShaderResourceView(9);
    //AQUI graphics.UnbindRenderTargetView();
}
