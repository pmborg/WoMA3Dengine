// --------------------------------------------------------------------------------------------
// Filename: w3d.h
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
// PURPOSE: Entry point of all WoMA ENGINE Applications OS: WINDOWS
// --------------------------------------------------------------------------------------------
// LOAD / SAVE: The fast static 3D object format of Woma.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#pragma once
// --------------------------
// "W3D" HEADER FORMAT:
// --------------------------
struct MyObject3D
{
	MyObject3D() { /*ZeroMemory(this, sizeof(this));*/ }

	CHAR	 version[9] = "W3D v1.0";
	XMFLOAT3 min = { 0,0,0 };
	XMFLOAT3 max = { 0,0,0 };

	UINT	 verticesCount = 0;
	UINT	 size_verticesCount = 0;

	UINT	 indicesCount = 0;
	UINT	 size_indicesCount = 0;

	UINT	 meshSubsetIndexStartCount = 0;
	UINT	 size_meshSubsetIndexStartCount = 0;

	UINT	 subsetMaterialArrayCount = 0;
	UINT	 size_subsetMaterialArrayCount = 0;

	UINT	 materialCount = 0;
	UINT	 size_materialCount = 0;

	UINT	 texturenameCount = 0;
	UINT	 size_texturenameCount = 0;
};

namespace DirectX
{
    // --------------------------
    // "W3D" MATERIAL FORMAT:
    // --------------------------
    //Aux struct:
    typedef struct
    {
        char fileName[256]; //Have to be CHAR!
    } textureName;
};

extern bool extLoadW3DfromMEM(void* pContext, DirectX::DXmodelClass* dxmodelClass, SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow, bool renderShadow, UINT instanceCount);
