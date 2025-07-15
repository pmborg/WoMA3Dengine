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
//WomaIntegrityCheck = 1234525256;

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
#define N_BILLBOARD 1000
#define N_FENCES 0
#define N_FIRE 0
#define N_GRASS_0 0
#define N_BUSH_0  0

#if false //defined SIMPLE
#define N_BILLBOARD 100
#define N_FENCES    55
#define N_FIRE      1
#define N_GRASS_0   1
#define N_BUSH_0    1
#else
#define N_BILLBOARD 1000
#define N_FENCES      55
#define N_FIRE         1
#define N_GRASS_0   2000
#define N_BUSH_0     750
#endif

extern ID3D11ShaderResourceView* billFileLoaded[];

extern Tree			m_Trees[N_BILLBOARD];						        // Array of tree info.



////////////////////////////////////////////////////////////////////////////////
// Class name: BillClass
////////////////////////////////////////////////////////////////////////////////
class BillClass
{
public:
	BillClass();
	~BillClass();

	xmlobj3d* fillxml(int id, UINT type);
	bool Initialize(int m_terrainWidth, int m_terrainHeight, bool instance);
	void Shutdown();
	CTerrain* mainTerrainPath = NULL;

public:
int		BillrenderCount=0;
UINT	billTotal = 0;

private:

UINT				billNames_length=0;
};

extern BillClass* m_billTreeClass;

#endif
