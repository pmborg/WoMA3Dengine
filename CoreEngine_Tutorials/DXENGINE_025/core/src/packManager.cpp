// --------------------------------------------------------------------------------------------
// Filename: packManager.cpp
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// -------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// Extract used packs to: "C:\Users\<user>\AppData\Local\Temp\<temp dir>"
//
// ********************************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

#include "stateMachine.h"
#include "systemManager.h" // isXP & isWow64
#include "packManager.h"
#include "OSmain_dir.h"
#include "winSystemClass.h"
//#include "SystemPlatform.h"		// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID

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

