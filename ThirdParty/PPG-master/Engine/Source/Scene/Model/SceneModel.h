// --------------------------------------------------------------------------------------------
// Filename: SceneModel.h
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

#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Scene.h"
#include "Skeleton.h"

class Graphics;
class SceneObject;
class ModelLoader;

class SceneModel
{
public:
    std::shared_ptr<SceneObject> m_SceneObject;
    SceneModel(std::shared_ptr<SceneObject>& sceneObject);
    ~SceneModel();
    Skeleton* m_Skeleton;
    static SceneModel* LoadModelToScene(std::string fileName, Scene& Scene, Graphics& graphics, SceneObject::Index parentIndex = 0);
private:
    friend class ModelLoader;
    std::vector<Mesh*> m_Meshes;
    std::vector<PBRMaterial*> m_Materials;
    std::vector<Texture*> m_Textures;
};

