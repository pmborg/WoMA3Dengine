// --------------------------------------------------------------------------------------------
// Filename: RCameraClass.h
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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut14.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//////////////
// INCLUDES //
//////////////
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: RCameraClass
////////////////////////////////////////////////////////////////////////////////
class RCameraClass
{
private:
    struct VectorType
    {
        float x, y, z;
    };

public:
    RCameraClass();
    RCameraClass(const RCameraClass&);
    ~RCameraClass();

    void SetPosition(float, float, float);
    void SetRotation(float, float, float);

    void GetPosition(float*);
    void GetRotation(float*);

    void Render();
    void GetViewMatrix(float*);

private:
    void MatrixRotationYawPitchRoll(float*, float, float, float);
    void TransformCoord(VectorType&, float*);
    void BuildViewMatrix(VectorType, VectorType, VectorType);

private:
    float m_positionX, m_positionY, m_positionZ;
    float m_rotationX, m_rotationY, m_rotationZ;
    float m_viewMatrix[16];
};

#endif
#endif