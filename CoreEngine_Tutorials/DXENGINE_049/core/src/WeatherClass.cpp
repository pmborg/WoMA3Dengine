// --------------------------------------------------------------------------------------------
// Filename: WeatherClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
//   Get the nearest airport, based on our IP.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "OSengine.h"
#if CORE_ENGINE_LEVEL >= 9 && defined USE_METARCLASS //ENGINE_LEVEL >= 13
#include "main.h"
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include "weatherClass.h"
#include "Math3D.h"			//sqrt
#include "TrigonometryMathClass.h"	//sim, cos table
#include "fileLoader.h"
//#include "log.h"

// Internet:
#if defined USE_NETWORK && defined WINDOWS_PLATFORM
#include <urlmon.h>
#pragma comment( lib, "urlmon.lib" )
#endif

#include <vector>
extern bool download(const std::string url, const std::string file);

#ifdef GENERATE
#define DATAFILE TEXT("engine/data/airports.dat")
#else
#define DATAFILE TEXT("engine/data/airports.metar")
#endif

WeatherClass::WeatherClass()
{
	CLASSLOADER();

    Metar = TEXT("");
    Initialize();
}

WeatherClass::~WeatherClass() {CLASSDELETE();}

typedef struct
{
    int id;
    TCHAR CityName[100];
    TCHAR InternationalCityName[100];
    TCHAR Country[100];
    TCHAR ICAO[100];
    float latitude;
    float longitude;
} Airport;

std::vector <Airport>  airportVec;

void WeatherClass::Initialize()
{
    IFSTREAM fileIn(WOMA::LoadFile(DATAFILE));    //Open file for Read : cpp
	if (!fileIn) {
        TCHAR TextMsg[MAX_STR_LEN];
		StringCchPrintf(TextMsg, sizeof(TextMsg), TEXT("WARNING: WeatherClass::File not found: %s"), DATAFILE);
        WomaMessageBox(DATAFILE, TextMsg);
    } //Warning only!

    STRING line;

#ifdef GENERATE
    OFSTREAM fileOut(DATAFILE);    //Open file for Write : cpp
    std::vector <STRING>  metarListVec;
    IFSTREAM fileMetarList("engine/data/metarList.txt");    //Open file for Read : cpp
    if (fileMetarList)
    {
        while (!fileMetarList.eof()) {
            getline(fileMetarList, line);		// Read all Line
            metarListVec.push_back(line);
        }
    }
#endif

    Airport airport;
    if (fileIn) 
    {
        TCHAR IATA[100]; //Discard IATA

        while (!fileIn.eof()) 
        {
            getline(fileIn, line);		// Read all Line
			TCHAR *oneLine = &line[0];
            _stscanf(oneLine, TEXT("%d %s %s %s %s %s %f %f"), &airport.id, &airport.CityName, &airport.InternationalCityName, &airport.Country, &IATA, &airport.ICAO, &airport.latitude, &airport.longitude); //swscanf
            
            airportVec.push_back(airport);

#ifdef GENERATE
            if (wcslen(airport.ICAO) > 1 && wcslen(IATA) > 1) // Discard non IATA or non ICAO aeroports
            {
                for (UINT i = 0; i < metarListVec.size(); i++)
                {
                    CW2T oneLine(metarListVec[i].c_str());	// Convert std::wstring -> _TCHAR

                    if (wcscmp(oneLine, airport.ICAO) == 0) 
                    {
                        airportVec.push_back(airport);
                        fileOut << line << endl;
                    }
                }
            }
#endif   

        }
    }

    fileIn.close();
}

TCHAR* WeatherClass::findClosestAirport(float latitude, float longitude) 
{
    UINT GoodIndx = 0;
    float minDist = FLT_MAX;
    
    for (UINT i = 0; i < airportVec.size(); i++)
    {
        float dist = distHaversine(latitude, longitude, airportVec[i].latitude, airportVec[i].longitude);
        if (dist < minDist)  {
            GoodIndx = i;
            minDist = dist;
        }
    }

	womalog(
		"Closest ICAO=%s  Lat=%f Lon=%f Dist=%f km\n",
		airportVec[GoodIndx].ICAO,
		airportVec[GoodIndx].latitude,
		airportVec[GoodIndx].longitude,
		minDist
	);

    if (GoodIndx >= airportVec.size()) {
        womalog(TEXT("Airports file not found!"));
		return TEXT("");
	}

    return airportVec[GoodIndx].ICAO;
}

float WeatherClass::distHaversine(float lat1, float  lon1, float  lat2, float  lon2)
{
    float R = 6371; // earth's mean radius in km
    float dLat = (lat2 - lat1) *0.0174532925f; //0.0174532925f (PI / 180.0f): Convert degrees to radians
    float dLon = (lon2 - lon1) *0.0174532925f;

	float a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1) * cos(lat2) * sin(dLon / 2) * sin(dLon / 2);
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float d = R * c;
    return d;
}

// Get Present Weather:
//
bool WeatherClass::GetPresentWeather(TCHAR* ICAO)
{
    IFSTREAM	fin;
    TCHAR		szFileName[80] = TEXT("present.weather");

#if (defined USE_NETWORK) || CORE_ENGINE_LEVEL == 9
    DeleteFile(WOMA::LoadFile (szFileName));

    TCHAR downloadFilename[100] = { 0 };
    //https://metar.vatsim.net/metar.php?id=LPPT
    //StringCchPrintf(downloadFilename, sizeof(downloadFilename), TEXT("http://%s=%s"), TEXT("www.allmetsat.com/pt/metar-taf/portugal-espanha.php?icao"), ICAO);
    //...
    //<b>METAR:< / b> LPPT 011430Z 32009KT 280V360 9999 FEW022 BKN029 24 / 17 Q1018
    //...

    StringCchPrintf(downloadFilename, sizeof(downloadFilename), TEXT("https://%s=%s"), TEXT("metar.vatsim.net/metar.php?id"), ICAO);
    //LPPT 011400Z 32008KT 290V360 9999 FEW022 BKN026 23/17 Q1019

    womalog("URL: %s\n", downloadFilename);

#if defined WINDOWS_PLATFORM
    HRESULT hr = URLDownloadToFile(NULL, downloadFilename, WOMA::LoadFile (szFileName), 0, NULL);
    if (hr != S_OK) 
#else
    if (!URLDownloadToFile(NULL, downloadFilename, WOMA::LoadFile(szFileName), 0, NULL))
#endif
    {
        womalog("Could not comunicate with %s.com Server!", downloadFilename);
        return false;
    }
#else
    UNREFERENCED_PARAMETER(ICAO);
#endif

    STRING line;
    IFSTREAM fileIn(WOMA::LoadFile (szFileName));
	if (!fileIn)
	{
#ifdef RELEASE
		STRING err = TEXT("File not found: "); err += szFileName;
		WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "));
		WomaFatalExceptionW((TCHAR*)err.c_str());
		return false;
#else
        return true;
#endif
	}

    int i = 0; // String Index
    while (!fileIn.eof()) {
        getline(fileIn, line);
        i = (int)line.find(ICAO); //look for "METAR:" at html file
        womalog("%s\n", line.c_str());
        if (i > 1)
            break;
    }

    fileIn.close();
    Metar = line;
    return true;
}

#endif

