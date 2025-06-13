// --------------------------------------------------------------------------------------------
// Filename: Scene.cpp
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
#include "OSengine.h"
#include "DX11Class.h"
#include "Scene/Scene.h"
#include "Scene/Material/Material.h"
#include "Resources/Mesh.h"
#include "LowLevel/Graphics.h"

static const XMMATRIX myXMMatrixIdentity = XMMatrixIdentity();

Scene::Scene()
{
    CreateSceneObject("Root", 0);
}

Scene::~Scene()
{
    SAFE_RELEASE(m_ModelBuffer);
    SAFE_RELEASE(m_ViewBuffer);
    SAFE_RELEASE(m_ProjBuffer);
}

void Scene::Start(Graphics& graphics)
{
    lightManager
        .SetEyePosition(m_MainCamera.m_EyePosition)
        .Update(graphics);

    auto clientRect = graphics.m_ClientRect;
    float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
    float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);
    m_MainCamera.m_AspectRatio = clientWidth / clientHeight;

    m_ModelBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &myXMMatrixIdentity);
    m_ViewBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &myXMMatrixIdentity);
    m_ProjBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &myXMMatrixIdentity);
}

void Scene::UpdateModelRecursive(SceneObject::Index idx, XMMATRIX model)
{
    auto obj = GetSceneObjectByIndex(idx);
    model = XMMatrixMultiply(obj->m_Transform.GetLocalModel(), model);
    obj->m_Transform.SetAndDecomposeModel(model);

	for (size_t i = 0; i < obj->m_ChildrenIndices.size(); ++i)
		UpdateModelRecursive(obj->m_ChildrenIndices[i], model);

}

void Scene::Update(Graphics& graphics, float deltaTime)
{
    //m_MainCamera.HandleMovement(input, deltaTime);
    lightManager.SetEyePosition(m_MainCamera.m_EyePosition);
    lightManager.Update(graphics);
	for (size_t i = 0; i < m_Node[0]->m_ChildrenIndices.size(); ++i)
	{
		auto objIndex = m_Node[0]->m_ChildrenIndices[i];
		UpdateModelRecursive(objIndex, XMMatrixIdentity());
	}
}

void Scene::UseModel(Graphics& graphics)
{
    auto deviceContext = graphics.m_DeviceContext;
    deviceContext->VSSetConstantBuffers(0, 1, &m_ModelBuffer);
}

void Scene::UpdateModel(Graphics& graphics, const XMMATRIX& model)
{
    graphics.UpdateBuffer(m_ModelBuffer, &model);
}

void Scene::UseCamera(Graphics& graphics, Camera& camera)
{
    #define m_driver11 ((DirectX::DX11Class*)driverList[SystemHandle->AppSettings->DRIVER])
    XMMATRIX* cameraP = ((DirectX::DX11Class*)m_driver11)->GetProjectionMatrix(CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_OPAC, NULL, NULL);
    XMMATRIX* cameraV = ((DirectX::DX11Class*)m_driver11)->GetViewMatrix(CAMERA_NORMAL, PROJECTION_PERSPECTIVE, PASS_OPAC, NULL, NULL);

    graphics.UpdateBuffer(m_ViewBuffer, cameraV);
    graphics.UpdateBuffer(m_ProjBuffer, cameraP);
    auto deviceContext = graphics.m_DeviceContext;
    deviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);
    deviceContext->VSSetConstantBuffers(2, 1, &m_ProjBuffer);
}

std::shared_ptr<SceneObject> Scene::CreateSceneObject(const std::string& name, SceneObject::Index parentIndex /*=0*/)
{
    SceneObject::Index index = m_Node.size();
    m_Node.push_back(std::make_shared<SceneObject>( name, index, parentIndex ));
    auto parentObj = GetSceneObjectByIndex(parentIndex);
    if (parentObj->m_Index != index)
        parentObj->m_ChildrenIndices.push_back(index);
    return GetSceneObjectByIndex(index);
}

std::shared_ptr<SceneObject> Scene::GetSceneObjectByIndex(SceneObject::Index index)
{
    if (index < m_Node.size())
    {
        return m_Node[index];
    }
    return m_Node[0];
}
