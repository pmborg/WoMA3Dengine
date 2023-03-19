// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: WomaCube.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2003 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: Define a Color and Texture Cube
// --------------------------------------------------------------------------------------------

//-------------------+
// Project Includes: |
//-------------------+
#include "WomaCube.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCube::CCube(int x_0, int y_0, int z_0)
{
	const float l = 1;		///*txtSize*/128 / 2;
	const float y = 1;		//ZZ (D3D)
	const float x = 1;	//XX (D3D)
	const float z = 1;	//YY (D3D)
	const float X = 1;
	const float Z = 1;
	const float Y = 1;

	struct VERTEX
	{
		float fX, fY, fZ;
	};

	static VERTEX cube[n_faces][n_vertex] =
	{
		{{X - l / x,Y + l / y,Z - l / z},{X + l / x,Y + l / y, Z - l / z},{X - l / x,Y - l / y,Z - l / z},	{X + l / x,Y - l / y,Z - l / z}},	//0 front
		{{X + l / x,Y + l / y,Z + l / z},{X - l / x,Y + l / y, Z + l / z},{X + l / x,Y - l / y,Z + l / z},	{X - l / x,Y - l / y,Z + l / z}},	//1 back
		{{X - l / x,Y + l / y,Z + l / z},{X - l / x,Y + l / y, Z - l / z},{X - l / x,Y - l / y,Z + l / z},	{X - l / x,Y - l / y,Z - l / z}},	//2 left
		{{X + l / x,Y + l / y,Z - l / z},{X + l / x,Y + l / y, Z + l / z},{X + l / x,Y - l / y,Z - l / z},	{X + l / x,Y - l / y,Z + l / z}},	//3 right
		{{X - l / x,Y - l / y,Z - l / z},{X + l / x,Y - l / y, Z - l / z},{X - l / x,Y - l / y,Z + l / z},	{X + l / x,Y - l / y,Z + l / z}},	//4 bot
		{{X - l / x,Y + l / y,Z + l / z},{X + l / x,Y + l / y, Z + l / z},{X - l / x,Y + l / y,Z - l / z},	{X + l / x,Y + l / y,Z - l / z}}	//5 top
	};
	/*
	static VERTEX cube2[n_faces][n_vertex] =
	{
		{{X - l / x,Y + l / y,Z - l / z},{X + l / x,Y + l / y, Z - l / z},{X - l / x,Y - l / y,Z - l / z},	{X + l / x,Y - l / y,Z - l / z}},	//0 front
		{{X + l / x,Y + l / y,Z + l / z},{X - l / x,Y + l / y, Z + l / z},{X + l / x,Y - l / y,Z + l / z},	{X - l / x,Y - l / y,Z + l / z}},	//1 back
		{{X - l / x,Y + l / y,Z + l / z},{X - l / x,Y + l / y, Z - l / z},{X - l / x,Y - l / y,Z + l / z},	{X - l / x,Y - l / y,Z - l / z}},	//2 left
		{{X + l / x,Y + l / y,Z - l / z},{X + l / x,Y + l / y, Z + l / z},{X + l / x,Y - l / y,Z - l / z},	{X + l / x,Y - l / y,Z + l / z}},	//3 right
		{{X - l / x,Y - l / y,Z - l / z},{X + l / x,Y - l / y, Z - l / z},{X - l / x,Y - l / y,Z + l / z},	{X + l / x,Y - l / y,Z + l / z}},	//4 bot
		{{X - l / x,Y + l / y,Z + l / z},{X + l / x,Y + l / y, Z + l / z},{X - l / x,Y + l / y,Z - l / z},	{X + l / x,Y + l / y,Z - l / z}}	//5 top
	};
	*/
	int MAX_OBJ = 6*magic*magic;
	int vertCounter=0;
	//-----------------------------------------------------------------------------------------
	//if (magic_ == 1) 
	{
	ModelColorVertexType Vertex;
	int c = 0;
		for (int f = 0; f < MAX_OBJ; f++) {
			for (int v = 0; v < n_vertex; v++) {
				VertexCube1.push_back(Vertex);
				VertexCube1[c].x= cube[f][v].fX - 1;
				VertexCube1[c].y= cube[f][v].fY - 1;
				VertexCube1[c].z= cube[f][v].fZ - 1;
				VertexCubeColor(c, f, v);
				c++;
			}
			vertCounter++;
		}
	}

	//-----------------------------------------------------------------------------------------
	//if (magic_ == 1) 
	{
		ModelTextureVertexType Vertex;
		int c = 0;
		for (int f=0;f<MAX_OBJ;f++) {
			for (int v=0;v<n_vertex;v++) {
				VertexCube2.push_back(Vertex);
				VertexCube2[c].x = cube[f][v].fX - 1;
				VertexCube2[c].y = cube[f][v].fY - 1;
				VertexCube2[c].z = cube[f][v].fZ - 1;
				VertexCubeTexture (c, f, v);
				c++;
			}
			vertCounter++;
		} 
	}

	for (int f = 0; f < 6; f++)
	{
		IndexCubeList.push_back(0 + f * 4); IndexCubeList.push_back(1 + f * 4); IndexCubeList.push_back(3 + f * 4);
		IndexCubeList.push_back(3 + f * 4); IndexCubeList.push_back(2 + f * 4); IndexCubeList.push_back(0 + f * 4);
	}
}

CCube::~CCube()
{
	Shutdown();
}

void CCube::Shutdown()
{
/*
	SAFE_RELEASE( pWallTexture );
	for (int r=0;r<rec;r++)
		SAFE_RELEASE( pCubeBuffer[r] );
	//SAFE_DELETE_ARRAY( pCubeBuffer );
*/
}


void CCube::Render(int r) 
{
/*
	if (m_pD3DDevice) {
		m_pD3DDevice->SetTexture(0,pWallTexture);
		m_pD3DDevice->SetStreamSource(0, pCubeBuffer[r], 0, sizeof(MYD3DVERTEX));
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	}
*/
}

void CCube::VertexCubeColor(int c, int i, int f)
{
	if (i == 0)
		VertexCube1[c].r = 1;	//.dwColor = 0x00ff0000; //R
	else if (i == 1)
		VertexCube1[c].b = 1;	//.dwColor = 0x000000ff; //B
	else if (i == 2)
		VertexCube1[c].g = 1;	//.dwColor = 0x0000ff00; //G
	else if (i == 3)
	{
		VertexCube1[c].r = 1;	//dwColor = 0x00ffff00; //Y
		VertexCube1[c].g = 1;
	}
	else if (i == 4)
	{
		VertexCube1[c].g = 1;	//.dwColor = 0x0000ffff; //C
		VertexCube1[c].b = 1;	//.dwColor = 0x0000ffff; //C
	}
	else
	{
		VertexCube1[c].r = 1;	//.dwColor = 0x00ffffff; //W
		VertexCube1[c].g = 1;
		VertexCube1[c].b = 1;
	}
}

void CCube::VertexCubeTexture (int c, int i, int f)
{
	if (f==0) {
		VertexCube2[c].tu= 0; 
		VertexCube2[c].tv = 0;
	}
	if (f==1) {
		VertexCube2[c].tu = 1; 
		VertexCube2[c].tv = 0;
	}
	if (f==2) {
		VertexCube2[c].tu = 0; 
		VertexCube2[c].tv = 1;
	}
	if (f==3) {
		VertexCube2[c].tu = 1; 
		VertexCube2[c].tv = 1;
	}
}
