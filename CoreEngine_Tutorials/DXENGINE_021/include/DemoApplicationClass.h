// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DemoApplicationClass.h
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
#pragma once

#include "ApplicationClass.h"

class DemoApplicationClass : public ApplicationClass
{
public:
	DemoApplicationClass();
	~DemoApplicationClass();

	void DemoRender();
	void DemoPosRender();
	void Shutdown();

	//3D
	bool WOMA_APPLICATION_Initialize3D(WomaDriverClass* Driver); // APP_Load

	// 2D

	void initColorDemo(WomaDriverClass* Driver);

	//	-------------------------------------------------------------------------------------------
	//	WoMA Vertex(s) Arrays:  NOTE: Cant be used to create and Obj more than ONCE!
	//	-------------------------------------------------------------------------------------------
	//DEMO-1:
	std::vector<ModelColorVertexType> SquarColorVertexVector;	// COLOR-DEMO-1: CREATE_VERTEXVECTOR_SQUAR_MODEL_OPTIMIZED
	VirtualModelClass* m_1stSquar3DColorModel = NULL;			// COLOR-DEMO-1: CREATE_MODELDX_IF_NOT_EXCEPTION

	//DEMO-2:
	std::vector<ModelColorVertexType> TriangleColorVertexVector;// COLOR-DEMO-2: CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED
	VirtualModelClass* m_1stTriangle3DColorModel = NULL;		// COLOR-DEMO-2: CREATE_MODELDX_IF_NOT_EXCEPTION

	// TERRAIN

};