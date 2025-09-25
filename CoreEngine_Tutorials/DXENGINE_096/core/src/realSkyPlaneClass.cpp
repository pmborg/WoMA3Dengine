// --------------------------------------------------------------------------------------------
// Filename: realSkyPlaneClass.cpp
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
// PURPOSE: Adopt the base idea of https://rastertek.com/tertut12.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#if defined USE_CURVED_REAL_SKY_PLANE
#include "realSkyPlaneClass.h"
#include "vertexTypes.h"
#include "mem_leak.h"
#include "DXmodelClass.h"

#if defined USE_CURVED_REAL_SKY_PLANE && DX_ENGINE_LEVEL >= 96
RealSkyPlaneClass realSkyPlaneClass;
#endif

RealSkyPlaneClass::RealSkyPlaneClass()
{
	CLASSLOADER();
}

RealSkyPlaneClass::~RealSkyPlaneClass()
{
	Shutdown();
	CLASSDELETE();
}

void RealSkyPlaneClass::Shutdown()
{
#if (defined DX_ENGINE)
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELDX(SystemHandle->m_Application->model_skyPlane);
	}
#endif
#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(SystemHandle->m_Application->model_skyPlane);
	}
#endif
}

bool RealSkyPlaneClass::Initialize(void* ctx, void* Driver)
{
	// Set the sky plane parameters.
	skyPlaneResolution = 50;
	skyPlaneWidth = 10.0f;
	skyPlaneTop = 0.5f;
	skyPlaneBottom = -2.0f;
	textureRepeat = 2;

	// Initialize the translation to zero.
	m_translation = 0.0f;

	// Create the sky plane.
	IF_NOT_RETURN_FALSE(InitializeSkyPlane(ctx, Driver, skyPlaneResolution, skyPlaneWidth, skyPlaneTop, skyPlaneBottom, textureRepeat));

	return false;
}

struct SkyPlaneType
{
	float x, y, z;
	float tu, tv;
};

