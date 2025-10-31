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

#if defined GENERATE_ATLAS_INTEGRATION_DDS
std::vector<STRING> billboardTextures;

ID3D11ShaderResourceView* billboardAtlasSRV = nullptr;  // One SRV for the entire atlas
std::vector<AtlasRegion> billboardAtlasRegions;         // UV regions for each billboard
#endif

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

	#if defined GENERATE_ATLAS_INTEGRATION_DDS
	SAFE_RELEASE(billboardAtlasSRV);
	billboardAtlasRegions.clear();
	#endif

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

std::vector<Tree> m_Trees;

ID3D11ShaderResourceView* billFileLoaded[] =
{			//Type:
	NULL,	//0 bush
	NULL,	//1 bush
	NULL,	//2 bush
	NULL,	//3 tree
	NULL,	//4 tree
	NULL,	//5 tree		
							
	NULL,	//6	 flower	
	NULL,	//7	 flower	
	NULL,	//8	 flower	
	NULL,	//9  flower	
	NULL,	//10 flower	

	NULL,	//11 animated grass 

	NULL,	//12 cross bush
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
	xmlobj.shader = SHADER_TEXTURE_LIGHT_FAST; //83
	xmlobj.instances = 0;			//40
	xmlobj.castShadow = false;		//41
	xmlobj.renderShadows = false;	//41
	xmlobj.meshSRV = NULL;

	xmlobj.Bill = true;

	if (m_Trees[id].type < 100)
#if DX_ENGINE_LEVEL >= 70/*90*/ && !defined NO3DBILL
		if (xmlobj.posX <= 98 && xmlobj.posZ <= 51)
#endif
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

	if (m_Trees[id].type == 300)
	{
		_tcscpy_s(xmlobj.filename, 256, FENCE1_MODEL);					//300: 3D engine/data/scene87ForestHuntress.priv/worldMap/woodfence/Fence_module.obj
	}

	xmlobj.moveUp = false;

#if !defined SIMPLE && !defined NO3DBILL
	if (xmlobj.posX > 98 || xmlobj.posZ > 51)
	{
		xmlobj.Bill = false;
		m_Trees[id].bill = false;
		switch (m_Trees[id].type)
		{
		case 0:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Hedge-01.obj");
			xmlobj.scale = 0.3f;
			xmlobj.moveUp = true;
			break;
		case 1:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Bush-03.obj");
			xmlobj.scale = 0.5f;
			xmlobj.moveUp = true;
			break;
		case 2:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Bush-04.obj");
			xmlobj.scale = 0.5f;
			xmlobj.moveUp = true;
			break;
		case 3:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Tree-03-1.obj");
			xmlobj.scale = 1.4f;
			xmlobj.moveUp = true;
			xmlobj.shader = SHADER_TEXTURE_LIGHT;
			break;
		case 4:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Tree-03-2.obj");
			xmlobj.scale = 1.4f;
			xmlobj.moveUp = true;
			xmlobj.shader = SHADER_TEXTURE_LIGHT;
			break;
		case 5:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Tree-03-3.obj");
			xmlobj.scale = 1.4f;
			xmlobj.moveUp = true;
			xmlobj.shader = SHADER_TEXTURE_LIGHT;
			break;
		case 6:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Flowers-01.obj");
			xmlobj.scale = 0.6f;
			xmlobj.moveUp = true;
			break;
		case 7:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Flowers-02.obj");
			xmlobj.scale = 0.6f;
			xmlobj.moveUp = true;
			break;
		case 8:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Flowers-03.obj");
			xmlobj.scale = 0.6f;
			xmlobj.moveUp = true;
			break;
		case 9:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Flowers-04.obj");
			xmlobj.scale = 0.6f;
			xmlobj.moveUp = true;
			break;
		case 10:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Clover-01.obj");
			xmlobj.scale = 0.6f;
			xmlobj.moveUp = true;
			break;

		case 11:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Grass-02.obj");
			xmlobj.scale = 0.6f;
			xmlobj.moveUp = true;
			break;
		case 12:
			strcpy_s(xmlobj.filename, 256, "engine/data/scene87ForestHuntress.priv/worldMap/models_plant_bush_shape_spark/Grass-03.obj");
			xmlobj.scale = 0.5f;
			xmlobj.moveUp = true;
			break;

		default:
			xmlobj.moveUp = false;
		}
	}
#endif

	xmlobj.WOMA_object = WOMA_OBJECT();

	if (m_Trees[id].type == 200) //FIRE
		xmlobj.WOMA_object.shaderType = SHADER_FIRE;
	else
		xmlobj.WOMA_object.shaderType = SHADER_TEXTURE_LIGHT_FAST;

#if defined GENERATE_ATLAS_INTEGRATION_DDS
	#if defined  NO3DBILL
	if (xmlobj.Bill && m_Trees[id].type <= 12)
	#else
	if (xmlobj.Bill && m_Trees[id].type <= 10)
	#endif
		xmlobj.shader = SHADER_BILLBOARD_ATLAS_FAST;
#endif

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
	billNames_length = sizeof(billFileName) / sizeof(billFileName[0]);
	UINT type = 0;

	// BILLBOARDs
	{
	int i;
	for (i = 0; i < N_BILLBOARD; i++)
	{
		// Tree.type: (type of tree)
		type = rand() % billNames_length; //random number between 0 and 10

		m_Trees.push_back(tree_);
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

		m_Trees[i].sortKey = xmlobj->Bill;
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
					m_Trees.push_back(tree_);
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
					m_Trees[nx].sortKey = xmlobj->Bill;
					SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

					if (nx > N_BILLBOARD + N_FENCES + N_FIRE)
						return false;
				}
			}
		}

		//Fences in Z:	
		for (int y = 0; y < 11; y++) {
			for (int x = 0; x < 2; x++) {
				m_Trees.push_back(tree_);



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
				m_Trees[nx].sortKey = xmlobj->Bill;
				SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

				if (nx > N_BILLBOARD + N_FENCES + N_FIRE)
					return false;

			}
		}
	}

	if (N_FIRE > 0)
	{
		m_Trees.push_back(tree_);

		//m_Trees[i].ID = i;
		UINT nx = (UINT)m_Trees.size() - 1;
		m_Trees[nx].ID = nx;
		m_Trees[nx].type = 200; // 200 = Fire
		m_Trees[nx].scale = 1.4f;
		m_Trees[nx].rotY = 0;

		m_Trees[nx].vPos.x = 112;
		m_Trees[nx].vPos.z = 239;
		m_Trees[nx].vPos.y = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Trees[nx].vPos.x, m_Trees[nx].vPos.z);

		xmlobj3d* xmlobj = fillxml(pContext, nx, 200);
		xmlobj->Bill = true;
		m_Trees[nx].sortKey = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

		if (nx > N_BILLBOARD + N_FENCES + N_FIRE)
			return false;
	}

	for (UINT b = 0; b < N_GRASS_0; b++)
	{
		m_Trees.push_back(tree_);
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
		m_Trees[nx].sortKey = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);

	}

	for (UINT b = 0; b < N_BUSH_0; b++)
	{
		m_Trees.push_back(tree_);
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
		m_Trees[nx].sortKey = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);
	}

