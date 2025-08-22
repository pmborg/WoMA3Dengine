// --------------------------------------------------------------------------------------------
// Filename: SpritePass.h
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
#include "Resources/Mesh.h"
#include "Scene/Camera.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;
class Sampler;

class SpritePass
{
public:
    SpritePass(Graphics& graphics, Texture& renderTarget);
    ~SpritePass();
    void Render(Graphics& graphics, Scene& scene);
private:
    Texture& m_RenderTarget;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Mesh> m_QuadMesh;
    ID3D11DepthStencilState* m_NoWriteDepthStencil;
    OrthographicCamera m_Camera;
};


