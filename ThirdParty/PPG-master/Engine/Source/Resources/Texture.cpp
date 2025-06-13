// --------------------------------------------------------------------------------------------
// Filename: Texture.cpp
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

#include "stdafx.h"
#include "OSengine.h"
#include "DX11Class.h"
#include "Texture.h"
#include "LowLevel/Graphics.h"
#include "DirectXTex.h"

Texture* Texture::CreateTexture(Graphics& graphics, int width, int height, const std::string& name,
    DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data /*= NULL*/)
{
    D3D11_TEXTURE2D_DESC textureDesc = { 0 }; //ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the render target texture description.
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = texFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.BindFlags = bindFlags;
    ID3D11Texture2D* texturePtr;
    HRESULT result = graphics.m_Device->CreateTexture2D(&textureDesc, data, &texturePtr);
    if (FAILED(result))
    {
        return nullptr;
    }

    return new Texture(texturePtr, name);
}

Texture* Texture::CreateTextureDepthStencil(Graphics& graphics, int width, int height, const std::string& name,
    DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data /*= NULL*/)
{
    #define m_driver11 ((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])
    ID3D11Texture2D* texturePtr = ((DirectX::DX11Class*)m_driver11)->m_depthStencilBuffer;

    return new Texture(texturePtr, name);
}

Texture* Texture::CreateTextureCube(Graphics& graphics, int size, const std::string& name,
    DXGI_FORMAT texFormat, UINT bindFlags, UINT mipLevels /* = 1*/, D3D11_SUBRESOURCE_DATA* data /*= NULL*/)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = size;
    textureDesc.Height = size;
    textureDesc.MipLevels = mipLevels;
    textureDesc.ArraySize = 6; // 6 faces
    textureDesc.Format = texFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = bindFlags;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    ID3D11Texture2D* texturePtr;
    HRESULT result = graphics.m_Device->CreateTexture2D(&textureDesc, data, &texturePtr);
    if (FAILED(result))
    {
        return nullptr;
    }

    return new Texture(texturePtr, name);
}

extern Texture* LoadTextureFromPathFBX(Graphics& graphics, LPCWSTR& texturePath);

