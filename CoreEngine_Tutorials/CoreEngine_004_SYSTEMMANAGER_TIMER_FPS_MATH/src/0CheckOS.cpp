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

#include "OSengine.h"
#include "OSmain_dir.h"		//#include "OsDirectories.h"
#include "systemManager.h"
#include "mem_leak.h"
#include <map>
#if defined WINDOWS_PLATFORM
#include <atlstr.h>
#endif

SystemManager::SystemManager()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

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


#if defined WINDOWS_PLATFORM
//------------------------------------------------------------------
struct RTL_OSVERSIONINFOW_EX {
	ULONG dwOSVersionInfoSize;
	ULONG dwMajorVersion;
	ULONG dwMinorVersion;
	ULONG dwBuildNumber;
	ULONG dwPlatformId;
	WCHAR szCSDVersion[128];
};

typedef LONG(WINAPI* RtlGetVersionPtr)(RTL_OSVERSIONINFOW_EX*);

bool GetRealVersion(DWORD& major, DWORD& minor, DWORD& build)
{
	HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
	if (!ntdll) return false;

	auto fn = reinterpret_cast<RtlGetVersionPtr>(GetProcAddress(ntdll, "RtlGetVersion"));
	if (!fn) return false;

	RTL_OSVERSIONINFOW_EX vi{};
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (fn(&vi) == 0) {
		major = vi.dwMajorVersion;
		minor = vi.dwMinorVersion;
		build = vi.dwBuildNumber;
		return true;
	}
	return false;
}
//------------------------------------------------------------------
#endif

