// --------------------------------------------------------------------------------------------
// Filename: lightClass.h
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525256;

#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

//Now we will look at the new light class which is very simple. Its purpose is only to maintain the direction and color of lights.

//////////////
// INCLUDES //
//////////////
#include "main.h"

#pragma warning( disable : 4324 ) // 4324: '': structure was padded due to __declspec(align())
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition

#if defined DX11
	#include <d3d11.h>
	#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
	#include <d3d11.h>		//#include "DX11Class.h"

	#if D3D11_SPEC_DATE_YEAR == 2009 // #if defined USE_OLD_DirectX_SDK_June2010
		#pragma warning( disable : 4324 ) // 4324: '': structure was padded due to __declspec(align())
		#pragma warning( disable : 4838 )
		#include <xnamath.h>	//#include <d3dx10math.h>
	#endif
#endif

#if defined DX_ENGINE
#include <DirectXMath.h>// Use Math
#if defined DX12
	// DX12 includes
	#include <dxgi1_4.h>	// Always 1st!	(Select Driver)
	#include <d3d12.h>		// DX12			(Select Device)
	#include <D3Dcompiler.h>// Use Compiler
	//#include "d3dx12.h"
#endif
	using namespace DirectX;
#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	UINT WomaIntegrityCheck = 1234525256;
	LightClass();
	~LightClass();

	//CH10
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseLightColor(float, float, float, float);
	void SetDirection(float, float, float);

#if defined DX_ENGINE
	void GenerateViewMatrix(float SunX, float SunY, float SunZ);
#endif

	void GenerateOrthoMatrix(float width, float height, float depthPlane, float nearPlane);

#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);
#endif

	float* GetDirection();
	float* GetDiffuseColor();
    const float* GetAmbientColor();

//	-------------------------------------------------------------------------------------------
//public:
//	-------------------------------------------------------------------------------------------
#if defined DX_ENGINE 
	XMFLOAT4 m_lightDirection = {};
	XMFLOAT4 m_ambientColor = {};
	XMFLOAT4 m_diffuseColor = {};

	XMMATRIX m_viewMatrix = {};
#else
	float m_lightDirection[4] = {};
	float m_ambientColor[4] = {};
	float m_diffuseColor[4] = {};

	float m_viewMatrix[4] = {};
#endif

	XMMATRIX m_ligth_orthoMatrix = {};

//	-------------------------------------------------------------------------------------------
private:
//	-------------------------------------------------------------------------------------------
#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

	XMFLOAT4 m_specularColor = {};
	float m_specularPower=0;
#endif
};

#endif
