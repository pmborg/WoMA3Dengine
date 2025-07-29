// --------------------------------------------------------------------------------------------
// Filename: Tree.h
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

#ifndef _TREE_H_
#define _TREE_H_

//-----------------------------------------------------------------------------   
// Name: Tree   
// Desc: Simple structure to hold data for rendering a tree   
//-----------------------------------------------------------------------------   
struct Tree   
{   
	XMFLOAT3    vPos;				// Origin of tree   
	float		rotY;
	float		scale;
	UINT        type;
	UINT        ID;
	//0-10
	//100: FENCE
	//200: FIRE
};   

#endif
