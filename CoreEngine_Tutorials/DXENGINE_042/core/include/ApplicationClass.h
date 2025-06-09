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
//WomaIntegrityCheck = 1234567155;

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

#if CORE_ENGINE_LEVEL >= 7 && defined USE_ASTRO_CLASS
#include "initWorld.h"
#include "astroClass.h"
#endif

#if CORE_ENGINE_LEVEL >= 9 && defined USE_METARCLASS
#include "weatherClass.h"
#include "metarClass.h"
#endif

#if CORE_ENGINE_LEVEL >= 10 && !defined NewWomaEngine
#include "womadriverclass.h"
#endif

#if defined USE_ASSIMP_LATEST && DEMO_LEVEL >= 79 && DEMO_LEVEL <= 80
#include "GLAnimationScene.h"
#endif

#if TUTORIAL_CHAP >= 90
#include "../network/NetworkClass.h"
#else
#define MAX_CLIENTS 1
#endif

#define PASS_OPAC			0
#define PASS_TRANSPARENT	1
#define PASS_SHADOWS		2
#define PASS_MINIMAP1		11
#define PASS_MINIMAP2		12
#define PASS_MINIMAP3		13
#define PASS_BILL			70

#include "virtualModelClass.h"
extern std::vector<VirtualModelClass*> m_screenShots;

// -------------------------------------------------------------------------------------------------
extern bool FORCE_RENDER_ALL;

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
extern UINT g_NetID;
#endif

#if defined INTRO_DEMO && CORE_ENGINE_LEVEL >= 10 //29
extern int SpriteScreenToShow;
extern float fadeIntro;
#endif

struct InstanceType
{
	WOMA::vec3	position;
};

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
#include "positionClass.h"
#include "playerClass.h"
#if defined DX_ENGINE
	#include "DXinputClass.h"
#endif
#endif

#if defined USE_SCENE_MANAGER
#include "SceneManager.h"
#endif

#if  defined USE_RASTERTEK_TEXT_FONT
#include "ApplicationTextClass.h"
#endif

#include "lightClass.h"	

#if defined USE_DX_DRIVER_FONT
#include "DXshaderClass.h"
#endif

    #define MAX_TERRAINS 1
#if defined SCENE_WATER_TERRAIN
	#define MAX_TERRAINS 2
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN
	#define MAX_TERRAINS 3
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
	#define MAX_TERRAINS 4
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
#include "DXrenderTextureClass.h"
#endif

#if defined SCENE_TERRAIN_QUAD_TREE
#include "TerrainQuadtreeClass.h"
#endif

#if defined CHECK_OBJ_COLISION //CHECK_COMPOUND_COLISION
struct compoundTreeLoadOrder {
    UINT compoundTreeId;
    UINT order;
};

extern int __cdecl CompoundSortCB(const VOID* arg1, const VOID* arg2);
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
	UINT WomaIntegrityCheck = 1234567155;
	ApplicationClass();
	~ApplicationClass();
	
	void Shutdown();
	// --------------------------------------------------------------
	#ifdef WINDOWS_PLATFORM && defined USE_TIMER_CLASS
	void Benchmark();
	#endif

	//bool Initialize();
	bool Start();
	void WOMA_APPLICATION_Shutdown();

	#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION //#if ENGINE_LEVEL >= 33
	float SunX, SunY, SunZ;
	float MoonX, MoonY, MoonZ;
	void Calc3DSunMoonPosition();
	#endif

	virtual bool WOMA_APPLICATION_InitGUI();

	float dayLightFade;

#if CORE_ENGINE_LEVEL >= 10 && !defined NewWomaEngine //#if DX_ENGINE_LEVEL >= 19 && !defined NewWomaEngine
	void RenderScene(UINT monitorWindow, WomaDriverClass* driver);
	float Update();						// PROCESS User Update
	void AppRender(UINT monitorWindow,  float fadeLight);								// RENDER - 3D
	bool Initialize(WomaDriverClass* Driver);
#endif

	void AppPosRender(UINT monitorWindow);																// POS-RENDER - 2D: Render 

	void RenderModel(UINT monitorWindow, WomaDriverClass* driver, UINT modelID, UINT pass, XMMATRIX* m_viewMatrix=NULL, XMMATRIX* m_projectionMatrix = NULL);
	
	void AppPreRender(UINT monitorWindow, WomaDriverClass* Driver, float fadeLight);	// PRE-RENDER - Shadows

	virtual bool WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver); // APP_Load

#if defined USE_LIGHT_RAY
	void CalculateLightRayVertex (float SunDistance);
#endif

    UINT world_main_size=0;

	std::vector<ModelColorVertexType> MyLightVertexVector;
	std::vector<ModelColorVertexType>* m_LightVertexVector;
	LightClass* m_Light = NULL;

#if defined USE_DX_DRIVER_FONT
	DXshaderClass* m_FontV2Shader = NULL;
	IDXGIKeyedMutex* keyedMutex11 = NULL;
	IDXGIKeyedMutex* keyedMutex10 = NULL;
#endif

	UINT totalRendered = 0;

	UINT world_xml_objs = 0;
#if defined USE_LIGHT_RAY
	void initLightRay();
#endif

//83&84:

	//TERRAINs:

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	std::vector<PositionClass*> m_Position;
#endif

	std::vector<VirtualModelClass*> objModel;

#if defined CHECK_OBJ_COLISION
    void pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir);
    float pick(XMVECTOR pickRayInWorldSpacePos, XMVECTOR pickRayInWorldSpaceDir,
        std::vector<XMFLOAT3>& vertPosArray, std::vector<UINT/*DWORD*/>& indexPosArray, XMMATRIX& worldSpace, bool getPoligon = false);
    bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point);
    void anyMouseClickToPick();
