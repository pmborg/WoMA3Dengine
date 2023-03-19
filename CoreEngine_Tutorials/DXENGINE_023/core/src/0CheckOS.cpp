// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: 0CheckOS.cpp
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
// --------------------------------------------------------------------------------------------

#include "OSengine.h"
#include "OSmain_dir.h"		//#include "OsDirectories.h"
#include "systemManager.h"
#include "mem_leak.h"

SystemManager::SystemManager()
{
	CLASSLOADER();

	CPUSpeedMHz = 0;

	driveLetter = 0;

	//CheckOS:
	ZeroMemory(&pszOS, sizeof(pszOS));
	MajorVersion = MinorVersion = BuildVersion = 0;

	ZeroMemory(&osPlatform, sizeof(osPlatform));
	DXGI_H = 0; DXGI_L = 0;
}

SystemManager::~SystemManager() { CLASSDELETE(); }

char osName[1024 * 4];

// PUBLIC FUNCTIONS:
//------------------------------------------------------------------
bool SystemManager::CheckOS()
//------------------------------------------------------------------
{
	// Check Platform: WINDOWS / LINUX / ANDROID
	//------------------------------------------------------------------
	StringCchPrintf(SystemHandle->systemDefinitions.platform, MAX_STR_LEN, TEXT("Platform: %s"), GetOSversionPlatform());
	WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), SystemHandle->systemDefinitions.platform);

	// String Character Set: UNICODE / ANSI
	//------------------------------------------------------------------	
	StringCchPrintf(SystemHandle->systemDefinitions.characterSet, MAX_STR_LEN, TEXT("Character Set: %s"),
#ifdef UNICODE
		TEXT("UNICODE")
#else
		TEXT("ANSI")
#endif
		);

	WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), SystemHandle->systemDefinitions.characterSet);

	// Check Bits Architecture: 32Bits+SSE2 vs 64Bits+AVX
	//------------------------------------------------------------------

	#ifdef X64
	#define _BITS_ 64
	#else
	#define _BITS_ 32
	#endif

	StringCchPrintf(SystemHandle->systemDefinitions.binaryArchitecture, MAX_STR_LEN, TEXT("Binary Architecture: %d bits"), _BITS_);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.binaryArchitecture);

#ifdef WIN10
	#define _BINARY_CODE_ TEXT("Windows 10 Code")
#elif defined WIN6x
	#define _BINARY_CODE_ TEXT("Windows Vista Code")
#elif defined WIN_XP
	#define _BINARY_CODE_ TEXT("Windows XP Code")
#endif

	StringCchPrintf(SystemHandle->systemDefinitions.binaryCode, MAX_STR_LEN, TEXT("Binary Code: %s"), _BINARY_CODE_);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.binaryCode);

	// Check OS Version
	//------------------------------------------------------------------
	IF_NOT_RETURN_FALSE(CheckOSVersion());

  //#if !defined WIN10
	StringCchPrintf(SystemHandle->systemDefinitions.windowsVersion, MAX_STR_LEN, TEXT("Windows Version: %d.%d"), MajorVersion, MinorVersion);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.windowsVersion);

	StringCchPrintf(SystemHandle->systemDefinitions.windowsBuildNumber, MAX_STR_LEN, TEXT("Windows Build Number: %d"), BuildVersion);
	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.windowsBuildNumber);
  //#endif

	StringCchPrintf(SystemHandle->systemDefinitions.osName, MAX_STR_LEN, TEXT("Current OS: %s"), pszOS);

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.osName);

	// Command LINE:
	//------------------------------------------------------------------
	if (WOMA::Scmdline)
		StringCchPrintf(SystemHandle->systemDefinitions.cmdLine, MAX_STR_LEN, TEXT("CMD LINE: %s %s"), WOMA::filename.c_str(), WOMA::Scmdline);
	else
		StringCchPrintf(SystemHandle->systemDefinitions.cmdLine, MAX_STR_LEN, TEXT("CMD LINE: %s"), WOMA::filename.c_str());

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.cmdLine);

	return true;
}

//------------------------------------------------------------------
// PRIVATE FUNCTIONS:
//------------------------------------------------------------------
#pragma warning( push )
#pragma warning( disable : 4996 )
TCHAR* SystemManager::GetOSversionPlatform()
//------------------------------------------------------------------
{
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
}

#ifdef WIN10
#include <VersionHelpers.h> // IsWindows10OrGreater
#endif

#include <iostream>
#include <windows.h>
#pragma comment(lib, "Version.lib" )

