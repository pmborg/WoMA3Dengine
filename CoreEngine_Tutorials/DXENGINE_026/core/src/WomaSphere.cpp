// --------------------------------------------------------------------------------------------
// Filename: WomaSphere.cpp
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

#include "platform.h"
#if defined USE_SPHERE
#include "OSengine.h"
#include "ApplicationClass.h"
#include "Math3D.h"
#include "mem_leak.h"
#include "womadriverclass.h"	//woma
#include "GLmodelClass.h"
#if (defined DX_ENGINE)
#include "dxWinSystemClass.h"
#include "DXmodelClass.h"
#endif
#if defined ANDROID_PLATFORM
#include "math.h"
#endif
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

	#define SPHERE_GRIDPOINTS 100
extern std::vector<WomaDriverClass*> driverList;
void ApplicationClass::initSphere1(float SPHERE_SIZE)
{
	if (Sphere_vertexdata.size() == 0)
		CreateSphereModel((int)SPHERE_SIZE, SPHERE_GRIDPOINTS);	//(UINT SPHERE_SIZE, int Sphere_gridpoints)

	std::vector<STRING> Textures;

	//DAY:
	Textures.push_back(SKY_DOME_DAY_TEXTURE);

	CREATE_MODEL_IF_NOT_EXCEPTION(m_SphereModel1, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);
	#define m_Driver  driverList[SystemHandle->AppSettings->DRIVER]
	//m_SphereModel1->ModelHASlight = false; // Before Load: Dont Calculate light/shadow for Sky-Dome! 
	ASSERT(m_SphereModel1->LoadLight(TEXT("m_SphereModel"), m_Driver, SHADER_TEXTURE_LIGHT, &Textures, &Sphere_vertexdata, &Sphere_indexdata));
	m_SphereModel1->PrimitiveTopology = TRIANGLESTRIP;
}
void ApplicationClass::initSphere2(float SPHERE_SIZE)
{
	if (Sphere_vertexdata.size() == 0)
		CreateSphereModel((int)SPHERE_SIZE, SPHERE_GRIDPOINTS);	//(UINT SPHERE_SIZE, int Sphere_gridpoints)

	std::vector<STRING> Textures;

	//NIGHT:
	Textures.push_back(SKY_DOME_NIGHT_TEXTURE);

	CREATE_MODEL_IF_NOT_EXCEPTION(m_SphereModel2, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);

	//m_SphereModel2->ModelHASlight = false; // Before Load: Dont Calculate light/shadow for Sky-Dome! 
	ASSERT(m_SphereModel2->LoadLight(TEXT("m_SphereModel2"), m_Driver, SHADER_TEXTURE_LIGHT, &Textures, &Sphere_vertexdata, &Sphere_indexdata));
	m_SphereModel2->PrimitiveTopology = TRIANGLESTRIP;
}

// ----------------------------------------------------------------------------
void ApplicationClass::CreateSphereModel(UINT SPHERE_SIZE, int Sphere_gridpoints)
// ----------------------------------------------------------------------------
{
	UINT nVertices = Sphere_gridpoints * (Sphere_gridpoints + 2) * 2;
	std::vector<ModelTextureLightVertexType> vertices;
	ModelTextureLightVertexType vertice;

	for (int j = 0; j < Sphere_gridpoints; j++)
	{
		int i = 0;
		vertice.x = cos(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)j / (float)Sphere_gridpoints);
		vertice.y = sin(-0.5f * PI + PI * (float)(j) / (float)Sphere_gridpoints);
		vertice.z = sin(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)j / (float)Sphere_gridpoints);

		vertice.tu = ((float)i / (float)Sphere_gridpoints * 2);
		vertices.push_back(vertice);


		for (i = 0; i < Sphere_gridpoints + 1; i++)
		{
			vertice.x = cos(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)j / (float)Sphere_gridpoints);
			vertice.y = sin(-0.5f * PI + PI * (float)(j) / (float)Sphere_gridpoints);
			vertice.z = sin(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)j / (float)Sphere_gridpoints);

			vertice.tu = ((float)i / (float)Sphere_gridpoints * 2);
			vertices.push_back(vertice);

			vertice.x = cos(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)(j + 1) / (float)Sphere_gridpoints);
			vertice.y = sin(-0.5f * PI + PI * (float)(j + 1) / (float)Sphere_gridpoints);
			vertice.z = sin(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)(j + 1) / (float)Sphere_gridpoints);

			vertice.tu = ((float)i / (float)Sphere_gridpoints * 2);
			vertices.push_back(vertice);
		}

		i = Sphere_gridpoints;
		vertice.x = cos(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)(j + 1) / (float)Sphere_gridpoints);
		vertice.y = sin(-0.5f * PI + PI * (float)(j + 1) / (float)Sphere_gridpoints);
		vertice.z = sin(2.0f * PI * (float)i / (float)Sphere_gridpoints) * cos(-0.5f * PI + PI * (float)(j + 1) / (float)Sphere_gridpoints);

		vertice.tu = ((float)i / (float)Sphere_gridpoints * 2);
		vertices.push_back(vertice);
	}

	ModelTextureLightVertexType ShaderVertices;

	for (UINT i = 0; i < nVertices; i++)
	{
		// Positions: V0, V1, V2
		ShaderVertices.x = SPHERE_SIZE * vertices[i].x;
		ShaderVertices.y = SPHERE_SIZE * vertices[i].y;
		ShaderVertices.z = SPHERE_SIZE * vertices[i].z;

		ShaderVertices.nx = -1 * vertices[i].x;
		ShaderVertices.ny = -1 * vertices[i].y;
		ShaderVertices.nz = -1 * vertices[i].z;

		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
			ShaderVertices.tu = 1 - vertices[i].tu/2;
		else
			ShaderVertices.tu = 1 - vertices[i].tu;

		ShaderVertices.tv = 1 - (asinf(vertices[i].y) / PI + 0.5f);	// Map full Sphere
		//ShaderVertices.tv = 1 - (asinf(vertices[i].y) / (PI/2));	// Map half Sphere

		Sphere_vertexdata.push_back(ShaderVertices);
	}
}


// SKY + SUN + MOON 
// --------------------------------------------------------------------------------------------
void ApplicationClass::Render_SKY_SUN_MOON(float fade)
{
	float bright = 0.1f + 0.3f * fade;
	m_Light->SetAmbientColor(bright, bright, bright, 1);	// Fade: light with sun set

	// SUN & MOON:
	// --------------------------------------------------------------------------------------------
}

#endif
