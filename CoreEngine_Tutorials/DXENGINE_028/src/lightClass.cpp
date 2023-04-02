// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: lightClass.cpp
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
// PURPOSE: Paint the main window depending of engine state screen page.
// --------------------------------------------------------------------------------------------

#include "platform.h"

#include "lightclass.h"

#ifdef DX9sdk
	#include "SystemPlatform.h"		// To define OS [SystemHandle] Pointer (System Class) & define WomaSYSTEM for: WINDOWS, LINUX & ANDROID
	#include "winsystemclass.h"	// SystemHandle
#endif

LightClass::LightClass() 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567891;

#ifdef DX9sdk
	ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
	light.Range = 1000.0f;
#endif
}

LightClass::~LightClass() {CLASSDELETE();}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
#ifdef DX9sdk
	 alpha = 0;
	 DWORD color = D3DCOLOR_ARGB((BYTE)(alpha*255.0f), (BYTE)(red*255.0f), (BYTE)(green*255.0f), (BYTE)(blue*255.0f));
	 color = 0xffffffff ;
	((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetRenderState(D3DRS_AMBIENT,color);
#else
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
#endif
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
#ifdef DX9sdk
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = red;
	light.Diffuse.g = green;
	light.Diffuse.b = blue;
#else
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
#endif
}

void LightClass::SetDirection(float x, float y, float z)
{
    m_lightDirection = XMFLOAT4(x, y, z, 0); // Invert Direction to avoid "-" in shader code for every vertex

#ifdef DX9sdk
	D3DXVECTOR3 vecDir = D3DXVECTOR3(m_lightDirection.x, m_lightDirection.y, m_lightDirection.z);
	D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction, &vecDir );
#endif
}


const float* LightClass::GetAmbientColor()
{
	static float ambientColor[4];		// Alocate static memory for result
	ambientColor[0] = m_ambientColor.x;
	ambientColor[1] = m_ambientColor.y;
	ambientColor[2] = m_ambientColor.z;
	ambientColor[3] = m_ambientColor.w;

	return ambientColor;				// Convert to be used by OPEN GL
}

const float* LightClass::GetDiffuseColor()
{
	static float diffuseColor[4];		// Alocate static memory for result
	diffuseColor[0] = m_diffuseColor.x; 
	diffuseColor[1] = m_diffuseColor.y; 
	diffuseColor[2] = m_diffuseColor.z; 
	diffuseColor[3] = m_diffuseColor.w; 

	return diffuseColor;				// Convert to be used by OPEN GL
}

const float* LightClass::GetDirection()
{
	static float lightDirection[4];		// Alocate static memory for result

	#if defined _XM_NO_INTRINSICS_
		lightDirection[0]  = m_lightDirection.vector4_f32[0];
		lightDirection[1]  = m_lightDirection.vector4_f32[1];
		lightDirection[2]  = m_lightDirection.vector4_f32[2];
		lightDirection[3]  = m_lightDirection.vector4_f32[3];
	#else
		//NEED: _XM_SSE_INTRINSICS_ for fast code:
		lightDirection[0] = m_lightDirection.x; //m128_f32[0];
		lightDirection[1] = m_lightDirection.y;// m128_f32[1];
		lightDirection[2] = m_lightDirection.z;//m128_f32[2];
		lightDirection[3] = m_lightDirection.w;// m128_f32[3];
	#endif

	return lightDirection;				// Convert to be used by OPEN GL
}

