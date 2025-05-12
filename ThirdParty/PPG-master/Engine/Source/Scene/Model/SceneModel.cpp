// --------------------------------------------------------------------------------------------
// Filename: SceneModel.cpp
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

#include "stdafx.h"
#include <map>
#include "ModelLoader.h"
#include "SceneModel.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "LowLevel/Graphics.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Material/Material.h"
#include "Scene/Scene.h"
#include "Skeleton.h"
#include <assimp/version.h>
//#include <assimp/revision.h>

#define ASSIMP_LOAD_FLAGS aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded
SceneModel* SceneModel::LoadModelToScene(std::string fileName, Scene& scene, Graphics& graphics, SceneObject::Index parentIndex /*= 0*/)
{
    UINT versionMajor = aiGetVersionMajor();
    UINT versionMinor = aiGetVersionMinor();
    // UINT revision = aiGetVersionRevision();

#ifdef DEBUG_MESH
    LOG_FILE << "versionMajor : " << versionMajor << endl;
    LOG_FILE << "versionMinor : " << versionMinor << endl;
    //LOG_FILE << "revision : " << revision << endl;
#endif

    Assimp::Importer importer;
    const aiScene* assimpScene = importer.ReadFile(fileName, ASSIMP_LOAD_FLAGS);
    if (assimpScene == NULL)
        throw std::exception("ModelLoader::Model file not found");

    ModelLoader ml = ModelLoader(assimpScene, scene, graphics, fileName, parentIndex);
	return ml.LoadModel();
}

SceneModel::SceneModel(std::shared_ptr<SceneObject>& sceneObject) :
    m_SceneObject(sceneObject)
{
}

SceneModel::~SceneModel()
{
    for (Mesh* mesh : m_Meshes)
        delete mesh;

    for (PBRMaterial* mat : m_Materials)
        delete mat;

    for (Texture* tex : m_Textures)
        delete tex;

    if (m_Skeleton != nullptr)
        delete m_Skeleton;
}

