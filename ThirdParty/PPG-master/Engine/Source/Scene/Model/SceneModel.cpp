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

#include "platform.h"
#include "woma_exception.h"
//#include <assimp/version.h>
//#if DX_ENGINE_LEVEL >= 85
//#include <assimp/revision.h>
//#endif

#define ASSIMP_LOAD_FLAGS   aiProcess_Triangulate | \
                            aiProcess_GenSmoothNormals | \
                            aiProcess_MakeLeftHanded     | \
                            aiProcess_FlipUVs            | \
                            aiProcess_FlipWindingOrder
//Ori:
/*
#define ASSIMP_LOAD_FLAGS   aiProcess_Triangulate | \
                            aiProcess_GenSmoothNormals | \
                            aiProcess_CalcTangentSpace | \
                            aiProcess_ConvertToLeftHanded
*/
/*
#define ASSIMP_LOAD_FLAGS   aiProcess_FlipUVs | \
                            aiProcess_LimitBoneWeights | \
                            aiProcess_Triangulate | \
                            aiProcess_GenSmoothNormals | \
                            aiProcess_CalcTangentSpace | \
                            aiProcess_ConvertToLeftHanded | \
                            aiProcess_Triangulate | \
                            aiProcess_GenSmoothNormals
*/

#ifdef DEBUG_MESH
void showNodeName(aiNode* node, UINT i = 0);
void showNodeName(aiNode* node, UINT i)
{
#ifdef DEBUG_MESH
    LOG_FILE << "[i: " << i << "] " << node->mName.data << endl;
#endif
    for (UINT i = 0; i < node->mNumChildren; i++) {
        showNodeName(node->mChildren[i], i);
    }
}
#endif

double TicksPerSecond;
SceneModel* SceneModel::LoadModelToScene(std::string fileName, Scene& scene, Graphics& graphics, SceneObject::Index parentIndex /*= 0*/)
{
    Assimp::Importer importer;
    const aiScene* pAssimpScene = importer.ReadFile(fileName, ASSIMP_LOAD_FLAGS);
    if (pAssimpScene == NULL)
        throw woma_exception("ModelLoader::Model file not found", __FILE__, __FUNCTION__, __LINE__);

    if (!pAssimpScene || pAssimpScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pAssimpScene->mRootNode || !pAssimpScene->HasMeshes())
    {
#ifdef DEBUG_MESH
        LOG_FILE << "error in assimp: " << importer.GetErrorString() << std::endl;
#endif
        throw woma_exception("ModelLoader::error in assimp", __FILE__, __FUNCTION__, __LINE__);
    }

    if (pAssimpScene->mAnimations)
    {
        if (pAssimpScene->mAnimations[0]->mTicksPerSecond != 0.0)
        {
            TicksPerSecond = pAssimpScene->mAnimations[0]->mTicksPerSecond;
        }
        else {
            TicksPerSecond = 30;
        }
#ifdef DEBUG_MESH
        LOG_FILE << "scene->HasAnimations: " << pAssimpScene->HasAnimations() << endl;
        LOG_FILE << "scene->mNumMeshes: " << pAssimpScene->mNumMeshes << endl;
        LOG_FILE << "scene->mAnimations[0]->mNumChannels: " << pAssimpScene->mAnimations[0]->mNumChannels << endl;
        LOG_FILE << "scene->mAnimations[0]->mDuration: " << pAssimpScene->mAnimations[0]->mDuration << endl;
        LOG_FILE << "scene->mAnimations[0]->mTicksPerSecond: " << pAssimpScene->mAnimations[0]->mTicksPerSecond << endl;
#endif
    }

#ifdef DEBUG_MESH
    LOG_FILE << "---		Node Names: " << endl;
    showNodeName(pAssimpScene->mRootNode);
    LOG_FILE << endl;
    // Process(get data of): Vertex, Indices and Textures
    LOG_FILE << "---		Bone Names : " << endl;
#endif

#ifdef DEBUG_MESH
    if (pAssimpScene->mAnimations)
    {
        LOG_FILE << "---		Animation Channels: " << endl;
        for (UINT i = 0; i < pAssimpScene->mAnimations[0]->mNumChannels; i++) {
            LOG_FILE << i << ":" << pAssimpScene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << endl;
            LOG_FILE << "mNumRotationKeys: " << pAssimpScene->mAnimations[0]->mChannels[i]->mNumRotationKeys << endl;
            for (size_t k = 0; k < pAssimpScene->mAnimations[0]->mChannels[i]->mNumRotationKeys; k++) {
                LOG_FILE << "\tChannel[" << i << "]: ";
                LOG_FILE << "RotationKeys[" << k << "]: ";
                LOG_FILE << pAssimpScene->mAnimations[0]->mChannels[i]->mRotationKeys[k].mValue.x << " ";
                LOG_FILE << pAssimpScene->mAnimations[0]->mChannels[i]->mRotationKeys[k].mValue.y << " ";
                LOG_FILE << pAssimpScene->mAnimations[0]->mChannels[i]->mRotationKeys[k].mValue.z << " ";
                LOG_FILE << endl;
            }
        }
        LOG_FILE << endl;
    }
#endif

    ModelLoader ml = ModelLoader(pAssimpScene, scene, graphics, fileName, parentIndex);
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

