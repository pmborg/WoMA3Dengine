// --------------------------------------------------------------------------------------------
// Filename: SkyboxPass.h
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

class Texture;
class Mesh;
class Shader;
class Graphics;
class Scene;

class SkyboxPass
{
public:
    SkyboxPass(Graphics& graphics, Texture& renderTarget, const LPCWSTR& fileName, float size = 50);
    void GenerateCubeMap(Graphics& graphics, Scene& scene);
    Texture* GenerateEnvMap(Graphics& graphics, Scene& scene);
    Texture* GenerateEnvPreFilter(Graphics& graphics, Scene& scene);
    Texture* GenerateBrdfLUT(Graphics& graphics, Scene& scene);
    std::unique_ptr<Texture> m_EnvironmentMap;
    std::unique_ptr<Texture> m_SpecularMap;
    std::unique_ptr<Texture> m_BrdfLUT;
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Shader> rectToCubeMapShader;
    std::unique_ptr<Shader> cubeToEnvMapShader;
    std::unique_ptr<Shader> prefilterShader;
    std::unique_ptr<Shader> brdfIntegrateShader;
    std::unique_ptr<Mesh> skyboxMesh;
    std::unique_ptr<Texture> skyboxTexture;
    std::unique_ptr<Texture> skyboxCubeMap;
    XMMATRIX scaleMatrix;
    Texture& m_RenderTarget;
    bool m_CubeMapGenerationRequired = false;
    int m_CubeMapSize = 512;
};

