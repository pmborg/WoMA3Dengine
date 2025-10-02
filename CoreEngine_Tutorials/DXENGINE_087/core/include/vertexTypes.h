// ----------------------------------------------------------------------------------------------
// Filename: vertexTypes.h
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
//  MAIN PURPOSE: Define WoMA Vertices:
//
//	vertexTypes.h
//		DXshaderClass.h
//
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#pragma once

#include "platform.h"

#if defined DX_ENGINE //defined WINDOWS_PLATFORM && 
#include <D3D11.h>
#include <d3d12.h>		// DX12
#include "DXbasicTypes.h"
#endif

#if defined LINUX_PLATFORM
#include "linux.h"
#endif
#if defined ANDROID_PLATFORM
#include "defs.h"
#endif

//	-------------------------------------------------------------------------------------------
#pragma pack(push, 1)
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
#pragma pack(pop)

#pragma pack(push, 1)
union Vector3 {
	Vector3() { x = 0; y = 0; z = 0; }
	Vector3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
	struct {
		float x, y, z;
	};
};
#pragma pack(pop)
extern Vector3 operator+(const Vector3 vec, const Vector3& vec1);
extern Vector3 operator-(const Vector3 vec, const Vector3& vec1);
extern Vector3 operator*(const Vector3& vec, const float scale);

#pragma pack(push, 1)
union Vector2 {
	Vector2() { u = 0; v = 0; }
	Vector2(float u_, float v_) { u = u_; v = v_; }
	struct {
		float u, v;
	};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ModelTextureVertexType
{
	ModelTextureVertexType() { x = 0; y = 0; z = 0; tu = 0; tv = 0; }
	ModelTextureVertexType(Vector3 inPosition, Vector2 inUV) 
	{
		x = inPosition.x; y = inPosition.y; z = inPosition.z;
		tu = inUV.u;
		tv = inUV.v;
	}
	float x, y, z;
	float tu, tv;
};
#pragma pack(pop)

//	-------------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct ModelTextureLightVertexType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};
#pragma pack(pop)

//	-------------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct ModelNormalBumpVertexType
{
	float x, y, z;				//3
	float tu, tv;				//2
	float nx, ny, nz;			//3
	float tx, ty, tz;	//35	//3
	float bx, by, bz;	//35	//3
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ModelShadowMapVertexType
	//	-------------------------------------------------------------------------------------------
{
	float x, y, z;
};
#pragma pack(pop)

//	-------------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct ModelTextureDouble_Color_TerrainType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
	float r, g, b, a;
};
#pragma pack(pop)

//	-------------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct ModelTextureDouble_Color_TexMapping_TerrainType
{
	float x, y, z;		//POSITION
	float tu, tv;		//TEXCOORD0
	float nx, ny, nz;	//NORMAL
	float r, g, b, a;	//COLOR
	float tu2, tv2;		//TEXCOORD1
};
#pragma pack(pop)

#if DX_ENGINE_LEVEL >= 60 && defined USE_TERRAIN_TUTORIAL_CHAP_24
#pragma pack(push, 1)
struct HeightMapType_24						//vertex2 = {};
{											//modelVertexVector2;
	HeightMapType_24() 
	{
		x = 0; y = 0; z = 0;	//POSITION
		tu = 0; tv = 0; 		//TEXCOORD0
		tu2 = 0; tv2 = 0; 		
		nx= 0; ny= 0; nz = 0;	//NORMAL
		r= 0; g= 0; b = 0;		//COLOR
		Maptu= 0; Maptv = 0;	//TEXCOORD1
		tx= 0; ty= 0; tz = 0;	//TANGENT
		bx= 0; by= 0; bz = 0;	//BINORMAL
	}
	HeightMapType_24(
		float x_, float y_, float z_, 		//POSITION
		float tu_, float tv_, 				//TEXCOORD0
		float tu2_, float tv2_, 				
		float nx_, float ny_, float nz_,	//NORMAL
		float r_, float g_, float b_,		//COLOR
		float Maptu_, float Maptv_,			//TEXCOORD1
		float Maptu2_, float Maptv2_,			
		float tx_, float ty_, float tz_,	//TANGENT
		float bx_, float by_, float bz_		//BINORMAL	
	) {
		x = x_; y = y_; z = z_;				//POSITION
		tu = tu_; tv = tv_; 				//TEXCOORD0
		tu2 = tu2_; tv2 = tv2_;
		nx = nx_; ny = ny_; nz = nz_;		//NORMAL
		r = r_; g = g_; b = b_;				//COLOR
		Maptu = Maptu_; Maptv = Maptv_;		//TEXCOORD1
		Maptu2 = Maptu2_; Maptv2 = Maptv2_;	
		tx = tx_; ty = ty_; tz = tz_;		//TANGENT
		bx = bx_; by = by_; bz = bz_;		//BINORMAL
	}
	float x, y, z;			//POSITION
	float tu, tv, tu2, tv2;	//TEXCOORD0
	float nx, ny, nz;		//NORMAL
	float r, g, b;			//COLOR
	float Maptu, Maptv;		//TEXCOORD1
	float Maptu2, Maptv2;
	float tx, ty, tz;		//TANGENT
	float bx, by, bz;		//BINORMAL
};
#pragma pack(pop)
#endif


struct SentenceType
{
#if (defined OPENGL3 || defined OPENGL4)
	ModelTextureVertexType* GLvertices = NULL;	//OPENGL
	unsigned int	m_vertexArrayId = NULL;		//OPENGL
	unsigned int	m_vertexBufferId = NULL;	//OPENGL
	unsigned int	m_indexBufferId = NULL;		//OPENGL
#endif

