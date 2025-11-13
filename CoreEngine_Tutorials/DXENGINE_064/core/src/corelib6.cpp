// --------------------------------------------------------------------------------------------
// Filename: corelib6.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#include "OSengine.h"
#include "unzip.h"
#include "zip.h"

namespace WOMA
{
	extern BOOL IsDots(const TCHAR* str);
}

//
//
BOOL PackDirectory(HZIP hz, const TCHAR* sPath)
{
	HANDLE hFind; // file handle
	WIN32_FIND_DATA FindFileData;

	TCHAR DirPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	_tcscpy(DirPath, sPath);
	_tcscat(DirPath, TEXT("\\*")); // searching all files
	_tcscpy(FileName, sPath);
	_tcscat(FileName, TEXT("\\"));

	// find the first file
	hFind = FindFirstFile(DirPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return FALSE;
	_tcscpy(DirPath, FileName);

	bool bSearch = true;
	while (bSearch)
	{ // until we find an entry
		if (FindNextFile(hFind, &FindFileData))
		{
			if (WOMA::IsDots(FindFileData.cFileName)) continue;
			_tcscat(FileName, FindFileData.cFileName);
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// we have found a directory, recurse:
				if (!PackDirectory(hz, FileName))
				{
					FindClose(hFind);
					return FALSE; // directory couldn't be deleted
				}

				// empty directory:
				_tcscpy(FileName, DirPath);
			}
			else {

				_tprintf(TEXT("%s\n"), FileName);	// ACTION!
				ZipAdd(hz, FileName, FileName);		// id, ZIP DEST, Source
				womalogauto(TEXT("ZipAdd: %s\n"), FileName);
				_tcscpy(FileName, DirPath);
			}
		}
		else {
			// no more files there
			if (GetLastError() == ERROR_NO_MORE_FILES)
				bSearch = false;
			else {
				// some error occurred; close the handle and return FALSE
				FindClose(hFind);
				return FALSE;
			}
		}
	}
	FindClose(hFind); // close the file handle

	return TRUE;
}
