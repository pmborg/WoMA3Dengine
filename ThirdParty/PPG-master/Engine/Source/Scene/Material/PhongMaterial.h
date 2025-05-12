#pragma once
#include "stdafx.h"
#include "Resources/Texture.h"

using namespace DirectX;

_declspec(align(16)) struct PhongMaterialInfo
{
    XMFLOAT4 m_Diffuse = XMFLOAT4(0, 0, 0, 0);
    XMFLOAT4 m_Specular = XMFLOAT4(0, 0, 0, 0);

    float m_Shininess = 0;
    int m_UseDiffuse = 0;
    int m_NormalState = 0;
    int m_UseSpecular = 0;
};

class PhongMaterial
{
public:
    PhongMaterial() = default;
    PhongMaterial(PhongMaterial& material) = delete;
    PhongMaterial(PhongMaterial&& material);

    PhongMaterial& SetDiffuse(float r, float g, float b);
    PhongMaterial& SetSpecular(float r, float g, float b);
    PhongMaterial& SetShininess(float n);
    PhongMaterial& UseDiffuseMap(Texture* diffuse);
    PhongMaterial& UseNormalMap(Texture* normal);
    PhongMaterial& UseBumpMap(Texture* bump);
    PhongMaterial& UseSpecularMap(Texture* specular);

    Texture* m_Diffuse = nullptr;
    Texture* m_Normal = nullptr;
    Texture* m_Specular = nullptr;

    PhongMaterialInfo m_MaterialInfo;
};

