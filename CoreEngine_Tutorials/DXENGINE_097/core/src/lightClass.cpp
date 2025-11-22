// --------------------------------------------------------------------------------------------
// Filename: lightClass.cpp
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
// PURPOSE: Paint the main window depending of engine state screen page.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"

#include "lightClass.h"
#include "TrigonometryMathClass.h"
#include "OSengine.h"
#include "ApplicationClass.h"
#include "DXinputclass.h"

LightClass::LightClass() 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

}

LightClass::~LightClass() {CLASSDELETE();}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
#if defined DX_ENGINE
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
#else
	m_ambientColor[0] = red;
	m_ambientColor[1] = green;
	m_ambientColor[2] = blue;
	m_ambientColor[3] = alpha;
#endif
}

void LightClass::SetDiffuseLightColor(float red, float green, float blue, float alpha)
{
#if defined DX_ENGINE
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
#else
	m_diffuseColor[0] = red;
	m_diffuseColor[1] = green;
	m_diffuseColor[2] = blue;
	m_diffuseColor[3] = alpha;
#endif
}

void LightClass::SetDirection(float x, float y, float z)
{
#if defined DX_ENGINE
	m_lightDirection = XMFLOAT4(x, y, z, 0); // Invert Direction to avoid "-" in shader code for every vertex
#else
	m_lightDirection[0] = x;
	m_lightDirection[1] = y;
	m_lightDirection[2] = z;
#endif
}

#if defined DX_ENGINE
void LightClass::GenerateViewMatrix(float SunX, float SunY, float SunZ)
{
	// Used on Shadow Map

	// Setup the vector that points upwards.
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR position = XMVectorSet(SunX, SunY, SunZ, 0.0f);

	// Create the view matrix from the three vectors.
	XMVECTOR lightDirection = XMVectorSet(m_lightDirection.x, m_lightDirection.y, m_lightDirection.z, m_lightDirection.w);
	m_viewMatrix = XMMatrixLookAtLH(position, lightDirection, up);
}
#endif

/*const*/ float* LightClass::GetDiffuseColor()
{
#if defined DX_ENGINE
	static float diffuseColor[4];		// Allocate static memory for result
	diffuseColor[0] = m_diffuseColor.x;
	diffuseColor[1] = m_diffuseColor.y;
	diffuseColor[2] = m_diffuseColor.z;
	diffuseColor[3] = m_diffuseColor.w;
	return diffuseColor;				// Convert to be used by OPEN GL
#else
	return m_diffuseColor;
#endif
}

/*const*/ float* LightClass::GetDirection()
{
	static float lightDirection[4];		// Allocate static memory for result
#if defined _XM_NO_INTRINSICS_
	lightDirection[0] = m_lightDirection.vector4_f32[0];
	lightDirection[1] = m_lightDirection.vector4_f32[1];
	lightDirection[2] = m_lightDirection.vector4_f32[2];
	lightDirection[3] = m_lightDirection.vector4_f32[3];
#else
	#if defined DX_ENGINE
	//NEED: _XM_SSE_INTRINSICS_ for fast code:
	lightDirection[0] = m_lightDirection.x;		//m128_f32[0];
	lightDirection[1] = m_lightDirection.y;		//m128_f32[1];
	lightDirection[2] = m_lightDirection.z;		//m128_f32[2];
	lightDirection[3] = m_lightDirection.w;		//m128_f32[3];
	#else
	lightDirection[0] = m_lightDirection[0];	//m128_f32[0];
	lightDirection[1] = m_lightDirection[1];	//m128_f32[1];
	lightDirection[2] = m_lightDirection[2];	//m128_f32[2];
	lightDirection[3] = m_lightDirection[3];	//m128_f32[3];
	#endif
#endif

	return &lightDirection[0];				// Convert to be used by OPEN GL
}

void LightClass::GenerateOrthoMatrix(float width, float height, float depthPlane, float nearPlane)
{
	// Create the orthographic matrix for the light.
	m_ligth_orthoMatrix = XMMatrixOrthographicLH((float)width, (float)height, nearPlane, depthPlane);  // 2D PROJECTION
}

