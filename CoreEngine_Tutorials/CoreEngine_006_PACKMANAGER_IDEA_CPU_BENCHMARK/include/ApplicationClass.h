// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass.h
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
//WomaIntegrityCheck = 1234567142;

#ifndef _ApplicationClass_H_
#define _ApplicationClass_H_
#pragma once

// -------------------------------------------------------------------------------------------------
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'
#include "main.h"

#if defined WINDOWS_PLATFORM && defined DX_ENGINE
#include <d3d11.h>
#endif

// Set Basic Defaults:
#define I_AM_2D				/*2D*/		false
#define I_AM_3D				/*3D*/		true

#define I_HAVE_NO_SHADOWS	/*NoShadow*/false
#define I_HAVE_SHADOWS		/*Shadow*/	true

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#if defined WINDOWS_PLATFORM
#include "winCpuClass.h"
#endif

	#if defined USE_TIMER
	#include "Math3D.h"
	#include "fpsClass.h"
	#include "timerClass.h"
	#endif

#if TUTORIAL_CHAP >= 90
#include "../network/NetworkClass.h"
#else
#define MAX_CLIENTS 1
#endif

/*
#if DX_ENGINE_LEVEL >= 56
struct compoundTreeLoadOrder {
	UINT compoundTreeId;
	UINT order;
};
#endif
*/

// -------------------------------------------------------------------------------------------------

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
extern UINT g_NetID;
#endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
#include "positionClass.h"
#include "playerClass.h"
#if defined DX_ENGINE
	#include "DXinputClass.h"
#endif
#endif

#if  defined USE_RASTERTEK_TEXT_FONT
#include "ApplicationTextClass.h"
#endif

#if defined USE_DX10DRIVER_FONTS
#include "DXshaderClass.h"
#endif

#if defined SCENE_GENERATEDUNDERWATER
	#define MAX_TERRAINS 1
#endif
#if defined SCENE_WATER_TERRAIN
	#define MAX_TERRAINS 2
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN
	#define MAX_TERRAINS 3
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
	#define MAX_TERRAINS 4
#endif

#if defined (SCENE_COMPOUND)
#include "compound.h"
#endif

#pragma warning( push )
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition

#if true //WINDOWS_PLATFORM

	#if defined DX_ENGINE
		#define CREATE_MODELDX_IF_NOT_EXCEPTION(model, model3D, renderShadow1, renderShadow2) {\
			model = NEW DirectX::DXmodelClass(model3D, TRIANGLELIST, false, renderShadow1); IF_NOT_THROW_EXCEPTION (model); \
		}

		#define SAFE_SHUTDOWN_MODELDX(model) {\
			if(model) { (model)->Shutdown(); delete ((DirectX::DXmodelClass*)model); model=NULL; } \
		}
	#else
		#define CREATE_MODELDX_IF_NOT_EXCEPTION(model, model3D, renderShadow1) {}
		#define CREATE_MODELDX_IF_NOT_EXCEPTION(model, model3D, renderShadow1, renderShadow2) {}
	#endif

	#if (defined OPENGL3 || defined OPENGL4)
		#define CREATE_MODELGL3_IF_NOT_EXCEPTION(model, model3D, renderShadow1, renderShadow2) {\
			model = NEW GLmodelClass(model3D); IF_NOT_THROW_EXCEPTION (model); \
		}

		#define SAFE_SHUTDOWN_MODELGL3(model) {\
			if (model) { (model)->Shutdown(); delete ((GLmodelClass*)model); model=NULL; } \
		}
	#else
		#define CREATE_MODELGL3_IF_NOT_EXCEPTION(model, model3D, renderShadow) {}
	#endif


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

#else
	// LINUX & ANDROID
	//#define CREATE_MODEL_IF_NOT_EXCEPTION(model, model3D) {\
	//	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { model = NEW GLmodelClass(model3D); IF_NOT_THROW_EXCEPTION (model); } \
	//}
#define CREATE_MODELDX_IF_NOT_EXCEPTION(model, model3D, renderShadow1) {}
#define CREATE_MODELDX_IF_NOT_EXCEPTION(model, model3D, renderShadow1, renderShadow2) {}

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

	#define SAFE_SHUTDOWN_MODELGL3(model) {\
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) if (model) { (model)->Shutdown(); delete ((GLmodelClass*)model); model=NULL; } \
	}

	#define CAMERA_RENDER(camera) {\
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) ((GLopenGLclass*)m_Driver)->camera->Render(); \
	}
#endif
#pragma warning( pop )

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	UINT WomaIntegrityCheck = 1234567142;
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

	virtual bool WOMA_APPLICATION_InitGUI();

	float dayLightFade;

