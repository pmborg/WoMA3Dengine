// --------------------------------------------------------------------------------------------
// Filename: virtualCameraClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// PURPOSE:
//
// ********************************************************************************************
#pragma once

#include "main.h"

typedef struct  
{ 
	float x, y, z;
} VectorType;

class VirtualCameraClass
{
public:
	virtual void SetPosition(float, float, float)=0;
	virtual void SetRotation(float, float, float)=0;
	virtual void Render()=0;

private:
};