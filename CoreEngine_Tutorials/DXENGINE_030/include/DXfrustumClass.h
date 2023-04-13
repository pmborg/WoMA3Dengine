// --------------------------------------------------------------------------------------------
// Filename: DXfrustumClass.h
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
//WomaIntegrityCheck = 1234567830;

#pragma once

#include "main.h"
#include "DXdrivers.h"

#if D3D11_SPEC_DATE_YEAR > 2009 && ENGINE_LEVEL < 60
//error "You should Use: C:\\WoMAengine2014\\Microsoft_DirectX_SDK_June_2010\\Include"
//C:\WoMAengine2014\Microsoft_DirectX_SDK_June_2010\Include
//C:\WoMAengine2014\Microsoft_DirectX_SDK_June_2010\Lib\x64
#endif

// Class name: DXfrustumClass
class DXfrustumClass
{
public:
	DXfrustumClass();
	~DXfrustumClass();

	void ConstructFrustum(float, XMMATRIX* projectionMatrix, XMMATRIX* g_viewMatrix);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
    float SphereDistance(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	XMVECTOR m_planes[6];

public:
    float m_screenDepth;
};

