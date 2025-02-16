// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: initWorld.cpp
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
//   Main Functions:
//	- Get IP and Location
//  - Get data from AstroClass
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#include "OSengine.h"
#include "mem_leak.h"
#if defined USE_ASTRO_CLASS
#include <GeoLite2PP.hpp>
#pragma comment(lib, "Ws2_32.lib")	//undefined reference to imp_getaddrinfo
extern bool download(const std::string url, const std::string file_path);
#if UNICODE
	#ifdef X64
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "x64/WDebug/maxminddb_LIBX64_d.lib" )		//DEBUG
			#pragma comment( lib, "x64/WDebug/GeoLite2PP_LIBX64_d.lib" )	//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			#pragma comment( lib, "x64/Release/maxminddb_LIBX64.lib" )		//RELEASE
			#pragma comment( lib, "x64/Release/GeoLite2PP_LIBX64.lib" )	//RELEASE
		#else
			#pragma comment( lib, "x64/WRelease/maxminddb_LIBX64.lib" )		//DBGREL
			#pragma comment( lib, "x64/WRelease/GeoLite2PP_LIBX64.lib" )	//DBGREL
		#endif
	#else
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "Win32/WDebug/maxminddb_LIB_d.lib" )		//DEBUG
			#pragma comment( lib, "Win32/WDebug/GeoLite2PP_LIB_d.lib" )		//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			#pragma comment( lib, "Win32/WRelease/maxminddb_LIB.lib" )		//RELEASE
			#pragma comment( lib, "Win32/WRelease/GeoLite2PP_LIB.lib" )		//RELEASE
		#else
			#pragma comment( lib, "Win32/WRelease/maxminddb_LIB.lib" )		//DBGREL
			#pragma comment( lib, "Win32/WRelease/GeoLite2PP_LIB.lib" )		//DBGREL
		#endif
	#endif
#else
	#ifdef X64
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "x64/Debug/maxminddb_LIBX64_d.lib" )		//DEBUG
			#pragma comment( lib, "x64/Debug/GeoLite2PP_LIBX64_d.lib" )		//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			#pragma comment( lib, "x64/Release/maxminddb_LIBX64.lib" )		//RELEASE
			#pragma comment( lib, "x64/Release/GeoLite2PP_LIBX64.lib" )		//RELEASE
		#else
			#pragma comment( lib, "x64/Release/maxminddb_LIBX64.lib" )		//DBGREL
			#pragma comment( lib, "x64/Release/GeoLite2PP_LIBX64.lib" )		//DBGREL
		#endif
	#else
		#if defined(_DEBUG) & !defined(NDEBUG)
			#pragma comment( lib, "Win32/Debug/maxminddb_LIB_d.lib" )		//DEBUG
			#pragma comment( lib, "Win32/Debug/GeoLite2PP_LIB_d.lib" )		//DEBUG
		#elif !defined _DEBUG && defined NDEBUG
			#pragma comment( lib, "Win32/Release/maxminddb_LIB.lib" )		//RELEASE
			#pragma comment( lib, "Win32/Release/GeoLite2PP_LIB.lib" )		//RELEASE
		#else
			#pragma comment( lib, "Win32/Release/maxminddb_LIB.lib" )		//DBGREL
			#pragma comment( lib, "Win32/Release/GeoLite2PP_LIB.lib" )		//DBGREL
		#endif
	#endif
#endif

#include "astroClass.h"
#include "initWorld.h"
#include "OSmain_dir.h"		//#include "OsDirectories.h"
#include "log.h"			//#include "logManager.h"
//#include <urlmon.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

// Internet:
#ifdef USE_NETWORK
	#if defined WINDOWS_PLATFORM //getMyIp
		#include <urlmon.h>
		#pragma comment( lib, "urlmon.lib" )
#include <winsock2.h>
#include <iphlpapi.h>
		// Link with Iphlpapi.lib
		#pragma comment(lib, "IPHLPAPI.lib")
	#endif
#endif



// --------------------------------------------------------------------------------------------
// Globals:
// --------------------------------------------------------------------------------------------
// Init Sun Light Class:
AstroClass* astroClass = NULL;	

