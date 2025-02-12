// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: 0CheckOS.cpp
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
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "OSengine.h"
#include "log.h"
#include "OSmain_dir.h"		//#include "OsDirectories.h"
#include "systemManager.h"

#include <map>
#if defined WINDOWS_PLATFORM
#include <atlstr.h>
#endif

SystemManager::SystemManager()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567311;

	CPUSpeedMHz = 0;

#if CORE_ENGINE_LEVEL >= 4 && defined WINDOWS_PLATFORM
	driveLetter = 0;
#endif

	//CheckOS:
	ZeroMemory(&pszOS, sizeof(pszOS));
	MajorVersion = MinorVersion = BuildVersion = 0;

	ZeroMemory(&osPlatform, sizeof(osPlatform));
	DXGI_H = 0; DXGI_L = 0;
}

SystemManager::~SystemManager() { CLASSDELETE(); }

char osName[1024 * 4];
#if defined LINUX_PLATFORM
char* OS_name()
{
	FILE *fp = NULL;
	fp = popen("/bin/bash -c set | grep 'OSTYPE=' | awk -F= '{print $2}'", "r");
	fread(osName, 1, sizeof(osName)-1, fp);
	fclose(fp);

	for (int i = 0; osName[i] != 0; i++)
	if (osName[i] == 10)
		osName[i] = 0;

	return osName;
}
#endif

#if defined ANDROID_PLATFORM
char* OS_name()
{
	FILE *fp = NULL;
	fp = popen("/bin/sh -c set | grep 'OSTYPE=' | awk -F= '{print $2}'", "r");
	fread(osName, 1, sizeof(osName)-1, fp);
	fclose(fp);

	for (int i = 0; osName[i] != 0; i++)
	if (osName[i] == 10)
		osName[i] = 0;

	return osName;
}
#endif


// PUBLIC FUNCTIONS:
//------------------------------------------------------------------

//------------------------------------------------------------------
// PRIVATE FUNCTIONS:
//------------------------------------------------------------------
#pragma warning( push )
#pragma warning( disable : 4996 )
TCHAR* SystemManager::GetOSversionPlatform()
//------------------------------------------------------------------
{
#if defined WINDOWS_PLATFORM
	// GET OS Version and Platform name:
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	//Explicitly map the win32 dwPlatformId to our own values 
	switch (osvi.dwPlatformId)
	{
	case 0:
		_tcscpy_s(osPlatform, (TCHAR*)TEXT("Windows 3.1")); /*
													Windows 3.1
													Windows 3.11
													*/
		break;
	case 1:
		_tcscpy_s(osPlatform, (TCHAR*)TEXT("Windows 9x"));	/*
													Windows 95				  - 386 de 33 MHz
													Windows 98				  - 486 de 90 MHz
													Windows ME				  - Pentium/K5 de 133 MHz
													*/
		break;
	case 2:
		_tcscpy_s(osPlatform, (TCHAR*)TEXT("Windows NT")); /*
												   Windows NT 3.1 				- 386 16 MHz
												   Windows NT 3.51 				- 386 25 MHz
												   Windows NT 4.0 				- 486 33 MHz
												   Windows 2000 Professional	- Pentium 2 300 MHz
												   Windows XP Professional		- Pentium MMX / AMD Athlon 350 MHz
												   Windows Vista				- CPU de 1 GHz
												   Windows Server 2008 			- Dual Core 1.4 GHz
												   Windows 7 					- CPU de 1 GHz
												   Windows 8 e Windows 8.1		- CPU de 1 GHz
												   Windows 10 					- CPU de 1 GHz
												   */
		break;
	case 3:
		_tcscpy_s(osPlatform, (TCHAR*)TEXT("Windows CE"));
		break;

	default:
		_tcscpy_s(osPlatform, (TCHAR*)TEXT(""));
		break;
	}

	return osPlatform;
#elif defined LINUX_PLATFORM
	return TEXT("LINUX");
#elif defined ANDROID_PLATFORM	
	return TEXT("ANDROID");
#endif
}

#if defined WINDOWS_PLATFORM

#ifdef WIN10
#include <VersionHelpers.h> // IsWindows10OrGreater
#endif

#include <iostream>
#include <windows.h>
#pragma comment(lib, "Version.lib" )

