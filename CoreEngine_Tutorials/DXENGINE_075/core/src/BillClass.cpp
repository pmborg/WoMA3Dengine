// --------------------------------------------------------------------------------------------
// Filename: BillClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

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
	fence = nullptr; // Initialize fence  

	fire = nullptr; // Initialize fire  

	grass = nullptr; // Initialize grass  
	m_FrustumGrass = nullptr; // Initialize m_FrustumGrass  

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

#if (defined OPENGL3 || defined OPENGL40) 
#include "GLopenGLclass.h"
#include "GLmodelClass.h"
#endif

#if defined DX_ENGINE
#include "DXmodelClass.h"
#endif

Tree			m_Trees[N_BILLBOARD + N_FENCES + N_FIRE + N_GRASS_0 + N_BUSH_0];	// Array of tree info.

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
	// N_BILLBOARD

	//TREEs: 6      //Type:
	BILL_TREE_0,	//0 bush
	BILL_TREE_1,	//1 bush
	BILL_TREE_2,	//2 bush
	BILL_TREE_3,	//3 tree
	BILL_TREE_4,	//4 tree
	BILL_TREE_5,	//5 tree

	//FLOWERs: 5
	BILL_FLOWER_0,	//6	 flower
	BILL_FLOWER_1,	//7	 flower
	BILL_FLOWER_2,	//8	 flower
	BILL_FLOWER_3,	//9  flower
	BILL_FLOWER_4,	//10 flower
	// N_GRASS_0
	BILL_GRASS_0,   //11 animated grass 
	//N_BUSH_0
	BILL_BUSH_0,    //12 cross bush

	//Meta Type:
	// 100: engine/data/scene70Bill/fence.obj
	// 200: engine/data/scene72Fire/072fire.obj
	// 300: 3D fence model engine/data/scene87ForestHuntress.priv/worldMap/woodfence/Fence_module.obj
};

xmlobj3d xmlobj;

xmlobj3d* BillClass::fillxml(ID3D11DeviceContext* pContext, int id, UINT type)
{
	DirectX::DX11Class* m_driver11 = (DirectX::DX11Class*)m_Driver;

	xmlobj.id = id + SystemHandle->m_Application->world_xml_objs;
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
	xmlobj.meshSRV = NULL;

	xmlobj.Bill = true;

	if (m_Trees[id].type < 100)
		{
#if !defined GENERATE_ATLAS_INTEGRATION_DDS
			// Create the texture object, if not created before
			if (!billFileLoaded[type])
			{
				ID3D11ShaderResourceView* tempMeshSRV = NULL;
				HRESULT hr = S_OK;

				LOADTEXTURE(WOMA::LoadFile(billFileName[type]), tempMeshSRV);
				billFileLoaded[type] = tempMeshSRV;
			}

			xmlobj.meshSRV = billFileLoaded[type];
#endif
			if (m_Trees[id].type < 11)
				_tcscpy_s(xmlobj.filename, 256, BILLBOARD_MODEL);				//engine/data/scene70Bill/060square.obj
			else
				if (m_Trees[id].type == 11)		//Windy
					_tcscpy_s(xmlobj.filename, 256, BILLBOARD_GRASS_MODEL);		//11: engine/data/scene73grass/grass.obj
				else
					if (m_Trees[id].type == 12)	//Cross
						_tcscpy_s(xmlobj.filename, 256, BILLBOARD_BUSH_MODEL);	//12: engine/data/scene74grass/bush.obj
		}

	if (m_Trees[id].type == 100)
		_tcscpy_s(xmlobj.filename, 256, BILLBOARD_FENCE_MODEL);		    //100: engine/data/scene70Bill/fence.obj

	if (m_Trees[id].type == 200) //FIRE
		_tcscpy_s(xmlobj.filename, 256, BILLBOARD_FIRE_MODEL);		    //200: engine/data/scene72Fire/072fire.obj

	xmlobj.WOMA_object = WOMA_OBJECT();

	xmlobj.WOMA_object.shaderType = SHADER_TEXTURE_LIGHT;

#ifdef _DEBUG
	if (xmlobj.filename[0] == '\0') {
		// filename is empty
		ASSERT_DEBUG(false);
	}
#endif

	womalog("xmlobj(%d) TYPE:%d x:%d,y:%d\n", (int)xmlobj.id, (int)xmlobj.type, (int)xmlobj.posX, (int)xmlobj.posX);																											 
	return &xmlobj;
}

