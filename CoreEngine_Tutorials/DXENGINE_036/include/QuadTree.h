// --------------------------------------------------------------------------------------------
// Filename: QuadTree.h
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

#include "main.h"
#include "SceneNode.h"
#include "DXFrustumClass.h"

#define MAX_NODE_SIZE DXsystemHandle->world.patchSize

//Each node in the quad tree will be defined as follows with position, size, triangle count, buffers, and four child nodes:
typedef struct Node_Type
{
	// Reset Childs by default
	Node_Type()
	{
		nodes[0]=nodes[1]=nodes[2]=nodes[3]=0;
		positionX = positionZ = width = 0;
	}

	// Node Location Data:
	float positionX=0, positionZ = 0, width = 0;
	std::vector<SceneNode*> sceneNodes;	// SceneNodes on this QuadNode

	// Childs:
	Node_Type* nodes[4];
} NodeType;


class QuadTree
{
public:
	QuadTree();
	~QuadTree();

	void Shutdown();
	void ReleaseNode(NodeType* node);

	void Initialize(SceneNode* worldRootNode);
	void CreateTreeNode(NodeType* node, float positionX, float positionZ, float width);
	void AddSceneNode(NodeType* node, SceneNode* sceneNode);

	void RenderNode(NodeType* m_parentNode);

	NodeType* m_QuadRootNode = NULL;

#ifdef _DEBUG
	UINT totalLoaded=0, totalRendered = 0, totalVertexRendered = 0;
#endif

private:
	DXfrustumClass* frustum = NULL;
	WomaDriverClass* driver = NULL;
};
