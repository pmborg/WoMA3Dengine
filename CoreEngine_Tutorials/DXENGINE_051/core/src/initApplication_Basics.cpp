// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: initApplication_Basics.cpp
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
//WomaIntegrityCheck = 1234567222;

#include "main.h"
#include "ApplicationClass.h"
#include "OSengine.h"
#include "Math3D.h"
#include "mem_leak.h"
#include "log.h"
#include <cinttypes>

#pragma warning(push)
#pragma warning(disable : 4002) // warning C4002: too many arguments for function-like macro invocation 'CREATE_MODELGL3_IF_NOT_EXCEPTION'
#pragma warning(disable : 4244) // warning C4244: '=': conversion from 'int' to 'float', possible

#ifdef OPENGL3
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

#if defined SCENE_COLOR
// ----------------------------------------------------------------------------
void ApplicationClass::initColorDemo()
// ----------------------------------------------------------------------------
{
	//if (RENDER_PAGE == 21 || RENDER_PAGE == 22 || RENDER_PAGE == 23 || RENDER_PAGE == 24 || FORCE_RENDER_ALL)
	{
		//DEMO-1:
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

		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stSquare3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		ASSERT (m_1stSquare3DColorModel->LoadColor(TEXT("m_1stSquare3DColorModel"), m_Driver, SHADER_COLOR, &SquareColorVertexVector, &IndexSquarList));	// LOAD the Model //UINT IndexSquarList[] = {0,1,2, 0,3,1};
	}
	//--------------------------------------------------------------------------------------------------------------------------
	//if (RENDER_PAGE >= 21 || FORCE_RENDER_ALL)
	{
		//DEMO-2:
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
		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stTriangle3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		ASSERT(m_1stTriangle3DColorModel->LoadColor(TEXT("m_1stTriangle3DColorModel"), m_Driver, SHADER_COLOR, &TriangleColorVertexVector, &IndexTriangleList));	// LOAD the Model //UINT IndexTriangleList[] = {0,1,2};
	}
}
//END: initColorDemo
#endif