#if DX_ENGINE_LEVEL >= 90 && !defined SIMPLE
	for (int idx = 0; idx < 31; idx++)
	{
		m_Trees.push_back(tree_);
		UINT nx = (UINT)m_Trees.size() - 1;

		//m_Trees[nx].ID = i;
		m_Trees[nx].ID = nx;
		m_Trees[nx].type = 300; // 300 = Fence1
		m_Trees[nx].scale = 1.0f;
		m_Trees[nx].vPos.x = 97;
		m_Trees[nx].vPos.z = idx * 2.4f;
		m_Trees[nx].vPos.y = 0.1f;
		m_Trees[nx].rotY = PI / 2.0f;

		xmlobj3d* xmlobj = fillxml(pContext, nx, m_Trees[nx].type);
		xmlobj->Bill = false;
		m_Trees[nx].sortKey = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);
	}

	for (int idx = 0; idx < 3; idx++)
	{
		m_Trees.push_back(tree_);
		UINT nx = (UINT)m_Trees.size() - 1;

		//m_Trees[nx].ID = i;
		m_Trees[nx].ID = nx;
		m_Trees[nx].type = 300; // 300 = Fence1
		m_Trees[nx].scale = 1.0f;
		m_Trees[nx].vPos.x = 96 - idx * 2.4f - 0.2f;
		m_Trees[nx].vPos.z = 31 * 2.4f - 1.2f;
		m_Trees[nx].vPos.y = 0.1f;

		xmlobj3d* xmlobj = fillxml(pContext, nx, m_Trees[nx].type);
		xmlobj->Bill = false;
		m_Trees[nx].sortKey = xmlobj->Bill;
		SystemHandle->xml_loader.theWorldXML.push_back(*xmlobj);
	}
#endif

	billTotal = (UINT)m_Trees.size();

#if defined GENERATE_ATLAS_INTEGRATION_DDS
	// Use billFileName[] from woma_engine_assets.h
	for (size_t i = 0; i < _countof(billFileName); i++)
	{
		billboardTextures.push_back((STRING)(billFileName[i]));
	}

	if (FAILED(GenerateBillboardAtlasDDS(
		((DirectX::DX11Class*)m_Driver)->m_device11,
		billboardTextures,
		&billboardAtlasSRV,
		billboardAtlasRegions)))
	{
		womalog(TEXT("[ERROR] Billboard atlas generation failed!\n"));
	}
