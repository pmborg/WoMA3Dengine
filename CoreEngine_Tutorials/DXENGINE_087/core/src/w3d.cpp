// --------------------------------------------------------------------------------------------
// Filename: w3d.cpp
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
//WomaIntegrityCheck = 1234525256;

#include "platform.h"
#include "OSengine.h"		// Get [SystemHandle] Pointer to System Class & define WomaSYSTEM for: WINDOWS, LINUX & ANDROID
#include "OSmain_dir.h"
#include "DXmodelClass.h"
#include "fileLoader.h"
#include "w3d.h"
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
	// "W3D" MATERIAL FORMAT:
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
	bool DXmodelClass::SaveW3D(STRING filename, void* indices, void* vertices, UINT sizeofMODELvertex)
	{
		if (WOMA::game_state == GAME_STOP)
			return false;

		MyObject3D W3D;
		//------------------------------------------------------------------------------------------------------------------
		if (ModelShaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW ||
			ModelShaderType == SHADER_TEXTURE_LIGHT_INSTANCED ||
			ModelShaderType == SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED  ||
            ModelShaderType == SHADER_TEXTURE_GS_INSTANCED)
			ModelShaderType = SHADER_TEXTURE_LIGHT;

		if (ModelShaderType == SHADER_NORMAL_BUMP_INSTANCED)
			ModelShaderType = SHADER_NORMAL_BUMP;

		float version = 1.0f + (float)(ModelShaderType-1) / 10;
		if (ModelShaderType >= SHADER_TEXTURE_FONT)
			version = version - 0.1f;

		StringCchPrintfA(W3D.version, sizeof(W3D.version), "W3D v%.1f", version);
		//WOMA_LOGManager_DebugMSG("W3D.version: %s\n", W3D.version);

		W3D.min = minVertex;
		W3D.max = maxVertex;

		W3D.verticesCount = m_vertexCount;	// TOTAL: Vertice ==> Equal to: "m_vertexCount"
		W3D.size_verticesCount = sizeofMODELvertex;
		W3D.indicesCount = m_indexCount;	// TOTAL: Indice  ==> Equal to: "m_indexCount"
		W3D.size_indicesCount = sizeof(UINT);

		W3D.materialCount = obj3d.material.size();
		W3D.size_materialCount = sizeof(SurfaceMaterial);
		W3D.texturenameCount = obj3d.textureNameArray.size();				// Related with N. of "Texture" Resources
		W3D.size_texturenameCount = sizeof(textureName);

		W3D.meshSubsetIndexStartCount = obj3d.meshSubsetIndexStart.size();// meshSubsetIndexStart = subsetMaterialArray + 1
		W3D.size_meshSubsetIndexStartCount = sizeof(int);
		W3D.subsetMaterialArrayCount = obj3d.subsetMaterialArray.size();
		W3D.size_subsetMaterialArrayCount = sizeof(int);
		// Check: m_indexCount (too big: to be used in real time rendering...)
		if (m_indexCount > 65535)
		{
			WOMA_LOGManager_DebugMSGAUTO(TEXT("FATAL ERROR SaveW3D: indicesCount > 65535: %d\n"), m_indexCount);
			MessageBox(NULL, filename.c_str(), TEXT("SaveW3D: indicesCount > 65535 (i.e. This object is too Big!)"), MB_OK); return false;
		}
		// Check: meshSubsetIndexStart (use 3d studio max to optimize mesh!)
		if (obj3d.meshSubsetIndexStart.size() > 255)
		{
			WOMA_LOGManager_DebugMSGAUTO(TEXT("WARNING! SaveW3D: meshSubsetIndexStartCount > 255: %d\n"), obj3d.meshSubsetIndexStart.size());
		}
		// Check: meshSubsetIndexStart (too complex: to be used in real time rendering...)
		if (obj3d.meshSubsetIndexStart.size() > 65535)
		{
			WOMA_LOGManager_DebugMSGAUTO(TEXT("ERROR SaveW3D: meshSubsetIndexStartCount > 65535: %d\n"), obj3d.meshSubsetIndexStart.size());
			MessageBox(NULL, filename.c_str(), TEXT("SaveW3D: meshSubsetIndexStartCount > 65535"), MB_OK); return false;
		}
		//Check: subsetMaterialArray (use too many materials: )
		if (obj3d.subsetMaterialArray.size() > 65535)
		{
			WOMA_LOGManager_DebugMSGAUTO(TEXT("ERROR SaveW3D: subsetMaterialArrayCount > 65535: %d\n"), obj3d.subsetMaterialArray.size());
			MessageBox(NULL, filename.c_str(), TEXT("SaveW3D: subsetMaterialArrayCount > 65535"), MB_OK); return false;
		}

		// OPEN: Target file to write

		filename.replace(filename.size() - 3, 3, TEXT("W3D"));

		std::ofstream fin(filename, std::ios::out | std::ios::binary); // Dont use: OFSTREAM

		// DUMP: HEADER DATA INDX
		// --------------------------------------------------------------------------------------------------------------------------------

		//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(W3D): %d\n"), sizeof(W3D));
		FIN_WRITE((char*)&W3D, sizeof(W3D));

		// DUMP: vertices
		// --------------------------------------------------------------------------------------------------------------------------------
		//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeofMODELvertex: %d\n"), sizeofMODELvertex);
		FIN_WRITE((char*)vertices, W3D.verticesCount * sizeofMODELvertex);

		// DUMP: indices
		// --------------------------------------------------------------------------------------------------------------------------------
		// Scale down all "int" to "short"
//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(UINT): %d\n"), sizeof(UINT));
		FIN_WRITE((char*)&obj3d.indices32[0], W3D.indicesCount * sizeof(UINT));

		// DUMP: SubsetIndexStart
		// --------------------------------------------------------------------------------------------------------------------------------
		//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(int): %d\n"), sizeof(int));
		FIN_WRITE((char*)&obj3d.meshSubsetIndexStart[0], W3D.meshSubsetIndexStartCount * sizeof(int));

		// DUMP: subsetMaterialArray
		// --------------------------------------------------------------------------------------------------------------------------------
		//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(int): %d\n"), sizeof(int));
		FIN_WRITE((char*)&obj3d.subsetMaterialArray[0], W3D.subsetMaterialArrayCount * sizeof(int));

		// DUMP: Materials
		// --------------------------------------------------------------------------------------------------------------------------------
		//WOMA_LOGManager_DebugMSGAUTO(TEXT("sizeof(SurfaceMaterial): %d\n"), sizeof(SurfaceMaterial));
		FIN_WRITE((char*)&obj3d.material[0], W3D.materialCount * sizeof(SurfaceMaterial));

		// DUMP: Texture Names
		// --------------------------------------------------------------------------------------------------------------------------------
		textureName* texturename = NEW textureName[W3D.texturenameCount];
		ZeroMemory(texturename, W3D.texturenameCount * sizeof(textureName));

		for (UINT i = 0; i < W3D.texturenameCount; i++)
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
		FIN_WRITE((char*)&texturename[0], W3D.texturenameCount * sizeof(textureName));

		fin.close();
		SAFE_DELETE_ARRAY(texturename);

		return true;
	}
