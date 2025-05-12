// --------------------------------------------------------------------------------------------
// Filename: BillClass.cpp
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#include "OSengine.h"

#include "BillClass.h"
#include "ApplicationClass.h"
#include "mem_leak.h"

#if TUTORIAL_CHAP >= 60  && defined SCENE_BILLBOARDS // BILLBOARD
BillClass* m_billTreeClass = NULL;
#endif

#define borderLimit 13 // Border Limit without Bills

BillClass::BillClass()
{
    CLASSLOADER();

    billNames_length = 0;
    BillrenderCount = 0;
    billTotal = 0;

    mainTerrainPath = NEW CTerrain(TERRAIN);
    mainTerrainPath->LoadHeightMapTerrain(BILLBOARD_TERRAIN, 0, 0); //engine/data/scene73grass/t_025TerrainMappingV4.bmp
}

BillClass::~BillClass()
{
    Shutdown();
    CLASSDELETE();
}

void BillClass::Shutdown()
{
    SAFE_DELETE(mainTerrainPath);
    return;
}

#if defined SCENE_BILLBOARDS
#include "fileLoader.h"

#ifdef OPENGL3
#include "GLopenGLclass.h"
#include "GLmodelClass.h"
#endif

#if defined DX_ENGINE
#include "DXmodelClass.h"
#endif

Tree			m_Trees[N_BILLBOARD + N_FENCES + N_FIRE + N_GRASS+ N_BUSH];	// Array of tree info. NOTE: SHARED on 2 (BILLBOARD Instances)



ID3D11ShaderResourceView* billFileLoaded[] = 
{
	NULL,//0
	NULL,//1
	NULL,//2
	NULL,//3
	NULL,//4
	NULL,//5

	NULL,//6
	NULL,//7
	NULL,//8
	NULL,//9 
	NULL,//10

	NULL,//11

	NULL,//12
};

TCHAR billFileName[][MAX_STR_LEN] = 
{
	//TREEs: 6
	BILL_TREE_0,	//0 
	BILL_TREE_1,	//1
	BILL_TREE_2,	//2
	BILL_TREE_3,	//3
	BILL_TREE_4,	//4
	BILL_TREE_5,	//5

	//FLOWERs: 5
	BILL_FLOWER_0,	//6	
	BILL_FLOWER_1,	//7
	BILL_FLOWER_2,	//8
	BILL_FLOWER_3,	//9 
	BILL_FLOWER_4,	//10
	// GRASS
	BILL_GRASS, //11
	BILL_BUSH_0, //12
};



xmlobj3d* BillClass::fillxml(int id, UINT type)
{
	DirectX::DX11Class* m_driver11 = (DirectX::DX11Class*)m_Driver;

	static xmlobj3d xmlobj;
	xmlobj.id = id+ SystemHandle->m_Application->world_xml_objs;
	xmlobj.type = type;
	xmlobj.fromPage = 0;
	xmlobj.toPage = 0;
	xmlobj.depend = 0;
	xmlobj.scale = m_Trees[id].scale;
	xmlobj.posX = m_Trees[id].vPos.x;
	xmlobj.posZ = m_Trees[id].vPos.z;
	xmlobj.translateY = m_Trees[id].vPos.y;
	xmlobj.rotY = m_Trees[id].rotY;
	xmlobj.shader = SHADER_TEXTURE_LIGHT;
	xmlobj.instances = 0;			//40
	xmlobj.castShadow = false;		//41
	xmlobj.renderShadows = false;	//41

	if (m_Trees[id].type < 100)
	{
		// Create the texture object, if not created before
		if (!billFileLoaded[type])
		{
			ID3D11ShaderResourceView* tempMeshSRV = NULL;
			HRESULT hr = S_OK;

			LOADTEXTURE(WOMA::LoadFile(billFileName[type]), tempMeshSRV);
			billFileLoaded[type] = tempMeshSRV;
		}

		xmlobj.meshSRV = billFileLoaded[type];
		if (m_Trees[id].type < 11)
			strcpy_s(xmlobj.filename, 256, BILLBOARD_MODEL);		//engine/data/scene70Bill/060square.obj
		else
			if (m_Trees[id].type == 11)
				strcpy_s(xmlobj.filename, 256, BILLBOARD_GRASS_MODEL);	//engine/data/scene73grass/grass.obj
		else
			if (m_Trees[id].type == 12)
				strcpy_s(xmlobj.filename, 256, BILLBOARD_BUSH_MODEL);	//engine/data/scene73grass/grass.obj
	}
	else
		xmlobj.meshSRV = NULL;

	if (m_Trees[id].type == 100)
		strcpy_s(xmlobj.filename, 256, BILLBOARD_FENCE_MODEL);		//100: engine/data/scene70Bill/fence.obj
	if (m_Trees[id].type == 200)
		strcpy_s(xmlobj.filename, 256, BILLBOARD_FIRE_MODEL);		//200: engine/data/scene72Fire/072fire.obj

	xmlobj.WOMA_object = WOMA_OBJECT();
	xmlobj.WOMA_object.shaderType = SHADER_TEXTURE_LIGHT;

	return &xmlobj;
}

