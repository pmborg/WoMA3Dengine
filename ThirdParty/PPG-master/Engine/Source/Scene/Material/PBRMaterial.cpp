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

PBRMaterial::PBRMaterial(PBRMaterial&& PhongMaterial)
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
    m_MaterialInfo.m_UseAlbedo = 1;
    return *this;
}

PBRMaterial& PBRMaterial::UseNormalMap(Texture* normal)
{
    m_Normal = normal;
    m_MaterialInfo.m_NormalState = 1;
    return *this;
}

PBRMaterial& PBRMaterial::UseBumpMap(Texture* bump)
{
    m_Normal = bump;
    m_MaterialInfo.m_NormalState = 2;
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
    m_MaterialInfo.m_UseEmissive = 1;
    return *this;
}

PBRMaterial& PBRMaterial::ConvertToLinear(bool convertToLinear)
{
    m_MaterialInfo.m_ConvertToLinear = convertToLinear;
    return *this;
}
