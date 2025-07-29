// --------------------------------------------------------------------------------------------
// Filename: NewTerrain.h
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

#pragma once

struct HeightMapType_21
{
	float x, y, z;		//POSITION
	float tu, tv;		//TEXCOORD0
	float nx, ny, nz;	//NORMAL
	float r, g, b;		//COLOR
	float Maptu, Maptv;	//TEXCOORD1
	float tx, ty, tz;	//TANGENT
	float bx, by, bz;	//BINORMAL
};

struct TerrainModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
	float tx, ty, tz;
	float bx, by, bz;
};

struct TempVertexType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

#define TERRAINmodelUPPERleft(m_heightMap){\
	/* Upper left.*/\
	m_TerrainModel[index].x  = m_heightMap[index3].x;\
	m_TerrainModel[index].y  = m_heightMap[index3].y;\
	m_TerrainModel[index].z  = m_heightMap[index3].z;\
	m_TerrainModel[index].nx = m_heightMap[index3].nx;\
	m_TerrainModel[index].ny = m_heightMap[index3].ny;\
	m_TerrainModel[index].nz = m_heightMap[index3].nz;\
	m_TerrainModel[index].tu = 0.0f;\
	m_TerrainModel[index].tv = 0.0f;\
	index++;\
}


#define TERRAINmodelUPPERright(m_heightMap){\
	/* Upper right.*/\
	m_TerrainModel[index].x  = m_heightMap[index4].x;\
	m_TerrainModel[index].y  = m_heightMap[index4].y;\
	m_TerrainModel[index].z  = m_heightMap[index4].z;\
	m_TerrainModel[index].nx = m_heightMap[index4].nx;\
	m_TerrainModel[index].ny = m_heightMap[index4].ny;\
	m_TerrainModel[index].nz = m_heightMap[index4].nz;\
	m_TerrainModel[index].tu = 1.0f;\
	m_TerrainModel[index].tv = 0.0f;\
	index++;\
}

#define TERRAINmodelBOTTOMleft(m_heightMap){\
	/* Bottom left.*/\
	m_TerrainModel[index].x  = m_heightMap[index1].x;\
	m_TerrainModel[index].y  = m_heightMap[index1].y;\
	m_TerrainModel[index].z  = m_heightMap[index1].z;\
	m_TerrainModel[index].nx = m_heightMap[index1].nx;\
	m_TerrainModel[index].ny = m_heightMap[index1].ny;\
	m_TerrainModel[index].nz = m_heightMap[index1].nz;\
	m_TerrainModel[index].tu = 0.0f;\
	m_TerrainModel[index].tv = 1.0f;\
	index++;\
}

#define TERRAINmodelBOTTOMright(m_heightMap){\
	/* Bottom right.*/\
	m_TerrainModel[index].x  = m_heightMap[index2].x;\
	m_TerrainModel[index].y  = m_heightMap[index2].y;\
	m_TerrainModel[index].z  = m_heightMap[index2].z;\
	m_TerrainModel[index].nx = m_heightMap[index2].nx;\
	m_TerrainModel[index].ny = m_heightMap[index2].ny;\
	m_TerrainModel[index].nz = m_heightMap[index2].nz;\
	m_TerrainModel[index].tu = 1.0f;\
	m_TerrainModel[index].tv = 1.0f;\
	index++;\
}
/*
#define MAPTEXTUREcoordinate_19(firstTme, m_heightMap){\
	if (firstTme) \
	{\
		m_heightMap[(m_terrainWidth * j) + i].tu = tuCoordinate;\
		m_heightMap[(m_terrainWidth * j) + i].tv = tvCoordinate;\
	} \
	else\
	{\
		m_heightMap[(m_terrainWidth * j) + i].Maptu = tuCoordinate;\
		m_heightMap[(m_terrainWidth * j) + i].Maptv = tvCoordinate;\
	}\
}
*/

// Index Map:
//  3-----4
//	|   / |
//	|  /  |
//	| /   |
//  1-----2
#define MAP_Upper_left_19(m_heightMap, m_vertices){\
		tv = m_heightMap[index3].tv;\
		\
		/* Modify the texture coordinates to cover the top edge.*/\
		if(tv == 1.0f) { tv = 0.0f; }\
		m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);\
		m_vertices[index].texture = D3DXVECTOR4(m_heightMap[index3].tu, tv, 0.0f, 0.0f);\
		m_vertices[index].mappingTexture = D3DXVECTOR4(m_heightMap[index3].Maptu, m_heightMap[index3].Maptv, 0.0f, 0.0f);\
		m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);\
		\
		/*The vertex buffer now has a color component. So for each vertex we create in the buffer we */\
		/*load in the color map information that is stored in the height map array.*/\
		\
		m_vertices[index].color = D3DXVECTOR4(m_heightMap[index3].r, m_heightMap[index3].g, m_heightMap[index3].b, 1.0f);\
}

#define MAP_Upper_right_19(m_heightMap, m_vertices){\
		tu = m_heightMap[index4].tu;\
		tv = m_heightMap[index4].tv;\
		\
		/* Modify the texture coordinates to cover the top and right edge.*/\
		if(tu == 0.0f) { tu = 1.0f; }\
		if(tv == 1.0f) { tv = 0.0f; }\
		\
		m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);\
		m_vertices[index].texture = D3DXVECTOR4(tu, tv, 1.0f, 0.0f);\
		m_vertices[index].mappingTexture = D3DXVECTOR4(m_heightMap[index4].Maptu, m_heightMap[index4].Maptv, 1.0f, 0.0f);\
		m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);\
		m_vertices[index].color = D3DXVECTOR4(m_heightMap[index4].r, m_heightMap[index4].g, m_heightMap[index4].b, 1.0f);\
}

#define MAP_Bottom_left_19(m_heightMap, m_vertices){\
		m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);\
		m_vertices[index].texture = D3DXVECTOR4(m_heightMap[index1].tu, m_heightMap[index1].tv, 0.0f, 1.0f);\
		\
		m_vertices[index].mappingTexture = D3DXVECTOR4(m_heightMap[index1].Maptu, m_heightMap[index1].Maptv, 0.0f, 1.0f);\
		m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);\
		m_vertices[index].color = D3DXVECTOR4(m_heightMap[index1].r, m_heightMap[index1].g, m_heightMap[index1].b, 1.0f);\
}

#define MAP_Bottom_right_19(m_heightMap, m_vertices){\
		tu = m_heightMap[index2].tu;\
		\
		/* Modify the texture coordinates to cover the right edge.*/\
		if(tu == 0.0f) { tu = 1.0f; }\
		\
		m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);\
		m_vertices[index].texture = D3DXVECTOR4(tu, m_heightMap[index2].tv, 1.0f, 1.0f);\
		m_vertices[index].mappingTexture = D3DXVECTOR4(m_heightMap[index2].Maptu, m_heightMap[index2].Maptv, 1.0f, 1.0f);\
		m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);\
		m_vertices[index].color = D3DXVECTOR4(m_heightMap[index2].r, m_heightMap[index2].g, m_heightMap[index2].b, 1.0f);\
}

