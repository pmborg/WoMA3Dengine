// --------------------------------------------------------------------------------------------
// Filename: lightClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
// NOTE: Used by WINDOWS, LINUX and ANDROID
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "platform.h"

#include "lightClass.h"
#include "TrigonometryMathClass.h"
#include "OSengine.h"
#include "ApplicationClass.h"
#if defined WINDOWS_PLATFORM && defined USE_LEVEL_36V2
#include "DXinputclass.h"
#endif

LightClass::LightClass() 
{
	CLASSLOADER();


}

LightClass::~LightClass() {CLASSDELETE();}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{

	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDiffuseLightColor(float red, float green, float blue, float alpha)
{

	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDirection(float x, float y, float z)
{

	m_lightDirection = XMFLOAT4(x, y, z, 0); // Invert Direction to avoid "-" in shader code for every vertex
}

#if defined USE_SHADOW_MAP
void LightClass::GenerateViewMatrix(float SunX, float SunY, float SunZ)
{
#if defined WINDOWS_PLATFORM

	// DirectXMath (Windows: DX11/DX12/OpenGL/Vulkan)
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR position = XMVectorSet(SunX, SunY, SunZ, 0.0f);
	XMVECTOR lightDirection = XMVectorSet(
		m_lightDirection.x,
		m_lightDirection.y,
		m_lightDirection.z,
		m_lightDirection.w);

	m_viewMatrix = XMMatrixLookAtLH(position, lightDirection, up);

#else

	// GLmath (Linux / Android)
	VectorType position, lookAt, up;
	VectorType zAxis, xAxis, yAxis;
	float length, result1, result2, result3;

	position.x = SunX;
	position.y = SunY;
	position.z = SunZ;

	// Keep the same behaviour as XMMatrixLookAtLH:
	lookAt.x = m_lightDirection.x;
	lookAt.y = m_lightDirection.y;
	lookAt.z = m_lightDirection.z;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// zAxis = normalize(lookAt - position)
	zAxis.x = lookAt.x - position.x;
	zAxis.y = lookAt.y - position.y;
	zAxis.z = lookAt.z - position.z;

	length = sqrtf(zAxis.x * zAxis.x + zAxis.y * zAxis.y + zAxis.z * zAxis.z);
	zAxis.x /= length;
	zAxis.y /= length;
	zAxis.z /= length;

	// xAxis = normalize(cross(up, zAxis))
	xAxis.x = up.y * zAxis.z - up.z * zAxis.y;
	xAxis.y = up.z * zAxis.x - up.x * zAxis.z;
	xAxis.z = up.x * zAxis.y - up.y * zAxis.x;

	length = sqrtf(xAxis.x * xAxis.x + xAxis.y * xAxis.y + xAxis.z * xAxis.z);
	xAxis.x /= length;
	xAxis.y /= length;
	xAxis.z /= length;

	// yAxis = cross(zAxis, xAxis)
	yAxis.x = zAxis.y * xAxis.z - zAxis.z * xAxis.y;
	yAxis.y = zAxis.z * xAxis.x - zAxis.x * xAxis.z;
	yAxis.z = zAxis.x * xAxis.y - zAxis.y * xAxis.x;

	result1 = -(xAxis.x * position.x + xAxis.y * position.y + xAxis.z * position.z);
	result2 = -(yAxis.x * position.x + yAxis.y * position.y + yAxis.z * position.z);
	result3 = -(zAxis.x * position.x + zAxis.y * position.y + zAxis.z * position.z);

	m_viewMatrix.m[0] = xAxis.x;
	m_viewMatrix.m[1] = yAxis.x;
	m_viewMatrix.m[2] = zAxis.x;
	m_viewMatrix.m[3] = 0.0f;

	m_viewMatrix.m[4] = xAxis.y;
	m_viewMatrix.m[5] = yAxis.y;
	m_viewMatrix.m[6] = zAxis.y;
	m_viewMatrix.m[7] = 0.0f;

	m_viewMatrix.m[8] = xAxis.z;
	m_viewMatrix.m[9] = yAxis.z;
	m_viewMatrix.m[10] = zAxis.z;
	m_viewMatrix.m[11] = 0.0f;

	m_viewMatrix.m[12] = result1;
	m_viewMatrix.m[13] = result2;
	m_viewMatrix.m[14] = result3;
	m_viewMatrix.m[15] = 1.0f;

#endif
}
#endif

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT4 LightClass::GetDirection()
{
	return m_lightDirection;				// Convert to be used by OPEN GL
}

void LightClass::GenerateOrthoMatrix(float width, float height, float depthPlane, float nearPlane)
{
#if defined WINDOWS_PLATFORM

	// DirectXMath (Windows)
	m_ligth_orthoMatrix = XMMatrixOrthographicLH(width, height, nearPlane, depthPlane);

#else

	// GLmath (Linux / Android) - same as XMMatrixOrthographicLH
	ZeroMemory(&m_ligth_orthoMatrix, sizeof(mat4));

	m_ligth_orthoMatrix.m[0] = 2.0f / width;
	m_ligth_orthoMatrix.m[5] = 2.0f / height;
	m_ligth_orthoMatrix.m[10] = 1.0f / (depthPlane - nearPlane);
	m_ligth_orthoMatrix.m[14] = nearPlane / (nearPlane - depthPlane);
	m_ligth_orthoMatrix.m[15] = 1.0f;

#endif
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


XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;				// Convert to be used by OPEN GL
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
	float terrainHeight = mainTerrain->getTerrainHeight(TERRAIN_ID, cameraPos.x, cameraPos.z);
	
	// Always keep shadow camera at least +10 above terrain
	float safeY = terrainHeight + 10.0f;

	// Apply only if camera is lower than terrain+10
	float camY = max(cameraPos.y, safeY);

	XMFLOAT3 camRaised = cameraPos;
	camRaised.y = camY;

	//----------------------------------------------------------
	// 2. Calculate sun direction
	//----------------------------------------------------------
	XMVECTOR sunDir = XMVector3Normalize(XMVectorSet(-m_lightDirection.x, -m_lightDirection.y, -m_lightDirection.z, 0.0f));

	XMVECTOR centerWS = XMVectorSet(camRaised.x, camRaised.y, camRaised.z, 1.0f);

	//----------------------------------------------------------
	// 3. Offset the light backwards along the sun direction
	//----------------------------------------------------------
	float lightDist = orthoHalfSize * lightDistanceMultiplier;

	XMVECTOR lightEyeWS = XMVectorSubtract(centerWS, XMVectorScale(sunDir, lightDist));

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

	m_ligth_orthoMatrix = XMMatrixOrthographicOffCenterLH(L, R, B, T, nearZ, farZ);
}



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




