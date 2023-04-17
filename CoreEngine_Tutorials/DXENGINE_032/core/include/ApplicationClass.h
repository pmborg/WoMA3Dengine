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

#define PASS_OPAC			0
#define PASS_TRANSPARENT	1
#define PASS_SHADOWS		2

#include "virtualModelClass.h"
extern std::vector<VirtualModelClass*> m_screenShots;
// -------------------------------------------------------------------------------------------------
extern UINT RENDER_PAGE;
extern bool FORCE_RENDER_ALL;

extern int SpriteScreenToShow;
extern float fadeIntro;
extern UINT g_NetID;

#include "PositionClass.h"
#include "PlayerClass.h"
#include "DXinputClass.h"

#include "SceneManager.h"

#include "ApplicationTextClass.h"

#include "lightClass.h"	

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

	void RenderScene(UINT monitorWindow);

	float Update(UINT monitorWindow, WomaDriverClass* m_Driver);					// PROCESS User Update
	void AppPreRender(float fadeLight);					// PRE-RENDER - Shadows
	void AppRender(UINT monitorWindow, float fadeLight);	// RENDER - 3D
	void AppPosRender();									// POS-RENDER - 2D: Render 

	virtual bool WOMA_APPLICATION_InitGUI();

	// 3D
	void	defineSquarModel(float unit);
	bool	Initialize(WomaDriverClass* Driver);
	virtual bool WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver); // APP_Load

	void CalculateLightRayVertex (float SunDistance);

	std::vector<ModelColorVertexType> MyLightVertexVector;
	std::vector<ModelColorVertexType>* m_LightVertexVector;
	LightClass* m_Light = NULL;

	void initLightRay(WomaDriverClass* m_Driver);

	std::vector<PositionClass*> m_Position;

	std::vector<VirtualModelClass*> objModel;

	// SKY

	void	SetPlayerPosition(UINT netID);
	bool	HandleUserInput(float frameTime);

	void	initText(WomaDriverClass* m_Driver);

	DirectX::ApplicationTextClass* AppTextClass=NULL;

private:
	void	Render_SKY_SUN_MOON(float);				//30



//VARS:
// ---------------------------------------------------------------------
private:


	PositionClass* m_NextPosition;

public:
	float	dt=0;	// Delta time

	//---------------------------------------------------------------------
	//TO SAFE DELETE: void ApplicationClass::WOMA_APPLICATION_Shutdown()
	//---------------------------------------------------------------------
	InitWorld*		initWorld = NULL;    // Get Astro Positions

	WeatherClass*	weatherClass = NULL;
	MetarClass*		metarClass = NULL;

	VirtualModelClass* m_lightRayModel = NULL;

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	ModelColorVertexType colorVertex = { 0 };					// Use this "VERTEX" on macro
	std::vector<UINT> IndexSquarList;							// COLOR-DEMO-1: UINT indexList[6] = {0,1,2, 0,3,1};
	std::vector<UINT> IndexTriangleList;						// COLOR-DEMO-2: UINT indexList[6] = {0,1,2};

	float ClearColor[4]={0};

	//SceneManager*				m_sceneManager=NULL;


// --------------------------------------------------------------------------------------------
// PURPOSE: 
// --------------------------------------------------------------------------------------------
public:
	void DemoRender();
	void RenderSprites();

	//3D
	bool DEMO_WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver); // APP_Load

	// 2D
	bool DEMO_WOMA_APPLICATION_InitializeSprites2D();
	void DEMO_WOMA_APPLICATION_Shutdown2D();

	void initColorDemo(WomaDriverClass* Driver);
	void initTextureDemo();
	void initLightDemo();
	void	initTitleBanner2D();
	bool initCubes3D(WomaDriverClass* m_Driver);

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	//DEMO-1:
	std::vector<ModelColorVertexType> SquarColorVertexVector;	// COLOR-DEMO-1: CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED
	VirtualModelClass* m_1stSquar3DColorModel = NULL;			// COLOR-DEMO-1: CREATE_MODELDX_IF_NOT_EXCEPTION

	//DEMO-2:
	std::vector<ModelColorVertexType> TriangleColorVertexVector;// COLOR-DEMO-2: CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED
	VirtualModelClass* m_1stTriangle3DColorModel = NULL;		// COLOR-DEMO-2: CREATE_MODELDX_IF_NOT_EXCEPTION

	ModelTextureVertexType textureVertex = { 0 };					// Use this "VERTEX" on macro

	//DEMO-1:
	std::vector<ModelTextureVertexType> SquarTextureVertexVector;	// TEXTURE-DEMO-1: CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED

	VirtualModelClass* m_2nd3DModel = NULL;						// Model

	VirtualModelClass* m_bmp3DModel = NULL;						// Model
	VirtualModelClass* m_jpg3DModel = NULL;						// Model
	VirtualModelClass* m_png3DModel = NULL;						// Model
	VirtualModelClass* m_tif3DModel = NULL;						// Model
	VirtualModelClass* m_dds3DModel = NULL;						// Model
#if defined SUPPORT_TGA
	VirtualModelClass* m_tga3DModel = NULL;						// Model
#endif

	//DEMO-2:
	std::vector<ModelTextureVertexType> TriangleTextureVertexVector;	// TEXTURE-DEMO-2: CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED
	VirtualModelClass* m_1stTriangleTextureVertexModel = NULL;			// TEXTURE-DEMO-2: initLoadTexture()

	//DEMO-1:
	ModelTextureLightVertexType lightVertex1 = { 0 };					// Use this "VERTEX" on macro
	std::vector<ModelTextureLightVertexType> My3thModelVertexVector1;	// Declare: the Vector with Vertex "TYPE"
	VirtualModelClass* m_3th3DModel1 = NULL;							// Model

	//DEMO-2:
	std::vector<ModelTextureLightVertexType> TriangleLightVertexVector;	// TEXTURE-DEMO-2: CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED
	VirtualModelClass* m_1stTriangleLightVertexModel = NULL;			// TEXTURE-DEMO-2: initLoadTexture()
	VirtualModelClass* m_3th3DModel2 = NULL;							// Model

	// TERRAIN

	VirtualModelClass* m_titleModel = NULL;

	VirtualModelClass* m_cube1Model = NULL;
	VirtualModelClass* m_cube2Model = NULL;
	VirtualModelClass* m_cube3Model = NULL;

	std::vector<ModelTextureLightVertexType> Sphere_vertexdata; //CreateSphereModel()
	std::vector<UINT>						 Sphere_indexdata;	//CreateSphereModel()
	void	CreateSphereModel(UINT SPHERE_SIZE, int Sphere_gridpoints);

	VirtualModelClass* m_SphereModel1 = NULL;
	VirtualModelClass* m_SphereModel2 = NULL;
	void	initSphere1(float SPHERE_SIZE);
	void	initSphere2(float SPHERE_SIZE);

	VirtualModelClass* m_SkyModel = NULL;
	void	initSky(float SPHERE_SIZE);















};

#define SunDistance 512

#endif

