// --------------------------------------------------------------------------------------------
// Filename: ModelLoader.h
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

#include <map>
#include "SceneModel.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/pbrmaterial.h"
#include "LowLevel/Graphics.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Material/Material.h"
#include "Scene/Scene.h"
#include "SkeletonLoader.h"
#include "Skeleton.h"
#include <fstream> 

class ModelLoader
{
    enum class LoadType
    {
        UNKNOWN,
        OBJ,
        GLTF,
    };
private:
    ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& fileName, SceneObject::Index parentIndex);

    void ProcessMeshes();
    Mesh* GenerateMesh(aiMesh* mesh);
    PBRMaterial* GenerateMaterial(aiMesh* mesh);
    void ProcessBones(aiMesh* mesh, std::vector<Vertex>& vertices);
    void GenerateSceneObjectHierarchy(aiNode* node, bool isRoot, int parentIndex);

    Texture* loadTexture(aiMaterial* mat, aiTextureType type, unsigned int index = 0);
    std::map<std::string, Texture*> m_TextureMap;
    SceneModel* LoadModel();

    const aiScene* m_AiScene;
    Scene& m_Scene;
    LoadType m_LoadType = LoadType::UNKNOWN;
    Graphics& m_Graphics;
    std::string m_Directory;

    SceneModel* m_Model;
    SkeletonLoader m_SkeletonLoader;
    Animator* m_Animator;
    friend class SceneModel;

    bool m_HasBones = false;
};

ModelLoader::ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& fileName, SceneObject::Index parentIndex) :
    m_AiScene(assimpScene),
    m_Scene(scene),
    m_Graphics(graphics),
    m_SkeletonLoader(assimpScene)
{
    std::filesystem::path filePath = fileName;
    m_Directory = filePath.parent_path().string() + "\\";
    if (filePath.has_extension())
    {
        std::filesystem::path extension = filePath.extension();
        if (extension == ".obj")
        {
            m_LoadType = LoadType::OBJ;
        }
        else if (extension == ".gltf")
        {
            m_LoadType = LoadType::GLTF;
        }
    }
    std::shared_ptr<SceneObject> object = m_Scene.CreateSceneObject(m_AiScene->mRootNode->mName.C_Str(), parentIndex);
    m_Model = new SceneModel(object);
}

SceneModel* ModelLoader::LoadModel()
{
    ProcessMeshes();

    if (m_HasBones)
    {
		m_Model->m_Skeleton = m_SkeletonLoader.GenerateSkeleton(m_AiScene->mRootNode);
		m_Animator = &m_Model->m_SceneObject->m_Animator;
		m_Animator->m_IsEnabled = true;
		m_Animator->m_Skeleton = m_Model->m_Skeleton;
    }

    GenerateSceneObjectHierarchy(m_AiScene->mRootNode, true, m_Model->m_SceneObject->m_Index);

    return m_Model;
}

void ModelLoader::ProcessMeshes()
{
    std::shared_ptr<SceneObject> object = m_Model->m_SceneObject;
    for (UINT i = 0; i < m_AiScene->mNumMeshes; i++)
    {
		LOG_FILE << "MESH id: " << i << std::endl;
        aiMesh* aimesh = m_AiScene->mMeshes[i];
        GenerateMesh(aimesh);
        GenerateMaterial(aimesh);
    }
}

void ModelLoader::ProcessBones(aiMesh* mesh, std::vector<Vertex>& vertices)
{
    for (UINT i = 0; i < mesh->mNumBones; ++i)
    {
        m_HasBones = true;
        aiBone* bone = mesh->mBones[i];
        int boneId = m_SkeletonLoader.AddBone(bone);
        for (UINT j = 0; j < bone->mNumWeights; ++j)
        {
            aiVertexWeight vertWeight = bone->mWeights[j];
            UINT id = vertWeight.mVertexId;
            float weight = vertWeight.mWeight;

            // only support 4 weights
            for (UINT k = 0; k < 4; ++k)
            {
                Vertex& vertex = vertices[id];
                if (GetFloatAtIndex(vertex.BoneWeights, k) == 0.0)
                {
                    SetFloatAtIndex(vertex.BoneIds, k, boneId);
                    SetFloatAtIndex(vertex.BoneWeights, k, weight);
                    break;
                }
            }
        }
    }
}

void ModelLoader::GenerateSceneObjectHierarchy(aiNode* node, bool isRoot, int parentIndex)
{
    if (node->mNumMeshes > 0)
    {
        for (UINT i = 0; i < node->mNumMeshes; ++i)
        {
            std::shared_ptr<SceneObject> object = isRoot ? m_Model->m_SceneObject : m_Scene.CreateSceneObject(node->mName.C_Str(), parentIndex);
            unsigned int meshId = node->mMeshes[i];
            Mesh* mesh = m_Model->m_Meshes[meshId];
            PBRMaterial* material = m_Model->m_Materials[meshId];
            MeshRenderer& meshRenderer = object->m_MeshRenderer;
            meshRenderer.m_IsEnabled = true;
            meshRenderer.m_Mesh = mesh;
            meshRenderer.m_Material = material;
            object->m_Transform.SetLocalModel(XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)));

            if (m_HasBones)
            {
                meshRenderer.m_Animator = m_Animator;
            }
            isRoot = false;
            parentIndex = object->m_Index;
        }
    }

    for (UINT i = 0; i < node->mNumChildren; ++i)
    {
        GenerateSceneObjectHierarchy(node->mChildren[i], false, parentIndex);
    }
}

