// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: renderTextureClass.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------

#include "OSengine.h"
#if DX_ENGINE_LEVEL >= 36 ShadowMap
#include "main.h"
#include "DXrendertextureclass.h"
#if defined DX11
#include "dx11Class.h"
#endif
#if defined DX12
#include "dx12Class.h"
#endif
#include "log.h"
#include "Math3D.h"

DXrendertextureclass::DXrendertextureclass()
{
	CLASSLOADER();

	//public:
	m_shaderTextureResourceView	= NULL;
	m_renderTextureTargetView		= NULL;

	//private:
	m_renderTextureTarget	= NULL;
	m_depthStencilTextureBuffer	= NULL;
	m_depthStencilTextureView		= NULL;

	ZeroMemory(&m_TextureViewport, sizeof(m_TextureViewport));
	m_TextureProjectionMatrix = XMMatrixIdentity();
	m_TextureOrthoMatrix = XMMatrixIdentity();
}

DXrendertextureclass::~DXrendertextureclass() { Shutdown(); CLASSDELETE(); }

bool DXrendertextureclass::Initialize(void* Driver, int textureWidth, int textureHeight,
							int viewportX, int viewportY,
							float screenDepth, float screenNear)
{
	DirectX::DX11Class* m_driver11 = (DirectX::DX11Class*)Driver;

	HRESULT result;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_D24_UNORM_S8_UINT; //DXGI_FORMAT_R32G32B32A32_FLOAT;

	textureDesc.SampleDesc.Quality = m_driver11->MSAA_QUALITY;
	textureDesc.SampleDesc.Count = m_driver11->MSAA_COUNT;
	//textureDesc.SampleDesc.Quality = 1;
	//textureDesc.SampleDesc.Count = 0;

	textureDesc.Usage = D3D11_USAGE_DEFAULT; // Note: Can't Be D3D11_USAGE_IMMUTABLE
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//textureDesc.CPUAccessFlags = 0;
    //textureDesc.MiscFlags = 0;

	// CreateTexture2D: Create the render "target texture"
	// ------------------------------------------------
    result = m_driver11->m_device11->CreateTexture2D(&textureDesc, NULL, &m_renderTextureTarget);
	if (FAILED(result)) { WomaMessageBox(TEXT("CreateTexture2D"), TEXT("Error, Could not Create: ")); return false; }

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = (SystemHandle->AppSettings->MSAA_Anisotropic) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D; //MSAA
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// CreateRenderTargetView: Create the render target view.
	// ------------------------------------------------
    result = m_driver11->m_device11->CreateRenderTargetView(m_renderTextureTarget, &renderTargetViewDesc, &m_renderTextureTargetView);
	if (FAILED(result)) { WomaMessageBox(TEXT("CreateRenderTargetView"), TEXT("Error, Could not Create: ")); return false; }

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = (SystemHandle->AppSettings->MSAA_Anisotropic) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D; //MSAA
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// CreateShaderResourceView: Create the shader "resource view"
	// ------------------------------------------------
    result = m_driver11->m_device11->CreateShaderResourceView(m_renderTextureTarget, &shaderResourceViewDesc, &m_shaderTextureResourceView);
	if(FAILED(result)){ WomaMessageBox(TEXT("CreateShaderResourceView"), TEXT("Error, Could not Create: ")); return false; }

	// CreateTexture2D
	// ------------------------------------------------
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = m_driver11->MSAA_COUNT;
	depthBufferDesc.SampleDesc.Quality = m_driver11->MSAA_QUALITY;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_driver11->m_device11->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilTextureBuffer);
	if (FAILED(result)) { WomaMessageBox(TEXT("CreateTexture2D"), TEXT("Error, Could not Create: ")); return false; }

	// CreateDepthStencilView:
	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = (SystemHandle->AppSettings->MSAA_Anisotropic) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_driver11->m_device11->CreateDepthStencilView(m_depthStencilTextureBuffer, &depthStencilViewDesc, &m_depthStencilTextureView);
	if (FAILED(result)) { WomaMessageBox(TEXT("CreateDepthStencilView"), TEXT("Error, Could not Create: ")); return false; }

	// Setup the viewport for rendering.
    m_TextureViewport.Width = (float)textureWidth;
    m_TextureViewport.Height = (float)textureHeight;
    m_TextureViewport.MinDepth = 0.0f;
    m_TextureViewport.MaxDepth = 1.0f;
    m_TextureViewport.TopLeftX = 0.0f; //viewportX;
    m_TextureViewport.TopLeftY = 0.0f; //viewportY;

	// Create the projection matrix for 3D rendering:
	float fieldOfView, screenAspect;

	ASSERT (screenDepth > 0);
	fieldOfView = (float)PI / 4.0f; // Or... fieldOfView = (90 / 2) * 0,0174532925f; (which is equal) for 45 degrees...
	screenAspect =((float)textureWidth / (float)textureHeight);
	m_TextureProjectionMatrix = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, screenNear, screenDepth);    // 3D PROJECTION
	m_TextureOrthoMatrix = XMMatrixOrthographicLH ((float) textureWidth, (float) textureHeight, screenNear, screenDepth);  // 2D PROJECTION

	return true;
}

void DXrendertextureclass::Shutdown()
{
	SAFE_RELEASE(m_depthStencilTextureView);
	SAFE_RELEASE(m_depthStencilTextureBuffer);

	SAFE_RELEASE(m_shaderTextureResourceView);
	SAFE_RELEASE(m_renderTextureTargetView);
	SAFE_RELEASE(m_renderTextureTarget);
}

void DXrendertextureclass::SetRenderTarget(void* Driver)
{
	DirectX::DX11Class* m_driver11 = (DirectX::DX11Class*)Driver;
	ID3D11DeviceContext* pContext = m_driver11->m_deviceContext;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	pContext->OMSetRenderTargets(1, &m_renderTextureTargetView, m_depthStencilTextureView);
	
	// Set the viewport.
	pContext->RSSetViewports(1, &m_TextureViewport);
}

void DXrendertextureclass::ClearRenderTarget(void* Driver, float red, float green, float blue, float alpha)
{	
	DirectX::DX11Class* m_driver11 = (DirectX::DX11Class*)Driver;
	ID3D11DeviceContext* pContext = m_driver11->m_deviceContext;

	// Setup the color to clear the buffer to.
	float color[4] = {red, green, blue, alpha};

	// Clear the back buffer.
	pContext->ClearRenderTargetView(m_renderTextureTargetView, color);
    
	// Clear the depth buffer.
	pContext->ClearDepthStencilView(m_depthStencilTextureView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

#endif