InitWorld::InitWorld ()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567142;

	//public:
    LatDir = TEXT("N");
    LonDir = TEXT("W");

	latitude  = NULL;
	longitude = NULL;

    // Sun:
    h_sunrise = NULL;
    m_sunrise = NULL;

    h_sunset = NULL;
    m_sunset = NULL;

	geometricElevation = SunElevation = SunAzimuth = NULL;

    // Moon:
	moonDistance = NULL;
    moonScale = NULL;
	moon_image_indx = NULL;
	ZeroMemory(&moonFilename, sizeof(moonFilename));

	geometricMoonElevation = moonElevation = MoonAz = NULL;

	//-----------------------------------------------------------------------------
    astroClass = NEW AstroClass();
	IF_NOT_THROW_EXCEPTION (astroClass);

	//
	// http://whatismyipaddress.com/
	//-----------------------------------------------------------------------------
	STRING ip = getMyIp();	//get my external IP!

	//
	// http://whatismyipaddress.com/ip/xxx.xxx.xxx.xxx
	//-----------------------------------------------------------------------------

	bool gotLocation = false;

	if (ip.length() > 0) {
#if defined NDEBUG || CORE_ENGINE_LEVEL == 8
		gotLocation = getMyLocation(&latitude, &longitude, ip);
#endif
		WOMA_LOGManager_DebugMSGAUTO(TEXT("gotLocation: true\n"));
	}
	gotLocation = false;//
	if (!gotLocation || latitude==0 || longitude==0) 
	{							// Default: Lisbon Location:
		latitude  = 38.7167;	// Latitude:	38.7167  (38° 43′ 0.12″ N)
		longitude = -9.1333;	// Longitude:	-9.1333  (9° 7′ 59.88″ W)
	}

    astroClass->Initialize(latitude, longitude);

    // Moon Calc (Moon's age ):
    UINT moonAge = (UINT) astroClass->MoonAge(astroClass->day, astroClass->month, astroClass->year);		// Values from: 1 to 30 (30: New MOON) (15: FULL MOON)
	WOMA_LOGManager_DebugMSGAUTO(TEXT("moonAge: %d\n"), moonAge);

    // Moon 24 Index Images
    moon_image_indx = moonAge * 24 / 30; // We just have 24 images and not 30, do the conversion
	WOMA_LOGManager_DebugMSGAUTO(TEXT("moon_image_indx: %d\n"), moon_image_indx);

    StringCchPrintf(moonFilename, sizeof(moonFilename), TEXT("engine/moon%d.jpg"), moon_image_indx);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("moonFilename: %s\n"), moonFilename);

    // Get sunRise in Local Time!:
    //-----------------------------------------------------------------------------
    double sunRise = astroClass->SunRise();			//UTC + offset Time: not GMT (at summer GMT = UTC + 1)
    sunRise -= astroClass->UTC_adjustment / 60;			//to convert "UTC + offset" to "GMT + offset": GMT  = UTC - UTC_adjustment;
    h_sunrise = floor(sunRise);						//Local Time
    m_sunrise = (sunRise - h_sunrise) * 60;			//Local Time

    // Get sunSet in Local Time!:
    //-----------------------------------------------------------------------------
    double sunSet = astroClass->SunSet();				//UTC + offset Time: not GMT (at summer GMT = UTC + 1)
    sunSet -= astroClass->UTC_adjustment / 60;	//to convert "UTC + offset" to "GMT + offset": GMT  = UTC - UTC_adjustment;
    h_sunset = floor(sunSet);						//Local Time
    m_sunset = (sunSet - h_sunset) * 60;			//Local Time

    // Far away Position: 405948 km Scale: 11,5  cm 
    // Closest Position : 357643 km Scale: 12,66 cm (12% bigger)
    // DI = 12756.28 km (The diameter of the earth at the equator)
    //
    #define earth_eq 6378.14f // Convert to km
    moonDistance = earth_eq * astroClass->calculateMoonDistance();
    moonScale = (float) moonDistance / 405948; //MAX Moon Dist. 405948 km

    //
    // Calc Sun / Moon position:
    //
    // Fix: latitude / longitude (Only for this calculations)
    if (latitude < 0) latitude += 360;
    if (latitude > 360) latitude -= 360;

    if (longitude < 0) longitude += 360;
    if (longitude > 360) longitude -= 360;

	astroClass->Initialize(latitude, longitude);	
	Calculate();
}

