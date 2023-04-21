// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: SceneManager.cpp
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

// PURPOSE: The AudioClass is the Interface Audio to Client EXE 
//			 No need to Link audio LIBs on client EXE
//
/*
[SceneManger].create
-------------------------------
(winSystemClass.cpp)
CreateRootNode (worldSize, clearColor)
	SceneNode* worldNode = NEW SceneNode(0,0,0,worldSizeDiameter);
	worldNode->AddStateClearColor (color);
	quadTree.Initialize(RootNode);

[VirtualModelClass]->Load
rotate		(if needed)
scale		(if needed)
translation	(if needed)

(Sample*.cpp)
addModel (sceneManager->RootNode, m_1st3DModel);

[NODE] SceneNode.h
-------------------------------
NodeState:
	-clearColor
	-Virtual model
	-State:	shader extra params
			-light(s)
			-fog
			...
-list: Child nodes(s)

[QuadTree]
-------------------------------
-NodeType
	(list: NODEs)

[SceneManger].render
-------------------------------
QuadTree + Camera = RenderGraph(s)
Render (RenderGraph[4])
- RenderThread
- RenderThread
- RenderThread
- RenderThread

-------------------------------------------------------------------------------------------------------------------
A spatial graph, which is the spatial view of the world (for grouping nearby objects together, for culling).
	The QuadTree
	The OcTree		http://www.beyond3d.com/images/articles/ingenu-part-2/octree.png
	The BSP tree	http://www.beyond3d.com/images/articles/ingenu-part-2/bsp.png

	Sectors and portals
	ABT: the Adaptive Binary Tree is somewhat similar to the BSP, except this one is updated real-time

A scene tree, which is the hierarchical view of the world (for handling hierarchical animation).

A render graph, which is the state optimal view of the world (for grouping objects by states, such as shaders, materials, etc).
*/
//------------------------------------------------------------------------------------------------------------------ -
#include "platform.h"

#include "ApplicationClass.h"
#include "OSengine.h"		// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "SceneManager.h"
#include "mem_leak.h"

SceneManager::SceneManager()
{
	CLASSLOADER();

	RootNode = NULL;
}

SceneManager::~SceneManager() { Shutdown(); CLASSDELETE(); }

void SceneManager::Shutdown()
{
	quadTree.Shutdown();

	//Shutdown calls the ReleaseNode function which recursively traces down the tree and removes all the nodes.
	if(RootNode)
	{
		ReleaseNode(RootNode);
		delete RootNode;
		RootNode = 0;
	}
}

//The ReleaseNode function is used for releasing all the nodes in the quad tree as well as the data inside each node. The function is recursive and will call itself to traverse the entire node tree.
void SceneManager::ReleaseNode(SceneNode* node)
{
	UINT i;

	// Recursively go down the tree and release the bottom nodes first.
	for(i=0; i<node->listChildNodes.size(); i++)
	{
		if(node->listChildNodes[i] != 0)
			ReleaseNode(node->listChildNodes[i]);
	}

	// Release the child nodes of this node
	for(i=0; i<node->listChildNodes.size(); i++)
	{
		if(node->listChildNodes[i])
		{
			delete node->listChildNodes[i];
			node->listChildNodes[i] = 0;
		}
	}
}

SceneManager* SceneManager::GetInstance()
{
	if (!WOMA::sceneManager)
		WOMA::sceneManager = NEW SceneManager (/*Driver*/);

	return WOMA::sceneManager;
}

void SceneManager::Render()
{
#ifdef _DEBUG
	quadTree.totalVertexRendered = 0;
	quadTree.totalRendered = 0;
#endif	

	quadTree.RenderNode(quadTree.m_QuadRootNode);
}

//Default Original World Size: -512,-512, 512, 512
void SceneManager::CreateRootNode(UINT worldSizeDiameter, float* color)
{
	//         |
	//     ---0,0--- (THE BIG WORLD SQUARE: worldSizeDiameter)
	//         |
	RootNode = NEW SceneNode(0,0,0,(float)worldSizeDiameter);
	RootNode->AddStateClearColor (color);
}

void SceneManager::addModel (SceneNode* node, VirtualModelClass* model)
{
	// Create SceneNode
	SceneNode* newNode = NEW SceneNode(model->PosX, model->PosY, model->PosZ, model->boundingSphere);

	// addModel to SceneNode
	newNode->AddStateModel (model);

	// Add SceneNode to the father's list:
	node->listChildNodes.push_back (newNode);

	// Add SceneNode to QuadTree
	quadTree.AddSceneNode (quadTree.m_QuadRootNode, newNode);
}

