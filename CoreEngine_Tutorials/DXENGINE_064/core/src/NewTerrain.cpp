// --------------------------------------------------------------------------------------------
// Filename: NewTerrain.cpp
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

#pragma warning( disable : 4018 ) //'<': signed/unsigned mismatch

#include "platform.h"
#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
#include "OSengine.h"				// To define OS [SystemHandle] Pointer (System Class) & define WomaSYSTEM for: WINDOWS, LINUX & ANDROID
#include "AutoGenTerrain.h"		
#include "DXmodelClass.h"
#include "NewTerrain.h"
#include "mem_leak.h"

#define D3DXVECTOR2 XMFLOAT2
#define D3DXVECTOR3 XMFLOAT3
#define D3DXVECTOR4 XMFLOAT4

#undef  TERRAIN_SLOP_GRASS_TEXTURE
#undef  TERRAIN_SLOP_ROCK_TEXTURE
#undef  TERRAIN_SLOP_SLOPE_TEXTURE
#undef	TERRAIN_SLOP_ROCK2_TEXTURE

#define TERRAIN_SLOP_GRASS_TEXTURE			  TEXT("engine/data/scene60DEMO/004grass_texture.bmp")		//reg0: COLOR BMP R G B
#define TERRAIN_SLOP_SLOPE_TEXTURE			  TEXT("engine/data/scene60DEMO/017slope.jpg")				//reg1: texture2
#define TERRAIN_SLOP_ROCK_TEXTURE			  TEXT("engine/data/scene60DEMO/017rock.jpg")				//reg2: texture3
#define TERRAIN_SLOP_DETAIL_TEXTURE			  TEXT("engine/data/scene60DEMO/017grass.jpg")				//reg3: texture4/detail
#define TERRAIN_SLOP_TEXTUREMAPPING_TEXTURE	  TEXT("engine/data/scene60DEMO/t_019TerrainMapping.png")	//reg4: textureMappingTexture
#define TERRAIN_SLOP_SIDEWALK_TEXTURE		  TEXT("engine/data/scene60DEMO/019sidewalk.jpg")			//reg5: sidewalkTexture
#define TERRAIN_SLOP_SAND_TEXTURE			  TEXT("engine/data/scene60DEMO/019sand.jpg")				//reg6: sandTexture
#define TERRAIN_SLOP_MUD_TEXTURE			  TEXT("engine/data/scene60DEMO/CrackedMud.jpg")			//reg7: mudTexture
#define TERRAIN_SLOP_ALPHAMAPPING_TEXTURE	  TEXT("engine/data/scene60DEMO/t_019TerrainAlfa.png")		//reg8: alphaMappingTexture
#define TERRAIN_SLOP_NORMAL_TEXTURE			  TEXT("engine/data/scene60DEMO/dirt_bump.jpg")				//reg9: sidewalkBumpTexture
#define TERRAIN_SLOP_SHADER_TEXTURE			  TEXT("engine/data/scene60DEMO/t_023TerrainMapping.jpg")	//reg10: colorLightMapTexture
/* world_60.xml
  <world hVisibility="128" seaLevel="0" size="512" patchSize="64"
		 water="engine/data/Terrain/heightmap256x256.bmp"
		 waterTexture="engine/data/Terrain/008water_tex04.jpg"
		 mainTexture="engine/data/scene60DEMO/t_011map1_heightmap01_v7.bmp"
		 skyDayTexture="engine/data/sky/desert_equi.jpgd5a2d427-55de-4d44-a8ec-5ba440b11e2fZoom.jpg"
		 skyNightTexture=""/>*/

#undef TERRAIN_SLOP_GRASS_TEXTURE			  
#undef TERRAIN_SLOP_SLOPE_TEXTURE			  
#undef TERRAIN_SLOP_ROCK_TEXTURE			  
#undef TERRAIN_SLOP_DETAIL_TEXTURE			  
#undef TERRAIN_SLOP_TEXTUREMAPPING_TEXTURE	  
#undef TERRAIN_SLOP_SIDEWALK_TEXTURE		  
#undef TERRAIN_SLOP_SAND_TEXTURE			  
#undef TERRAIN_SLOP_MUD_TEXTURE			  
#undef TERRAIN_SLOP_ALPHAMAPPING_TEXTURE	  
#undef TERRAIN_SLOP_NORMAL_TEXTURE			  
#undef TERRAIN_SLOP_SHADER_TEXTURE			  

