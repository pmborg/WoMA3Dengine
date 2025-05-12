// --------------------------------------------------------------------------------------------
// Filename: PBRMaterial.h
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

#pragma once
#include "stdafx.h"
#include "Resources/Texture.h"

using namespace DirectX;

_declspec(align(16)) struct PBRMaterialInfo
{
    XMFLOAT4 m_Albedo = XMFLOAT4(0, 0, 0, 0);
    float m_Metallic = 0.0f;
    float m_Roughness = 1.0f;
    int m_UseAlbedo = 0;
    int m_UseOccRoughMetal = 0;
    int m_UseAOMap = 0;
    int m_UseEmissive = 0;
    int m_NormalState = 0;
    int m_ConvertToLinear = 0;
};

class PBRMaterial
{
public:
    PBRMaterial() = default;
    PBRMaterial(PBRMaterial& material) = delete;
    PBRMaterial(PBRMaterial&& material);

    PBRMaterial& SetAlbedo(float r, float g, float b);
    PBRMaterial& SetMetallic(float n);
    PBRMaterial& SetRoughness(float n);
    PBRMaterial& UseAlbedoMap(Texture* albedo);
    PBRMaterial& UseNormalMap(Texture* normal);
    PBRMaterial& UseBumpMap(Texture* bump);
    PBRMaterial& UseOccRoughMetal(Texture* occMetalRough);
    PBRMaterial& UseAoMap(Texture* aoMap);
    PBRMaterial& UseEmissiveMap(Texture* emissive);
    PBRMaterial& ConvertToLinear(bool convertToLinear);

    Texture* m_Albedo = nullptr;
    Texture* m_Normal = nullptr;
    Texture* m_OccRoughMetal = nullptr;
    Texture* m_AoMap = nullptr;
    Texture* m_Emissive = nullptr;

    PBRMaterialInfo m_MaterialInfo;
};

