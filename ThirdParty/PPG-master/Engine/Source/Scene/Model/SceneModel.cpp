// --------------------------------------------------------------------------------------------
// Filename: SceneModel.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG
// WomaIntegrityCheck = 1234525217;

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
#include "OSengine.h"
#include "woma_exception.h"
#include <assimp/version.h>
#include <assimp/revision.h>

//FBX: SceneModel.h
unsigned char* Forest_Huntress_idle_fbx_Model_LOD0_fbxBuffer = NULL;            //==86 & 87
unsigned char* Forest_Huntress_idle2_fbx_Model_LOD0_fbxBuffer = NULL;           //==87 & 88
unsigned char* Forest_Huntress_idle3_fbx_Model_LOD0_fbxBuffer = NULL;           //==87

//unsigned char* Forest_Huntress_idle2_fbx_Model_LOD0_fbxBuffer = NULL;         // >= 88
unsigned char* Forest_Huntress_Walk_fbx_Model_LOD0_fbxBuffer = NULL;            // >= 88
unsigned char* Forest_Huntress_Walk_back_fbx_Model_LOD0_fbxBuffer = NULL;       // >= 88
unsigned char* Forest_Huntress_Run2_fbx_Model_LOD0_fbxBuffer = NULL;            // >= 88

unsigned char* Forest_Huntress_Strafe_left_fbx_Model_LOD0_fbxBuffer = NULL;     // >= 88
unsigned char* Forest_Huntress_Strafe_right_fbx_Model_LOD0_fbxBuffer = NULL;    // >= 88
unsigned char* Forest_Huntress_Draw_sword_fbx_Model_LOD0_fbxBuffer = NULL;      // >= 88

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

double TicksPerSecond=0;

