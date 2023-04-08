// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: DXbasicTypes.h
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
// PURPOSE: 
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567829;

#pragma once

#include "platform.h"
#include "DXdrivers.h"
#include "standard_platform.h"
#include <DirectXMath.h>// Use Math
#include <fstream>

//#if defined DX_ENGINE
using namespace std;

typedef struct
{
	void *blobVS, *blobGS, *blobPS;
	UINT sizeVS, sizeGS, sizePS;
} shaderTree;

namespace DirectX {
	//private:

	//21:
	// -------------------------------------------------------------------------------------------
	struct DXcolorVertexType
	{
	DXcolorVertexType(){}
	DXcolorVertexType(float x, float y, float z,
		float r, float g, float b, float a)
		: position(x, y, z), color(r, g, b, a) {}
		XMFLOAT3 position;
	    XMFLOAT4 color;
#if defined DX9sdk
		D3DXVECTOR3 position;
		DWORD		color;
#endif
	};

	#if defined DX9sdk
		#define CUSTOMVERTEX_XYZ_DIFFUSE_DX9 DXcolorVertexType
		#define CUSTOMFVF_XYZ_DIFFUSE_DX9 (D3DFVF_XYZ | D3DFVF_DIFFUSE)
	#endif

	//22:
	// -------------------------------------------------------------------------------------------
	struct DXtextureVertexType
	{
	DXtextureVertexType(){}

	DXtextureVertexType(float x, float y, float z, 
						float u, float v)
						: position(x,y,z), texCoord(u, v){}
//#if defined DX11 || defined DX12 || defined DX9
		XMFLOAT3 position;
	    XMFLOAT2 texCoord;
//#endif
#if defined DX9sdk
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texCoord;
#endif
	};

	#if defined DX9sdk
		#define CUSTOMVERTEX_XYZ_TEXTURE_DX9 DXtextureVertexType
		#define CUSTOMFVF_XYZ_TEXTURE_DX9 (D3DFVF_XYZ | D3DFVF_TEX1)
	#endif

	//23:
	// -------------------------------------------------------------------------------------------
	struct DXtextureLightVertexType
	{
	DXtextureLightVertexType(){}

	DXtextureLightVertexType(	float x, float y, float z,
								float u, float v,
								float nx, float ny, float nz)
								: position(x,y,z), texCoord(u, v), normal(nx, ny, nz){}
//#if defined DX11 || defined DX12 || defined DX9
		XMFLOAT3 position;
	    XMFLOAT2 texCoord;
		XMFLOAT3 normal;
//#endif
#if defined DX9sdk
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
#endif
	};

	#if defined DX9sdk
		#define CUSTOMVERTEX_XYZ_LIGHT_DX9 DXtextureLightVertexType
		#define CUSTOMFVF_XYZ_LIGHT_DX9 (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL)
	#endif

	// 45: Shadow
	// -------------------------------------------------------------------------------------------
	struct DXShadowMapVertexType
	{
	DXShadowMapVertexType(){}
	DXShadowMapVertexType(	float x, float y, float z, 
							float r, float g, float b, float a)
							: position(x,y,z){}

		XMFLOAT3 position;
	};
	
	// 47: NormalBump
	// -------------------------------------------------------------------------------------------
	struct DXNormalBumpVertexType
	{
	DXNormalBumpVertexType(){}
	DXNormalBumpVertexType(	float x, float y, float z, 
							float u, float v, 
							float nx, float ny, float nz, 
							float tx, float ty, float tz)
							: position(x,y,z), texCoord(u, v), normal(nx, ny, nz),tangent(tx, ty, tz){}

		XMFLOAT3 position;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;

		//XMFLOAT3 biTangent;
	};

	// 66: SHADER_Double_Color_Terrain
	// -------------------------------------------------------------------------------------------
	struct DXTextureDouble_Color_TerrainType
	{
		DXTextureDouble_Color_TerrainType() {}
		DXTextureDouble_Color_TerrainType(
			float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz,
			float r, float g, float b, float a)
			: position(x, y, z), texCoord(u, v), normal(nx, ny, nz), color(r, g, b, a) {}

		XMFLOAT3 position;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT4 color;
	};
}
