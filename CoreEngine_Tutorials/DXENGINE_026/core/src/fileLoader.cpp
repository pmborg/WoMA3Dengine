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
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#include "fileLoader.h"
#include "OSmain_dir.h"
#pragma warning( disable : 6386 )

#if DX_ENGINE_LEVEL >= 90
#if defined USE_W3D_LOADER_INSTEAD
bool TryReplaceWithW3D(TCHAR* filename, TCHAR* outPath, size_t outPathSize)
{
    // Only apply for .obj files
    TCHAR* ext = _tcsrchr(filename, '.');
    if (!ext || _tcsicmp(ext, TEXT(".obj")) != 0)
        return false;

    // Replace .obj with .W3D
    TCHAR w3dPath[MAX_STR_LEN * 2];
    _tcscpy_s(w3dPath, filename);
    TCHAR* lastDot = _tcsrchr(w3dPath, '.');
    if (lastDot)
        _tcscpy_s(lastDot, MAX_STR_LEN, TEXT(".W3D"));
    else
        return false;

    // Check if the .W3D file exists
    FILE* fp = _tfopen(w3dPath, TEXT("rb"));
    if (fp) {
        fclose(fp);
        _tcscpy_s(outPath, outPathSize, w3dPath);
        return true;
    }

    return false;
}
#endif

#if defined USE_PNG_LOADER_INSTEAD
bool TryReplaceWithDDS(TCHAR* filename, TCHAR* outPath, size_t outPathSize)
{
    // Check the current extension
    TCHAR* ext = _tcsrchr(filename, '.');
    if (!ext) return false;

    // Allowed extensions (case-insensitive)
    const TCHAR* allowed[] = {
        TEXT(".bmp"), TEXT(".BMP"),
        //TEXT(".jpg"), TEXT(".JPG"),
        //TEXT(".png"), TEXT(".PNG"),
        TEXT(".tif"), TEXT(".TIF"),
        nullptr
    };

    bool allowedExt = false;
    for (int i = 0; allowed[i]; ++i)
    {
        if (_tcsicmp(ext, allowed[i]) == 0) {
            allowedExt = true;
            break;
        }
    }
    if (!allowedExt) return false;

    // Replace extension with .dds
    TCHAR ddsPath[MAX_STR_LEN * 2];
    _tcscpy_s(ddsPath, filename);
    TCHAR* lastDot = _tcsrchr(ddsPath, '.');
    if (lastDot)
        _tcscpy_s(lastDot, MAX_STR_LEN, TEXT(".dds"));
    else
        return false;

    // Check if the .dds file exists
    FILE* fp = _tfopen(ddsPath, TEXT("rb"));
    if (fp) {
        fclose(fp);
        _tcscpy_s(outPath, outPathSize, ddsPath);
        return true;
    }

    return false;
}
#endif
#endif

namespace WOMA
{
	STRING lastfile;
	STRING file;

#if DX_ENGINE_LEVEL < 90
#if !defined UNICODE && defined WINDOWS_PLATFORM
	WCHAR* LoadFileW(WCHAR* filename) // Load Unicode filename in a Multi-byte code build.
	{
		static WCHAR wfilename[MAX_STR_LEN] = { 0 };

		TCHAR file[MAX_STR_LEN] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, filename, -1, file, MAX_STR_LEN, NULL, NULL);
		TCHAR* cfile = LoadFile(file, true);
		MultiByteToWideChar(CP_ACP, 0, cfile, -1, wfilename, MAX_STR_LEN);

		return wfilename;
	}
#endif

#else

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
        TCHAR* cfile = LoadFile(file, true);

        // Convert back to WCHAR
        MultiByteToWideChar(CP_ACP, 0, cfile, -1, wfilename, MAX_STR_LEN);
        return wfilename;
    }
#endif

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
			if (shader) {
				StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("./"), filename);
            }
            else {
				StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("../"), filename);
            }
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
			if (shader) {
				strcpy_s(file_, sizeof(file_), filename);
            } else {
				StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("../"), filename);
            }
    #endif
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

#if DX_ENGINE_LEVEL >= 90
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
#endif
	}
}
