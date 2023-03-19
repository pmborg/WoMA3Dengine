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
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

#if defined WINDOWS_PLATFORM
#include "stateMachine.h"
#include "systemManager.h" // isXP & isWow64
#include "packManager.h"
#include "OSmain_dir.h"
#include "winSystemClass.h"

#if defined USE_IDEA_PACK
#include "idea.h"
extern int EncodeIDEA(char* filename, int whatTOdo);
#endif

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

#if defined RELEASE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "unzip.h"
#include "zip.h"

// --------------------------------------------------------------------------------------------
// Globals:
// --------------------------------------------------------------------------------------------
int numZipItems=0;
int zipIndx=0;

#define PACK_COUNTER_FILE "counter.dat"
UINT packCounter = 0, totalPackCounter = 0;

// wildcmp SOURCE: Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
//
int wildcmp(const char *wild, const char *string) 
{
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

bool InitPackLib(char* packfilename)				// Need to be CHAR!
{	
	char File_Pak[256] = {0};						// Need to be CHAR!

	CHAR A_APPDATA[MAX_STR_LEN]={ 0 };			
	wtoa(A_APPDATA, WOMA::APPDATA, MAX_STR_LEN);

#ifdef RELEASE
	strcpy (File_Pak, A_APPDATA);					// Need to be CHAR!
#endif
	strcat (File_Pak, packfilename);				// Need to be CHAR!

	// Uncode Pack: Make sure that you RUN LEVEL 16 First!
	int res = EncodeIDEA(File_Pak, DECODE);			// engine.pck (encoded) --> engine.pack (decoded)

	char *File_Pack = strrchr(File_Pak, '.');		// Need to be CHAR!
	if (File_Pack) {
		*File_Pack = 0;
		strcat (File_Pak, ".pack.zip");
	}

	TCHAR Wbuffer[MAX_STR_LEN] = {0};	
	atow(Wbuffer, File_Pak, MAX_STR_LEN);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HZIP hz = OpenZip(Wbuffer, 0);
	if (hz) 
	{
		ZIPENTRY ze; 
		GetZipItem(hz,-1,&ze); 
		numZipItems=ze.index;	// -1 gives overall information about the zipfile
		SetUnzipBaseDir ( hz, WOMA::APPDATA );

		for (zipIndx=0; zipIndx<numZipItems; zipIndx++)
		{ZIPENTRY ze; 
			GetZipItem(hz,zipIndx,&ze);			// fetch individual details

			CHAR psz[MAX_STR_LEN] = {0};
			wtoa(psz, ze.name, 100);
			#ifdef ZIP_MEM
				if (wildcmp("*.M3D", psz) || 
					wildcmp("*.wav", psz) ||
					wildcmp("*.mp3", psz) ||
					wildcmp("*.md5*", psz) ||
					wildcmp("*.txt", psz) ||
					wildcmp("*.obj", psz) ||
					wildcmp("*.mtl", psz) ||
					wildcmp("*.bmp", psz)) 
			#endif
			{
				UnzipItem(hz, zipIndx, ze.name);    // e.g. the item's name.
			}

			packCounter++;

			#ifdef _DEBUG
				if ((zipIndx % 10) == 0)				// Update every 10 files... (to avoid a massive update!)
                    RedrawWindow(SystemHandle->m_hWnd, NULL, NULL, RDW_UPDATENOW|RDW_INVALIDATE);// Invoke: Window PAINT
			#endif
		}
	
		CloseZip(hz);
		if (numZipItems == 0)
			{WomaFatalExceptionW(TEXT("Could not open the pack file"));return false;}

		#ifdef zero
        //RedrawWindow(g_hwnd, NULL, NULL, RDW_UPDATENOW|RDW_INVALIDATE/*|RDW_ERASE*/);// Invoke: Window PAINT
		#endif

	} else {
		//Run Launcher, before run the app. once data files are missing:
		static TCHAR str[MAX_STR_LEN];
		StringCchPrintf(str, sizeof(str), TEXT("File: %s, please Run \"Woma Launcher\" first, in order to run Woma."), packfilename);
		WOMA::WomaMessageBox(str, TEXT("File Pack not Found!"), MB_OK); 
		return false;
	}

	DeleteFile (Wbuffer);

	return true;
}

STRING REPORT_FILE;

bool InitPackLibs() 
{
	// Read the Total Number of Files that will be Uncompressed:
    std::fstream fs;
	REPORT_FILE = WOMA::APPDATA; REPORT_FILE.append(TEXT(PACK_COUNTER_FILE));
	fs.open(REPORT_FILE.c_str(), std::fstream::in);
    fs >> totalPackCounter;
	fs.close();

	IF_NOT_RETURN_FALSE (InitPackLib("windows.pck"));	// Need to be CHAR!

	return true;
}

bool StartPackLibs() 
{
#if defined USE_MAIN_THREAD
	threadLoadPacksAlive = true;
#endif
	WOMA_LOGManager_DebugMSGAUTO (TEXT("CreateThread: Initialize LoadPacks ThreadFunction\n"));

	#if DX_ENGINE_LEVEL >= 20
		IF_NOT_RETURN_FALSE (InitPackLib("engine.pck"));	// Need to be CHAR!
	#endif

  #if TUTORIAL_PRE_CHAP >= 11
    IF_NOT_RETURN_FALSE(InitPackLib(L"Terrain.pack"));

    #ifdef SCENE_COMPOUND
	IF_NOT_RETURN_FALSE(InitPackLib(L"ALLData.pack"));
    #endif
  #endif

  #if TUTORIAL_PRE_CHAP >= 30
	IF_NOT_RETURN_FALSE(InitPackLib(L"Clouds.pack"));
  #endif

  #if TUTORIAL_PRE_CHAP >= 55
	IF_NOT_RETURN_FALSE(InitPackLib(L"Mesh.pack"));
  #endif

    // We need to repaint at this point:
  #ifdef _DEBUG
    RedrawWindow(SystemHandle->m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);  // Invoke: Window PAINT before end.
  #endif

	// Write the Total Number of Files that were Uncompressed:
	{
    std::fstream fs;
	fs.open(REPORT_FILE.c_str(), std::fstream::out);
    fs << packCounter;
	fs.close();
	}

#if defined USE_MAIN_THREAD
	threadLoadPacksAlive = false;
	WOMA::num_running_THREADS--;
#endif

	return true;
}

#endif

#if CORE_ENGINE_LEVEL >= 6 // ZIP PACK: engine dir
void PackDir(STRING dir, STRING packName)
{
	int res = _tchdir(dir.c_str()); //_chdir
	DeleteFile(packName.c_str());
	HZIP hz = CreateZip(packName.c_str(), 0);
	bool b = PackDirectory(hz, TEXT("engine"));			// Pack engine directory --> engine.pack (zip)
	IF_NOT_THROW_EXCEPTION(b);
	CloseZip(hz);
}
#endif

#endif//EOF