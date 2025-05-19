// --------------------------------------------------------------------------------------------
// Filename: Application_LightRay.cpp
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
// PURPOSE: Paint the main window depending of engine state screen page.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;

#include "platform.h"
#include "OSengine.h"
#include "Math3D.h"
#include "mem_leak.h"
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined DXENGINE_LEVEL >= 28
	#include "SKY_30.h"					// Get SunDistance
#endif

#if (defined DX_ENGINE)
	#include "DXmodelClass.h"
#endif

#ifdef OPENGL3
	#include "GLmodelClass.h"
#endif

#if defined ANDROID_PLATFORM
#include <cmath> // Add this for mathematical functions like cos and sin
#endif

//------------------------------------------------------------------------------------------
#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION //#if ENGINE_LEVEL >= 33
	void ApplicationClass::Calc3DSunMoonPosition()
	{
		SunX = SunDistance * FAST_sin(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		SunZ = SunDistance * FAST_cos(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		SunY = SunDistance * FAST_sin(initWorld->SunElevation);	// Sun Elevation
	}
#endif

#if defined USE_LIGHT_RAY
	void ApplicationClass::CalculateLightRayVertex(float localSunDistance)
	{
		ModelColorVertexType vertex = { 0 };
		MyLightVertexVector.clear(); // Clean Vector

		// Ray Sun Color:
		vertex.r = 255.0f / 255;
		vertex.g = 201.0f / 255;
		vertex.b = 14.0f / 255;
		vertex.a = 1;

		// [1] DEST: 0,0,0
		vertex.x = 0; vertex.y = -7; vertex.z = 0;
		MyLightVertexVector.push_back(vertex);

		// [2]  SRC
#if defined USE_REAL_SUNLIGHT_DIRECTION
		// Real Sun Position on Light:
		vertex.x = SunX; // SunDistance* FAST_sin(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		vertex.y = SunY; // SunDistance* FAST_sin(initWorld->SunElevation);	// Sun Elevation
		vertex.z = SunZ; // SunDistance* FAST_cos(initWorld->SunAzimuth);	// Real Sun Position on Sky:
#else
		if (SUN_LIGHT_DEMO_ANIMATION)
		{
			static double rY = 0.0f;
			rY += dt * (0.010f / 16.66f);		// MOVIMENT FORMULA!
			vertex.x = (float)cos(rY) * localSunDistance;
			vertex.y = 400;
			vertex.z = (float)sin(rY) * localSunDistance;
		}
		else
		{
			// Get Fixed Light:
			const float* light = SystemHandle->m_Application->m_Light->GetDirection();
			vertex.x = MyLightVertexVector[0].x - light[0] * localSunDistance;
			vertex.y = localSunDistance;
			vertex.z = MyLightVertexVector[0].z - light[2] * localSunDistance;
		}
#endif

		MyLightVertexVector.push_back(vertex);

#if defined DX_ENGINE
		XMVECTOR vec = XMVector3Normalize(XMVectorSet(vertex.x, vertex.y, vertex.z, 1));
		m_Light->SetDirection(-vec.m128_f32[0], -vec.m128_f32[1], -vec.m128_f32[2]);
#else
		WOMA::vec3 vec = {};
		WOMA::vec3 Init(vertex.x, vertex.y, vertex.z);

		vec = vector3dNormalize(Init);
	  #if defined WINDOWS_PLATFORM
		m_Light->SetDirection(-vec.m128_f32[0], -vec.m128_f32[1], -vec.m128_f32[2]);
	  #else
		m_Light->SetDirection(-vec.x, -vec.y, vec.z);
	  #endif
#endif

		//Generate the new m_Light: ViewMatrix NO NEED, for shadows!??
		#if defined DX_ENGINE //&& future
		m_Light->GenerateViewMatrix(vertex.x, vertex.y, vertex.z);
		#endif
	}
#endif

#if defined USE_LIGHT_RAY
void ApplicationClass::initLightRay()
{
	m_LightVertexVector = &MyLightVertexVector;

	// ----------------------------------------------------------------------------
	#if defined USE_LIGHT_RAY // Where is the Light (Ray of the Light)
	// ----------------------------------------------------------------------------
	{
		ModelColorVertexType vertex = {0};
		MyLightVertexVector.push_back(vertex); //2 Fake inital Vertices!
		MyLightVertexVector.push_back(vertex);

		// Step 2: Create a model: NEW GLmodelClass; || NEW DXmodelClass;
	#if (defined OPENGL3 || defined OPENGL4)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{ 
			CREATE_MODELGL3_IF_NOT_EXCEPTION(m_lightRayModel,  /*Fake*/I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		} else
	#endif
		{
			CREATE_MODELDX_IF_NOT_EXCEPTION(m_lightRayModel,  /*Fake*/I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		}

		m_lightRayModel->PrimitiveTopology = LINELIST; // Draw just a line
		ASSERT (m_lightRayModel->LoadColor(TEXT("m_lightRayModel"),m_Driver, SHADER_COLOR, &MyLightVertexVector, NULL));
	}
	#endif
}

#endif