#define TERRAIN_COLOR_MAP_TEXTURE			  TEXT("engine/data/scene61DEMO/Grass512.bmp")					 //BMP: COLOR MAP R G B
		 //       VAR NAME: C++:
#define TERRAIN_SLOP_TEXTURE				  TEXT("engine/data/scene61DEMO/Grass.jpg")						 //reg0:  m_Texture
#define TERRAIN_SLOP_SLOPE_TEXTURE			  TEXT("engine/data/scene60DEMO/017slope.jpg")					 //reg1:  m_SlopeTexture
#define TERRAIN_SLOP_ROCK_TEXTURE			  TEXT("engine/data/scene61DEMO/rock02_savdf.jpg")				 //reg2:  m_RockTexture
#define TERRAIN_SLOP_DETAIL_TEXTURE			  TEXT("engine/data/scene60DEMO/017grass.jpg")					 //reg3:  m_DetailTexture
#define TERRAIN_SLOP_TEXTUREMAPPING_TEXTURE	  TEXT("engine/data/scene61DEMO/t_019TerrainMapping.png")		 //reg4:  m_textureMappingTexture
											  //will mix: reg0 reg3

#define TERRAIN_SLOP_SIDEWALK_TEXTURE		  TEXT("engine/data/scene61DEMO/stone001.dds")					 //reg5:  m_sidewalkTexture

#define TERRAIN_SLOP_SAND_TEXTURE			  TEXT("engine/data/scene61DEMO/019sandV2.jpg")					 //reg6:  m_sandTexture
#define TERRAIN_SLOP_MUD_TEXTURE			  TEXT("engine/data/scene61DEMO/seamless_dry_mud_textureV2.jpg") //reg7:  m_mudTexture
#define TERRAIN_SLOP_ALPHAMAPPING_TEXTURE	  TEXT("engine/data/scene61DEMO/t_025TerrainMappingV2.png")		 //reg8:  m_alphaMappingTexture
#define TERRAIN_SLOP_NORMAL_TEXTURE			  TEXT("engine/data/scene61DEMO/stone001normal.dds")			 //reg9:  m_sidewalkTexture_bump
#define TERRAIN_SLOP_SHADER_TEXTURE			  TEXT("engine/data/scene61DEMO/normal001.dds")					 //reg10: m_generalNormalTexture
#define TERRAIN_grassNormalTexture            TEXT("engine/data/scene61DEMO/t_025TerrainMappingV3.png")		 //reg11: m_grassNormalTexture
#define TERRAIN_smallstonePathTexture		  TEXT("engine/data/scene61DEMO/011map1_dirt01.jpg")			 //reg12: m_smallstonePathTexture
#define TERRAIN_colorLightMapTexture		  TEXT("engine/data/scene61DEMO/t_023TerrainMappingV2.jpg")		 //reg13: m_colorLightMapTexture

/* world_60.xml
  <world hVisibility="128" seaLevel="0" size="512" patchSize="64"
		 water="engine/data/Terrain/heightmap256x256.bmp"
		 waterTexture="engine/data/Terrain/008water_tex04.jpg"
		 mainTexture="engine/data/scene60DEMO/t_011map1_heightmap01_v7.bmp"
		 skyDayTexture="engine/data/sky/desert_equi.jpgd5a2d427-55de-4d44-a8ec-5ba440b11e2fZoom.jpg"
		 skyNightTexture=""/>*/

const int TEXTURE_REPEAT = 16; //<---------
TerrainModelType* m_TerrainModel;
DXVertexTerrainType_21* m_vertices_21;


