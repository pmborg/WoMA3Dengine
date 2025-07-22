// --------------------------------------------------------------------------------------------
// Filename: Shader.cpp
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
#include "Shader.h"
#include "LowLevel/Graphics.h"

//C:\WoMAengine2023\PPG-master\Bin\Debug\VertexShader.cso
//C:\WoMAengine2023\PPG-master\Bin\Debug\GBuffer.ps.cso
Shader::Shader(LPCWSTR vertexPath, LPCWSTR pixelPath, Graphics& graphics)
{
    ID3DBlob* vertexShaderBlob;
    HRESULT hr = D3DReadFileToBlob(vertexPath, &vertexShaderBlob);

    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create vertex shader blob");
    }

    hr = graphics.m_Device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &m_VertexShader);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create vertex shader");
    }

    // Currently only support this type
    D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = graphics.m_Device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_InputLayout);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create input layout");
    }
    SAFE_RELEASE(vertexShaderBlob);

    ID3DBlob* pixelShaderBlob;
    LPCWSTR compiledPixelShaderObject = pixelPath;

    hr = D3DReadFileToBlob(compiledPixelShaderObject, &pixelShaderBlob);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create pixel shader blob");
    }

    hr = graphics.m_Device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &m_PixelShader);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create pixel shader");
    }

    SAFE_RELEASE(pixelShaderBlob);
}

Shader::~Shader()
{
    SAFE_RELEASE(m_InputLayout);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_PixelShader);
}

void Shader::Use(ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetInputLayout(m_InputLayout);
    deviceContext->VSSetShader(m_VertexShader, nullptr, 0);
    deviceContext->PSSetShader(m_PixelShader, nullptr, 0);
}
