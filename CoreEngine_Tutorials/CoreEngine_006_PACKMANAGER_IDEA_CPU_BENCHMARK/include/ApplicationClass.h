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

// -------------------------------------------------------------------------------------------------


#if defined USE_SCENE_MANAGER
	#include "SceneManager.h"
#endif

#if  defined USE_RASTERTEK_TEXT_FONT
	#include "ApplicationTextClass.h"
#endif

#if defined SCENE_MAIN_TOPO_TERRAIN
	#define MAX_TERRAINS 3

#elif defined SCENE_WATER_TERRAIN
	#define MAX_TERRAINS 2

#elif defined SCENE_GENERATEDUNDERWATER
	#define MAX_TERRAINS 1
#elif defined SCENE_MAIN_TERRAIN
	#define MAX_TERRAINS 1
#else
	#define MAX_TERRAINS 0
#endif


#if defined SCENE_TERRAIN_QUAD_TREE
#include "TerrainQuadtreeClass.h"
#endif

#pragma warning( push )
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition

		#define CREATE_MODELGL3_IF_NOT_EXCEPTION(model, model3D, renderShadow) {}


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

/*
	// DX & OPENGL3
	#if (defined DX9 || defined DX11 || defined DX12) && defined OPENGL3
		#define CREATE_MODEL_IF_NOT_EXCEPTION(model, model3D, castShadow, renderShadow) {\
			if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { model = NEW GLmodelClass(model3D); IF_NOT_THROW_EXCEPTION (model); } \
			if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) { model = NEW DirectX::DXmodelClass(model3D, TRIANGLELIST, true, castShadow, renderShadow); IF_NOT_THROW_EXCEPTION (model); } \
		}

		#define SAFE_SHUTDOWN_MODEL(model) {\
			if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) if (model) { (model)->Shutdown(); delete ((GLmodelClass*)model); model=NULL; } \
			if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) if(model) { (model)->Shutdown(); delete ((DirectX::DXmodelClass*)model); model=NULL; } \
		}\
		/*\
		#define CAMERA_RENDER(camera) {\
			if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) ((GLopenGLclass*)m_Driver)->camera->Render(); \
			if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) ((DX_CLASS*)m_Driver)->camera->Render(); \
		}
	#endif
*/
#pragma warning( pop )

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
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

	void RenderScene(UINT monitorWindow);

	float Update(UINT monitorWindow, WomaDriverClass* m_Driver);					// PROCESS User Update
	void AppPreRender(float fadeLight);					// PRE-RENDER - Shadows
	void AppRender(UINT monitorWindow, float fadeLight);	// RENDER - 3D
	void AppPosRender();									// POS-RENDER - 2D: Render 




#if defined USE_LIGHT_RAY
	void initLightRay(WomaDriverClass* m_Driver);
#endif

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN //24
	CTerrain*	autoGenUnderWaterTerrain;
	void		initUnderWaterDemo(UINT terrainId);
#endif

#if defined SCENE_MAIN_TOPO_TERRAIN
	CTerrain*	mainTopoTerrain;
	void		initMainTopoTerrainDemo(UINT terrainId);
#endif

#if defined SCENE_MAIN_TERRAIN
	CTerrain*	mainTerrain;
	void		initMainTerrainDemo(UINT terrainId);
#endif

	// SKY
#if defined USE_SKY2D || ENGINE_LEVEL >= 27
	std::vector<ModelTextureLightVertexType> sky_vertexdata; //std::vector<ModelTextureVertexType> sky_vertexdata;
	std::vector<UINT>						 sky_indexdata;
#endif

#if defined USE_SKYSPHERE
	void	CreateSkyModel(int SKY_SIZE, int sky_gridpoints);
	void	initSky();
#endif

#if defined SCENE_WATER_TERRAIN
	void		initTerrainWaterMeshDemo(UINT terrainId);	
#endif




#if  DX_ENGINE_LEVEL >= 22 || defined USE_RASTERTEK_TEXT_FONT
	void	initText();
#endif

#if  defined USE_RASTERTEK_TEXT_FONT
	DirectX::ApplicationTextClass* AppTextClass;
#endif

private:

//VARS:
// ---------------------------------------------------------------------
private:

public:
	UINT	RENDER_PAGE;
	float	dt;	// Delta time

	//---------------------------------------------------------------------
	//TO SAFE DELETE: void ApplicationClass::WOMA_APPLICATION_Shutdown()
	//---------------------------------------------------------------------

#if defined USE_LIGHT_RAY // LightModel
	VirtualModelClass* m_lightRayModel = NULL;
#endif

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------

#if defined USE_SCENE_MANAGER
	//SceneManager*				m_sceneManager;
#endif

};

#endif