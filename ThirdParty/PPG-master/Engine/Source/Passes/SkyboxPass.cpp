#include "stdafx.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "SkyboxPass.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"

XMVECTOR forward[6] =
{
    { 1, 0, 0, 0 },
    { -1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, -1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, -1, 0 },
};

XMVECTOR up[6] =
{
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, -1, 0 },
    { 0, 0, 1, 0 },
    { 0, 1, 0,  0 },
    { 0, 1, 0, 0 },
};

SkyboxPass::SkyboxPass(Graphics& graphics, Texture& renderTarget, const LPCWSTR& fileName, float size /*=50*/) :
    m_RenderTarget(renderTarget)
{
    std::vector<WORD> skyboxIndices =
    {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9, 10,  8, 10, 11,
       12, 13, 14, 12, 14, 15,
       16, 17, 18, 16, 18, 19,
       20, 21, 22, 20, 22, 23
    };

    skyboxMesh = std::make_unique<Mesh>(CubeVertices(), std::move(skyboxIndices), graphics);
    scaleMatrix = XMMatrixScaling(size, size, size);
    shader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Skybox.vs.cso", L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Skybox.ps.cso", graphics);
    cubeToEnvMapShader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Skybox.vs.cso", L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\IrradianceMap.ps.cso", graphics);
    prefilterShader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Skybox.vs.cso", L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\SpecularPreFilter.ps.cso", graphics);
    brdfIntegrateShader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Fullscreen.vs.cso", L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\IntegrateBRDF.ps.cso", graphics);

    WCHAR ext[_MAX_EXT];
    _wsplitpath_s(fileName, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
    if (_wcsicmp(ext, L".dds") == 0)
    {
        skyboxCubeMap = std::unique_ptr<Texture>(Texture::LoadTextureFromPath(graphics, fileName));
        m_CubeMapGenerationRequired = false;
    }
    else
    {
        skyboxTexture = std::unique_ptr<Texture>(Texture::LoadTextureFromPath(graphics, fileName));
        rectToCubeMapShader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Skybox.vs.cso", L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\RectToCubeMap.ps.cso", graphics);
        skyboxCubeMap = std::unique_ptr<Texture>(Texture::CreateTextureCube(
            graphics, m_CubeMapSize, "Cube Map", DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE));
        skyboxCubeMap->CreateSRV(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_SRV_DIMENSION_TEXTURECUBE);
        skyboxCubeMap->CreateTextureCubeRTVs(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT);
        m_CubeMapGenerationRequired = true;
    }
    m_EnvironmentMap = std::unique_ptr<Texture>(Texture::CreateTextureCube(
        graphics, m_CubeMapSize, "Env Map", DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE));
    m_EnvironmentMap->CreateSRV(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_SRV_DIMENSION_TEXTURECUBE);
    m_EnvironmentMap->CreateTextureCubeRTVs(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT);

    m_SpecularMap = std::unique_ptr<Texture>(Texture::CreateTextureCube(
        graphics, m_CubeMapSize, "Specular Map", DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 6));
    m_SpecularMap->CreateSRV(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_SRV_DIMENSION_TEXTURECUBE, 6);
    m_SpecularMap->CreateTextureCubeRTVs(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT, 6);

    m_BrdfLUT = std::unique_ptr<Texture>(Texture::CreateTexture(
        graphics, 512, 512, "Brdf LUT", DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE));
    m_BrdfLUT->CreateSRV(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT);
    m_BrdfLUT->CreateRTV(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT);
}

void SkyboxPass::GenerateCubeMap(Graphics& graphics, Scene& scene)
{
    if (!m_CubeMapGenerationRequired)
    {
        return;
    }

    auto deviceContext = graphics.m_DeviceContext;
    D3D11_VIEWPORT viewport = { 0 };
    viewport.Width = m_CubeMapSize;
    viewport.Height = m_CubeMapSize;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    deviceContext->RSSetViewports(1, &viewport);

    for (int i = 0; i < 6; ++i)
    {
        graphics.SetRenderTarget(skyboxCubeMap->GetRTV(i), false);
        OrthographicCamera ortho;
        ortho.m_EyePosition = XMVectorSet(0, 0, 0, 0);
        ortho.m_LookAt = forward[i];
        ortho.m_Up = up[i];
        ortho.m_NearZ = 0;
        ortho.m_FarZ = 10;
        ortho.m_ViewHeight = 2;
        ortho.m_ViewWidth = 2;
        rectToCubeMapShader->Use(deviceContext);
        skyboxTexture->UseSRV(deviceContext, 0);
        scene.UseModel(graphics);
        scene.UseCamera(graphics, ortho);
        scene.UpdateModel(graphics, XMMatrixIdentity());

        skyboxMesh->Draw(deviceContext);
    }

    graphics.SetUp();
    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
    m_CubeMapGenerationRequired = false;
    GenerateEnvMap(graphics, scene);
}