static Tree tree_ = {};

bool BillClass::Initialize(ID3D11DeviceContext* pContext, int m_terrainWidth, int m_terrainHeight, bool instance)
{
	UNREFERENCED_PARAMETER(instance);
	ZeroMemory(&m_Trees, sizeof(m_Trees));
	billNames_length = sizeof(billFileName) / sizeof(billFileName[0]);
	UINT type = 0;

	// BILLBOARDs
	{
	int i;
	for (i = 0; i < N_BILLBOARD; i++)
	{
		// Tree.type: (type of tree)
		type = rand() % billNames_length; //random number between 0 and 10

		// Tree.vPos:
		float height = -100; //Initially Invalid
		float PosX = 0;
		float PosZ = 0;
		while (height <= 0 ||		//not on water
			height > 1.0f	//not above 1m
			|| (m_Trees[i].vPos.x >= 93 && m_Trees[i].vPos.x <= 101)
			|| (m_Trees[i].vPos.x >= 27 && m_Trees[i].vPos.x <= 53) && (m_Trees[i].vPos.z >= 20 && m_Trees[i].vPos.z <= 38) //out of house (compound)
			|| (m_Trees[i].vPos.x < borderLimit || m_Trees[i].vPos.x > m_terrainWidth - borderLimit)		//no near limits
			|| (m_Trees[i].vPos.z < borderLimit || m_Trees[i].vPos.z > 220 /*m_terrainHeight - borderLimit*/)		//no near limits
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z) - 1][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z)][(UINT)m_Trees[i].vPos.x] > 0				//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[i].vPos.z + 1)][(UINT)m_Trees[i].vPos.x] > 0			//no grass on main PATH (terrain)
			)
		{

			//TREEs: 6      //Type:
			//BILL_TREE_0,	//0 bush
			//BILL_TREE_1,	//1 bush
			//BILL_TREE_2,	//2 bush
			//BILL_TREE_3,	//3 tree
			//BILL_TREE_4,	//4 tree
			//BILL_TREE_5,	//5 tree
			//
			//BILL_FLOWER_0,	//6	

			{
				PosX = (float)((rand() % (m_terrainWidth * 100)) / 100.0f);
				PosZ = (float)((rand() % (m_terrainHeight * 100)) / 100.0f);
			}
			m_Trees[i].vPos.x = PosX;
			m_Trees[i].vPos.z = PosZ;

			height = mainTerrain->getTerrainHeight(TERRAIN_ID, PosX, PosZ);
		}


		ASSERT_DEBUG(type <= billNames_length - 1);

		// Tree.scale:
		float scale = 0;
		if (type >= 6 && type < 11)
			scale = 0.1f + (rand() % 10) / 10.0f;
		else
			scale = 0.25f + (rand() % 30) / 10.0f;

		if (type >= 3 && type <= 5) {	// Make Trees Bigger (EU)
			scale += 1.5f;
			//height -= 0.5f;
		}
		if (type >= 6)					// Make flowers Smaller
			scale = scale / 1.5f;

		m_Trees[i].ID = (UINT)m_Trees.size()-1;
		m_Trees[i].type = type;
		m_Trees[i].scale = scale;
		m_Trees[i].vPos.y = height;

		xmlobj3d* xmlobj = fillxml(pContext, i, m_Trees[i].type);

		if (type <= 10)
			xmlobj->Bill = true;

		m_Trees[i].bill = xmlobj->Bill;

		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

	}
	//N_BILLBOARD
	}

	if (N_FENCES > 0)
	{
		//Fences in X:
		for (int z = 0; z < 2; z++) {

			for (int x = 0; x < 17; x++) {

				if (!(z == 0 && (x == 7 || x == 8))) //Exclude: DOOR at Fences
				{
					//m_Trees[i].ID = i;
					UINT nx = (UINT)m_Trees.size() - 1;
					m_Trees[nx].ID = nx;
					m_Trees[nx].type = 100; // 100 = Fence
					m_Trees[nx].scale = 1.0f;
					m_Trees[nx].rotY = 0.0f;

					m_Trees[nx].vPos.x = 29.5f + x * 1.33f;
					m_Trees[nx].vPos.z = 21.5f + 0.5f * z + z * 14.0f;
					m_Trees[nx].vPos.y = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[nx].vPos.x, m_Trees[nx].vPos.z);

					xmlobj3d* xmlobj = fillxml(pContext, nx, 100);
					xmlobj->Bill = false;
					m_Trees[nx].bill = xmlobj->Bill;
					SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

					if (nx > N_BILLBOARD + N_FENCES + N_FIRE)
						return false;
				}
			}
		}

		//Fences in Z:	
		for (int y = 0; y < 11; y++) {
			for (int x = 0; x < 2; x++) {



				//m_Trees[i].ID = i;
				UINT nx = (UINT)m_Trees.size() - 1;
				m_Trees[nx].ID = nx;
				m_Trees[nx].type = 100; // 100 = Fence
				m_Trees[nx].scale = 1.0f;
				m_Trees[nx].rotY = PI / 2.0f;

				m_Trees[nx].vPos.x = 29.5f + 0.5f * x + x * 22.0f;
				m_Trees[nx].vPos.z = 23 + y * 1.33f;
				m_Trees[nx].vPos.y = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[nx].vPos.x, m_Trees[nx].vPos.z);

				xmlobj3d* xmlobj = fillxml(pContext, nx, 100);
				xmlobj->Bill = false;
				m_Trees[nx].bill = xmlobj->Bill;
				SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

				if (nx > N_BILLBOARD + N_FENCES + N_FIRE)
					return false;

			}
		}
	}

	if (N_FIRE > 0)
	{

		//m_Trees[i].ID = i;
		UINT nx = (UINT)m_Trees.size() - 1;
		m_Trees[nx].ID = nx;
		m_Trees[nx].type = 200; // 200 = Fire
		m_Trees[nx].scale = 1.4f;
		m_Trees[nx].rotY = 0;

		m_Trees[nx].vPos.x = 112;
		m_Trees[nx].vPos.z = 228;
		m_Trees[nx].vPos.y = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[nx].vPos.x, m_Trees[nx].vPos.z);

		xmlobj3d* xmlobj = fillxml(pContext, nx, 200);
		xmlobj->Bill = true;
		m_Trees[nx].bill = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

		if (nx > N_BILLBOARD + N_FENCES + N_FIRE)
			return false;
	}

	for (UINT b = 0; b < N_GRASS_0; b++)
	{
		UINT nx = (UINT)m_Trees.size() - 1;
		// Tree.vPos:
		float height = -100; //Initially Invalid
		while (height <= 0		//not on water
			|| height > 1.0f	//not above 1m
			|| (m_Trees[nx].vPos.x >= 93 && m_Trees[nx].vPos.x <= 101)
			|| (m_Trees[nx].vPos.x >= 27 && m_Trees[nx].vPos.x <= 53) && (m_Trees[nx].vPos.z >= 20 && m_Trees[nx].vPos.z <= 38) //out of house (compound)
			|| (m_Trees[nx].vPos.x < borderLimit || m_Trees[nx].vPos.x > m_terrainWidth - borderLimit)			//no near limits
			|| (m_Trees[nx].vPos.z < borderLimit || m_Trees[nx].vPos.z > 220 /*m_terrainHeight - borderLimit*/)	//no near limits
			|| mainTerrainPath->height[(UINT)(m_Trees[nx].vPos.z) - 1][(UINT)m_Trees[nx].vPos.x] > 0			//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[nx].vPos.z)][(UINT)m_Trees[nx].vPos.x] > 0				//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[nx].vPos.z + 1)][(UINT)m_Trees[nx].vPos.x] > 0			//no grass on main PATH (terrain)
			)
		{
			m_Trees[nx].vPos.x = (float)(1 + (rand() % (m_terrainWidth * 30)) / 100.0f);
			m_Trees[nx].vPos.z = (float)(1 + (rand() % (m_terrainHeight * 30)) / 100.0f);
			height = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[nx].vPos.x, m_Trees[nx].vPos.z);
		}

		//m_Trees[nx].ID = i;
		
		m_Trees[nx].ID = nx;
		m_Trees[nx].type = 11;// type;
		m_Trees[nx].scale = 1.0f;
		m_Trees[nx].vPos.y = height;

		xmlobj3d* xmlobj = fillxml(pContext, nx, m_Trees[nx].type);
		xmlobj->Bill = true;
		m_Trees[nx].bill = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

		//i++;
	}

	for (UINT b = 0; b < N_BUSH_0; b++)
	{
		UINT nx = (UINT)m_Trees.size() - 1;
		// Tree.vPos:
		float height = -100; //Initially Invalid
		while (height <= 0		//not on water
			|| height > 1.0f	//not above 1m
			|| (m_Trees[nx].vPos.x >= 93 && m_Trees[nx].vPos.x <= 101)
			|| (m_Trees[nx].vPos.x >= 27 && m_Trees[nx].vPos.x <= 53) && (m_Trees[nx].vPos.z >= 20 && m_Trees[nx].vPos.z <= 38) //out of house (compound)
			|| (m_Trees[nx].vPos.x < borderLimit || m_Trees[nx].vPos.x > m_terrainWidth - borderLimit)			//no near limits
			|| (m_Trees[nx].vPos.z < borderLimit || m_Trees[nx].vPos.z > 220 /*m_terrainHeight - borderLimit*/)	//no near limits
			|| mainTerrainPath->height[(UINT)(m_Trees[nx].vPos.z) - 1][(UINT)m_Trees[nx].vPos.x] > 0			//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[nx].vPos.z)][(UINT)m_Trees[nx].vPos.x] > 0				//no grass on main PATH (terrain)
			|| mainTerrainPath->height[(UINT)(m_Trees[nx].vPos.z + 1)][(UINT)m_Trees[nx].vPos.x] > 0			//no grass on main PATH (terrain)
			)
		{
			m_Trees[nx].vPos.x = (float)(1 + (rand() % (m_terrainWidth * 100)) / 100.0f);
			m_Trees[nx].vPos.z = (float)(1 + (rand() % (m_terrainHeight * 100)) / 100.0f);
			height = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[nx].vPos.x, m_Trees[nx].vPos.z);
		}

		//m_Trees[nx].ID = i;
		m_Trees[nx].ID = nx;
		m_Trees[nx].type = 12;// type;
		m_Trees[nx].scale = 1.0f;
		m_Trees[nx].vPos.y = height;

		xmlobj3d* xmlobj = fillxml(pContext, nx, 12/*m_Trees[nx].type*/);
		xmlobj->Bill = true;
		m_Trees[nx].bill = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);
		//i++;
	}

	billTotal = (UINT)m_Trees.size();

	womalog("Bill Class: Initialized\n");

#if _DEBUG
	{
		auto countthis = 0;
		for (auto i = 0; i < m_Trees.size(); i++)
		{
			if (m_Trees[i].ID == 0)
				countthis++;
		}
		ASSERT_DEBUG(countthis == 1);
	}
#endif

	return true;
}

inline float Distance2D(const Tree* p, float camX, float camZ) {
	float dx = p->vPos.x - camX;
	float dz = p->vPos.z - camZ;
	return dx * dx + dz * dz;
}

int __cdecl BillSortCB(const void* arg1, const void* arg2)
{
	const Tree* p1 = static_cast<const Tree*>(arg1);
	const Tree* p2 = static_cast<const Tree*>(arg2);

	float d1 = Distance2D(p1, sort_cameraX, sort_cameraZ);
	float d2 = Distance2D(p2, sort_cameraX, sort_cameraZ);

	return (d1 < d2) ? +1 : -1;
}
#endif

