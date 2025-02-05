// --------------------------------------------------------------------------------------------
// Filename: OSmain_dir.cpp
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
// PURPOSE: 
//  - Log Current PATH
//  - Check if PATH if NETWORK PATH (Release)
//  - Init: PROGRAM_FILES "C:\ProgramFiles(x86)\Pmborg\Woma2017\"
//  - Init: APPDATA "C:\Users\"username"\AppData\Local\Pmborg\Woma2017\"
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "platform.h"

#include "OSengine.h" //#include "WinSystemClass.h"

#if defined ANDROID_PLATFORM && !defined NewWomaEngine
#include "AndroidSystemClass.h"
#endif

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

#if !defined WINDOWS_PLATFORM
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>				// define: FILENAME_MAX
#endif

namespace WOMA
{
	// State Vars:
	//---------------------------------------------------
	//int					game_state=0;
	//int					previous_game_state=0;

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

#if defined WINDOWS_PLATFORM
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
#endif

	//-------------------------------------------------------------------------------------------
	TCHAR* getCurrentDir()
		//-------------------------------------------------------------------------------------------
	{
#if defined WINDOWS_PLATFORM
		if (!GetCurrentDirectory(MAX_STR_LEN, cCurrentPath))
			return (TCHAR*)TEXT("");

		//WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("CurrentPath: %s\n"), cCurrentPath);
#else // Linux / Android
		char* CurrentPath;

		CurrentPath = getcwd(NULL, 0); // or _getcwd
		if (CurrentPath == NULL)
			return TEXT("");

#if defined USE_SYSTEM_CHECK & !defined NewWomaEngine
		strcpy_s(SystemHandle->systemDefinitions.cCurrentPath, FILENAME_MAX, CurrentPath);
#endif
#endif

		return cCurrentPath;
	}

	//-------------------------------------------------------------------------------------------
	TCHAR* getTemp()
		//-------------------------------------------------------------------------------------------
	{
		static TCHAR* strTEMPchar = TEXT("");

#if defined  WINDOWS_PLATFORM
		strTEMPchar = _tgetenv(TEXT("TEMP"));	// Get our Fullpath to all objects; ("TEMP" + /engine/)
#endif

#ifdef RELEASE
		womaTempPATH = strTEMPchar;
		womaTempPATH.append(TEXT("\\"));			// "C:\Users\"username"\AppData\Local\temp\"
		womaTempPATH.append(APP_COMPANY_NAME);		// "C:\Users\"username"\AppData\Local\temp\<pmborg>"
		womaTempPATH.append(TEXT("\\"));			// "C:\Users\"username"\AppData\Local\temp\<pmborg>\"
#endif

		return strTEMPchar;
	}

	//-------------------------------------------------------------------------------------------
	TCHAR* getHome()
		//-------------------------------------------------------------------------------------------
	{
		static TCHAR* strHOMEchar = TEXT("");

#if defined  WINDOWS_PLATFORM
		strHOMEchar = _tgetenv(TEXT("USERPROFILE"));
#elif defined LINUX_PLATFORM // Linux
		strHOMEchar = _tgetenv(TEXT("HOME"));
#endif

		return strHOMEchar;
	}

