// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: vertexTypes.h
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
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
//  MAIN PURPOSE: Define WoMA Vertices:
//
//	vertexTypes.h
//		DXshaderClass.h
//
// ----------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"

#include "main.h"

//	-------------------------------------------------------------------------------------------
struct ModelColorVertexType
{
#ifdef NOT_USED
	ModelColorVertexType(){x=0; y=0; z=0; r=0; g=0; b=0; a=0;}
	ModelColorVertexType(float X, float Y, float Z, 
						 float R, float G, float B, float A)
						{x=X; y=Y; z=Z; r=R; g=G; b=B; a=A;}
#endif
	float x, y, z;
	float r, g, b, a; // This will allow a dif. color per vertice
};

//	-------------------------------------------------------------------------------------------
struct ModelTextureVertexType
{
	float x, y, z;
	float tu, tv;
};

//	-------------------------------------------------------------------------------------------
struct ModelTextureLightVertexType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};


struct SentenceType
{
	void *vertexBuffer, *indexBuffer;		//ID3D11Buffer*
	int vertexCount, indexCount, maxLength;
	float red, green, blue;
};

//	-------------------------------------------------------------------------------------------
// NOTE: Based: https://stackoverflow.com/questions/20412807/directx-11-make-a-square

// DX		is left - handed
// OpenGL	is right - handed

//
// Used to Map Terrains:
//
// NOTE: Swapping y and z, really need? (YES, because Z brings height and in DirectX Y = Height)


#define ModelVertexs(vertex, modelVertexVector, Ul_x, Ul_y, Ul_z, Ur_x, Ur_y, Ur_z, Bl_x, Bl_y, Bl_z, Br_x, Br_y, Br_z)\
{\
	/* POINTS */\
    /*Ul    Ur*/\
	/*        */\
	/*        */\
	/*Bl    Br*/\
	\
	\
	/* |\				*/\
	/* ---				*/\
	/*  y:				*/\
	/*  1  v2 v3		*/\
	/* -1  v1 			*/\
	/*  x: -1  1		*/\
	/*					*/\
	/* v1 Top middle:	*/\
	vertex.x = Bl_x;\
	vertex.z = Bl_y;\
	vertex.y = Bl_z;\
	vertex.tu = 0;\
	vertex.tv = 1;\
	modelVertexVector.push_back(vertex);\
	\
	/* v2 Bottom right:	*/\
	vertex.x = Ul_x;\
	vertex.z = Ul_y;\
	vertex.y = Ul_z;\
	vertex.tu = 0;\
	vertex.tv = 0;\
	modelVertexVector.push_back(vertex);\
	\
	/* v3 Bottom left:	*/\
	vertex.x = Ur_x; \
	vertex.z = Ur_y; \
	vertex.y = Ur_z; \
	vertex.tu = 1; \
	vertex.tv = 0; \
	modelVertexVector.push_back(vertex);\
	\
	/* POINTS */\
    /*Ul    Ur*/\
	/*        */\
	/*        */\
	/*Bl    Br*/\
	\
	/* ---				*/\
	/*  \|				*/\
	/*  y:				*/\
	/*  1     v2		*/\
	/* -1  v1 v3   		*/\
	/*  x: -1  1		*/\
	\
	/* v1 Top middle:	*/\
	vertex.x = Bl_x;\
	vertex.z = Bl_y;\
	vertex.y = Bl_z;\
	vertex.tu = 0;\
	vertex.tv = 1;\
	modelVertexVector.push_back(vertex);\
	\
	/* v2 Bottom right:	*/\
	vertex.x = Ur_x; \
	vertex.z = Ur_y; \
	vertex.y = Ur_z; \
	vertex.tu = 1; \
	vertex.tv = 0; \
	modelVertexVector.push_back(vertex);\
	\
	/* v3 Bottom left:	*/\
	vertex.x = Br_x;\
	vertex.z = Br_y;\
	vertex.y = Br_z;\
	vertex.tu = 1;\
	vertex.tv = 1;\
	modelVertexVector.push_back(vertex);\
}

//	-------------------------------------------------------------------------------------------
// |\
// ---
//  y:
//  1  v1
// -1  v4 v3
//  x: -1  1

// ---
//  \|
//  y:
//  1  v1 v2
// -1     v3
//  x: -1  1