TCHAR* SystemManager::GetOsVersion()
{
	TCHAR path[200] = TEXT("C:\\Windows\\System32\\kernel32.dll");
	DWORD dwDummy;
	DWORD dwFVISize = GetFileVersionInfoSize(path, &dwDummy);
	LPBYTE lpVersionInfo = NEW BYTE[dwFVISize];
	if (GetFileVersionInfo(path, 0, dwFVISize, lpVersionInfo) == 0)
		return NULL;

	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi;
	BOOL bVer = VerQueryValue(lpVersionInfo, TEXT("\\"), (LPVOID*)&lpFfi, &uLen);

	if (!bVer || uLen == 0)
		return NULL;
	
	DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
	SAFE_DELETE(lpVersionInfo);

	if (HIWORD(dwProductVersionMS) == 10 && LOWORD(dwProductVersionMS) == 0)
	{
		return TEXT("Windows 10\n");
	}
	else if (HIWORD(dwProductVersionMS) == 6 && LOWORD(dwProductVersionMS) == 3)
	{
		return TEXT("Windows 8.1\n");
	}
	else if (HIWORD(dwProductVersionMS) == 6 && LOWORD(dwProductVersionMS) == 2)
	{
		return TEXT("Windows 8\n");
	}
	else if (HIWORD(dwProductVersionMS) == 6 && LOWORD(dwProductVersionMS) == 1)
	{
		return TEXT("Windows 7 or Windows Server 2008 R2\n");
	}
	else if (HIWORD(dwProductVersionMS) == 6 && LOWORD(dwProductVersionMS) == 0)
	{
		return TEXT("Windows Vista or Windows Server 2008\n");
	}
#if LEGACY_OS_SUPPORTED
	else if (HIWORD(dwProductVersionMS) == 5 && LOWORD(dwProductVersionMS) == 2)
	{
		return TEXT("Windows Server 2003\n");
	}
	else if (HIWORD(dwProductVersionMS) == 5 && LOWORD(dwProductVersionMS) == 1)
	{
		return TEXT("Windows Server XP\n");
	}
	else if (HIWORD(dwProductVersionMS) == 5 && LOWORD(dwProductVersionMS) == 0)
	{
		return TEXT("Windows 2000\n");
	}
	else if (lpFfi->dwFileVersionMS == 4 && lpFfi->dwFileVersionLS == 90)
	{
	    return TEXT("Windows  Me\n");
	}
	else if (lpFfi->dwFileVersionMS == 4 && lpFfi->dwFileVersionLS == 10)
	{
	    return TEXT("Windows  98\n");
	}
	else if (lpFfi->dwFileVersionMS == 4 && lpFfi->dwFileVersionLS == 0)
	{
	    return TEXT("Windows  95\n");
	}
#endif

	return TEXT("Windows version unknown!\n");
}
#endif


#if defined WINDOWS_PLATFORM

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL(WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

bool SystemManager::CheckDXGIVersion(bool *REQUIRES_WINDOWS_VISTA_SP2, bool *REQUIRES_UPDATE_KB971644)
//------------------------------------------------------------------
{
	*REQUIRES_WINDOWS_VISTA_SP2 = false;
	*REQUIRES_UPDATE_KB971644 = false;

	// (WINDOWS >= 6.3)?
	if (MajorVersion > 6 || MajorVersion == 6 && MinorVersion >= 3) {	// Windows 8.1 / SUPPORT: DX11.2 / DRIVER MODEL: WDDM 1.3
		DXGI_H = 1; DXGI_L = 3;
		return true;
	}
	// (WINDOWS == 6.2)?
	if (MajorVersion == 6 && MinorVersion >= 2) {	// Windows 8 / SUPPORT: DX11.1 / DRIVER MODEL: WDDM 1.2
		DXGI_H = 1; DXGI_L = 2;
		return true;
	}
	// (WINDOWS == 6.1)?
	if (MajorVersion == 6 && MinorVersion >= 1) {	// Windows 7 & Windows Vista + Update: KB971644 / SUPPORT: DX11.0 / DRIVER MODEL: WDDM 1.1
		DXGI_H = 1; DXGI_L = 1;
		return true;
	}
	// (WINDOWS = 6.0 with more than SP2) => Should only get here for Windows Vista or Windows Server 2008 SP2 (6.0.6002)
	if (BuildVersion > 6002)
	{
		// Windows Vista/Server 2008 Service Packs after SP2 should already include Direct3D 11
		WOMA_LOGManager_DebugMSGAUTO(TEXT("Windows Vista/Server 2008 With Service Packs SP2, Checked!\n"));
		return true;
	}

	// (WINDOWS = 6.0 without SP2)
	if (BuildVersion < 6002)
	{
		// Windows Vista/Server 2008 SP2 is required for Direct3D 11
		WOMA_LOGManager_DebugMSGAUTO(TEXT("Windows Vista/Server 2008 SP2 is required for Direct3D 11!\n"));
		*REQUIRES_WINDOWS_VISTA_SP2 = true;
		return false;
	}

	// (WINDOWS = 6.0 with SP2), Check if Update: KB971644 is needed?
	HMODULE hd3d = LoadLibrary(TEXT("D3D11.DLL"));
	if (hd3d)
	{
		FreeLibrary(hd3d);
		// If we find D3D11, we'll assume the Direct3D 11 Runtime is installed
		// (incl. Direct3D 11, DXGI 1.1, WARP10, 10level9, Direct2D, DirectWrite, updated Direct3D 10.1)
		DXGI_H = 1; DXGI_L = 1;
		return true;
	}

	// Did not find the D3D11.DLL, so "Windows Update": KB971644 not installed!
	DXGI_H = 1; DXGI_L = 0;

	// Verify it is a supported architecture for KB971644
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	switch (sysinfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
	case PROCESSOR_ARCHITECTURE_AMD64:
		// REQUIRES_UPDATE: KB971644
		*REQUIRES_UPDATE_KB971644 = true;
		break;

	default:
		//NOT_SUPPORTED;
		return false;
	}

	return true;
}
#endif

#pragma warning( pop )