SceneModel* SceneModel::LoadModelToScene(UINT dxlevel, bool enginefile, UINT type, std::string meshFileName, std::string animFileName, Scene& scene, Graphics& graphics, SceneObject::Index parentIndex /*= 0*/)
{
#ifdef DEBUG_MESH
    LOG_FILE << "WOMA (" << LEVEL << ") LOAD FILE : " << (char*)meshFileName.c_str() << endl;
    LOG_FILE << "LOADING... C:/WoMA3Dengine/ThirdParty/external/assimp" << endl;

    UINT versionMajor = aiGetVersionMajor();
    UINT versionMinor = aiGetVersionMinor();
    UINT revision = aiGetVersionRevision();

    LOG_FILE << "versionMajor : " << versionMajor << endl;
    LOG_FILE << "versionMinor : " << versionMinor << endl;
    LOG_FILE << "revision : " << revision << endl;
#endif

	if (WOMA::main_loop_state < 0)
		return NULL;

    unsigned int DX_ASSIMP_LOAD_FLAGS = 0;
    const TCHAR* extension = _tcsrchr(meshFileName.c_str(), '.');
    if (type >= 1 || _tcsicmp(extension, TEXT(".dae")) == 0 || _tcsicmp(extension, TEXT(".DAE")) == 0)
    {
        //MD5MESH (converted from DAE) or FBX:
        if (type >= 1) {
            DX_ASSIMP_LOAD_FLAGS = aiProcess_LimitBoneWeights | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_MakeLeftHanded| aiProcess_FlipUVs;
        }
        else//DAE:
            if (_tcsicmp(meshFileName.c_str(), TEXT("../engine\\data\\scene85\\rp_nathan_animated_003_walkingout.dae")) == 0)
                DX_ASSIMP_LOAD_FLAGS = aiProcess_LimitBoneWeights | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_MakeLeftHanded | aiProcess_FlipUVs;
            else
                DX_ASSIMP_LOAD_FLAGS = aiProcess_LimitBoneWeights | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcessPreset_TargetRealtime_Fast;

    }
    else
    {
        //MD5MESH / OBJ:
        DX_ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded;
    }

    aiScene* pAssimpScene=NULL;

    // IMPORT TO ASSIMP:
    Assimp::Importer importer;

    if (enginefile)
    {
        const aiScene* pScene = importer.ReadFile(meshFileName, DX_ASSIMP_LOAD_FLAGS);
        if (pScene == NULL)
            throw woma_exception("ModelLoader::Model file not found", __FILE__, __FUNCTION__, __LINE__);
        pAssimpScene = (aiScene*)pScene;
    } 
    else
    {
        const void* pBuffer=NULL;
        size_t bufferSize=0;

#ifndef GENERATE_PACK
        unsigned long filebufferSize=0;
        
        //Align with: SceneModel.h
        if (dxlevel == 86 || dxlevel == 87)
        {
            if (type== 1)
            {
                pBuffer = Forest_Huntress_idle_fbx_Model_LOD0_fbxBuffer;
                filebufferSize = Forest_Huntress_idle_fbx_Model_LOD0_fbx_size;
            }
        }
        if (dxlevel == 87)
        {
            if (type == 2) {
                pBuffer = Forest_Huntress_idle2_fbx_Model_LOD0_fbxBuffer;
                filebufferSize = Forest_Huntress_idle2_fbx_Model_LOD0_fbx_size;
            }
            if (type == 3) {
                pBuffer = Forest_Huntress_idle3_fbx_Model_LOD0_fbxBuffer;
                filebufferSize = Forest_Huntress_idle3_fbx_Model_LOD0_fbx_size;
            }
        }
        if (dxlevel >= 88)
        {
            if (type == 1) {
                pBuffer = Forest_Huntress_idle2_fbx_Model_LOD0_fbxBuffer;
                filebufferSize = Forest_Huntress_idle2_fbx_Model_LOD0_fbx_size;
            }
            if (type == 2) {
                pBuffer = Forest_Huntress_Walk_fbx_Model_LOD0_fbxBuffer;
                filebufferSize = Forest_Huntress_Walk_fbx_Model_LOD0_fbx_size;
            }
            if (type == 3) {
                pBuffer = Forest_Huntress_Walk_back_fbx_Model_LOD0_fbxBuffer;
                filebufferSize = Forest_Huntress_Walk_back_fbx_Model_LOD0_fbx_size;
            }
            if (type == 4) {
                pBuffer = Forest_Huntress_Run2_fbx_Model_LOD0_fbxBuffer;
                filebufferSize = Forest_Huntress_Run2_fbx_Model_LOD0_fbx_size;
            }
            if (type == 5) {
                pBuffer = Forest_Huntress_Strafe_left_fbx_Model_LOD0_fbxBuffer;     // >= 88 id=4
                filebufferSize = Forest_Huntress_Strafe_left_fbx_Model_LOD0_fbx_size;
            }
            if (type == 6) {
                pBuffer = Forest_Huntress_Strafe_right_fbx_Model_LOD0_fbxBuffer;    // >= 88 id=5
                filebufferSize = Forest_Huntress_Strafe_right_fbx_Model_LOD0_fbx_size;
            }
            if (type == 7) {
                pBuffer = Forest_Huntress_Draw_sword_fbx_Model_LOD0_fbxBuffer;      // >= 88 id=6
                filebufferSize = Forest_Huntress_Draw_sword_fbx_Model_LOD0_fbx_size;
            }
        }

        const aiScene* pScene = importer.ReadFileFromMemory(pBuffer, filebufferSize, DX_ASSIMP_LOAD_FLAGS, "fbx"); // format hint, e.g. "obj", "fbx", "gltf"
        if (pScene == NULL)
            throw woma_exception("ModelLoader::Model file not found", __FILE__, __FUNCTION__, __LINE__);
        pAssimpScene = (aiScene*)pScene;
#endif
    }

    if (!pAssimpScene  || pAssimpScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pAssimpScene->mRootNode || !pAssimpScene->HasMeshes())
    {
#ifdef DEBUG_MESH
        LOG_FILE << "error in assimp: " << importer.GetErrorString() << std::endl;
#endif
        throw woma_exception("ModelLoader::error in assimp", __FILE__, __FUNCTION__, __LINE__);
    }

    // Create ModelLoader:
    // -------------------
    ModelLoader ml = ModelLoader(pAssimpScene, scene, graphics, meshFileName, parentIndex);
    // LOAD ASSIMP data to our model:
    // ------------------------------
    SceneModel* model = ml.LoadModel(DX_ENGINE_LEVEL, type);

    aiScene* assimpScene;
    Assimp::Importer animImporter;
    const aiScene* pFbxAnimScene = animImporter.ReadFile(animFileName, DX_ASSIMP_LOAD_FLAGS);
    if (pFbxAnimScene && pFbxAnimScene->HasAnimations()) 
    {
        for (unsigned int i = 0; i < pFbxAnimScene->mNumAnimations; ++i) {
            const aiAnimation* anim = pFbxAnimScene->mAnimations[i];
            model->m_Animations.push_back(anim);
        }
        assimpScene = (aiScene*)pFbxAnimScene;
    }
    else
        assimpScene = (aiScene*) pAssimpScene;

    if (assimpScene->mAnimations)
    {
        if (assimpScene->mAnimations[0]->mTicksPerSecond != 0.0)
        {
            TicksPerSecond = assimpScene->mAnimations[0]->mTicksPerSecond;
        }
        else {
            TicksPerSecond = 30;
        }
#ifdef DEBUG_MESH
        LOG_FILE << "scene->HasAnimations: " << assimpScene->HasAnimations() << endl;
        LOG_FILE << "scene->mNumMeshes: " << assimpScene->mNumMeshes << endl;
        LOG_FILE << "scene->mAnimations[0]->mNumChannels: " << assimpScene->mAnimations[0]->mNumChannels << endl;
        LOG_FILE << "scene->mAnimations[0]->mDuration: " << assimpScene->mAnimations[0]->mDuration << endl;
        LOG_FILE << "scene->mAnimations[0]->mTicksPerSecond: " << assimpScene->mAnimations[0]->mTicksPerSecond << endl;
#endif
    }

#ifdef DEBUG_MESH
    LOG_FILE << "[1] ---		Node Names: " << endl;
    showNodeName(assimpScene->mRootNode);
    LOG_FILE << endl;
#endif

#ifdef DEBUG_MESH
    if (assimpScene->mAnimations)
    {
        LOG_FILE << "[2] ---		Animation Channels: " << endl;
        for (UINT i = 0; i < assimpScene->mAnimations[0]->mNumChannels; i++) {
            LOG_FILE << i << ":" << assimpScene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << endl;
            LOG_FILE << "mNumRotationKeys: " << assimpScene->mAnimations[0]->mChannels[i]->mNumRotationKeys << endl;
            for (size_t k = 0; k < assimpScene->mAnimations[0]->mChannels[i]->mNumRotationKeys; k++) {
                LOG_FILE << "\tChannel[" << i << "]: ";
                LOG_FILE << "RotationKeys[" << k << "]: ";
                LOG_FILE << assimpScene->mAnimations[0]->mChannels[i]->mRotationKeys[k].mValue.x << " ";
                LOG_FILE << assimpScene->mAnimations[0]->mChannels[i]->mRotationKeys[k].mValue.y << " ";
                LOG_FILE << assimpScene->mAnimations[0]->mChannels[i]->mRotationKeys[k].mValue.z << " ";
                LOG_FILE << endl;
            }
        }
        LOG_FILE << endl;
    }

    // Process(get data of): Vertex, Indices and Textures
    LOG_FILE << "[3] ---		Bone Names : " << endl;
#endif

    model->loaded = true; //ready to render!
	return model;
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

