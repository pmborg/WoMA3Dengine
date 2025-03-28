// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: AutoGenTerrain.h
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
// PURPOSE:
// 
// -GenerateRandomHeightMapTerrain:   rand()			-> POPULATE height[][]
//  SaveBMPHeightMapTerrain:		  SAVE height[][]	-> (multiple) BMP
// -LoadHeightMapTerrain:			  LOAD BMP			-> POPULATE height[][]
// -PopulateTerrainModelVertexVector: height[][]		-> POPULATE modelVertexVector
// 
// --------------------------------------------------------------------------------------------

#pragma once

#include "platform.h"
#include "Math3D.h"
#include "vertexTypes.h"
#include "WomaDriverClass.h"
#include "virtualModelClass.h"

#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
#include "NewTerrain.h"
#endif

		#define terrain_squares 256
		#define MAP_CHUNK_SIZE 128

//C:\WoMAengine2014\_WORD_HEIGHT_MAPS_\NASA\Topography\gebco_08_rev_elev_21600x10800.png

#if defined SCENE_GENERATEDUNDERWATER
#define terrain_squareSize						4.0f // Era 8...
#define terrain_smoothsteps						40
#define terrain_layerdef_map_texture_size		1024
#define terrain_depth_shadow_map_texture_size	512

#define terrain_numpatches_1d					64
#define terrain_geometry_scale					1.0f
#define terrain_maxheight						30//.0f 
#define terrain_minheight						-30//.0f 
#define terrain_fractalfactor					0.68f;
#define terrain_fractalinitialvalue				100.0f
#define terrain_smoothfactor1					0.99f
#define terrain_smoothfactor2					0.10f
#define terrain_rockfactor						0.95f

#define terrain_height_underwater_start			-100.0f
#define terrain_height_underwater_end			-8.0f

#define terrain_height_sand_start				-30.0f
#define terrain_height_sand_end					1.7f

#define terrain_height_grass_start				1.7f
#define terrain_height_grass_end				30.0f

#define terrain_height_rocks_start				-2.0f

#define terrain_height_trees_start				4.0f
#define terrain_height_trees_end				30.0f

#define terrain_slope_grass_start				0.96f
#define terrain_slope_rocks_start				0.85f
#endif

enum MY_TERRAIN_TYPE
{																																						    //RASTERTEK TUTORIAL
	TERRAIN,//																																woma2013:ch10	Tutorial 1: Grid and Camera Movement
	TERRAIN_LIGHT,//																														woma2013:ch12	Tutorial 4: Terrain Lighting
	TERRAIN_COLOR,//																														woma2013:ch13	Tutorial 5: Color Mapped Terrain
	TERRAIN_COLOR_MAP,//

	//TERRAIN_WITH_HEIGHT,//																												woma2013:ch11	Tutorial 2: Bitmap Height Maps
	//TERRAIN_COLOR_QUAD,//																													woma2013:ch14   
	//TERRAIN_WATER_COLOR_QUAD,//                                                                                                           woma2013:ch28
	//TERRAIN_COLOR_QUAD_MINI_MAP,//																										woma2013:ch15
	//TERRAIN_COLOR_QUAD_FOG_MINI_MAP,//																									woma2013:ch16
	//TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_MINI_MAP,//																						woma2013:ch17
	//TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_MINI_MAP,//																		woma2013:ch18
	////_19
	//TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_MINI_MAP,//															woma2013:ch19
	//TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_MINI_MAP,//											woma2013:ch20
	////_21
	//TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_BumpMapping_MINI_MAP,//															woma2013:ch21
	//TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_MINI_MAP,//								woma2013:ch22
	//TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_MINI_MAP,//					woma2013:ch23
	TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_TransparentTexture_MINI_MAP//	woma2013:ch24

	//NOTE Update Also: #define N_MAX_TERRAINS TERRAIN_WATER_COLOR_QUAD + 1 // TERRAIN TYPES!
};

//VERTEX:
#define VertexTerrainType_13 ModelTextureVertexType

// ---------------------------------------------------------------------------
#define  SAVE_BMP_HM		0
#define  SAVE_BMP_TEXTURE	1

class CTerrain
{
public:

	// SOURCE DATA TYPES: http://msdn.microsoft.com/en-us/library/s3f49ktz.aspx

#define word16 uint16_t // unsigned __int16
#define word32 uint32_t // unsigned __int32

// NOTE: default struct align 16 + 32 = 64 bits!

//#define CACHE_LINE  16
//#define CACHE_ALIGN __declspec(align(CACHE_LINE))
	struct  BMPHeader_id
	{
		// BMP Header
		word16	Id;		//	BM - Windows 3.1x, 95, NT, 98, 2000, ME, XP
		//	BA - OS/2 Bitmap Array
		//	CI - OS/2 Color Icon
		//	CP - OS/2 Color Pointer
		//	IC - OS/2 Icon
		//	PT - OS/2 Pointer
	};

	struct  BMPHeader
	{
		word32	FileSize;
		word32	Reserved;
		word32	BitmapDataOffset;
		//4x4=16
	};

	// DIB Header
	struct  DIBHeader_size
	{
		word32	BitmapHeaderSize;	// should be 28h for windows bitmaps or 0Ch for OS/2 1.x or F0h for OS/2 2.x
		word32	Width;
		word32	Height;
	};
	//28