#endif

#if defined CHECK_OBJ_COLISION //CHECK_COMPOUND_COLISION
    compoundTreeLoadOrder compoundTreeLoadingOrder[10000] = {}; // MAX 10000 Objs on Scene
#endif

	void initShadowTextureDemo();

#if defined USE_SKY2D || ENGINE_LEVEL >= 27 // SKY
	std::vector<ModelTextureLightVertexType> sky_vertexdata; //std::vector<ModelTextureVertexType> sky_vertexdata;
	std::vector<UINT>						 sky_indexdata;
#endif

	void WOMA_APPLICATION_FrameUpdateInstancesPositions(UINT m_ObjId, int m_instanceCount, InstanceType* instances_);

#if defined CHECK_OBJ_COLISION //CHECK_COMPOUND_COLISION //float	closestObjDist = FLT_MAX;
	float	closestObjDist = FLT_MAX;
#endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	void	SetPlayerPosition(UINT netID);
	bool	HandleUserInput(double frameTime);
#endif

#if  defined USE_RASTERTEK_TEXT_FONT
	bool	initText();
	DirectX::ApplicationTextClass* AppTextClass = NULL;
#endif

#if defined USE_TITLE_BANNER //24
	VirtualModelClass*		m_titleModel = NULL;
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	DXrendertextureclass*	m_RenderShadowTexture = NULL;	//TO INTERNAL RENDER!
#endif

	float scaleX = 0;
	float scaleY = 0;
	float rescale = 0;

#ifdef INTRO_DEMO
	void	initIntroDemo();
#endif

private:
	void	Render_SKY_SUN_MOON(float);				//30

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP
	void	RenderSceneToShadowMap(void* Driver);	//45
#endif

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

#if CORE_ENGINE_LEVEL >= 9 && defined USE_METARCLASS
	WeatherClass*	weatherClass = NULL;
	MetarClass*		metarClass = NULL;
#endif

#if defined USE_LIGHT_RAY
	VirtualModelClass* m_lightRayModel = NULL;
#endif

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	ModelColorVertexType colorVertex = { 0 };					// Use this "VERTEX" on macro
	std::vector<UINT> IndexSquarList;							// COLOR-DEMO-1: UINT indexList[6] = {0,1,2, 0,3,1};
	std::vector<UINT> IndexTriangleList;						// COLOR-DEMO-2: UINT indexList[6] = {0,1,2};

	float ClearColor[4]={0};

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	#define HowManyPlayers SystemHandle->m_player.size()
#endif

public:
	void DemoRender();
	void RenderDemoIntroSprites();

	// 2D
	void DEMO_WOMA_APPLICATION_Shutdown2D();
#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
	bool DEMO_WOMA_APPLICATION_InitializeSprites2D();
#endif

#if defined SCENE_COLOR
	void initColorDemo();
#endif
	void initTextureDemo();
	void initLightDemo();
#if defined USE_TITLE_BANNER	//24
	void	initStatic2D();
#endif
#if defined USE_CUBE // Cubes
	bool initCubes3D();
#endif

#if defined INTRO_DEMO // VIDEO+INTRO+DEMO
	float	WOMA_APPLICATION_DemoRender(UINT64 passedTotalTime);
	float	WOMA_APPLICATION_IntroRender(UINT64 passedTotalTime);

#endif

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	//DEMO-1:
	std::vector<ModelColorVertexType> SquareColorVertexVector;	// COLOR-DEMO-1: CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED
	VirtualModelClass* m_1stSquare3DColorModel = NULL;			// COLOR-DEMO-1: CREATE_MODELDX_IF_NOT_EXCEPTION

	//DEMO-2:
	std::vector<ModelColorVertexType> TriangleColorVertexVector;// COLOR-DEMO-2: CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED
	VirtualModelClass* m_1stTriangle3DColorModel = NULL;		// COLOR-DEMO-2: CREATE_MODELDX_IF_NOT_EXCEPTION

	ModelTextureVertexType textureVertex;					// Use this "VERTEX" on macro

	//DEMO-1:
	std::vector<ModelTextureVertexType> SquareTextureVertexVector;	// TEXTURE-DEMO-1: CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED

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



#if defined USE_CUBE // Cubes
	VirtualModelClass* m_cube1Model = NULL;
	VirtualModelClass* m_cube2Model = NULL;
	VirtualModelClass* m_cube3Model = NULL;
#endif

#if defined USE_SPHERE && DX_ENGINE_LEVEL >= 26
	std::vector<ModelTextureLightVertexType> Sphere_vertexdata; //CreateSphereModel()
	std::vector<UINT>						 Sphere_indexdata;	//CreateSphereModel()
	void	CreateSphereModel(UINT SPHERE_SIZE, int Sphere_gridpoints);

	VirtualModelClass* m_SphereModel1 = NULL;
	VirtualModelClass* m_SphereModel2 = NULL;
	void	initSphere1(float SPHERE_SIZE);
	void	initSphere2(float SPHERE_SIZE);
#endif

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	VirtualModelClass* m_SkyModel = NULL;
#endif
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
	void	initSky(float SPHERE_SIZE);
#endif

#if defined SCENE_TERRAIN_QUAD_TREE //67
	TerrainQuadtreeClass* TerrainQuadtree;
#endif
#if defined USE_ASSIMP_LATEST && DEMO_LEVEL >= 79 && DEMO_LEVEL <= 80
	//The Scene
	MyAnimationScene* myScene = NULL;
	bool myScene_has_animation = false;
#endif
};

#define SunDistance 512

#endif