Mesh* ModelLoader::GenerateMesh(aiMesh* aimesh)
{
    std::vector<Vertex> vertices;
    std::vector<WORD> indices;
    bool hasTexCoord = aimesh->mTextureCoords[0];

    for (UINT i = 0; i < aimesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position.x = aimesh->mVertices[i].x;
        vertex.Position.y = aimesh->mVertices[i].y;
        vertex.Position.z = aimesh->mVertices[i].z;
        if (hasTexCoord)
        {
            vertex.TexCoord.x = aimesh->mTextureCoords[0][i].x;
            vertex.TexCoord.y = aimesh->mTextureCoords[0][i].y;
        }
		//if (aimesh->mNormals)
		{
        vertex.Normal.x = aimesh->mNormals[i].x;
        vertex.Normal.y = aimesh->mNormals[i].y;
        vertex.Normal.z = aimesh->mNormals[i].z;
		}

		//if (aimesh->mTangents)
		{
        vertex.Tangent.x = aimesh->mTangents[i].x;
        vertex.Tangent.y = aimesh->mTangents[i].y;
        vertex.Tangent.z = aimesh->mTangents[i].z;
		}
		//if (aimesh->mBitangents)
		{
        vertex.Binormal.x = aimesh->mBitangents[i].x;
        vertex.Binormal.y = aimesh->mBitangents[i].y;
        vertex.Binormal.z = aimesh->mBitangents[i].z;
		}
		vertices.push_back(vertex);
    }

    for (UINT i = 0; i < aimesh->mNumFaces; i++)
    {
        aiFace face = aimesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back((WORD)face.mIndices[j]);
    }

    ProcessBones(aimesh, vertices);
    Mesh* mesh = new Mesh(vertices, indices, m_Graphics, false);
    m_Model->m_Meshes.push_back(mesh);

	//for (UINT i = 0; i < aimesh->mNumVertices; i++)
	//{
	//	LOG_FILE << "id: " << i << " weight: " << vertices[i].BoneWeights.x;
	//	LOG_FILE << " " << vertices[i].BoneWeights.y;
	//	LOG_FILE << " " << vertices[i].BoneWeights.z;
	//	LOG_FILE << " " << vertices[i].BoneWeights.w << std::endl;
	//	LOG_FILE << "id: " << i << " bone_index: " << vertices[i].BoneIds.x;
	//	LOG_FILE << " " << vertices[i].BoneIds.y;
	//	LOG_FILE << " " << vertices[i].BoneIds.z;
	//	LOG_FILE << " " << vertices[i].BoneIds.w << std::endl;
	//}

    return mesh;
}

PBRMaterial* ModelLoader::GenerateMaterial(aiMesh* mesh)
{
    PBRMaterial* material = new PBRMaterial();
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = m_AiScene->mMaterials[mesh->mMaterialIndex];

        Texture* normal = loadTexture(mat, aiTextureType_NORMALS);
        Texture* bump = loadTexture(mat, aiTextureType_HEIGHT);
        if (normal) material->UseNormalMap(normal);
        else if (bump) material->UseBumpMap(bump);

        Texture* ao = loadTexture(mat, aiTextureType_LIGHTMAP);
        if (ao) material->UseAoMap(ao);

        Texture* emissive = loadTexture(mat, aiTextureType_EMISSIVE);
        if (emissive) material->UseEmissiveMap(emissive);

        if (m_LoadType == LoadType::GLTF)
        {
            material->ConvertToLinear(true);
            Texture* albedo = loadTexture(mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE);
            if (albedo) material->UseAlbedoMap(albedo);

            Texture* occlusionMetalRough = loadTexture(mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE);
            if (occlusionMetalRough) material->UseOccRoughMetal(occlusionMetalRough);

            float metallic;
            if (mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic) == AI_SUCCESS)
            {
                material->SetMetallic(metallic);
            }
            float roughness;
            if (mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
            {
                material->SetRoughness(roughness);
            }
        }
        else
        {
            Texture* albedo = loadTexture(mat, aiTextureType_DIFFUSE);
            if (albedo) material->UseAlbedoMap(albedo);

            aiColor3D colour;
            aiReturn res = mat->Get(AI_MATKEY_COLOR_DIFFUSE, colour);
            if (res == aiReturn_SUCCESS)
                material->SetAlbedo(colour[0], colour[1], colour[2]);

            material->SetRoughness(0.9f);
            material->SetMetallic(0.0f);

            float shininess;
            res = mat->Get(AI_MATKEY_SHININESS, shininess);
            if (res == aiReturn_SUCCESS)
            {
                // convert shininess to roughness
                float roughness = sqrt(2.0f / (shininess + 2.0f));
                material->SetRoughness(roughness);
            }
        }
    }
    else
    {
        material
            ->SetAlbedo(1, 0, 1);
    }
    m_Model->m_Materials.push_back(material);
    return material;
}

Texture* ModelLoader::loadTexture(aiMaterial* mat, aiTextureType type, unsigned int index)
{
    bool hasTex = mat->GetTextureCount(type) > 0;
    Texture* texture = nullptr;
    if (hasTex)
    {
        aiString str;
        mat->GetTexture(type, index, &str);
        std::string textureName = str.C_Str();
        textureName = m_Directory + textureName;
        std::wstring stemp = std::wstring(textureName.begin(), textureName.end());
        LPCWSTR path = stemp.c_str();
        auto it = m_TextureMap.find(textureName);
        if (it != m_TextureMap.end())
        {
            texture = it->second;
        }
        else
        {
            texture = Texture::LoadTextureFromPath(m_Graphics, path);
            m_TextureMap.emplace(textureName, texture);
            m_Model->m_Textures.push_back(texture);
        }
    }
    return texture;
}