//To calculate shared normals you have to go through each vertex in the terrain grid and take an average of the normals for each face that the vertex is a part of. 
//Then each vertex in the grid will now be averaged to everything around it creating less abrupt changes in light direction giving the illusion of smoother surfaces. 
//This technique works well with any smooth or semi-smooth polygon surface. However it does not work well with surfaces that have sharp edges such as a cube.
bool CTerrain::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;

	// Create a temporary array to hold the un-normalized normal vectors.
	normals = NEW VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)return false;

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j < (m_terrainHeight - 1); j++)
	{
		for (i = 0; i < (m_terrainWidth - 1); i++)
		{
			index1 = (j * m_terrainWidth) + i;
			index2 = (j * m_terrainWidth) + (i + 1);
			index3 = ((j + 1) * m_terrainWidth) + i;

			//COPYheightMapTOvertex(m_heightMap):
			/* Get three vertices from the face.*/
			vertex1[0] = m_heightMap_21[index1].x;
			vertex1[1] = m_heightMap_21[index1].y;
			vertex1[2] = m_heightMap_21[index1].z;
			
			vertex2[0] = m_heightMap_21[index2].x;
			vertex2[1] = m_heightMap_21[index2].y;
			vertex2[2] = m_heightMap_21[index2].z;
			
			vertex3[0] = m_heightMap_21[index3].x;
			vertex3[1] = m_heightMap_21[index3].y;
			vertex3[2] = m_heightMap_21[index3].z;


			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainWidth - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = FAST_sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainWidth) + i;

			m_heightMap_21[index].nx = (sum[0] / length);
			m_heightMap_21[index].ny = (sum[1] / length);
			m_heightMap_21[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	SAFE_DELETE_ARRAY (normals);

	return true;
}

//The BuildTerrainModel function builds the terrain triangles the same way the InitializeBuffers function used to. 
//But instead of creating a vertex and index buffer we load the terrain data into a ModelType structure array.
bool CTerrain::BuildTerrainModel()
{
	int i, j, index, index1, index2, index3, index4;


	// Set the number of vertices in the model.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	// Create the terrain model array.
	IF_NOT_RETURN_FALSE(m_TerrainModel = NEW TerrainModelType[m_vertexCount]);

	// Load the terrain model with the height map terrain data.
	index = 0;

	for (j = 0; j < (m_terrainHeight - 1); j++)
	{
		for (i = 0; i < (m_terrainWidth - 1); i++)
		{
			index1 = (m_terrainWidth * j) + i;          // Bottom left.
			index2 = (m_terrainWidth * j) + (i + 1);      // Bottom right.
			index3 = (m_terrainWidth * (j + 1)) + i;      // Upper left.
			index4 = (m_terrainWidth * (j + 1)) + (i + 1);  // Upper right.

			//Populate: m_TerrainModel[index] x,y,z,nx,ny,nz,tu,tv

			// Upper left.
			TERRAINmodelUPPERleft(m_heightMap_21);

			// Upper right.
			TERRAINmodelUPPERright(m_heightMap_21);

			// Bottom left.
			TERRAINmodelBOTTOMleft(m_heightMap_21);

			// Bottom left.
			TERRAINmodelBOTTOMleft(m_heightMap_21);

			// Upper right.
			TERRAINmodelUPPERright(m_heightMap_21);

			// Bottom right.
			TERRAINmodelBOTTOMright(m_heightMap_21);

		}
	}

	return true;
}


void CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = FAST_sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = FAST_sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}




//CalculateTerrainVectors is the function that goes through each triangle in the terrain model and calculates and 
//stores the tangent and binormal vectors.
void CTerrain::CalculateTerrainVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal;


	// Calculate the number of faces in the terrain model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the terrain model.
		vertex1.x = m_TerrainModel[index].x;
		vertex1.y = m_TerrainModel[index].y;
		vertex1.z = m_TerrainModel[index].z;
		vertex1.tu = m_TerrainModel[index].tu;
		vertex1.tv = m_TerrainModel[index].tv;
		vertex1.nx = m_TerrainModel[index].nx;
		vertex1.ny = m_TerrainModel[index].ny;
		vertex1.nz = m_TerrainModel[index].nz;
		index++;

		vertex2.x = m_TerrainModel[index].x;
		vertex2.y = m_TerrainModel[index].y;
		vertex2.z = m_TerrainModel[index].z;
		vertex2.tu = m_TerrainModel[index].tu;
		vertex2.tv = m_TerrainModel[index].tv;
		vertex2.nx = m_TerrainModel[index].nx;
		vertex2.ny = m_TerrainModel[index].ny;
		vertex2.nz = m_TerrainModel[index].nz;
		index++;

		vertex3.x = m_TerrainModel[index].x;
		vertex3.y = m_TerrainModel[index].y;
		vertex3.z = m_TerrainModel[index].z;
		vertex3.tu = m_TerrainModel[index].tu;
		vertex3.tv = m_TerrainModel[index].tv;
		vertex3.nx = m_TerrainModel[index].nx;
		vertex3.ny = m_TerrainModel[index].ny;
		vertex3.nz = m_TerrainModel[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Store the tangent and binormal for this face back in the model structure.
		m_TerrainModel[index - 1].tx = tangent.x;
		m_TerrainModel[index - 1].ty = tangent.y;
		m_TerrainModel[index - 1].tz = tangent.z;
		m_TerrainModel[index - 1].bx = binormal.x;
		m_TerrainModel[index - 1].by = binormal.y;
		m_TerrainModel[index - 1].bz = binormal.z;

		m_TerrainModel[index - 2].tx = tangent.x;
		m_TerrainModel[index - 2].ty = tangent.y;
		m_TerrainModel[index - 2].tz = tangent.z;
		m_TerrainModel[index - 2].bx = binormal.x;
		m_TerrainModel[index - 2].by = binormal.y;
		m_TerrainModel[index - 2].bz = binormal.z;

		m_TerrainModel[index - 3].tx = tangent.x;
		m_TerrainModel[index - 3].ty = tangent.y;
		m_TerrainModel[index - 3].tz = tangent.z;
		m_TerrainModel[index - 3].bx = binormal.x;
		m_TerrainModel[index - 3].by = binormal.y;
		m_TerrainModel[index - 3].bz = binormal.z;
	}

	return;
}

