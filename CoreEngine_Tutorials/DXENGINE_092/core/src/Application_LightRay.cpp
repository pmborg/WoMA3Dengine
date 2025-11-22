// --------------------------------------------------------------------------------------------
// Filename: Application_LightRay.cpp
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
#include "OSengine.h"
#include "Math3D.h"
#include "mem_leak.h"

#if (defined DX_ENGINE)
	#include "DXmodelClass.h"
#endif

#if (defined OPENGL3 || defined OPENGL40) 
	#include "GLmodelClass.h"
#endif

#if defined ANDROID_PLATFORM
#include <cmath> // Add this for mathematical functions like cos and sin
#endif

//------------------------------------------------------------------------------------------
#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION //#if ENGINE_LEVEL >= 33
	void ApplicationClass::Use3DSunMoonPosition()
	{
		SunX = SunDistance * FAST_sin(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		SunZ = SunDistance * FAST_cos(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		SunY = SunDistance * FAST_sin(initWorld->SunElevation);	// Sun Elevation
	}
#endif

#if defined MAIN_RENDER_LIGHT_RAY
void ApplicationClass::CalculateLightRayVertex(float localSunDistance)
{
		//1. Initialize a temporary vertex + clear previous list:
		ModelColorVertexType vertex = { 0 };
		MyLightVertexVector.clear(); // Clean previous Vector

		// 2. Set ray color with: Sun Color:
		vertex.r = 255.0f / 255.0f;
		vertex.g = 201.0f / 255.0f;
		vertex.b = 14.0f / 255.0f;
		vertex.a = 1.0f;

		// 3 DEST: 0,-7,0 - vert1
		// ------------------------------------------------------------------------------------------------------
		vertex.x = 0; vertex.y = -7; vertex.z = 0;
		MyLightVertexVector.push_back(vertex);	//VERTICE1: Dest.
#if defined DEBUG_LOG_MAIN_RENDER_LIGHT_RAY
		womalog("LightVertex[0] %5.1f %5.1f %5.1f - ", MyLightVertexVector[0].x, MyLightVertexVector[0].y, MyLightVertexVector[0].z);
#endif
		// ------------------------------------------------------------------------------------------------------

		// 4. SRC = sunlight origin - vert2
		// ------------------------------------------------------------------------------------------------------
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
			vertex.y = localSunDistance;
			vertex.z = (float)sin(rY) * localSunDistance;
		}
		else
		{
			// Get Fixed Light:
			const float* light = SystemHandle->m_Application->app_Light->GetDirection();
			vertex.x = MyLightVertexVector[0].x - light[0] * localSunDistance;
			vertex.y = MyLightVertexVector[0].y - light[1] * localSunDistance;
			vertex.z = MyLightVertexVector[0].z - light[2] * localSunDistance;
		}
#endif

		MyLightVertexVector.push_back(vertex);	//VERTICE1: Source.
#if defined DEBUG_LOG_MAIN_RENDER_LIGHT_RAY
		womalog("LightVertex[1]: %5.1f %5.1f %5.1f\n", MyLightVertexVector[1].x, MyLightVertexVector[1].y, MyLightVertexVector[1].z);
		//LightVertex[0]   0.0  -7.0   0.0 - LightVertex[1]: -473.8 512.0 194.2
#endif

		// ------------------------------------------------------------------------------------------------------

		WOMA::vec3 lightDirTemp = {};

#if defined DX_ENGINE
		//vec = {-0.654295921, 0.707106709, 0.268135875, 0.00138106779}
		XMVECTOR vec = XMVector3Normalize(XMVectorSet(vertex.x, vertex.y, vertex.z, 1));	//SRC

		// Keep old logic by storing the old result
		// lightDirTemp = { x = 0.654295921 y = -0.707106709 z = 0.268135875 }
		lightDirTemp = WOMA::vec3(-vec.m128_f32[0], -vec.m128_f32[1], -vec.m128_f32[2]);

		// Now apply final correction:
		{
			float lx = lightDirTemp.x;
			float ly = lightDirTemp.y;
			float lz = lightDirTemp.z;

			float len = sqrtf(lx * lx + ly * ly + lz * lz);
			if (len > 0.0001f) {
				lx /= len; ly /= len; lz /= len;
			}

			// lx  0.654295981
			// ly -0.707106769
			// lz  0.268135905
			app_Light->SetDirection(lx, ly, lz);
		}
#else
		WOMA::vec3 vec = {};
		WOMA::vec3 Init(vertex.x, vertex.y, vertex.z);

		vec = vector3dNormalize(Init);
	  #if defined WINDOWS_PLATFORM
		app_Light->SetDirection(-vec.m128_f32[0], -vec.m128_f32[1], -vec.m128_f32[2]);
	  #else
		app_Light->SetDirection(-vec.x, -vec.y, vec.z);
	  #endif
#endif

		// Generate the new app_Light: ViewMatrix
		#if defined DX_ENGINE
		app_Light->GenerateViewMatrix(vertex.x, vertex.y, vertex.z);
		#endif
}

void ApplicationClass::initLightRay(void* ctx)
{
	m_LightVertexVector = &MyLightVertexVector;
#if defined DX_ENGINE
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;
#else
	#define pContext ctx
#endif

	// ----------------------------------------------------------------------------
	#if defined MAIN_RENDER_LIGHT_RAY // Where is the Light (Ray of the Light)
	// ----------------------------------------------------------------------------
	{
		ModelColorVertexType vertex = {0};
		MyLightVertexVector.push_back(vertex); //2 Fake initial Vertices!
		MyLightVertexVector.push_back(vertex);

		// Step 2: Create a model: NEW GLmodelClass; || NEW DXmodelClass;
	#if (defined OPENGL3 || defined OPENGL4)
		if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
		{ 
			CREATE_MODELGL3_IF_NOT_EXCEPTION(m_lightRayModel,  /*Fake*/I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Create MODEL
		} else
	#endif
		{
			CREATE_MODELDX_IF_NOT_EXCEPTION(m_lightRayModel,  /*Fake*/I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Create MODEL
		}
		
		m_lightRayModel->PrimitiveTopology = LINELIST; // Draw just a line
		ASSERT (m_lightRayModel->LoadColor(pContext,TEXT("m_lightRayModel"), m_Driver, SHADER_COLOR, &MyLightVertexVector, NULL, 0));

	}
	#endif
}
#endif
