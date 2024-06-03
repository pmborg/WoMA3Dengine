// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass.h
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
#ifndef _ApplicationClass_H_
#define _ApplicationClass_H_
#pragma once

// -------------------------------------------------------------------------------------------------
#include "main.h"
#include <d3d11.h>

// Set Basic Defaults:
#define I_AM_2D				/*2D*/		false
#define I_AM_3D				/*3D*/		true

#define I_HAVE_NO_SHADOWS	/*NoShadow*/false
#define I_HAVE_SHADOWS		/*Shadow*/	true

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

#include "Math3D.h"
#include "winCpuClass.h"

#include "fpsClass.h"
#include "timerClass.h"

#include "initWorld.h"
#include "astroClass.h"

#include "weatherClass.h"
#include "metarClass.h"

#include "WomaDriverClass.h"

// -------------------------------------------------------------------------------------------------

extern UINT RENDER_PAGE;

#define MAX_TERRAINS 0

#pragma warning( push )
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition

	#if defined DX_ENGINE
		#define CREATE_MODELDX_IF_NOT_EXCEPTION(model, model3D, renderShadow1, renderShadow2) {\
			model = NEW DirectX::DXmodelClass(model3D, TRIANGLELIST, false, renderShadow1); IF_NOT_THROW_EXCEPTION (model); \
		}

		#define SAFE_SHUTDOWN_MODELDX(model) {\
			if(model) { (model)->Shutdown(); delete ((DirectX::DXmodelClass*)model); model=NULL; } \
		}
	#else
		#define CREATE_MODELDX_IF_NOT_EXCEPTION(model, model3D, renderShadow) {}
	#endif

		#define CREATE_MODELGL3_IF_NOT_EXCEPTION(model, model3D, renderShadow1, renderShadow2) {\
			model = NEW GLmodelClass(model3D); IF_NOT_THROW_EXCEPTION (model); \
		}

		#define SAFE_SHUTDOWN_MODELGL3(model) {\
			if (model) { (model)->Shutdown(); delete ((GLmodelClass*)model); model=NULL; } \
		}


#define CREATE_MODEL_IF_NOT_EXCEPTION(model, IAM, SHADOW1, SHADOW2)\
{\
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)\
	{\
		CREATE_MODELGL3_IF_NOT_EXCEPTION(model, IAM, SHADOW1, SHADOW2);	\
	}\
	else\
	{\
		CREATE_MODELDX_IF_NOT_EXCEPTION(model, IAM, SHADOW1, SHADOW2);\
	}\
}

#pragma warning( pop )

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	ApplicationClass();
	~ApplicationClass();
	
	void Shutdown();
	// --------------------------------------------------------------
	#ifdef WINDOWS_PLATFORM && defined USE_TIMER_CLASS
	void Benchmark();
	#endif

	bool Initialize();
	bool Start();
	void WOMA_APPLICATION_Shutdown();

	#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION //#if ENGINE_LEVEL >= 33
	float SunX, SunY, SunZ;
	float MoonX, MoonY, MoonZ;
	void Calc3DSunMoonPosition();
	#endif

	void RenderScene(UINT monitorWindow, WomaDriverClass* driver);
	float Update(UINT monitorWindow, WomaDriverClass* m_Driver);					// PROCESS User Update
	void AppRender(UINT monitorWindow, float fadeLight);	// RENDER - 3D

	virtual bool WOMA_APPLICATION_InitGUI();

	// 3D
	bool	Initialize(WomaDriverClass* Driver);

	// SKY
#if defined USE_SKY2D || ENGINE_LEVEL >= 27
	std::vector<ModelTextureLightVertexType> sky_vertexdata; //std::vector<ModelTextureVertexType> sky_vertexdata;
	std::vector<UINT>						 sky_indexdata;
#endif

private:



//VARS:
// ---------------------------------------------------------------------
private:


public:
	float	dt=0;	// Delta time

	//---------------------------------------------------------------------
	//TO SAFE DELETE: void ApplicationClass::WOMA_APPLICATION_Shutdown()
	//---------------------------------------------------------------------
	InitWorld*		initWorld = NULL;    // Get Astro Positions

	WeatherClass*	weatherClass = NULL;
	MetarClass*		metarClass = NULL;

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------

	float ClearColor[4]={0};

public:
	void DemoRender();
	void RenderSprites();

	//3D
	bool DEMO_WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver); // APP_Load

	// 2D

	// TERRAIN


};

#endif

