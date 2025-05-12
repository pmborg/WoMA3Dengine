#include "stdafx.h"

#include "Light.h"
#include "LowLevel/Graphics.h"
#include "Resources/Texture.h"
#include "Passes/ShadowMapPass.h"
#include "Resources/Shader.h"
#include "Scene/Camera.h"

LightManager::~LightManager()
{
   SAFE_RELEASE(m_Buffer);
   SAFE_RELEASE(m_OneShadowMapCBuffer);
}

void LightManager::Update(Graphics& graphics)
{
    if (m_Buffer == nullptr)
    {
        m_Buffer = graphics.CreateBuffer(sizeof(LightProperties), D3D11_BIND_CONSTANT_BUFFER, &m_LightProps);
    }
    else
    {
        graphics.UpdateBuffer(m_Buffer, &m_LightProps);
    }
}

LightManager& LightManager::AddLight(Light& light)
{
    if (m_NumLights < MAX_LIGHTS)
    {
        light.m_Status = LightStatus::Enabled;
        m_LightProps.m_Lights[m_NumLights] = light;
        m_NumLights++;
        return *this;
    }
    else
    {
        throw std::exception("LightManager:: Too many lights added");
    }
}

Light& LightManager::GetLight(unsigned int index)
{
    if (index < MAX_LIGHTS)
    {
        return m_LightProps.m_Lights[index];
    }
    else
    {
        throw std::exception("LightManager:: light index out of range");
    }
}


LightManager& LightManager::SetEyePosition(XMVECTOR eyePositionVec)
{
    XMStoreFloat4(&(m_LightProps.m_EyePosition), eyePositionVec);
    return *this;
}

LightManager& LightManager::SetGlobalAmbient(XMFLOAT4 colour)
{
    m_LightProps.m_GlobalAmbient = colour;
    return *this;
}

XMFLOAT4& LightManager::GetEyePosition()
{
    return m_LightProps.m_EyePosition;
}

void LightManager::Use(ID3D11DeviceContext* deviceContext, UINT slot /* = 0*/)
{
    deviceContext->PSSetConstantBuffers(slot, 1, &m_Buffer);
}

void LightManager::SetLightWithShadows(Graphics& graphics, unsigned int index, ShadowMapRenderDesc& desc)
{
    Light& light = GetLight(index);
    if (light.m_LightType != LightType::DirectionalLight)
    {
        throw std::exception("LightManager:: Only support static directional light");
    }

    if (hasLightWithShadows)
    {
        throw std::exception("LightManager:: Only support one shadow map");
    }

    light.m_Status = LightStatus::Static_Shadows;

    OrthographicCamera shadowMapCamera;
    shadowMapCamera.m_EyePosition = desc.m_EyePosition;
    shadowMapCamera.m_LookAt = desc.m_LookAt;
    shadowMapCamera.m_NearZ = desc.m_NearZ;
    shadowMapCamera.m_FarZ = desc.m_FarZ;
    shadowMapCamera.m_ViewHeight = desc.m_ViewHeight;
    shadowMapCamera.m_ViewWidth = desc.m_ViewWidth;

    RECT& clientRect = graphics.m_ClientRect;
       ShadowMapConstant shadowMapConstant = {
         desc.m_TextureWidth,
         desc.m_TextureHeight,
         XMMatrixMultiply(shadowMapCamera.CalculateView(), shadowMapCamera.CalculateProjection())
    };
    m_OneShadowMapDesc = desc;
    m_OneShadowMapCBuffer = graphics.CreateBuffer(sizeof(ShadowMapConstant), D3D11_BIND_CONSTANT_BUFFER, &shadowMapConstant);
    m_ShadowMapPass = std::make_unique<ShadowMapPass>(graphics);
    Texture* shadowMapTexture = Texture::CreateTexture(graphics, desc.m_TextureWidth, desc.m_TextureHeight, "Shadow Map",
        DXGI_FORMAT_R32_TYPELESS, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
    shadowMapTexture->CreateRTV(graphics, DXGI_FORMAT_R32_FLOAT);
    shadowMapTexture->CreateSRV(graphics, DXGI_FORMAT_R32_FLOAT);
    m_OneShadowMapTexture = std::unique_ptr<Texture>(shadowMapTexture);
    hasLightWithShadows = true;
}

void LightManager::RenderAnyShadowMap(Graphics& graphics, Scene& scene)
{
    if (hasLightWithShadows)
    {
        m_ShadowMapPass->Render(graphics, scene, *m_OneShadowMapTexture, m_OneShadowMapDesc);
    }
}