	struct  DIBHeader_planesBPP
	{
		word16	Planes;
		word16	BPP;				// 1: Monochrome bitmap
		// 4: 16 color bitmap
		// 8: 256 color bitmap
		// 16: 16bit (high color) bitmap
		// 24: 24bit (true color) bitmap
		// 32: 32bit (true color) bitmap
		//32
	};

	struct  DIBHeader_body
	{
		word32  Compression;		// 0: none (Also identified by BI_RGB)
		// 1: RLE 8-bit / pixel (Also identified by BI_RLE4)
		// 2: RLE 4-bit / pixel (Also identified by BI_RLE8)
		// 3: Bitfields  (Also identified by BI_BITFIELDS)

		word32  BitmapDataSize;		// Size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary.
		//40

		word32  PixelPerMeterX;
		word32  PixelPerMeterY;
		// 48
		word32  Colors;
		word32  ImportantColors;
		// 56
	};


	//CTerrain();
	CTerrain(MY_TERRAIN_TYPE m_terrainType);
	~CTerrain();

	// TERRAIN:
#if DX_ENGINE_LEVEL != 62 && defined SCENE_GENERATEDUNDERWATER
	void	GenerateRandomHeightMapTerrain(UINT randValue, bool Move_down_edges = false);
#endif
	int		GetVertexCount();

#if defined SCENE_GENERATEDUNDERWATER
	void	CopyVertexArray(UINT id, ModelTextureVertexType* vertexList);
#endif



	void PopulateTerrainModelVertexVector(UINT id, float unit);

	void SaveBMPHeightMapTerrain(CHAR* maps, UINT bmp_type = SAVE_BMP_HM);

	bool InBounds(int i, int j);
	float Average(int i, int j);
	void Terrain_Smooth();
	bool LoadHeightMapTerrain(TCHAR* filename, float xPos, float zPos, bool skipZero = false);

	//>=49:
	ModelTextureVertexType vertex0 = {};												// Use this "VERTEX" on macro
	std::vector<ModelTextureVertexType> modelVertexVector0;								// Declare: the Vector with Vertex "TYPE"

	//vertex1
	ModelTextureVertexType vertex1 = {};												// Use this "VERTEX" on macro
	std::vector<ModelTextureVertexType> modelVertexVector1;

	//vertex2
#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
	int m_indexCount = 0, m_vertexCount = 0;
	HeightMapType_24 vertex2 = {};									// Use this "VERTEX" on macro
	std::vector<HeightMapType_24> modelVertexVector2;				// Declare: the Vector with Vertex "TYPE"
	std::vector<UINT> Terrain60indices;
	std::vector<STRING> Terrain60Textures;
#else
	ModelTextureDouble_Color_TerrainType vertex2 = {};									// Use this "VERTEX" on macro
	std::vector<ModelTextureDouble_Color_TerrainType> modelVertexVector2;				// Declare: the Vector with Vertex "TYPE"

#endif

	//vertex3
	#if DX_ENGINE_LEVEL >= 52 && defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
	ModelTextureVertexType vertex3 = {};												// Use this "VERTEX" on macro
	std::vector<ModelTextureVertexType> modelVertexVector3;
	#endif

	//TERRAINs:
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN //24
	void		initUnderWaterDemo		(UINT terrainId);
#endif
#if defined SCENE_WATER_TERRAIN
	bool		initTerrainWaterMeshDemo(UINT terrainId);
#endif
	void		NormalizeHeightMap(float scale, float moveY);
#if defined SCENE_MAIN_TOPO_TERRAIN
	bool		initMainTopoTerrainDemo	(UINT terrainId);
	float				m_xPos, m_zPos;
	float				m_xCenterPos, m_zCenterPos, m_radius;
	HeightMapType_21*	m_heightMap_21 = NULL;
	bool		CalculateNormals();
	bool		BuildTerrainModel();
	void		CalculateTerrainVectors();
	void		CalculateTextureCoordinates(float textureRepeat);
	bool		LoadColorMap(char* filename);
	bool		CreateTerrain60(float xPos, float zPos);
#endif
#if defined SCENE_MAIN_TERRAIN
	void		initMainTerrainDemo(UINT terrainId);
#endif
	void		CreateTerrainModel(UINT id, std::vector<STRING> Textures, SHADER_TYPE shader_type);

#if defined SCENE_TERRAIN_COLLISION
	float	getTerrainHeight(UINT terrainId, float xPos, float zPos);
	//bool	CheckHeightOfTriangle(float x, float z, float& height, float v0[3], float v1[3], float v2[3]);
	bool	CheckHeightOfTrianglev2(float x, float z, float& height, float v0[3], float v1[3], float v2[3]);
#endif

	void	CalculateMaxMin();

	// VARS:
	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
	};

	float height[terrain_squares][terrain_squares] = {};	// Auto Generated data

	MY_TERRAIN_TYPE	m_terrainType;
	UINT			m_terrainWidth = 0, m_terrainHeight = 0;
	bool			loaded = false;

private:
	void	DUMP_TEXT_Version(TCHAR* filename);
};