//CalculateTextureCoordinates generates texture coordinates for the terrain grid. The new global TEXTURE_REPEAT is used to determine how often the texture should repeat over the grid. 
//We use TEXTURE_REPEAT to calculate the incremental texture coordinates for each vertex in the terrain grid and then store it in the height map array.

//Note that since we are using texture coordinates per vertex we will not handle the edges properly. For example if we have a texture that repeats every two quads 
//we would only get two texture coordinates 0.0f and 0.5f. When really we need to have 0.0f, 0.5f, and 1.0f. To handle this issue we just determine when the edge coordinates 
//should be 0.0f or 1.0f when building the vertex array in the InitializeBuffers function.
void CTerrain::CalculateTextureCoordinates(float textureRepeat)
{
	//float firstTme=1;
	static float firstTme = 1;
	//static float firstTme = 0;

	int i, j, tuCount, tvCount;
	float tuCoordinate, tvCoordinate;

	float incrementValue = textureRepeat / (float)m_terrainWidth;	// Calculate how much to increment the texture coordinates by.
	int incrementCount = m_terrainWidth / (int)textureRepeat;		// Calculate how many times to repeat the texture.

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{

			// Store the texture coordinate in the height map.
			//MAPTEXTUREcoordinate_19(firstTme, m_heightMap_21);
			if (firstTme) 
			{
				m_heightMap_21[(m_terrainWidth * j) + i].tu = tuCoordinate;
				m_heightMap_21[(m_terrainWidth * j) + i].tv = tvCoordinate;
			} 
			else
			{
				m_heightMap_21[(m_terrainWidth * j) + i].Maptu = tuCoordinate;
				m_heightMap_21[(m_terrainWidth * j) + i].Maptv = tvCoordinate;
			}


			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}
	firstTme = false;
}

//LoadColorMap is very similar to the LoadHeightMap function. It opens a bitmap file and loads in color component into the height map structure array.
bool CTerrain::LoadColorMap(char* filename)
{
	int error, imageSize, i, j, k, index, colorMapWidth, colorMapHeight;
	FILE* filePtr;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapImage;

	// Open the color map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)return false;

	// Read in the file header.
	count = (UINT) fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)return false;

	// Read in the bitmap info header.
	count = (UINT)fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)return false;

	//For simplicity I ensure that the color map size is the same as the height map size. 
	//If you would like to use any size color map and have it stretch, 
	//shrink, or interpolate you can remove this check and modify the code below.

	// Make sure the color map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
	colorMapWidth = bitmapInfoHeader.biWidth;
	colorMapHeight = bitmapInfoHeader.biHeight;

	if((colorMapWidth != m_terrainWidth) || (colorMapHeight != m_terrainHeight))
			return false;

	// Calculate the size of the bitmap image data.
	imageSize = colorMapWidth * colorMapHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = NEW unsigned char[imageSize];
	if(!bitmapImage)return false;

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = (UINT) fread(bitmapImage, 1, imageSize, filePtr);
	if(count != (UINT)imageSize)return false;

	// Close the file.
	if (filePtr)
		error = fclose(filePtr);
	if(error != 0)return false;

	//The color map data is read into the r, g, and b components of the height map array.

	// Initialize the position in the image data buffer.
	k=0;

	// Read the image data into the color map portion of the height map structure.
	for(j=0; j<colorMapHeight; j++)
	{
		for(i=0; i<colorMapWidth; i++)
		{
			index = (colorMapWidth * j) + i;
				m_heightMap_21[index].b = (float)bitmapImage[k]   / 255.0f;
				m_heightMap_21[index].g = (float)bitmapImage[k+1] / 255.0f;
				m_heightMap_21[index].r = (float)bitmapImage[k+2] / 255.0f;
			k+=3;
		}
	}

	// Release the bitmap image data.
	delete [] bitmapImage;
	bitmapImage = 0;

	return true;
}