//
// Used by: SPRITES: Sun, Moon, initIntroDemo (square titles), 2D Sky, Title Banner
//
#define CREATE_VERTEXVECTOR_SQUAR_MODEL(model, X,Z,Y){\
	/* v1 Left Top: */\
	vertex.x =-X;\
	vertex.z = Z;\
	vertex.y = Y;\
	model.push_back(vertex);\
	/* v3 Right Bottom: */\
	vertex.x =  X;\
	vertex.z = -Z;\
	vertex.y =  Y;\
	model.push_back(vertex);\
	/* v4 Left Bottom: */\
	vertex.x = -X;\
	vertex.z = -Z;\
	vertex.y =  Y;\
	model.push_back(vertex);\
	/* v1 Left Top: */\
	vertex.x =-X;\
	vertex.z = Z;\
	vertex.y = Y;\
	model.push_back(vertex);\
	/* v2 Right Top: */\
	vertex.x =  X;\
	vertex.z =  Z;\
	vertex.y =  Y;\
	model.push_back(vertex);\
	/* v3 Right Bottom: */\
	vertex.x =  X;\
	vertex.z = -Z;\
	vertex.y =  Y;\
	model.push_back(vertex);\
}

//	-------------------------------------------------------------------------------------------
// ---
// |\|
// ---
//  y:
//  1  v1 v2
// -1  v4 v3
//  x: -1  1

//
// Used by Demo: 21: Color, 22: Texture, 23 Light (with Index: indexList[6] = {0,1,2, 0,3,1};)
//
#define CREATE_VERTEXVECTOR_SQUAR_MODEL_OPTIMIZED(model, X,Z,Y){\
	/* v1 Left Top: */\
	vertex.x =-X;\
	vertex.z = Z;\
	vertex.y = Y;\
	model.push_back(vertex);\
	/* v3 Right Bottom: */\
	vertex.x =  X;\
	vertex.z = -Z;\
	vertex.y =  Y;\
	model.push_back(vertex);\
	/* v4 Left Bottom: */\
	vertex.x = -X;\
	vertex.z = -Z;\
	vertex.y =  Y;\
	model.push_back(vertex);\
	/* v2 Right Top: */\
	vertex.x =  X;\
	vertex.z =  Z;\
	vertex.y =  Y;\
	model.push_back(vertex);\
}

//   v1
// v2  v3

#define CREATE_VERTEXVECTOR_TRIANGLE_MODEL_OPTIMIZED(model, X,Z,Y){\
	/* v1 Left Top: */\
	vertex.x = -X;\
	vertex.y = -Y;\
	vertex.z = 0;\
	model.push_back(vertex);\
	/* v3 Right Bottom: */\
	vertex.x =  0;\
	vertex.y =  Y;\
	vertex.z =  0;\
	model.push_back(vertex);\
	/* v2 Left Bottom: */\
	vertex.x = X;\
	vertex.y = -Y;\
	vertex.z = 0;\
	model.push_back(vertex);\
}

//	-------------------------------------------------------------------------------------------
#define MAP_XZtoUV(model,X,Y,Z) {\
for (UINT i = 0; i < model.size(); i++)\
{\
	if (model[i].x == X)\
		model[i].tu = 1.0f;\
	else\
		model[i].tu = 0.0f;\
	\
	if (model[i].z == Y) \
		model[i].tv = 0.0f;\
	else\
		model[i].tv = 1.0f;\
}\
}

//	-------------------------------------------------------------------------------------------
#define MAP_XYtoUV(model,X,Y,Z) {\
for (UINT i = 0; i < model.size(); i++)\
{\
	if (model[i].x == X)\
		model[i].tu = 1.0f;\
	else\
		model[i].tu = (i==1)?0.5f:0.0f;\
	\
	if (model[i].y == Y) \
		model[i].tv = 0.0f;\
	else\
		model[i].tv = 1.0f;\
}\
}

//	-------------------------------------------------------------------------------------------
#define ModelTex(vertex)\
{\
	vertex.x = x0 + x*unit;\
	vertex.z = y0 + y*unit;\
	vertex.y = (scaleFactor * height[y][x]) - 5;\
	if (y % 2 == 0) {\
		if (x % 2 == 0) {\
			vertex.tu = 0;\
			vertex.tv = 0;\
		}\
		else {\
			vertex.tu = 1;\
			vertex.tv = 0;\
		}\
	}\
	else {\
		if (x % 2 == 0) {\
			vertex.tu = 0;\
			vertex.tv = 1;\
		}\
		else {\
			vertex.tu = 1;\
			vertex.tv = 1;\
		}\
	}\
}