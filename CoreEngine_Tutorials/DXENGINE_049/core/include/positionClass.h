// --------------------------------------------------------------------------------------------
// Filename: positionClass.h
// ----------------------------------------------------------------------------------------------
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#if defined USE_DIRECT_INPUT || defined INTRO_DEMO
#include "main.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	UINT WomaIntegrityCheck = 1234525217;
	PositionClass(UINT id);
	~PositionClass();

	//The PositionClass has some helper functions to set and retrieve the position and rotation of the viewer/camera.
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	//SetFrameTime is used to keep the viewer/camera in sync with the speed of the application.
	void SetFrameTime(double);

	//The movement functions are called to move the viewer/camera based on the user input.
	void MoveForward(bool, bool ctrl, bool water = false);
    void StrafeLeft(bool keydown, bool ctrl, bool water);
    void StrafeRight(bool keydown, bool ctrl, bool water);
	void MoveBackward(bool, bool ctrl, bool water = false);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void TurnLeft(bool, bool ctrl);
	void TurnRight(bool, bool ctrl);
	void LookUpward(bool);
	void LookDownward(bool);

public:
	float m_positionX=0, m_positionY = 0, m_positionZ = 0;
	float m_rotationX = 0, m_rotationY = 0, m_rotationZ = 0;
	float m_forwardSpeed = 0, m_backwardSpeed = 0;
    float m_strafeLeftSpeed = 0, m_strafeRightSpeed = 0;

private:
	float m_frameTime = 0;
	float m_upwardSpeed = 0, m_downwardSpeed = 0;
	float m_leftTurnSpeed = 0, m_rightTurnSpeed = 0;
	float m_lookUpSpeed = 0, m_lookDownSpeed = 0;
};
#endif

#endif
