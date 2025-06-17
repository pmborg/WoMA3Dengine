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

#include <assimp\anim.h>

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
    static SceneModel * LoadModelToScene(UINT type, std::string meshFileName, std::string animFileName, Scene& Scene, Graphics& graphics, SceneObject::Index parentIndex = 0);

    std::vector<const aiAnimation*> m_Animations; // Store pointers to aiAnimation ANIM!

private:
    friend class ModelLoader;
    std::vector<Mesh*> m_Meshes;
    std::vector<PBRMaterial*> m_Materials;
    std::vector<Texture*> m_Textures;
};

#ifndef GENERATE_PACK
#define Forest_Huntress_idle_fbx_Model_LOD0_fbx_size 3438412    //Forest_Huntress_idle_fbx_Model_LOD0_fbx
#define Forest_Huntress_idle2_fbx_Model_LOD0_fbx_size 3393868   //Forest_Huntress_idle2_fbx_Model_LOD0_fbx
#define Forest_Huntress_idle3_fbx_Model_LOD0_fbx_size 3372588   //Forest_Huntress_idle3_fbx_Model_LOD0_fbx

#define Skin_1_Armor_and_Weapon_Albedo_png_size 42376836
#define Skin_1_Body_Albedo_png_size 23213119
#define Skin_1_Hair_Albedo_png_size 9668891
#define Skin_1_Head_Albedo_png_size 12728492
#endif

//DX_ENGINE_LEVEL >= 86:
extern unsigned char* Forest_Huntress_idle_fbx_Model_LOD0_fbxBuffer;
//DX_ENGINE_LEVEL >= 87:
extern unsigned char* Forest_Huntress_idle2_fbx_Model_LOD0_fbxBuffer;
extern unsigned char* Forest_Huntress_idle3_fbx_Model_LOD0_fbxBuffer;