#endif

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


#if defined GENERATE_ATLAS_INTEGRATION_DDS
#include <array>

static const ModelTextureVertexType kBaseQuad[4] = {
	// x, y, z | tu, tv
	{ {  0.5f, 0.0f, 0.0f }, {1.0f, 0.0f} }, // v1
	{ { -0.5f, 0.0f, 0.0f }, {0.0f, 0.0f} }, // v2
	{ { -0.5f, 1.0f, 0.0f }, {0.0f, 1.0f} }, // v3
	{ {  0.5f, 1.0f, 0.0f }, {1.0f, 1.0f} }, // v4
};

std::vector<ModelBillboardAtlasVertexType> outVerts;
std::vector<uint32_t> outIdx;

VirtualModelClass* AtlasobjModel;

#define MAX_BILLBOARD_VERTS (MAX_BILLBOARDS * 4)
#define MAX_BILLBOARD_IDX   (MAX_BILLBOARDS * 6)

void BuildBillboardAtlasMesh_FromTrees(
	std::vector<ModelBillboardAtlasVertexType>& outVerts,
	std::vector<uint32_t>& outIdx,
	const std::vector<Tree>& treesSorted,
	size_t& outVertsCount,
	size_t& outIdxCount
)
{
	// Reserve memory up front to avoid repeated reallocations in push_back
	size_t billCount = 0;
	for (const Tree& t : treesSorted) {
		if (!t.sortKey) continue;
#if defined  NO3DBILL
		if (t.type > 12) continue; //Use all from 0..12
#else
		if (t.type > 10) continue;
#endif
		if (t.type >= billboardAtlasRegions.size()) continue;
		++billCount;
	}

	if (m_Driver->RenderfirstTime)
	{
		//outVerts.clear();
		//outIdx.clear();
		outVerts.reserve(MAX_BILLBOARDS * 4);
		outIdx.reserve(MAX_BILLBOARDS * 6);
		ModelBillboardAtlasVertexType v;
		for (size_t i = 0; i < MAX_BILLBOARDS * 4; i++)
			outVerts.push_back(v);
		for (size_t i = 0; i < MAX_BILLBOARDS * 6; i++)
			outIdx.push_back(0);
	}

	int v = 0, i = 0;
	for (const Tree& t : treesSorted)
	{
		if (!t.sortKey) continue;
#if defined  NO3DBILL
		if (t.type > 12) continue; //Use all from 0..12
#else
		if (t.type > 10) continue;
#endif
		if (t.type >= billboardAtlasRegions.size()) continue;

		const uint32_t base = v; // (uint32_t)outVerts.size();

		// Unroll the loop for 4 vertices for better performance
		{
			ModelBillboardAtlasVertexType out0{}, out1{}, out2{}, out3{};
			out0.x = kBaseQuad[0].x; out0.y = kBaseQuad[0].y; out0.z = kBaseQuad[0].z;
			out1.x = kBaseQuad[1].x; out1.y = kBaseQuad[1].y; out1.z = kBaseQuad[1].z;
			out2.x = kBaseQuad[2].x; out2.y = kBaseQuad[2].y; out2.z = kBaseQuad[2].z;
			out3.x = kBaseQuad[3].x; out3.y = kBaseQuad[3].y; out3.z = kBaseQuad[3].z;

			out0.tu = kBaseQuad[0].tu; out0.tv = 1.0f - kBaseQuad[0].tv;
			out1.tu = kBaseQuad[1].tu; out1.tv = 1.0f - kBaseQuad[1].tv;
			out2.tu = kBaseQuad[2].tu; out2.tv = 1.0f - kBaseQuad[2].tv;
			out3.tu = kBaseQuad[3].tu; out3.tv = 1.0f - kBaseQuad[3].tv;

			out0.nx = out1.nx = out2.nx = out3.nx = 0.0f;
			out0.ny = out1.ny = out2.ny = out3.ny = 1.0f;
			out0.nz = out1.nz = out2.nz = out3.nz = 0.0f;
			out0.atlasIndex = out1.atlasIndex = out2.atlasIndex = out3.atlasIndex = t.type;

			out0.ox = out1.ox = out2.ox = out3.ox = t.vPos.x;
			out0.oy = out1.oy = out2.oy = out3.oy = t.vPos.y;
			out0.oz = out1.oz = out2.oz = out3.oz = t.vPos.z;
			out0.scale = out1.scale = out2.scale = out3.scale = t.scale;
			out0.rotY = out1.rotY = out2.rotY = out3.rotY = t.rotY;

			outVerts[v++] = out0;
			outVerts[v++] = out1;
			outVerts[v++] = out2;
			outVerts[v++] = out3;
		}

		// 2 triangles
		outIdx[i++] = (base + 0);
		outIdx[i++] = (base + 1);
		outIdx[i++] = (base + 2);
		outIdx[i++] = (base + 2);
		outIdx[i++] = (base + 3);
		outIdx[i++] = (base + 0);
	}

	outVertsCount = v;
	outIdxCount = i;
}

