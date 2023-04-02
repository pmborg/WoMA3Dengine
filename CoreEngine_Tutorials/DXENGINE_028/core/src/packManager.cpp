// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: packManager.cpp
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
// Extract used packs to: "C:\Users\<user>\AppData\Local\Temp\<temp dir>"
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567891;

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

#include "stateMachine.h"
#include "systemManager.h" // isXP & isWow64
#include "packManager.h"
#include "OSmain_dir.h"
#include "winSystemClass.h"

#include "idea.h"
extern int EncodeIDEA(char* filename, int whatTOdo);

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
     
	_tcscpy(DirPath,sPath);
	_tcscat(DirPath, TEXT("\\*")); // searching all files
	_tcscpy(FileName,sPath);
	_tcscat(FileName, TEXT("\\"));
     
	// find the first file
	hFind = FindFirstFile(DirPath,&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE) return FALSE;
	_tcscpy(DirPath,FileName);
     
	bool bSearch = true;
	while(bSearch) 
	{ // until we find an entry
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(WOMA::IsDots(FindFileData.cFileName)) continue;
			_tcscat(FileName,FindFileData.cFileName);
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{
				// we have found a directory, recurse:
				if(!PackDirectory(hz, FileName)) 
				{
					FindClose(hFind);
					return FALSE; // directory couldn't be deleted
				}

				// empty directory:
				_tcscpy(FileName,DirPath);
			} else {

				_tprintf(TEXT("%s\n"), FileName);	// ACTION!
				ZipAdd(hz,FileName, FileName);		// id, ZIP DEST, Source

				_tcscpy(FileName,DirPath);
			}
		} else {
			// no more files there
			if(GetLastError() == ERROR_NO_MORE_FILES)
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

void PackDir(STRING dir, STRING packName)
{
	int res = _tchdir(dir.c_str()); //_chdir
	DeleteFile(packName.c_str());
	HZIP hz = CreateZip(packName.c_str(), 0);
	bool b = PackDirectory(hz, TEXT("engine"));			// Pack engine directory --> engine.pack (zip)
	IF_NOT_THROW_EXCEPTION(b);
	CloseZip(hz);
}

