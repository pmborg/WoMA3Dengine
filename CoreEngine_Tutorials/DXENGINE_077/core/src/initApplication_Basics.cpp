// --------------------------------------------------------------------------------------------
// Filename: initApplication_Basics.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "main.h"
#include "ApplicationClass.h"
#include "OSengine.h"
#include "Math3D.h"
#include "mem_leak.h"
#include "log.h"
#include <cinttypes>
#include <string.h>
#include "fileLoader.h"
#include "BillClass.h"

#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'
#pragma warning(disable : 4244) // warning C4244: '=': conversion from 'int' to 'float', possible

#if (defined OPENGL3 || defined OPENGL40) 
	#include "GLopenGLclass.h"
	#include "GLmodelClass.h"
#endif

#if defined DX_ENGINE
	#include "DXmodelClass.h"
#endif

#if defined USE_CUBE // Cubes
	#include "WomaCube.h"
#endif

	#include "SceneManager.h"


#if defined USE_DIRECT_INPUT && DX_ENGINE_LEVEL >= 63
float sort_cameraX = 0, sort_cameraY = 0, sort_cameraZ = 0;
#endif

#if defined SCENE_COLOR
// ----------------------------------------------------------------------------
void ApplicationClass::initColorDemo(void* pContext)
// ----------------------------------------------------------------------------
{
	{
		// --- DEMO-1 ---
		// Step 1: Prepare Vertex(s)
		float X = 4, Y = 4, Z = 0;
		ModelColorVertexType vertex = {0};
		CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(SquareColorVertexVector, X,Y,Z);			// Step 1: Populate SquareColorVertexVector with, all vertices positions: X, Y, Z

		float color;
		float Start = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1));	// Float between 0..1
		float End = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1));		// Float between 0..1

		for (UINT i = 0; i < SquareColorVertexVector.size(); i++)	// Step 2: Add a color to all vertices
		{															
			if ( i == 0 || i == 3 /*|| i == 4*/ )					//  |\            \|
				color = End;										//  1  v0		  1  v0 v3
			else                                                    // -1  v2 v1	 -1     v1
				color = Start;										//  x: -1  1	  x: -1  1

			SquareColorVertexVector[i].r = color;// Red
			SquareColorVertexVector[i].g = color;// Green
			SquareColorVertexVector[i].b = 0.6f;	// Blue
		}

		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stSquare3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Allocate the MODEL
		ASSERT (m_1stSquare3DColorModel->LoadColor(pContext, TEXT("m_1stSquare3DColorModel"), m_Driver, SHADER_COLOR, &SquareColorVertexVector, &IndexSquarList));	// LOAD the Model //UINT IndexSquarList[] = {0,1,2, 0,3,1};
	}
	//--------------------------------------------------------------------------------------------------------------------------
	{
		// --- DEMO-2 ---
		// Step 1: Prepare Vertex(s)
		float X = 1, Y = 1, Z = 1;
		ModelColorVertexType vertex = { 0 };
		CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(TriangleColorVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		for (UINT i = 0; i < TriangleColorVertexVector.size(); i++)	// Step 2: Add a color to all vertices
		{
			TriangleColorVertexVector[i].r = 0.0f;
			TriangleColorVertexVector[i].b = 0.0f;
			TriangleColorVertexVector[i].g = 1.0f; 	// Green
		}
		// Step 2: Create a model: NEW GLmodelClass; || NEW DXmodelClass;
		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stTriangle3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Allocate the MODEL
		ASSERT(m_1stTriangle3DColorModel->LoadColor(pContext, TEXT("m_1stTriangle3DColorModel"), m_Driver, SHADER_COLOR, &TriangleColorVertexVector, &IndexTriangleList));	// LOAD the Model //UINT IndexTriangleList[] = {0,1,2};
	}
}
//END: initColorDemo
#endif

