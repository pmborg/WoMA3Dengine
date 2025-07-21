// --------------------------------------------------------------------------------------------
// Filename: AutoGenTerrain.cpp
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
//WomaIntegrityCheck = 1234525217;

#include "platform.h"

#pragma warning(disable : 4244) // warning C4244: '=': conversion from 'int' to 'float', possible
#include <stdint.h>
#include "Math3D.h"
#include "AutoGenTerrain.h"			
#include "TrigonometryMathClass.h"	//sim, cos table
#include "mem_leak.h"
#include "ImageLoaderClass.h"
#include "OSengine.h"				// To define OS [SystemHandle] Pointer (System Class) & define WomaSYSTEM for: WINDOWS, LINUX & ANDROID
#include "winsystemclass.h"			// SystemHandle
#include "fileLoader.h"
#include "ApplicationClass.h"

// ---------------------------------------------------------------------------
// LOAD HEIGHT MAP
// ---------------------------------------------------------------------------

// NOTE: Original version from Rastertek.

// LoadHeightMapTerrain is a new function that loads the bitmap file containing the height map into the new height map array. 
// If you want to use a more optimal file structure like .raw 
// you can change the code to load that in instead. For simplicity however I used the bitmap format since it is very common 
// and most people have worked with it before. 
// Note that bitmap contains red, green, and blue colors. 
// But since this is a grey scale image you can read either the red, green, or blue color 
// as they will all be the same grey value and you only need one of them.
bool CTerrain::LoadHeightMapTerrain(TCHAR* file, float xPos, float zPos, bool skipZero)
{
	UINT imageSize = 0;
	unsigned char* bitmapImage = NULL;
	STRING filename;
	filename.append(WOMA::LoadFile(file));

#if D3D11_SPEC_DATE_YEAR == 2009 || LEVEL >= 60
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;

	//[1] Begin by opening the file and then read it into a unsigned char array. Close the file after we are finished reading the data from it.
	//	  Open the height map file in binary.
	error = _tfopen_s(&filePtr, (TCHAR*)filename.c_str(), TEXT("rb"));
	if (error != 0)
		throw woma_exception("LoadHeightMapTerrain failed!", __FILE__, __FUNCTION__, __LINE__);

	//[2] Read in the file header.
	count = (UINT)fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
		throw woma_exception("LoadHeightMapTerrain failed!", __FILE__, __FUNCTION__, __LINE__);

	//[3] Check Signature:
	if (bitmapFileHeader.bfType != 0x4d42)	// "BM"
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	//[4] Read in the bitmap info header.
	count = (UINT)fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
		throw woma_exception("LoadHeightMapTerrain failed!", __FILE__, __FUNCTION__, __LINE__);

	//[5] 1, 4 and 8 not supported:
	if (bitmapInfoHeader.biBitCount < 24)
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	//Store the size of the terrain so we can use these values for building the vertex and index buffers as well as rendering the terrain.

	// Save the dimensions of the terrain.
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_terrainWidth * m_terrainHeight * bitmapInfoHeader.biBitCount / 8; //  x (24/8) = x 3

	// Allocate memory for the bitmap image data.
	bitmapImage = NEW unsigned char[imageSize];
	IF_NOT_THROW_EXCEPTION(bitmapImage);

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = (UINT)fread(bitmapImage, 1, imageSize, filePtr);
	if (count != (UINT)imageSize)
		throw woma_exception("LoadHeightMapTerrain failed!", __FILE__, __FUNCTION__, __LINE__);

	// Close the file.
	if (filePtr)
		error = fclose(filePtr);
	if (error != 0)
		throw woma_exception("LoadHeightMapTerrain failed!", __FILE__, __FUNCTION__, __LINE__);
#else
	ImageLoaderClass ImageLoader;

	const TCHAR* extension = _tcsrchr(filename.c_str(), '.');
	if (extension == NULL) return false;

	if (_tcsicmp(extension, TEXT(".jpg")) == 0 || _tcsicmp(extension, TEXT(".jpeg")) == 0)
	{
		bitmapImage = ImageLoader.loadJPEG((TCHAR*)filename.c_str(), &m_terrainWidth, &m_terrainHeight, &imageSize, LOAD_IMAGE_24bits);
	}
	else if (_tcsicmp(extension, TEXT(".png")) == 0)
	{
		bitmapImage = ImageLoader.loadPNG((TCHAR*)filename.c_str(), &m_terrainWidth, &m_terrainHeight, &imageSize, LOAD_IMAGE_24bits);
	}
	else if (_tcsicmp(extension, TEXT(".tga")) == 0)
	{
		bitmapImage = ImageLoader.loadTGA((TCHAR*)filename.c_str(), &m_terrainWidth, &m_terrainHeight, &imageSize, LOAD_IMAGE_24bits);
	}
	else if (_tcsicmp(extension, TEXT(".bmp")) == 0)
	{
		if (m_terrainType == TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_TransparentTexture_MINI_MAP)
			bitmapImage = ImageLoader.loadBMP((TCHAR*)filename.c_str(), &m_terrainWidth, &m_terrainHeight, &imageSize, LOAD_IMAGE_24bits, false);
		else
			bitmapImage = ImageLoader.loadBMP((TCHAR*)filename.c_str(), &m_terrainWidth, &m_terrainHeight, &imageSize, LOAD_IMAGE_24bits);
	}
	else if (_tcsicmp(extension, TEXT(".tif")) == 0)
	{
		bitmapImage = ImageLoader.loadTIF((TCHAR*)filename.c_str(), &m_terrainWidth, &m_terrainHeight, &imageSize, LOAD_IMAGE_24bits);
	}
	else if (_tcsicmp(extension, TEXT(".dds")) == 0)
	{
		bitmapImage = ImageLoader.loadDDS((TCHAR*)filename.c_str(), &m_terrainWidth, &m_terrainHeight, &imageSize, LOAD_IMAGE_24bits);
	}
	else {
		WomaFatalExceptionW(TEXT("IMAGE: Format not supported!"));
		return false;
	}

#endif

	ASSERT(m_terrainWidth == terrain_squares);
	ASSERT(m_terrainHeight == terrain_squares);

	// Now that the bitmap has been read in create the two dimensional height map array and read the buffer into it. 
	// Note that during the for loop I use the two loop variables (i and j) to be the X (width) and Z (depth) of the terrain. 
	// And then I use the bitmap value to be the Y (height) of the terrain. You will also see I increment the index into the bitmap (k) by three 
	// since we only need one of the color values (red, green, or blue) to be used as the grey scale value.

	// Convert 32bits into 8 bits:
	// Read the image data into the height map array.
	int k = 0; int x, y;
#define j y
#define i x
	const static float fixedBorderHeight = FIXEDBORDERHEIGHT;

#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
	if (m_terrainType == TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_TransparentTexture_MINI_MAP)
	{
		m_heightMap_21 = NEW HeightMapType_21[m_terrainWidth * m_terrainHeight];
		#define m_heightMap m_heightMap_21

		int index = 0;

		// Read the image data into the height map.
		for (j = 0; j < (int)m_terrainHeight; j++) {
			for (i = 0; i < (int)m_terrainWidth; i++) {

				float height = bitmapImage[k];
				index = (m_terrainWidth * j) + i;

				m_heightMap[index].x = (float)i + xPos;
				m_heightMap[index].y = (float)height;
				m_heightMap[index].z = (float)j + zPos;

				k += 3;
			}
		}
		for (j = 0; j < (int)m_terrainHeight; j++) {

			for (i = 0; i < 1; i++) {

				index = (m_terrainWidth * j) + i;
				m_heightMap[index].y = fixedBorderHeight;
			}
		}
		for (j = 0; j < (int)m_terrainHeight; j++) {

			for (i = m_terrainWidth - 1; i < (int)m_terrainWidth; i++) {

				index = (m_terrainWidth * j) + i;
				m_heightMap[index].y = fixedBorderHeight;
			}
		}
		for (j = 0; j < 1; j++) {

			for (i = 0; i < (int)m_terrainWidth; i++) {

				index = (m_terrainWidth * j) + i;
				m_heightMap[index].y = fixedBorderHeight;
			}
		}
		for (j = m_terrainHeight - 1; j < (int)m_terrainHeight; j++) {

			for (i = 0; i < (int)m_terrainWidth; i++) {

				index = (m_terrainWidth * j) + i;
				m_heightMap[index].y = fixedBorderHeight;
			}
		}
	}
	else
#endif
	{
		for (j = 0; j < (int)m_terrainHeight; j++) {

			for (i = 0; i < (int)m_terrainWidth; i++) {
				height[y][x] = (float)bitmapImage[k]; 

				k += 3;
			}
		}
		for (j = 0; j < (int)m_terrainHeight; j++) {

			for (i = 0; i < 1; i++) {
				height[(terrain_squares - 1) - y][x] = fixedBorderHeight;
			}
		}
		for (j = 0; j < (int)m_terrainHeight; j++) {

			for (i = m_terrainWidth - 1; i < (int)m_terrainWidth; i++) {
				height[(terrain_squares - 1) - y][x] = fixedBorderHeight;
			}
		}
		for (j = 0; j < 1; j++) {

			for (i = 0; i < (int)m_terrainWidth; i++) {
				height[(terrain_squares - 1) - y][x] = fixedBorderHeight;
			}
		}
		for (j = m_terrainHeight - 1; j < (int)m_terrainHeight; j++) {

			for (i = 0; i < (int)m_terrainWidth; i++) {
				height[(terrain_squares - 1) - y][x] = fixedBorderHeight;
			}
		}
	}

#undef j
#undef i
#undef m_heightMap

	// Now that we have stored the height map data for the terrain in our own array we can release the bitmap array.
	// Release the bitmap image data:
	SAFE_DELETE_ARRAY(bitmapImage); //delete [] bitmapImage; bitmapImage = 0;
	return true;
}

