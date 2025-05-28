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

#pragma once

#include "platform.h"
#if _DEBUG
    #pragma comment( lib, "C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin-latest\\Debug\\Assimp-latest-Engine.lib" )
#else
    #pragma comment( lib, "C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin-latest\\Release\\Assimp-latest-Engine.lib" )
#endif

#if _DEBUG
    #pragma comment(lib, "C:\\WoMA3Dengine\\ThirdParty\\external\\assimp-build\\lib\\Debug\\assimp-vc143-mtd.lib")
#else
    #pragma comment(lib, "C:\\WoMA3Dengine\\ThirdParty\\external\\assimp-build\\lib\\Release\\assimp-vc143-mtd.lib")
#endif

#include "../Engine/Source/PPG.h"
#include <math.h>

class MyDemo : public Demo
{
public:
    std::unique_ptr<SceneModel> assimpSceneModel;
    std::unique_ptr<SceneModel> assimpSceneModel2;
    Scene scene;
#if DX_ENGINE_LEVEL >= 84 && defined (SCENE_SKIN)
    Scene scene2;
#endif

private:

    std::unique_ptr<Sampler> linearSampler;
    std::unique_ptr<Sampler> pointSampler;

    std::unique_ptr<GBufferPass> gBufferPass;

    std::unique_ptr<Texture> colour;
    std::unique_ptr<Texture> diffuse;
    std::unique_ptr<Texture> metalRough;
    std::unique_ptr<Texture> normals;
    std::unique_ptr<Texture> emissive;

    AnimationJob animJob;

    std::unique_ptr<Texture> LoadTextureFromPath(Graphics& graphics, const LPCWSTR& path)
    {
        Texture* tex = Texture::LoadTextureFromPath(graphics, path);
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
        normals = CreateRenderTexture(graphics, clientWidth, clientHeight, "Normals", DXGI_FORMAT_R16G16B16A16_FLOAT);
        metalRough = CreateRenderTexture(graphics, clientWidth, clientHeight, "MetalRough", DXGI_FORMAT_R16G16B16A16_FLOAT);
        emissive = CreateRenderTexture(graphics, clientWidth, clientHeight, "Emissive", DXGI_FORMAT_R16G16B16A16_FLOAT);

        /////////////////////////brickTexture = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_COLOR.jpg");
        /////////////////////////brickNormalMap = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_NORM.jpg");

        linearSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
        pointSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);

		//VertexShader.cso
        gBufferPass = std::make_unique<GBufferPass>(graphics, *diffuse.get(), *metalRough.get(), *normals.get(), *emissive.get());
        
        // Lighting
        auto lightColour = XMFLOAT4(5.0f, 5.0f, 5.0f, 1.0f);

        Light pointLight;
        pointLight.m_Color = XMFLOAT4(1, 1, 0, 0);
        pointLight.m_Position = XMFLOAT4(4, 3, 0, 0);
        pointLight.m_LightType = LightType::PointLight;

        Light dirLight;
        dirLight.m_Color = lightColour;
        dirLight.m_Direction = XMFLOAT4(-1, -1, 1, 0);
        dirLight.m_LightType = LightType::DirectionalLight;

        Light spotLight;
        spotLight.m_Color = XMFLOAT4(Colors::Magenta);
        spotLight.m_Direction = XMFLOAT4(0, -1, 0, 0);
        spotLight.m_Position = XMFLOAT4(3, 2, 0, 0);
        spotLight.m_LightType = LightType::SpotLight;
        spotLight.m_SpotAngle = 3.142f / 4.0f;

        scene.lightManager
            .AddLight(dirLight)
            .AddLight(pointLight)
            .AddLight(spotLight)
            .SetGlobalAmbient(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
#if DX_ENGINE_LEVEL >= 84 && defined (SCENE_SKIN)
        scene2.lightManager
            .AddLight(dirLight)
            .AddLight(pointLight)
            .AddLight(spotLight)
            .SetGlobalAmbient(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
#endif
		
        scene.Start(graphics);
#if DX_ENGINE_LEVEL >= 84 && defined (SCENE_SKIN)
        scene2.Start(graphics);
#endif
        auto deviceContext = graphics.m_DeviceContext;
        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);
    }

    void Update(Graphics& graphics, float deltaTime) override
    {

        animJob.UpdateTimeElapsed(scene, deltaTime);
#if DX_ENGINE_LEVEL >= 84 && defined (SCENE_SKIN)
        animJob.UpdateTimeElapsed(scene2, deltaTime);
#endif
    }

	void Render(Graphics& graphics) 
	{

        #define m_Driver driverList[SystemHandle->AppSettings->DRIVER]
        {
            XMMATRIX world = XMMatrixIdentity();
            //Scale:
            //_11+		BoneIds	{x=0.00000000 y=0.00000000 z=0.00000000 ...}	DirectX::XMFLOAT4

            //_22
            //_33
            world.r[0].m128_f32[0] = world.r[1].m128_f32[1] = world.r[2].m128_f32[2] = 0.2f;
            XMMATRIX rotX = XMMatrixRotationX(PI / 2);
            world *= rotX;
            
            //Translate:
            world.r[3].m128_f32[0] = 39;    //_41: X
            world.r[3].m128_f32[1] = 0.4f;  //_42: Y 
            world.r[3].m128_f32[2] = 20;    //_43: Z

            scene.UpdateModel(graphics, world);
        }
        gBufferPass->Render(graphics, scene);

#if defined SCENE_SKIN
        {
            XMMATRIX world = XMMatrixIdentity();
            //Scale:
            //_11
            //_22
            //_33
            world.r[0].m128_f32[0] = world.r[1].m128_f32[1] = world.r[2].m128_f32[2] = 0.7f;

            XMMATRIX rotX = XMMatrixRotationX(PI/2);
            world *= rotX;
            XMMATRIX rotY = XMMatrixRotationY(PI +SystemHandle->m_Application->m_characterPos->m_rotationY);
            world *= rotY;
            //Translate:
            world.r[3].m128_f32[0] = SystemHandle->m_Application->m_characterPos->m_positionX;  //_41: X
            world.r[3].m128_f32[1] = SystemHandle->m_Application->m_characterPos->m_positionY;  //_42: Y 
            world.r[3].m128_f32[2] = SystemHandle->m_Application->m_characterPos->m_positionZ;  //_43: Z

            scene2.UpdateModel(graphics, world);
        }
        gBufferPass->Render(graphics, scene2);
#endif

	}

    void End()
    {
    }
};