#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}
#endif


const float* LightClass::GetAmbientColor()
{
	static float ambientColor[4];		// Allocate static memory for result
	ambientColor[0] = m_ambientColor.x;
	ambientColor[1] = m_ambientColor.y;
	ambientColor[2] = m_ambientColor.z;
	ambientColor[3] = m_ambientColor.w;

	return ambientColor;				// Convert to be used by OPEN GL
}



#if defined USE_LEVEL_36V2 //ON/OFF
// Minimal camera-centered light view/proj generator

void LightClass::GenerateCameraCenteredShadowMatrices(
	const XMFLOAT3& cameraPos,
	float orthoHalfSize,
	float nearZ,
	float farZ,
	float lightDistanceMultiplier)
{
	//----------------------------------------------------------
	// 1. Raise camera to avoid terrain clipping
	//----------------------------------------------------------
	float terrainHeight = mainTerrain->getTerrainHeight(TERRAIN_ID,
		cameraPos.x,
		cameraPos.z);

	// Always keep shadow camera at least +10 above terrain
	float safeY = terrainHeight + 10.0f;

	// Apply only if camera is lower than terrain+10
	float camY = max(cameraPos.y, safeY);

	XMFLOAT3 camRaised = cameraPos;
	camRaised.y = camY;

	//----------------------------------------------------------
	// 2. Calculate sun direction
	//----------------------------------------------------------
	XMVECTOR sunDir = XMVector3Normalize(
		XMVectorSet(-m_lightDirection.x, -m_lightDirection.y,
			-m_lightDirection.z, 0.0f));

	XMVECTOR centerWS = XMVectorSet(camRaised.x, camRaised.y, camRaised.z, 1.0f);

	//----------------------------------------------------------
	// 3. Offset the light backwards along the sun direction
	//----------------------------------------------------------
	float lightDist = orthoHalfSize * lightDistanceMultiplier;

	XMVECTOR lightEyeWS =
		XMVectorSubtract(centerWS, XMVectorScale(sunDir, lightDist));

	//----------------------------------------------------------
	// 4. Build view matrix
	//----------------------------------------------------------
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	m_viewMatrix = XMMatrixLookAtLH(lightEyeWS, centerWS, up);

	//----------------------------------------------------------
	// 5. Build orthographic projection
	//----------------------------------------------------------
	float L = -orthoHalfSize;
	float R = orthoHalfSize;
	float B = -orthoHalfSize;
	float T = orthoHalfSize;

	m_ligth_orthoMatrix =
		XMMatrixOrthographicOffCenterLH(L, R, B, T, nearZ, farZ);
}



// V1-compatible shadow region for small scenes
//#if defined USE_SMALL_SHADOWS   // Levels 36–39

void LightClass::GenerateSmallSceneShadowMatrices(float sunAzimuthDeg, float sunElevationDeg, float useLightSize)
{
	//
	// --- 1. Compute sun position EXACTLY like original Level 36 ---
	//
	float LightX = useLightSize * FAST_sin(sunAzimuthDeg);
	float LightZ = useLightSize * FAST_cos(sunAzimuthDeg);
	float LightY = useLightSize * FAST_sin(sunElevationDeg);

	XMVECTOR lightEye = XMVectorSet(LightX, LightY, LightZ, 0.0f);

	//
	// --- 2. ALWAYS LOOK AT ORIGIN like original Level 36 ---
	//
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_viewMatrix = XMMatrixLookAtLH(lightEye, lookAt, up);

	//
	// --- 3. ORIGINAL ORTHOGRAPHIC PROJECTION ---
	//
	float width = 15.0f;
	float height = 15.0f;
	float nearZ = 0.1f;
	float farZ = 20.0f;

	m_ligth_orthoMatrix = XMMatrixOrthographicLH(width, height, nearZ, farZ);
}

#endif




