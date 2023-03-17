// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
#include "main.h"
#include "ApplicationClass.h"
#include "winSystemClass.h"
#include "mem_leak.h"

		#include "GLmodelClass.h"

		#include "DXmodelClass.h"

#include "DemoApplicationClass.h"

DemoApplicationClass::DemoApplicationClass()
{
	CLASSLOADER();

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	m_1stSquar3DColorModel = NULL;						// Model1
	m_1stTriangle3DColorModel = NULL;					// Model2

};

DemoApplicationClass::~DemoApplicationClass()
{
	Shutdown();
	//CLASSDELETE();
};

//DemoApplicationClass* demoApplicationClass;

bool DemoApplicationClass::WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver)
{
	//INIT ALL
	//-----------------------------------------------------------------------------------------------------------------
	initColorDemo(SystemHandle->driverList[SystemHandle->AppSettings->DRIVER]);

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

void DemoApplicationClass::Shutdown()
{

	//3D:

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
	//#if DX_ENGINE_LEVEL == 21
		SAFE_SHUTDOWN_MODELGL3(m_1stSquar3DColorModel);			//DEMO1:
	//#endif
		SAFE_SHUTDOWN_MODELGL3(m_1stTriangle3DColorModel);		//DEMO2:

	}
	else
	//#endif
	{
	//#if DX_ENGINE_LEVEL == 21
		SAFE_SHUTDOWN_MODELDX(m_1stSquar3DColorModel);
	//#endif
		SAFE_SHUTDOWN_MODELDX(m_1stTriangle3DColorModel);
	}


		//2D:

}

