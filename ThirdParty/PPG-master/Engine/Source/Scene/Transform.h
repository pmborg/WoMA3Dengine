// --------------------------------------------------------------------------------------------
// Filename: Transform.h
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
using namespace DirectX;

class Graphics;

struct Transform // try new code style
{
public:
    Transform();
    ~Transform() = default;
    Transform(Transform& t) = default;
    Transform(Transform&& t) = default;
    XMVECTOR position;
    XMVECTOR rotation;
    float scale;
    Transform& SetPosition(float x, float y, float z);
    Transform& Translate(float x, float y, float z);
    Transform& RotateEulerAngles(float x, float y, float z);
    Transform& UniformScale(float s);
    void Update(Graphics& graphics, ID3D11Buffer* buffer);
    XMMATRIX GetModel();
    const XMMATRIX& GetLocalModel();
    const XMVECTOR& GetWorldPosition();
    void SetLocalModel(const XMMATRIX& model);
    void SetAndDecomposeModel(const XMMATRIX& model);
private:
    friend class Scene;
    float dirty;
    XMMATRIX model;
    XMMATRIX localModel;
    XMVECTOR worldPos;
    XMVECTOR worldScale;
    XMVECTOR worldQuat;
};
