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
    // 0: Albedo/BaseColor
    XMFLOAT4 m_Albedo = XMFLOAT4(1, 1, 1, 0);
    int   m_UseAlbedoMap = 1;

    // 1: Normal
    int   m_NormalState = 0; //1 = NormalMap | 2 = BumpMap | 3 = both

    // 2: Bump
    // m_NormalState  = 2

    // 3: Ambient Occlusion
    int   m_UseAOMap = 0;

    // 4: Emissive
    int   m_UseEmissiveMap = 0;
    // -------------------------------------
    XMFLOAT4 ambientColor = { 0.55f, 0.55f, 0.55f, 0 };
    XMFLOAT4 lightColor = { 1, 1, 1 , 1};
    
    XMFLOAT4 m_LightDirection = XMFLOAT4(0, 0, 0, 0);
    XMFLOAT3 m_LightPos = XMFLOAT3(0, 0, 0);

    // 5: Opacity/Alpha
    int   m_UseAlphaMap = 0;

    // -------------------------------------
    //6: Metallic
    float m_Metallic = 0.0f;
    int   m_UseMetallicMap = 0;

    //7: Roughness
    float m_Roughness = 1.0f;
    int   m_UseRoughnessMap = 0;

    // -------------------------------------
    // 8: Emissive color
    int   m_UseEmissiveColor = 0;

    int   m_UseGLTF = 0;            //Future: Type::GLTF
    int   m_UseOccRoughMetal = 0;   //Future: Type::GLTF
    int   m_ConvertToLinear = 0;    //Future: Type::GLTF
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
														  
    PBRMaterial& UseAoMap(Texture* aoMap);
    PBRMaterial& UseEmissiveMap(Texture* emissive);
    PBRMaterial& ConvertToLinear(bool convertToLinear);
    PBRMaterial& SetEmissive(float r, float g, float b);
    PBRMaterial& UseAlphaMap(Texture* alpha);
    PBRMaterial& UseMetallicMap(Texture* alpha);
    PBRMaterial& UseRoughnessMap(Texture* alpha);

    PBRMaterial& UseOccRoughMetal(Texture* occMetalRough);

    Texture* m_Albedo = nullptr;            //0: albedo
    Texture* m_Normal = nullptr;            //1: normal
    Texture* m_OccRoughMetal = nullptr;     //2: future: GLTF Format
    Texture* m_AoMap = nullptr;             //3: ao
    Texture* m_Emissive = nullptr;          //4: emissive
    Texture* m_Metallic = nullptr;          //5: Metallic
    Texture* m_Roughness = nullptr;         //6: Roughness
    Texture* m_AlphaMap = nullptr;          //7: opacity/AlphaMap
    Texture* m_Bump = nullptr;              //8: bump
    
    // ... existing members ...
    XMFLOAT3 m_EmissiveColor = XMFLOAT3(0, 0, 0);
								  
    PBRMaterialInfo m_MaterialInfo;
};