#if defined DX11 || defined DX9
#include "Dx11Class.h"
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 //Use: WIN10SDK
#include "Dx12Class.h"
#endif
#if (defined OPENGL3 || defined OPENGL40) 
#include "womadriverclass.h"	//woma
#include "GLmathClass.h"		//woma	
#include "GLopenGLclass.h"		//woma
#if defined WINDOWS_PLATFORM
#include "wGLopenGLclass.h"		// Windows
#endif
#endif

#include "DXmodelClass.h"

CTerrain::CTerrain(MY_TERRAIN_TYPE terrainType)
{
	loaded = false;
	m_terrainWidth = 0;
	m_terrainHeight = 0;
	m_terrainType = terrainType;
	CLASSLOADER(); //In static classes we can't call: CLASSLOADER();
}

#if D3D11_SPEC_DATE_YEAR == 2009 && DX_ENGINE_LEVEL >= 70
	#error "From this LEVEL 60 onwards, we need to run DX outside the original: C:\WoMAengine2014\Microsoft_DirectX_SDK_June_2010"
#endif

CTerrain::~CTerrain() { 
	//SAFE_DELETE(m_heightMap);
	CLASSDELETE(); //In static classes we can't call: CLASSDELETE();
} 

#if defined SCENE_GENERATEDUNDERWATER
void WorldUnderWaterMapGenerator(MY_TERRAIN_TYPE terrainType)
{
	CTerrain* terrain = NEW CTerrain(terrainType);

	// "Initial" Entry Point for: CHAP 14
	womalogauto(TEXT("Sizeof terrain: %d KBs\n"), (terrain_squares + 1)*(terrain_squares + 1) / KBs);
	womalogauto(TEXT("Sizeof terrain: %d MBs\n"), (terrain_squares + 1)*(terrain_squares + 1) / MBs);

#if !defined GOTO_BASICS
	double delta = 0;
	static float t = 0;
	INT64 currentTime, currentTime2;
	TimerClass m_Timer;
	m_Timer.Initialize();	// Initialize the timer object
	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime); // Measure the initial Time
#endif
	// ................................................................................................
	// 1024 -> 22 Secs.
	terrain->GenerateRandomHeightMapTerrain(12);	// Random Seed Value
	SAFE_DELETE(terrain);

	// ................................................................................................
#if !defined GOTO_BASICS
	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime2);// Measure current Time
	if (m_Timer.m_ticksPerUs > 0)
		delta = ((((float)currentTime2 - (float)currentTime) / m_Timer.m_ticksPerUs) / 1000.0f);

	TCHAR txt[MAX_STR_LEN];
	StringCchPrintf(txt, MAX_STR_LEN, TEXT("\nBenchmark to generate terrain: %f ms\n"), delta);
	womalogw(txt);
#endif
}
#endif

////////////////////////////////////////////////////////////////////////////////
int gp_wrap(int a)
{
	if (a<0) return (a + terrain_squares);
	if (a >= terrain_squares) return (a - terrain_squares);
	return a;
}


void CTerrain::DUMP_TEXT_Version(TCHAR* filename)
{
#if _DEBUG && false // Write ALL to a TEXT file Format, just to Debug it! 
	{
		OFSTREAM fileOut(filename);    //Open file for Write : cpp
		for (int i = 0; i<terrain_squares; i++)
		{
			for (int j = 0; j<terrain_squares; j++)
			{
				fileOut << height[i][j] << TEXT(" ");
			}
			fileOut << endl;
		}
		fileOut.close();
	}
#endif
}

void CTerrain::CalculateMaxMin()
{
	float maxY = -10000; //DEBUG ONLY!!!
	float minY = +10000; //DEBUG ONLY!!!
	for (int y = 0; y<terrain_squares; y++)
		for (int x = 0; x<terrain_squares; x++)
		{
			maxY = MAX(maxY, height[y][x]);
			minY = MIN(minY, height[y][x]);
		}

	womalogauto(TEXT("Terrain MAX Height: %f\n"), maxY);	// 23
	womalogauto(TEXT("Terrain MIN Height: %f\n"), minY);	//-29
}

#if defined SCENE_GENERATEDUNDERWATER
//The CopyVertexArray function is a new function that allows the QuadTreeClass to copy the vertex array from inside the TerrainClass into a vertex list for itself.
void CTerrain::CopyVertexArray(UINT id, ModelTextureVertexType* vertexList)
{
	if (id == 0) memcpy(vertexList, (ModelTextureVertexType*)&modelVertexVector0[0], sizeof(vertex0) * terrain_squares * terrain_squares);
	if (id == 1) memcpy(vertexList, (ModelTextureVertexType*)&modelVertexVector1[0], sizeof(vertex1) * terrain_squares * terrain_squares);
	if (id == 2) memcpy(vertexList, (ModelTextureVertexType*)&modelVertexVector2[0], sizeof(vertex2) * terrain_squares * terrain_squares);
}

int CTerrain::GetVertexCount()
{
	return terrain_squares*terrain_squares;
}
#endif

