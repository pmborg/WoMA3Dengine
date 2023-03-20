// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
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
	const float l = 1;	///*txtSize*/128 / 2;
	const float y = 1;	//ZZ (D3D)
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

	int MAX_OBJ = 6*magic*magic;
	int vertCounter=0;

	//COLOR:
	//-----------------------------------------------------------------------------------------
	{
	ModelColorVertexType Vertex;
	int c = 0;
		for (int f = 0; f < MAX_OBJ; f++) {
			for (int v = 0; v < n_vertex; v++) {
				VertexCubeColorModel.push_back(Vertex);
				VertexCubeColorModel[c].x= cube[f][v].fX - 1; //-1 to center middle of cube on 0,0
				VertexCubeColorModel[c].y= cube[f][v].fY - 1;
				VertexCubeColorModel[c].z= cube[f][v].fZ - 1;
				VertexCubeColor(c, f, v);
				c++;
			}
			vertCounter++;
		}
	}

	//TEXTURE:
	//-----------------------------------------------------------------------------------------
	{
		ModelTextureVertexType Vertex;
		int c = 0;
		for (int f=0;f<MAX_OBJ;f++) {
			for (int v=0;v<n_vertex;v++) {
				VertexCubeTextureModel.push_back(Vertex);
				VertexCubeTextureModel[c].x = cube[f][v].fX - 1;
				VertexCubeTextureModel[c].y = cube[f][v].fY - 1;
				VertexCubeTextureModel[c].z = cube[f][v].fZ - 1;
				VertexCubeTexture (c, f, v);
				c++;
			}
			vertCounter++;
		} 
	}

	//LIGHT:
	//-----------------------------------------------------------------------------------------
	{
		ModelTextureLightVertexType Vertex;
		int c = 0;
		for (int f = 0; f < MAX_OBJ; f++) {
			for (int v = 0; v < n_vertex; v++) {
				VertexCubeTextureLightModel.push_back(Vertex);
				VertexCubeTextureLightModel[c].x = cube[f][v].fX - 1;
				VertexCubeTextureLightModel[c].y = cube[f][v].fY - 1;
				VertexCubeTextureLightModel[c].z = cube[f][v].fZ - 1;
				VertexCubeTextureLight(c, f, v);
				c++;
			}
			vertCounter++;
		}
	}



	//-----------------------------------------------------------------------------------------
	for (int f = 0; f < MAX_OBJ; f++)
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
}

void CCube::VertexCubeColor(int c, int i, int f)
{
	if (i == 0)
		VertexCubeColorModel[c].r = 1;	//.dwColor = 0x00ff0000; //R
	else if (i == 1)
		VertexCubeColorModel[c].b = 1;	//.dwColor = 0x000000ff; //B
	else if (i == 2)
		VertexCubeColorModel[c].g = 1;	//.dwColor = 0x0000ff00; //G
	else if (i == 3)
	{
		VertexCubeColorModel[c].r = 1;	//dwColor = 0x00ffff00; //Y
		VertexCubeColorModel[c].g = 1;
	}
	else if (i == 4)
	{
		VertexCubeColorModel[c].g = 1;	//.dwColor = 0x0000ffff; //C
		VertexCubeColorModel[c].b = 1;	//.dwColor = 0x0000ffff; //C
	}
	else
	{
		VertexCubeColorModel[c].r = 1;	//.dwColor = 0x00ffffff; //W
		VertexCubeColorModel[c].g = 1;
		VertexCubeColorModel[c].b = 1;
	}
}

void CCube::VertexCubeTexture (int c, int i, int v)
{
	if (v==0) {
		VertexCubeTextureModel[c].tu= 0; 
		VertexCubeTextureModel[c].tv = 0;
	}
	if (v==1) {
		VertexCubeTextureModel[c].tu = 1; 
		VertexCubeTextureModel[c].tv = 0;
	}
	if (v==2) {
		VertexCubeTextureModel[c].tu = 0; 
		VertexCubeTextureModel[c].tv = 1;
	}
	if (v==3) {
		VertexCubeTextureModel[c].tu = 1; 
		VertexCubeTextureModel[c].tv = 1;
	}
}

void CCube::VertexCubeTextureLight(int c, int face, int v)
{
	if (v == 0) {
		VertexCubeTextureLightModel[c].tu = 0;
		VertexCubeTextureLightModel[c].tv = 0;
	}
	if (v == 1) {
		VertexCubeTextureLightModel[c].tu = 1;
		VertexCubeTextureLightModel[c].tv = 0;
	}
	if (v == 2) {
		VertexCubeTextureLightModel[c].tu = 0;
		VertexCubeTextureLightModel[c].tv = 1;
	}
	if (v == 3) {
		VertexCubeTextureLightModel[c].tu = 1;
		VertexCubeTextureLightModel[c].tv = 1;
	}

	if (face == 0) {
		VertexCubeTextureLightModel[c].nx = 0;
		VertexCubeTextureLightModel[c].ny = 0;
		VertexCubeTextureLightModel[c].nz = -1;
	}
	if (face == 1) {
		VertexCubeTextureLightModel[c].nx = 1;
		VertexCubeTextureLightModel[c].ny = 0;
		VertexCubeTextureLightModel[c].nz = 0;
	}
	if (face == 2) {
		VertexCubeTextureLightModel[c].nx = 0;
		VertexCubeTextureLightModel[c].ny = 0;
		VertexCubeTextureLightModel[c].nz = 1;
	}
	if (face == 3) {
		VertexCubeTextureLightModel[c].nx = -1;
		VertexCubeTextureLightModel[c].ny = 0;
		VertexCubeTextureLightModel[c].nz = 0;
	}
	if (face == 4) {
		VertexCubeTextureLightModel[c].nx = 0;
		VertexCubeTextureLightModel[c].ny = -1;
		VertexCubeTextureLightModel[c].nz = 0;
	}
	if (face == 5) {
		VertexCubeTextureLightModel[c].nx = 0;
		VertexCubeTextureLightModel[c].ny = 1;
		VertexCubeTextureLightModel[c].nz = 0;
	}

}