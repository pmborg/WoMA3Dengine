// --------------------------------------------------------------------------------------------
// Filename: MyDemo.h
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
// WomaIntegrityCheck = 1234525256;

#pragma once

#include "platform.h"
#if _DEBUG
    #pragma comment( lib, "C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin-latest\\Debug\\Assimp-latest-Engine.lib" )
#else
    #pragma comment( lib, "C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Release\\Assimp-latest-Engine.lib" )
#endif

#if _DEBUG
    #pragma comment(lib, "C:\\WoMA3Dengine\\ThirdParty\\external\\assimp-build\\lib\\Debug\\assimp-vc143-mtd.lib")
#else
    #pragma comment(lib, "C:\\WoMA3Dengine\\ThirdParty\\external\\assimp-build\\lib\\Release\\assimp-vc143-mt.lib")
#endif

#include "../Engine/Source/PPG.h"
#include <math.h>

class MyDemo : public Demo
{
public:
    AnimationModelLoader animJob;
    std::unique_ptr<GBufferPass> gBufferPass;
private:
    std::unique_ptr<Texture> colour;
    std::unique_ptr<Texture> diffuse;
    std::unique_ptr<Texture> metalRough;
    std::unique_ptr<Texture> normals;
    std::unique_ptr<Texture> emissive;

    std::unique_ptr<Sampler> linearSampler;
    std::unique_ptr<Sampler> pointSampler;

    std::unique_ptr<Texture> LoadTextureFromPath(UINT this_level, UINT modeltype, Graphics& graphics, LPCWSTR& path)
    {
        Texture* tex = Texture::LoadTextureFromPath(this_level, modeltype, graphics, path);
        return std::unique_ptr<Texture>(tex);
    }

    std::unique_ptr<Texture> CreateRenderTexture(Graphics& graphics, int width, int height, const std::string name, DXGI_FORMAT format)
    {
        Texture* tex = Texture::CreateTexture(graphics, width, height, name, format, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
        tex->CreateRTV(graphics, format);
        tex->CreateSRV(graphics, format);
        return std::unique_ptr<Texture>(tex);
    }

public:
    void Start(Graphics& graphics) override
    {
        // Rendering Stuff
        auto& clientRect = graphics.m_ClientRect;
        unsigned int clientWidth = clientRect.right - clientRect.left;
        unsigned int clientHeight = clientRect.bottom - clientRect.top;

        colour = CreateRenderTexture(graphics, clientWidth, clientHeight, "Colour", DXGI_FORMAT_R16G16B16A16_FLOAT);
        diffuse = CreateRenderTexture(graphics, clientWidth, clientHeight, "Diffuse", DXGI_FORMAT_R16G16B16A16_FLOAT);
        metalRough = CreateRenderTexture(graphics, clientWidth, clientHeight, "MetalRough", DXGI_FORMAT_R16G16B16A16_FLOAT);
        normals = CreateRenderTexture(graphics, clientWidth, clientHeight, "Normals", DXGI_FORMAT_R16G16B16A16_FLOAT);
        emissive = CreateRenderTexture(graphics, clientWidth, clientHeight, "Emissive", DXGI_FORMAT_R16G16B16A16_FLOAT);

		//VertexShader.cso
        gBufferPass = std::make_unique<GBufferPass>(graphics, *diffuse.get(), *metalRough.get(), *normals.get(), *emissive.get());

        linearSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
        pointSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);

        auto deviceContext = graphics.m_DeviceContext;
        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);
    }
    
    void Update(Graphics& graphics, float deltaTime) override
    {
    }

	void Render(Graphics& graphics) 
	{
	}

    void End()
    {
    }
};