// Create once (sizes big enough for your typical worst case), e.g. 10k verts / 15k indices.
ID3D11Buffer* gBillVB = nullptr;
ID3D11Buffer* gBillIB = nullptr;
void UpdateBills(ID3D11DeviceContext* ctx,
	const std::vector<ModelBillboardAtlasVertexType>& verts,
	const std::vector<uint32_t>& idx,
	size_t& outVertsCount,
	size_t& outIdxCount
)
{
	if (verts.empty() || idx.empty()) return;

	// Update VB
	D3D11_MAPPED_SUBRESOURCE m;
	ctx->Map(gBillVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &m);
	memcpy(m.pData, verts.data(), outVertsCount /*verts.size()*/ * sizeof(verts[0]));
	ctx->Unmap(gBillVB, 0);

	// Update IB
	ctx->Map(gBillIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &m);
	memcpy(m.pData, idx.data(), outIdxCount /*idx.size()*/ * sizeof(idx[0]));
	ctx->Unmap(gBillIB, 0);

	((DXmodelClass*)AtlasobjModel)->m_indexCount = (UINT)outIdxCount; // idx.size();
}

#endif

#if (DX_ENGINE_LEVEL >= 94 && defined USE_TREE_POINTERV2) || defined INTRO_DEMO
bool BillSortCB_CPP_key(const Tree& a, const Tree& b)
{
	return a.sortKey > b.sortKey; // Farther first (back-to-front)
}
#endif

bool BillSortCB_CPP(const Tree& a, const Tree& b)
{
	float dx1 = a.vPos.x - sort_cameraX;
	float dz1 = a.vPos.z - sort_cameraZ;
	float dx2 = b.vPos.x - sort_cameraX;
	float dz2 = b.vPos.z - sort_cameraZ;

	float d1 = dx1 * dx1 + dz1 * dz1;
	float d2 = dx2 * dx2 + dz2 * dz2;

	return d1 > d2; // Farther first (back-to-front)
}

// =============================================================================================
// Function: RunBillboardSortDemo
// Purpose : Central dispatcher for all historical billboard sort variants (v70–v98)
// =============================================================================================

void RunBillboardSortDemo(UINT RENDER_PAGE, std::vector<Tree>& m_Trees)
{
	// Skip if before billboards
	if (RENDER_PAGE < 70)
		return;

	// Only log once at first frame
	womalogATfirstframe(TEXT("[DEMO99] Billboard sort replay for level %d\n"), RENDER_PAGE);

	switch (RENDER_PAGE)
	{
		// ---------------------------------------------------------
		// 70–91 : Legacy m_Trees (AQUICHECKv4)
		// ---------------------------------------------------------
	case 70: case 71: case 72: case 73: case 74: case 75:
	case 76: case 77: case 78: case 82: case 83: case 84:
	case 85: case 86: case 87: case 88: case 89: case 90:
	case 91:
		std::sort(m_Trees.begin(), m_Trees.end(), BillSortCB_CPP);
		break;

		// ---------------------------------------------------------
		// 92–93 : SceneManager list (AQUICHECKv3)
		// ---------------------------------------------------------
	case 92:
	case 93:
		std::sort(
			WOMA::sceneManager->visibleBillboardList.begin(),
			WOMA::sceneManager->visibleBillboardList.end(),
			BillSortCB_CPP);
		break;


		// ---------------------------------------------------------
		// 94+ : Pointer-based versions (AQUICHECKv1 / v2)
		// ---------------------------------------------------------
	case 94:
	default:
#if defined USE_TREE_POINTER
		std::sort(
			WOMA::sceneManager->visibleBillboardList.begin(),
			WOMA::sceneManager->visibleBillboardList.end(),
			[](const Tree* a, const Tree* b) { return a->sortKey > b->sortKey; });
#elif defined USE_TREE_POINTERV2
		std::sort(
			WOMA::sceneManager->visibleBillboardList.begin(),
			WOMA::sceneManager->visibleBillboardList.end(),
			BillSortCB_CPP_key);
#else
		std::sort(
			WOMA::sceneManager->visibleBillboardList.begin(),
			WOMA::sceneManager->visibleBillboardList.end(),
			BillSortCB_CPP);
#endif
		break;

	}

	womalogATfirstframe(TEXT("[DEMO99] Billboard sort completed for level %d\n"), RENDER_PAGE);
}

//#endif