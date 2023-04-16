// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: initWorld.cpp
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
//   Main Functions:
//	- Get IP and Location
//  - Get data from AstroClass
//
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#include "platform.h"

#include <GeoLite2PP.hpp>
#pragma comment(lib, "Ws2_32.lib")	//undefined reference to imp_getaddrinfo

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
#include "mem_leak.h"


// Internet:
	#include <urlmon.h>
	#pragma comment( lib, "urlmon.lib" )

	#if defined WINDOWS_PLATFORM //getMyIp
		#include <winsock2.h>
		#include <iphlpapi.h>
		#include <stdio.h>
		#include <stdlib.h>
		// Link with Iphlpapi.lib
		#pragma comment(lib, "IPHLPAPI.lib")
	#endif

// --------------------------------------------------------------------------------------------
// Globals:
// --------------------------------------------------------------------------------------------
// Init Sun Light Class:
AstroClass* astroClass = NULL;	

InitWorld::InitWorld ()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

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
		gotLocation = getMyLocation(&latitude, &longitude, ip);
		WOMA_LOGManager_DebugMSGAUTO(TEXT("gotLocation: true\n"));
	}
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
	Frame();
}

void InitWorld::Frame() 
{
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

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

STRING InitWorld::getMyIp() 
{
IFSTREAM	fin;
STRING		wLOCAL_APPDATA = WOMA::APPDATA;
STRING		szFileName = wLOCAL_APPDATA + TEXT("my.ip");

	DeleteFile(szFileName.c_str());

	WOMA_LOGManager_DebugMSGAUTO(TEXT("Download: http://myexternalip.com/raw\n")); // Note: Dont use DEBUG_MSG yet...
	HRESULT hr = URLDownloadToFile(NULL, TEXT("http://myexternalip.com/raw"), szFileName.c_str(), 0, NULL);
	if (hr != S_OK) {
		WomaFatalException("Could not comunicate with whatismyipaddress.com Server!");
		return TEXT("");
	}

	STRING str;
    IFSTREAM fileIn(szFileName.c_str());
	if (!fileIn) 
		{ WOMA::WomaMessageBox(TEXT("MyIp File not found, (use: #define USE_NETWORK) at core_engine_level.h"), (TCHAR*)szFileName.c_str()); return TEXT(""); }

	getline(fileIn, str);
	WOMA_LOGManager_DebugMSG("IP: %s\n", str.c_str()); // Note: Dont use DEBUG_MSG yet...
	fileIn.close();	// Close the file.

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

