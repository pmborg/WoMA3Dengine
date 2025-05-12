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
