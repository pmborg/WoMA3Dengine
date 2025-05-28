// --------------------------------------------------------------------------------------------
// Filename: Camera.h
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

class Camera
{
public:
    Camera() = default;

    const XMVECTOR FORWARD = XMVectorSet(0, 0, 1, 0);
    const XMVECTOR RIGHT = XMVectorSet(1, 0, 0, 0);
    const XMVECTOR UP = XMVectorSet(0, 1, 0, 0);

    XMVECTOR m_EyePosition = XMVectorSet(0, 0, -10, 1);
    XMVECTOR m_Forward = FORWARD;
    XMVECTOR m_Right = RIGHT;
    XMVECTOR m_Up = UP;
    XMVECTOR m_LookAt = m_EyePosition + m_Forward;
    float m_Pitch = 0;
    float m_Yaw = 0;
    float m_NearZ = 0.1f;
    float m_FarZ = 200;
    float m_Speed = 10;

    virtual XMMATRIX CalculateProjection() = 0;
    XMMATRIX CalculateView();
 };

class PerspectiveCamera : public Camera
{
public:
    float m_AspectRatio = 1.777f;
    float m_FovY = 45;

    XMMATRIX CalculateProjection() override;
};

class OrthographicCamera : public Camera
{
public:
    float m_ViewWidth = 1;
    float m_ViewHeight = 1;

    XMMATRIX CalculateProjection() override;
};
