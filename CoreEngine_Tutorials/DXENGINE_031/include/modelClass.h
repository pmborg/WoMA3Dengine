// --------------------------------------------------------------------------------------------
// Filename: ModelClass.h
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
//WomaIntegrityCheck = 1234567830;

#pragma once
#include "womadriverclass.h"
#include "DX12TextureClass.h"

// --------------------------
// "OBJ" MATERIAL FORMAT:
// --------------------------
typedef struct
{
	char matName[100];					//size:100 100xchar

	XMFLOAT4 diffuseColor;				//size:16
	XMFLOAT4 ambientColor;				//size:16
	XMFLOAT4 emissiveColor;				//size:16

	int texArrayIndex;					//size:4
	bool hasTexture;					//size:1

	bool transparent;					//size:1	>= 43
	ID3D11ShaderResourceView* alfaMap11;//size:8	>= 43

	bool bSpecular;						//size:1	>= 44: NEW SPECULAR + SHININESS:
	XMFLOAT3 specularColor;				//size:12	>= 44: NEW SPECULAR + SHININESS:
	int nShininess;						//size:4	//>= 44: NEW SPECULAR + SHININESS:

	bool hasNormMap;					//size:1	>=47: NEW BUMP
	int normMapTexArrayIndex;			//size:4	>=47: NEW BUMP
	//							TOTAL		 184
} SurfaceMaterial;

typedef struct
{
	STRING fileNameOnly;

	//Vertex definition indices
	std::vector<int> vertPosIndex;	//vertPos Indexs
	std::vector<int> vertTCIndex;	//vertTexCoord Indexs
	std::vector<int> vertNormIndex;	//vertNorm Indexs

	UINT	triangleCount = 0;	//Total Triangles
	UINT	meshTriangles = 0;

	std::vector<STRING>		meshMaterials;

	std::vector<XMFLOAT3>	vertPos;		//40
	std::vector<XMFLOAT2>	vertTexCoord;	//41
	std::vector<XMFLOAT3>	vertNorm;		//42

	//Make sure we have a default if no tex coords or normals are defined
	bool hasTexCoord = false;				//ch07
	bool hasNorm = false;					//ch12
	bool hasRenderShadow = false;	//ch45
	bool hasNormMap = false;		//ch51
	bool hasTransparent = false;	// has transparent sub-meshes?

	UINT meshSubsets = 0;					// Num. of sub-meshes

	std::vector<UINT> indices32;	// DX >= 9.2
	std::vector<WORD> indices16;	// DX >= 9.0 Shader 2.0

	std::vector<int> meshSubsetIndexStart;	// Start Index of each subset
	std::vector<int> subsetMaterialArray;	// Index of material to use in each subset

	std::vector<SurfaceMaterial> material;
	std::vector<STRING> textureNameArray;	// filename of all textures loaded

	//---------------------------------------------------------------------
	UINT m_vertexCount = 0, m_indexCount = 0;			//virtualModelClass.h
} OBJ3D;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
public:
	UINT WomaIntegrityCheck = 1234567830;
	OBJ3D obj3d;
	
	ModelClass();
	~ModelClass();

	bool LoadOBJ(/*DXmodelClass*/ void* dxmodelClass, SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow = false, bool renderShadow = false, UINT instanceCount = 0);
	bool CreateObject(/*DXmodelClass*/ void* dxmodelClass, TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow, bool renderShadow);

	TCHAR	checkChar;			//The variable we will use to store one char from file at a time
	STRING	face;				//Holds the string containing our face vertices
	int		vIndex = 0;			//Keep track of our vertex index count

	//Temp variables to store into vectors
	STRING	meshMaterialsTemp;
	int		vertPosIndexTemp = 0;
	int		vertNormIndexTemp = 0;
	int		vertTCIndexTemp = 0;

	STRING	meshMatLib;		//String to hold our obj material library filename
};
