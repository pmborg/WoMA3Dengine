// --------------------------------------------------------------------------------------------
// Filename: BillClass.h
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

#ifndef _BILLCLASS_H_
#define _BILLCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "OSengine.h"
#include "womadriverclass.h"
#include "virtualModelClass.h"
#include "xml_loader.h"
#include "Tree.h"

// Number of Billboard Trees

#if defined SIMPLE
#define N_BILLBOARD 1
#define N_FENCES    55
#define N_FIRE      1
#define N_GRASS_0   1
#define N_BUSH_0    1
#else
#define N_BILLBOARD 1000
#define N_FENCES      55
#define N_FIRE         1
#define N_GRASS_0   2000 //11 animated grass 
#define N_BUSH_0     750 //12 BUSHs
#endif

extern ID3D11ShaderResourceView* billFileLoaded[];

extern Tree			m_Trees[N_BILLBOARD + N_FENCES + N_FIRE + N_GRASS_0 + N_BUSH_0];	// Array of tree info.



////////////////////////////////////////////////////////////////////////////////
// Class name: BillClass
////////////////////////////////////////////////////////////////////////////////
class BillClass
{
public:
	BillClass();
	~BillClass();

    xmlobj3d* fillxml(ID3D11DeviceContext* pContext, int id, UINT type);
	bool Initialize(ID3D11DeviceContext* pContext, int m_terrainWidth, int m_terrainHeight, bool instance);
	void Shutdown();
	CTerrain* mainTerrainPath = NULL;

#if DX_ENGINE_LEVEL >= 77 && defined USE_INSTANCES_FOR_TREES
	void Tree0GS(InstanceType* instances, int m_instanceCount);
#endif

public:
	int		BillrenderCount = 0;
	UINT	billTotal = 0;

private:
	VirtualModelClass* fence = nullptr;
	VirtualModelClass* fire = nullptr;
	VirtualModelClass* grass = nullptr;
	FrustumClass* m_FrustumGrass = nullptr;

	UINT				billNames_length = 0;
};

extern BillClass* m_billTreeClass;

#endif