#if DX_ENGINE_LEVEL != 62 && defined SCENE_GENERATEDUNDERWATER
// backterrain[(terrain_squares + 1)] [(terrain_squares + 1)]
// ----------------------------------------------------------------------------
void CTerrain::GenerateRandomHeightMapTerrain(UINT randValue, bool Move_down_edges) // OLD: void CTerrain::CreateTerrain(UINT randValue)
// ----------------------------------------------------------------------------
{
	int i, j, k;
	float* backterrain;
	WOMA::vec3 vec1, vec2, vec3;
	int currentstep = terrain_squares;
	float mv, rm;
	float offset = 0, yscale = 0, maxheight = 0, minheight = 0;

	womalogauto(TEXT("Terrain Size: %d x %d\n"), (terrain_squares + 1), (terrain_squares + 1));	// 23

	// [backterrain]
	const UINT floatSize = sizeof(float); //4
	const UINT TerrainSize = (terrain_squares + 1)*(terrain_squares + 1)*floatSize;
	backterrain = (float*) MALLOC(TerrainSize);
	for (size_t i = 0; i < (terrain_squares + 1)*(terrain_squares + 1); i++)
		backterrain[i] = 0;

	rm = terrain_fractalinitialvalue;
	backterrain[0] = 0;
	backterrain[0 + terrain_squares*terrain_squares] = 0;
	backterrain[terrain_squares] = 0;
	backterrain[terrain_squares + terrain_squares*terrain_squares] = 0;
	currentstep = terrain_squares;

	// Generating fractal terrain using "square-diamond" method:
	while (currentstep>1)
	{
		// Square step:
		womalog("Square Step...\n");
		i = 0;
		j = 0;

		while (i<terrain_squares)
		{
			j = 0;
			while (j<terrain_squares)
			{
				UINT index = i + terrain_squares*j;
				mv = backterrain[index];
				index = (i + currentstep) + terrain_squares*j;
				mv += backterrain[index];
				index = (i + currentstep) + terrain_squares*(j + currentstep);
				mv += backterrain[index];
				index = i + terrain_squares*(j + currentstep);
				mv += backterrain[index];
				mv /= 4.0;
				index = i + currentstep / 2 + terrain_squares*(j + currentstep / 2);
				backterrain[index] = (float)(mv + rm*((rand() % 1000) / 1000.0f - 0.5f)); // Force to have most, below water
				j += currentstep;
			}
			i += currentstep;
		}

		// Diamond step:
		UINT index;
		i = 0;
		j = 0;
		womalog("Diamond Step...\n");
		while (i<terrain_squares)
		{
			j = 0;

			while (j<terrain_squares)
			{
				mv = 0;
				mv = backterrain[i + terrain_squares*j];
				mv += backterrain[(i + currentstep) + terrain_squares*j];
				mv += backterrain[(i + currentstep / 2) + terrain_squares*(j + currentstep / 2)];
				mv += backterrain[i + currentstep / 2 + terrain_squares*gp_wrap(j - currentstep / 2)];
				mv /= 4;
				index = i + currentstep / 2 + terrain_squares*j;
				backterrain[index] = (float)(mv + rm*((rand() % 1000) / 1000.0f - 0.5f));

				mv = 0;
				mv = backterrain[i + terrain_squares*j];
				mv += backterrain[i + terrain_squares*(j + currentstep)];
				mv += backterrain[(i + currentstep / 2) + terrain_squares*(j + currentstep / 2)];
				mv += backterrain[gp_wrap(i - currentstep / 2) + terrain_squares*(j + currentstep / 2)];
				mv /= 4;
				index = i + terrain_squares*(j + currentstep / 2);
				backterrain[index] = (float)(mv + rm*((rand() % 1000) / 1000.0f - 0.5f));

				mv = 0;
				mv = backterrain[i + currentstep + terrain_squares*j];
				mv += backterrain[i + currentstep + terrain_squares*(j + currentstep)];
				mv += backterrain[(i + currentstep / 2) + terrain_squares*(j + currentstep / 2)];
				mv += backterrain[gp_wrap(i + currentstep / 2 + currentstep) + terrain_squares*(j + currentstep / 2)];
				mv /= 4;
				index = i + currentstep + terrain_squares*(j + currentstep / 2);
				backterrain[index] = (float)(mv + rm*((rand() % 1000) / 1000.0f - 0.5f));

				mv = 0;
				mv = backterrain[i + currentstep + terrain_squares*(j + currentstep)];
				mv += backterrain[i + terrain_squares*(j + currentstep)];
				mv += backterrain[(i + currentstep / 2) + terrain_squares*(j + currentstep / 2)];
				mv += backterrain[i + currentstep / 2 + terrain_squares*gp_wrap(j + currentstep / 2 + currentstep)];
				mv /= 4;
				index = i + currentstep / 2 + terrain_squares*(j + currentstep);
				backterrain[index] = (float)(mv + rm*((rand() % 1000) / 1000.0f - 0.5f));
				j += currentstep;
			}
			i += currentstep;
		}
		//changing current step;
		currentstep /= 2;
		rm *= terrain_fractalfactor;
	}


	// ----------------------------------------------------------------------------
	// Scaling to minheight..maxheight range:
	// ----------------------------------------------------------------------------

	// backterrain[(terrain_squares + 1)] [(terrain_squares + 1)] ------> height[terrain_squares] [terrain_squares]
	for (i = 0; i<terrain_squares; i++)
		for (j = 0; j<terrain_squares; j++)
			height[i][j] = backterrain[i + (terrain_squares+1)*j];


	womalog("Scaling to minheight...\n");
	maxheight = height[0][0];
	minheight = height[0][0];
	for (i = 0; i<terrain_squares; i++)
		for (j = 0; j<terrain_squares; j++)
		{
			if (height[i][j]>maxheight) maxheight = height[i][j];
			if (height[i][j]<minheight) minheight = height[i][j];
		}

	offset = minheight - terrain_minheight;
	if (maxheight - minheight)
		yscale = (terrain_maxheight - terrain_minheight) / (maxheight - minheight);
	else
		yscale = 1;

	for (i = 0; i<terrain_squares; i++)
		for (j = 0; j<terrain_squares; j++)
		{
			height[i][j] -= minheight;
			height[i][j] *= yscale;
			height[i][j] += terrain_minheight;
		}

	if (Move_down_edges)
	{
		// Moving down edges of heightmap:
		womalog("Moving down edges...\n");
		for (i = 0; i<terrain_squares; i++)
			for (j = 0; j<terrain_squares; j++)
			{
				mv = (float)((i - terrain_squares / 2.0f)*(i - terrain_squares / 2.0f) + (j - terrain_squares / 2.0f)*(j - terrain_squares / 2.0f));
				rm = (float)((terrain_squares*0.8f)*(terrain_squares*0.8f) / 4.0f);
				if (mv>rm)
				{
					height[i][j] -= ((mv - rm) / 1000.0f)*terrain_geometry_scale;
				}
				if (height[i][j]<terrain_minheight)
				{
					height[i][j] = terrain_minheight;
				}
			}
	}

	// Terrain banks:
	womalog("Terrain Banks...\n");
	for (k = 0; k<10; k++)
	{
		for (i = 0; i<terrain_squares; i++)
			for (j = 0; j<terrain_squares; j++)
			{
				mv = height[i][j];
				if ((mv)>0.02f)
					mv -= 0.02f;

				if (mv<-0.02f)
					mv += 0.02f;

				height[i][j] = mv;
			}
	}

	// Smoothing:
	womalog("Smoothing...\n");
	for (k = 0; k<terrain_smoothsteps; k++)
	{
		womalogauto(TEXT("Terrain Smoothstep: %d/%d\n"), k, terrain_smoothsteps);
		for (i = 0; i<terrain_squares; i++)
			for (j = 0; j<terrain_squares; j++)
			{

				vec1.x = 2 * terrain_geometry_scale;
				vec1.y = terrain_geometry_scale*(height[gp_wrap(i + 1)][j] - height[gp_wrap(i - 1)][j]);
				vec1.z = 0;
				vec2.x = 0;
				vec2.y = -terrain_geometry_scale*(height[i][gp_wrap(j + 1)] - height[i][gp_wrap(j - 1)]);
				vec2.z = -2 * terrain_geometry_scale;

				vec3 = vector3dCrossProduct(vec1, vec2);	//D3DXVec3Cross(&vec3,&vec1,&vec2);
				vec3 = vector3dNormalize(vec3);				//D3DXVec3Normalize(&vec3,&vec3);

				if (((vec3.y>terrain_rockfactor) || (height[i][j]<1.2f)))
				{
					rm = terrain_smoothfactor1;
					mv = height[i][j] * (1.0f - rm) + rm*0.25f*(height[gp_wrap(i - 1)][j] + height[i][gp_wrap(j - 1)] + height[gp_wrap(i + 1)][j] + height[i][gp_wrap(j + 1)]);
					backterrain[i + terrain_squares*j] = mv;
				}
				else
				{
					rm = terrain_smoothfactor2;
					mv = height[i][j] * (1.0f - rm) + rm*0.25f*(height[gp_wrap(i - 1)][j] + height[i][gp_wrap(j - 1)] + height[gp_wrap(i + 1)][j] + height[i][gp_wrap(j + 1)]);
					backterrain[i + terrain_squares*j] = mv;
				}

			}
		for (i = 0; i<terrain_squares; i++)
			for (j = 0; j<terrain_squares; j++)
			{
				height[i][j] = (backterrain[i + terrain_squares*j]);
			}
	}
	for (i = 0; i<terrain_squares; i++)
		for (j = 0; j<terrain_squares; j++)
		{
			rm = 0.5f;
			mv = height[i][j] * (1.0f - rm) + rm*0.25f*(height[gp_wrap(i - 1)][j] + height[i][gp_wrap(j - 1)] + height[gp_wrap(i + 1)][j] + height[i][gp_wrap(j + 1)]);
			backterrain[i + terrain_squares*j] = mv;
		}
	for (i = 0; i<terrain_squares; i++)
		for (j = 0; j<terrain_squares; j++)
		{
			height[i][j] = (backterrain[i + terrain_squares*j]);
		}

	free(backterrain);

	//DUMP here
#ifdef _DEBUG
	DUMP_TEXT_Version(TEXT("MAPFILE_Step1"));
#endif
	//					MAX			MIN
	CalculateMaxMin();	// Before Values:	22.768532, -29.148338, 
						// Current Values:	11.441930, -29.799995

						// Correct Margins and convert color from: -25 to 25 --> -1 to -30 (Once this is UnderWater)
	for (i = 0; i<terrain_squares; i++)
		for (j = 0; j<terrain_squares; j++)
		{
			height[i][j] = MIN(-1, height[i][j] - 10);	//At max: -1
			height[i][j] = MAX(-20, height[i][j]);		//At MIN: -20
		}

	// Terrain MAX Height : -13.558070
	// Terrain MIN Height : -30.000000
	CalculateMaxMin();

	// Simulationg a globe MAP:
	// 0..-8k, 0..8k MAP (mirror)	[0..8k, 0..8k MAP] (Generated)
	// 0..-8k, 0..-8k MAP (mirror)	0..8k, 0..-8k MAP (mirror)

	// DUMP here TEXT VERSION FILE
#ifdef _DEBUG
	DUMP_TEXT_Version(TEXT("MAPFILE_Step2"));
#endif

#if defined WRITE_BMP // DEBUG: Write MAP to Disk to Debug it!
	SaveBMPHeightMapTerrain("maps");
#endif
}
#endif

