// --------------------------------------------------------------------------------------------
// Filename: SceneManager.h
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
#pragma once

#include "WomaDriverClass.h"
#include "SceneNode.h"
#include "QuadTree.h"
#include "virtualModelClass.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void Shutdown();
	void ReleaseNode(SceneNode* node);

	static SceneManager* GetInstance();

	void Render();
	void addModel (SceneNode* node, VirtualModelClass* model);
	void CreateRootNode(UINT worldSizeDiameter, float* color);

	SceneNode* RootNode;

	std::vector<VirtualModelClass*> opacModelList;
	std::vector<VirtualModelClass*> shadowModelList;

	//private:
	QuadTree quadTree;
};

namespace WOMA
{
	extern SceneManager* sceneManager;
}