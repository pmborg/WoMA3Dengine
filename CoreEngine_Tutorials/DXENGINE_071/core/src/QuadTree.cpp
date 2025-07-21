// --------------------------------------------------------------------------------------------
// Filename: QuadTree.cpp
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
//WomaIntegrityCheck = 1234525217;

#include "platform.h"

#if defined USE_SCENE_MANAGER
#include "ApplicationClass.h"
#include "OSengine.h"		// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "mem_leak.h"
#include "QuadTree.h"
#include "SceneManager.h"
#include "xml_loader.h"
#include "DXmodelClass.h"

QuadTree::QuadTree() 
{
	CLASSLOADER();
#if defined USE_TINYXML_LOADER //5
    _xml_loader = &SystemHandle->xml_loader;
#endif
	// public:
	m_QuadRootNode = NULL;

	// private:
	_frustum = NULL;

#ifdef _DEBUG
	totalLoaded = totalVertexRendered = 0;
#endif
}

QuadTree::~QuadTree() { Shutdown(); CLASSDELETE(); }

void QuadTree::Shutdown()
{
	//Shutdown calls the ReleaseNode function which recursively traces down the tree and removes all the nodes.
	if(m_QuadRootNode)
	{
		ReleaseNode(m_QuadRootNode);
		SAFE_DELETE (m_QuadRootNode);
	}
}

//The ReleaseNode function is used for releasing all the nodes in the quad tree as well as the data inside each node. The function is recursive and will call itself to traverse the entire node tree.
void QuadTree::ReleaseNode(NodeType* node)
{
	UINT i;

	// Recursively go down the tree and release the bottom nodes first.
	for(i=0; i<4; i++)
	{
		if(node->nodes[i] != 0)
			ReleaseNode(node->nodes[i]);
	}

	// Release the four child nodes.
	for(i=0; i<4; i++)
	{
		if(node->nodes[i])
			SAFE_DELETE (node->nodes[i]);
	}
}

void QuadTree::Initialize(SceneNode* worldRootNode)
{
	_frustum = m_Driver->frustum;	// Save: Application Frustum

	m_QuadRootNode = NEW NodeType();			// Create QUAD-ROOT Node:

    float width = (float)worldRootNode->width;
	float centerX = width/2-1;
	float centerZ = width/2-1;

	CreateTreeNode(m_QuadRootNode, centerX, centerZ,  width);
}

void QuadTree::CreateTreeNode(NodeType* node, float positionX, float positionZ, float width)
{
	node->positionX = positionX;
	node->positionZ = positionZ;
	node->width = width;

#if defined yes_please_debug_me && defined _DEBUG
	womalog("SceneManager::QuadTree::CreateTreeNode(): X: %f Y: %f width: %f\n", positionX, positionZ, width);
#endif

	// Create Childs if needed:
	for(UINT i=0; i<4; i++)
	{
		// Calculate the position offsets for the new child node.
		float offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
		float offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

		if (width > MAX_NODE_SIZE)
		{
			// If there are triangles inside where this new node would be then create the child node.
			node->nodes[i] = NEW NodeType;

			// Extend the tree starting from this new child node now.
			CreateTreeNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
		}
	}
}

bool checkIfPointIsInsideSquare (float p_x, float p_z, float square_left, float square_right, float square_top, float square_bottom)
{
	return  ((p_x >= square_left && p_x <= square_right) && (p_z >= square_bottom && p_z <= square_top));
}


//Used by: void SceneManager::addModel (SceneNode* node, VirtualModelClass* model)
void QuadTree::AddSceneNode(NodeType* quadNode, SceneNode* node)
{
	// Check if the SceneNode (point) is inside a Quad:
	float square_left	= quadNode->positionX - quadNode->width/2+1;
	float square_right	= quadNode->positionX + quadNode->width/2+1;
	float square_top	= quadNode->positionZ + quadNode->width/2+1;
	float square_bottom	= quadNode->positionZ - quadNode->width/2+1;

	bool result = checkIfPointIsInsideSquare (node->positionX, node->positionZ, 
										square_left, square_right, square_top, square_bottom);
	if (!result) return;

	// If it can be seen then check all four child nodes to see if they can also be seen.
	int count = 0;
	for(UINT i=0; i<4; i++)
	{
		if(quadNode->nodes[i] != NULL)
		{
			count++;
		    AddSceneNode(quadNode->nodes[i], node);
		}
	}

	// If there were any children nodes then there is no need to continue as parent nodes won't contain anything to render.
	if (count != 0) return;

	// Add Scene Node
	quadNode->sceneNodes.push_back (node);
#ifdef _DEBUG
	totalLoaded++;
#endif
}

//void SceneManager::Render()
void QuadTree::RenderNode(NodeType* node)
{
#if !defined USE_MAP_EDITOR
	// Check to see if the node can be viewed, height doesn't matter in a quad tree.
	//bool result = frustum->CheckCube(node->positionX, 0.0f, node->positionZ, node->width/2);   // More accurated but slower
	bool result = _frustum->CheckSphere(node->positionX, 0.0f, node->positionZ, (node->width/2)*1.4142135623730950488016887242097f );   // Faster
	if (!result) return;
#endif

	// If it can be seen then check all four child nodes to see if they can also be seen.
	int count = 0;
	for(UINT i=0; i<4; i++)
	{
		if(node->nodes[i] != 0)
		{
			count++;
		    RenderNode(node->nodes[i]);
		}
	}

	// If there were any children nodes then there is no need to continue as parent nodes won't contain any triangles to render.
	if (count != 0) return;

	// Not really Render! But List All Models/objects to render, on this Node: (this quad is in front of camera)
    UINT world_xml_objs = (UINT)_xml_loader->theWorld.size(); //Get 
    VirtualModelClass* model;
	for (int i = 0; i < node->sceneNodes.size(); i++)
	{
		 model = node->sceneNodes[i]->nodeState.model;
        
         UINT modelID = model->m_ObjId;

         if (SystemHandle->xml_loader.theWorld[modelID].depend == -1)
         {
             _xml_loader->theWorld[modelID].render = true;      //FASTER-AQUI2
         }else{
#if !defined USE_MAP_EDITOR
            float positionX, positionY, positionZ;
            positionX = _xml_loader->theWorld[modelID].posX;
            positionY = _xml_loader->theWorld[modelID].translateY;
            positionZ = _xml_loader->theWorld[modelID].posZ;
            if ((((DXmodelClass*)model)->m_instanceCount == 0) && !_frustum->CheckSphere(positionX, positionY, positionZ, model->boundingSphere*2)) {
                _xml_loader->theWorld[modelID].render = false;  //FASTER-AQUI2
                continue;
            } else
#endif
               _xml_loader->theWorld[modelID].render = true;    //FASTER-AQUI2
        }
		//This Model have transparent parts?, note it! to render transparent parts later.
        if (model->ModelHASAlfaColor)
            SystemHandle->m_Application->billboardRrenderCount++;
		else
        WOMA::sceneManager->opacModelList.push_back(model);

	#ifdef _DEBUG
		totalVertexRendered += node->sceneNodes[i]->nodeState.model->m_vertexCount;
	#endif
	}
}
#endif
