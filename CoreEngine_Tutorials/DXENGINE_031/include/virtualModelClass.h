// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: virtualModelClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#pragma once

#include "platform.h"

/*
// d3dcommon.h
//

D3D11_PRIMITIVE_TOPOLOGY_POINTLIST	= D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
D3D11_PRIMITIVE_TOPOLOGY_LINELIST	= D3D_PRIMITIVE_TOPOLOGY_LINELIST,
D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP	= D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST	= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP	= D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ	= D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ	= D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ	= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ	= D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST

//
// USAGE: D3D_PRIMITIVE_TOPOLOGY_LINELIST + PrimitiveTopology
*/

typedef
enum PRIMITIVE_TOPOLOGY
{
	//https://msdn.microsoft.com/en-us/library/windows/desktop/bb205124(v=vs.85).aspx
	NONNE = 0,
	POINTLIST,
	LINELIST,		// Sun Ray														2 Vertex = 1 LINE  --> DrawPrimitive( D3DPT_LINELIST, 0, 1 );
	LINESTRIP,		// ...not used...												6 Vertex = 5 LINEs --> DrawPrimitive( D3DPT_LINESTRIP, 0, 5 );
	TRIANGLELIST,	// DxModel default Defined in (CREATE_MODEL_IF_NOT_EXCEPTION)   1 Triang. = 3 Vert.--> DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
	TRIANGLESTRIP,	// Used on Optimized Sky Mesh: Sky/Sun/Moon Terrain:Level25		4 Triang. = 6 Vert.--> DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
					//LINELIST_ADJ,
					//LINESTRIP_ADJ,
					//TRIANGLELIST_ADJ,
					//TRIANGLESTRIP_ADJ,
};


#include "main.h"
#include "womadriverclass.h"
#include "vertexTypes.h"
#include "GLmathClass.h"

////////////////////////////////////////////////////////////////////////////////	
// Class name: virtualModelClass
////////////////////////////////////////////////////////////////////////////////
class VirtualModelClass
{
public:
	virtual void Shutdown() = 0;

	virtual void RenderWithFade(WomaDriverClass* driver, float fadeLight = 1) = 0;
	virtual void RenderSky(WomaDriverClass* driver, UINT camera, float fadeLight = 1) = 0;
	virtual void Render(WomaDriverClass* driver, UINT camera = 0, UINT projection = 0, UINT pass = 0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL) = 0;

	virtual bool LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0) = 0;
	virtual bool LoadTexture(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0) = 0;
	virtual bool LoadLight(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureLightVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0) = 0;

	virtual bool RenderSprite(void* Driver, int positionX, int positionY, float scale=1.0f, float fade = 1.0f)=0;
	virtual bool UpdateBuffersRotY(void* Driver, int positionX, int positionY)=0;
	virtual void UpdateDynamic(void* Driver, std::vector<ModelColorVertexType>* lightVertexVector) = 0;
	virtual bool LoadModel(TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow = false, bool renderShadow=false, UINT instanceCount=0) = 0;

	virtual void Identity() = 0;
	virtual void multiply (void* m)=0;	//XMMATRIX* or mat4*

	virtual void rotateX (float rad)=0;
	virtual void rotateY (float rad)=0;
	virtual void rotateZ (float rad)=0;
	virtual void scale(float x, float y, float z)=0;
	virtual void translation(float x, float y, float z)=0;

	//VARS:
	// ----------------------------------------------------------------------
	UINT WomaIntegrityCheck = 1234567831;

	UINT				m_ObjId;
	SHADER_TYPE			ModelShaderType;

	bool				Model3D;
	bool				ModelHASfog;
	bool				ModelHASlight;
	bool				ModelHASAlfaColor;
	bool				ModelHASColorMap;
	float				ModelAlfaColor;
	PRIMITIVE_TOPOLOGY  PrimitiveTopology;

	UINT				m_vertexCount, m_indexCount;

	float				PosX, PosY, PosZ;
	float				boundingSphere;

	int		SpriteTextureWidth;		//NOTE: Have to be "int"
	int		SpriteTextureHeight;	//NOTE: Have to be "int"

	bool	ModelHASNormals = false;
	bool	ModelcomputeNormals = false;
};

