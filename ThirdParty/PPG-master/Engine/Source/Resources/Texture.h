// --------------------------------------------------------------------------------------------
// Filename: Texture.h
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

class Graphics;

class Texture
{
public:
    Texture(ID3D11Texture2D* texture, const std::string& name);
    Texture(Texture&& texture);
    ~Texture();

    static Texture* CreateTexture(Graphics& graphics, int width, int height, const std::string& name, DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data = NULL);
    static Texture* CreateTextureDepthStencil(Graphics& graphics, int width, int height, const std::string& name, DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data = NULL);
    static Texture* CreateTextureCube(Graphics& graphics, int size, const std::string& name, DXGI_FORMAT texFormat, UINT bindFlags, UINT mipLevels = 1, D3D11_SUBRESOURCE_DATA* data = NULL);
    //Texture* LoadTextureFromMemory(Graphics& graphics, const uint8_t* data, size_t size, const std::wstring& ext);
    static Texture* LoadTextureFromPath(UINT this_level, UINT modeltype, Graphics& graphics, LPCWSTR& texturePath);
    bool CreateSRV(Graphics& graphics, DXGI_FORMAT texFormat, D3D11_SRV_DIMENSION viewDimension = D3D11_SRV_DIMENSION_TEXTURE2D, UINT mipLevels = 1);
    bool CreateRTV(Graphics& graphics, DXGI_FORMAT texFormat);
    bool CreateTextureCubeRTVs(Graphics& graphics, DXGI_FORMAT texFormat, UINT mipLevels = 1);
    bool CreateDSV(Graphics& graphics, DXGI_FORMAT texFormat);

    ID3D11RenderTargetView* GetRTV(UINT index = 0);
    void UseSRV(ID3D11DeviceContext* deviceContext, UINT startSlot);

    ID3D11Texture2D* m_Texture=NULL;
    ID3D11ShaderResourceView* m_TextureSRV = NULL;
    ID3D11DepthStencilView* m_TextureDSV = NULL;

    std::string m_Name;

    Texture() = default;
private:
    std::vector<ID3D11RenderTargetView*> m_TextureRTVs;
};
