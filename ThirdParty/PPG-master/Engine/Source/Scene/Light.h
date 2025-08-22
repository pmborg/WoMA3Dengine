// --------------------------------------------------------------------------------------------
// Filename: Light.h
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

#pragma once
#include "stdafx.h"
#include "Passes/ShadowMapPass.h"
#define MAX_LIGHTS 4

enum LightType
{
    DirectionalLight = 0,
    PointLight = 1,
    SpotLight = 2
};

enum LightStatus
{
    Disabled = 0,
    Enabled = 1,
    Static_Shadows = 2
};

using namespace DirectX;

_declspec(align(16)) struct Light
{
    XMFLOAT4 m_Position;
    XMFLOAT4 m_Direction;
    XMFLOAT4 m_Color;

    float m_ConstantAttenuation = 1.0f;
    float m_LinearAttenuation = 0.09f;
    float m_QuadAttenuation = 0.032f;
    float m_SpotAngle;

    int m_LightType;
    int m_Status = LightStatus::Disabled;
};

_declspec(align(16)) struct LightProperties
{
    XMFLOAT4 m_EyePosition;
    XMFLOAT4 m_GlobalAmbient;
    Light m_Lights[MAX_LIGHTS];
};

class Graphics;
class Texture;
class Scene;
class ForwardPass;

class LightManager
{
public:
    LightManager() = default;
    ~LightManager();
    void Update(ID3D11DeviceContext* pContext, Graphics& graphics);
    void Use(ID3D11DeviceContext* deviceContext, UINT slot = 0);
    Light& GetLight(unsigned int index);
    XMFLOAT4& GetEyePosition();
    LightManager& AddLight(Light& light);
    LightManager& SetGlobalAmbient(XMFLOAT4 colour);
    LightManager& SetEyePosition(XMVECTOR eyePositionVec);
    unsigned int m_NumLights = 0;
private:
    ID3D11Buffer* m_Buffer;
    LightProperties m_LightProps;

    friend class ForwardPass;
    friend class DeferredPass;

    bool hasLightWithShadows = false;
    ID3D11Buffer* m_OneShadowMapCBuffer;
    std::unique_ptr<Texture> m_OneShadowMapTexture;
};
