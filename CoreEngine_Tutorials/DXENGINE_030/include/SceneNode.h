// --------------------------------------------------------------------------------------------
// Filename: SceneNode.h
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
//WomaIntegrityCheck = 1234567830;
#pragma once
#include "main.h"
#include "virtualModelClass.h"



enum NodeStateTypes
{
	ClearColorType = 0,
	ModelType,
};

typedef struct
{
	float ClearColor[4];		//0
	VirtualModelClass* model;	//1

} NodeState;

class SceneNode
{
public:
	SceneNode(float posX, float posY, float posZ, float boundingSphere);
	~SceneNode();

	void AddStateClearColor(float* ClearColor);
	void AddStateModel(VirtualModelClass* model);

	//Node:
	float positionX, positionY, positionZ, width;	// Real Position
	NodeState nodeState;							// Node States: Possible State Values
	std::vector<NodeStateTypes> NodeStates;			// Node States: Used

	//Childs:
	std::vector<SceneNode*> listChildNodes;
	

private:

};

