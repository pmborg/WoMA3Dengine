// --------------------------------------------------------------------------------------------
// Filename: ShadowMapPass.h
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

#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"
#include "Scene/Camera.h"

using namespace DirectX;

class Graphics;
class Texture;
class Scene;

_declspec(align(16)) struct ShadowMapConstant
{
    float mapWidth;
    float mapHeight;
    XMMATRIX lightViewProjection;
};

struct ShadowMapRenderDesc
{
    XMVECTOR m_EyePosition;
    XMVECTOR m_LookAt;
    float m_NearZ = 0.1;
    float m_FarZ = 200;
    float m_ViewWidth = 1;
    float m_ViewHeight = 1;
    float m_TextureWidth;
    float m_TextureHeight;
};

class ShadowMapPass
{
public:
    ShadowMapPass(Graphics& graphics);
    void Render(Graphics& graphics, Scene& scene, Texture& shadowMapTexture, ShadowMapRenderDesc& desc);
private:
    std::unique_ptr<Shader> shader;
};
