// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass.h
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
//WomaIntegrityCheck = 1234525217;

#ifndef _ApplicationClass_H_
#define _ApplicationClass_H_
#pragma once

// -------------------------------------------------------------------------------------------------
#pragma warning( push )
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#pragma warning( disable : 4002 ) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'

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
#include "playerClass.h"

#if defined CHECK_OBJ_COLISION
#include "Tree.h"
#endif

#if defined USE_ASSIMP_LATEST && DEMO_LEVEL >= 79 && DEMO_LEVEL <= 80
#include "GLAnimationScene.h"
#endif


#if TUTORIAL_CHAP >= 140
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

#if defined USE_DIRECT_INPUT
extern UINT g_NetID;
#endif

#if defined INTRO_DEMO && CORE_ENGINE_LEVEL >= 10
extern int SpriteScreenToShow;
extern float fadeIntro;
#endif

#if false //DX_ENGINE_LEVEL >= 99
struct InstanceType
{
	DirectX::XMMATRIX worldMatrix;   // Per-instance world transform
	DirectX::XMMATRIX WVP;           // Precomputed World * View * Projection (optional)

	BOOL  VShasLight;
	BOOL  VShasSpecular;
	BOOL  VShasNormMap;
	BOOL  VShasShadowMap;

	UINT  VSshaderType;				// 0=2D, 1=Sky, 2=3D, etc.
	BOOL  vsIsSky;
	DirectX::XMFLOAT2 pad;

	float VSrotX;
	float VSrotY;
	float VSrotZ;
	BOOL  isAnimatedBill;

	DirectX::XMFLOAT4 VSambientColor;
	DirectX::XMFLOAT4 VSdiffuseColor;
	DirectX::XMFLOAT4 VSemissiveColor;
	DirectX::XMMATRIX ViewToLightProj;
	DirectX::XMMATRIX WorldInverseTranspose;
};
#else
#endif

#if defined USE_DIRECT_INPUT
#include "positionClass.h"
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

#if defined SCENE_TERRAIN_QUAD_TREE
#include "TerrainQuadtreeClass.h"
#endif

#if defined CHECK_OBJ_COLISION //CHECK_COMPOUND_COLISION
extern int __cdecl CompoundSortCB(const VOID* arg1, const VOID* arg2);
#endif



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
	#define CREATE_MODELGL3_IF_NOT_EXCEPTION(model, model3D, renderShadow1, renderShadow2) {}
#endif


#define CREATE_MODEL_IF_NOT_EXCEPTION(model, IAM, SHADOW1, SHADOW2)\
{\
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)\
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
	UINT WomaIntegrityCheck = 1234525217;
	ApplicationClass();
	~ApplicationClass();
	
	void Shutdown();
	// --------------------------------------------------------------
	#if defined WINDOWS_PLATFORM && defined USE_TIMER_CLASS
	void Benchmark();
	#endif

	//bool Initialize();
	bool Start();
	void WOMA_APPLICATION_Shutdown();

	#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION
	float SunX, SunY, SunZ;
	float MoonX, MoonY, MoonZ;
	void Use3DSunMoonPosition();
	#endif

	virtual bool WOMA_APPLICATION_InitGUI();

	float dayLightFade;

#if CORE_ENGINE_LEVEL >= 10 && !defined NewWomaEngine
    void SortOutWhatNeedToBeRendered(void* pContext, WomaDriverClass* driver);
    void RenderScene(void* mainCtx, UINT monitorWindow, WomaDriverClass* driver);
	float ProcessMovementInput_and_UpdateDemos();						// PROCESS User Update
	void RenderMainSky(UINT monitorWindow, float fadeLight, void* pContext);
	void WaterTerrain(UINT monitorWindow, float fadeLight, void* pContext);
	void AppRender(UINT monitorWindow, UINT level, float fadeLight, void * pContext);								// RENDER - 3D
	bool Initialize(void* pContext, WomaDriverClass* Driver);
#endif
#if defined CHECK_OBJ_COLISION
    XMVECTOR prwsPos = {}, prwsDir = {};
#endif
	bool InitLightandDemos(void* pContext, WomaDriverClass* Driver);
	void InitMainSky(void* pContext, WomaDriverClass* Driver);
	void InitTerrainandWaterSurfaces(void* pContext, WomaDriverClass* Driver);
	void AddObjsWithInstancesToXML();
	virtual bool WOMA_APPLICATION_Initialize3D(void * pContext, WomaDriverClass* Driver, UINT level); // APP_Load
	void AppPosRender(UINT monitorWindow, UINT level, float dayLightFade, void* mainCtx);																// POS-RENDER - 2D: Render 
	std::vector<ModelColorVertexType> MyLightVertexVector;
	std::vector<ModelColorVertexType>* m_LightVertexVector;
	LightClass* app_Light = NULL;

	#if defined MAIN_RENDER_LIGHT_RAY
	void CalculateLightRayVertex(float SunDistance);
	void initLightRay(void* pContext);
	#endif
	void RenderHUD_Logo(void* pContext);
	void RenderModel(void* pContext, UINT level, UINT threadID, UINT monitorWindow, WomaDriverClass* driver, UINT modelID, UINT pass, XMMATRIX* m_viewMatrix = NULL, XMMATRIX* m_projectionMatrix = NULL, float fadeLight = 1, UINT op = 0);
#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER && defined USE_FRUSTUM
	bool WOMA_LOAD_OBJ(void* pContext, UINT threadID, UINT level, WomaDriverClass* Driver, UINT i, TCHAR* wfilename);