bool RealSkyPlaneClass::InitializeSkyPlane(void* ctx, void* Driver, int skyPlaneResolution, float skyPlaneWidth, float skyPlaneTop, float skyPlaneBottom, int textureRepeat)
{
	float quadSize, radius, constant, textureDelta;
	int i, j, index;
	float positionX, positionY, positionZ, tu, tv;

	DX11Class* m_driver11 = (DirectX::DX11Class*)Driver;
	ID3D11DeviceContext* pContext11 = (ID3D11DeviceContext*)ctx;

	// Create the array to hold the sky plane coordinates.
	SkyPlaneType* m_skyPlane = NEW SkyPlaneType[(skyPlaneResolution + 1) * (skyPlaneResolution + 1)];
	if (!m_skyPlane)return false;

	// Determine the size of each quad on the sky plane.
	quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// Calculate the radius of the sky plane based on the width.
	radius = skyPlaneWidth / 2.0f;

	// Calculate the height constant to increment by.
	constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// Calculate the texture coordinate increment value.
	textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	
	// Loop through the sky plane and build the coordinates based on the increment values given.
	for (j = 0; j <= skyPlaneResolution; j++)
	{
		for (i = 0; i <= skyPlaneResolution; i++)
		{
			// Calculate the vertex coordinates.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// Calculate the texture coordinates.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// Calculate the index into the sky plane array to add this coordinate.
			index = j * (skyPlaneResolution + 1) + i;

			// Add the coordinates to the sky plane array.
			m_skyPlane[index].x = positionX;
			m_skyPlane[index].y = positionY;
			m_skyPlane[index].z = positionZ;
			m_skyPlane[index].tu = tu;
			m_skyPlane[index].tv = tv;
		}
	}

	// Initialize the index into the vertex array.
	index = 0;

	ModelTextureVertexType v;
	int index1, index2, index3, index4;

	// Load the vertex and index array with the sky plane array data.
	for (j = 0; j < skyPlaneResolution; j++)
	{
		for (i = 0; i < skyPlaneResolution; i++)
		{
			index1 = j * (skyPlaneResolution + 1) + i;
			index2 = j * (skyPlaneResolution + 1) + (i + 1);
			index3 = (j + 1) * (skyPlaneResolution + 1) + i;
			index4 = (j + 1) * (skyPlaneResolution + 1) + (i + 1);

			// Triangle 1 - Upper Left
			v.x = m_skyPlane[index1].x; v.y = m_skyPlane[index1].y;  v.z = m_skyPlane[index1].z;
			v.tu = m_skyPlane[index1].tu; v.tv = m_skyPlane[index1].tv;
			woma_skyPlane.push_back(v);

			// Triangle 1 - Upper Right
			v.x = m_skyPlane[index2].x; v.y = m_skyPlane[index2].y;  v.z = m_skyPlane[index2].z;
			v.tu = m_skyPlane[index2].tu; v.tv = m_skyPlane[index2].tv;
			woma_skyPlane.push_back(v);

			// Triangle 1 - Bottom Left
			v.x = m_skyPlane[index3].x; v.y = m_skyPlane[index3].y;  v.z = m_skyPlane[index3].z;
			v.tu = m_skyPlane[index3].tu; v.tv = m_skyPlane[index3].tv;
			woma_skyPlane.push_back(v);

			// Triangle 2 - Bottom Left
			v.x = m_skyPlane[index3].x; v.y = m_skyPlane[index3].y;  v.z = m_skyPlane[index3].z;
			v.tu = m_skyPlane[index3].tu; v.tv = m_skyPlane[index3].tv;
			woma_skyPlane.push_back(v);

			// Triangle 2 - Upper Right
			v.x = m_skyPlane[index2].x; v.y = m_skyPlane[index2].y;  v.z = m_skyPlane[index2].z;
			v.tu = m_skyPlane[index2].tu; v.tv = m_skyPlane[index2].tv;
			woma_skyPlane.push_back(v);

			// Triangle 2 - Bottom Right
			v.x = m_skyPlane[index4].x; v.y = m_skyPlane[index4].y;  v.z = m_skyPlane[index4].z;
			v.tu = m_skyPlane[index4].tu; v.tv = m_skyPlane[index4].tv;
			woma_skyPlane.push_back(v);
		}
	}

	SAFE_DELETE(m_skyPlane);

	std::vector<STRING> sky_plane_textures;
	sky_plane_textures.push_back(CLOUDTEXTUREFILENAME);
	sky_plane_textures.push_back(PERTURBTEXTUREFILENAME);

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) {CREATE_MODELGL3_IF_NOT_EXCEPTION(SystemHandle->m_Application->model_skyPlane, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);}
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) {CREATE_MODELDX_IF_NOT_EXCEPTION(SystemHandle->m_Application->model_skyPlane, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);}
	ASSERT(SystemHandle->m_Application->model_skyPlane->LoadTexture(pContext11, TEXT("sky_plane_textures"), m_driver11, SHADER_USE_CURVED_REAL_SKY_PLANE, &sky_plane_textures, &woma_skyPlane, NULL));

	return false;
}

void RealSkyPlaneClass::Render(void* pContext)
{
	// Increment the texture translation value each frame.
	m_translation += (float)SystemHandle->m_Application->dt/500000;
	if (m_translation > 1.0f)
		m_translation -= 1.0f;

	if (m_Driver->RenderfirstTime)
	{
		SystemHandle->m_Application->model_skyPlane->translation(0, 0, 0);
		SystemHandle->m_Application->model_skyPlane->scale(SystemHandle->AppSettings->SCREEN_DEPTH-1, SystemHandle->AppSettings->SCREEN_DEPTH-1, SystemHandle->AppSettings->SCREEN_DEPTH-1);
	}
	SystemHandle->m_Application->model_skyPlane->RenderSky(pContext, CAMERA_SKY, 1); // Camera with fixed position: 0,0,0: (CAMERA_SKY)
	return;
}
#endif
