// --------------------------------------------------------------------------------------------
// Filename: GBufferPass.h
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

class GBufferPass
{
public:
    GBufferPass(Graphics& graphics, Texture& diffuse, Texture& metalRough, Texture& normals, Texture& emissive);
    ~GBufferPass();
    void Render(ID3D11DeviceContext* pContext, Graphics& graphics, Scene& scene);

    Texture& m_Diffuse;
    Texture& m_MetalRough;
    Texture& m_Normals;
    Texture& m_Emissive;

private:
    static const UINT NUM_RENDER_TARGETS = 4;
    std::unique_ptr<Shader> shader;

    ID3D11Buffer* m_PBRMaterialBuffer = nullptr;
    //ID3D11Buffer* m_LightBuffer = nullptr;
    ID3D11Buffer* m_BoneBuffer = nullptr;
    ID3D11RenderTargetView* m_RenderTargets[NUM_RENDER_TARGETS];
};
