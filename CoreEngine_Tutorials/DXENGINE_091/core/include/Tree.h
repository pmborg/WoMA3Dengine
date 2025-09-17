// --------------------------------------------------------------------------------------------
// Filename: Tree.h
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
//WomaIntegrityCheck = 1234525217;

#ifndef _TREE_H_
#define _TREE_H_

//-----------------------------------------------------------------------------   
// Name: Tree   
// Desc: Simple structure to hold data for rendering a tree   
//-----------------------------------------------------------------------------   
struct Tree   
{   
	UINT        ID=0;
	bool		bill=false;
	XMFLOAT3    vPos = {};	// Origin of tree   
	float		rotY=0;
	float		scale=0;
	UINT        type=0;
	
	// type: 0-10 normal billboards
	// 100: FENCE 2D
	// 200: FIRE
	// 300: FENCE 3D
};

struct compoundTreeLoadOrder {
	int compoundTreeId = -1;
	int modelId = -1;
	UINT order = 0;
};
#endif
