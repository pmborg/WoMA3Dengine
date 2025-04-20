// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: virtualModelClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#pragma once
#include "platform.h"

/*
//
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
...
*/

//
// USAGE: D3D_PRIMITIVE_TOPOLOGY_LINELIST + PrimitiveTopology
//
enum PRIMITIVE_TOPOLOGY
{
	//MORE INFO: https://msdn.microsoft.com/en-us/library/windows/desktop/bb205124(v=vs.85).aspx
	NONNE = 0,
	POINTLIST,
	LINELIST,		// Sun Ray														2 Vertex = 1 LINE  --> DrawPrimitive( D3DPT_LINELIST, 0, 1 );
	LINESTRIP,		// ...not used...												6 Vertex = 5 LINEs --> DrawPrimitive( D3DPT_LINESTRIP, 0, 5 );
	TRIANGLELIST,	// DxModel default Defined in (CREATE_MODEL_IF_NOT_EXCEPTION)   1 Triang. = 3 Vert.--> DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
	TRIANGLESTRIP,	// Used on Optimized Sky Mesh: Sky/Sun/Moon Terrain          	4 Triang. = 6 Vert.--> DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
	// LINELIST_ADJ,
	// LINESTRIP_ADJ,
	// TRIANGLELIST_ADJ,
	// TRIANGLESTRIP_ADJ,
};

#include "main.h"
#include "WomaDriverClass.h"
#include "vertexTypes.h"
#include "GLmathClass.h"

////////////////////////////////////////////////////////////////////////////////	
// Class name: virtualModelClass
////////////////////////////////////////////////////////////////////////////////
class VirtualModelClass
{
public:
	virtual void Shutdown() = 0;

	virtual void RenderWithFade(float fadeLight = 1, bool FOG = false) = 0;
	virtual void Render(UINT camera = 0, UINT projection = 0, UINT pass = 0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL) = 0;

	virtual bool LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0) = 0;
#if DX_ENGINE_LEVEL >= 22 || defined INTRO_DEMO
	virtual bool LoadTexture(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureVertexType>* model, std::vector<UINT>* indexList = NULL, UINT instanceCount = 0) = 0;
#endif

#if defined USE_VIEW2D_SPRITES // Sprites
	virtual bool RenderSprite(int positionX, int positionY, float scale = 1.0f, float fade = 1.0f) = 0;
	virtual bool UpdateBuffersRotY(int positionX, int positionY) = 0;
	virtual bool UpdateSpriteBuffersRotY(int positionX, int positionY) = 0;
#endif
#if defined USE_LIGHT_RAY
	virtual void UpdateDynamic(std::vector<ModelColorVertexType>* lightVertexVector) = 0;
#endif

	virtual void Identity() = 0;
	virtual void multiply(void* m) = 0;	//XMMATRIX* or mat4*

	virtual void rotateX(float rad) = 0;
	virtual void rotateY(float rad) = 0;
	virtual void rotateZ(float rad) = 0;
	virtual void scale(float x, float y, float z) = 0;
	virtual void translation(float x, float y, float z) = 0;

	//VARS:
	// ----------------------------------------------------------------------
	UINT				WomaIntegrityCheck = 1234567222;

	INT					m_ObjId = -1;
	SHADER_TYPE			ModelShaderType = SHADER_AUTO;	//Used by shader.cpp
	PRIMITIVE_TOPOLOGY  PrimitiveTopology;
	UINT				m_vertexCount = 0, m_indexCount = 0;

	float				PosX = 0, PosY = 0, PosZ = 0;

	bool				Model3D = false;
	bool				ModelHASfog = false;
	bool				ModelHASlight = false;
	bool				ModelHASAlfaColor = false;
	bool				ModelHASColorMap = false;
	float				ModelAlfaColor = 0;
	int					shaderTypeParameter = 0;			//Used by HLSL CODE
	bool				isAnimatedBill = false;

	float				boundingSphere = 0;
	bool				ModelHASNormals = false;
	bool				ModelcomputeNormals = false;

};

