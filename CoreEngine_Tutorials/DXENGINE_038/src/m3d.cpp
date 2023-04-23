// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: m3d.cpp
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
// PURPOSE: Entry point of all WoMA ENGINE Applications OS: WINDOWS
// --------------------------------------------------------------------------------------------
// LOAD / SAVE: The fast static 3D object format of Woma.
// --------------------------------------------------------------------------------------------

#include "platform.h"
#include "OSengine.h"		// Get [SystemHandle] Pointer to System Class & define WomaSYSTEM for: WINDOWS, LINUX & ANDROID
#include "OSmain_dir.h"
#include "DXmodelClass.h"
#include "fileLoader.h"
#include "m3d.h"
#include "mem_leak.h"

namespace DirectX 
{

STRING ExtractFilename1(const STRING& path)
{
    return path.substr(path.find_last_of('\\') + 1);
}
STRING ExtractFilename2(const STRING& path)
{
    return path.substr(path.find_last_of('/') + 1);
}

// --------------------------
// "M3D" MATERIAL FORMAT:
// --------------------------
//Aux struct:
typedef struct
{
	char fileName[100]; //Have to be CHAR!
} textureName;

#pragma warning( push )
#pragma warning( disable : 4127 ) // Disable warning C4127: conditional expression is constant
#pragma warning( disable : 4102 ) // Disable warning C4102: 'found' : unreferenced label
#pragma warning( disable : 4702 ) // Disable warning C4702 : unreachable code
#pragma warning( disable : 4267 ) // Disable warning C4267: '=' : conversion from 'size_t' to 'int', possible loss of data
// --------------------------------------------------------------------------------------------
//ModelColorVertexType t;
//ModelTextureVertexType
//ModelTextureLightVertexType
//ModelNormalBumpVertexType

#define FIN_WRITE(fullMsg, sizeoffullMsg) { fin.write(fullMsg, sizeoffullMsg); /*WOMA_LOGManager_DebugMSGAUTO(TEXT("WRITE: sizeof(fullMsg): %d\n"), sizeoffullMsg);*/ }

// --------------------------------------------------------------------------------------------
bool DXmodelClass::SaveM3D(STRING filename, void* indices, void* vertices, UINT sizeofMODELvertex)
{
	if (WOMA::game_state == GAME_STOP)
		return false;

    MyObject3D M3D;
	//------------------------------------------------------------------------------------------------------------------
	if (ModelShaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW) // No special case for: SHADER_TEXTURE_LIGHT_RENDERSHADOW
		ModelShaderType = SHADER_TEXTURE_LIGHT;
	float version = 1.0f + (float)ModelShaderType / 10;
	StringCchPrintfA(M3D.version, sizeof(M3D.version), "M3D v%.1f", version);
	WOMA_LOGManager_DebugMSG("M3D.version: %s\n", M3D.version);

    M3D.min = minVertex;
    M3D.max = maxVertex;

    M3D.verticesCount = m_vertexCount;	// TOTAL: Vertice ==> Equal to: "m_vertexCount"
	M3D.size_verticesCount = sizeofMODELvertex;
	M3D.indicesCount = m_indexCount;	// TOTAL: Indice  ==> Equal to: "m_indexCount"
	M3D.size_indicesCount = sizeof(UINT);

    M3D.materialCount = obj3d.material.size();
	M3D.size_materialCount = sizeof(SurfaceMaterial);
    M3D.texturenameCount = obj3d.textureNameArray.size();				// Related with N. of "Texture" Resources
	M3D.size_texturenameCount = sizeof(textureName);

    M3D.meshSubsetIndexStartCount = obj3d.meshSubsetIndexStart.size();// meshSubsetIndexStart = subsetMaterialArray + 1
	M3D.size_meshSubsetIndexStartCount = sizeof(int);
    M3D.subsetMaterialArrayCount = obj3d.subsetMaterialArray.size();
	M3D.size_subsetMaterialArrayCount = sizeof(int);
	#if need_on_debug
	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.min: %.1f %.1f %.1f\n"), M3D.min.x, M3D.min.y, M3D.min.z );
	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.max: %.1f %.1f %.1f\n"), M3D.max.x, M3D.max.y, M3D.max.z);

	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.verticesCount: %d\n"), M3D.verticesCount);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.indicesCount: %d\n"), M3D.indicesCount);

	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.materialCount: %d\n"), M3D.materialCount);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.texturenameCount: %d\n"), M3D.texturenameCount);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.meshSubsetIndexStartCount: %d\n"), M3D.meshSubsetIndexStartCount);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("M3D.subsetMaterialArrayCount: %d\n"), M3D.subsetMaterialArrayCount);
	#endif
	// Check: m_indexCount (too big: to be used in real time rendering...)
    if (m_indexCount > 65535)
    {
        WOMA_LOGManager_DebugMSGAUTO(TEXT("FATAL ERROR SaveM3D: indicesCount > 65535: %d\n"), m_indexCount);
        MessageBox(NULL, filename.c_str(), TEXT("SaveM3D: indicesCount > 65535 (i.e. This object is too Big!)"), MB_OK); return false;
    }
	// Check: meshSubsetIndexStart (use 3d studio max to optimize mesh!)
    if (obj3d.meshSubsetIndexStart.size() > 255)
    {
        WOMA_LOGManager_DebugMSGAUTO(TEXT("WARNING! SaveM3D: meshSubsetIndexStartCount > 255: %d\n"), obj3d.meshSubsetIndexStart.size());
    }
	// Check: meshSubsetIndexStart (too complex: to be used in real time rendering...)
    if (obj3d.meshSubsetIndexStart.size() > 65535)
    {
        WOMA_LOGManager_DebugMSGAUTO(TEXT("ERROR SaveM3D: meshSubsetIndexStartCount > 65535: %d\n"), obj3d.meshSubsetIndexStart.size());
        MessageBox(NULL, filename.c_str(), TEXT("SaveM3D: meshSubsetIndexStartCount > 65535"), MB_OK); return false;
    }
	//Check: subsetMaterialArray (use too many materials: )
    if (obj3d.subsetMaterialArray.size() > 65535)
    {
        WOMA_LOGManager_DebugMSGAUTO(TEXT("ERROR SaveM3D: subsetMaterialArrayCount > 65535: %d\n"), obj3d.subsetMaterialArray.size());
        MessageBox(NULL, filename.c_str(), TEXT("SaveM3D: subsetMaterialArrayCount > 65535"), MB_OK); return false;
    }

	// OPEN: File
