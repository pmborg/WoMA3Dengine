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

void Transform::Update(Graphics& graphics, ID3D11Buffer* buffer)
{
    graphics.UpdateBuffer(buffer, &model);
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