	unsigned long* indices = NULL;				//DX Common?

#if defined DX_ENGINE
	DirectX::DXtextureVertexType* vertices = NULL;

#if defined DX9 || defined DX11
	ID3D11Buffer* vertexBuffer = NULL;	//DX11
	ID3D11Buffer* indexBuffer = NULL;	//DX11
#endif
#if defined DX12
	ID3D12Resource*				vertexBufferUpload = NULL;	//DX12
	ID3D12Resource*				indexBufferUpload = NULL;	//DX12
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView;	//DX12
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView; 	//DX12
	ID3D12Resource*				m_vertexBuffer = NULL;		//DX12
	ID3D12Resource*				m_indexBuffer = NULL; 		//DX12
#endif
#endif

	int vertexCount=0, indexCount=0, maxLength=0;			//Common
	float red=-1, green = -1, blue = -1;					//Common
};

//	-------------------------------------------------------------------------------------------
// NOTE: Based: https://stackoverflow.com/questions/20412807/directx-11-make-a-square

// DX		is left - handed
// OpenGL	is right - handed

//
// Used to Map Terrains:
//
// NOTE: Swapping y and z, really need? (YES, because Z brings height and in DirectX Y = Height)

#define ModelVertexs0(vertex, modelVertexVector, Ul_x, Ul_y, Ul_z, Ur_x, Ur_y, Ur_z, Bl_x, Bl_y, Bl_z, Br_x, Br_y, Br_z)\
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
	/*  1  v1 v2		*/\
	/* -1  v3 			*/\
	/*  x: -1  1		*/\
	/*					*/\
	/* v1 Upper left:	*/\
	vertex.x = Ul_x;\
	vertex.z = Ul_y;\
	vertex.y = Ul_z;\
	vertex.tu = 0;\
	vertex.tv = 0;\
	modelVertexVector.push_back(vertex);\
	\
	/* v2 Upper right:	*/\
	vertex.x = Ur_x;\
	vertex.z = Ur_y;\
	vertex.y = Ur_z;\
	vertex.tu = 1;\
	vertex.tv = 0;\
	modelVertexVector.push_back(vertex);\
	\
	/* v3 Bottom left:	*/\
	vertex.x = Bl_x; \
	vertex.z = Bl_y; \
	vertex.y = Bl_z; \
	vertex.tu = 0; \
	vertex.tv = 1; \
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
	/* v1 Bottom Left:	*/\
	vertex.x = Bl_x;\
	vertex.z = Bl_y;\
	vertex.y = Bl_z;\
	vertex.tu = 0;\
	vertex.tv = 1;\
	modelVertexVector.push_back(vertex);\
	\
	/* v2 Upper right:	*/\
	vertex.x = Ur_x; \
	vertex.z = Ur_y; \
	vertex.y = Ur_z; \
	vertex.tu = 1; \
	vertex.tv = 0; \
	modelVertexVector.push_back(vertex);\
	\
	/* v3 Bottom Right:	*/\
	vertex.x = Br_x;\
	vertex.z = Br_y;\
	vertex.y = Br_z;\
	vertex.tu = 1;\
	vertex.tv = 1;\
	modelVertexVector.push_back(vertex);\
}

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
#define ModelVertexs2(vertex, modelVertexVector, Ul_x, Ul_y, Ul_z, Ur_x, Ur_y, Ur_z, Bl_x, Bl_y, Bl_z, Br_x, Br_y, Br_z)\
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
#define CREATE_VERTEXVECTOR_SQUAD_MODEL(model, X,Z,Y){\
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
#define CREATE_VERTEXVECTOR_SQUAD_MODEL_OPTIMIZED(model, X,Z,Y){\
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