#if defined SAVEM3D
	filename.replace(filename.size() - 3, 3, TEXT("M3D"));
#endif
    std::ofstream fin(filename, std::ios::out | std::ios::binary); // Dont use: OFSTREAM

	// DUMP: HEADER DATA INDX
	// --------------------------------------------------------------------------------------------------------------------------------
	
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(M3D): %d\n"), sizeof(M3D));
	FIN_WRITE((char*) &M3D, sizeof(M3D));

	// DUMP: vertices
    // --------------------------------------------------------------------------------------------------------------------------------
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeofMODELvertex: %d\n"), sizeofMODELvertex);
	FIN_WRITE((char*) vertices, M3D.verticesCount*sizeofMODELvertex);

	// DUMP: indices
    // --------------------------------------------------------------------------------------------------------------------------------
	// Scale down all "int" to "short"
	#if DX9sdk
	FIN_WRITE((char*) &indices16[0], M3D.indicesCount * sizeof(WORD));
	#else
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(UINT): %d\n"), sizeof(UINT));
	FIN_WRITE((char*) &obj3d.indices32[0], M3D.indicesCount * sizeof(UINT));
	#endif

	// DUMP: SubsetIndexStart
    // --------------------------------------------------------------------------------------------------------------------------------
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(int): %d\n"), sizeof(int));
	FIN_WRITE((char*) &obj3d.meshSubsetIndexStart[0], M3D.meshSubsetIndexStartCount * sizeof(int));

	// DUMP: subsetMaterialArray
    // --------------------------------------------------------------------------------------------------------------------------------
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(int): %d\n"), sizeof(int));
	FIN_WRITE((char*) &obj3d.subsetMaterialArray[0], M3D.subsetMaterialArrayCount * sizeof(int));

	// DUMP: Materials
    // --------------------------------------------------------------------------------------------------------------------------------
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(SurfaceMaterial): %d\n"), sizeof(SurfaceMaterial));
	FIN_WRITE((char*) &obj3d.material[0], M3D.materialCount * sizeof(SurfaceMaterial));

	// DUMP: Texture Names
    // --------------------------------------------------------------------------------------------------------------------------------
    textureName *texturename = NEW textureName[M3D.texturenameCount];
    ZeroMemory(texturename, M3D.texturenameCount * sizeof(textureName));

    for (UINT i = 0; i < M3D.texturenameCount; i++) 
	{
		#if defined UNICODE
			char textureNameArrayA[MAX_STR_LEN] = { 0 };
			wtoa(textureNameArrayA, textureNameArray[i].c_str(), MAX_STR_LEN);
			strcpy_s(texturename[i].fileName, sizeof(textureName), &textureNameArrayA[i]); //dest,size,source: WCHAR
		#else
			strcpy_s(texturename[i].fileName, sizeof(textureName), obj3d.textureNameArray[i].c_str());  //dest,size,source: CHAR
		#endif
    }

	//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(textureName): %d\n"), sizeof(textureName));
	FIN_WRITE((char*) &texturename[0], M3D.texturenameCount * sizeof(textureName));

    fin.close();
    SAFE_DELETE_ARRAY(texturename);

    return true;
}
#pragma warning( pop )


} //namespace DirectX 

