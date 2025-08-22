// --------------------------------------------------------------------------------------------
// Filename: Scene.h
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
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "SceneObject.h"

class Mesh;
class PhongMaterial;
class Graphics;

class Scene
{
public:
    Scene();
    ~Scene();
    PerspectiveCamera m_MainCamera;
    LightManager lightManager;
    std::shared_ptr<SceneObject> CreateSceneObject(const std::string& name, SceneObject::Index parentIndex = 0);
    std::shared_ptr<SceneObject> GetSceneObjectByIndex(SceneObject::Index index);
    std::vector<std::shared_ptr<SceneObject>> m_Node;
    void Start(ID3D11DeviceContext* pContext, Graphics& graphics);
    void Update(ID3D11DeviceContext* pContext, Graphics& graphics,float deltaTime);
    void UseModel(ID3D11DeviceContext* deviceContext, Graphics& graphics);
    void UpdateWorldMatrixModel(ID3D11DeviceContext* deviceContext, Graphics& graphics, const XMMATRIX& model);
    void UseCamera(ID3D11DeviceContext* pContext, Graphics& graphics, Camera& camera);
private:
    void UpdateModelRecursive(SceneObject::Index objIdx, XMMATRIX model);
    ID3D11Buffer* m_ModelBuffer;
    ID3D11Buffer* m_ViewBuffer;
    ID3D11Buffer* m_ProjBuffer;
};


