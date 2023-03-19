// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: WomaCube.h
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
// Downloaded from : http://woma.servegame.com
// --------------------------------------------------------------------------------------------
// PURPOSE: interface for a Color and Texture Cube
// --------------------------------------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vertexTypes.h"

const int magic = 1;
const int n_vertex = 4;
const int n_faces = 6;

class CCube
{
public:
	CCube(int x_0, int y_0, int z_0);
	virtual ~CCube();
	void Shutdown();

	void Render(int r);
	void VertexCubeColor(int c, int i, int f);
	void VertexCubeTexture (int c, int i, int f);

	//ModelColorVertexType VertexCube1[6*magic*magic][n_faces*n_vertex*magic*magic];
	//ModelTextureVertexType VertexCube2[6 * magic * magic][n_faces * n_vertex * magic * magic];

	std::vector <ModelColorVertexType> VertexCube1;
	std::vector <ModelTextureVertexType> VertexCube2;
	std::vector<UINT> IndexCubeList;
};

