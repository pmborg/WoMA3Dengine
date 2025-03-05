////////////////////////////////////////////////////////////////////////////////
// Filename: RCameraClass.h
////////////////////////////////////////////////////////////////////////////////
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