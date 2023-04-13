// --------------------------------------------------------------------------------------------
// Filename: QuadTree.cpp
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

#include "platform.h"

#include "ApplicationClass.h"
#include "OSengine.h"		// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "QuadTree.h"
#include "mem_leak.h"
#include "xml_loader.h"
#include "DXmodelClass.h"

QuadTree::QuadTree() 
{
	CLASSLOADER();

	// public:
	m_QuadRootNode = NULL;

	// private:
	frustum = NULL;
	driver = NULL;

#ifdef _DEBUG
	totalLoaded = totalRendered = totalVertexRendered = 0;
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
	frustum = SystemHandle->m_Driver->frustum;	// Save: Application Frustum
	driver = SystemHandle->m_Driver;			// Save: System Virtual Driver

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

#ifdef _DEBUG
	WOMA_LOGManager_DebugMSG("QuadTree::CreateTreeNode(): X: %f Y: %f width: %f\n", positionX, positionZ, width);
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

bool checkIfPointIsInsideSquare (float p_x, float p_z, float squar_left, float squar_right, float squar_top, float squar_bottom)
{
	return  ((p_x >= squar_left && p_x <= squar_right) && (p_z >= squar_bottom && p_z <= squar_top));
}

void QuadTree::AddSceneNode(NodeType* quadNode, SceneNode* node)
{
	// Check if the SceneNode (point) is inside a Quad:
	float squar_left	= quadNode->positionX - quadNode->width/2+1;
	float squar_right	= quadNode->positionX + quadNode->width/2+1;
	float squar_top		= quadNode->positionZ + quadNode->width/2+1;
	float squar_bottom	= quadNode->positionZ - quadNode->width/2+1;

	bool result = checkIfPointIsInsideSquare (node->positionX, node->positionZ, 
										squar_left, squar_right, squar_top, squar_bottom);
	if (!result) return;

	// If it can be seen then check all four child nodes to see if they can also be seen.
	int count = 0;
	for(UINT i=0; i<4; i++)
	{
		if(quadNode->nodes[i] != 0)
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

void QuadTree::RenderNode(NodeType* node)
{
	// Check to see if the node can be viewed, height doesn't matter in a quad tree.
	//result = frustum->CheckCube(node->positionX, 0.0f, node->positionZ, node->width/2);   // More accurated but slower
	bool result = frustum->CheckSphere(node->positionX, 0.0f, node->positionZ, (node->width/2)*1.4142135623730950488016887242097f );   // Faster: node->width/2
	if (!result) return;

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

	// Render All objects on this Node: (this quad is in front of camera)
	for (int i = 0; i < node->sceneNodes.size(); i++)
	{
		VirtualModelClass* model = node->sceneNodes[i]->nodeState.model;

		UINT posX = SystemHandle->xml_loader.theWorld[model->m_ObjId].posX;
		UINT posY = SystemHandle->xml_loader.theWorld[model->m_ObjId].translateY;
		UINT posZ = SystemHandle->xml_loader.theWorld[model->m_ObjId].posZ;

		UINT scale = SystemHandle->xml_loader.theWorld[model->m_ObjId].scale;
		if (scale == 0) 
			scale = 1;

		((DXmodelClass*)model)->m_worldMatrix = XMMatrixIdentity();
		float rx = SystemHandle->xml_loader.theWorld[model->m_ObjId].rotX;
		if (rx == -1000) {
			float rX = 0.0f;
			rX = SystemHandle->m_Application->dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
			model->rotateX(rX);
		}
		else
			model->rotateX(rx);

		float ry = SystemHandle->xml_loader.theWorld[model->m_ObjId].rotY;
		if (ry == -1000) {
			float rY = 0.0f;
			rY = SystemHandle->m_Application->dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
			model->rotateY(rY);
		}
		else
			model->rotateY(ry);

		float rz = SystemHandle->xml_loader.theWorld[model->m_ObjId].rotZ;
		if (rz == -1000) {
			float rZ = 0.0f;
			rZ = SystemHandle->m_Application->dt * (0.005f / 16.66f);	// MOVIMENT FORMULA!
			model->rotateZ(rZ);
		}
		else
			model->rotateZ(rz);

		model->translation(posX, posY, posZ);
		model->scale(scale, scale, scale);

		model->Render(driver);

	#ifdef _DEBUG
		totalRendered++;
		totalVertexRendered += node->sceneNodes[i]->nodeState.model->m_vertexCount;
	#endif
	}
}
