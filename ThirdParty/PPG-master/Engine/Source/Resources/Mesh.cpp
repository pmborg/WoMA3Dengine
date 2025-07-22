// --------------------------------------------------------------------------------------------
// Filename: Mesh.cpp
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
#include "LowLevel/Graphics.h"
#include "Mesh.h"
#include "Texture.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<WORD> indices, Graphics& graphics, bool calcTangents /*= true*/) :
    m_Vertices(std::move(vertices)),
    m_Indices(std::move(indices))
{
    // calculate tangent
    if (calcTangents)
    {
        for (int i = 0; i < m_Indices.size()-3; i += 3)
        {
            WORD index0 = m_Indices[i];
            WORD index1 = m_Indices[i + 1];
            WORD index2 = m_Indices[i + 2];

            Vertex& v0 = m_Vertices[index0];
            Vertex& v1 = m_Vertices[index1];
            Vertex& v2 = m_Vertices[index2];

            XMFLOAT3 edge0 = XMFLOAT3Subtract(v1.Position, v0.Position);
            XMFLOAT3 edge1 = XMFLOAT3Subtract(v2.Position, v0.Position);
            XMFLOAT2 dUV0 = XMFLOAT2Subtract(v1.TexCoord, v0.TexCoord);
            XMFLOAT2 dUV1 = XMFLOAT2Subtract(v2.TexCoord, v0.TexCoord);

            float f = 1.0f / (dUV0.x * dUV1.y - dUV1.x * dUV0.y);
            XMFLOAT3 tangent =
            {
                f * (dUV1.y * edge0.x - dUV0.y * edge1.x),
                f * (dUV1.y * edge0.y - dUV0.y * edge1.y),
                f * (dUV1.y * edge0.z - dUV0.y * edge1.z),
            };
            XMFLOAT3 binormal =
            {
                f * (dUV1.x * edge0.x - dUV0.x * edge1.x),
                f * (dUV1.x * edge0.y - dUV0.x * edge1.y),
                f * (dUV1.x * edge0.z - dUV0.x * edge1.z),
            };

            v0.Tangent = tangent; // currently we don't average tangent results, last write wins
            v1.Tangent = tangent;
            v2.Tangent = tangent;

            v0.Binormal = binormal;
            v1.Binormal = binormal;
            v2.Binormal = binormal;
        }
    }

    m_VertexBuffer = graphics.CreateBuffer(sizeof(Vertex) * m_Vertices.size(), D3D11_BIND_VERTEX_BUFFER, m_Vertices.data());
    m_IndexBuffer = graphics.CreateBuffer(sizeof(WORD) * m_Indices.size(), D3D11_BIND_INDEX_BUFFER, m_Indices.data());
}

Mesh::Mesh(Mesh&& mesh) :
    m_Vertices(std::move(mesh.m_Vertices)),
    m_Indices(std::move(mesh.m_Indices))
{
    mesh.m_Vertices.clear();
    mesh.m_Indices.clear();
    m_IndexBuffer = mesh.m_IndexBuffer;
    m_VertexBuffer = mesh.m_VertexBuffer;
    mesh.m_VertexBuffer = NULL;
    mesh.m_IndexBuffer = NULL;
}

Mesh::~Mesh()
{
    SAFE_RELEASE(m_VertexBuffer);
    SAFE_RELEASE(m_IndexBuffer);
}

void Mesh::Draw(ID3D11DeviceContext* deviceContext)
{
    const UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_VertexStride, &offset);
    deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->DrawIndexed(m_Indices.size(), 0, 0); //12132
}
