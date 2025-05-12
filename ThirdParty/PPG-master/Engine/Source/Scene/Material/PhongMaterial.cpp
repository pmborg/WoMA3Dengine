#include "stdafx.h"
#include "PhongMaterial.h"
#include "Resources/Texture.h"


PhongMaterial::PhongMaterial(PhongMaterial&& PhongMaterial)
{
    m_Diffuse = PhongMaterial.m_Diffuse;
    m_Normal = PhongMaterial.m_Normal;
    m_Specular = PhongMaterial.m_Specular;

    m_MaterialInfo = PhongMaterial.m_MaterialInfo;
}

PhongMaterial& PhongMaterial::SetDiffuse(float r, float g, float b)
{
    m_MaterialInfo.m_Diffuse.x = r;
    m_MaterialInfo.m_Diffuse.y = g;
    m_MaterialInfo.m_Diffuse.z = b;
    return *this;
}

PhongMaterial& PhongMaterial::SetSpecular(float r, float g, float b)
{
    m_MaterialInfo.m_Specular.x = r;
    m_MaterialInfo.m_Specular.y = g;
    m_MaterialInfo.m_Specular.z = b;
    return *this;
}

PhongMaterial& PhongMaterial::SetShininess(float n)
{
    m_MaterialInfo.m_Shininess = n;
    return *this;
}

PhongMaterial& PhongMaterial::UseDiffuseMap(Texture* diffuse)
{
    m_Diffuse = diffuse;
    m_MaterialInfo.m_UseDiffuse = 1;
    return *this;
}

PhongMaterial& PhongMaterial::UseNormalMap(Texture* normal)
{
    m_Normal = normal;
    m_MaterialInfo.m_NormalState = 1;
    return *this;
}

PhongMaterial& PhongMaterial::UseBumpMap(Texture* bump)
{
    m_Normal = bump;
    m_MaterialInfo.m_NormalState = 2;
    return *this;
}

PhongMaterial& PhongMaterial::UseSpecularMap(Texture* specular)
{
    m_Specular = specular;
    m_MaterialInfo.m_UseSpecular = 1;
    return *this;
}