void InitWorld::Calculate() 
{
	ASSERT(astroClass);
	astroClass->updateTime ();
	astroClass->AstroCalculations (latitude, longitude);

	// Moon Values
	geometricMoonElevation = astroClass->GeometricMoonElevation;
	moonElevation = astroClass->moonElevation;
	MoonAz = astroClass->MoonAz;

	// Sun Values:
    geometricElevation = astroClass->GeometricElevation;
	SunElevation = astroClass->SunElevation;
	SunAzimuth = astroClass->SunAzimuth;
}

InitWorld::~InitWorld() {SAFE_DELETE(astroClass); CLASSDELETE();}

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

//#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
//#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

STRING InitWorld::getMyIp() 
{
IFSTREAM	fin;
STRING		wLOCAL_APPDATA = WOMA::APPDATA;
STRING		szFileName = wLOCAL_APPDATA + TEXT("myip.txt");

#define EXTERNAL_IP TEXT("https://myexternalip.com/raw")
//#define EXTERNAL_IP TEXT("https://myexternalip.com")

#ifdef USE_NETWORK
	DeleteFile(szFileName.c_str());
	//URLDownloadToFileA(LPUNKNOWN, _In_ LPCSTR, _In_opt_ LPCSTR, DWORD, _In_opt_ LPBINDSTATUSCALLBACK);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("Download: %s\n"), EXTERNAL_IP); // Note: Dont use DEBUG_MSG yet...
#if defined WINDOWS_PLATFORM
	HRESULT hr = URLDownloadToFile(NULL, EXTERNAL_IP, szFileName.c_str(), 0, NULL);
	if (hr != S_OK) 
#else
	if(!URLDownloadToFile(NULL, EXTERNAL_IP, szFileName.c_str(), 0, NULL))
#endif
	{
		_tprintf("Could not comunicate with whatismyipaddress.com Server!");
		return TEXT("");
	}
#endif

	STRING str="127.0.0.1";

#if defined NDEBUG || CORE_ENGINE_LEVEL == 8 || defined ANDROID_PLATFORM
	
    IFSTREAM fileIn(szFileName.c_str());
	if (!fileIn) 
		{ WomaMessageBox(TEXT("MyIp File not found, (use: #define USE_NETWORK) at core_engine_level.h"), (TCHAR*)szFileName.c_str()); return TEXT(""); }

	getline(fileIn, str);
	WOMA_LOGManager_DebugMSG("IP: %s\n", str.c_str()); // Note: Dont use DEBUG_MSG yet...
	fileIn.close();	// Close the file.
#endif

	return str;
}

