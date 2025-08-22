// --------------------------------------------------------------------------------------------
// Filename: ModelLoader.h
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

#pragma once
#include "stdafx.h"
#include "OSengine.h"

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

    void ProcessMeshes(UINT this_level, UINT type);
    Mesh* GenerateMesh(UINT type, UINT m, aiMesh* mesh);
    PBRMaterial* GenerateMaterial(UINT this_level, UINT type, aiMesh* mesh);
    void LoadBones(UINT meshindex, aiMesh* mesh, std::vector<Vertex>& vertices);
    void GenerateSceneObjectHierarchy(aiNode* node, bool isRoot, int parentIndex);

    Texture* loadTexture(UINT this_level, UINT modeltype, aiMaterial* mat, aiTextureType type, unsigned int index = 0);
    std::map<std::string, Texture*> m_TextureMap;
    SceneModel* LoadModel(UINT this_level, UINT type);

    const aiScene* pAssimpScene;
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
    pAssimpScene(assimpScene),
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

    std::shared_ptr<SceneObject> object = m_Scene.CreateSceneObject(pAssimpScene->mRootNode->mName.C_Str(), parentIndex);
    m_Model = new SceneModel(object);
}

SceneModel* ModelLoader::LoadModel(UINT this_level, UINT type)
{
    // Create Mesh:
    ProcessMeshes(this_level, type); //Vetices + Indices + (Normals+Binormals+Tangents) + Materials + Bones

    // Create Skeleton Structure:
    if (m_HasBones)
    {
		m_Model->m_Skeleton = m_SkeletonLoader.GenerateSkeleton(pAssimpScene->mRootNode);
		m_Animator = &m_Model->m_SceneObject->m_Animator;
		m_Animator->m_IsEnabled = true;
		m_Animator->m_Skeleton = m_Model->m_Skeleton;
    }
    
    // For each sub-mesh create: SceneObject (Create Bones Matrix structure)
    GenerateSceneObjectHierarchy(pAssimpScene->mRootNode, true, m_Model->m_SceneObject->m_Index);

    // Return Model ready to: Update & Render
    return m_Model;
}

void ModelLoader::ProcessMeshes(UINT this_level, UINT type)
{
    std::shared_ptr<SceneObject> object = m_Model->m_SceneObject;
    for (UINT meshindex = 0; meshindex < pAssimpScene->mNumMeshes; meshindex++) //MESH-1
    {
		if (WOMA::main_loop_state < 0)
			return;
		LOG_FILE << "MESH id: " << meshindex << std::endl;
        aiMesh* rootMesh = pAssimpScene->mMeshes[meshindex];
        GenerateMesh(type, meshindex, rootMesh);
		if (WOMA::main_loop_state < 0)
			return;
        GenerateMaterial(this_level, type, rootMesh);
    }
}

void ModelLoader::LoadBones(UINT meshindex, aiMesh* pMesh, std::vector<Vertex>& vertices)
{
    if (pMesh->mNumBones>0)
        m_HasBones = true;

    for (UINT i = 0; i < pMesh->mNumBones; ++i)
    {
        // Obtain the bone name.
        std::string BoneName(pMesh->mBones[i]->mName.data);
#ifdef DEBUG_MESH
        LOG_FILE << pMesh->mBones[i]->mName.data << std::endl;
#endif

        aiBone* bone = pMesh->mBones[i];
        UINT bone_index = m_SkeletonLoader.AddBone(bone);
#ifdef DEBUG_MESH
        LOG_FILE << "#### bone_name: " << BoneName << "			 bone_index: " << i << std::endl;
#endif
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
                    SetFloatAtIndex(vertex.BoneWeights, k, weight);
                    switch (k)
                    {
                    case 0:
                        vertex.BoneIds.x = bone_index;
                        break;
                    case 1:
                        vertex.BoneIds.y = bone_index;
                        break;
                    case 2:
                        vertex.BoneIds.z = bone_index;
                        break;
                    case 3:
                        vertex.BoneIds.w = bone_index;
                        break;
                    }
                    
                    break;
                }
            }
