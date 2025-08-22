// --------------------------------------------------------------------------------------------
// Filename: Graphics.h
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
#include <atlbase.h>
#include <combaseapi.h>
using namespace DirectX;

class Window;
class Texture;

class Graphics
{
public:
    Graphics(HINSTANCE hInstance, BOOL vSync, Window& window);
    ~Graphics();
    
    void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
    void ClearRenderTargetView(ID3D11RenderTargetView* rtv, const FLOAT clearColor[4]);
    void ClearDepthStencil(FLOAT clearDepth, UINT8 clearStencil);
    ID3D11Buffer* CreateBuffer(UINT byteWidth, UINT bindFlags, const void* data);
	void UpdateBuffer(ID3D11DeviceContext* pContext, ID3D11Buffer* buffer, const void* resource);
    void UnbindShaderResourceView(UINT startSlot);

    ID3D11Device* m_Device = nullptr;
    ID3D11DeviceContext* m_DeviceContext = nullptr;

    std::unique_ptr<Texture> m_BackBuffer;
    std::unique_ptr<Texture> m_DepthStencilBuffer;

    ID3D11DepthStencilState* m_DepthStencilState = nullptr;
    ID3D11RasterizerState* m_RasterizerState = nullptr;
    ID3D11BlendState* m_AlphaBlendState = nullptr;
    D3D11_VIEWPORT m_Viewport = { 0 };

    RECT m_ClientRect;

private:
};
