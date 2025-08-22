// --------------------------------------------------------------------------------------------
// Filename: Light.cpp
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
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG
// WomaIntegrityCheck = 1234525217;

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

void LightManager::Update(ID3D11DeviceContext* pContext, Graphics& graphics)
{
    if (m_Buffer == nullptr)
    {
        m_Buffer = graphics.CreateBuffer(sizeof(LightProperties), D3D11_BIND_CONSTANT_BUFFER, &m_LightProps);
    }
    else
    {
        graphics.UpdateBuffer(pContext, m_Buffer, &m_LightProps);
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
}

Light& LightManager::GetLight(unsigned int index)
{
    if (index < MAX_LIGHTS)
    {
        return m_LightProps.m_Lights[index];
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

