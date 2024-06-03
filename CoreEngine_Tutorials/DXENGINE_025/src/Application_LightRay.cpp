// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: Application_LightRay.cpp
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
//WomaIntegrityCheck = 1234567831;

#include "OSengine.h"
#include "mem_leak.h"

#if (defined DX_ENGINE)
	#include "DXmodelClass.h"
#endif

	#include "GLmodelClass.h"

//------------------------------------------------------------------------------------------
#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION //#if ENGINE_LEVEL >= 33
	void ApplicationClass::Calc3DSunMoonPosition()
	{
		SunX = SunDistance * FAST_sin(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		SunZ = SunDistance * FAST_cos(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		SunY = SunDistance * FAST_sin(initWorld->SunElevation);	// Sun Elevation
	}
#endif

void ApplicationClass::CalculateLightRayVertex (float localSunDistance)
{
	ModelColorVertexType vertex = {0};
	MyLightVertexVector.clear(); // Clean Vector

	// Ray Sun Color:
	vertex.r = 255.0f/255;
	vertex.g = 201.0f/255;
	vertex.b = 14.0f/255;
	vertex.a = 1;

	// [1] DEST: 0,0,0
	vertex.x = 0; vertex.y = 0; vertex.z = 0;
	MyLightVertexVector.push_back(vertex);

	#if defined USE_REAL_SUNLIGHT_DIRECTION
		// Real Sun Position on Light:

		// [2]  SRC
		vertex.x = SunDistance * FAST_sin(initWorld->SunAzimuth);	// Real Sun Position on Sky:
		vertex.y = SunDistance * FAST_sin(initWorld->SunElevation);	// Sun Elevation
		vertex.z = SunDistance * FAST_cos(initWorld->SunAzimuth);	// Real Sun Position on Sky:

		static XMVECTOR vec = XMVector3Normalize(XMVectorSet(vertex.x, vertex.y, vertex.z, 1));
		m_Light->SetDirection (-vec.m128_f32[0], -vec.m128_f32[1], -vec.m128_f32[2]);
	#else
		// Get Fixed Light:
		const float* light = SystemHandle->m_Application->m_Light->GetDirection();

		// [2]  SRC
		vertex.x = MyLightVertexVector[0].x - light[0] * localSunDistance; //x
		vertex.y = MyLightVertexVector[0].y - light[1] * localSunDistance; //y
		vertex.z = MyLightVertexVector[0].z - light[2] * localSunDistance; //z
	#endif

	MyLightVertexVector.push_back(vertex);
}


void ApplicationClass::initLightRay(WomaDriverClass* m_Driver)
{
	m_LightVertexVector = &MyLightVertexVector;

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	{
		ModelColorVertexType vertex = {0};
		MyLightVertexVector.push_back(vertex); //2 Fake inital Vertices!
		MyLightVertexVector.push_back(vertex);

		// Step 2: Create a model: NEW GLmodelClass; || NEW DXmodelClass;
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{ 
			CREATE_MODELGL3_IF_NOT_EXCEPTION(m_lightRayModel,  /*Fake*/I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		} else
		{
			CREATE_MODELDX_IF_NOT_EXCEPTION(m_lightRayModel,  /*Fake*/I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		}

		m_lightRayModel->PrimitiveTopology = LINELIST; // Draw just a line
		ASSERT (m_lightRayModel->LoadColor(TEXT("m_lightRayModel"),m_Driver, SHADER_COLOR, &MyLightVertexVector, NULL));
	}
}