#if true //DX_ENGINE_LEVEL <56 62								 
void CTerrain::SaveBMPHeightMapTerrain(CHAR* maps, UINT bmp_type) // NEED TO BE: "char"!
{
	//0...128+1 128..256+1
	for (UINT tY = 0; tY<terrain_squares; tY++)
	{
		for (UINT tX = 0; tX<terrain_squares; tX++)
		{
			// Write MAP (0,0) == 0...128+1 (129x129 Floats)
			//----------------------------------------------------------------------------
			CHAR txt[MAX_STR_LEN];

			if (bmp_type == SAVE_BMP_HM)
			{
				StringCchPrintfA(txt, MAX_STR_LEN, "engine/%s/MAPFILE_%d_%d.bmp", maps, 15 - tY, tX);
			}
			else
			{
				StringCchPrintfA(txt, MAX_STR_LEN, "engine/%s/MAPFILE_%d_%d.bmp", maps, tY, tX);		// TO INVERT IMAGE
			}

			CHAR dir_txt[MAX_STR_LEN];
			StringCchPrintfA(dir_txt, MAX_STR_LEN, "engine");				// Create Directory: [C:\WoMAengine2014\woma_developer\SAMPLES\x64\lvl014]\engine
			CreateDirectoryA(dir_txt, NULL);
			StringCchPrintfA(dir_txt, MAX_STR_LEN, "engine/%s", maps);		// Create Directory: [C:\WoMAengine2014\woma_developer\SAMPLES\x64\lvl014]\engine\maps
			CreateDirectoryA(dir_txt, NULL);

			womalog("Writing MAP to Disk: %s\n", txt);

			// WRITE - Binary (float) version:
			std::ofstream fileBMP(txt, std::ios::out | std::ios::binary);	//OFSTREAM fileBMP( txt );    //Open file for Write : cpp

			// WRITE - Image (BMP) version: BMP 24-bit color
			// ----------------------------------------------------------------------------
			// ORIGINAL SOURCE: irrlicht 1-8
			//

			// PART 1: HEADER
			// ----------------------------------------------------------------------------
			// SOURCE INFO: http://en.wikipedia.org/wiki/BMP_file_format

			BMPHeader_id		imageHeader_id = { 0 };
			BMPHeader			imageHeader = { 0 };

			DIBHeader_size		imageHeader_size = { 0 };
			DIBHeader_planesBPP	imageHeader_planesBPP = { 0 };
			DIBHeader_body		imageHeader_body = { 0 };

#if defined _DEBUG 
			int debug_;
			debug_ = sizeof(imageHeader_id);		//2
			debug_ = sizeof(imageHeader);			//12

			debug_ = sizeof(imageHeader_size);		//12
			debug_ = sizeof(imageHeader_planesBPP);	//4
			debug_ = sizeof(imageHeader_body);		//24
#endif

			// BMP Header:
			/* 0h*/		imageHeader_id.Id = 0x4d42;						// BM � Windows

			/* 2h*/		imageHeader.FileSize = 0;
			/* 6h*/		imageHeader.Reserved = 0;
			/* Ah*/		imageHeader.BitmapDataOffset = 54;				// 14 + 40 sizeof(imageHeader); | 56 = sizeof(imageHeader);

																		// DIB Header
			/* Eh*/		imageHeader_size.BitmapHeaderSize = 0x28;		// 40 = 28h for windows bitmaps / 0Ch for OS/2 1.x or F0h for OS/2 2.x
			/*12h*/		imageHeader_size.Width = MAP_CHUNK_SIZE/*+1*/;	// 128+1
			/*16h*/		imageHeader_size.Height = MAP_CHUNK_SIZE/*+1*/;	// 128+1

			/*1Ah*/		imageHeader_planesBPP.Planes = 1;
			/*1Ch*/		imageHeader_planesBPP.BPP = 24;					// 4/8/16/24/32 -bit color

			/*1Eh*/		imageHeader_body.Compression = 0;				// 0: none (Also identified by BI_RGB)

			// data size is rounded up to next larger 4 bytes boundary:
			/*22h*/		imageHeader_body.BitmapDataSize = imageHeader_size.Width * imageHeader_planesBPP.BPP / 8;
			imageHeader_body.BitmapDataSize = (imageHeader_body.BitmapDataSize + 3) & ~3;
			imageHeader_body.BitmapDataSize *= imageHeader_size.Height;

			/*26h*/		imageHeader_body.PixelPerMeterX = 2835;			// 72 DPI � 39.3701 = 2834.6472
			/*2Ah*/		imageHeader_body.PixelPerMeterY = 2835;			// 72 DPI � 39.3701 = 2834.6472
			/*2Eh*/		imageHeader_body.Colors = 0;					// Number of colors in the palette
			/*32h*/		imageHeader_body.ImportantColors = 0;			// 0 means all colors are important

			// file size is data size plus offset to data:
			imageHeader.FileSize = imageHeader.BitmapDataOffset + imageHeader_body.BitmapDataSize;

			// Write all bitmap header part(s):
			// ----------------------------------------------------------------------------
			fileBMP.write((CHAR*)&imageHeader_id, sizeof(imageHeader_id));
			fileBMP.write((CHAR*)&imageHeader, sizeof(imageHeader));
			fileBMP.write((CHAR*)&imageHeader_size, sizeof(imageHeader_size));
			fileBMP.write((CHAR*)&imageHeader_planesBPP, sizeof(imageHeader_planesBPP));
			fileBMP.write((CHAR*)&imageHeader_body, sizeof(imageHeader_body));

			// PART 2: BODY IMAGE
			// ----------------------------------------------------------------------------
			u32 pixel_size = imageHeader_planesBPP.BPP / 8;					// size of one pixel in bytes
			u32 row_stride = (pixel_size * imageHeader_size.Width);			// length of one row of the source image in bytes
			s32 row_size = ((pixel_size * imageHeader_size.Width) + 3) & ~3;// length of one row in bytes, rounded up to nearest 4-byte boundary

			// Convert the image to 24-bit BGR and flip it over
			// ----------------------------------------------------------------------------
			// TO INVERT BMP
			if (bmp_type == SAVE_BMP_HM)
			{
				for (UINT y = 0; y < imageHeader_size.Height; ++y)		//DONT TOUCH: this line!
				{
					// allocate memory for our scan line:
					unsigned char* row_pointer = NEW unsigned char[row_size];
					IF_NOT_THROW_EXCEPTION(row_pointer);

					// Copy all line of Image:
					for (UINT x = 0; x < imageHeader_size.Width; x++)		// NOTE: have to be int!
					{
						unsigned char color = (unsigned char)height[y + tY*MAP_CHUNK_SIZE][x + tX*MAP_CHUNK_SIZE];
						row_pointer[x * 3] = row_pointer[x * 3 + 1] = row_pointer[x * 3 + 2] = color;	// row_stride (real size)
					}

					// Dump to file this line:
					fileBMP.write((CHAR*)row_pointer, row_size);

					// clean up our scratch area
					SAFE_DELETE_ARRAY(row_pointer);
				}
			}

			// Flush all data to file:
			fileBMP.close();
		}
	}

}
#endif

