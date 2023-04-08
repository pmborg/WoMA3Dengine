// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: positionClass.cpp
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
// PURPOSE: Define the position of a player.
//			Process the Dynamic movement: Newton Laws of the player.
// --------------------------------------------------------------------------------------------
#include "main.h"

#include "OSengine.h"

#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include "positionClass.h"
#include "TrigonometryMathClass.h" //sim, cos table
#include <mmsystem.h> //timeGetTime


//The class constructor initializes all the position, rotation, frame time, and speed variables to zero.
PositionClass::PositionClass(UINT id)
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567829;

	m_positionX = SystemHandle->AppSettings->INIT_CAMX;
	m_positionY = SystemHandle->AppSettings->INIT_CAMY;
	m_positionZ = SystemHandle->AppSettings->INIT_CAMZ;
	
	m_rotationX = SystemHandle->AppSettings->INIT_ROTX;
	m_rotationY = SystemHandle->AppSettings->INIT_ROTY;
	m_rotationZ = SystemHandle->AppSettings->INIT_ROTZ;

	m_frameTime = 0.0f;

	m_forwardSpeed   = 0.0f;
	m_backwardSpeed  = 0.0f;
	m_upwardSpeed    = 0.0f;
	m_downwardSpeed  = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed    = 0.0f;
	m_lookDownSpeed  = 0.0f;
}

PositionClass::~PositionClass(){/*if (WOMA::logManager)*/ CLASSDELETE();}

//The SetPosition and SetRotation functions are used for setting the position and rotation of the viewer/camera. 
//These functions are generally used to initialize the position of the camera other than at the origin. 
//In this tutorial the camera will be set slightly back from the grid and in the center of it.
void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

//The GetPosition and GetRotation functions return the current position and rotation of the camera location. 
//In this tutorial these functions are called to provide the location and rotation of the camera for display purposes. 
//We will draw the position/rotation as text strings on the left side of the screen. This is very useful for debugging.
void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
}


void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
}

//The SetFrameTime function needs to be called each frame. It stores the current frame time inside a private member variable and 
//is then used by the movement calculation functions. This way regardless of the speed that the application is running at the 
//movement and rotation speed remains the same. If this wasn't done then the movement rate would speed up or down with the frame rate.
void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
}

//The following eight movement functions all work nearly the same. All eight functions are called each frame. 
//The keydown input variable to each function indicates if the user is pressing the forward key, the backward key, and so forth. 
//If they are pressing the key then each frame the speed will accelerate until it hits a maximum. 
//This way the camera speeds up similar to the acceleration in a vehicle creating the effect of smooth movement and high responsiveness. 
//Likewise if the user releases the key and the keydown variable is false it will then smoothly slow down each frame until 
//the speed hits zero. The speed is calculated against the frame time to ensure the movement speed remains the same regardless of the 
//frame rate. Each function then uses some basic math to calculate the new position of the viewer/camera.

//This function calculates the forward speed and movement of the viewer/camera.
void PositionClass::MoveForward(bool keydown, bool ctrl, bool mouseWhell, bool water)
{
	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown||mouseWhell)
	{
		m_forwardSpeed += m_frameTime * 0.0005f;
		#if defined _DEBUG
		float maxSpeed = ctrl ? 0.1f : 0.004f; // MAX SPEED
		#else
		float maxSpeed = ctrl ? 0.04f : 0.004f; // MAX SPEED
		#endif
		if (water)
			maxSpeed/=3;

		if (mouseWhell)
			m_forwardSpeed = ctrl ? 0.1f : 0.01f;
		else
		if(m_forwardSpeed > (m_frameTime * maxSpeed))
			m_forwardSpeed = m_frameTime * maxSpeed;

	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.0004f;

		if(m_forwardSpeed < 0.0f)
			m_forwardSpeed = 0.0f;
	}

	m_positionX += FAST_sin(m_rotationY) * m_forwardSpeed;
	m_positionZ += FAST_cos(m_rotationY) * m_forwardSpeed;
}

//This function calculates the backward speed and movement of the viewer/camera.
void PositionClass::MoveBackward(bool keydown, bool ctrl, bool mouseWhell, bool water)
{
	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown||mouseWhell)
	{
		m_backwardSpeed += m_frameTime * 0.0005f;
		float maxSpeed = ctrl ? 0.04f : 0.004f; // MAX SPEED

		maxSpeed /= 8; // MAX SPEED for INTRO_DEMO

		if (water)
			maxSpeed/=3;

		if (mouseWhell)
			m_backwardSpeed = ctrl ? 0.1f : 0.01f;
		else
		if(m_backwardSpeed > (m_frameTime * maxSpeed))
			m_backwardSpeed = m_frameTime * maxSpeed;
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;
		
		if(m_backwardSpeed < 0.0f)
			m_backwardSpeed = 0.0f;
	}

	m_positionX -= FAST_sin(m_rotationY) * m_backwardSpeed;
	m_positionZ -= FAST_cos(m_rotationY) * m_backwardSpeed;
}

//This function calculates the upward speed and movement of the viewer/camera.

void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_upwardSpeed += m_frameTime * 0.003f;

		if(m_upwardSpeed > (m_frameTime * 0.01f))
		{
			m_upwardSpeed = m_frameTime * 0.01f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.002f;

		if(m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY += m_upwardSpeed;
}

//This function calculates the downward speed and movement of the viewer/camera.

void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_downwardSpeed += m_frameTime * 0.003f;

		if(m_downwardSpeed > (m_frameTime * 0.01f))
		{
			m_downwardSpeed = m_frameTime * 0.01f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.002f;

		if(m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY -= m_downwardSpeed;
}

//This function calculates the left turn speed and rotation of the viewer/camera.

void PositionClass::TurnLeft(bool keydown, bool ctrl)
{

	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		float maxSpeed = ctrl ? 0.1f : 0.05f;
		m_leftTurnSpeed += m_frameTime * 0.02f;

		if(m_leftTurnSpeed > (m_frameTime * maxSpeed))
		{
			m_leftTurnSpeed = m_frameTime * maxSpeed;
		}
}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	m_rotationY -= m_leftTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if(m_rotationY < 0.0f)
		m_rotationY += 360.0f;
}

//This function calculates the right turn speed and rotation of the viewer/camera.

void PositionClass::TurnRight(bool keydown, bool ctrl)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		float maxSpeed = ctrl ? 0.1f : 0.05f;
		m_rightTurnSpeed += m_frameTime * 0.02f;

		if(m_rightTurnSpeed > (m_frameTime * maxSpeed))
		{
			m_rightTurnSpeed = m_frameTime * maxSpeed;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if(m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY += m_rightTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if(m_rotationY > 360.0f)
		m_rotationY -= 360.0f;
}

//This function calculates the upward turn speed and rotation of the viewer/camera.

void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.02f;

		if(m_lookUpSpeed > (m_frameTime * 0.05f))
		{
			m_lookUpSpeed = m_frameTime * 0.05f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime* 0.005f;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep this rotation maximum: to avoid look back
	if(m_rotationX > 89.0f)
		m_rotationX = 89.0f;
}

//This function calculates the downward turn speed and rotation of the viewer/camera.

void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.02f;

		if(m_lookDownSpeed > (m_frameTime * 0.05f))
		{
			m_lookDownSpeed = m_frameTime * 0.05f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* 0.005f;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep this rotation maximum: to avoid a ugly sky on sphere
	if(m_rotationX < -89.0f)
		m_rotationX = -89.0f;
}

