// --------------------------------------------------------------------------------------------
// Filename: commonfunctionsInterfaces.h
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
// Original Code updated by: Pedro Borges
// --------------------------------------------------------------------------------------------

#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "platform.h"
#include "standard_platform.h"

#include <string>
#include <d3d11.h>

namespace DirectX {
	class DXmodelClass;
	class DX11Class;
}
enum SHADER_TYPE;

COMMON_API bool Startauxcommonfunctions(UINT this_level, HWND hWnd);
COMMON_API void Shutdownauxcommonfunctions(void);
COMMON_API std::string CleanFilePath(const std::string& input);
COMMON_API HRESULT extLoadBinfromMEM(DirectX::DX11Class* dx11class, ID3D11Device* pDevice,
	STRING filename, ID3D11ShaderResourceView** ppShaderResourceView);
COMMON_API bool extLoadW3DfromMEM(void* pContext, DirectX::DXmodelClass* dxmodelClass,
	SHADER_TYPE shader_type, void* g_driver, STRING filename,
	bool castShadow, bool renderShadow, UINT instanceCount);
COMMON_API bool InitPackLib(char* packfilename);
COMMON_API int EncodeIDEA90(char* filename, int whatToDo, void** filebuf, unsigned int* filelen);

#if defined  COMMONFUNCTIONS_DLL_EXPORTS
#include "Texture.h"
COMMON_API Texture* LoadTextureFromPathFBX86(UINT model_type, Graphics& graphics, LPCWSTR& texture); //LEVEL >= 86
#endif

extern COMMON_API unsigned int size_original_files;

// ensure COMMON_API expands to __declspec(dllexport/dllimport) appropriately
typedef void (*PFN_LOADDRIVER)(void* modelPtr, void* driverPtr);

#ifdef __cplusplus
extern "C" {
#endif

	// register callback (engine calls this at startup)
	COMMON_API void RegisterLoadDriverCallback(PFN_LOADDRIVER fn);

	// optional: query if callback is set
	COMMON_API bool IsLoadDriverCallbackRegistered();

#ifdef __cplusplus
}
#endif

//#if defined COMMONFUNCTIONS_DLL_EXPORTS
namespace WOMA {
	extern COMMON_API Settings* AppSettings;
}
extern COMMON_API UINT CLASS_LOAD_N;
extern COMMON_API UINT CLASS_DELETE_N;
extern COMMON_API bool threadLoadMeshAlive;
extern COMMON_API HANDLE threadLoadMeshHandle;
extern COMMON_API unsigned long threadLoadMeshId;
extern COMMON_API int numZipItems;
extern COMMON_API int zipIndx;
extern COMMON_API UINT packCounter, totalPackCounter;
extern COMMON_API int Command;
//#endif