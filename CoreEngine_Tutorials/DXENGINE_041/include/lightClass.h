// --------------------------------------------------------------------------------------------
// Filename: lightClass.h
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------

#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

//Now we will look at the new light class which is very simple. Its purpose is only to maintain the direction and color of lights.

//////////////
// INCLUDES //
//////////////
#include "main.h"
#pragma warning( disable : 4324 ) // 4324: '': structure was padded due to __declspec(align())
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition

#if defined DX9sdk
	#include "DX9Class.h" // Driver
#endif

#include <d3d11.h>
#include <DirectXMath.h>// Use Math
using namespace DirectX;

	#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
	#include <d3d11.h>		//#include "DX11Class.h"

	#if D3D11_SPEC_DATE_YEAR == 2009 // #if defined USE_OLD_DirectX_SDK_June2010
		#pragma warning( disable : 4324 ) // 4324: '': structure was padded due to __declspec(align())
		#pragma warning( disable : 4838 )
		#include <xnamath.h>	//#include <d3dx10math.h>
	#else
		//#include <DirectXMath.h>
		using namespace DirectX;
	#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	LightClass();
	~LightClass();

	//CH10
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	void GenerateOrthoMatrix(float width, float height, float depthPlane, float nearPlane);
	void GenerateViewMatrix(float SunX, float SunY, float SunZ);

#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);
#endif

    const float* GetAmbientColor();
	const float* GetDiffuseColor();
    const float* GetDirection();

//	-------------------------------------------------------------------------------------------
//public:
//	-------------------------------------------------------------------------------------------
	XMFLOAT4 m_lightDirection;
	XMFLOAT4 m_ambientColor;
    XMFLOAT4 m_diffuseColor;

#if defined DX9sdk
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
#endif

	XMMATRIX m_viewMatrix;
	XMMATRIX m_orthoMatrix;

//	-------------------------------------------------------------------------------------------
private:
//	-------------------------------------------------------------------------------------------
#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

	XMFLOAT4 m_specularColor;
	float m_specularPower;
#endif
};

#endif
