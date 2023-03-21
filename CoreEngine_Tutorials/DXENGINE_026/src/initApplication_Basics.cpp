// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
#include "main.h"
#include "ApplicationClass.h"
#include "winSystemClass.h"
#include "mem_leak.h"

	#include "GLmodelClass.h"

	#include "DXmodelClass.h"

#include "WomaCube.h"

#include "DemoApplicationClass.h"

DemoApplicationClass::DemoApplicationClass()
{
	CLASSLOADER();

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	m_1stTriangle3DColorModel = NULL;					// Model2

		m_2nd3DModel = NULL;						// Model

};

DemoApplicationClass::~DemoApplicationClass()
{
	Shutdown();
	//CLASSDELETE();
};

bool DemoApplicationClass::WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver)
{
	//INIT ALL
	//-----------------------------------------------------------------------------------------------------------------
	initColorDemo(SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);

	initTextureDemo();

	initLightDemo();

	if (RENDER_PAGE >= 24)
		initTitleBanner2D();

	if (RENDER_PAGE >= 25)
		IF_NOT_RETURN_FALSE(initCubes3D(SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]));

#if DX_ENGINE_LEVEL >= 26 && defined USE_SPHERE  // SPHEREs
	initSphere1(3);
	initSphere2(3);
#endif

	return true;
}

// ----------------------------------------------------------------------------
void DemoApplicationClass::initColorDemo(WomaDriverClass* m_Driver)
// ----------------------------------------------------------------------------
{
	if (RENDER_PAGE == 21)
	{
		//DEMO-1:
		// Step 1: Prepare Vertex(s)
		float X = 4, Y = 4, Z = 0;
		ModelColorVertexType vertex = {0};
		CREATE_VERTEXVECTOR_SQUAR_MODEL_OPTIMIZED(SquarColorVertexVector, X,Y,Z);	// Step 1: Populate SquarColorVertexVector with, all vertices positions: X, Y, Z

		float color;
		float Start = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1));	// Float between 0..1
		float End = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1));		// Float between 0..1

		for (UINT i = 0; i < SquarColorVertexVector.size(); i++)	// Step 2: Add a color to all vertices
		{															
			if ( i == 0 || i == 3 /*|| i == 4*/ )					//  |\            \|
				color = End;										//  1  v0		  1  v0 v3
			else                                                    // -1  v2 v1	 -1     v1
				color = Start;										//  x: -1  1	  x: -1  1

			SquarColorVertexVector[i].r = color;// Red
			SquarColorVertexVector[i].g = color;// Green
			SquarColorVertexVector[i].b = 0.6f;	// Blue
		}

		CREATE_MODEL_IF_NOT_EXCEPTION(m_1stSquar3DColorModel, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
	
		//DEMO-2:
		ASSERT (m_1stSquar3DColorModel->LoadColor(TEXT("m_1stSquar3DColorModel"), m_Driver, SHADER_COLOR, &SquarColorVertexVector, &IndexSquarList));	// LOAD the Model //UINT IndexSquarList[] = {0,1,2, 0,3,1};
		m_1stSquar3DColorModel->translation(0,-1,4); //move UP:4 and towards camera: 1
	}
	//--------------------------------------------------------------------------------------------------------------------------
	if (RENDER_PAGE >= 21)
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
			
		if (RENDER_PAGE == 21)
			m_1stTriangle3DColorModel->translation(0, 3.5, 1);	//
		else
			m_1stTriangle3DColorModel->translation(-7, 5.5, 1);
	}
}
//END: initColorDemo