#if DX_ENGINE_LEVEL >= 23 || defined USE_VIEW2D_SPRITES
	void AppPosRender();																// POS-RENDER - 2D: Render 
#endif

#if (defined USE_LIGHT_RAY) //&& defined USE_ASTRO_CLASS
	void CalculateLightRayVertex (float SunDistance);
#endif

#if defined USE_DX10DRIVER_FONTS
	DXshaderClass* m_FontV2Shader = NULL;
	IDXGIKeyedMutex* keyedMutex11 = NULL;
	IDXGIKeyedMutex* keyedMutex10 = NULL;
#endif

#if defined USE_LIGHT_RAY
	void initLightRay();
#endif

	//TERRAINs:
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN //24
	//CTerrain*	autoGenUnderWaterTerrain;
	//void		initUnderWaterDemo(void* m_Driver, UINT terrainId);
#endif

//#if defined SCENE_MAIN_TOPO_TERRAIN
	//CTerrain*	mainTerrain;
	//void		initMainTopoTerrainDemo(void* m_Driver, UINT terrainId);
//#endif

#if defined SCENE_MAIN_TERRAIN
	CTerrain*	mainTerrain;
	//void		initMainTerrainDemo(UINT terrainId);
#endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	std::vector<PositionClass*> m_Position;
#endif

#if defined USE_SKY2D || ENGINE_LEVEL >= 27 // SKY
	std::vector<ModelTextureLightVertexType> sky_vertexdata; //std::vector<ModelTextureVertexType> sky_vertexdata;
	std::vector<UINT>						 sky_indexdata;
#endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	void	SetPlayerPosition(UINT netID);
	bool	HandleUserInput(double frameTime);
#endif

#if  defined USE_RASTERTEK_TEXT_FONT
	bool	initText(/*WomaDriverClass* m_Driver*/);
	DirectX::ApplicationTextClass* AppTextClass = NULL;
#endif

#if defined USE_TITLE_BANNER //24
	VirtualModelClass*		m_titleModel = NULL;
#endif

	float scaleX = 0;
	float scaleY = 0;
	float rescale = 0;

private:

//VARS:
// ---------------------------------------------------------------------
private:

#if defined USE_DIRECT_INPUT//|| defined INTRO_DEMO
	PositionClass* m_NextPosition;
#endif

public:
	double	dt=0;	// Delta time

#if defined SCENE_COMPOUND
	void LoadCompound(int i, WomaDriverClass* Driver);
	UINT totalCompoundLoaded = 0;
	UINT totalRenderCompound = 0;
	void RenderCompound(UINT i, UINT p);
	void RenderAllCompounds(UINT p);
	void RenderAllTransparentCompounds();

	UINT N_COMPOUNDS;
	UINT CHG_COMPOUND;

	/*
	// Originally: G:\DRIVE_MY_SOURCE_CODE\Dx11Engine3D\Dx11Engine3Dx64\src\Applicationclass.cpp
	compoundTree compound[] = {

	//G:\woma2013\trunk\Part1\source\engine\application
	std::vector <compoundTree>  compound;

	compoundTree compound[];
	compoundTreeLoadOrder compoundTreeLoadingOrder[];
	*/
#endif

	//---------------------------------------------------------------------
	//TO SAFE DELETE: void ApplicationClass::WOMA_APPLICATION_Shutdown()
	//---------------------------------------------------------------------
#if defined USE_ASTRO_CLASS
	InitWorld*		initWorld = NULL;    // Get Astro Positions
#endif

#if defined USE_LIGHT_RAY // LightModel
	VirtualModelClass* m_lightRayModel = NULL;
#endif

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	#define HowManyPlayers SystemHandle->m_player.size()
#endif

public:
	void DemoRender();
	void RenderDemoIntroSprites();

	// 2D
#if DX_ENGINE_LEVEL >= 22 || defined USE_VIEW2D_SPRITES
	void DEMO_WOMA_APPLICATION_Shutdown2D();
#endif
#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
	bool DEMO_WOMA_APPLICATION_InitializeSprites2D();
#endif

#if defined SCENE_COLOR
	void initColorDemo();
#endif
#if defined USE_TITLE_BANNER	//24
	void	initStatic2D();
#endif
#if defined USE_CUBE // Cubes
	bool initCubes3D(/*WomaDriverClass* m_Driver*/);
#endif



#if defined USE_IMGUI
	VirtualModelClass* m_iconSettings = NULL;
#endif

#if defined USE_CUBE // Cubes
	VirtualModelClass* m_cube1Model = NULL;
	VirtualModelClass* m_cube2Model = NULL;
	VirtualModelClass* m_cube3Model = NULL;
#endif


};

#endif