// ----------------------------------------------------------------------------
void ApplicationClass::initTextureDemo()
// ----------------------------------------------------------------------------
{
	ModelTextureVertexType vertex;

#if !defined  NO_SCENE_IMAGE_LOAD
	//if (RENDER_PAGE == 22 || RENDER_PAGE == 28 || FORCE_RENDER_ALL)
	{
		//DEMO-1:
		float X = 2.5f, Y = 1.5f, Z = 0;
		CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(SquareTextureVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XZtoUV(SquareTextureVertexVector, X, Y, Z);				// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

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
	//if ((RENDER_PAGE >= 22 && RENDER_PAGE < 24) || FORCE_RENDER_ALL)
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
void ApplicationClass::initLightDemo()
// ----------------------------------------------------------------------------
{
#if defined SCENE_TEXTURE_LIGHT
/*
	//DEMO-1:
	//if ((RENDER_PAGE >= 23 && RENDER_PAGE <= 25) || FORCE_RENDER_ALL)
	{
		ModelTextureLightVertexType vertex = { 0 };
		float X = 2.0f, Y = 1.0f, Z = 0;
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
			CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(My3thModelVertexVector1, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		}
		else {
			CREATE_VERTEXVECTOR_SQUAD_MODEL(My3thModelVertexVector1, X, Y, Z);
		}
		MAP_XZtoUV(My3thModelVertexVector1, X, Y, Z);								// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv			

		// Calculate Normals, only once per triangle, (i.e. each 3 vertices):
		vec3 normal; // "static": to preserve the value in all iterations
		normal = CalcNormals(&My3thModelVertexVector1[0]);

		for (UINT i = 0; i < My3thModelVertexVector1.size(); i++)					// Step 3: Add normals to all vertices
		{
			My3thModelVertexVector1[i].nx = normal.x;
			My3thModelVertexVector1[i].ny = -normal.y;
			My3thModelVertexVector1[i].nz = normal.z;
		}

		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
			initLoadTextureLight3D(m_3th3DModel1, TEXT("engine/data/original/Earth_Diffuse.bmp"), My3thModelVertexVector1, IndexSquarList, SHADER_TEXTURE_LIGHT);
		}
		else {
			std::vector<UINT> empty;
			initLoadTextureLight3D(m_3th3DModel1, TEXT("engine/data/original/Earth_Diffuse.bmp"), My3thModelVertexVector1, empty, SHADER_TEXTURE_LIGHT);
		}

		m_3th3DModel1->scale(3, 3, 3);
	}
*/
	//DEMO-2:

	//--------------------------------------------------------------------------------------------------------------------------
	//if ((RENDER_PAGE >= 23 && RENDER_PAGE < 24) || FORCE_RENDER_ALL)
	{
		// Step 1: Prepare Vertex(s)
		float X = 1, Y = 1, Z = 1;
		ModelTextureLightVertexType vertex = { 0 };
		CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(TriangleLightVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XYtoUV(TriangleLightVertexVector, X, Y, Z);										// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

		// Calculate Normals, only once per triangle, (i.e. each 3 vertices):
		vec3 normal; // "static": to preserve the value in all iterations
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
void ApplicationClass::initStatic2D()
{
		// Step 1: Prepare Vertex(s)
		std::vector<ModelTextureVertexType> SpriteVertexVector;				// 1 Declare: the Vector with Vertex "TYPE"
		ModelTextureVertexType vertex = {};									// 2 Use this "VERTEX" on macro
		CREATE_VERTEXVECTOR_SQUAD_MODEL(SpriteVertexVector, 0, 0, 0);		// 3 Initialize Vertex ARRAY at world center at first
		std::vector<UINT> emptyIndexList;									// Empty index list.

	//--------------------------------------------------------------------------------
	//CreateDXbuffers for 2D:
	#if defined USE_TITLE_BANNER
		// # Title #
		initModelwithTexture2D(m_titleModel, DEMO_TITLE_TEXTURE, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE);
	#endif

	#if defined USE_IMGUI	
		//Settings Icon:
		initModelwithTexture2D(m_iconSettings, DEMO_SETTINGS_ICON, SpriteVertexVector, emptyIndexList, SHADER_TEXTURE);
	#endif
	
}
#endif

#if defined USE_CUBE
bool ApplicationClass::initCubes3D()
{
	//DEMO1:
	CCube cube = CCube(0,0,0);
	{
		CREATE_MODEL_IF_NOT_EXCEPTION(m_cube1Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
		ASSERT(m_cube1Model->LoadColor(TEXT("m_cube1Model"), m_Driver, SHADER_COLOR, &cube.VertexCubeColorModel, &cube.IndexCubeList));
	}

	//DEMO2:
	{
		initLoadTexture3D(m_cube2Model, LEVEL22_DEMO_TEXTURE, cube.VertexCubeTextureModel, cube.IndexCubeList, SHADER_TEXTURE);
		#if NOTES
		//#define initLoadTexture3D(model, texture, vertexVector, IndexList, shader_type)
		//{
		//	std::vector<STRING> Textures;
		//	Textures.push_back(TEXT("engine/data/seafloor.dds"));
		//	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) { CREATE_MODELGL3_IF_NOT_EXCEPTION(m_cube2Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
		//	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) { CREATE_MODELDX_IF_NOT_EXCEPTION(m_cube2Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS); }
		//	ASSERT(m_cube2Model->LoadTexture(TEXT("engine/data/seafloor.dds"), SystemHandle->m_Driver, SHADER_TEXTURE, &Textures, &cube.VertexCube2, &cube.IndexCubeList));
		//}
		#endif
	}
	//DEMO3:
	{
		initLoadTextureLight3D(m_cube3Model, LEVEL22_DEMO_TEXTURE, cube.VertexCubeTextureLightModel, cube.IndexCubeList, SHADER_TEXTURE_LIGHT);
	}

	return true;
}
#endif

#if defined USE_VIEW2D_SPRITES
// INIT/LOAD 2D (SPRITE or TEXT) Objects
// --------------------------------------------------------------------------------------------
bool ApplicationClass::DEMO_WOMA_APPLICATION_InitializeSprites2D()
// --------------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("DEMO_WOMA_APPLICATION_InitializeSprites2D()\n");

#if defined USE_TITLE_BANNER
	initStatic2D();			//TITLE + MAP + MINI-MAP
#endif

	return true;
}
#endif

// --------------------------------------------------------------------------------------------
void ApplicationClass::DEMO_WOMA_APPLICATION_Shutdown2D()
// --------------------------------------------------------------------------------------------
{
#if defined USE_DX10DRIVER_FONTS
	if ((DirectX::DX11Class*)/*SystemHandle->*/driverList.size() > 0)
		((DirectX::DX11Class*)/*SystemHandle->*/driverList[SystemHandle->AppSettings->DRIVER])->Shutdown2D();
#endif

	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown2D()\n");

	#if (defined DX_ENGINE)
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
		#if defined USE_TITLE_BANNER // TITLE-SHUTDOWN
			SAFE_SHUTDOWN_MODELDX(m_titleModel);
		#endif
		#if defined USE_IMGUI
			SAFE_SHUTDOWN_MODELDX(m_iconSettings);
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
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
		#if defined USE_TITLE_BANNER // TITLE-SHUTDOWN
			SAFE_SHUTDOWN_MODELGL3(m_titleModel);
		#endif
		#if defined USE_IMGUI
			SAFE_SHUTDOWN_MODELGL3(m_iconSettings);
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
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
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

bool ApplicationClass::initText()
{
	_tprintf(TEXT("[%d]: initText()\n"), gettid());

	AppTextClass = NEW DirectX::ApplicationTextClass();
	IF_NOT_THROW_EXCEPTION(AppTextClass);

	switch (SystemHandle->AppSettings->DRIVER)
	{
#ifdef OPENGL3
	case DRIVER_GL3:
		ASSERT(AppTextClass->Initialize((GLopenGLclass*)m_Driver));
		break;
#endif

#if defined DX11 || defined DX9
	case DRIVER_DX9:
	case DRIVER_DX11:
		ASSERT(AppTextClass->Initialize((DirectX::DX11Class*)m_Driver));
		break;
#endif

#ifdef DX12
	case DRIVER_DX12:
		ASSERT(AppTextClass->Initialize((DirectX::DX12Class*)m_Driver));
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
void ApplicationClass::initIntroDemo()
{
	std::vector<STRING> INTRO_TEXT;

	INTRO_TEXT.push_back(DEMO1_BANNER1_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER2_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER3_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER4_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER5_TEXTURE);
	INTRO_TEXT.push_back(DEMO1_BANNER6_TEXTURE);

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

		switch (SystemHandle->AppSettings->DRIVER)
		{
		#if defined DX9sdk
		case DRIVER_DX9:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader9->isFontShader = false;
			break;
		#endif

		#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX9:
			((DirectX::DXmodelClass*)m_spriteModel)->m_Shader11->isFontShader = false;
			break;
		#endif

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

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER //FOR SHADOW
// ----------------------------------------------------------------------------
void ApplicationClass::initShadowTextureDemo()
// ----------------------------------------------------------------------------
{
	ModelTextureVertexType vertex = { };
	float X = 4.0f, Y = 2.0f, Z = 0;
	CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(SquareTextureVertexVector, X, Y, Z);				// Step 1: Setup all vertices positions: X, Y, Z
	MAP_XZtoUV(SquareTextureVertexVector, X, Y, Z);												// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv
	initLoadTexture3D(m_2nd3DModel, TEXT("engine/data/basics/Earth_Diffuse.bmp"), SquareTextureVertexVector, IndexSquarList, SHADER_TEXTURE);

	m_2nd3DModel->rotateX(-3.14f / 2.0f);
	m_2nd3DModel->translation(0, 2.5f, 5);
}
#endif

// ----------------------------------------------------------------------------
// WOMA_APPLICATION_FrameUpdateInstancesPositions
// ----------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
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

void ApplicationClass::WOMA_APPLICATION_FrameUpdateInstancesPositions(UINT m_ObjId, int m_instanceCount, InstanceType* instances_)
{
	InstanceType* instances = instances_;
	
	for (int i = 0; i < m_instanceCount; i++)
	{
		switch (m_ObjId)
		{
		case BLACK_QUEEN:
		case WHITE_QUEEN:
			instances[i].position.x = (float)i+3;
			instances[i].position.z = (float)-2;
			instances[i].position.y = (float)0.1f;
			break;
		case BLACK_KING:
		case WHITE_KING:
			instances[i].position.x = (float)i+3;
			instances[i].position.z = (float)-2;
			instances[i].position.y = (float)0.1f;
			break;
		case BLACK_BISHOP:
		case WHITE_BISHOP:
			if (i == 0) {
				instances[i].position.x = (float)-2 + i;
				instances[i].position.z = (float)-2;
				instances[i].position.y = (float)0.1f;
			}
			else {
				instances[i].position.x = (float) + i;
				instances[i].position.z = (float)-2;
				instances[i].position.y = (float)0.1f;
			}
			break;
		case BLACK_KNIGHT:
		case WHITE_KNIGHT:
			if (i == 0) {
				instances[i].position.x = (float)i-2;
				instances[i].position.z = (float)-2;
				instances[i].position.y = (float)0.1f;
			}
			else {
				instances[i].position.x = (float)2 + i;
				instances[i].position.z = (float)-2;
				instances[i].position.y = (float)0.1f;
			}
			break;
		case BLACK_PAWN:
		case WHITE_PAWN:
			instances[i].position.x = (float)-2 + i;
			instances[i].position.z = (float)-3;
			instances[i].position.y = (float)0.1f;
			break;
		case BLACK_ROOK:
		case WHITE_ROOK:
			if (i == 0) {
				instances[i].position.x = (float)i + 1;
				instances[i].position.z = (float)-2;
				instances[i].position.y = (float)0.1f;
			}
			else {
				instances[i].position.x = (float)7 + i;
				instances[i].position.z = (float)-2;
				instances[i].position.y = (float)0.1f;
			}
			break;
		}

		if (m_ObjId == BLACK_PAWN)
			instances[i].position.z += (float)2;

		instances[i].position.z = instances[i].position.z + (float)2;
		instances[i].position.x = instances[i].position.x - (float)1;
	}

}
#endif

// --------------------------------------------------------------------------------------------
// INIT/LOAD 3D Objects
// --------------------------------------------------------------------------------------------
bool ApplicationClass::WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver)
// --------------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSGAUTO(TEXT("----------------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO(TEXT("[%d]: WOMA_APPLICATION_Initialize3D()\n"), gettid());

	//ASTRO ////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined USE_ASTRO_CLASS
	initWorld->Calculate();
#endif

#if defined USE_ASTRO_CLASS && defined USE_REAL_SUNLIGHT_DIRECTION
	Calc3DSunMoonPosition();
	//if (WOMA::game_state == GAME_STOP) return false;
#endif

	//LIGHT ////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_Light = NEW LightClass;	// Create the light object
	IF_NOT_THROW_EXCEPTION(m_Light);
	m_Light->SetAmbientColor(0.55f, 0.55f, 0.55f, 1);	//later in world.xml
	m_Light->SetDiffuseColor(1, 1, 1, 1.0f);			//later in world.xml
#if defined USE_REAL_SUNLIGHT_DIRECTION
	m_Light->SetDirection(SunX / 1000, SunY / 1000, SunZ / 1000);
#else
	m_Light->SetDirection(-0.535041273f, -1, 0);		//later in world.xml
#endif

	//LIGHT_RAY /////////////////////// DO: ///////////////////////////////////////////////////////////////////////////
#if defined USE_LIGHT_RAY	//	CalculateLightRayVertex(SunDistance);							// Calculate Light Source Position
	initLightRay();			//	m_lightRayModel->UpdateDynamic(m_Driver, m_LightVertexVector);	// Update LightRay vertex(s)
#endif						//	m_lightRayModel->Render(m_Driver);								// Render LightRay

	//-----------------------------------------------------------------------------------------------------------------
	// INIT ALL BASIC DEMOS:
	//-----------------------------------------------------------------------------------------------------------------
#if defined SCENE_COLOR
	initColorDemo();

#endif

#if DX_ENGINE_LEVEL >= 22 && LEVEL < 60	// 22:TEXTURE
	initTextureDemo();

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	initShadowTextureDemo();
#endif

#endif

#if DX_ENGINE_LEVEL >= 23  && LEVEL < 60	// 23:LIGHT
	initLightDemo();

#endif

	//Sphere+SKY:
#if DX_ENGINE_LEVEL >= 25 && defined USE_CUBE
	IF_NOT_RETURN_FALSE(initCubes3D());
#endif

//Sphere:
	float size = 3.0f;
#if defined USE_SPHERE
	if (RENDER_PAGE >= 61)
		size = SystemHandle->world.skySize;
	else
		if (RENDER_PAGE >= 55)
			size = 512;	// SYNC/CHECK AT WOMA_APPLICATION_Initialize3D():

	initSphere1(size);
	initSphere2(size);
#endif

//Sky:
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE  // 28: SPHEREs
	size = 48;
	if (RENDER_PAGE >= 61)
		size = SystemHandle->world.skySize;
	else
		if (RENDER_PAGE >= 55)
			size = 512;	// SYNC/CHECK AT WOMA_APPLICATION_Initialize3D():
		else
			size = 48;

	initSky(size);
#endif

	//TERRAINs /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//0
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN		// UNDER WATER: Terrain
	loadedTerrain[0] = NEW CTerrain(TERRAIN);
	loadedTerrain[0]->initUnderWaterDemo(0);			//UNDERWATER	(populate: modelVertexVector) 2022:LEVEL_ENGINE: 25
#endif

	//1 WATER TERRAIN MESH: 6 vertex + 6 index
#if defined SCENE_WATER_TERRAIN
	loadedTerrain[1] = NEW CTerrain(TERRAIN);
	loadedTerrain[1]->initTerrainWaterMeshDemo(1);		//WATER			(populate: modelVertexVector)
#endif

	//2 MAIN TERRAIN MESH: 4 vertex + 6 index
#if defined SCENE_MAIN_TOPO_TERRAIN	&& !defined USE_TERRAIN_ALFA_MAP
	loadedTerrain[2] = NEW CTerrain(TERRAIN);
	loadedTerrain[2]->initMainTopoTerrainDemo(2);		//TERRAIN		(populate: modelVertexVector)
#endif

	//3 TERRAIN:6 vertex + 6 index: TO BE USED BY COLLISION TERRAIN
#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX && defined SCENE_TERRAIN_COLLISION
	loadedTerrain[3] = NEW CTerrain(TERRAIN);
	loadedTerrain[3]->initMainTopoTerrainDemo(3);
#endif

	world_xml_objs = (UINT)SystemHandle->xml_loader.theWorld.size();
	//-----------------------------------------------------------------------------------------	
	// Create Bill Board for Trees / Flowers
	//-----------------------------------------------------------------------------------------	
#if TUTORIAL_CHAP >= 60 && defined (SCENE_MAIN_TOPO_TERRAIN) && defined (SCENE_BILLBOARDS) // BILLBOARD
	IF_NOT_RETURN_FALSE(m_billTreeClass = NEW BillClass);
	if (!m_billTreeClass->Initialize(loadedTerrain[2]->m_terrainWidth/2, loadedTerrain[2]->m_terrainHeight/2, false))
	{
		WomaMessageBox(TEXT("Could not initialize the billboardClass"), TEXT("Create Bill Board for Trees / Flowers"));
		return false;
	}
#endif

	//-----------------------------------------------------------------------------------------------------------------
	// Create "model OBJECTS" from loaded "XML OBJECTS" in file WORLD.XML
	//-----------------------------------------------------------------------------------------------------------------

#if DX_ENGINE_LEVEL >= 30 && defined USE_SCENE_MANAGER && defined USE_FRUSTRUM

	// Load 3D Objects: convert XML "objects" -- Load OBJ or W3D --> VirtualModelClass:
	UINT len = (UINT)SystemHandle->xml_loader.theWorld.size();
	UINT objModel_size = (UINT)objModel.size();
	for (UINT i = objModel_size; i < objModel_size+len; i++)
	{
		objModel.push_back(NULL);
		CREATE_MODEL_IF_NOT_EXCEPTION(objModel[i], I_AM_3D, SystemHandle->xml_loader.theWorld[i].WOMA_object.castShadows, SystemHandle->xml_loader.theWorld[i].WOMA_object.renderShadows);
		objModel[i]->m_ObjId = i; //SYNC-ID: objModel[i] with: xml_loader.theWorld[i]

#if   !defined USE_SHADOW_INSTANCES
		SystemHandle->xml_loader.theWorld[i].WOMA_object.castShadows = false;
		SystemHandle->xml_loader.theWorld[i].WOMA_object.renderShadows = false;
		objModel[i]->ModelCastShadow = false;
		objModel[i]->ModelRenderShadow = false;
#else
	#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
		objModel[i]->ModelCastShadow = SystemHandle->xml_loader.theWorld[i].WOMA_object.castShadows = SystemHandle->xml_loader.theWorld[i].castShadow;
		objModel[i]->ModelRenderShadow = SystemHandle->xml_loader.theWorld[i].WOMA_object.renderShadows = SystemHandle->xml_loader.theWorld[i].renderShadows;
	#endif
#endif
		TCHAR wfilename[MAX_STR_LEN] = { 0 }; atow(wfilename, SystemHandle->xml_loader.theWorld[i].filename, MAX_STR_LEN);

		if (!(objModel[i]->LoadModel(wfilename, Driver, 
			(SHADER_TYPE)SystemHandle->xml_loader.theWorld[i].shader,
			wfilename,
			SystemHandle->xml_loader.theWorld[i].WOMA_object.castShadows, SystemHandle->xml_loader.theWorld[i].WOMA_object.renderShadows,
			SystemHandle->xml_loader.theWorld[i].WOMA_object.instances)))
		{
			WomaMessageBox(wfilename, TEXT("Error Loading: "), FALSE); return false;
		}
		{
			RedrawWindow(SystemHandle->m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);	// Invoke: Window PAINT before end.
		}

		//Allow Refresh on Timer:
		MSG msg = { 0 };
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// There is any OS messages to handle?
		{
			TranslateMessage(&msg); // TranslateMessage produces WM_CHAR messages only for keys that are mapped to ASCII characters by the keyboard driver.
			DispatchMessage(&msg);  // Process Msg:  (INVOKE: WinSystemClass::MessageHandler)
		}
		
		WOMA::sceneManager->addModel(WOMA::sceneManager->RootNode, objModel[i]);			// Add node to nodesList: RootNode

		WOMA::num_loading_objects++;
	}
#endif

	//ANIMATED SKELETON MESHs //////////////////////////////////////////////////////////////////////////////////////////////////////////

	//RENDER ASTROs ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//SHADOWMAP //////////////////////////////////////////////////////////////////////////////////////////////////////
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	m_Light->GenerateOrthoMatrix(15, 15, 20, 0.1f);						// Control Zoom in Shadow Map here! 15, 15

#if defined USE_REAL_SUNLIGHT_DIRECTION
	float LightX = USELIGHTSIZE * FAST_sin(initWorld->SunAzimuth);		// Real Sun Position on Sky:
	float LightZ = USELIGHTSIZE * FAST_cos(initWorld->SunAzimuth);		// Real Sun Position on Sky:
	float LightY = USELIGHTSIZE * FAST_sin(initWorld->SunElevation);	// Sun Elevation
#else
	CalculateLightRayVertex(SunDistance);
	float LightX = MyLightVertexVector[1].x;
	float LightY = MyLightVertexVector[1].y;
	float LightZ = MyLightVertexVector[1].z;
#endif
	m_Light->GenerateViewMatrix(LightX, LightY, LightZ);

	m_RenderShadowTexture = NEW DXrendertextureclass;
	const int SHADOWMAP_WIDTH = SystemHandle->AppSettings->MaxTextureSize;  //2048;
	const int SHADOWMAP_HEIGHT = SystemHandle->AppSettings->MaxTextureSize; //2048; 
	IF_NOT_RETURN_FALSE(m_RenderShadowTexture->Initialize(Driver, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, 0, SystemHandle->AppSettings->SCREEN_DEPTH, SystemHandle->AppSettings->SCREEN_NEAR));

	// DEBUG SPRITE: Render Shadows on a texture:
	// --------------------------------------------------------------------------------------------
#endif




	//Finally, launch dynamic Load Compound/OBJ Thread /////////////////////////////////////////////////////////////////////////////////
#if defined (CHECK_COMPOUND_COLISION) && defined (SCENE_COMPOUND) //TUTORIAL_CHAP >= 55 && 
	for (UINT i = 0; i < N_COMPOUNDS; i++) {
		compoundTreeLoadingOrder[i].compoundTreeId = i;
		compoundTreeLoadingOrder[i].order = 0;
	}

#if TUTORIAL_CHAP < 95
	CompoundReadFunction(Driver);
#else
	// Create a thread to load our compounds:
	threadCompoundLoaderAlive = true;
	threadCompoundLoaderHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CompoundReadFunction, (void*)this, 0, &threadCompoundLoaderId);
	if (!threadCompoundLoaderHandle) { return false; }
	if (!SetThreadPriority(threadCompoundLoaderHandle, THREAD_PRIORITY_IDLE/*THREAD_PRIORITY_LOWEST*//*THREAD_PRIORITY_BELOW_NORMAL*/)) { return false; }
#endif
#endif

#if defined SAVEW3D
	WomaMessageBox(TEXT("Conversion from OBJ to W3D, ended."), TEXT("SAVEW3D"));
	WOMA::main_loop_state = -1; //WOMA::game_state = GAME_STOP; //Publish_Quit_Message();
	return false;
#endif

	//NETWORK //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

#pragma warning(pop)