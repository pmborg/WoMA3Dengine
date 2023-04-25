// --------------------------------------------------------------------------------------------
// Filename: positionClass.h
// ----------------------------------------------------------------------------------------------
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------

#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	PositionClass(UINT id);
	~PositionClass();

	//The PositionClass has some helper functions to set and retrieve the position and rotation of the viewer/camera.
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	//SetFrameTime is used to keep the viewer/camera in sync with the speed of the application.
	void SetFrameTime(float);
	void GetRotation(float&);

	//The movement functions are called to move the viewer/camera based on the user input.
	void MoveForward(bool, bool ctrl, bool mouseWhell, bool water = false);
	void MoveBackward(bool, bool ctrl, bool mouseWhell, bool water = false);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void TurnLeft(bool, bool ctrl);
	void TurnRight(bool, bool ctrl);
	void LookUpward(bool);
	void LookDownward(bool);

public:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_forwardSpeed, m_backwardSpeed;

private:
	float m_frameTime;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;
};

#endif
