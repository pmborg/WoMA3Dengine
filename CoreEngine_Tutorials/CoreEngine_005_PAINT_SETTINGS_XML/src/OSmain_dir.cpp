// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: OsDirectories.cpp
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
// PURPOSE: 
//  - Log Current PATH
//  - Check if PATH if NETWORK PATH (Release)
//  - Init: PROGRAM_FILES "C:\ProgramFiles(x86)\Pmborg\Woma2017\"
//  - Init: APPDATA "C:\Users\"username"\AppData\Local\Pmborg\Woma2017\"
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "OSmain_dir.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <map>

#ifndef _MSC_VER
#include <unistd.h>
#endif

namespace WOMA
{
	// State Vars:
	//---------------------------------------------------
	int					game_state=0;
	int					previous_game_state=0;

	// Directory Vars:
	//---------------------------------------------------
	STRING HOME;
	TCHAR PUBLIC_DOCUMENTS0[MAX_STR_LEN];
	TCHAR PUBLIC_DOCUMENTS1[MAX_STR_LEN];
	TCHAR PUBLIC_DOCUMENTS[MAX_STR_LEN];
	TCHAR PROGRAM_FILES[MAX_STR_LEN];
	TCHAR APPDATA1[MAX_STR_LEN];
	TCHAR APPDATA[MAX_STR_LEN];
	TCHAR cCurrentPath[FILENAME_MAX]; //Use now: APPDATA
	STRING womaTempPATH;

	bool fileExists(STRING Filename)
	{
		return _taccess(Filename.c_str(), 0) == 0;
	}

	bool dirExists(STRING& dirName_in)
	{
		DWORD ftyp = GetFileAttributes(dirName_in.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;  //something is wrong with your path!

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;   // this is a directory!

		return false;    // this is not a directory!
	}

	BOOL IsDots(const TCHAR* str) {
		if (_tcscmp(str, TEXT(".")) && _tcscmp(str, TEXT(".."))) return FALSE;
		return TRUE;
	}

	BOOL DeleteDirectory(const TCHAR* sPath)
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
				if (IsDots(FindFileData.cFileName)) continue;
				_tcscat(FileName, FindFileData.cFileName);
				if ((FindFileData.dwFileAttributes &
					FILE_ATTRIBUTE_DIRECTORY))
				{
					// we have found a directory, recurse
					if (!DeleteDirectory(FileName))
					{
						FindClose(hFind);
						return FALSE; // directory couldn't be deleted
					}
					// remove the empty directory
					RemoveDirectory(FileName);
					_tcscpy(FileName, DirPath);
				}
				else {
					if (FindFileData.dwFileAttributes &
						FILE_ATTRIBUTE_READONLY)
						// change read-only file mode
						_tchmod(FileName, _S_IWRITE);
					if (!DeleteFile(FileName)) { // delete the file
						FindClose(hFind);
						return FALSE;
					}
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

		return RemoveDirectory(sPath); // remove the empty directory
	}


	//-------------------------------------------------------------------------------------------
	TCHAR* getCurrentDir()
	//-------------------------------------------------------------------------------------------
	{
		if (!GetCurrentDirectory(MAX_STR_LEN, cCurrentPath))
			return (TCHAR*)TEXT("");

		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("CurrentPath: %s\n"), cCurrentPath);

		return cCurrentPath;
	}

	//-------------------------------------------------------------------------------------------
	TCHAR* getTemp()
		//-------------------------------------------------------------------------------------------
	{
		static TCHAR* strTEMPchar = _tgetenv(TEXT("TEMP"));	// Get our Fullpath to all objects; ("TEMP" + /engine/)

		return strTEMPchar;
	}

	//-------------------------------------------------------------------------------------------
	TCHAR* getHome()
	//-------------------------------------------------------------------------------------------
	{
		static TCHAR* strHOMEchar = _tgetenv(TEXT("USERPROFILE"));

		return strHOMEchar;
	}

	//-------------------------------------------------------------------------------------------
	bool logDirs(BOOL WINXP_FLAG, BOOL bIsWow64)
	//-------------------------------------------------------------------------------------------
	{
		TCHAR currentPath[MAX_STR_LEN] = { 0 };
		GetCurrentDirectory(MAX_STR_LEN, currentPath);
		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Current Path: %s\n"), currentPath);

		// Check if is a network PATH:

		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("PROGRAM_FILES Directory: %s\n"), PROGRAM_FILES);
		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("APPDATA Directory: %s\n"), APPDATA);
		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("TEMP Directory: %s\n"), getTemp());
		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("HOME Directory: %s\n"), getHome());

		return true;
	}

	//-------------------------------------------------------------------------------------------
	bool init_os_main_dirs(TCHAR* currentdir)
	//-------------------------------------------------------------------------------------------
	{

		return true;
	}

	void setup_OSmain_dirs() {
		char path[MAX_PATH] = { 0 };
		// (No Need on DEBUG!)

		// [1] init_os_main_dirs!
		// -------------------------------------------------------------------------------------------
		_tprintf(TEXT("Init: init_os_main_dirs()\n"));
			#if defined UNICODE
				TCHAR WDirbuffer[MAX_STR_LEN] = { 0 };
				atow(WDirbuffer, path, MAX_STR_LEN);
				init_os_main_dirs(WDirbuffer);
			#else
				init_os_main_dirs(path);
			#endif
	}
	BOOL bIsWow64;

	bool is64()
	//------------------------------------------------------------------
	{
	#ifdef X64
		return true;
	#else
		return false;
	#endif
	}

	BOOL isWow64()
	//------------------------------------------------------------------
	{
		typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
		static LPFN_ISWOW64PROCESS fnIsWow64Process = NULL;

		if (fnIsWow64Process) //Did we ran already?
			return bIsWow64;

		//IsWow64Process is not available on all supported versions of Windows!
		//Use GetModuleHandle to get a handle to the DLL that contains the function
		//and GetProcAddress to get a pointer to the function if available.
		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

		if (fnIsWow64Process)
			if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) //Out: We are 32 bits in a OS64bits
				return false;

		return bIsWow64;
	}

	#pragma warning( disable : 4996 ) // Disable warning C4996:
	bool isXP()
	//------------------------------------------------------------------
	{
	#if defined WINDOWS_PLATFORM && _MSC_VER < 1900
		// OS Version check tells us most of what we need to know
		OSVERSIONINFOEX osinfo;
		osinfo.dwOSVersionInfoSize = sizeof(osinfo);
		if (!GetVersionEx((OSVERSIONINFO*)&osinfo))
			return false;

		if (osinfo.dwMajorVersion < 6)
			return true;
		else
	#endif

			return false;
	}

}