// ----------------------------------------------------------------------------
void ApplicationClass::initTextureDemo(void *pContext)
// ----------------------------------------------------------------------------
{
	ModelTextureVertexType vertex;

#if (!defined  NO_SCENE_IMAGE_LOAD) || defined INTRO_DEMO
	{
		//DEMO-1:
		float X = 2.5f, Y = 1.5f, Z = 0;
		CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(SquareTextureVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XZtoUV(SquareTextureVertexVector, X, Y, Z);				                    // Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

		// Image Converter: \WoMA3Dengine\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x86\texconv.exe -ft PNG Earth_Diffuse.bmp

		//Line1:
		#define X_pos 4.1f
		#define Y_pos +3

	#if defined USE_IMAGE_BMP
		initLoadTexture3D(m_bmp3DModel, LEVEL22_IMAGE_bmp, SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE);
		m_bmp3DModel->rotateX(-3.14f / 2.0f);
		m_bmp3DModel->translation(-X_pos, Y_pos+8, 5);
	#endif
	#if defined USE_IMAGE_PNG
		initLoadTexture3D(m_png3DModel, LEVEL22_IMAGE_png, SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE);
		m_png3DModel->rotateX(-3.14f / 2.0f);
		m_png3DModel->translation(X_pos, Y_pos + 8, 5);
	#endif

		//Line2:
	#if defined USE_IMAGE_JPG
		initLoadTexture3D(m_jpg3DModel, LEVEL22_IMAGE_jpg, SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE); // Color: OPENGL
		m_jpg3DModel->rotateX(-3.14f / 2.0f);
		m_jpg3DModel->translation(-X_pos, Y_pos + 4.5, 5);
	#endif
	#if defined SUPPORT_TGA
		initLoadTexture3D(m_tga3DModel, LEVEL22_IMAGE_tga, SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE);
		m_tga3DModel->rotateX(-3.14f / 2.0f);
		m_tga3DModel->translation(X_pos, Y_pos + 4.5, 5);
	#endif

		//Line3:
	#if defined USE_IMAGE_DDS
		initLoadTexture3D(m_dds3DModel, LEVEL22_IMAGE_dds, SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE); // Color: OPENGL
		m_dds3DModel->rotateX(-3.14f / 2.0f);
		m_dds3DModel->translation(-X_pos, Y_pos + 1, 5);
	#endif
	#if defined USE_IMAGE_TIFF
		initLoadTexture3D(m_tif3DModel, LEVEL22_IMAGE_tif, SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE); // Inverted+Color: OPENGL
		m_tif3DModel->rotateX(-3.14f / 2.0f);
		m_tif3DModel->translation(X_pos, Y_pos + 1, 5);
	#endif
	}
#endif

#if defined SCENE_TEXTURE
	//--------------------------------------------------------------------------------------------------------------------------
	{
		//DEMO-2:
		// Step 1: Prepare Vertex(s)
		float X = 1, Y = 1, Z = 1;
		ModelTextureVertexType vertex;
		CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(TriangleTextureVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XYtoUV(TriangleTextureVertexVector, X, Y, Z);				// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

		initLoadTexture3D(m_1stTriangleTextureVertexModel, LEVEL22_DEMO_TEXTURE, TriangleTextureVertexVector, IndexTriangleList, SHADER_TEXTURE);
	}
#endif
}

// ----------------------------------------------------------------------------
void ApplicationClass::initLightDemo(void* pContext)
// ----------------------------------------------------------------------------
{
#if defined SCENE_TEXTURE_LIGHT

	//DEMO-2:
	//--------------------------------------------------------------------------------------------------------------------------
	{
		// Step 1: Prepare Vertex(s)
		float X = 1, Y = 1, Z = 1;
		ModelTextureLightVertexType vertex = { 0 };
		CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(TriangleLightVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XYtoUV(TriangleLightVertexVector, X, Y, Z);										// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

		// Calculate Normals, only once per triangle, (i.e. each 3 vertices):
		WOMA::vec3 normal; // "static": to preserve the value in all iterations
		normal = CalcNormals(&TriangleLightVertexVector[0]);

		for (UINT i = 0; i < TriangleLightVertexVector.size(); i++)							// Step 3: Add normals to all vertices
		{
			TriangleLightVertexVector[i].nx = normal.x;
			TriangleLightVertexVector[i].ny = -normal.y;
			TriangleLightVertexVector[i].nz = normal.z;
		}

		initLoadTextureLight3D(m_3th3DModel2, LEVEL22_DEMO_TEXTURE, TriangleLightVertexVector, IndexTriangleList, SHADER_TEXTURE_LIGHT);
	}
#endif
}

#if defined USE_TITLE_BANNER	//24 DX_ENGINE_LEVEL >= 24
void ApplicationClass::initStatic2D(void* ctx)
{
#if defined DX_ENGINE
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;
#else
	#define pContext ctx
#endif

	// Step 1: Prepare Vertex(s)
	std::vector<ModelTextureVertexType> SpriteVertexVector;				// 1 Declare: the Vector with Vertex "TYPE"
	ModelTextureVertexType vertex = {};									// 2 Use this "VERTEX" on macro
	CREATE_VERTEXVECTOR_SQUAD_MODEL(SpriteVertexVector, 0, 0, 0);		// 3 Initialize Vertex ARRAY at world center at first
	std::vector<UINT> emptyIndexList;									// Empty index list.

	//--------------------------------------------------------------------------------
	//CreateDXbuffers for 2D:
	#if defined USE_TITLE_BANNER
		const SHADER_TYPE shadertype = SHADER_TEXTURE;
		initModelwithTexture2D(m_titleModel, DEMO_TITLE_TEXTURE, SpriteVertexVector, emptyIndexList, shadertype);
	#endif

#if DX_ENGINE_LEVEL >= 63 && defined USE_MINI_MAP
		{
			m_MiniMapBitmapTexture = NEW DXrendertextureclass;
			//ORI: m_MiniMapTexture->Initialize(m_DirectX11, g_ScreenWidth, g_ScreenHeight))
			IF_NOT_THROW_EXCEPTION(m_MiniMapBitmapTexture->Initialize(m_Driver, 
				WOMA::AppSettings->WINDOW_WIDTH,
				WOMA::AppSettings->WINDOW_HEIGHT, 
				0, 0,
				WOMA::AppSettings->SCREEN_DEPTH, WOMA::AppSettings->SCREEN_NEAR));

			// # Mini Map #
			initModelwithTexture2D(m_miniMapBorderModel, DEMO_MINIMMAP_BORDER_TEXTURE, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE);
			initModelwithTexture2D(m_miniMapModel, DEMO_MINIMMAP_BORDER_TEXTURE, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE);
			initModelwithTexture2D(m_miniMapArrowModel, DEMO_MINIMMAP_ARROW_TEXTURE, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE);

			((DXmodelClass*)m_miniMapModel)->meshSRV11[0] = m_MiniMapBitmapTexture->m_shaderTextureResourceView;
		}
#endif

#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP 
		{
			m_RenderMapTexture = NEW DXrendertextureclass;
			//ORI: m_MapTexture->Initialize(m_DirectX11, g_ScreenWidth, g_ScreenHeight))
			IF_NOT_THROW_EXCEPTION(m_RenderMapTexture->Initialize(m_Driver, 
				1000 /*WOMA::AppSettings->WINDOW_WIDTH*/,
				1000 /*WOMA::AppSettings->WINDOW_HEIGHT*/,
				0, 30 * SystemHandle->m_scaleY,
				WOMA::AppSettings->SCREEN_DEPTH, WOMA::AppSettings->SCREEN_NEAR));

			// # Main Map #
			initModelwithTexture2DMAP(m_mainMapFrameModel, DEMO_MAINMMAP_TEXTURE, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE, 0.15f);

			//OPEN Macro we Need those: 1000
			//initModelwithTexture2D(m_mainMapModel, DEMO_MAINMMAP_TEXTURE, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE);
			{
			std::vector<STRING> Textures; 
			Textures.push_back(DEMO_MAINMMAP_TEXTURE);
			if (WOMA::AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(m_mainMapModel, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
			if (WOMA::AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION(m_mainMapModel, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
			((DXmodelClass*)m_mainMapModel)->SpriteTextureWidth = 1000;
			((DXmodelClass*)m_mainMapModel)->SpriteTextureHeight = 1000;
			ASSERT(m_mainMapModel->LoadTexture(pContext, DEMO_MAINMMAP_TEXTURE, m_Driver, SHADER_TEXTURE, &Textures, &SpriteVertexVector, &emptyIndexList, 0));
			}

			((DXmodelClass*)m_mainMapModel)->meshSRV11[0] = m_RenderMapTexture->m_shaderTextureResourceView;
		}
#endif
}
#endif

#if defined USE_CUBE
bool ApplicationClass::initCubes3D(void* pContext)
{
	//DEMO1:
	CCube cube = CCube(0,0,0);
	{
		CREATE_MODEL_IF_NOT_EXCEPTION(m_cube1Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Allocate the MODEL
		ASSERT(m_cube1Model->LoadColor(pContext, TEXT("m_cube1Model"), m_Driver, SHADER_COLOR, &cube.VertexCubeColorModel, &cube.IndexCubeList));
	}

#if NOTES
	//#define initLoadTexture3D(model, texture, vertexVector, IndexList, shader_type)
	//{
	//	std::vector<STRING> Textures;
	//	Textures.push_back(TEXT("engine/data/seafloor.dds"));
	//	if (WOMA::AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(m_cube2Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
	//	if (WOMA::AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION(m_cube2Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
	//	ASSERT(m_cube2Model->LoadTexture(TEXT("engine/data/seafloor.dds"), SystemHandle->m_Driver, SHADER_TEXTURE, &Textures, &cube.VertexCube2, &cube.IndexCubeList));
	//}
#endif
	
	{	//DEMO2:
		initLoadTexture3D(m_cube2Model, LEVEL22_DEMO_TEXTURE, cube.VertexCubeTextureModel, cube.IndexCubeList, SHADER_TEXTURE);
	}
	{	//DEMO3:
		initLoadTextureLight3D(m_cube3Model, LEVEL22_DEMO_TEXTURE, cube.VertexCubeTextureLightModel, cube.IndexCubeList, SHADER_TEXTURE_LIGHT);
	}

	return true;
}
#endif

#if defined USE_VIEW2D_SPRITES
// INIT/LOAD 2D (SPRITE or TEXT) Objects
// --------------------------------------------------------------------------------------------
bool ApplicationClass::DEMO_WOMA_APPLICATION_InitializeSprites2D(void* pContext)
// --------------------------------------------------------------------------------------------
{
	womalog("DEMO_WOMA_APPLICATION_InitializeSprites2D()\n");

#if defined USE_TITLE_BANNER
	initStatic2D(pContext);			//TITLE + 2D:MAP + 2D:MINI-MAP
#endif

	return true;
}
#endif

// --------------------------------------------------------------------------------------------
void ApplicationClass::DEMO_WOMA_APPLICATION_Shutdown2D()
// --------------------------------------------------------------------------------------------
{
#if defined USE_DX_DRIVER_FONT
	if ((DirectX::DX11Class*)driverList.size() > 0)
		((DirectX::DX11Class*)driverList[WOMA::AppSettings->DRIVER])->Shutdown2D();
#endif

	womalog("WOMA_APPLICATION_Shutdown2D()\n");

	#if (defined DX_ENGINE)
		if (WOMA::AppSettings->DRIVER != DRIVER_GL3)
		{
		#if defined USE_TITLE_BANNER // TITLE-SHUTDOWN
			SAFE_SHUTDOWN_MODELDX(m_titleModel);
		#endif
		#if defined USE_MAIN_MAP
			SAFE_SHUTDOWN_MODELDX(m_mainMapFrameModel);
			SAFE_SHUTDOWN_MODELDX(m_mainMapModel);
		#endif
		#if defined USE_MINI_MAP
			SAFE_SHUTDOWN_MODELDX(m_miniMapModel);
			SAFE_SHUTDOWN_MODELDX(m_miniMapBorderModel);
			SAFE_SHUTDOWN_MODELDX(m_miniMapArrowModel);
		#endif
		}
	#endif
	#if (defined OPENGL3 || defined OPENGL4)
		if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
		{
		#if defined USE_TITLE_BANNER // TITLE-SHUTDOWN
			SAFE_SHUTDOWN_MODELGL3(m_titleModel);
		#endif
		#if defined USE_MAIN_MAP
			SAFE_SHUTDOWN_MODELGL3(m_mainMapFrameModel);
			SAFE_SHUTDOWN_MODELGL3(m_mainMapModel);
		#endif
		#if defined USE_MINI_MAP
			SAFE_SHUTDOWN_MODELGL3(m_miniMapModel);
			SAFE_SHUTDOWN_MODELGL3(m_miniMapBorderModel);
			SAFE_SHUTDOWN_MODELGL3(m_miniMapArrowModel);
		#endif
		}
	#endif


#if defined INTRO_DEMO //29
	for (int i = 0; i < m_screenShots.size(); i++) {
		if (WOMA::AppSettings->DRIVER != DRIVER_GL3)
		{
			#if defined DX_ENGINE
			SAFE_SHUTDOWN_MODELDX(m_screenShots[i]);
			#endif
		} 
		else
		#if (defined OPENGL3 || defined OPENGL4)
		{
			SAFE_SHUTDOWN_MODELGL3(m_screenShots[i]);
		}
		#else
		{
		}
		#endif
	}

	while (!m_screenShots.empty())
		m_screenShots.pop_back();
#endif
}

#ifdef USE_RASTERTEK_TEXT_FONT

bool ApplicationClass::initText(void* pContext)
{
	_tprintf(TEXT("[%d]: initText()\n"), gettid());

	AppTextClass = NEW DirectX::ApplicationTextClass();
	IF_NOT_THROW_EXCEPTION(AppTextClass);

	switch (WOMA::AppSettings->DRIVER)
	{
#if (defined OPENGL3 || defined OPENGL40) 
	case DRIVER_GL3:
		ASSERT(AppTextClass->Initialize((ID3D11DeviceContext*)pContext, (GLopenGLclass*)m_Driver));
		break;
#endif

#if defined DX11 || defined DX9
	case DRIVER_DX9:
	case DRIVER_DX11:
		ASSERT(AppTextClass->Initialize((ID3D11DeviceContext*)pContext, (DirectX::DX11Class*)m_Driver));
		break;
#endif

#ifdef DX12
	case DRIVER_DX12:
		ASSERT(AppTextClass->Initialize((ID3D11DeviceContext*)pContext, (DirectX::DX12Class*)m_Driver));
		break;
#endif

	default:
#if defined USE_WOMA_EXCEPTION
		throw woma_exception("Not Enough Memory!", __FILE__, __FUNCTION__, __LINE__);
#else
		WomaFatalException("Not Enough Memory!");
#endif
		break;
	}

	return true;
}

#endif

#ifdef INTRO_DEMO
void ApplicationClass::initIntroCreditsDemo(void* pContext)
{
	std::vector<STRING> INTRO_TEXT;

	INTRO_TEXT.push_back(DEMO1_BANNER1_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER2_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER3_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER4_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER5_TEXTURE);
#if defined USE_DEMO29_v2
	if (WOMA::settings.WINDOW_WIDTH > (1920 + 100))
		INTRO_TEXT.push_back(DEMO1_BANNER6_TEXTURE);
	else
		INTRO_TEXT.push_back(DEMO1_BANNER6L_TEXTURE);
#else
	INTRO_TEXT.push_back(DEMO1_BANNER6_TEXTURE);
#endif

	//1way
	for (int i = 0; i < INTRO_TEXT.size(); i++)
	{
		// Step 1: Prepare Vertex(s)
		std::vector<ModelTextureVertexType> SpriteVertexVector;				// 1 Declare: the Vector with Vertex "TYPE"
		ModelTextureVertexType vertex = { };								// 2 Use this "VERTEX" on macro
		CREATE_VERTEXVECTOR_SQUAD_MODEL(SpriteVertexVector, 0, 0, 0);		// 3 Initialize Vertex ARRAY at world center at first
		std::vector<UINT> emptyIndexList;
		VirtualModelClass* m_spriteModel = NULL;
		initModelwithTexture2D(m_spriteModel, (TCHAR*)INTRO_TEXT[i].c_str(), SpriteVertexVector, emptyIndexList, SHADER_TEXTURE_FONT);

		switch (WOMA::AppSettings->DRIVER)
		{

		#if defined DX11
		case DRIVER_DX11:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader11->isFontShader = false;
			break;
		#endif

		#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader->isFontShader = false;
			break;
		#endif

		#if (defined OPENGL3 || defined OPENGL4)
		case DRIVER_GL3:
			((GLmodelClass*)m_spriteModel)->m_Shader->isFont = false;
			break;
		#endif
		}
		
		m_screenShots.push_back(m_spriteModel);

#if defined DX12

#endif
	}

}
#endif

#if (DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER) || DX_ENGINE_LEVEL == 99//FOR SHADOW
// ----------------------------------------------------------------------------
void ApplicationClass::initShadowTextureDemo(void* ctx)
// ----------------------------------------------------------------------------
{
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;

	ModelTextureVertexType vertex = { };
	float X = 4.0f, Y = 2.0f, Z = 0;
	CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(SquareTextureVertexVector, X, Y, Z);				// Step 1: Setup all vertices positions: X, Y, Z
	MAP_XZtoUV(SquareTextureVertexVector, X, Y, Z);												// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: TU, TV
	initLoadTexture3D(m_2nd3DModel, TEXT("engine/data/basics/Earth_Diffuse.bmp"), SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE);

	m_2nd3DModel->rotateX(-3.14f / 2.0f);
	m_2nd3DModel->translation(0, 2.5f, 5);
}
#endif

// ----------------------------------------------------------------------------
// WOMA_APPLICATION_FrameUpdateInstancesPositions
// ----------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
#if DX_ENGINE_LEVEL <= 42 || defined INTRO_DEMO
//CHESS BOARD OBJ       0

#define WHITE_BISHOP	1  //BISPO_BRANCO
#define WHITE_KNIGHT	2  //CAVALO_BRANCO
#define WHITE_PAWN		3  //PEAO_BRANCO
#define WHITE_QUEEN		4  //RAINHA_BRANCO
#define WHITE_KING		5  //REI_BRANCO	
#define WHITE_ROOK		6  //TORRE_BRANCO

#define BLACK_BISHOP	7  //BISPO_PRETO
#define BLACK_KNIGHT	8  //CAVALO_PRETO
#define BLACK_PAWN		9  //PEAO_PRETO
#define BLACK_QUEEN		10 //RAINHA_PRETO
#define BLACK_KING		11 //REI_PRETO	
#define BLACK_ROOK		12 //TORRE_PRETO	
#endif

void ApplicationClass::WOMA_APPLICATION_SetInstancePositions(UINT m_ObjId, int m_instanceCount, InstanceType* instances_, UINT type)
{
	InstanceType* instances = instances_;

	BillClass bill;

	switch (type)
	{
#if defined USE_INSTANCES_FOR_TREES
	case 0:
		bill.Tree0GS(instances, m_instanceCount);
		break;
#endif

	default:
		break;
	}

}
#endif

#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER && defined USE_FRUSTUM
bool ApplicationClass::WOMA_LOAD_OBJ(void* pContext, UINT threadID, UINT level, WomaDriverClass* Driver, UINT i, TCHAR* wfilename)
{
	objModel.push_back(NULL);

	if (i > 0
		&& _tcscmp(SystemHandle->xml_loader.theWorldXML[i].filename, SystemHandle->xml_loader.theWorldXML[i - 1].filename) == 0
		&& (SystemHandle->xml_loader.theWorldXML[i].type == 11			//11 animated grass, Clone it its faster
			|| SystemHandle->xml_loader.theWorldXML[i].type == 12)		//12 BUSHs, Clone it its faster
		)
	{
		objModel[i] = objModel[i - 1];
		SystemHandle->xml_loader.theWorldXML[i].CLONE = true;
	}
	else
	{

		if (SystemHandle->xml_loader.theWorldXML[i].type < 200)
		{
			CREATE_MODEL_IF_NOT_EXCEPTION(objModel[i], I_AM_2D, SystemHandle->xml_loader.theWorldXML[i].WOMA_object.castShadows, SystemHandle->xml_loader.theWorldXML[i].WOMA_object.renderShadows);
		}
		else
		{
			CREATE_MODEL_IF_NOT_EXCEPTION(objModel[i], I_AM_3D, SystemHandle->xml_loader.theWorldXML[i].WOMA_object.castShadows, SystemHandle->xml_loader.theWorldXML[i].WOMA_object.renderShadows);
		}

		objModel[i]->m_ObjId = i; //SYNC-ID: objModel[i] with: xml_loader.theWorldXML[i]
		objModel[i]->xmlId = SystemHandle->xml_loader.theWorldXML[i].id;

#if   !defined USE_SHADOW_INSTANCES
		SystemHandle->xml_loader.theWorldXML[i].WOMA_object.castShadows = false;
		SystemHandle->xml_loader.theWorldXML[i].WOMA_object.renderShadows = false;
		objModel[i]->ModelCastShadow = false;
		objModel[i]->ModelRenderShadow = false;
#else
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
		objModel[i]->ModelCastShadow = SystemHandle->xml_loader.theWorldXML[i].WOMA_object.castShadows = SystemHandle->xml_loader.theWorldXML[i].castShadow;
		objModel[i]->ModelRenderShadow = SystemHandle->xml_loader.theWorldXML[i].WOMA_object.renderShadows = SystemHandle->xml_loader.theWorldXML[i].renderShadows;
#endif
#endif
		
		SystemHandle->xml_loader.theWorldXML[i].WOMA_object.instances = SystemHandle->xml_loader.theWorldXML[i].instances;

		if (WOMA::game_state == GAME_STOP)
			return false;

#if DX_ENGINE_LEVEL >= 73 && defined BILLBOARD_FOR_WINDY_GRASS
		if (i >= world_xml_objs)
		{
			if (SystemHandle->xml_loader.theWorldXML[i].type == 11)
				((DXmodelClass*)objModel[i])->isAnimatedBill = true;
		}
#endif

		

		//Load OBJ or W3D:
		if (!(objModel[i]->LoadModel(pContext, wfilename,
			Driver,
			(SHADER_TYPE)SystemHandle->xml_loader.theWorldXML[i].shader,
			wfilename, SystemHandle->xml_loader.theWorldXML[i].WOMA_object.castShadows,
			SystemHandle->xml_loader.theWorldXML[i].WOMA_object.renderShadows, SystemHandle->xml_loader.theWorldXML[i].WOMA_object.instances)))
		{
			WomaMessageBox(wfilename, TEXT("Error Loading: "), FALSE); return false;
		}

	}

	if (i >= world_xml_objs) //Are we a Billboard?
	{
		((DXmodelClass*)objModel[i])->isBill = true;

		if (SystemHandle->xml_loader.theWorldXML[i].meshSRV) {
			if (((DXmodelClass*)objModel[i])->meshSRV11.size() > 0)
				((DXmodelClass*)objModel[i])->meshSRV11[0] = SystemHandle->xml_loader.theWorldXML[i].meshSRV;
		}
	}

#if defined ALLOW_CBIND_PROGRESS_BAR
#if defined USE_INTRO_VIDEO_DEMO
	if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
#endif
	{
		UINT progress = ((float)num_loading_objects / (float)(objModel_size + theWorld_size)) * 100.0f;
		SendMessage(SystemHandle->hwndPrgBar, PBM_SETPOS, (WPARAM)progress, 0);
#if defined USE_MINIMAP_EXPANSION
		StringCchPrintf(title, MAX_STR_LEN, TEXT("Loading: %d / %d"), num_loading_objects, objModel_size + theWorld_size+ initial_world_xml_objs);
#else
		StringCchPrintf(title, MAX_STR_LEN, TEXT("Loading: %d / %d"), num_loading_objects, objModel_size + theWorld_size);
#endif
		SetWindowText(SystemHandle->settingstext, title);
	}
#endif

	WOMA::sceneManager->addModel(WOMA::sceneManager->RootNode, objModel[i]);			// Add node to nodesList: RootNode

	return true;
}
#endif

bool ApplicationClass::InitLightandDemos(void* pContext, WomaDriverClass* Driver)
{
	//-----------------------------------------------------------------------------------------------------------------
	// INIT ASTROs (Sun Moon) /////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
#if defined USE_ASTRO_CLASS
	initWorld->Calculate(); //Calculate Real position of Sun and Moon.
#endif

#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION
	Use3DSunMoonPosition();
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// INIT LIGHT /////////////////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
	app_Light = NEW LightClass;	// Create the light object
	IF_NOT_THROW_EXCEPTION(app_Light);
	app_Light->SetAmbientColor(0.55f, 0.55f, 0.55f, 1);	//later in world.xml
	app_Light->SetDiffuseLightColor(1, 1, 1, 1.0f);		//later in world.xml
  #if defined USE_REAL_SUNLIGHT_DIRECTION
	app_Light->SetDirection(SunX / 1000, SunY / 1000, SunZ / 1000);
  #else
	app_Light->SetDirection(-0.535041273f, -1, 0);		//later in world.xml
  #endif

	//LIGHT_RAY ////////////////////////////////////////////////////////////////////////////////////////////////////
  #if defined MAIN_RENDER_LIGHT_RAY	//DO: CalculateLightRayVertex(SunDistance);							  // Calculate Light Source Position
	initLightRay(pContext);	//	  m_lightRayModel->UpdateDynamic(m_Driver, m_LightVertexVector);  // Update LightRay vertex(s)
  #endif					//	  m_lightRayModel->Render(m_Driver);							  // Render LightRay

	//-----------------------------------------------------------------------------------------------------------------
	// INIT ALL BASIC DEMOS: //////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
#if defined SCENE_COLOR
	initColorDemo(pContext);

#endif

#if (DX_ENGINE_LEVEL >= 22 && LEVEL < 60) || defined INTRO_DEMO || DX_ENGINE_LEVEL == 99// 22:TEXTURE
	initTextureDemo(pContext);

  #if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	initShadowTextureDemo(pContext);
  #endif
#endif

#if (DX_ENGINE_LEVEL >= 23  && LEVEL < 60) || defined INTRO_DEMO	// 23:LIGHT
	initLightDemo(pContext);
#endif

#if DX_ENGINE_LEVEL >= 25 && defined USE_CUBE
	IF_NOT_RETURN_FALSE(initCubes3D(pContext));
#endif


	return true;
}

void ApplicationClass::InitMainSky(void* pContext, WomaDriverClass* Driver)
{
	//=================================================================================================================
	// INIT SKY ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //=================================================================================================================
	
//Sphere:
	float size = 3.0f;
#if defined USE_SPHERE
	if (RENDER_PAGE >= 60)
		size = 25;// get_world_size();
	else
		if (RENDER_PAGE >= 55)
			size = 512;	// SYNC/CHECK AT WOMA_APPLICATION_Initialize3D():

  #if DX_ENGINE_LEVEL < 60 || defined INTRO_DEMO
	initSphere1(pContext, size);
	initSphere2(pContext, size);
  #endif
#endif

//Sky:
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE  // 28: SPHEREs
	size = 48;
	if (RENDER_PAGE >= 61)
		size = 25;// get_world_size();
	else
		if (RENDER_PAGE >= 55)
			size = 512;	// SYNC/CHECK AT WOMA_APPLICATION_Initialize3D():
		else
			size = 48;

	#ifndef MAIN_RENDER_MAIN_XML_OBJ
	size = 30;
	#endif
	ASSERT_DEBUG(size > 0);
	initSky(pContext, size);
#endif
}

void ApplicationClass::InitTerrainandWaterSurfaces(void* pContext, WomaDriverClass* Driver)
{
#if defined MAIN_RENDER_TERRAIN
	//=================================================================================================================
	// INIT TERRAINs //////////////////////////////////////////////////////////////////////////////////////////////////
	//=================================================================================================================
	//0
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN		// UNDER WATER: Terrain
	loadedTerrain[0] = NEW CTerrain(TERRAIN);
	loadedTerrain[0]->initUnderWaterDemo(pContext, 0);			//UNDERWATER	(populate: modelVertexVector) 2022:LEVEL_ENGINE: 25
#endif

	//1 WATER TERRAIN MESH: 6 vertex + 6 index
	//---------------------
#if defined SCENE_WATER_TERRAIN
	loadedTerrain[1] = NEW CTerrain(TERRAIN);
	loadedTerrain[1]->initTerrainWaterMeshDemo(pContext, 1);		//WATER			(populate: modelVertexVector)
#endif

	//2 MAIN TERRAIN MESH: 4 vertex + 6 index
	//--------------------
#if defined SCENE_MAIN_TOPO_TERRAIN	&& !defined USE_TERRAIN_ALFA_MAP
#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
	loadedTerrain[2] = NEW CTerrain(TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_TransparentTexture_MINI_MAP);				//TERRAIN_LIGHT+COLOR
#else
	loadedTerrain[2] = NEW CTerrain(TERRAIN_COLOR);				//TERRAIN_LIGHT+COLOR
#endif
	loadedTerrain[2]->initMainTopoTerrainDemo(2, (ID3D11DeviceContext*)pContext);		//TERRAIN		(populate: modelVertexVector)
#endif

	//3  COLLISION TERRAIN:6 vertex + 6 index: TO BE USED BY COLLISION TERRAIN
	//---------------------
#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX && defined SCENE_TERRAIN_COLLISION
	loadedTerrain[3] = NEW CTerrain(TERRAIN);
	loadedTerrain[3]->initMainTopoTerrainDemo(3, (ID3D11DeviceContext*)pContext);
#endif
#endif

}

void ApplicationClass::AddObjsWithInstancesToXML()
{
	//-----------------------------------------------------------------------------------------------------------------
	// Add Instanced Billboards to World.xml
	//-----------------------------------------------------------------------------------------------------------------
#if defined USE_INSTANCES_FOR_TREES
	{
		xmlobj3d XMLobj3D = {};

		XMLobj3D.id = (int)SystemHandle->xml_loader.theWorldXML.size();
		XMLobj3D.posX = 0; XMLobj3D.translateY = 0; XMLobj3D.posZ = 0;
		XMLobj3D.shader = SHADER_TEXTURE_GS_INSTANCED;
		XMLobj3D.scale = 0.0215f;
		XMLobj3D.instances = N_INSTANCE_TREES;
		XMLobj3D.type = 0;

		strcpy_s(XMLobj3D.filename, sizeof(XMLobj3D.filename), BILL_GS);
		SystemHandle->xml_loader.theWorldXML.push_back(XMLobj3D);  //Add obj as XML entry
	}
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// Add Instanced TREES90 to World.xml
	//-----------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------
	// Add Instanced LAMPs to World.xml
	//-----------------------------------------------------------------------------------------------------------------
}

// --------------------------------------------------------------------------------------------
// INIT/LOAD ALL 3D Objects
// --------------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_Initialize3D(void* pContext, WomaDriverClass* Driver, UINT level)
// --------------------------------------------------------------------------------------------
{
	womalogauto(TEXT("----------------------------------------------------------------------------------------\n"));
	womalogauto(TEXT("[%d]: WOMA_APPLICATION_Initialize3D()\n"), gettid());

	// Log XML objects:
	world_xml_objs = (UINT)SystemHandle->xml_loader.theWorldXML.size(); //15 @lvl:93
	initial_world_xml_objs = world_xml_objs;
	womalogauto("Number of objects loaded in: WORLD.XML %d\n", world_xml_objs);

	InitLightandDemos(pContext, Driver);

	#ifdef MAIN_RENDER_SKY
	InitMainSky(pContext, Driver);
	#endif

	#ifdef MAIN_RENDER_TERRAIN
	InitTerrainandWaterSurfaces(pContext, Driver);
	#endif

	//=================================================================================================================
	// Init MAIN 3D Scene       ///////////////////////////////////////////////////////////////////////////////////////
	//=================================================================================================================

	//-----------------------------------------------------------------------------------------------------------------
	// CREATE BILLBOARDs (populate Trees[]) (extra populate WORLD.XML)
	//-----------------------------------------------------------------------------------------------------------------
#if TUTORIAL_CHAP >= 60 && defined SCENE_MAIN_TOPO_TERRAIN && defined SCENE_BILLBOARDS && defined MAIN_RENDER_TERRAIN
	IF_NOT_RETURN_FALSE(m_billTreeClass = NEW BillClass);
	if (!m_billTreeClass->Initialize((ID3D11DeviceContext*)pContext, loadedTerrain[2]->m_terrainWidth / 2, loadedTerrain[2]->m_terrainHeight / 2, false))
	{
		WomaMessageBox(TEXT("Could not initialize the billboard Class"), TEXT("Create Billboard for Trees / Flowers"));
		return false;
	}
	womalogauto("Number of billboard objects added %d\n", SystemHandle->xml_loader.theWorldXML.size()- world_xml_objs);
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// LOAD PROGRESS BAR
	//-----------------------------------------------------------------------------------------------------------------
#if defined ALLOW_CBIND_PROGRESS_BAR
	// --- CREATE PROGRESS BAR:
	#if defined USE_INTRO_VIDEO_DEMO
	if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
	#endif
	{
		SystemHandle->hwndPrgBar = SystemHandle->WomaCreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 50, WOMA::AppSettings->WINDOW_HEIGHT - 100,
			WOMA::AppSettings->WINDOW_WIDTH - 100, 20, SystemHandle->m_hWnd, (HMENU)401, SystemHandle->m_hinstance, NULL);

		SendMessage(SystemHandle->hwndPrgBar, PBM_SETRANGE, 0, (LPARAM)MAKELPARAM(0, 100));
		SendMessage(SystemHandle->hwndPrgBar, PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
		SendMessage(SystemHandle->hwndPrgBar, PBM_SETBARCOLOR, 0, RGB(0, 0, 128));
		SendMessage(SystemHandle->hwndPrgBar, PBM_SETPOS, (WPARAM)(0), 0);

		::ShowWindow(SystemHandle->hwndPrgBar, 1);
	}

	// --- CREATE PROGRESS TEXT:
	SystemHandle->settingstext = SystemHandle->WomaCreateWindowEx(WS_EX_TRANSPARENT, TEXT("STATIC"), TEXT(""),
		WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER | SS_OWNERDRAW, 25, 25, 175, 22, SystemHandle->m_hWnd, 0, SystemHandle->m_hinstance, NULL);

#if defined USE_INTRO_VIDEO_DEMO
	if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
#endif
		::ShowWindow(SystemHandle->settingstext, 1);
#endif

	// Temporarily disable log file (on this loop) due performance:
	//-----------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 70 && defined USE_LOG_MANAGER //SCENE_BILLBOARDS
	WOMA::logManager_bk = WOMA::logManager;
	WOMA::logManager = 0;
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// [MAIN OBJ LOAD]: 3D-Load Scene: Create "model OBJECTS" from loaded "XML OBJECTS" in file WORLD.XML     /////////
	//-----------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER && defined USE_FRUSTUM

	// Load 3D Objects: convert XML "objects" -- Load OBJ or W3D --> VirtualModelClass:
	theWorld_size = (UINT)SystemHandle->xml_loader.theWorldXML.size();
	objModel_size = (UINT)objModel.size();
	num_loading_objects = 1;

#if defined WINDOWS_PLATFORM
	MSG msg = { 0 };
#endif
	// world_xml_objs = 15
	// theWorld_size  = 3854
	// objModel_size  = 0

	for (UINT i = objModel_size; i < objModel_size + theWorld_size; i++) //theWorld_size
	{

		// LOAD MAIN OBJECTS:
		// ---------------------------------------------------------------------------------------------------------
		WOMA_LOAD_OBJ(pContext, 0, RENDER_PAGE, Driver, i, SystemHandle->xml_loader.theWorldXML[i].filename);

		num_loading_objects++;

		// LOAD MINIMAP OBJECTS: Create 2D objects for mini-map
		// ---------------------------------------------------------------------------------------------------------

		// Allow Refresh:
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// There is any OS messages to handle?
		{
			TranslateMessage(&msg); // TranslateMessage produces WM_CHAR messages only for keys that are mapped to ASCII characters by the keyboard driver.
			DispatchMessage(&msg);  // Process MSG:  (INVOKE: WinSystemClass::MessageHandler)
		}
	}
#endif
// END MAIN LOAD: Cycle...

#if defined ALLOW_CBIND_PROGRESS_BAR
	#if defined USE_INTRO_VIDEO_DEMO
	if (DXsystemHandle->g_DShowPlayer == NULL || (DXsystemHandle->g_DShowPlayer->m_state != STATE_RUNNING))
	#endif
	{
		::ShowWindow(SystemHandle->hwndPrgBar, SW_HIDE);
		::ShowWindow(SystemHandle->settingstext, SW_HIDE);
		RedrawWindow(SystemHandle->m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);	// Invoke: Window PAINT before end.
	}
#endif

	// Restore: Temp. disabled log file:
	// ----------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 70 && defined USE_LOG_MANAGER //SCENE_BILLBOARDS
	WOMA::logManager = WOMA::logManager_bk;
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// DEMO-29                  ///////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------
	// RENDER ASTROs //////////////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------
	// SHADOWMAP //////////////////////////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------------------------------------
#if (DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER) || DX_ENGINE_LEVEL == 99
	app_Light->GenerateOrthoMatrix(15, 15, 20, 0.1f);						// Control Zoom in Shadow Map here! 15, 15

  #if defined USE_REAL_SUNLIGHT_DIRECTION || !defined MAIN_RENDER_LIGHT_RAY
	float LightX = USELIGHTSIZE * FAST_sin(initWorld->SunAzimuth);		// Real Sun Position on Sky:
	float LightZ = USELIGHTSIZE * FAST_cos(initWorld->SunAzimuth);		// Real Sun Position on Sky:
	float LightY = USELIGHTSIZE * FAST_sin(initWorld->SunElevation);	// Sun Elevation
  #else
	CalculateLightRayVertex(SunDistance);
	float LightX = MyLightVertexVector[1].x;
	float LightY = MyLightVertexVector[1].y;
	float LightZ = MyLightVertexVector[1].z;
  #endif
	app_Light->GenerateViewMatrix(LightX, LightY, LightZ);

	m_RenderShadowTexture = NEW DXrendertextureclass;
	const int SHADOWMAP_WIDTH = WOMA::AppSettings->MaxTextureSize;  //2048;
	const int SHADOWMAP_HEIGHT = WOMA::AppSettings->MaxTextureSize; //2048; 
	IF_NOT_RETURN_FALSE(m_RenderShadowTexture->Initialize(Driver, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, 0, WOMA::AppSettings->SCREEN_DEPTH, WOMA::AppSettings->SCREEN_NEAR));

	// --------------------------------------------------------------------------------------------
	// Optionally: DEBUG SPRITE Model: (m_2nd3DModel) Render Shadows on a texture:
	// --------------------------------------------------------------------------------------------
#endif

#if defined SAVEW3D && DX_ENGINE_LEVEL < 89
	WomaMessageBox(TEXT("Conversion from OBJ to W3D, ended."), TEXT("SAVEW3D"));
	WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
	return false;
#endif

	return true;
}

#pragma warning(pop)