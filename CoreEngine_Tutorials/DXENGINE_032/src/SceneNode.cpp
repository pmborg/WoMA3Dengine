// --------------------------------------------------------------------------------------------
// Filename: SceneNode.cpp
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
//WomaIntegrityCheck = 1234567831;

#include "platform.h"

#include "SceneNode.h"

SceneNode::SceneNode(float posX, float posY, float posZ, float boundingSphere)
{
	CLASSLOADER();

	positionX = posX;
	positionY= posY;
	positionZ = posZ;
	width = boundingSphere;

	#if _DEBUG
	WOMA_LOGManager_DebugMSG("SceneManager::Node: X,Y,Z - DIM: %d %d %d - %d\n", (int)positionX, (int)positionY, (int)positionZ, (int)width);
	#endif
}

SceneNode::~SceneNode() { CLASSDELETE(); }

void SceneNode::AddStateClearColor(float* clearColor)
{
	nodeState.ClearColor[0] = *clearColor++;
	nodeState.ClearColor[1] = *clearColor++;
	nodeState.ClearColor[2] = *clearColor++;
	nodeState.ClearColor[3] = *clearColor;

	NodeStates.push_back (ClearColorType);
}

void SceneNode::AddStateModel(VirtualModelClass* model)
{
	nodeState.model = model;			//populate data
	NodeStates.push_back (ModelType);	//signal that data exist!
}

