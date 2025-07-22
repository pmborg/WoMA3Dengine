// --------------------------------------------------------------------------------------------
// Filename: Camera.cpp
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
// WomaIntegrityCheck = 1234525217;

#include "stdafx.h"
#include "Camera.h"
#include "LowLevel/Graphics.h"


XMMATRIX Camera::CalculateView()
{
    return XMMatrixLookAtLH(m_EyePosition, m_LookAt, m_Up);
}

XMMATRIX PerspectiveCamera::CalculateProjection()
{
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FovY), m_AspectRatio, 0.1f, 200.0f);
}

XMMATRIX OrthographicCamera::CalculateProjection()
{
    return XMMatrixOrthographicLH(m_ViewWidth, m_ViewHeight, m_NearZ, m_FarZ);
}
