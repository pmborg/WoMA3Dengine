// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: DXcameraClass.h
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
// ----------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"

#if defined DX9sdk
	//////////////
	// INCLUDES //
	//////////////
	#include <d3d9.h>
	#include <d3dx9.h>
	#pragma warning( disable : 4324 )
	#include <DirectXMath.h> //#include <xnamath.h>

	#include "womadriverclass.h"
#endif

	// -------------------------------------------------------------------------------------------
	// Use OLD xnamath from DirectX SDK June2010 or Windows Kit 8?
	// -------------------------------------------------------------------------------------------
	#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition
	#include <d3d11.h>

#if D3D11_SPEC_DATE_YEAR == 2009		// Use the OLD DirectX_SDK_June2010 ?
	#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
	#pragma warning( disable : 4838 )
	#include <DirectXMath.h> //#include <xnamath.h>				// #include <d3dx10math.h>
#else
	#include <DirectXMath.h>			// Use the NEW DirectX11
	using namespace DirectX;
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
// Windows includes
#include <Windows.h>

	// STL includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <conio.h>
#include <vector>
#include <string>
#include <array>

	// DX12 includes
#include <dxgi1_4.h>	// Always 1st!	(Select Driver)
#include <d3d12.h>		// DX12			(Select Device)
#include <D3Dcompiler.h>// Use Comiler
#include <DirectXMath.h>// Use Math
#include <wrl.h>
#endif



//////////////
// INCLUDES //
//////////////
#include "main.h"
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#pragma warning( disable : 4324 ) // 4324: '': structure was padded due to __declspec(align())

#include  "virtualCameraClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: DXcameraClass
////////////////////////////////////////////////////////////////////////////////
namespace DirectX
{

class DXcameraClass : public VirtualCameraClass
{
public:
	UINT WomaIntegrityCheck = 1234567890;
	DXcameraClass(UINT camera_type = 0);
	~DXcameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

    /*D3DXVECTOR3*/ XMFLOAT3 GetPosition();
    /*D3DXVECTOR3*/ XMFLOAT3 GetRotation();

	void Render();

#if defined DX9sdk
	void GetViewMatrix(D3DXMATRIX&);
#else
	void GetViewMatrix(XMMATRIX&);
#endif

	void Set2DViewMatrix();

	//VARS:
	// --------------------------------------------------------------------------------------------
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

  #if defined DX9sdk
	D3DXMATRIX m_viewMatrix;
  #else
	XMMATRIX m_viewMatrix;
  #endif

  #ifndef DX9sdk
	XMMATRIX m_viewmatrix2D;
  #else
	D3DXMATRIX m_viewmatrix2D;
  #endif

private:
	XMVECTOR /*XMFLOAT3*/ lookAt;
	XMVECTOR /*XMFLOAT3*/ up;
#if defined DX9sdk
	D3DXVECTOR3 lookAt9;
	D3DXVECTOR3 up9;
#endif

};

}