Texture* SkyboxPass::GenerateEnvMap(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    D3D11_VIEWPORT viewport = { 0 };
    viewport.Width = m_CubeMapSize;
    viewport.Height = m_CubeMapSize;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    deviceContext->RSSetViewports(1, &viewport);

    for (int i = 0; i < 6; ++i)
    {
        graphics.SetRenderTarget(m_EnvironmentMap->GetRTV(i), false);
        OrthographicCamera ortho;
        ortho.m_EyePosition = XMVectorSet(0, 0, 0, 0);
        ortho.m_LookAt = forward[i];
        ortho.m_Up = up[i];
        ortho.m_NearZ = 0;
        ortho.m_FarZ = 10;
        ortho.m_ViewHeight = 2;
        ortho.m_ViewWidth = 2;
        cubeToEnvMapShader->Use(deviceContext);
        skyboxCubeMap->UseSRV(deviceContext, 0);
        scene.UseModel(graphics);
        scene.UseCamera(graphics, ortho);
        scene.UpdateModel(graphics, XMMatrixIdentity());
        skyboxMesh->Draw(deviceContext);
        deviceContext->Flush();
    }

    graphics.SetUp();
    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
    return m_EnvironmentMap.get();
}

Texture* SkyboxPass::GenerateEnvPreFilter(Graphics& graphics, Scene& scene)
{
    int mapSize = m_CubeMapSize;

    _declspec(align(16)) struct PrefilterCBuffer
    {
        float roughness;
    };
    ID3D11Buffer* buffer = graphics.CreateBuffer(sizeof(PrefilterCBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr);

    PrefilterCBuffer cbuffer;
    auto deviceContext = graphics.m_DeviceContext;
    deviceContext->PSSetConstantBuffers(0, 1, &buffer);
    for (int i = 0; i < 6; ++i)
    {
        cbuffer.roughness = (float)i / 5.0;
        graphics.UpdateBuffer(buffer, &cbuffer);
        D3D11_VIEWPORT viewport = { 0 };
        viewport.Width = mapSize;
        viewport.Height = mapSize;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        deviceContext->RSSetViewports(1, &viewport);
        for (int j = 0; j < 6; ++j)
        {
            graphics.SetRenderTarget(m_SpecularMap->GetRTV(i * 6 + j), false);
            OrthographicCamera ortho;
            ortho.m_EyePosition = XMVectorSet(0, 0, 0, 0);
            ortho.m_LookAt = forward[j];
            ortho.m_Up = up[j];
            ortho.m_NearZ = 0;
            ortho.m_FarZ = 10;
            ortho.m_ViewHeight = 2;
            ortho.m_ViewWidth = 2;
            prefilterShader->Use(deviceContext);
            skyboxCubeMap->UseSRV(deviceContext, 0);
            scene.UseModel(graphics);
            scene.UseCamera(graphics, ortho);
            scene.UpdateModel(graphics, XMMatrixIdentity());
            skyboxMesh->Draw(deviceContext);
        }
        mapSize /= 2;
    }
    graphics.SetUp();
    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
    SAFE_RELEASE(buffer);
    return m_SpecularMap.get();
}

Texture* SkyboxPass::GenerateBrdfLUT(Graphics& graphics, Scene& scene)
{
    D3D11_VIEWPORT viewport = { 0 };
    viewport.Width = 512;
    viewport.Height = 512;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    auto deviceContext = graphics.m_DeviceContext;
    deviceContext->RSSetViewports(1, &viewport);
    graphics.SetRenderTarget(m_BrdfLUT->GetRTV(), false);
    brdfIntegrateShader->Use(deviceContext);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    deviceContext->Draw(4, 0);
    graphics.SetUp();
    graphics.UnbindRenderTargetView();
    return m_BrdfLUT.get();
}

void SkyboxPass::Render(Graphics& graphics, Scene& scene)
{
    graphics.SetRenderTarget(m_RenderTarget);

    auto deviceContext = graphics.m_DeviceContext;
    scene.UseCamera(graphics, scene.m_MainCamera);
    scene.UseModel(graphics);

    auto model = XMMatrixMultiply(scaleMatrix, XMMatrixTranslationFromVector(scene.m_MainCamera.m_EyePosition));
    shader->Use(deviceContext);
    scene.UpdateModel(graphics, model);
    skyboxCubeMap->UseSRV(deviceContext, 0);
    skyboxMesh->Draw(deviceContext);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
}