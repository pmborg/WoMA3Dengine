// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXcameraClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: Process the basic Camera Calculations
// --------------------------------------------------------------------------------------------

#include "platform.h"
#include <d3d11.h>
#if (defined DX_ENGINE) && (defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21 || defined USE_VIEW2D_SPRITES)

#include "DXcameraClass.h"

namespace DirectX
{

//The class constructor will initialize the position and rotation of the camera to be at the origin of the scene.
DXcameraClass::DXcameraClass(UINT camera_type)
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

	//public:
	m_positionX = m_positionY = m_positionZ = 0.0f;
	m_rotationX = m_rotationY = m_rotationZ = 0.0f;

	//private:
	lookAt = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
#if defined DX9sdk
	up9.x = 0.0f;
	up9.y = 0.0f;
	up9.z = 1.0f;
#endif

	// Setup the vector that points upwards.
	up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
#if defined DX9sdk
	up9.x = 0.0f;
	up9.y = 1.0f;
	up9.z = 0.0f;
#endif

}

DXcameraClass::~DXcameraClass() {CLASSDELETE();}

//The SetPosition and SetRotation functions are used for setting up the position and rotation of the camera.
void DXcameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void DXcameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

//The GetPosition and GetRotation functions return the location and rotation of the camera to calling functions.
/*D3DXVECTOR3*/ XMFLOAT3 DXcameraClass::GetPosition()
{
    return /*D3DXVECTOR3*/ XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

/*D3DXVECTOR3*/ XMFLOAT3 DXcameraClass::GetRotation()
{
    return /*D3DXVECTOR3*/ XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

//The Render function uses the position and rotation of the camera to build and update the view matrix. We first setup our variables for up, position, rotation, and so forth. Then at the origin of the scene we first rotate the camera based on the x, y, and z rotation of the camera. Once it is properly rotated when then translate the camera to the position in 3D space. With the correct values in the position, lookAt, and up we can then use the D3DXMatrixLookAtLH function to create the view matrix to represent the current camera rotation and translation.
void DXcameraClass::Render()
{
    static XMMATRIX rotationMatrix;
	static XMVECTOR position;
#if defined DX9sdk
	static D3DXMATRIX  rotationMatrix;
	static D3DXVECTOR3 position;
#endif

	// Setup the position of the camera in the world.
	position = XMVectorSet( m_positionX, m_positionY, m_positionZ, 0.0f );
#if defined DX9sdk
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;
#endif

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(m_rotationX * 0.0174532925f,	    
													m_rotationY * 0.0174532925f,	
													m_rotationZ * 0.0174532925f);
#if defined DX9sdk
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationY * 0.0174532925f,	//yaw
													m_rotationX * 0.0174532925f,	//pitch
													m_rotationZ * 0.0174532925f);	//roll
#endif

	lookAt = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
#if defined DX9sdk
	lookAt9.x = 0.0f;
	lookAt9.y = 0.0f;
	lookAt9.z = 1.0f;
#endif

	
	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;
#if defined DX9sdk
	D3DXVec3TransformCoord(&lookAt9, &lookAt9, &rotationMatrix);
	lookAt9 = position + lookAt9;
#endif

	// Finally create the view matrix from: EyePosition, FocusPosition, UpDirection
	/*
	//XMMatrixLookAtRH:
	m_viewMatrix.r[0] = { 1,0,0,0 };
	m_viewMatrix.r[1] = { 0,1,0,0 };
	m_viewMatrix.r[2] = { 0,0,1,0 };
	m_viewMatrix.r[3] = { m_positionX, m_positionY, m_positionZ, 1 };
	*/

	m_viewMatrix = XMMatrixLookAtLH( position, lookAt, up );
#if defined DX9sdk
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt9, &up9);
#endif
}

// After the Render function has been called to create the view matrix we can provide the update view matrix to calling functions 
// using this GetViewMatrix function. The view matrix will be one of the three main matrices used in the HLSL vertex shader.
	void DXcameraClass::GetViewMatrix(XMMATRIX& viewMatrix)

#if defined DX9sdk
	void DXcameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
#endif
{
	viewMatrix = m_viewMatrix;
}

void DXcameraClass::Set2DViewMatrix()
{
	m_viewmatrix2D = m_viewMatrix;
}

}

#endif

