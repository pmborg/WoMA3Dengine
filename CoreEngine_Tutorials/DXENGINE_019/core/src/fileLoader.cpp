// --------------------------------------------------------------------------------------------
// Filename: fileLoader.cpp
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
// PURPOSE: Get full filename inside diretory or woma.pck files depending DEBUG or RELEASE build
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;

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
		WideCharToMultiByte(CP_ACP, 0, filename, -1, file, MAX_STR_LEN, NULL, NULL);
		TCHAR* cfile = LoadFile(file, true);
		MultiByteToWideChar(CP_ACP, 0, cfile, -1, wfilename, MAX_STR_LEN);
		return wfilename;
	}
#endif

	TCHAR* LoadFile(TCHAR* filename, bool shader)
	{
		static TCHAR file_[MAX_STR_LEN * 2];
		ZeroMemory(&file_, sizeof(file));

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
#if CORE_ENGINE_LEVEL >= 8 && !defined RELEASE
			if (shader)
				StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("./"), filename);
			else
				StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("../"), filename);
#else
			StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("./"), filename);
#endif
		}
		else {
#if defined UNICODE
#if CORE_ENGINE_LEVEL >= 8 && defined _DEBUG
			if (shader)
				_tcscpy_s(file_, sizeof(file_), filename);
			else
				StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("../"), filename);
#endif
#else
#if CORE_ENGINE_LEVEL >= 8 && !defined RELEASE
			if (shader)
				strcpy_s(file_, sizeof(file_), filename);
			else
				StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("../"), filename);
#endif
#endif
		}
		file = file_;
		lastfile = file;
#if defined WINDOWS_PLATFORM && _DEBUG
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			WOMA_LOGManager_DebugMSG((CHAR*)"[FILE LOAD] %s\n", file_);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE);
		}
#endif
		return (TCHAR*)&file_;
#endif
	}

}