	//LINUX:	/home/pedro/projects/LinuxWoma/bin/x64/Debug
	//WINDOWS:	C:\WoMAengine2023\DXEngine_055
	//-------------------------------------------------------------------------------------------
	bool logDirs(BOOL WINXP_FLAG, BOOL bIsWow64)
		//-------------------------------------------------------------------------------------------
	{
		TCHAR currentPath[MAX_STR_LEN] = { 0 };
#if defined  WINDOWS_PLATFORM
		GetCurrentDirectory(MAX_STR_LEN, currentPath);
#else
		//char cwd[MAX_STR_LEN] = {0};
		char* res = getcwd(currentPath, sizeof(currentPath));
		if (!res) {
			perror("getcwd() error");
			return 1;
		}
#endif

		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Current Directory: %s\n"), currentPath);

#if defined  WINDOWS_PLATFORM && defined RELEASE // In "Debug" we use the local data at disk / At production "Release" we use the default "Dirs"
		// Check if is a network PATH:
		//if (PathIsNetworkPath  (SystemHandle->systemDefinitions.cCurrentPath)) 
		//	{WomaFatalException( TEXT("On Release version, no NETWORK PATH is allowed!") ); return false;}
#endif
		char* Temp = getTemp();
		char* Home = getHome();

		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("PROGRAM_FILES Directory: %s\n"), PROGRAM_FILES);
		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("APPDATA Directory: %s\n"), APPDATA);
		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("TEMP Directory: %s\n"), Temp);
		WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("HOME Directory: %s\n"), Home);

		return true;
	}

	//-------------------------------------------------------------------------------------------
	bool init_os_main_dirs(TCHAR* currentdir)
		//-------------------------------------------------------------------------------------------
	{

#if defined RELEASE && !defined ANDROID_PLATFORM
		// DONT LOG HERE!

#if defined WINDOWS_PLATFORM
#if defined _NOT
		// "C:\Users\"username"\AppData\Local\Pmborg\WoMA3Dengine\"
		//StringCchPrintf(APPDATA, MAX_STR_LEN, TEXT("%s\\%s\\%s\\"), _tgetenv(TEXT("APPDATA")), APP_COMPANY_NAME, APP_PROJECT_NAME);
		//if (!bIsWow64 || WINXP_FLAG)
		//	_tcscpy_s(PROGRAM_FILES, _tgetenv(TEXT("ProgramFiles")));	//is Not Wow64
		//else
#endif
		_tcscpy_s(PROGRAM_FILES, _tgetenv(TEXT("ProgramFiles(x86)")));//Is Wow64: 32 bits on a 64bits OS.

		StringCchPrintf(APPDATA1, MAX_STR_LEN, TEXT("%s\\%s"), _tgetenv(TEXT("TEMP")), APP_COMPANY_NAME);
		StringCchPrintf(APPDATA, MAX_STR_LEN, TEXT("%s\\%s\\%s\\"), _tgetenv(TEXT("TEMP")), APP_COMPANY_NAME, APP_PROJECT_NAME);
		StringCchPrintf(PUBLIC_DOCUMENTS0, MAX_STR_LEN, TEXT("%s\\%s\\%s"), _tgetenv(TEXT("PUBLIC")), TEXT("Documents"), APP_COMPANY_NAME);
		StringCchPrintf(PUBLIC_DOCUMENTS1, MAX_STR_LEN, TEXT("%s\\%s\\%s\\%d"), _tgetenv(TEXT("PUBLIC")), TEXT("Documents"), APP_COMPANY_NAME, CORE_ENGINE_LEVEL);
#if CORE_ENGINE_LEVEL >= 10
		StringCchPrintf(PUBLIC_DOCUMENTS, MAX_STR_LEN, TEXT("%s\\%s\\%s\\%d\\%d\\"), _tgetenv(TEXT("PUBLIC")), TEXT("Documents"), APP_COMPANY_NAME, CORE_ENGINE_LEVEL, DX_ENGINE_LEVEL);
#else
		StringCchPrintf(PUBLIC_DOCUMENTS, MAX_STR_LEN, TEXT("%s\\%s\\%s\\%d\\"), _tgetenv(TEXT("PUBLIC")), TEXT("Documents"), APP_COMPANY_NAME, CORE_ENGINE_LEVEL);
#endif

		TCHAR src[MAX_STR_LEN] = { 0 };
		TCHAR dest[MAX_STR_LEN] = { 0 };

		TCHAR ChkSettings[MAX_STR_LEN] = { 0 };
		StringCchPrintf(ChkSettings, MAX_STR_LEN, TEXT("%s%s"), PUBLIC_DOCUMENTS, TEXT("settings.xml"));	//C:\Users\Public\Documents\\Pmborg\\CORE_ENGINE_LEVEL\\DX_ENGINE_LEVEL\\settings.xml
		bool settings = fileExists(ChkSettings);
		if (!settings)
		{

			bool isDocDirCreated0 = CreateDirectory(PUBLIC_DOCUMENTS0, 0);	//TO:		//C:\Users\Public\Documents\\Pmborg
			bool isDocDirCreated1 = CreateDirectory(PUBLIC_DOCUMENTS1, 0);	//TO:		//C:\Users\Public\Documents\\Pmborg\\CORE_ENGINE_LEVEL
			bool isDocDirCreated = CreateDirectory(PUBLIC_DOCUMENTS, 0);	//TO:		//C:\Users\Public\Documents\\Pmborg\\CORE_ENGINE_LEVEL\\DX_ENGINE_LEVEL

			StringCchPrintf(src, MAX_STR_LEN, TEXT("%s\\%s"), currentdir, TEXT("settings.xml"));
			StringCchPrintf(dest, MAX_STR_LEN, TEXT("%s%s"), PUBLIC_DOCUMENTS, TEXT("settings.xml"));
			bool b = CopyFile(src, dest, true);
		}

		//APPDATA = TEMP
		STRING dirName1 = WOMA::APPDATA1;
#if defined _NOT
		/*
		char r1; //PATH string replacement()
		std::map<char, char> rs1 = { {'\\', '/'} };	//CONVERT:	C:\\Users\\<username>\\AppData\\Roaming\\Pmborg
		std::replace_if(dirName1.begin(), dirName1.end(), [&](char c) { return r1 = rs1[c]; }, r1);
		*/
#endif
		STRING dirName = WOMA::APPDATA;
#if defined _NOT
		/*
		char r; //PATH string replacement()
		std::map<char, char> rs = { {'\\', '/'} };	//CONVERT:	C:\\Users\\<username>\\AppData\\Roaming\\Pmborg\\WoMA
		std::replace_if(dirName.begin(), dirName.end(), [&](char c) { return r = rs[c]; }, r);
		*/
#endif
		bool isDirCreated1 = CreateDirectory(dirName1.c_str(), 0);	//TO:		dirName1 = "\\Temp\\Pmborg"
		DWORD dw1 = GetLastError();
		if (!isDirCreated1 && dw1 == ERROR_PATH_NOT_FOUND)
		{
			//Errors: 
			// ERROR_ALREADY_EXISTS
			// ERROR_PATH_NOT_FOUND
			TCHAR ErrorMsg[MAX_STR_LEN] = { 0 };
			StringCchPrintf(ErrorMsg, MAX_STR_LEN, TEXT("CreateDirectory() %s Failed with (%d)"), dirName1.c_str(), dw1);
			_stprintf(TEXT("CreateDirectory() %s Failed with (%d)"), dirName1.c_str(), dw1);
			WomaMessageBox(ErrorMsg, TEXT("FATAL ERROR:"));
			if (ERROR_ALREADY_EXISTS != dw1)
				return false;
		}

		bool isDirCreated = CreateDirectory(dirName.c_str(), 0);	//TO:		dirName = "\\Temp\\Pmborg\\WoMA3Dengine\\"
		DWORD dw = GetLastError();
		if (!isDirCreated && dw == ERROR_PATH_NOT_FOUND)
		{
			//Errors: 
			// ERROR_ALREADY_EXISTS
			// ERROR_PATH_NOT_FOUND
			TCHAR ErrorMsg[MAX_STR_LEN] = { 0 };
			StringCchPrintf(ErrorMsg, MAX_STR_LEN, TEXT("CreateDirectory() %s Failed with (%d)"), dirName.c_str(), dw);
			_stprintf(TEXT("\nCreateDirectory() %s Failed with (%d)"), dirName.c_str(), dw);
			WomaMessageBox(ErrorMsg, TEXT("FATAL ERROR:"));
			if (ERROR_ALREADY_EXISTS != dw)
				return false;
		}

		StringCchPrintf(src, MAX_STR_LEN, TEXT("%s\\%s"), currentdir, TEXT("windows.pck"));
		StringCchPrintf(dest, MAX_STR_LEN, TEXT("%s%s"), APPDATA, TEXT("windows.pck"));
		bool b = CopyFile(src, dest, true);

		StringCchPrintf(src, MAX_STR_LEN, TEXT("%s\\%s"), currentdir, TEXT("woma.pck"));
		StringCchPrintf(dest, MAX_STR_LEN, TEXT("%s%s"), APPDATA, TEXT("woma.pck"));
		b = CopyFile(src, dest, true);
#if defined _NOT
		StringCchPrintf(src, MAX_STR_LEN, TEXT("%s\\%s"), currentdir, TEXT("my.ip"));
		StringCchPrintf(dest, MAX_STR_LEN, TEXT("%s%s"), APPDATA, TEXT("my.ip"));
		b = CopyFile(src, dest, true);

		StringCchPrintf(src, MAX_STR_LEN, TEXT("%s\\%s"), currentdir, TEXT("present.weather"));
		StringCchPrintf(dest, MAX_STR_LEN, TEXT("%s%s"), APPDATA, TEXT("present.weather"));
		b = CopyFile(src, dest, true);
#endif
#endif
		// "C:\ProgramFiles(x86)\Pmborg\WoMA3Dengine\"
		// "C:\Program Files\Pmborg\WoMA3Dengine\"
		//StringCchPrintf(PROGRAM_FILES, MAX_STR_LEN, TEXT("%s\\%s\\%s\\"), (!bIsWow64 || WINXP_FLAG) ? _tgetenv(TEXT("ProgramFiles")) : _tgetenv(TEXT("ProgramFiles(x86)")), APP_COMPANY_NAME, APP_PROJECT_NAME);
		// --OR--
		//Installer will install on: C:\Program Files (x86)\WoMA3Dengine
		StringCchPrintf(PROGRAM_FILES, MAX_STR_LEN, TEXT("%s\\%s\\"), _tgetenv(TEXT("ProgramFiles(x86)")), APP_PROJECT_NAME);
#endif

#if CORE_ENGINE_LEVEL >= 2 && defined LINUX_PLATFORM
		// "C:\Program Files\Pmborg\Woma2017\" ==> "/opt/Pmborg/Woma2017"
		// "C:\Users\"username"\AppData\Local\Pmborg\Woma2017\" ==> ~/.pmborg/Woma2017
		StringCchPrintf(PROGRAM_FILES, MAX_STR_LEN, TEXT("/opt/%s/%s/"), APP_COMPANY_NAME, APP_PROJECT_NAME);
		StringCchPrintf(APPDATA, MAX_STR_LEN, TEXT("~/.%s/%s/"), APP_COMPANY_NAME, APP_PROJECT_NAME);
#endif

		return true;
	}

	void setup_OSmain_dirs() {
		char path[MAX_PATH] = { 0 };
		// (No Need on DEBUG!)
#ifdef RELEASE
// Make sure we're running in the exe's directory:
// -------------------------------------------------------------------------------------------
#if defined WINDOWS_PLATFORM
		if (GetModuleFileNameA(NULL, path, sizeof(path))) {
			char* slash = strrchr(path, '\\');
			if (slash) *slash = NULL;
			_chdir(path);
		}
#else
		if (realpath("/proc/self/exe", path)) {
			char* slash = strrchr(path, '/');
			if (slash) *slash = NULL;
			chdir(path);
		}
#endif
#endif

		//#if defined LINUX_PLATFORM
		//chdir("..");
		//#endif

		// [1] init_os_main_dirs!
		// -------------------------------------------------------------------------------------------
		_tprintf(TEXT("Init: init_os_main_dirs()\n"));
#if defined WINDOWS_PLATFORM
#if defined UNICODE
		TCHAR WDirbuffer[MAX_STR_LEN] = { 0 };
		atow(WDirbuffer, path, MAX_STR_LEN);
		init_os_main_dirs(WDirbuffer);
#else
		init_os_main_dirs(path);
#endif
#else
		//Populate: PROGRAM_FILES & APPDATA
		init_os_main_dirs();
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

#if defined WINDOWS_PLATFORM
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
#else
	BOOL isWow64()
	{
		return FALSE;
	}
#endif

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