bool BillClass::Initialize(int m_terrainWidth, int m_terrainHeight, bool instance)
{	
	UNREFERENCED_PARAMETER(instance);
	ZeroMemory( &m_Trees, sizeof( m_Trees ) );
	billNames_length = sizeof( billFileName ) / sizeof (billFileName[0]);
	UINT type = 0;

	// BILLBOARDs
	int i;
	for (i=0; i< N_BILLBOARD;i++)
	{
		// Tree.vPos:
		float height = -1; //Initially Invalid
		float PosX = 0;
		float PosZ = 0;
		while (height <= 0		//not on water
			|| height > 1.0f	//not above 1m
			|| (m_Trees[i].vPos.x >= 27 && m_Trees[i].vPos.x <= 53) && (m_Trees[i].vPos.z >= 20 && m_Trees[i].vPos.z <= 38) //out of house (compound)
			|| (m_Trees[i].vPos.x < borderLimit || m_Trees[i].vPos.x > m_terrainWidth - borderLimit)		//no near limits
			|| (m_Trees[i].vPos.z < borderLimit || m_Trees[i].vPos.z > m_terrainHeight - borderLimit)		//no near limits
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z) - 1][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z)][(UINT)m_Trees[i].vPos.x] > 0				//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z + 1)][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			)
		{
			PosX = (float)((rand() % (m_terrainWidth * 100)) / 100.0f);
			PosZ = (float)((rand() % (m_terrainHeight * 100)) / 100.0f);
			m_Trees[i].vPos.x = PosX;
			m_Trees[i].vPos.z = PosZ;
			
			height = mainTerrain->getTerrainHeight(TERRAIN_ID, PosX, PosZ);
		}

		// Tree.type: (type of tree)
		type = rand() % (billNames_length-1);
		// Tree.scale:
		float scale = 0;
		if (type >= 6 && type < 11)
			scale =  0.1f + (rand() % 10)/10.0f;
		else
			scale = 0.25f + (rand() % 30) / 10.0f;

		if (type >= 3 && type <= 5) {	// Make Trees Bigger (EU)
			scale += 1.5f;
			height -= 0.5f;
		}
		if (type >= 6)					// Make flowers Smaller
			scale = scale/2;

		if (i == 0) { //Make 1 special tree on the first compound
			m_Trees[i].vPos.x = 27;
			m_Trees[i].vPos.z = 27;
			height = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[i].vPos.x, m_Trees[i].vPos.z);
			type = 3;
			scale = 4;
		}

		m_Trees[i].ID = i;
		m_Trees[i].type = type;
		m_Trees[i].scale = scale;		
		m_Trees[i].vPos.y = height;

		xmlobj3d* xmlobj = fillxml(i, m_Trees[i].type);
		SystemHandle->xml_loader.theWorld.push_back(*xmlobj);
	}
	//N_BILLBOARD

	if (N_FENCES > 0)
	{
		//Fences in X:
		for (int z=0; z<2; z++) {

			for (int x=0; x<17; x++) {

				if ( !(z == 0 && (x == 7 || x == 8)) ) //Exclude: DOOR at Fences
				{
					m_Trees[i].ID = i;
					m_Trees[i].type = 100; // 100 = Fence
					m_Trees[i].scale = 1.0f;
					m_Trees[i].rotY = 0.0f;

					m_Trees[i].vPos.x = 29.5f + x * 1.33f;
					m_Trees[i].vPos.z = 21.5f+0.5f*z + z * 14.0f;
					m_Trees[i].vPos.y = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[i].vPos.x, m_Trees[i].vPos.z);

					xmlobj3d* xmlobj = fillxml(i, 100);
					SystemHandle->xml_loader.theWorld.push_back(*xmlobj);

					if (i++ > N_BILLBOARD + N_FENCES + N_FIRE)
						return false;
				}
			}
		}

		//Fences in Z:	
		for (int y = 0; y < 11; y++) {
			for (int x=0; x<2; x++) {
				m_Trees[i].ID = i;
				m_Trees[i].type = 100; // 100 = Fence
				m_Trees[i].scale = 1.0f;
				m_Trees[i].rotY = PI/2.0f;

				m_Trees[i].vPos.x = 29.5f+0.5f*x + x * 22.0f;
				m_Trees[i].vPos.z = 23 + y * 1.33f;
				m_Trees[i].vPos.y = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[i].vPos.x, m_Trees[i].vPos.z);

				xmlobj3d* xmlobj = fillxml(i, 100);
				SystemHandle->xml_loader.theWorld.push_back(*xmlobj);

				if (i++ > N_BILLBOARD + N_FENCES)
					return false;
			}
		}
	}
	if (N_FIRE > 0)
	{
		m_Trees[i].ID = i;
		m_Trees[i].type = 200; // 200 = Fire
		m_Trees[i].scale = 1.4f;
		m_Trees[i].rotY = 0;

		m_Trees[i].vPos.x = 112;
		m_Trees[i].vPos.z = 239;
		m_Trees[i].vPos.y = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[i].vPos.x, m_Trees[i].vPos.z);

		xmlobj3d* xmlobj = fillxml(i, 200);
		xmlobj->Bill = true;
		SystemHandle->xml_loader.theWorld.push_back(*xmlobj);

		if (i++ > N_BILLBOARD + N_FENCES + N_FIRE)
			return false;
	}

	for (UINT b=0;b<N_GRASS;b++) 
	{
		// Tree.vPos:
		float height = -1; //Initially Invalid
		while (height <= 0		//not on water
			|| height > 1.0f	//not above 1m
			|| (m_Trees[i].vPos.x >= 27 && m_Trees[i].vPos.x <= 53) && (m_Trees[i].vPos.z >= 20 && m_Trees[i].vPos.z <= 38) //out of house (compound)
			|| (m_Trees[i].vPos.x < borderLimit || m_Trees[i].vPos.x > m_terrainWidth - borderLimit)		//no near limits
			|| (m_Trees[i].vPos.z < borderLimit || m_Trees[i].vPos.z > m_terrainHeight - borderLimit)		//no near limits
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z) - 1][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z)][(UINT)m_Trees[i].vPos.x] > 0				//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z + 1)][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			)
		{
			m_Trees[i].vPos.x = (float) (1+(rand() % (m_terrainWidth*30))/100.0f);
			m_Trees[i].vPos.z = (float) (1+(rand() % (m_terrainHeight*30))/100.0f);
			height = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[i].vPos.x, m_Trees[i].vPos.z);
		}

		m_Trees[i].ID = i;
		m_Trees[i].type = 11;// type;
		m_Trees[i].scale = 1.0f;
		m_Trees[i].vPos.y = height;

		xmlobj3d* xmlobj = fillxml(i, m_Trees[i].type);
		xmlobj->Bill = true;
		SystemHandle->xml_loader.theWorld.push_back(*xmlobj);

		i++;
	}

	for (UINT b = 0; b < N_BUSH; b++)
	{
		// Tree.vPos:
		float height = -1; //Initially Invalid
		while (height <= 0		//not on water
			|| height > 1.0f	//not above 1m
			|| (m_Trees[i].vPos.x >= 27 && m_Trees[i].vPos.x <= 53) && (m_Trees[i].vPos.z >= 20 && m_Trees[i].vPos.z <= 38) //out of house (compound)
			|| (m_Trees[i].vPos.x < borderLimit || m_Trees[i].vPos.x > m_terrainWidth - borderLimit)		//no near limits
			|| (m_Trees[i].vPos.z < borderLimit || m_Trees[i].vPos.z > m_terrainHeight - borderLimit)		//no near limits
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z) - 1][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z)][(UINT)m_Trees[i].vPos.x] > 0				//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z + 1)][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			)
		{
			m_Trees[i].vPos.x = (float)(1 + (rand() % (m_terrainWidth * 100)) / 100.0f);
			m_Trees[i].vPos.z = (float)(1 + (rand() % (m_terrainHeight * 100)) / 100.0f);
			height = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[i].vPos.x, m_Trees[i].vPos.z);
		}

		m_Trees[i].ID = i;
		m_Trees[i].type = 12;// type;
		m_Trees[i].scale = 1.0f;
		m_Trees[i].vPos.y = height;

		xmlobj3d* xmlobj = fillxml(i, 12/*m_Trees[i].type*/);
		xmlobj->Bill = true;
		SystemHandle->xml_loader.theWorld.push_back(*xmlobj);
		i++;
	}

	billTotal = i;
	WOMA_LOGManager_DebugMSG( "Bill Class: Initialized\n" );

	return true;
}

//-----------------------------------------------------------------------------   
// Name: TreeSortCB()   
// Desc: Callback function for sorting Bill/trees in back-to-front order   
//-----------------------------------------------------------------------------   
int __cdecl BillSortCB( const VOID* arg1, const VOID* arg2 )   
{   
    Tree* p1 = (Tree*)arg1;   
    Tree* p2 = (Tree*)arg2;   

	// Version 3: Faster and accurated
	float cameraX = SystemHandle->m_Application->m_Position[g_NetID]->m_positionX;
	float cameraZ = SystemHandle->m_Application->m_Position[g_NetID]->m_positionZ;
	float X1 = p1->vPos.x - cameraX;
	float Z1 = p1->vPos.z - cameraZ;
	float X2 = p2->vPos.x - cameraX;
	float Z2 = p2->vPos.z - cameraZ;
    if ((X1*X1 + Z1*Z1) < (X2*X2 + Z2*Z2))
        return +1;

    return -1;   
}
#endif

