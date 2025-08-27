// --------------------------------------------------------------------------------------------
// Filename: DXcameraClass.cpp
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
// PURPOSE: Process the basic Camera Calculations
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#if defined DX_ENGINE
#include <d3d11.h>
#if (defined DX_ENGINE) && (defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21 || defined USE_VIEW2D_SPRITES)

#include "DXcameraClass.h"

//The class constructor will initialize the position and rotation of the camera to be at the origin of the scene.
DXcameraClass::DXcameraClass(UINT camera_type)
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

	//public:
	m_positionX = m_positionY = m_positionZ = 0.0f;
	m_rotationX = m_rotationY = m_rotationZ = 0.0f;

	//private:
#if defined DX12 || defined DX11 || defined DX9
	lookAt = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
#endif

	// Setup the vector that points upwards.
#if defined DX12 || defined DX11 || defined DX9
	up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
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
XMFLOAT3 DXcameraClass::GetPosition()
{
    return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 DXcameraClass::GetRotation()
{
    return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

//The Render function uses the position and rotation of the camera to build and update the view matrix. We first setup our variables for up, position, rotation, and so forth. Then at the origin of the scene we first rotate the camera based on the x, y, and z rotation of the camera. Once it is properly rotated when then translate the camera to the position in 3D space. With the correct values in the position, lookAt, and up we can then use the D3DXMatrixLookAtLH function to create the view matrix to represent the current camera rotation and translation.
#if defined USE_3RD_PERSON_CAMERA
void DXcameraClass::CalculateViewMatrix_3rd_PersonCamera(float camYaw, float camPitch, bool sky)
{
    static float charCamDist = 2.0f; // 15.0f This is the distance between the camera and the character
    static float YcamDist = 0.0f;    // 5.0f

    // Third Person Camera
    // Set the cameras target to be looking at the character.
    if (sky)
        camTarget = XMVectorSet(0, 0, 0, 0.0f); //char position
    else
        camTarget = XMVectorSet(m_positionX, m_positionY, m_positionZ, 0.0f); //char position

    // This line is because this lessons model was set to stand on the point (0,0,0) (my bad), and we
    // don't want to just be looking at the models feet, so we move the camera's target vector up 5 units
    camTarget = XMVectorSetY(camTarget, XMVectorGetY(camTarget) + YcamDist );

    // Unlike before, when we rotated the cameras target vector around the cameras position,
    // we are now rotating the cameras position around it's target (which is the character)
    // Rotate camera around target
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);
    camPosition = XMVector3TransformNormal(DefaultForward, camRotationMatrix);
    camPosition = XMVector3Normalize(camPosition);

    // Set our cameras position to rotate around the character. We need to add 5 to the characters
    // position's y axis because i'm stupid and modeled the character in the 3d modeling program
    // to be "standing" on (0,0,0), instead of centered around it ;) Well target her head here though
    camPosition = (camPosition * charCamDist) + camTarget;

    // We need to set our cameras forward and right vectors to lay
    // in the worlds xz plane, since they are the vectors we will
    // be using to determine the direction our character is running
    camForward = XMVector3Normalize(camTarget - camPosition);	// Get forward vector based on target
    camForward = XMVectorSetY(camForward, 0.0f);	// set forwards y component to 0 so it lays only on
    // the xz plane
    camForward = XMVector3Normalize(camForward);
    // To get our camera's Right vector, we set it's x component to the negative z component from the
    // camera's forward vector, and the z component to the camera forwards x component
    camRight = XMVectorSet(-XMVectorGetZ(camForward), 0.0f, XMVectorGetX(camForward), 0.0f);

    // Our camera does not "roll", so we can safely assume that the cameras right vector is always
    // in the xz plane, so to get the up vector, we just get the normalized vector from the camera
    // position to the cameras target, and cross it with the camera's Right vector
    camUp = XMVector3Normalize(XMVector3Cross(XMVector3Normalize(camPosition - camTarget), camRight));

    m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, up);
}
#endif
void DXcameraClass::CalculateViewMatrix()
{
#if defined DX12 || defined DX11 || defined DX9
	XMMATRIX rotationMatrix = {};
	XMVECTOR position = {};
#endif

	// Setup the position of the camera in the world.
#if defined DX12 || defined DX11 || defined DX9
	position = XMVectorSet( m_positionX, m_positionY, m_positionZ, 0.0f );
#endif

	// Create the rotation matrix from the yaw, pitch, and roll values.
#if defined DX12 || defined DX11 || defined DX9
	rotationMatrix = XMMatrixRotationRollPitchYaw(  m_rotationX * 0.0174532925f,	    
													m_rotationY * 0.0174532925f,	
													m_rotationZ * 0.0174532925f);
#endif

#if defined DX12 || defined DX11 || defined DX9
	lookAt = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
#endif
	
#if defined DX12 || defined DX11 || defined DX9
	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;
#endif

	// Finally create the view matrix from: EyePosition, FocusPosition, UpDirection
#if defined DX12 || defined DX11 || defined DX9
	#if NOTES
	m_viewMatrix.r[0] = { 1,0,0,0 };
	m_viewMatrix.r[1] = { 0,1,0,0 };
	m_viewMatrix.r[2] = { 0,0,1,0 };
	m_viewMatrix.r[3] = { m_positionX, m_positionY, m_positionZ, 1 };
	#endif

	m_viewMatrix = XMMatrixLookAtLH( position, lookAt, up );
#endif
}

// After the Render function has been called to create the view matrix we can provide the update view matrix to calling functions 
// using this GetViewMatrix function. The view matrix will be one of the three main matrices used in the HLSL vertex shader.
#if defined DX12 || defined DX11 || defined DX9
	void DXcameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
#endif

{
	viewMatrix = m_viewMatrix;
}

#if  DX_ENGINE_LEVEL >= 22 || defined USE_VIEW2D_SPRITES
void DXcameraClass::Use2DViewMatrix()
{
	m_viewmatrix2D = m_viewMatrix;
}
#endif


#endif

#endif