#endif																			  
	// VARS:
	// ----------------------------------------------------------------

    UINT world_xml_objs = 0;
	UINT initial_world_xml_objs = 0;
	UINT theWorld_size=0;
	UINT objModel_size=0;
	UINT num_loading_objects = 1;


    UINT world_main_size = 0;
    UINT totalRendered = 0;

#if defined USE_DX_DRIVER_FONT
    DXshaderClass* m_FontV2Shader = NULL;
    IDXGIKeyedMutex* keyedMutex11 = NULL;
    IDXGIKeyedMutex* keyedMutex10 = NULL;
#endif

//83&84:



#if defined USE_DIRECT_INPUT
	std::vector<PositionClass*> m_Position;
#endif

#if defined CHECK_OBJ_COLISION
    void pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir);
	XMVECTOR faceNormalClosestObject = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    float pick(
                XMVECTOR pickRayInWorldSpacePos, 
                XMVECTOR pickRayInWorldSpaceDir,
                std::vector<XMFLOAT3>& vertPosArray, 
                std::vector<UINT/*DWORD*/>& indexPosArray, 
                XMMATRIX& worldSpace, 
                bool getPoligon = false);
    bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point);
    
	compoundTreeLoadOrder compoundLoadingOrder[5000] = {}; // MAX: 5000 OBJs on Scene
#endif

#if defined USE_SKY2D || ENGINE_LEVEL >= 27 // SKY
    std::vector<ModelTextureLightVertexType> sky_vertexdata;
    std::vector<UINT>						 sky_indexdata;
#endif
    std::vector<VirtualModelClass*> objModel;

#if defined CHECK_OBJ_COLISION //CHECK_COMPOUND_COLISION //float	closestObjDist = FLT_MAX;
	float	closestObjDist = FLT_MAX;
#endif

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	void	SetPlayerPosition(UINT netID);
	bool	ProcessUserKeyboardInputAndTerrainCollision(double frameTime);
#endif

#if  defined USE_RASTERTEK_TEXT_FONT
	bool	initText(void* pContext);
	DirectX::ApplicationTextClass* AppTextClass = NULL;
#endif

#if defined USE_TITLE_BANNER //24
	VirtualModelClass*		m_titleModel = NULL;
#endif

	float scaleX = 0;
	float scaleY = 0;
	float rescale = 0;

#ifdef INTRO_DEMO
	void	initIntroCreditsDemo(void* pContext);
#endif

// ---------------------------------------------------------------------
// PRIVATE VARS:
// ---------------------------------------------------------------------

private:
#if defined ALLOW_CBIND_PROGRESS_BAR
	TCHAR title[MAX_STR_LEN] = {};
#endif
	void	Render_SKY_SUN_MOON(float);				//30

#if defined USE_DIRECT_INPUT//|| defined INTRO_DEMO
	PositionClass* m_NextPosition;
#endif

public:
	float	dt=0;	// Delta time

#if defined SCENE_COMPOUND
	void LoadCompound(int i, WomaDriverClass* Driver);
	UINT totalCompoundLoaded = 0;
	UINT totalRenderCompound = 0;
	void RenderCompound(UINT i, UINT p);
	void RenderAllCompounds(UINT p);
	void RenderAllTransparentCompounds();

	UINT N_COMPOUNDS;
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

#if defined MAIN_RENDER_LIGHT_RAY
	VirtualModelClass* m_lightRayModel = NULL;
#endif

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
    float ClearColor[4] = { 0 };

	ModelColorVertexType colorVertex = { 0 };					// Use this "VERTEX" on macro
	std::vector<UINT> IndexSquarList;							// COLOR-DEMO-1: UINT indexList[6] = {0,1,2, 0,3,1};
	std::vector<UINT> IndexSquarList2;							// COLOR-DEMO-1: UINT indexList[6] = {0,1,2, 0,3,1};
	std::vector<UINT> IndexTriangleList;						// COLOR-DEMO-2: UINT indexList[6] = {0,1,2};

#if defined USE_DIRECT_INPUT// || defined INTRO_DEMO
	#define HowManyPlayers SystemHandle->m_player.size()
#endif

public:
	void DemoRender(void* pContext);
	void RenderDemoIntroSprites(void* pContext);

	// 2D
	void DEMO_WOMA_APPLICATION_Shutdown2D();
#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
	bool DEMO_WOMA_APPLICATION_InitializeSprites2D(void* pContext);
#endif

#if defined SCENE_COLOR
	void initColorDemo(void* pContext);
#endif
	void initTextureDemo(void* pContext);
	void initLightDemo(void* pContext);
#if defined USE_TITLE_BANNER	//24
	void	initStatic2D(void* pContext);
#endif
#if defined USE_CUBE // Cubes
	bool initCubes3D(void*);
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
	//ModelTextureVertexType lightVertex1 = { 0 };					// Use this "VERTEX" on macro
	std::vector<ModelTextureVertexType> My3thModelVertexVector1;	// Declare: the Vector with Vertex "TYPE"
	VirtualModelClass* m_3th3DModel1 = NULL;						// Model

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
	void	initSphere1(void* pContext, float SPHERE_SIZE);
	void	initSphere2(void* pContext, float SPHERE_SIZE);
#endif

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	VirtualModelClass* m_SkyModel = NULL;
#endif
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
	void	initSky(void* pContext, float SPHERE_SIZE);
#endif

    float shadergrassframeTime = 0;

#if defined SCENE_TERRAIN_QUAD_TREE //67
	TerrainQuadtreeClass* TerrainQuadtree;
#endif
#if defined USE_ASSIMP_LATEST && DEMO_LEVEL >= 79 && DEMO_LEVEL <= 80
	//The Scene
	MyAnimationScene* myScene = NULL;
	bool myScene_has_animation = false;
#endif

};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define SunDistance 512

extern void* getvoidcontext();

#endif