// PUBLIC FUNCTIONS:
//------------------------------------------------------------------
#if defined USE_SYSTEM_CHECK
bool SystemManager::CheckOS()
//------------------------------------------------------------------
{
	// Check Platform: WINDOWS / LINUX / ANDROID
	//------------------------------------------------------------------
#if defined WINDOWS_PLATFORM
	//StringCchPrintf(SystemHandle->systemDefinitions.platform, MAX_STR_LEN, TEXT("Platform: %s - %s"), GetOSversionPlatform(), GetOsVersion());
	//womalogauto(TEXT("%s\n"), SystemHandle->systemDefinitions.platform);
  #if _DEBUG
	IF_NOT_RETURN_FALSE(CheckOSVersion());
  #endif
#else
	IF_NOT_RETURN_FALSE(CheckOSVersion());
#endif

	// String Character Set: UNICODE / ANSI
	//------------------------------------------------------------------	
	StringCchPrintf(SystemHandle->systemDefinitions.characterSet, MAX_STR_LEN, TEXT("Character Set: %s"),
#ifdef UNICODE
		TEXT("UNICODE")
#else
		TEXT("ANSI")
#endif
		);

	womalogauto(TEXT("%s\n"), SystemHandle->systemDefinitions.characterSet);

	// Check Bits Architecture: 32Bits+SSE2 vs 64Bits+AVX
	//------------------------------------------------------------------

#if defined WINDOWS_PLATFORM
#ifdef X64
#define _BITS_ 64
#else
#define _BITS_ 32
#endif
#else // LINUX || ANDROID
#define _BITS_ __WORDSIZE
#endif

	StringCchPrintf(SystemHandle->systemDefinitions.binaryArchitecture, MAX_STR_LEN, TEXT("Binary Architecture: %d bits"), _BITS_);
	womalogauto((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.binaryArchitecture);

#ifdef WIN10 //NOTE: WIN11 is WIN10 Platform upgraded.
	#define _BINARY_CODE_ TEXT("Windows 10+ Code")
#elif defined WIN6x
	#define _BINARY_CODE_ TEXT("Windows Vista+ Code")
#elif defined WIN_XP
	#define _BINARY_CODE_ TEXT("Windows XP Code")
#elif defined LINUX_PLATFORM
	#define _BINARY_CODE_ TEXT("Linux Code")
#elif defined ANDROID_PLATFORM
	#define _BINARY_CODE_ TEXT("Android Code")
#endif

	StringCchPrintf(SystemHandle->systemDefinitions.binaryCode, MAX_STR_LEN, TEXT("Binary Code: %s"), _BINARY_CODE_);
	womalogauto((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.binaryCode);

#if defined WINDOWS_PLATFORM

	StringCchPrintf(SystemHandle->systemDefinitions.windowsVersion, MAX_STR_LEN, TEXT("Build Version: %d"), BuildVersion);
	womalogauto((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.windowsVersion);

	std::map<CString, CString> mapWindowsVersions
	{
		{ L"10240", L"1507" }, // Windows10 (Original  version)
		{ L"10586", L"1511" }, // Windows10 November Update	  
		{ L"14393", L"1607" }, // Windows10 (Anniversary Update)
		{ L"15063", L"1703" }, // Windows10 (Creators Update)
		{ L"16299", L"1709" }, // Windows10 (Fall Creators Update)
		{ L"17134", L"1803" }, // Windows10 (April 2018 Update)
		{ L"17763", L"1809" }, // Windows10 (October 2018 Update)
		{ L"18362", L"1903" }, // Windows10 (May 2019 Update)
		{ L"19002", L"1909" }, // Windows 10 19H2 November 2019
		{ L"19041", L"2004" }, // May 2020 Update
		{ L"19042", L"20H2" }, // October 2020 Update
		{ L"19043", L"2009" }, // May 2021 Update
		{ L"19044", L"21H2" }, // November 2021 Update
		{ L"19045", L"22H2" }, // Windows10 (October 2023 Update)

		//---------------------------------------------------------------------
		{ L"22000", L"21H2" }, // Windows11 (Original  version) October 5, 2021 
		{ L"22621", L"22H2" }, // Windows11 September 20, 2022 
		{ L"22631", L"23H2" }, // Windows11 October 31, 2023 
		{ L"26100", L"24H2" }, // Windows11 October 1, 2024
	};
	// Get the Windows Build Version:
	std::wstring v = std::to_wstring(BuildVersion);
	auto it = mapWindowsVersions.find(v.c_str());
	if (it != mapWindowsVersions.end())
	{
		DWORD maj, min, bld;
		bool isWin11 = false;
		if (GetRealVersion(maj, min, bld)) {
			// Windows 11 heuristic:
			isWin11 = (maj == 10 && bld >= 22000);
		}
		const TCHAR* family = isWin11 ? "11" : "10";

		StringCchPrintf(SystemHandle->systemDefinitions.windowsBuildVersion, MAX_STR_LEN,
			TEXT("Windows %s Version: %s"), family, it->second);
		womalogauto(TEXT("%s\n"), SystemHandle->systemDefinitions.windowsBuildVersion);
	}
#else
	womalogauto(TEXT("sysname: %s\n"), SystemHandle->systemDefinitions.ver.sysname);
	womalogauto(TEXT("nodename: %s\n"), SystemHandle->systemDefinitions.ver.nodename);
	womalogauto(TEXT("release: %s\n"), SystemHandle->systemDefinitions.ver.release);
	womalogauto(TEXT("version: %s\n"), SystemHandle->systemDefinitions.ver.version);
	womalogauto(TEXT("machine: %s\n"), SystemHandle->systemDefinitions.ver.machine);
#endif

#if defined WINDOWS_PLATFORM
	StringCchPrintf(SystemHandle->systemDefinitions.osName, MAX_STR_LEN, TEXT("OS name: %s"), pszOS);
#else
	StringCchPrintf(SystemHandle->systemDefinitions.osName, MAX_STR_LEN, TEXT("OS name: %s"), OS_name());
#endif

	womalogauto((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.osName);

	// Command LINE:
	//------------------------------------------------------------------
#if defined WINDOWS_PLATFORM
	if (WOMA::Scmdline)
		StringCchPrintf(SystemHandle->systemDefinitions.cmdLine, MAX_STR_LEN, TEXT("CMD LINE: %s %s"), WOMA::filename.c_str(), WOMA::Scmdline);
	else
		StringCchPrintf(SystemHandle->systemDefinitions.cmdLine, MAX_STR_LEN, TEXT("CMD LINE: %s"), WOMA::filename.c_str());

	womalogauto((TCHAR*)TEXT("%s\n"), SystemHandle->systemDefinitions.cmdLine);
#else // LINUX || ANDROID
	// Remember argv[0] is the "Program fileName":
	STRING CMD_LINE = TEXT("CMD LINE:");
	for (UINT i = 0; i < WOMA::ARGc; ++i) {
		if (i < WOMA::ARGc) {
			CMD_LINE += TEXT(" ");
			CMD_LINE += WOMA::ARGv[i];
		}
	}
	womalog("%s", CMD_LINE.c_str());
	womalog("\n");
#endif

	return true;
}
#endif

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

#if defined USE_SYSTEM_CHECK && defined WINDOWS_PLATFORM
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
		womalogauto(TEXT("GetOsVersionEx failed with HRESULT %x\n"), hr);
		return false;
	}

	//NT 5.1: XP
	//NT 6.0: VISTA
	//NT 6.1: WIN7
	//NT 6.2: WIN8
	//NT 6.3: WIN8.1

	//womalog( TEXT("Windows Version: %d.%d\n"), osinfo.dwMajorVersion, osinfo.dwMinorVersion );
	MajorVersion = osinfo.dwMajorVersion;
	MinorVersion = osinfo.dwMinorVersion;

	//womalog( TEXT("Windows Build Number: %d\n"), osinfo.dwBuildNumber );
	BuildVersion = osinfo.dwBuildNumber;

	BYTE b1 = HIBYTE(_WIN32_WINNT_WINTHRESHOLD);
	BYTE b2 = LOBYTE(_WIN32_WINNT_WINTHRESHOLD);

	if (IsWindowsServer())
	{
		//printf("Server\n");
		womalog(TEXT("Windows Server WINNT version: %d.%d\n"), b1, b2);
	}
	else
	{
		//printf("Client\n");
		womalog(TEXT("Windows WINNT version: %d.%d\n"), b1, b2);
	}


	//IsWindows10OrGreater is never detected...:
	if (IsWindows10OrGreater())
	{
		womalog("Windows10OrGreater\n"); return true;
	}
	if (IsWindows8Point1OrGreater())
	{
		womalog("Windows8Point1OrGreater\n"); return true;
	}
	if (IsWindows8OrGreater())
	{
		womalog("Windows8 OrGreater\n"); return true;
	}
	if (IsWindows7SP1OrGreater())
	{
		womalog("Windows7SP1OrGreater\n"); return true;
	}
	if (IsWindows7OrGreater())
	{
		womalog("Windows7OrGreater\n"); return true;
	}
	if (IsWindowsVistaSP2OrGreater())
	{
		womalog("VistaSP2OrGreater\n"); return true;
	}
	if (IsWindowsVistaSP1OrGreater())
	{
		womalog("VistaSP1OrGreater\n"); return true;
	}
	if (IsWindowsVistaOrGreater())
	{
		womalog("VistaOrGreater\n"); return true;
	}
	if (IsWindowsXPSP3OrGreater())
	{
		womalog("XPSP3OrGreater\n"); return true;
	}
	if (IsWindowsXPSP2OrGreater())
	{
		womalog("XPSP2OrGreater\n"); return true;
	}
	if (IsWindowsXPSP1OrGreater())
	{
		womalog("XPSP1OrGreater\n"); return true;
	}
	if (IsWindowsXPOrGreater())
	{
		womalog("XPOrGreater\n"); return true;
	}

	return false;
}
#else // LINUX || ANDROID
#if defined USE_SYSTEM_CHECK
#include <sys/utsname.h>
bool SystemManager::CheckOSVersion()
{
	//struct utsname ver;
	uname(&SystemHandle->systemDefinitions.ver);
	return true;
}
#endif
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
		womalogauto(TEXT("Windows Vista/Server 2008 With Service Packs SP2, Checked!\n"));
		return true;
	}

	// (WINDOWS = 6.0 without SP2)
	if (BuildVersion < 6002)
	{
		// Windows Vista/Server 2008 SP2 is required for Direct3D 11
		womalogauto(TEXT("Windows Vista/Server 2008 SP2 is required for Direct3D 11!\n"));
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