TCHAR downloadFilename[100] ={0};
#pragma warning(disable: 4129)
bool InitWorld::getMyLocation(double* latitude, double* longitude, STRING ip)
{
	IFSTREAM	fin;
	STRING		wLOCAL_APPDATA = WOMA::APPDATA;
	STRING		szFileName = wLOCAL_APPDATA + TEXT("my.location");

	std::string database_filename = "GeoLite2-City.mmdb";	//program file <dir>
	GeoLite2PP::DB db(database_filename);
	CHAR ip_str[MAX_STR_LEN] = { 0 };
	#if defined UNICODE
		wtoa(ip_str, ip.c_str(), MAX_STR_LEN); // wchar ==> char
	#else
		strcpy_s(ip_str, sizeof(ip_str), ip.c_str());
	#endif
	std::string json = db.lookup(ip_str);

{
	std::size_t found_latitude = json.find("latitude");
	TCHAR Wbuffer[5000] = { 0 };
	atow(Wbuffer, (CHAR*)json.substr(found_latitude, 19).c_str(), 5000);
	WOMA::logManager->DEBUG_MSG((TCHAR*)Wbuffer);				//latitude" : 38.8691
	WOMA::logManager->DEBUG_MSG(TEXT("\n"));

	/*std::wstring*/STRING str_latitude_float = Wbuffer;
	std::size_t found_latitude_float = str_latitude_float.find(TEXT(":"));
	/*std::wstring*/STRING str_latitude = str_latitude_float.substr(found_latitude_float+2, str_latitude_float.length());

	float ulat = (float)_ttof(str_latitude.c_str());
	*latitude = ulat;
}

{
	std::size_t found_longitude = json.find("longitude");	
	TCHAR Wbuffer[5000] = { 0 };
	atow(Wbuffer, (CHAR*)json.substr(found_longitude, 20).c_str(), 5000);
	WOMA::logManager->DEBUG_MSG((TCHAR*)Wbuffer);				//longitude" : -9.0656
	WOMA::logManager->DEBUG_MSG(TEXT("\n"));

	/*std::wstring*/STRING str_logitude_float = Wbuffer;
	std::size_t found_logitude_float = str_logitude_float.find(TEXT(":"));
	/*std::wstring*/STRING str_logitude = str_logitude_float.substr(found_logitude_float+2, str_logitude_float.length());

	float ulongi = (float)_ttof(str_logitude.c_str());
	*longitude = ulongi;
}

	TCHAR Wbuffer[5000] = { 0 };
	atow(Wbuffer, (CHAR*)json.c_str(), 5000);
#if UNICODE
	WOMA::logManager->DEBUG_MSG((WCHAR*)Wbuffer);	//std::cout << json << std::endl;
	WOMA::logManager->DEBUG_MSG(L"\n");				// Add ENTER to finish the
#else
	WOMA::logManager->DEBUG_MSG((CHAR*)json.c_str());	//std::cout << json << std::endl;
	WOMA::logManager->DEBUG_MSG("\n");					// Add ENTER to finish the
#endif
	return true;
}

#endif

#define initWorld SystemHandle->m_Application->initWorld

#if CORE_ENGINE_LEVEL >= 7 && defined USE_ASTRO_CLASS
void InitializeCelestialInfoScreen(int x, int y)
//-----------------------------------------------------------------------------------------
{
	WOMA::logManager->DEBUG_MSG("InitializeCelestialInfoScreen...");

	TCHAR str[50];

	float LINE = MIN(25, 2 * SystemHandle->fontSizeY);
	float LINE_SPACE = MIN(35, 3 * SystemHandle->fontSizeY);

	if (initWorld) // Needed because paint is called before create initWorld
	{
		// Make sure that is Reset!
		while (!SystemHandle->TextToPrint[1].empty())
			SystemHandle->TextToPrint[1].pop_back();

		// CelestialInfo:
#ifdef USE_NETWORK //at: main.h
		STRING infomode = TEXT("online");
#else
		STRING infomode = TEXT("offline");
#endif

		StringCchPrintf(str, sizeof(str), TEXT("CELESTIAL INFO: %s"), infomode.c_str());
		Woma_Label text = { str, x, y };
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Latitude: %f"), initWorld->latitude);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Longitude: %f"), initWorld->longitude);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		// Static Sun Values:
		// ------------------
		text.y += 30;
		StringCchPrintf(str, sizeof(str), TEXT("Sunrise: %d:%d"), (UINT)initWorld->h_sunrise, (UINT)initWorld->m_sunrise);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Sunset: %d:%d"), (UINT)initWorld->h_sunset, (UINT)initWorld->m_sunset);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		// Static Moon Values:
		// -------------------
		text.y += 30;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Distance: %f (km)"), initWorld->moonDistance);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Scale: %f / 1.0"), initWorld->moonScale);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		// Dynamic Sun Values:
		// -------------------
		text.y += 30;
		StringCchPrintf(str, sizeof(str), TEXT("Sun Geometric Elevation: %f"), initWorld->geometricElevation);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Elevation: %f (with refraction)"), (initWorld->geometricMoonElevation > -5) ? initWorld->moonElevation : NULL);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		text.y += (int)LINE;
		StringCchPrintf(str, sizeof(str), TEXT("Moon Azimuth: %f"), initWorld->MoonAz);
		text.label = str;
		SystemHandle->TextToPrint[1].push_back(text);

		WOMA::logManager->DEBUG_MSG(" done\n");
	}
}
#endif