// ----------------------------------------------------------------------------
void DemoApplicationClass::initTextureDemo()
// ----------------------------------------------------------------------------
{
	if (RENDER_PAGE == 22)
	{
		//DEMO-1:
		ModelTextureVertexType vertex = { 0 };
		float X = 2.5f, Y = 1.5f, Z = 0;
		CREATE_VERTEXVECTOR_SQUAR_MODEL_OPTIMIZED(SquarTextureVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XZtoUV(SquarTextureVertexVector, X, Y, Z);				// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

		//ORIGNAL IMAGE:
		initLoadTexture3D(m_bmp3DModel, TEXT("engine/data/Earth_Diffuse.bmp"), SquarTextureVertexVector, IndexSquarList, SHADER_TEXTURE);
		m_bmp3DModel->rotateX(-3.14f / 2.0f);
		m_bmp3DModel->translation(-6, 7.5, 7);

		// Image Converter: \WoMA3Dengine\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x86\texconv.exe -ft JPG Earth_Diffuse.bmp
		initLoadTexture3D(m_jpg3DModel, TEXT("engine/data/Earth_Diffuse.jpg"), SquarTextureVertexVector, IndexSquarList, SHADER_TEXTURE); // Color: OPENGL
		m_jpg3DModel->rotateX(-3.14f / 2.0f);
		m_jpg3DModel->translation(0, 7.5, 7);

		// Image Converter: \WoMA3Dengine\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x86\texconv.exe - ft PNG Earth_Diffuse.bmp
		initLoadTexture3D(m_png3DModel, TEXT("engine/data/Earth_Diffuse.png"), SquarTextureVertexVector, IndexSquarList, SHADER_TEXTURE);
		m_png3DModel->rotateX(-3.14f / 2.0f);
		m_png3DModel->translation(6, 7.5, 7);

		// Image Converter USED: %windir%\system32\mspaint.exe
		initLoadTexture3D(m_tif3DModel, TEXT("engine/data/Earth_Diffuse.tif"), SquarTextureVertexVector, IndexSquarList, SHADER_TEXTURE); // Inverted+Color: OPENGL
		m_tif3DModel->rotateX(-3.14f / 2.0f);
		m_tif3DModel->translation(-6, 1, 7);

		// Image Converter USED: \WoMA3Dengine\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\DxTex.exe
		initLoadTexture3D(m_dds3DModel, TEXT("engine/data/Earth_Diffuse.dds"), SquarTextureVertexVector, IndexSquarList, SHADER_TEXTURE); // Color: OPENGL
		m_dds3DModel->rotateX(-3.14f / 2.0f);
		m_dds3DModel->translation(0, 1, 7);

		// Image Converter: \WoMA3Dengine\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x86\texconv.exe - ft TGA Earth_Diffuse.bmp
	#if defined SUPPORT_TGA
		initLoadTexture3D(m_tga3DModel, TEXT("engine/data/Earth_Diffuse.tga"), SquarTextureVertexVector, IndexSquarList, SHADER_TEXTURE);
		m_tga3DModel->rotateX(-3.14f / 2.0f);
		m_tga3DModel->translation(6, 1, 7);
	#endif
	}
	//--------------------------------------------------------------------------------------------------------------------------
	if (RENDER_PAGE >= 22)
	{
		//DEMO-2:
		// Step 1: Prepare Vertex(s)
		float X = 1, Y = 1, Z = 1;
		ModelTextureVertexType vertex = { 0 };
		CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(TriangleTextureVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XYtoUV(TriangleTextureVertexVector, X, Y, Z);				// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

		initLoadTexture3D(m_1stTriangleTextureVertexModel, TEXT("engine/data/seafloor.dds"), TriangleTextureVertexVector, IndexTriangleList, SHADER_TEXTURE);

		if (RENDER_PAGE == 22)
			m_1stTriangleTextureVertexModel->translation(0, 3.5, 1);
		else
			m_1stTriangleTextureVertexModel->translation(-7, 3.5, 1);

	}

}

// ----------------------------------------------------------------------------
void DemoApplicationClass::initLightDemo()
// ----------------------------------------------------------------------------
{
	//DEMO-1:
	if (RENDER_PAGE >= 23 && RENDER_PAGE <= 25)
	{
		ModelTextureLightVertexType vertex = { 0 };
		float X = 2.0f, Y = 1.0f, Z = 0;
		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
			CREATE_VERTEXVECTOR_SQUAR_MODEL_OPTIMIZED(My3thModelVertexVector1, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		}
		else {
			CREATE_VERTEXVECTOR_SQUAR_MODEL(My3thModelVertexVector1, X, Y, Z);
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
			initLoadTextureLight3D(m_3th3DModel1, TEXT("engine/data/Earth_Diffuse.bmp"), My3thModelVertexVector1, IndexSquarList, SHADER_TEXTURE_LIGHT);
		}
		else {
			std::vector<UINT> empty;
			initLoadTextureLight3D(m_3th3DModel1, TEXT("engine/data/Earth_Diffuse.bmp"), My3thModelVertexVector1, empty, SHADER_TEXTURE_LIGHT);
		}

		m_3th3DModel1->scale(3, 3, 3);
	}

	//DEMO-2:
	//--------------------------------------------------------------------------------------------------------------------------
	if (RENDER_PAGE >= 23 && RENDER_PAGE <= 25)
	{
		// Step 1: Prepare Vertex(s)
		float X = 1, Y = 1, Z = 1;
		ModelTextureLightVertexType vertex = { 0 };
		CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(TriangleLightVertexVector, X, Y, Z);	// Step 1: Setup all vertices positions: X, Y, Z
		MAP_XYtoUV(TriangleLightVertexVector, X, Y, Z);										// Step 2: ADD TEXTURING: Auto-Map textures to all vertices: tu, tv

		initLoadTextureLight3D(m_3th3DModel2, TEXT("engine/data/seafloor.dds"), TriangleLightVertexVector, IndexTriangleList, SHADER_TEXTURE_LIGHT);

		if (RENDER_PAGE == 23)
		{
			m_3th3DModel2->translation(0, 4.75f, 1);
			m_3th3DModel2->scale(1.75f, 1.75f, 1.75f);
		}
		else {
			m_3th3DModel2->translation(-7, 1.5f, 1);
		}
	}
}

void DemoApplicationClass::initTitleBanner2D()
{
	if (RENDER_PAGE >= 24)
	{
		// Step 1: Prepare Vertex(s)
		std::vector<ModelTextureVertexType> TitleVertexVector;				// 1 Declare: the Vector with Vertex "TYPE"
		ModelTextureVertexType vertex = { 0 };								// 2 Use this "VERTEX" on macro
		CREATE_VERTEXVECTOR_SQUAR_MODEL(TitleVertexVector, 0, 0, 0);		// 3 Initialize Vertex ARRAY at world center at first
		std::vector<UINT> empty;
		initLoadTexture2D(m_titleModel, TEXT("engine/data/WOMA_ENGINE.png"), TitleVertexVector, empty/*IndexSquarList*/, SHADER_TEXTURE);
	}
}

bool DemoApplicationClass::initCubes3D(WomaDriverClass* m_Driver)
{
	if (RENDER_PAGE == 25)
	{

		//DEMO1:
		CCube cube = CCube(0,0,0);
		{
			CREATE_MODEL_IF_NOT_EXCEPTION(m_cube1Model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);	// Alocate the MODEL
			ASSERT(m_cube1Model->LoadColor(TEXT("m_cube1Model"), m_Driver, SHADER_COLOR, &cube.VertexCubeColorModel, &cube.IndexCubeList));
		}

		//DEMO2:
		{
			initLoadTexture3D(m_cube2Model, TEXT("engine/data/seafloor.dds"), cube.VertexCubeTextureModel, cube.IndexCubeList, SHADER_TEXTURE);
		}
		//DEMO3:
		{
			initLoadTextureLight3D(m_cube3Model, TEXT("engine/data/seafloor.dds"), cube.VertexCubeTextureLightModel, cube.IndexCubeList, SHADER_TEXTURE_LIGHT);
		}
	}

	return true;
}

// INIT/LOAD 2D (SPRITE or TEXT) Objects
// --------------------------------------------------------------------------------------------
bool DemoApplicationClass::WOMA_APPLICATION_InitializeSprites2D()
// --------------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_InitializeSprites2D()\n");

	return true;
}

void DemoApplicationClass::Shutdown()
{

	//3D:

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
	//#if DX_ENGINE_LEVEL == 21
		SAFE_SHUTDOWN_MODELGL3(m_1stSquar3DColorModel);			//DEMO1:
	//#endif
		SAFE_SHUTDOWN_MODELGL3(m_1stTriangle3DColorModel);		//DEMO2:

	//#if DX_ENGINE_LEVEL != 22
		SAFE_SHUTDOWN_MODELGL3(m_2nd3DModel);
	//#else
		SAFE_SHUTDOWN_MODELGL3(m_bmp3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_jpg3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_png3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_tif3DModel);		//DEMO1:
		SAFE_SHUTDOWN_MODELGL3(m_dds3DModel);		//DEMO1:
	#if defined SUPPORT_TGA
		SAFE_SHUTDOWN_MODELGL3(m_tga3DModel);		//DEMO1:
	#endif

		SAFE_SHUTDOWN_MODELGL3(m_1stTriangleTextureVertexModel);
	}
	else
	//#endif
	{
	//#if DX_ENGINE_LEVEL == 21
		SAFE_SHUTDOWN_MODELDX(m_1stSquar3DColorModel);
	//#endif
		SAFE_SHUTDOWN_MODELDX(m_1stTriangle3DColorModel);
	//#if DX_ENGINE_LEVEL != 22
		SAFE_SHUTDOWN_MODELDX(m_2nd3DModel);
	//#else

		SAFE_SHUTDOWN_MODELDX(m_bmp3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_jpg3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_png3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_tif3DModel);	//DEMO1:
		SAFE_SHUTDOWN_MODELDX(m_dds3DModel);	//DEMO1:
	#if defined SUPPORT_TGA
		SAFE_SHUTDOWN_MODELDX(m_tga3DModel);	//DEMO1:
	#endif

		SAFE_SHUTDOWN_MODELDX(m_1stTriangleTextureVertexModel);
	//#endif
	}


	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELDX(m_3th3DModel1);
		SAFE_SHUTDOWN_MODELDX(m_3th3DModel2);
	}

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_3th3DModel1);
		SAFE_SHUTDOWN_MODELGL3(m_3th3DModel2);
	}

	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELDX(m_cube1Model);
		SAFE_SHUTDOWN_MODELDX(m_cube2Model);
		SAFE_SHUTDOWN_MODELDX(m_cube3Model);
#if defined USE_SPHERE
		SAFE_SHUTDOWN_MODELDX(m_SphereModel1);
		SAFE_SHUTDOWN_MODELDX(m_SphereModel2);
#endif
	}

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		SAFE_SHUTDOWN_MODELGL3(m_cube1Model);
		SAFE_SHUTDOWN_MODELGL3(m_cube2Model);
		SAFE_SHUTDOWN_MODELGL3(m_cube3Model);
#if defined USE_SPHERE
		SAFE_SHUTDOWN_MODELGL3(m_SphereModel1);
		SAFE_SHUTDOWN_MODELGL3(m_SphereModel2);
#endif
	}

		//2D:
		WOMA_APPLICATION_Shutdown2D();

}

// --------------------------------------------------------------------------------------------
void DemoApplicationClass::WOMA_APPLICATION_Shutdown2D()
// --------------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown2D()\n");

		if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		{
			SAFE_SHUTDOWN_MODELDX(m_titleModel);
		}
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
			SAFE_SHUTDOWN_MODELGL3(m_titleModel);
		}

}

