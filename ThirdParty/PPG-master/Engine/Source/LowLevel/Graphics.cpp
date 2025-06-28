// --------------------------------------------------------------------------------------------
// Filename: Graphics.cpp
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

#include "stdafx.h"
#include "OSengine.h"
#include "DX11Class.h"
#include "Window.h"
#include "LowLevel/Graphics.h"
#include "Resources/Texture.h"

using namespace DirectX;

ID3D11ShaderResourceView* nullSRV[] = { nullptr };
ID3D11RenderTargetView* nullRTV[] = { nullptr };

Graphics::Graphics(HINSTANCE hInstance, BOOL vSync, Window& window)
{
    HRESULT hr;

    m_ClientRect = window.GetWindowRect();

    // Compute the exact client dimensions. This will be used
    // to initialize the render targets for our swap chain.
    unsigned int clientWidth = m_ClientRect.right - m_ClientRect.left;
    unsigned int clientHeight = m_ClientRect.bottom - m_ClientRect.top;

    #define m_driver11 ((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])
    m_Device = ((DirectX::DX11Class*)m_driver11)->m_device11;
    m_DeviceContext = ((DirectX::DX11Class*)m_driver11)->m_deviceContext;


    // Next initialize the back buffer of the swap chain and associate it to a
    // render target view.
    ID3D11Texture2D* backBuffer = ((DirectX::DX11Class*)m_driver11)->DX11windowsArray[0].m_backBuffer;

    m_BackBuffer = std::make_unique<Texture>(backBuffer, "Back Buffer");
    m_BackBuffer->CreateRTV(*this, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
    Texture* depthStencil = Texture::CreateTextureDepthStencil(*this, clientWidth, clientHeight, "Depth Stencil Buffer",
        DXGI_FORMAT_R24G8_TYPELESS, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
    depthStencil->CreateDSV(*this, DXGI_FORMAT_D24_UNORM_S8_UINT);
    depthStencil->CreateSRV(*this, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
    m_DepthStencilBuffer = std::unique_ptr<Texture>(depthStencil);



    m_DepthStencilState = ((DirectX::DX11Class*)m_driver11)->m_depthStencilState;

    

    m_RasterizerState = ((DirectX::DX11Class*)m_driver11)->m_rasterState[CULL_NONE][FILL_SOLID];

    

    m_Viewport = ((DirectX::DX11Class*)m_driver11)->DX11windowsArray[0].viewport;

    



    D3D11_BLEND_DESC BlendState;
    ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
    BlendState.RenderTarget[0].BlendEnable = TRUE;
    BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;
    m_Device->CreateBlendState(&BlendState, &m_AlphaBlendState);
}

ID3D11Buffer* Graphics::CreateBuffer(UINT byteWidth, UINT bindFlags, const void* data)
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.BindFlags = bindFlags;
    bufferDesc.ByteWidth = byteWidth;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ID3D11Buffer* constantBuffer;
    HRESULT hr;
    if (data == nullptr)
    {
        hr = m_Device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
    }
    else
    {
        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
        resourceData.pSysMem = data;
        hr = m_Device->CreateBuffer(&bufferDesc, &resourceData, &constantBuffer);
    }

    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create Buffer");
    }
    return constantBuffer;
}

void Graphics::UpdateBuffer(ID3D11Buffer* buffer, const void* resource)
{
    m_DeviceContext->UpdateSubresource(buffer, 0, nullptr, resource, 0, 0);
}
void Graphics::UnbindShaderResourceView(UINT startSlot)
{
    m_DeviceContext->PSSetShaderResources(startSlot, 1, nullSRV);
}


void Graphics::Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil)
{
    ClearRenderTargetView(m_BackBuffer->GetRTV(), clearColor);
    ClearDepthStencil(clearDepth, clearStencil);
}

void Graphics::ClearRenderTargetView(ID3D11RenderTargetView* rtv, const FLOAT clearColor[4])
{
    m_DeviceContext->ClearRenderTargetView(rtv, clearColor);
}

void Graphics::ClearDepthStencil(FLOAT clearDepth, UINT8 clearStencil)
{
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilBuffer->m_TextureDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

Graphics::~Graphics()
{
    SAFE_RELEASE(m_AlphaBlendState);
}