bool CTerrain::CreateTerrain60(float xPos, float zPos) 
{
	//Populate m_heightMap_21[index].nx...ny...nz
	IF_NOT_RETURN_FALSE (CalculateNormals());

	//Populate: m_TerrainModel[index] x,y,z,nx,ny,nz,tu,tv
	IF_NOT_RETURN_FALSE (BuildTerrainModel());

	// Calculate the normal, tangent, and binormal vectors for the terrain model.
	//Populate m_TerrainModel[index - 1].tx = tangent.x...y...z
	//Populate m_TerrainModel[index - 1].bx = binormal.x...y...z
	CalculateTerrainVectors();

	// We now generate our own terrain texture coordinates for the terrain grid during the Initialize function.
	// Calculate the texture coordinates.

	//[6]RASTER CalculateTextureCoordinates();
	//Populate: m_heightMap_21[(m_terrainWidth * j) + i].tu...tv
	//Populate: m_heightMap_21[(m_terrainWidth * j) + i].Maptu...Maptv
	CalculateTextureCoordinates((float)TEXTURE_REPEAT); // not needed? AQUI!

	CalculateTextureCoordinates(1);

	m_xPos = xPos;
	m_zPos = zPos;
	m_xCenterPos = (xPos + m_terrainWidth) / 2;
	m_zCenterPos = (zPos + m_terrainHeight) / 2;
	m_radius = FAST_sqrt(((float)m_terrainWidth / 2) * (float)(m_terrainWidth / 2) + ((float)m_terrainHeight / 2) * ((float)m_terrainHeight / 2));

	Terrain60Textures.push_back(TERRAIN_SLOP_TEXTURE);					//0:
	Terrain60Textures.push_back(TERRAIN_SLOP_SLOPE_TEXTURE);			//1:
	Terrain60Textures.push_back(TERRAIN_SLOP_ROCK_TEXTURE);				//2:
	Terrain60Textures.push_back(TERRAIN_SLOP_DETAIL_TEXTURE);			//3:
	Terrain60Textures.push_back(TERRAIN_SLOP_TEXTUREMAPPING_TEXTURE);	//4:
	Terrain60Textures.push_back(TERRAIN_SLOP_SIDEWALK_TEXTURE);			//5:
	Terrain60Textures.push_back(TERRAIN_SLOP_SAND_TEXTURE);				//6:
	Terrain60Textures.push_back(TERRAIN_SLOP_MUD_TEXTURE);				//7:

	Terrain60Textures.push_back(TERRAIN_SLOP_ALPHAMAPPING_TEXTURE);		//8:
	Terrain60Textures.push_back(TERRAIN_SLOP_NORMAL_TEXTURE);			//9:
	Terrain60Textures.push_back(TERRAIN_SLOP_SHADER_TEXTURE);			//10

	Terrain60Textures.push_back(TERRAIN_grassNormalTexture);			//11
	Terrain60Textures.push_back(TERRAIN_smallstonePathTexture);			//12
	Terrain60Textures.push_back(TERRAIN_colorLightMapTexture);			//13

	//Populate m_heightMap_21[index].b...g...r
	LoadColorMap(TERRAIN_COLOR_MAP_TEXTURE);

	//POPULATE m_vertices_21:
	m_vertices_21 = NEW DXVertexTerrainType_21[m_vertexCount];

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	//indices = NEW unsigned long[m_indexCount];
	std::vector<UINT> indices;

	int index = 0, i = 0, j = 0, index1 = 0, index2 = 0, index3 = 0, index4 = 0;
	float tu=0, tv = 0;

	// Load the vertex and index array with the terrain data.
	for (j = 0; j < (m_terrainHeight - 1); j++)
	{
		for (i = 0; i < (m_terrainWidth - 1); i++)
		{
			index1 = (m_terrainWidth * j) + i;				// Bottom left.
			index2 = (m_terrainWidth * j) + (i + 1);		// Bottom right.
			index3 = (m_terrainWidth * (j + 1)) + i;		// Upper left.
			index4 = (m_terrainWidth * (j + 1)) + (i + 1);  // Upper right.

			// Index Map:
			//  3-----4
			//	|   / |
			//	|  /  |
			//	| /   |
			//  1-----2

			// 1 Upper left:
			//------------------------------------------------------------------------------------------------------
			MAP_Upper_left_19(m_heightMap_21, m_vertices_21);
			Terrain60indices.push_back(index++);

			// 2 Upper right:
			//------------------------------------------------------------------------------------------------------
			MAP_Upper_right_19(m_heightMap_21, m_vertices_21);
			Terrain60indices.push_back(index++);

			// 3 Bottom left:
			//------------------------------------------------------------------------------------------------------
			MAP_Bottom_left_19(m_heightMap_21, m_vertices_21);
			Terrain60indices.push_back(index++);

			// 1 Bottom left:
			//------------------------------------------------------------------------------------------------------
			MAP_Bottom_left_19(m_heightMap_21, m_vertices_21);
			Terrain60indices.push_back(index++);

			// 2 Upper right:
			//------------------------------------------------------------------------------------------------------
			MAP_Upper_right_19(m_heightMap_21, m_vertices_21);
			Terrain60indices.push_back(index++);

			// 3 Bottom right:
			//------------------------------------------------------------------------------------------------------
			MAP_Bottom_right_19(m_heightMap_21, m_vertices_21);
			Terrain60indices.push_back(index++);
		}
	}

	// POPULATED above:
	//m_vertices[index].position = D3DXVECTOR3
	//m_vertices[index].texture = D3DXVECTOR4
	//m_vertices[index].normal = D3DXVECTOR3
	//m_vertices[index].color = D3DXVECTOR4
	//m_vertices[index].mappingTexture = D3DXVECTOR4

	DXVertexTerrainType_21* dx_vertice = NULL;
	TerrainModelType* terrain_vertice = NULL;

	for (size_t i = 0; i < m_vertexCount; i++)
	{
		dx_vertice = &m_vertices_21[i];
		terrain_vertice = &m_TerrainModel[i];

		//float woma type:
		HeightMapType_24 woma_vertice(
			dx_vertice->position.x, dx_vertice->position.y, dx_vertice->position.z,
			dx_vertice->texture.x, dx_vertice->texture.y, dx_vertice->texture.z, dx_vertice->texture.w,
			dx_vertice->normal.x, dx_vertice->normal.y, dx_vertice->normal.z,
			dx_vertice->color.x, dx_vertice->color.y, dx_vertice->color.z,
			dx_vertice->mappingTexture.x, dx_vertice->mappingTexture.y, dx_vertice->mappingTexture.z, dx_vertice->mappingTexture.w,
			terrain_vertice->tx, terrain_vertice->ty, terrain_vertice->tz,
			terrain_vertice->bx, terrain_vertice->by, terrain_vertice->bz
		);
		modelVertexVector2.push_back(woma_vertice);
	}

	SAFE_DELETE(m_heightMap_21);
	SAFE_DELETE(m_vertices_21);
	SAFE_DELETE(m_TerrainModel);

	CREATE_MODEL_IF_NOT_EXCEPTION(SystemHandle->m_Application->m_TerrainModel[MAIN_TERRAIN_ID], I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);  // m_TerrainModel[id] = NEW
	SystemHandle->m_Application->m_TerrainModel[MAIN_TERRAIN_ID]->ModelHASfog = true;
	
	ASSERT(SystemHandle->m_Application->m_TerrainModel[MAIN_TERRAIN_ID]->LoadTerrain(TEXT("Terrain 61"), m_Driver, SHADER_Terrain_Texture_DEMO61, &Terrain60Textures, &modelVertexVector2, &Terrain60indices));

	return true;
}
#endif

