// --------------------------------------------------------------------------------------------
// Filename: PBRMaterial.cpp
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
#include "PBRMaterial.h"
#include "Resources/Texture.h"

PBRMaterial::PBRMaterial(PBRMaterial&& PhongMaterial)// noexcept
{
    m_Albedo = PhongMaterial.m_Albedo;
    m_Normal = PhongMaterial.m_Normal;

    m_MaterialInfo = PhongMaterial.m_MaterialInfo;
}

PBRMaterial& PBRMaterial::SetAlbedo(float r, float g, float b)
{
    m_MaterialInfo.m_Albedo.x = r;
    m_MaterialInfo.m_Albedo.y = g;
    m_MaterialInfo.m_Albedo.z = b;
    return *this;
}

PBRMaterial& PBRMaterial::SetMetallic(float n)
{
    m_MaterialInfo.m_Metallic = n;
    return *this;
}

PBRMaterial& PBRMaterial::SetRoughness(float n)
{
    m_MaterialInfo.m_Roughness = n;
    return *this;
}

PBRMaterial& PBRMaterial::UseAlbedoMap(Texture* albedo)
{
    m_Albedo = albedo;
    m_MaterialInfo.m_UseAlbedoMap = 1;
    return *this;
}

PBRMaterial& PBRMaterial::UseNormalMap(Texture* normal)
{
    m_Normal = normal;
    m_MaterialInfo.m_NormalState += 1;
    return *this;
}

PBRMaterial& PBRMaterial::UseBumpMap(Texture* bump)
{
    m_Bump = bump;
    m_MaterialInfo.m_NormalState += 2;
    return *this;
}

PBRMaterial& PBRMaterial::UseOccRoughMetal(Texture* occMetalRough)
{
    m_OccRoughMetal = occMetalRough;
    m_MaterialInfo.m_UseOccRoughMetal = 1;
    return *this;
}

PBRMaterial& PBRMaterial::UseAoMap(Texture* aoMap)
{
    m_AoMap = aoMap;
    m_MaterialInfo.m_UseAOMap = 1;
    return *this;
}

PBRMaterial& PBRMaterial::UseEmissiveMap(Texture* emissive)
{
    m_Emissive = emissive;
    m_MaterialInfo.m_UseEmissiveMap = 1;
    return *this;
}

PBRMaterial& PBRMaterial::ConvertToLinear(bool convertToLinear)
{
    m_MaterialInfo.m_ConvertToLinear = convertToLinear;
    return *this;
}

// Set the emissive color (RGB)
PBRMaterial& PBRMaterial::SetEmissive(float r, float g, float b)
{
    m_MaterialInfo.m_UseEmissiveColor = 1;
    // Store emissive color in the alpha channel of m_Albedo or add a new member if needed
    m_EmissiveColor = XMFLOAT3(r, g, b);
    return *this;
}


// Add this method:
PBRMaterial& PBRMaterial::UseAlphaMap(Texture* alpha)
{
    m_AlphaMap = alpha;
    m_MaterialInfo.m_UseAlphaMap = (alpha != nullptr) ? 1 : 0;
    return *this;
}

// Add this method:
PBRMaterial& PBRMaterial::UseMetallicMap(Texture* metallic)
{
    m_Metallic = metallic;
    m_MaterialInfo.m_UseMetallicMap = (metallic != nullptr) ? 1 : 0;
    return *this;
}

// Add this method:
PBRMaterial& PBRMaterial::UseRoughnessMap(Texture* roughness)
{
    m_Roughness = roughness;
    m_MaterialInfo.m_UseRoughnessMap = (roughness != nullptr) ? 1 : 0;
    return *this;
}