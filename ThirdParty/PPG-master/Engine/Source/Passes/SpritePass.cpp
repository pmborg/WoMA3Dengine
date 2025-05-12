#include "stdafx.h"
#include <algorithm>
#include "SpritePass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"


SpritePass::SpritePass(Graphics& graphics, Texture& renderTarget) :
    m_RenderTarget(renderTarget)
{
    m_Shader = std::make_unique<Shader>(L"C:\\WoMAengine2023\\PPG-master\\Bin\\Debug\\VertexShader.cso", L"C:\\WoMAengine2023\\PPG-master\\Bin\\Debug\\Sprite.ps.cso", graphics);
    m_QuadMesh = std::make_unique<Mesh>(QuadVertices(), QuadIndices(), graphics);

    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;
    HRESULT hr = graphics.m_Device->CreateDepthStencilState(&depthStencilStateDesc, &m_NoWriteDepthStencil);

    if (FAILED(hr))
    {
        throw std::exception("Faied to create Depth stencil state in Sprite Pass");
    }
}

SpritePass::~SpritePass()
{
    SAFE_RELEASE(m_NoWriteDepthStencil);
}

void SpritePass::Render(Graphics& graphics, Scene& scene)
{
    graphics.SetRenderTarget(m_RenderTarget);

    auto deviceContext = graphics.m_DeviceContext;
    scene.UseCamera(graphics, scene.m_MainCamera);
    scene.UseModel(graphics);
    m_Shader->Use(deviceContext);

    deviceContext->OMSetDepthStencilState(m_NoWriteDepthStencil, 1);
    deviceContext->OMSetBlendState(graphics.m_AlphaBlendState, NULL, 0xffffffff);

    // Sort Sprites
    std::vector<std::shared_ptr<SceneObject>> sprites;
    std::copy_if(scene.m_Node.begin(), scene.m_Node.end(), std::back_inserter(sprites),
        [](auto& obj) { return obj->m_SpriteRenderer.m_IsEnabled; });

    std::vector<std::tuple<Transform*, SpriteRenderer*, float>> spriteInfos;
    auto& eyePos = scene.m_MainCamera.m_EyePosition;
    std::transform(sprites.begin(), sprites.end(), std::back_inserter(spriteInfos),
        [&eyePos](auto& obj)
        {
            Transform* pTransform = &(obj->m_Transform);
            float squaredLength;
            XMVectorGetByIndexPtr(&squaredLength, XMVector3LengthSq(eyePos - pTransform->GetWorldPosition()), 0);
            return std::make_tuple(pTransform, &(obj->m_SpriteRenderer), squaredLength);
        });

    std::sort(spriteInfos.begin(), spriteInfos.end(),
        [](auto& a, auto& b) { return std::get<float>(a) > std::get<float>(b); });


    for (auto& spriteInfo : spriteInfos)
    {
        Transform* pTransform = std::get<Transform*>(spriteInfo);
        SpriteRenderer* pSpriteRenderer = std::get<SpriteRenderer*>(spriteInfo);
        scene.UpdateModel(graphics, pTransform->GetModel());
        pSpriteRenderer->m_Sprite->UseSRV(deviceContext, 0);
        m_QuadMesh->Draw(deviceContext);
    }
    deviceContext->OMSetDepthStencilState(graphics.m_DepthStencilState, 1);
    deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
}
