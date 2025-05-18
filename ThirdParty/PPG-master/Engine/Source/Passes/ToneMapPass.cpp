// --------------------------------------------------------------------------------------------
// Filename: ToneMapPass.cpp
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
#include "ToneMapPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ToneMapPass::ToneMapPass(Graphics& graphics, Texture& colour, Texture& dest) :
    m_ColourTexture(colour),
    m_DestTexture(dest)
{
    m_Shader = std::make_unique<Shader>(L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\Fullscreen.vs.cso", 
                                        L"C:\\WoMA3Dengine\\ThirdParty\\PPG-master\\Bin\\Debug\\ToneMapReinhard.ps.cso", graphics);
}

ToneMapPass::~ToneMapPass()
{
}

void ToneMapPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    //  graphics.SetRenderTarget(m_DestTexture, false);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_ColourTexture.UseSRV(deviceContext, 0);
    m_Shader->Use(deviceContext);
    deviceContext->Draw(4, 0);

    graphics.UnbindShaderResourceView(0);
    //  graphics.UnbindRenderTargetView();
}