#ifdef DEBUG_MESH
            LOG_FILE << " vertex_id: " << id << "	bone_index: " << bone_index << "		weight: " << weight << std::endl;
#endif
        }
    }
}

void ModelLoader::GenerateSceneObjectHierarchy(aiNode* node, bool isRoot, int parentIndex)
{
    if (node->mNumMeshes > 0)
    {
        for (UINT i = 0; i < node->mNumMeshes; ++i) //MESH-1
        {
            std::shared_ptr<SceneObject> object = isRoot ? m_Model->m_SceneObject : m_Scene.CreateSceneObject(node->mName.C_Str(), parentIndex);
            unsigned int meshId = node->mMeshes[i];
			if (m_Model->m_Meshes.size() <= 0)
				return;
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
		if (WOMA::game_state == GAME_STOP)
			return;
        GenerateSceneObjectHierarchy(node->mChildren[i], false, parentIndex);
    }
}

Mesh* ModelLoader::GenerateMesh(UINT type, UINT meshindex, aiMesh* aimesh)
{
    std::vector<Vertex> vertices;
    std::vector<WORD> indices;
    bool hasTexCoord = aimesh->mTextureCoords[0];

#ifdef DEBUG_MESH
    LOG_FILE << " bones: " << aimesh->mNumBones << " vertices: " << aimesh->mNumVertices << std::endl;
#endif

    if (!aimesh->mNormals)
    {
        // Allocate normals array
        aimesh->mNormals = new aiVector3D[aimesh->mNumVertices];
        // Zero initialize
        for (UINT i = 0; i < aimesh->mNumVertices; ++i) {
            aimesh->mNormals[i] = aiVector3D(0.0f, 0.0f, 0.0f);
        }

        // Accumulate face normals to each vertex
        for (UINT i = 0; i < aimesh->mNumFaces; ++i) {
            const aiFace& face = aimesh->mFaces[i];
            if (face.mNumIndices < 3) continue; // Not a triangle

            UINT i0 = face.mIndices[0];
            UINT i1 = face.mIndices[1];
            UINT i2 = face.mIndices[2];

            const aiVector3D& v0 = aimesh->mVertices[i0];
            const aiVector3D& v1 = aimesh->mVertices[i1];
            const aiVector3D& v2 = aimesh->mVertices[i2];

            // Compute face normal
            XMVECTOR p0 = XMLoadFloat3((XMFLOAT3*)&v0);
            XMVECTOR p1 = XMLoadFloat3((XMFLOAT3*)&v1);
            XMVECTOR p2 = XMLoadFloat3((XMFLOAT3*)&v2);

            XMVECTOR edge1 = XMVectorSubtract(p1, p0);
            XMVECTOR edge2 = XMVectorSubtract(p2, p0);
            XMVECTOR faceNormal = XMVector3Cross(edge1, edge2);

            // Add face normal to each vertex normal
            XMFLOAT3 fn;
            XMStoreFloat3(&fn, faceNormal);

            aimesh->mNormals[i0] += aiVector3D(fn.x, fn.y, fn.z);
            aimesh->mNormals[i1] += aiVector3D(fn.x, fn.y, fn.z);
            aimesh->mNormals[i2] += aiVector3D(fn.x, fn.y, fn.z);
        }

        // Normalize all normals
        for (UINT i = 0; i < aimesh->mNumVertices; ++i) {
            aiVector3D& n = aimesh->mNormals[i];
            float len = n.Length();
            if (len > 0.0f)
                n /= len;
        }
    }    

    if (!aimesh->mTangents || aimesh->mBitangents)
    {
        // Allocate tangent and bitangent arrays
        aimesh->mTangents = new aiVector3D[aimesh->mNumVertices];
        aimesh->mBitangents = new aiVector3D[aimesh->mNumVertices];
        for (UINT i = 0; i < aimesh->mNumVertices; ++i) {
            aimesh->mTangents[i] = aiVector3D(0.0f, 0.0f, 0.0f);
            aimesh->mBitangents[i] = aiVector3D(0.0f, 0.0f, 0.0f);
        }

        // Accumulate tangents and bitangents per face
        for (UINT i = 0; i < aimesh->mNumFaces; ++i) {
            const aiFace& face = aimesh->mFaces[i];
            if (face.mNumIndices < 3) continue; // Not a triangle

            UINT i0 = face.mIndices[0];
            UINT i1 = face.mIndices[1];
            UINT i2 = face.mIndices[2];

            const aiVector3D& v0 = aimesh->mVertices[i0];
            const aiVector3D& v1 = aimesh->mVertices[i1];
            const aiVector3D& v2 = aimesh->mVertices[i2];

            // Texture coordinates must exist
            if (!aimesh->mTextureCoords[0]) continue;
            const aiVector3D& uv0 = aimesh->mTextureCoords[0][i0];
            const aiVector3D& uv1 = aimesh->mTextureCoords[0][i1];
            const aiVector3D& uv2 = aimesh->mTextureCoords[0][i2];

            // Edges of the triangle : position delta
            aiVector3D deltaPos1 = v1 - v0;
            aiVector3D deltaPos2 = v2 - v0;
            // UV delta
            aiVector3D deltaUV1 = uv1 - uv0;
            aiVector3D deltaUV2 = uv2 - uv0;

            float r = (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            if (fabs(r) < 1e-8f) r = 1.0f; // Prevent division by zero
            else r = 1.0f / r;

            aiVector3D tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            aiVector3D bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

            aimesh->mTangents[i0] += tangent;
            aimesh->mTangents[i1] += tangent;
            aimesh->mTangents[i2] += tangent;

            aimesh->mBitangents[i0] += bitangent;
            aimesh->mBitangents[i1] += bitangent;
            aimesh->mBitangents[i2] += bitangent;
        }

        // Normalize all tangents and bitangents
        for (UINT i = 0; i < aimesh->mNumVertices; ++i) {
            float lenT = aimesh->mTangents[i].Length();
            if (lenT > 0.0f)
                aimesh->mTangents[i] /= lenT;
            float lenB = aimesh->mBitangents[i].Length();
            if (lenB > 0.0f)
                aimesh->mBitangents[i] /= lenB;
        }
    }

#ifdef DEBUG_MESH
    LOG_FILE << "--- vertices" << std::endl;
#endif

    for (UINT v = 0; v < aimesh->mNumVertices; v++)
    {
        Vertex vertex = {};
        vertex.Position.x = aimesh->mVertices[v].x;
        vertex.Position.y = aimesh->mVertices[v].y;
        vertex.Position.z = aimesh->mVertices[v].z;

        if (hasTexCoord)
        {
            vertex.TexCoord.x = aimesh->mTextureCoords[0][v].x;
            vertex.TexCoord.y = aimesh->mTextureCoords[0][v].y;
            if (vertex.TexCoord.y > 1) {
                vertex.TexCoord.y -= 1;
            }
        }

        if (aimesh->mNormals)
		{
            vertex.Normal.x = aimesh->mNormals[v].x;
            vertex.Normal.y = aimesh->mNormals[v].y;
            vertex.Normal.z = aimesh->mNormals[v].z;
		}
#ifdef DEBUG_MESH
        LOG_FILE << "Vertice: [" << v << "] X: " << vertex.Position.x << " Y: " << vertex.Position.y << " Z: " << vertex.Position.z;
        LOG_FILE << " -- Ux: " << vertex.TexCoord.x << " Vy: " << vertex.TexCoord.y << std::endl;
#endif
        if (aimesh->mTangents)
		{
            vertex.Tangent.x = aimesh->mTangents[v].x;
            vertex.Tangent.y = aimesh->mTangents[v].y;
            vertex.Tangent.z = aimesh->mTangents[v].z;
		}
        if (aimesh->mBitangents)
		{
            vertex.Binormal.x = aimesh->mBitangents[v].x;
            vertex.Binormal.y = aimesh->mBitangents[v].y;
            vertex.Binormal.z = aimesh->mBitangents[v].z;
		}
		vertices.push_back(vertex);
    }

#ifdef DEBUG_MESH
    LOG_FILE << "---        indices" << std::endl;
#endif
    for (UINT i = 0; i < aimesh->mNumFaces; i++)
    {
        aiFace face = aimesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++) {
            int index = face.mIndices[j];
            indices.push_back(index);
#ifdef DEBUG_MESH
        LOG_FILE << index << " ";
#endif
        }
#ifdef DEBUG_MESH
        LOG_FILE << std::endl;
#endif
    }

    LoadBones(meshindex, aimesh, vertices);

    Mesh* mesh = new Mesh(vertices, indices, m_Graphics, (aimesh->mTangents && aimesh->mBitangents) ? false : true);
    m_Model->m_Meshes.push_back(mesh);

    return mesh;
}

PBRMaterial* ModelLoader::GenerateMaterial(UINT this_level, UINT modeltype, aiMesh* mesh)
{
    PBRMaterial* material = new PBRMaterial();
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = pAssimpScene->mMaterials[mesh->mMaterialIndex];

        // 0: Albedo/BaseColor
        Texture* albedo = nullptr;
        if (m_LoadType == LoadType::GLTF) {
            albedo = loadTexture(this_level, modeltype, mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE);
        }
        else {
            albedo = loadTexture(this_level, modeltype, mat, aiTextureType_DIFFUSE);
        }
        if (albedo) material->UseAlbedoMap(albedo);

        // 1: Normal
        Texture* normal = loadTexture(this_level, modeltype, mat, aiTextureType_NORMALS);
        if (normal) material->UseNormalMap(normal);

        // 2: Bump
        Texture* bump = loadTexture(this_level, modeltype, mat, aiTextureType_HEIGHT);
        if (bump) material->UseBumpMap(bump);

        // 3: Ambient Occlusion
        Texture* ao = loadTexture(this_level, modeltype, mat, aiTextureType_LIGHTMAP);
        if (ao) material->UseAoMap(ao);

        // 4: Emissive
        Texture* emissive = loadTexture(this_level, modeltype, mat, aiTextureType_EMISSIVE);
        if (emissive) material->UseEmissiveMap(emissive);
#if _NOT_YET
        // Specular (classic)
        Texture* specular = loadTexture(this_level, modeltype, mat, aiTextureType_SPECULAR);
        if (specular) material->UseSpecularMap(specular);
#endif
        // 5: Opacity/Alpha
        Texture* opacity = loadTexture(this_level, modeltype, mat, aiTextureType_OPACITY);
        if (opacity) material->UseAlphaMap(opacity);

        //6: Metallic
        Texture* metallic = loadTexture(this_level, modeltype, mat, aiTextureType_METALNESS);
        if (metallic) material->UseMetallicMap(metallic);

        //7: Roughness
        Texture* roughness = loadTexture(this_level, modeltype, mat, aiTextureType_DIFFUSE_ROUGHNESS);
        if (roughness) material->UseRoughnessMap(roughness);

        // Metallic-Roughness (GLTF)
        if (m_LoadType == LoadType::GLTF)
        {
            material->ConvertToLinear(true);
        
            Texture* occlusionMetalRough = loadTexture(this_level, modeltype, mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE);
            if (occlusionMetalRough) material->UseOccRoughMetal(occlusionMetalRough);
        
            float metallic = 1.0f, roughness = 1.0f;
            if (mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic) == AI_SUCCESS)
                material->SetMetallic(metallic);
            if (mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
                material->SetRoughness(roughness);
        
            // Handle GLTF extensions here (clearcoat, sheen, transmission, etc.)
        }
        else
        {
            // Classic workflow: set color, roughness, metallic, etc.
            aiColor3D colour;
            if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, colour) == aiReturn_SUCCESS)
                material->SetAlbedo(colour[0], colour[1], colour[2]);

            //material->SetRoughness(0.9f);
            //material->SetMetallic(0.0f);

            //float shininess;
            //if (mat->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
            //    material->SetRoughness(sqrt(2.0f / (shininess + 2.0f)));
        }

        // 8: Emissive color
        aiColor3D emissiveColor;
        if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) == aiReturn_SUCCESS)
            material->SetEmissive(emissiveColor[0], emissiveColor[1], emissiveColor[2]);