#pragma warning( pop )


#if defined LOADW3D //ENGINE_LEVEL >= 50
	// --------------------------------------------------------------------------------------------
	bool DXmodelClass::LoadW3D(SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow, bool renderShadow, UINT instanceCount)
	// --------------------------------------------------------------------------------------------
	{
		LOADDRIVER(g_driver);

#ifdef  FORCE_COMPOUNDLOAD_OBJ 
		//Force to load OBJ original version, instead our W3D

		filename.replace(filename.size() - 3, 3, TEXT("OBJ"));
		return LoadOBJ(g_driver, filename, castShadow, renderShadow);
#else

		MyObject3D W3D = MyObject3D();
		fstream obj3dfile;

		WOMA_LOGManager_DebugMSG(TEXT("W3D Loading: %s with shader: [%d]\n"), (TCHAR*)(filename + TEXT(" ")).c_str(), shader_type);
		obj3dfile.open(WOMA::LoadFile((TCHAR*)filename.c_str()), fstream::in | fstream::binary);
		if (!obj3dfile)
		{
			MessageBox(NULL, filename.c_str(), TEXT("W3D file not found"), MB_OK); return false;
		}

		// HEADER
		//obj3d.read(W3D.version, sizeof(W3D.version));			//[0]  8:	 HEADER SIGN
		//	--------------------------------------------------------------------------------------------------------------------------------

		// READ: HEADER DATA INDX
		// --------------------------------------------------------------------------------------------------------------------------------
		obj3dfile.read((char*)&W3D, sizeof(W3D));

		minVertex = W3D.min;
		maxVertex = W3D.max;

		m_vertexCount = W3D.verticesCount;	// TOTAL: Vertice ==> Equal to: "m_vertexCount"
		m_indexCount = W3D.indicesCount;	// TOTAL: Indice  ==> Equal to: "m_indexCount"

		// READ: VERTEX DATA:
		// --------------------------------------------------------------------------------------------------------------------------------
		if (strcmp(W3D.version, "W3D v1.0") == 0)	//21
		{
			modelColorVertex_.resize(m_vertexCount);
			obj3dfile.read((char*)&modelColorVertex_[0], W3D.verticesCount * W3D.size_verticesCount /*sizeof(ModelColorVertexType)*/);
			modelColorVertex = &modelColorVertex_;
		}
		else if ((strcmp(W3D.version, "W3D v1.1") == 0) || (strcmp(W3D.version, "W3D v2.9") == 0))	//22 || FIRE
		{
			modelTextureVertex_.resize(m_vertexCount);
			obj3dfile.read((char*)&modelTextureVertex_[0], W3D.verticesCount * W3D.size_verticesCount /*sizeof(ModelTextureVertexType)*/);
			modelTextureVertex = &modelTextureVertex_;
		}
		else if (strcmp(W3D.version, "W3D v1.2") == 0)	//23
		{
			modelTextureLightVertex_.resize(m_vertexCount);
			obj3dfile.read((char*)&modelTextureLightVertex_[0], W3D.verticesCount * W3D.size_verticesCount /*sizeof(ModelTextureLightVertexType)*/);
			modelTextureLightVertex = &modelTextureLightVertex_;
		}
		else if (strcmp(W3D.version, "W3D v1.3") == 0)	//35
		{
			modelNormalBumpVertex_.resize(m_vertexCount);
			obj3dfile.read((char*)&modelNormalBumpVertex_[0], W3D.verticesCount * W3D.size_verticesCount /*sizeof(ModelNormalBumpVertexType)*/);
			modelNormalBumpVertex = &modelNormalBumpVertex_;
		}

		obj3d.indices32.resize(m_indexCount);
		obj3dfile.read((char*)&obj3d.indices32[0], W3D.indicesCount * W3D.size_indicesCount /*sizeof(UINT)*/);

		// READ: meshSubsetIndexStart
		obj3d.meshSubsetIndexStart.resize(W3D.meshSubsetIndexStartCount);
		obj3dfile.read((char*)&obj3d.meshSubsetIndexStart[0], W3D.meshSubsetIndexStartCount * W3D.size_meshSubsetIndexStartCount /*sizeof(int)*/);

		// READ: subsetMaterialArray
		obj3d.meshSubsets = W3D.meshSubsetIndexStartCount - 1;
		obj3d.subsetMaterialArray.resize(obj3d.meshSubsets);
		obj3dfile.read((char*)&obj3d.subsetMaterialArray[0], obj3d.meshSubsets * W3D.size_subsetMaterialArrayCount /*sizeof(int)*/);

		if (W3D.materialCount)
		{
			obj3d.material.resize(W3D.materialCount);
			obj3dfile.read((char*)&obj3d.material[0], W3D.materialCount * W3D.size_materialCount /*sizeof(SurfaceMaterial)*/);

			for (UINT i = 0; i < W3D.materialCount; i++)
			{
#if defined UNICODE
				TCHAR Wbuffer[MAX_STR_LEN] = { 0 };	atow(Wbuffer, material[i].matName, MAX_STR_LEN);
				meshMaterials.push_back(Wbuffer);
#else
				obj3d.meshMaterials.push_back(obj3d.material[i].matName);
#endif

			}
		}

		if (W3D.texturenameCount)
		{
			textureName* textureNameArray_ = NEW textureName[W3D.texturenameCount];
			obj3dfile.read((char*)&textureNameArray_[0], W3D.texturenameCount * W3D.size_texturenameCount /*sizeof(textureName)*/);
			for (UINT i = 0; i < W3D.texturenameCount; i++)
			{
#if defined UNICODE
				TCHAR filename_[MAX_STR_LEN] = { 0 }; atow(filename_, textureNameArray_[i].fileName, MAX_STR_LEN);
				textureNameArray.push_back(filename_);								//Write on DX Driver as WCHAR
#else
				obj3d.textureNameArray.push_back(textureNameArray_[i].fileName);	//Write on DX Driver as CHAR
#endif
			}
			SAFE_DELETE_ARRAY(textureNameArray_);
		}

		// Get Path to Mat. Lib.
		// ---------------------
		if (W3D.texturenameCount > 0)
		{
			for (UINT i = 0; i < W3D.texturenameCount; i++)
			{
				STRING textureFilename;
				// Get full pathname for this texture:
				STRING fileNamePath = (TCHAR*)obj3d.textureNameArray[i].c_str();
				STRING pathtoengine = TEXT("../");
				if (fileNamePath.substr(0, 3) != pathtoengine)
					textureFilename = WOMA::LoadFile((TCHAR*)fileNamePath.c_str());
				else
					textureFilename = (TCHAR*)fileNamePath.c_str();

#if !defined(STANDALONE)
				ID3D11ShaderResourceView* tempMeshSRV = NULL;
				HRESULT hr = S_OK;
				if (textureFilename.find(TEXT("../none")) != 0 /* && textureFilename.find(TEXT("../"))*/) //dont load on special cases (like billboards)
				{
				LOADTEXTURE(textureFilename.c_str(), tempMeshSRV);
				//DX11
				if (SUCCEEDED(hr))
					meshSRV11.push_back(tempMeshSRV);		                //[4]
				else
					return false;
				}
				else
					meshSRV11.push_back(NULL); //on special cases (like billboards)
#endif
			}
		}

		obj3dfile.close();

#if !defined(STANDALONE)
		if (strcmp(W3D.version, "W3D v1.0") == 0)	//21 COLOR
			LoadColor((TCHAR*)filename.c_str(), g_driver, shader_type, modelColorVertex, &obj3d.indices32, instanceCount);
		else
		if (strcmp(W3D.version, "W3D v1.1") == 0)	//22 TEXTURE
			LoadTexture((TCHAR*)filename.c_str(), g_driver, shader_type, &obj3d.textureNameArray, modelTextureVertex, &obj3d.indices32, instanceCount);
		else
		if (strcmp(W3D.version, "W3D v1.2") == 0)	//23 LIGHT
			LoadLight((TCHAR*)filename.c_str(), g_driver, shader_type, &obj3d.textureNameArray, modelTextureLightVertex, &obj3d.indices32, instanceCount);
		else
		if (strcmp(W3D.version, "W3D v1.3") == 0)	//35
			LoadBump((TCHAR*)filename.c_str(), g_driver, shader_type, &obj3d.textureNameArray, modelNormalBumpVertex, &obj3d.indices32, instanceCount);
		else
		if (strcmp(W3D.version, "W3D v2.9") == 0)	// FIRE
			LoadTexture((TCHAR*)filename.c_str(), g_driver, SHADER_FIRE, &obj3d.textureNameArray, modelTextureVertex, &obj3d.indices32, instanceCount);
		else
		{			
			return false;
		}
		// "W3D v1.4"	//36
#endif

		return true;
#endif
	}
#endif

} //namespace DirectX 

