// --------------------------------------------------------------------------------------------
// Filename: GLcameraClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525256;

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
	UINT WomaIntegrityCheck = 1234525256;
	GLcameraClass();
	~GLcameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void CalculateViewMatrix();
	
	// ------------------------------------------------------------------------
	void GetViewMatrix(float*);

	#if  DX_ENGINE_LEVEL >= 22 || defined USE_VIEW2D_SPRITES
	void Use2DViewMatrix();
	#endif

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	mat4 m_viewMatrix; // [16] ;

	#if  DX_ENGINE_LEVEL >= 22 || defined USE_VIEW2D_SPRITES
	mat4 m_viewmatrix2D; // [16] ;
	#endif

private:
	void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(VectorType&, float*);
	void BuildViewMatrix(VectorType, VectorType, VectorType);
};