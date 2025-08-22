// --------------------------------------------------------------------------------------------
// Filename: SSAOPass.h
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
#include "Resources/Shader.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;
class Sampler;

_declspec(align(16))
struct SSAOBuffer
{
    XMMATRIX m_ViewProjection;
    XMMATRIX m_InverseViewProjection;
    XMFLOAT4 m_Kernel[64];
    XMFLOAT4 m_CameraPosition;
    float m_Radius;
    XMFLOAT2 m_WindowSize;
};

class SSAOPass
{
public:
    SSAOPass(Graphics& graphics, Texture& renderTarget, Texture& depth, Texture& normals);
    ~SSAOPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Noise;
    Texture& m_Depth;
    Texture& m_Normals;
    Texture& m_RenderTarget;
    ID3D11Buffer* m_Buffer = nullptr;
    SSAOBuffer m_SSAOBuffer;
};

