// --------------------------------------------------------------------------------------------
// Filename: Sampler.cpp
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
// WomaIntegrityCheck = 1234525217;

#include "stdafx.h"
#include "Sampler.h"
#include "LowLevel/Graphics.h"

Sampler::Sampler(Graphics& graphics, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode)
{
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = filter;
    samplerDesc.AddressU = addressMode;
    samplerDesc.AddressV = addressMode;
    samplerDesc.AddressW = addressMode;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.MinLOD = -FLT_MAX;
    samplerDesc.MaxLOD = FLT_MAX;

    graphics.m_Device->CreateSamplerState(&samplerDesc, &m_Sampler);
    SetDebugName(m_Sampler, "Sampler");
}

void Sampler::Use(ID3D11DeviceContext* deviceContext, UINT startSlot)
{
    deviceContext->PSSetSamplers(startSlot, 1, &m_Sampler);
}

Sampler::~Sampler()
{
    SAFE_RELEASE(m_Sampler);
}