bool CTerrain::InBounds(int i, int j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < (int)m_terrainHeight &&
		j >= 0 && j < (int)m_terrainWidth;
}

float CTerrain::Average(int i, int j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
	// and no iterations of the outer for loop occur.
	for (int m = i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				avg += height[m][n];	//avg += mHeightmap[m * m_terrainWidth + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void CTerrain::Terrain_Smooth()
{
	float dest[terrain_squares][terrain_squares] = { 0 };	//std::vector<float> dest(mHeightmap.size());

	for (UINT i = 0; i < m_terrainHeight; ++i)
	{
		for (UINT j = 0; j < m_terrainWidth; ++j)
		{
			dest[i][j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one. //mHeightmap = dest;
	for (UINT i = 0; i < m_terrainHeight; ++i)
	{
		for (UINT j = 0; j < m_terrainWidth; ++j)
		{
			height[i][j] = dest[i][j];
		}
	}

}

//0 UNDERWATER
#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN
// ----------------------------------------------------------------------------
void CTerrain::initUnderWaterDemo(UINT terrainId)
// ----------------------------------------------------------------------------
{
	//UINT id = 0; // Fix number for center terrain

	// Generate: Random Seed Value: 12 

#if !defined SCENE_UNDERWATER_BATH_TERRAIN
	GenerateRandomHeightMapTerrain(127);			// Will generate a mesh 512x512 with heightMap
#else
	//PORTUGAL NOR: B1 - 8_14
	//PORTUGAL SUL: B1 - 9_14
	autoGenUnderWaterTerrain->LoadHeightMapTerrain(TEXT("\\WoMAengine2014\\engineWorldData\\bathB1\\MAPFILE_8_14.bmp"), 0, 0);

	// Convert All HM reading for under water 0 to 255 -> -255 to 0
	for (UINT z = 0; z < autoGenUnderWaterTerrain->m_terrainHeight; z++)
		for (UINT x = 0; x < autoGenUnderWaterTerrain->m_terrainWidth; x++)
			autoGenUnderWaterTerrain->height[z][x] -= 255;
#endif

	// [BuildTerrainModel]: QUAD SIZE: Step 1: Setup all vertices positions: X, Y, Z:	// ADD Vertex: modelVertexVector.push_back(vertex);
	PopulateTerrainModelVertexVector(terrainId, 1); //1=terrain_squareSize

	// Add TEXTURE MAP: to all vertices
	UINT modelVertexVectorSize = (UINT)modelVertexVector0.size();
	for (UINT i = 0; i < modelVertexVectorSize; i++)			// Num Vertices: 6x256x256 = 393216
	{
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
			float aux = modelVertexVector0[i].tu;
			modelVertexVector0[i].tu = modelVertexVector0[i].tv;
			modelVertexVector0[i].tv = aux;
		}
		modelVertexVector0[i].y = modelVertexVector0[i].y / 3;

#if defined DEBUG_TERRAIN_VERTICES
		womalog("%d = (%d %d %d)\n", i, (int)modelVertexVector[i].x, (int)modelVertexVector[i].y, (int)modelVertexVector[i].z);
#endif
	}

}
#endif

//1 WATER
#if defined SCENE_WATER_TERRAIN
// ----------------------------------------------------------------------------------
bool CTerrain::initTerrainWaterMeshDemo(UINT terrainId) // Used to load WATER
// ----------------------------------------------------------------------------------
{
	SystemHandle->m_Application->loadedTerrain[terrainId]->LoadHeightMapTerrain(TEXT("engine/data/Terrain/heightmap512x512.bmp"), 0, 0);	// FLAT WATER!

		PopulateTerrainModelVertexVector(terrainId, 1); //1=terrain_squareSize

		TCHAR waterTexture[MAX_STR_LEN] = { 0 };	atow(waterTexture, SystemHandle->world.waterTexture.c_str(), MAX_STR_LEN);
		std::vector<STRING> Textures; Textures.push_back(waterTexture);		// WATER: Shader:TEXTURE
		CreateTerrainModel(terrainId,  Textures, SHADER_TEXTURE_WATER);		// m_TerrainModel[id]->Load

		// FORCE FOR NOW TRANSPARENT:
#if defined DX11 || defined DX9
		if (((DirectX::DXmodelClass*)SystemHandle->m_Application->m_TerrainModel[terrainId])->m_Shader11) {
			((DirectX::DXmodelClass*)SystemHandle->m_Application->m_TerrainModel[terrainId])->m_Shader11->hasAlfaColor = true;
			((DirectX::DXmodelClass*)SystemHandle->m_Application->m_TerrainModel[terrainId])->m_Shader11->alfaColor = 0.75f;
		}
#endif
#if defined DX12
		if (((DirectX::DXmodelClass*)SystemHandle->m_Application->m_TerrainModel[terrainId])->m_Shader)
		{
			((DirectX::DXmodelClass*)SystemHandle->m_Application->m_TerrainModel[terrainId])->m_Shader->hasAlfaColor = true;
			((DirectX::DXmodelClass*)SystemHandle->m_Application->m_TerrainModel[terrainId])->m_Shader->alfaColor = 0.75f;
		}
#endif

	loaded = true; // Ready!

	return true;
}
#endif

#define M_heightMap(x,y) height[(terrain_squares-1) -y][x]

#define MakeSoftBorders(m_heightMap){\
	int i, j;\
	/* Make a soft borders: Y0*/\
	UINT border = 12;\
	for(j=1; j<border; j++)\
		for(i=j; i<m_terrainWidth-j; i++)\
			m_heightMap[(m_terrainWidth * j) + i].y = m_heightMap[(m_terrainWidth * (border-j)) + i].y;\
	\
	border = 7;\
	/* Make a soft borders: Ymax*/\
	for(UINT k=0,j=m_terrainHeight-1-1; j>(m_terrainHeight-border); j--,k++)\
		for(i=1+k; i<m_terrainWidth-k; i++)\
			m_heightMap[(m_terrainWidth * j) + i].y = m_heightMap[(m_terrainWidth * ((m_terrainHeight-border)+k)) + i].y;\
	\
	/* Make a soft borders: X0*/\
	for(i=1; i<border; i++)\
		for(j=i; j<m_terrainHeight-i; j++)\
			m_heightMap[(m_terrainWidth * j) + i].y = m_heightMap[(m_terrainWidth * j) + (border-i)].y;\
	\
	/* Make a soft borders: Xmax*/\
	border = 10;\
	for(UINT k=0,i=m_terrainWidth-1-1; i>(m_terrainWidth-border); i--,k++)\
		for(j=1; j<m_terrainHeight-1; j++)\
			m_heightMap[(m_terrainWidth * j) + i].y = m_heightMap[(m_terrainWidth * j) + (m_terrainWidth-border)+k].y;\
}

#pragma warning( disable : 4018 ) //4018: '>': signed/unsigned mismatch
//The next new function is NormalizeHeightMap. All it does is it goes through the terrain and divides each height value by 15 so that the terrain doesn't look too spikey. 
//Generally its better just to do this work on the height map before loading it in.
void CTerrain::NormalizeHeightMap(float scale, float moveY)
{
	int x, y;

	#define j y
	#define i x
#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
	if (m_terrainType == TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_TransparentTexture_MINI_MAP)
	{
		int i, j;
		// Make a soft borders: Y0
		int border = 12;
		for(j=1; j<border; j++)
			for(i=j; i < (int)m_terrainWidth-j; i++)
				m_heightMap_21[(m_terrainWidth * j) + i].y = m_heightMap_21[(m_terrainWidth * (border-j)) + i].y;
		
		border = 7;
		// Make a soft borders: Ymax
		for(int k=0,j=m_terrainHeight-1-1; j>(m_terrainHeight-border); j--,k++)
			for(i=1+k; i < (int)(m_terrainWidth-k); i++)
				m_heightMap_21[(m_terrainWidth * j) + i].y = m_heightMap_21[(m_terrainWidth * ((m_terrainHeight-border)+k)) + i].y;
		
		// Make a soft borders: X0
		for(i=1; i<border; i++)
			for(j=i; j < (int)m_terrainHeight-i; j++)
				m_heightMap_21[(m_terrainWidth * j) + i].y = m_heightMap_21[(m_terrainWidth * j) + (border-i)].y;
		
		// Make a soft borders: Xmax
		border = 10;
		for(int k=0,i=m_terrainWidth-1-1; i>(m_terrainWidth-border); i--,k++)
			for(j=1; j < (int)m_terrainHeight-1; j++)
				m_heightMap_21[(m_terrainWidth * j) + i].y = m_heightMap_21[(m_terrainWidth * j) + (m_terrainWidth-border)+k].y;
	} else 
#endif
	{
	}

	for (j = 1; j < (int)m_terrainHeight - 1; j++)
	{
		for (i = 1; i < (int)m_terrainWidth - 1; i++)
		{
				UINT index = (m_terrainWidth * j) + i;

    #if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
				if (m_terrainType == TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_TransparentTexture_MINI_MAP)
				{
					//LVL:60
					m_heightMap_21[(m_terrainWidth * j) + i].y /= scale; //15.0f;
					if ((i >= 0 && i < (int)m_terrainWidth) || (j >= 0 && j < (int)m_terrainHeight))
						m_heightMap_21[(m_terrainWidth * j) + i].y += moveY;
				} else 
	#endif
				{
					//LVL:55
					height[(terrain_squares-1)  - y][x] /= scale; //15.0f;
					if ((i >= 0 && i < (int)m_terrainWidth) || (j >= 0 && j < (int)m_terrainHeight))
						height[(terrain_squares-1)  - y][x] += moveY;
				}
		}
	}

	#undef j
	#undef i
}



//2 MAIN-TERRAIN
#if defined SCENE_MAIN_TOPO_TERRAIN
// ----------------------------------------------------------------------------
bool CTerrain::initMainTopoTerrainDemo(UINT terrainId)
// ----------------------------------------------------------------------------
{
	float xpos = 0, zpos = 0;
	//[LoadHeightMapTerrain()] Load and Invert Image(to make it rigth) Populate: 
	//------------------------------------------------------------------------------------------
	// m_heightMap[index]
	// height[y][x]
	//[1]RASTER LoadHeightMap()
	TCHAR HeightMapMainTexture[MAX_STR_LEN] = { 0 }; atow(HeightMapMainTexture, SystemHandle->world.mainTexture.c_str(), MAX_STR_LEN);

	//Populate m_heightMap_21[index].x...y...z
	IF_NOT_RETURN_FALSE(LoadHeightMapTerrain(HeightMapMainTexture, xpos, zpos))

		// IMG new format equivalente: height[(terrain_squares-1)  - y][x] == m_heightMap[(m_terrainWidth * y) + x]
		//[2]RASTER NormalizeHeightMap()
		if (terrainId == 2 || terrainId == 3) 
		{
			//Populate m_heightMap[(m_terrainWidth * j) + i].y
			
			NormalizeHeightMap(5, -1.4f); //(float scale, float moveY) //equal to ReduceHeightMap();
			if (m_terrainType == TERRAIN_COLOR_QUAD_FOG_SLOP_TEXTURE_Detail_Mapping_TextureMapping_AlphaMapping_BumpMapping_LighMapping_TransparentTexture_MINI_MAP)
				return CreateTerrain60(xpos, zpos);
		}

	// RASTER: CalculateTextureCoordinates() |tu tv|
		//------------------------------------------------------------------------------------------
		// Step 2: QUAD SIZE: [PopulateTerrainModelVertexVector()] 
		//  Populate:	modelVertexVector2[i] x, y, z
		//				modelVertexVector2[i].tu
		//				modelVertexVector2[i].tv
		PopulateTerrainModelVertexVector(terrainId, 1);	//1=terrain_squareSize // 512x125	to 0 2048,2048

	//------------------------------------------------------------------------------------------
	// Step 3: ID2 SCALE: modelVertexVector2[i].y |tu tv OPEN GL|

	#if defined SCENE_TERRAIN_COLLISION
	//------------------------------------------------------------------------------------------
	// Step 3: ID3 Reduce terrain scale/height:	|tu tv OPEN GL|
		#if DX_ENGINE_LEVEL >= 52 && defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
		if (terrainId == 3) {
			// Add TEXTURE MAP: to all vertices
			for (UINT i = 0; i < modelVertexVector3.size(); i++)			// Num Vertices: 6x256x256
			{
				if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
				{
					float aux = modelVertexVector3[i].tu;
					modelVertexVector3[i].tu = modelVertexVector3[i].tv;
					modelVertexVector3[i].tv = aux;
				}
			}
		}
		#endif
	#endif

	// Step 5: Populate: VirtualModelClass* SystemHandle->m_Application->m_TerrainModel[id]
	//Populate: indices.push_back
	if (terrainId == 2 || terrainId == 3 || terrainId == 4)
	{
		std::vector<STRING> Textures;
		Textures.push_back(TERRAIN_LEVEL50_TEXTURE);
		CreateTerrainModel(terrainId,  Textures, SHADER_AUTO);
	}
	return true;
}
#endif


// ----------------------------------------------------------------------------
void CTerrain::PopulateTerrainModelVertexVector(UINT id, float unit)
// ----------------------------------------------------------------------------
{
	static float x0 = 0;// -terrain_squares / 2;
	static float y0 = 0;// -terrain_squares / 2;

	static float scaleFactor = 1;

	// Save: CTerrain: Terrain Size
	m_terrainWidth = (UINT)((UINT)terrain_squares * (UINT)unit);
	m_terrainHeight = (UINT)((UINT)terrain_squares * (UINT)unit);

	int k = 0;
	for (UINT y = 0; y < terrain_squares ; y++) {

		for (UINT x = 0; x < terrain_squares; x++)
		{
			// TRANSFOR: 1 QUAD (4 Vertices) ----> 6 Vertices
			// V1: Num Vertices: 6x256x256 || Note: Using 6 Vertex(s) is a Basic and Deprecated way!
			#if defined SCENE_GENERATEDUNDERWATER
			if (id == 0) {
				ModelVertexs0(vertex0, modelVertexVector0,
					x0 + x * unit,			y0 + y * unit,			scaleFactor * height[y]		[x],		// Upper left
					x0 + x * unit + unit,	y0 + y * unit,			scaleFactor * height[y]		[x + 1],	// Upper right
					x0 + x * unit,			y0 + (y + 1) * unit,	scaleFactor * height[y + 1]	[x],		// Bottom left
					x0 + x * unit + unit,	y0 + (y + 1) * unit,	scaleFactor * height[y + 1]	[x + 1]);	// Bottom right	
			}
			#endif
			#if defined SCENE_WATER_TERRAIN
			if (id == 1) {
				ModelVertexs(vertex1, modelVertexVector1,
					x0 + x * unit, y0 + (y + 1) * unit, scaleFactor * height[MIN(terrain_squares - 1, y + 1)][x],										// Upper left
					x0 + x * unit + unit, y0 + (y + 1) * unit, scaleFactor * height[MIN(terrain_squares - 1, y + 1)][MIN(terrain_squares - 1, x + 1)],	// Upper right	
					x0 + x * unit, y0 + y * unit, scaleFactor * height[y][x],																			// Bottom left
					x0 + x * unit + unit, y0 + y * unit, scaleFactor * height[y][MIN(terrain_squares - 1, x + 1)]);										// Bottom right
			}
			#endif
			#if !defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
			if (id == 2) {
				ModelVertexs(vertex2, modelVertexVector2,
					x0 + x * unit, y0 + (y + 1) * unit, scaleFactor * height[MIN(terrain_squares - 1, y + 1)][x],										// Upper left
					x0 + x * unit + unit, y0 + (y + 1) * unit, scaleFactor * height[MIN(terrain_squares - 1, y + 1)][MIN(terrain_squares - 1, x + 1)],	// Upper right	
					x0 + x * unit, y0 + y * unit, scaleFactor * height[y][x],																			// Bottom left
					x0 + x * unit + unit, y0 + y * unit, scaleFactor * height[y][MIN(terrain_squares - 1, x + 1)]);										// Bottom right
			}
			#endif
			#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX && defined SCENE_TERRAIN_COLLISION//DX_ENGINE_LEVEL >= 52
			if (id == 3) {
				//(vertex, modelVertexVector,	Ul_x, Ul_y, Ul_z, 
				//								Ur_x, Ur_y, Ur_z, 
				//								Bl_x, Bl_y, Bl_z, 
				//								Br_x, Br_y, Br_z)

				ModelVertexs(vertex3, modelVertexVector3,
					x0 + x * unit, y0 + (y + 1) * unit, scaleFactor * height[MIN(terrain_squares - 1, y + 1)][x],										// Upper left
					x0 + x * unit + unit, y0 + (y + 1) * unit, scaleFactor * height[MIN(terrain_squares - 1, y + 1)][MIN(terrain_squares - 1, x + 1)],	// Upper right	
					x0 + x * unit, y0 + y * unit, scaleFactor * height[y][x],																			// Bottom left
					x0 + x * unit + unit, y0 + y * unit, scaleFactor * height[y][MIN(terrain_squares - 1, x + 1)]);										// Bottom right
			}
			#endif

		#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX //DX_ENGINE_LEVEL >= 53 //index
			// V2: Num Vertices (ID = 2)
			if (id == 2) {
				vertex2.x = x0 + x * unit;
				vertex2.z = y0 + y * unit;

					vertex2.y = (scaleFactor * height[y][x]);

				if (y % 2 == 0) {

					if (x % 2 == 0) {
						vertex2.tu = 0;
						vertex2.tv = 0;
					}
					else {
						vertex2.tu = 1;
						vertex2.tv = 0;
					}
				}
				else {
					if (x % 2 == 0) {
						vertex2.tu = 0;
						vertex2.tv = 1;
					}
					else {
						vertex2.tu = 1;
						vertex2.tv = 1;
					}
				}

				modelVertexVector2.push_back(vertex2);
			}
		#endif

		}//x
	}//y
}

#define GETupperANDlower6(m_vertices){\
	/*Upper Triangle:*/\
	v1[0] = m_vertices[index].x;\
	v1[1] = m_vertices[index].y;\
	v1[2] = m_vertices[index].z;\
	index++;\
	v2[0] = m_vertices[index].x;\
	v2[1] = m_vertices[index].y;\
	v2[2] = m_vertices[index].z;\
	index++;\
	v3[0] = m_vertices[index].x;\
	v3[1] = m_vertices[index].y;\
	v3[2] = m_vertices[index].z;\
	index++;\
	\
	/*Lower Triangle:*/\
	v4[0] = m_vertices[index].x;\
	v4[1] = m_vertices[index].y;\
	v4[2] = m_vertices[index].z;\
	index++;\
	v5[0] = m_vertices[index].x;\
	v5[1] = m_vertices[index].y;\
	v5[2] = m_vertices[index].z;\
	index++;\
	v6[0] = m_vertices[index].x;\
	v6[1] = m_vertices[index].y;\
	v6[2] = m_vertices[index].z;\
	index++;\
}

#if defined SCENE_TERRAIN_COLLISION
float CTerrain::getTerrainHeight(UINT id, float xPos, float zPos)
{
	float height = 0;

	float realXpos, realZpos, v1[3], v2[3], v3[3], v4[3], v5[3], v6[3];
	int index;

	if (xPos < 0)
		xPos = 0;
	if (xPos >= m_terrainWidth - 1)
		xPos = m_terrainWidth - 1 - 0.01f;
	if (zPos < 0)
		zPos = 0;
	if (zPos >= m_terrainHeight - 1)
		zPos = m_terrainHeight - 1 - 0.01f;

	realXpos = xPos - ((int)xPos);
	realZpos = zPos - ((int)zPos);

	// Note: m_Terrain is matrix of 512 x 512 x 6
	index = (6 * (m_terrainWidth) * ((int)zPos) + (6 * (int)xPos));
	//  1---2,5
	//	|   / |
	//	|  /  |
	//	| /   |
	//  3,4---6

	if (id == 0 && m_terrainType < TERRAIN_LIGHT) { GETupperANDlower6(modelVertexVector0); }	//m_vertices_9
	if (id == 1 && m_terrainType < TERRAIN_LIGHT) { GETupperANDlower6(modelVertexVector1); }	//m_vertices_9
	#if !defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX //DX_ENGINE_LEVEL < 52
		if (id == 2 && m_terrainType < TERRAIN_LIGHT) { GETupperANDlower6(modelVertexVector2); }	//m_vertices_9
	#else
		if (id == 2 && m_terrainType == TERRAIN_LIGHT)	{ GETupperANDlower6(modelVertexVector2); } //m_vertices_12
		if (id == 3 && m_terrainType == TERRAIN)		{ GETupperANDlower6(modelVertexVector3); } //m_vertices_12
	#endif


	if (realXpos > realZpos)
		CheckHeightOfTrianglev2(xPos, zPos, height, v4, v5, v6);	//Lower Triangle
	else
		CheckHeightOfTrianglev2(xPos, zPos, height, v1, v2, v3);	//Upper Triangle
	return height;
}

bool CTerrain::CheckHeightOfTrianglev2(float x, float z, float& height, float v0[3], float v1[3], float v2[3])
{
	static float directionVector[3] = { 0.0f, -1.0f, 0.0f };	// The direction the ray is being cast.
	static float startVector[3] = { 0.0f, 0.0f, 0.0f };
	float edge1[3], edge2[3], normal[3];
	float magnitude, D, denominator, numerator, t;


	// Starting position of the ray that is being cast.
	startVector[0] = x;
	//startVector[1] = 0.0f;
	startVector[2] = z;

	// Calculate the two edges from the three points given.
	edge1[0] = v1[0] - v0[0];
	edge1[1] = v1[1] - v0[1];
	edge1[2] = v1[2] - v0[2];

	edge2[0] = v2[0] - v0[0];
	edge2[1] = v2[1] - v0[1];
	edge2[2] = v2[2] - v0[2];

	// Calculate the normal of the triangle from the two edges.
	normal[0] = (edge1[1] * edge2[2]) - (edge1[2] * edge2[1]);
	normal[1] = (edge1[2] * edge2[0]) - (edge1[0] * edge2[2]);
	normal[2] = (edge1[0] * edge2[1]) - (edge1[1] * edge2[0]);

	magnitude = FAST_sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) + (normal[2] * normal[2]));
	normal[0] = normal[0] / magnitude;
	normal[1] = normal[1] / magnitude;
	normal[2] = normal[2] / magnitude;

	// Find the distance from the origin to the plane.
	D = ((-normal[0] * v0[0]) + (-normal[1] * v0[1]) + (-normal[2] * v0[2]));

	// Get the denominator of the equation.
	denominator = ((normal[0] * directionVector[0]) + (normal[1] * directionVector[1]) + (normal[2] * directionVector[2]));

	// Make sure the result doesn't get too close to zero to prevent divide by zero.
	if (fabs(denominator) < 0.0001f)
		return false;

	// Get the numerator of the equation.
	numerator = -1.0f * (((normal[0] * startVector[0]) + (normal[1] * startVector[1]) +(normal[2] * startVector[2])) + D);

	// Calculate where we intersect the triangle.
	t = numerator / denominator;

	// Find the intersection vector.
	height = startVector[1] + (directionVector[1] * t);

	return true;
}
#endif