Texture* Texture::LoadTextureFromPath(Graphics& graphics, LPCWSTR& texturePath)
{
    return LoadTextureFromPathFBX(graphics, texturePath);

    std::wstring texturePath_ = texturePath;

    // Find the position of "../../AppData/Local"
    std::wstring marker = L"../../AppData/Local";
    size_t pos = texturePath_.find(marker);

    // Get everything before "../../AppData/Local"
    std::wstring beforeAppData;
    bool fix = false;
    if (pos != std::wstring::npos) {
        beforeAppData = texturePath_.substr(0, pos);
        fix = true;
    }

    // Get the file name only
    size_t lastSlash = texturePath_.find_last_of(L"/\\");
    std::wstring fileName;
    if (lastSlash != std::wstring::npos) {
        fileName = texturePath_.substr(lastSlash + 1);
    }
    beforeAppData.append(fileName);

    if (fix)
        texturePath = beforeAppData.c_str();
    else
        texturePath = texturePath_.c_str();

    std::filesystem::path filePath(texturePath);
    if (!std::filesystem::exists(filePath))
    {
        return nullptr;
    }

    WCHAR ext[_MAX_EXT];
    _wsplitpath_s(texturePath, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
    HRESULT hr;
    ScratchImage image;
    TexMetadata metadata;

    if (_wcsicmp(ext, L".dds") == 0)
    {
        hr = LoadFromDDSFile(texturePath, DDS_FLAGS_FORCE_RGB, &metadata, image);
    }
    else if (_wcsicmp(ext, L".tga") == 0)
    {
        hr = LoadFromTGAFile(texturePath, &metadata, image);
    }
    else if (_wcsicmp(ext, L".hdr") == 0)
    {
        hr = LoadFromHDRFile(texturePath, &metadata, image);
    }
    else
    {
        hr = LoadFromWICFile(texturePath, WIC_FLAGS_NONE, &metadata, image);
    }

    if (FAILED(hr))
    {
        return nullptr;
    }

    Texture* tex = new Texture();

    hr = CreateShaderResourceView(graphics.m_Device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &tex->m_TextureSRV);
    if (FAILED(hr))
    {
        throw std::exception("Texture::Failed to create texture");
    }
    return tex;
}


Texture::Texture(ID3D11Texture2D* texture, const std::string& name) :
    m_Texture(texture),
    m_Name(name)
{
    SetDebugName(m_Texture, name);
}

bool Texture::CreateSRV(Graphics& graphics, DXGI_FORMAT texFormat, D3D11_SRV_DIMENSION viewDimension /*= D3D11_SRV_DIMENSION_TEXTURE2D*/, UINT mipLevels /*= 1*/)
{
    #define m_driver11 ((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])
    return true;
}

bool Texture::CreateRTV(Graphics& graphics, DXGI_FORMAT texFormat)
{
#define m_driver11 ((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])

    ID3D11RenderTargetView* rtv = ((DirectX::DX11Class*)m_driver11)->DX11windowsArray[0].m_renderTargetView;
    m_TextureRTVs.push_back(rtv);
    return true;
}

bool Texture::CreateTextureCubeRTVs(Graphics& graphics, DXGI_FORMAT texFormat, UINT mipLevels/* = 1*/)
{
    for (UINT mipSlice = 0; mipSlice < mipLevels; ++mipSlice)
    {
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        renderTargetViewDesc.Format = texFormat;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        renderTargetViewDesc.Texture2DArray.MipSlice = mipSlice;
        renderTargetViewDesc.Texture2DArray.ArraySize = 1;
        for (UINT i = 0; i < 6; ++i)
        {
            renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
            ID3D11RenderTargetView* rtv;
            HRESULT result = graphics.m_Device->CreateRenderTargetView(m_Texture, &renderTargetViewDesc, &rtv);
            SetDebugName(rtv, m_Name + " RTV " + std::to_string(i));
            m_TextureRTVs.push_back(rtv);
            if (FAILED(result))
            {
                return false;
            }
        }

    }
    return true;
}

bool Texture::CreateDSV(Graphics& graphics, DXGI_FORMAT texFormat)
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = texFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = 0;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    HRESULT result = graphics.m_Device->CreateDepthStencilView(m_Texture, &depthStencilViewDesc, &m_TextureDSV);
    SetDebugName(m_TextureDSV, m_Name + " DSV");
    return SUCCEEDED(result);
}

ID3D11RenderTargetView* Texture::GetRTV(UINT index /*= 0*/)
{
    return m_TextureRTVs[index];
}

void Texture::UseSRV(ID3D11DeviceContext* deviceContext, UINT startSlot)
{
    deviceContext->PSSetShaderResources(startSlot, 1, &m_TextureSRV);
}

Texture::Texture(Texture&& texture)
{
    m_Texture = texture.m_Texture;
    m_TextureSRV = texture.m_TextureSRV;
    m_TextureRTVs = texture.m_TextureRTVs;
    m_TextureDSV = texture.m_TextureDSV;
    m_Name = std::move(texture.m_Name);

    texture.m_Texture = NULL;
    texture.m_TextureSRV = NULL;
    texture.m_TextureRTVs.clear();
    texture.m_TextureDSV = NULL;
}


Texture::~Texture()
{
    //SAFE_RELEASE(m_Texture);
    SAFE_RELEASE(m_TextureSRV);
    for (ID3D11RenderTargetView* rtv : m_TextureRTVs)
    {
        //SAFE_RELEASE(rtv);
    }
    m_TextureRTVs.clear();
    SAFE_RELEASE(m_TextureDSV);
}
