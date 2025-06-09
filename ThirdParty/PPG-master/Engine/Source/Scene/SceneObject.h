// --------------------------------------------------------------------------------------------
// Filename: SceneObject.h
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
#include "Transform.h"
#include "Scene/Component/Rendering.h"

class Mesh;
class Texture;
class PhongMaterial;

class SceneObject
{
public:
    using Index = int;
    SceneObject(const std::string& name, SceneObject::Index index, SceneObject::Index parentIndex);
    SceneObject(SceneObject& copy) = delete;
    SceneObject(SceneObject&& obj) = default;
    SceneObject& operator= (const SceneObject&) = delete;

    std::string m_Name;
    Transform m_Transform;
    Index m_Index;
    const Index  m_ParentIndex;
    std::vector<Index> m_ChildrenIndices;

    MeshRenderer m_MeshRenderer;
    SpriteRenderer m_SpriteRenderer;
    Animator m_Animator;

private:
    friend class Scene;
};