TCHAR* GetOsVersion()
{
	/*wchar_t*/TCHAR path[200] = TEXT("C:\\Windows\\System32\\kernel32.dll");
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
	//else if (lpFfi->dwFileVersionMS == 4 && lpFfi->dwFileVersionLS == 90)
	//{
	//    return TEXT("Windows  Me\n");
	//}
	//else if (lpFfi->dwFileVersionMS == 4 && lpFfi->dwFileVersionLS == 10)
	//{
	//    return TEXT("Windows  98\n");
	//}
	//else if (lpFfi->dwFileVersionMS == 4 && lpFfi->dwFileVersionLS == 0)
	//{
	//    return TEXT("Windows  95\n");
	//}

	return TEXT("Windows Unknown\n");
}

bool SystemManager::CheckOSVersion()
//------------------------------------------------------------------
{
	//
	// Check:
	//

#ifdef WIN_XP
	OSVERSIONINFOEX osvi = { 0 };
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	DWORDLONG dwlConditionMask = 0;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

	osvi.dwMajorVersion = 5;
	osvi.dwMinorVersion = 1;

	bool winXp = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
	ASSERT(winXp);
#endif

#ifdef WIN6x
	OSVERSIONINFOEX osvi = { 0 };
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	DWORDLONG dwlConditionMask = 0;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

	osvi.dwMajorVersion = 6;
	osvi.dwMinorVersion = 0;

	BOOL winVista = VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
	ASSERT(winVista);
#endif

	// OS Version check tells us most of what we need to know
	OSVERSIONINFOEX osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(osinfo);
	if (!GetVersionEx((OSVERSIONINFO*)&osinfo))
	{
		HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
		WOMA_LOGManager_DebugMSGAUTO(TEXT("GetOsVersionEx failed with HRESULT %x\n"), hr);
		return false;
	}

	//NT 5.1: XP
	//NT 6.0: VISTA
	//NT 6.1: WIN7
	//NT 6.2: WIN8
	//NT 6.3: WIN8.1

	//WOMA_LOGManager_DebugMSG( TEXT("Windows Version: %d.%d\n"), osinfo.dwMajorVersion, osinfo.dwMinorVersion );
	MajorVersion = osinfo.dwMajorVersion;
	MinorVersion = osinfo.dwMinorVersion;

	//WOMA_LOGManager_DebugMSG( TEXT("Windows Build Number: %d\n"), osinfo.dwBuildNumber );
	BuildVersion = osinfo.dwBuildNumber;


	/*
	if (IsWindowsServer())
	{
		printf("Server\n");
	}
	else
	{
		printf("Client\n");
	}
	*/

	//IsWindows10OrGreater is never detected...:
	if (IsWindows10OrGreater())
	{
		WOMA_LOGManager_DebugMSG("Windows10OrGreater\n"); return true;
	}
	if (IsWindows8Point1OrGreater())
	{
		WOMA_LOGManager_DebugMSG("Windows8Point1OrGreater\n"); return true;
	}
	if (IsWindows8OrGreater())
	{
		WOMA_LOGManager_DebugMSG("Windows8 OrGreater\n"); return true;
	}
	if (IsWindows7SP1OrGreater())
	{
		WOMA_LOGManager_DebugMSG("Windows7SP1OrGreater\n"); return true;
	}
	if (IsWindows7OrGreater())
	{
		WOMA_LOGManager_DebugMSG("Windows7OrGreater\n"); return true;
	}
	if (IsWindowsVistaSP2OrGreater())
	{
		WOMA_LOGManager_DebugMSG("VistaSP2OrGreater\n"); return true;
	}
	if (IsWindowsVistaSP1OrGreater())
	{
		WOMA_LOGManager_DebugMSG("VistaSP1OrGreater\n"); return true;
	}
	if (IsWindowsVistaOrGreater())
	{
		WOMA_LOGManager_DebugMSG("VistaOrGreater\n"); return true;
	}
	if (IsWindowsXPSP3OrGreater())
	{
		WOMA_LOGManager_DebugMSG("XPSP3OrGreater\n"); return true;
	}
	if (IsWindowsXPSP2OrGreater())
	{
		WOMA_LOGManager_DebugMSG("XPSP2OrGreater\n"); return true;
	}
	if (IsWindowsXPSP1OrGreater())
	{
		WOMA_LOGManager_DebugMSG("XPSP1OrGreater\n"); return true;
	}
	if (IsWindowsXPOrGreater())
	{
		WOMA_LOGManager_DebugMSG("XPOrGreater\n"); return true;
	}

	return false;
}


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
	/*
	// (WINDOWS < 6.0)?
	if (MajorVersion < 6)
	{
		// Windows XP, Windows Server 2003, and earlier versions of OS do not support Direct3D 11!
		WOMA_LOGManager_DebugMSGAUTO(TEXT("WARNING: Windows XP, Windows Server 2003, and earlier versions of OS do not support Direct3D 10 or 11!\n"));
	#if defined DX9
		return true;
	#else
		return false; // Make it Fatal
	#endif
	}
	*/
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

#pragma warning( pop )