// Used by TerrainId [0][1][2][3]
// ----------------------------------------------------------------------------
void CTerrain::CreateTerrainModel(UINT id, std::vector<STRING> Textures, SHADER_TYPE shader_type)
// ----------------------------------------------------------------------------
{
	std::vector<UINT> indices; // (2 * (terrain_squares)*(terrain_squares));

	// Add TEXTURE: Create a model: NEW GLmodelClass || NEW DXmodelClass
	CREATE_MODEL_IF_NOT_EXCEPTION(SystemHandle->m_Application->m_TerrainModel[id], I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);  // m_TerrainModel[id] = NEW
	SystemHandle->m_Application->m_TerrainModel[id]->ModelHASfog = true;

#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
	if (id == 2 || id ==4)
	{
		//TRIANGLESTRIP
		for (int y = 0; y < terrain_squares - 1; y++) {
			indices.push_back(y * terrain_squares);									// upper left
			for (int x = 0; x < terrain_squares; x++) {
				indices.push_back(y * terrain_squares + x);							// upper right
				indices.push_back((y + 1) * terrain_squares + x);					// bottom left
			}

			indices.push_back((y + 1) * terrain_squares + (terrain_squares - 1));	// bottom right
		}

		#if defined DEBUG_TERRAIN_VERTICES
		{
			int total = indices.size();
			int k = 0;
			printf("Terrain Indices:\n");
			for (int i = 0; i < total; i++)
			{
				printf("%d - %d\n", k++, indices[i]);
			}
			printf("\n");
		}
		#endif
	}
#endif

	womalog("\n");


#if DX_ENGINE_LEVEL >= 52 && (defined SCENE_TERRAIN_WITH_NORMALS && defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX)
	// Calculate Terrain Normals
	if (id == 2) 
	{
		int index1, index2, index3, index;
		float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;

		// normals = new VectorType[(m_terrainHeight-1) * (m_terrainWidth-1)];
		std::vector<ModelShadowMapVertexType> normals;
		normals.resize((terrain_squares ) * (terrain_squares ));

		for (int y = 0; y < (terrain_squares - 1); ++y)
		{
			for (int x = 0; x < (terrain_squares - 1); ++x)
			{
				index1 = ((y + 1) * terrain_squares) + x;			// Bottom left vertex.
				index2 = ((y + 1) * terrain_squares) + (x + 1);		// Bottom right vertex.
				index3 = (y * terrain_squares) + x;					// Upper left vertex.
				// 1---2
				// |  /
				// | /
				// 0/

				// Get three vertices from the face.
				vertex1[0] = modelVertexVector2[index1].x;
				vertex1[1] = modelVertexVector2[index1].y;
				vertex1[2] = modelVertexVector2[index1].z;

				vertex2[0] = modelVertexVector2[index2].x;
				vertex2[1] = modelVertexVector2[index2].y;
				vertex2[2] = modelVertexVector2[index2].z;

				vertex3[0] = modelVertexVector2[index3].x;
				vertex3[1] = modelVertexVector2[index3].y;
				vertex3[2] = modelVertexVector2[index3].z;

				// Calculate the two vectors for this face.
				vector1[0] = vertex1[0] - vertex3[0];
				vector1[1] = vertex1[1] - vertex3[1];
				vector1[2] = vertex1[2] - vertex3[2];
				vector2[0] = vertex3[0] - vertex2[0];
				vector2[1] = vertex3[1] - vertex2[1];
				vector2[2] = vertex3[2] - vertex2[2];

				index = (y * (terrain_squares - 1)) + x;

				// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
				normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
				normals[index].y = -(vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
				normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);

				length = (float)sqrt((normals[index].x * normals[index].x) +
					(normals[index].y * normals[index].y) +
					(normals[index].z * normals[index].z));

				// Normalize the final value for this face using the length.
				normals[index].x = (normals[index].x / length);
				normals[index].y = (normals[index].y / length);
				normals[index].z = (normals[index].z / length);
			}
		}

		// Now go through all the vertices and take a sum of the face normals that touch this vertex.
		for (int y = 0; y < terrain_squares; y++)
		{
			for (int x = 0; x < terrain_squares; x++)
			{
				// Initialize the sum.
				sum[0] = 0.0f;
				sum[1] = 0.0f;
				sum[2] = 0.0f;

				// Bottom left face.
				if (((x - 1) >= 0) && ((y - 1) >= 0))
				{
					index = ((y - 1) * (terrain_squares - 1)) + (x - 1);

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
				}
				else
				// Bottom right face.
				if ((x < (terrain_squares - 1)) && ((y - 1) >= 0))
				{
					index = ((y - 1) * (terrain_squares - 1)) + x;

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
				}
				else
				// Upper left face.
				if (((x - 1) >= 0) && (y < (terrain_squares - 1)))
				{
					index = (y * (terrain_squares - 1)) + (x - 1);

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
				}
				else
				// Upper right face.
				if ((x < (terrain_squares - 1)) && (y < (terrain_squares - 1)))
				{
					index = (y * (terrain_squares - 1)) + x;

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
				}

				// Calculate the length of this normal.
				length = (float)sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

				// Get an index to the vertex location in the height map array.
				index = (y * terrain_squares) + x;

				float nx = sum[0] / length;
				float ny = sum[1] / length;
				float nz = sum[2] / length;

				// Normalize the final shared normal for this vertex and store it in the height map array.
				modelVertexVector2[index].nx = nx;
				modelVertexVector2[index].ny = ny;
				modelVertexVector2[index].nz = nz;
				//printf	("(%d %d)=%.2f %.2f %.2f| ", y, x, nx, ny, nz);

				TERRAIN_LEVEL53_COLOR_MAP
				modelVertexVector2[index].r = MAP_COLOR.x;
				modelVertexVector2[index].g = MAP_COLOR.y;
				modelVertexVector2[index].b = MAP_COLOR.z;
			}
			//printf("\n");
		}
	}
#endif

#if _DEBUG
	womalogauto( TEXT("TERRAIN %d Shader Type: %d\n"), id, shader_type);
#endif

#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
	if (id == 0) ASSERT(SystemHandle->m_Application->m_TerrainModel[id]->LoadTexture(TEXT("id0:under water"), m_Driver, shader_type, &Textures, &modelVertexVector0));
	if (id == 1) ASSERT(SystemHandle->m_Application->m_TerrainModel[id]->LoadTexture(TEXT("id1:water"), m_Driver, shader_type, &Textures, &modelVertexVector1));
	if (id == 2) ASSERT(SystemHandle->m_Application->m_TerrainModel[id]->LoadTerrain(TEXT("id2:terrain"), m_Driver, shader_type, &Textures, &modelVertexVector2, &indices));
#if defined DEBUG_COLLISION_TERRAIN
	//ModelTextureVertexType
	//float x, y, z;
	//float tu, tv;
	if (id == 3) ASSERT(SystemHandle->m_Application->m_TerrainModel[id]->LoadTexture(TEXT("id3:terrain"), m_Driver, shader_type, &Textures, &modelVertexVector3, &indices)); 
#endif
#else
	if (id == 0) ASSERT(SystemHandle->m_Application->m_TerrainModel[id]->LoadTexture(TEXT("id0:under water"), m_Driver, shader_type, &Textures, &modelVertexVector0));
	if (id == 1) ASSERT(SystemHandle->m_Application->m_TerrainModel[id]->LoadTexture(TEXT("id1:water"), m_Driver, shader_type, &Textures, &modelVertexVector1));
	if (id == 2) ASSERT(SystemHandle->m_Application->m_TerrainModel[id]->LoadTerrain(TEXT("id2:terrain"), m_Driver, shader_type, &Textures, &modelVertexVector2));
#endif

	#if defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
	if (id == 2 || id == 4) {
		if (shader_type < SHADER_Terrain_Texture_DEMO60) {
			SystemHandle->m_Application->m_TerrainModel[id]->PrimitiveTopology = TRIANGLESTRIP; //After: Load
		} else {
			SystemHandle->m_Application->m_TerrainModel[id]->PrimitiveTopology = TRIANGLELIST; //After: Load
		}
	}
	#endif

}

