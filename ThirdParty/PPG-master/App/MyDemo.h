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
#if _DEBUG
#pragma comment( lib, "C://WoMA3Dengine//ThirdParty//PPG-master//Bin//Debug//AssimpEngine.lib" )
#else
#pragma comment( lib, "C://WoMA3Dengine//ThirdParty//PPG-master//Bin//Release//AssimpEngine.lib" )
#endif
#pragma comment(lib, "C://WoMA3Dengine//ThirdParty//PPG-master//External//assimp//lib//assimp-vc142-mtd.lib")

#include "../Engine/Source/PPG.h"
#include <math.h>

class MyDemo : public Demo
{
public:
    std::unique_ptr<SceneModel> assimpSceneModel;
    std::unique_ptr<SceneModel> assimpSceneModel2;
    Scene scene;

private:

    std::unique_ptr<Sampler> linearSampler;
    std::unique_ptr<Sampler> pointSampler;


    //  std::unique_ptr<BlitPass> blitPass;
	BlitPass* blitPass;
    std::unique_ptr<GBufferPass> gBufferPass;
    std::unique_ptr<DeferredPass> deferredPass;
    std::unique_ptr<SSAOPass> ssaoPass;
    std::unique_ptr<ToneMapPass> toneMapPass;

    std::unique_ptr<Texture> colour;
    std::unique_ptr<Texture> diffuse;
    std::unique_ptr<Texture> metalRough;
    std::unique_ptr<Texture> normals;
    std::unique_ptr<Texture> emissive;
    std::unique_ptr<Texture> ambientOcclusion;
    std::unique_ptr<Texture> toneMappedColour;

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
    void Start(Graphics& graphics, TCHAR* model) override
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

        toneMappedColour = CreateRenderTexture(graphics, clientWidth, clientHeight, "Tone Mapped Colour", DXGI_FORMAT_R16G16B16A16_FLOAT);
        Texture* ao = Texture::CreateTexture(graphics, clientWidth, clientHeight, "Ambient Occlusion",
            DXGI_FORMAT_R16_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
        ao->CreateRTV(graphics, DXGI_FORMAT_R16_UNORM);
        ao->CreateSRV(graphics, DXGI_FORMAT_R16_UNORM);
        ambientOcclusion = std::unique_ptr<Texture>(ao);

        /////////////////////////brickTexture = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_COLOR.jpg");
        /////////////////////////brickNormalMap = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_NORM.jpg");

        linearSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
        pointSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);

        auto& colourTexture = *(colour.get());
        auto& toneMappedTexture = *(toneMappedColour.get());
		//VertexShader.cso
        gBufferPass = std::make_unique<GBufferPass>(graphics, *diffuse.get(), *metalRough.get(), *normals.get(), *emissive.get());

		//Fullscreen.vs.cso
        deferredPass = std::make_unique<DeferredPass>(graphics, colourTexture, *diffuse.get(), *metalRough.get(), *normals.get(), *emissive.get());

		//Fullscreen.vs.cso
        ssaoPass = std::make_unique<SSAOPass>(graphics, *ao, *(graphics.m_DepthStencilBuffer).get(), *normals.get());

		//Fullscreen.vs.cso
        toneMapPass = std::make_unique<ToneMapPass>(graphics, colourTexture, toneMappedTexture);
		//Fullscreen.vs.cso
		blitPass = new BlitPass(graphics, toneMappedTexture, *(graphics.m_BackBuffer.get()));

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
        spotLight.m_SpotAngle = 3.142 / 4.0;

        scene.lightManager
            .AddLight(dirLight)
            .AddLight(pointLight)
            .AddLight(spotLight)
            .SetGlobalAmbient(XMFLOAT4(0.1, 0.1, 0.1, 1));
		
        scene.Start(graphics);

        auto deviceContext = graphics.m_DeviceContext;
        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);
    }

    void Update(Graphics& graphics, float deltaTime) override
    {

        animJob.UpdateTimeElapsed(scene, deltaTime);		//!!!!!!!!
    }

	void Render(Graphics& graphics) 
	{
		gBufferPass->Render(graphics, scene);
	}

    void End()
    {
    }
};
