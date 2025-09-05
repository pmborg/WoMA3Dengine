// ----------------------------------------------------------------------------------------------
// Filename: DXcameraClass.h
// --------------------------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#pragma once

#include "platform.h"
#if defined DX_ENGINE
#include <d3d11.h>

#if defined DX11 || defined DX9
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
class DXcameraClass : public VirtualCameraClass
{
public:
	UINT WomaIntegrityCheck = 1234525217;
	DXcameraClass(UINT camera_type = 0);
	~DXcameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();

	void CalculateViewMatrix();
#if defined USE_3RD_PERSON_CAMERA
    XMVECTOR camPosition;
    XMVECTOR camTarget;
    XMVECTOR camUp;
    XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    void CalculateViewMatrix_3rd_PersonCamera(float camYaw = 0.0f, float camPitch = 0.0f, bool sky = false);
#endif
    void GetViewMatrix(XMMATRIX&);

	void Use2DViewMatrix();

	//VARS:
	// --------------------------------------------------------------------------------------------
	float m_positionX=0, m_positionY = 0, m_positionZ = 0;
	float m_rotationX = 0, m_rotationY = 0, m_rotationZ = 0;
	float offsetDeg = 0;

	XMMATRIX m_viewMatrix;

	XMMATRIX m_viewmatrix2D;

private:
#if defined DX12 || defined DX11 || defined DX9
	XMVECTOR /*XMFLOAT3*/ lookAt;
	XMVECTOR /*XMFLOAT3*/ up;
#endif

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	UINT m_camera_type;
#endif

};


#endif
