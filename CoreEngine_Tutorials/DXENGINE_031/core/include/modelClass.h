// --------------------------------------------------------------------------------------------
// Filename: ModelClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once
#ifndef modelclass_h
#define modelclass_h

#pragma warning(push)
#pragma warning(disable : 5208) //warning C5208: unnamed class used in typedef name cannot declare members other than non-static data members, member enumerations, or member classes

#include "platform.h"
#include "womadriverclass.h"
#include "vertexTypes.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
public:
	ADVOBJ3D obj3d;
	
	ModelClass();
	~ModelClass();

	// Populate: obj3d
	bool LoadOBJ(void* pContext, VirtualModelClass* dxmodelClass, SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow = false, bool renderShadow = false, UINT instanceCount = 0, UINT instanceType = 0);

	// Create: DXmodelClass based on obj3D (an advanced model mesh)
	bool CreateObject(void* pContext, VirtualModelClass* dxmodelClass, TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow, bool renderShadow);

	// OBJ Loader aux vars:
	// --------------------
	TCHAR	checkChar=0;			//The variable we will use to store one char from file at a time
	STRING	face=TEXT("");				//Holds the string containing our face vertices's
	int		vIndex = 0;				//Keep track of our vertex index count
	
	STRING	meshMaterialsTemp = TEXT("");	//Temp variables to store into vectors
	int		vertPosIndexTemp = 0;
	int		vertNormIndexTemp = 0;
	int		vertTCIndexTemp = 0;
	STRING	meshMatLib;				//String to hold our obj material library filename
};

#pragma warning(pop)
#endif
