// --------------------------------------------------------------------------------------------
// Filename: Application.cpp
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
#include <random>
#include "SSAOPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Light.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Sampler.h"

SSAOPass::SSAOPass(Graphics& graphics, Texture& renderTarget, Texture& depth, Texture& normals) :
    m_Depth(depth),
    m_Normals(normals),
    m_RenderTarget(renderTarget)
{
    m_Shader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Fullscreen.vs.cso", 
                                        L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\SSAO.ps.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(SSAOBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr);

    // create texture
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;

    for (int i = 0; i < 64; ++i)
    {
        XMVECTOR sample = XMVectorSet(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator),
            0
        );
        sample = XMVector3Normalize(sample);
        float scale = (float)i / 64.0;
        // to lerp
        XMVectorScale(sample, scale);

        XMStoreFloat4(&m_SSAOBuffer.m_Kernel[i], sample);
    }

    using namespace DirectX::PackedVector;
    std::vector<XMBYTEN4> rotation;
    rotation.reserve(16);
    for (int i = 0; i < 16; ++i)
    {
        XMVECTOR rot = XMVectorSet(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0,
            0
        );
        XMBYTEN4 rotbyte;
        XMStoreByteN4(&rotbyte, rot);
        rotation.push_back(rotbyte);
    }

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = rotation.data();
    data.SysMemPitch = 4 * 4; // 4 pixels width, 4 bytes (32 bits)
    Texture* tex = Texture::CreateTexture(graphics, 4, 4, "Noise Tex", DXGI_FORMAT_R8G8B8A8_SNORM, D3D11_BIND_SHADER_RESOURCE, &data);
    tex->CreateSRV(graphics, DXGI_FORMAT_R8G8B8A8_SNORM);
    m_Noise = std::unique_ptr<Texture>(tex);
}

SSAOPass::~SSAOPass()
{
    SAFE_RELEASE(m_Buffer);
}

void SSAOPass::Render(Graphics& graphics, Scene& scene)
{
    graphics.ClearRenderTargetView(m_RenderTarget.GetRTV(), Colors::Transparent);
    auto deviceContext = graphics.m_DeviceContext;

    auto& camera = scene.m_MainCamera;
    auto view = camera.CalculateView();
    auto projection = camera.CalculateProjection();
    m_SSAOBuffer.m_ViewProjection = XMMatrixMultiply(view, projection);
    m_SSAOBuffer.m_InverseViewProjection = XMMatrixInverse(NULL, m_SSAOBuffer.m_ViewProjection);
    XMStoreFloat4(&m_SSAOBuffer.m_CameraPosition, camera.m_EyePosition);
    m_SSAOBuffer.m_Radius = 0.1;
    m_SSAOBuffer.m_WindowSize = XMFLOAT2(1280, 720);
    graphics.UpdateBuffer(m_Buffer, &(m_SSAOBuffer));

    deviceContext->PSSetConstantBuffers(3, 1, &m_Buffer);
    m_Depth.UseSRV(deviceContext, 0);
    m_Normals.UseSRV(deviceContext, 1);
    m_Noise->UseSRV(deviceContext, 2);
    m_Shader->Use(deviceContext);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    deviceContext->Draw(4, 0);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindShaderResourceView(1);
    graphics.UnbindShaderResourceView(2);
}
