// --------------------------------------------------------------------------------------------
// Filename: Transform.cpp
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
#include "Transform.h"
#include "LowLevel/Graphics.h"

Transform::Transform() :
    model(XMMatrixIdentity()),
    localModel(XMMatrixIdentity()),
    position(XMVectorZero()),
    rotation(XMVectorZero()),
    scale(1)
{
    dirty = true;
}

Transform& Transform::SetPosition(float x, float y, float z)
{
    dirty = true;
    position = XMVectorSet(x, y, z, 0);
    return *this;
}


Transform& Transform::Translate(float x, float y, float z)
{
    dirty = true;
    position = XMVectorAdd(position, XMVectorSet(x, y, z, 0));
    return *this;
}

Transform& Transform::RotateEulerAngles(float x, float y, float z)
{
    dirty = true;
    rotation = XMVectorAdd(rotation, XMVectorSet(x, y, z, 0));
    return *this;
}

Transform& Transform::UniformScale(float s)
{
    dirty = true;
    scale = s;
    return *this;
}

void Transform::Update(ID3D11DeviceContext* pContext, Graphics& graphics, ID3D11Buffer* buffer)
{
    graphics.UpdateBuffer(pContext, buffer, &model);
}

XMMATRIX Transform::GetModel()
{
    return model;
}

const XMMATRIX& Transform::GetLocalModel()
{
    if (dirty)
    {
        localModel = XMMatrixMultiply(XMMatrixScaling(scale, scale, scale),
            XMMatrixMultiply(XMMatrixRotationRollPitchYawFromVector(rotation),
                XMMatrixTranslationFromVector(position)));
        dirty = false;
    }
    return localModel;
}

void Transform::SetLocalModel(const XMMATRIX& model)
{
    dirty = false;
    localModel = model;
}

void Transform::SetAndDecomposeModel(const XMMATRIX& model)
{
    this->model = model;
    XMMatrixDecompose(&worldScale,&worldQuat, &worldPos, this->model);
}

const XMVECTOR& Transform::GetWorldPosition()
{
    return worldPos;
}
