// --------------------------------------------------------------------------------------------
// Filename: fileLoader.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: Get full filename inside directory or woma.pck files depending DEBUG or RELEASE build
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "OSengine.h"
#include "fileLoader.h"
#include "OSmain_dir.h"
#pragma warning( disable : 6386 )

namespace WOMA
{
	STRING lastfile;
	STRING file;


#if !defined UNICODE && defined WINDOWS_PLATFORM
	WCHAR* LoadFileW(WCHAR* filename)
	{
		static WCHAR wfilename[MAX_STR_LEN] = { 0 };
		TCHAR file[MAX_STR_LEN] = { 0 };

		// Convert from WCHAR to TCHAR (ANSI)
		WideCharToMultiByte(CP_ACP, 0, filename, -1, file, MAX_STR_LEN, NULL, NULL);
	#if defined USE_PNG_LOADER_INSTEAD
		// Try to replace with .dds if it exists
		TCHAR finalddsPath[MAX_STR_LEN] = { 0 };
		if (TryReplaceWithDDS(file, finalddsPath, MAX_STR_LEN))
			_tcscpy_s(file, finalddsPath);

		// Try to replace with .dds if it exists
		TCHAR finalobjPath[MAX_STR_LEN] = { 0 };
		if (TryReplaceWithW3D(file, finalobjPath, MAX_STR_LEN))
			_tcscpy_s(file, finalobjPath);
	#endif
		// Use standard LoadFile to resolve full path and base folder
		TCHAR* cfile = WOMA::LoadFile(file, true);

		// Convert back to WCHAR
		MultiByteToWideChar(CP_ACP, 0, cfile, -1, wfilename, MAX_STR_LEN);
		return wfilename;
	}
#endif


	TCHAR* LoadFile(TCHAR* filename, bool shader)
	{
		static TCHAR file_[MAX_STR_LEN * 2];
		ZeroMemory(&file_, sizeof(file_));

#if !defined ANDROID_PLATFORM
#ifdef RELEASE
		if (shader) {
			file = WOMA::APP_PROJECT_NAME;
			file.append(TEXT("/"));
		}
		else {
			file = WOMA::APPDATA; // WOMA::womaTempPATH;
		}
		file.append(filename);
		lastfile = file;
		return (TCHAR*)file.c_str();
#else
		if (filename[0] != '.') {
			StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("./"), filename);
		}
		else {
#if defined UNICODE
#else
#endif
		}

		file = file_;
		lastfile = file;

#if defined WINDOWS_PLATFORM && defined _DEBUG
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			womalog((CHAR*)"[FILE LOAD] %s\n", file_);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE);
		}
#endif

		#if defined USE_PNG_LOADER_INSTEAD
        // Try to use .dds if it exists
        TCHAR finalddsPath[MAX_STR_LEN * 2];
        if (TryReplaceWithDDS(file_, finalddsPath, sizeof(finalddsPath)))
            _tcscpy_s(file_, sizeof(file_), finalddsPath);
		#endif
		#if defined USE_W3D_LOADER_INSTEAD
        // Try to use .W3D if it exists
        TCHAR finalw3dPath[MAX_STR_LEN * 2];
        if (TryReplaceWithDDS(file_, finalw3dPath, sizeof(finalw3dPath)))
            _tcscpy_s(file_, sizeof(file_), finalw3dPath);
		#endif
#endif

		return (TCHAR*)&file_;
#else

	if (shader && WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		UINT shaderLevel = RENDER_PAGE;
		if (shaderLevel > 55)
			shaderLevel = 55;

		StringCchPrintf(file_, sizeof(file_), TEXT("VK/DXEngine_0%d/%s"), shaderLevel, filename);

		return file_;
	}

	return filename;
#endif
	}


	CHAR* LoadFileA(TCHAR* filename, bool shader)
	{
#if defined UNICODE && defined WINDOWS_PLATFORM
		static CHAR result[MAX_STR_LEN * 2];
		ZeroMemory(&result, sizeof(result));

		wtoa(result, LoadFile(filename, shader), MAX_STR_LEN * 2);
		return result;
#else
		return LoadFile(filename, shader);
#endif
	}


}