#if _NOT_YET
        // Double-sided
        int doubleSided = 0;
        if (mat->Get("$mat.twosided", 0, 0, doubleSided) == aiReturn_SUCCESS && doubleSided)
            material->SetDoubleSided(true);
#endif
    }
    else
    {
        material->SetAlbedo(1, 0, 1);
    }
    m_Model->m_Materials.push_back(material);
    return material;
}

Texture* ModelLoader::loadTexture(UINT this_level, UINT modeltype, aiMaterial* mat, aiTextureType type, unsigned int index)
{
    bool hasTex = mat->GetTextureCount(type) > 0;
    Texture* texture = nullptr;
    aiString str;
    if (hasTex)
    {
        mat->GetTexture(type, index, &str);
        std::string textureName = str.C_Str();
        textureName = textureName.substr(textureName.find_last_of("/\\") + 1);
        textureName = m_Directory + textureName;

        // Normalize to lowercase and forward slashes
        std::transform(textureName.begin(), textureName.end(), textureName.begin(), ::tolower);
        std::replace(textureName.begin(), textureName.end(), '\\', '/');

        std::wstring stemp = std::wstring(textureName.begin(), textureName.end());
        LPCWSTR path = stemp.c_str();

        auto it = m_TextureMap.find(textureName);
        if (it != m_TextureMap.end())
        {
            texture = it->second;
        }
        else
        {
            texture = Texture::LoadTextureFromPath(this_level, modeltype, m_Graphics, path);
            m_TextureMap.emplace(textureName, texture);
            m_Model->m_Textures.push_back(texture);
        }
    }
    else {
        if (this_level >= 89) 
        {
            switch (modeltype)
            {
            case 1:
                {
                    mat->GetTexture(aiTextureType_DIFFUSE, index, &str);
                    std::string albedo = str.C_Str();

                    std::string texFile;
                    if (_stricmp(albedo.c_str(), "Skin_1_Armor_and_Weapon_Albedo.png") == 0)
                    {
                        switch (type)
                        {
                        case aiTextureType_NORMALS:   // Normal
                            texFile = "Skin_1_Armor_and_Weapon_Normal.png";
                            break;
                        case aiTextureType_HEIGHT:    // Bump
                            texFile = "Skin_1_Armor_and_Weapon_Height.png";
                            break;
                        case aiTextureType_LIGHTMAP:  // Ambient Occlusion
                            texFile = "Skin_1_Armor_and_Weapon_AO.png";
                            break;
                        case aiTextureType_EMISSIVE:  // Emissive
                            return nullptr;
                        case aiTextureType_METALNESS: // Metallic
                            texFile = "Skin_1_Armor_and_Weapon_Metallic.png";
                            break;
                        case aiTextureType_DIFFUSE_ROUGHNESS: // Roughness
                            texFile = "Skin_1_Armor_and_Weapon_Roughness.png";
                            break;
                        case aiTextureType_OPACITY:   // Opacity/Alpha
                            texFile = "Skin_1_Armor_and_Weapon_Alpha.png";
                            break;
                        default:
                            return nullptr;
                        }
                    }
                    else if (_stricmp(albedo.c_str(), "Skin_1_Body_Albedo.png") == 0)
                    {
                        switch (type)
                        {
                        case aiTextureType_NORMALS:   // Normal
                            texFile = "Skin_1_Body_Normal.png";
                            break;
                        case aiTextureType_HEIGHT:    // Bump
                            texFile = "Skin_1_Body_Height.png";
                            break;
                        case aiTextureType_LIGHTMAP:  // Ambient Occlusion
                            texFile = "Skin_1_Body_Mixed_AO.png";
                            break;
                        case aiTextureType_EMISSIVE:  // Emissive
                            return nullptr;
                        case aiTextureType_OPACITY:   // Opacity/Alpha
                            return nullptr;
                        case aiTextureType_METALNESS: // Metallic
                            texFile = "Skin_1_Body_Metallic.png";
                            break;
                        case aiTextureType_DIFFUSE_ROUGHNESS: // Roughness
                            texFile = "Skin_1_Body_Roughness.png";
                            break;
                        default:
                            return nullptr;
                        }
                    }
                    else if (_stricmp(albedo.c_str(), "Skin_1_Hair_Albedo.png") == 0)
                    {
                        switch (type)
                        {
                        case aiTextureType_NORMALS:   // Normal
                            texFile = "Skin_1_Hair_normal.png";
                            break;
                        case aiTextureType_HEIGHT:    // Bump
                            texFile = "Skin_1_Hair_Height.png";
                            break;
                        case aiTextureType_LIGHTMAP:  // Ambient Occlusion
                            return nullptr;
                        case aiTextureType_EMISSIVE:  // Emissive
                            return nullptr;
                        case aiTextureType_OPACITY:   // Opacity/Alpha
                            texFile = "Skin_1_Hair_Alpha.png";
                            break;
                        case aiTextureType_METALNESS: // Metallic
                            texFile = "Skin_1_Hair_Metallic.png";
                            break;
                        case aiTextureType_DIFFUSE_ROUGHNESS: // Roughness
                            texFile = "Skin_1_Hair_Roughness.png";
                            break;
                        default:
                            return nullptr;
                        }
                    }
                    else if (_stricmp(albedo.c_str(), "Skin_1_Head_Albedo.png") == 0)
                    {
                        switch (type)
                        {
                        case aiTextureType_NORMALS:   // Normal
                            texFile = "Skin_1_Head2_Normal.png";
                            break;
                        case aiTextureType_HEIGHT:    // Bump
                            texFile = "Skin_1_Head_Height.png";
                            break;
                        case aiTextureType_LIGHTMAP:  // Ambient Occlusion
                            texFile = "Skin_1_Head_ao.png";
                            break;
                        case aiTextureType_EMISSIVE:  // Emissive
                            return nullptr;
                        case aiTextureType_OPACITY:   // Opacity/Alpha
                            texFile = "Skin_1_Head_Alpha.png";
                            break;
                        case aiTextureType_METALNESS: // Metallic
                            texFile = "Skin_1_Head_Metallic.png";
                            break;
                        case aiTextureType_DIFFUSE_ROUGHNESS: // Roughness
                            texFile = "Skin_1_Head_Roughness.png";
                            break;
                        default:
                            return nullptr;
                        }
                    }

                    // After the switch/case blocks for each albedo
                    if (texFile.empty())
                        return nullptr;

                    // Compose the full path
                    std::string textureName = m_Directory + texFile;
                    std::transform(textureName.begin(), textureName.end(), textureName.begin(), ::tolower);
                    std::replace(textureName.begin(), textureName.end(), '\\', '/');

                    // Check cache
                    auto it = m_TextureMap.find(textureName);
                    if (it != m_TextureMap.end()) {
                        return it->second;
                    }

                    // Load and cache
                    std::wstring stemp = std::wstring(textureName.begin(), textureName.end());
                    LPCWSTR path = stemp.c_str();
                    texture = Texture::LoadTextureFromPath(this_level, modeltype, m_Graphics, path);
                    if (texture) {
                        m_TextureMap.emplace(textureName, texture);
                        m_Model->m_Textures.push_back(texture);
                    }
                }
            }
        }
    }
    return texture;
}

extern std::string original_files[];