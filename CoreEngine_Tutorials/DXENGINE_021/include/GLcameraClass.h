// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: GLcameraClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
#pragma once

//////////////
// INCLUDES //
//////////////
#include "virtualCameraClass.h"
//#include <math.h>
#include "GLmathClass.h"
////////////////////////////////////////////////////////////////////////////////
// Class name: GLcameraClass
////////////////////////////////////////////////////////////////////////////////
class GLcameraClass : public VirtualCameraClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	GLcameraClass();
	~GLcameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void Render();
	
	// ------------------------------------------------------------------------
	void GetViewMatrix(float*);

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	mat4 m_viewMatrix; // [16] ;

private:
	void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(VectorType&, float*);
	void BuildViewMatrix(VectorType, VectorType, VectorType);
};