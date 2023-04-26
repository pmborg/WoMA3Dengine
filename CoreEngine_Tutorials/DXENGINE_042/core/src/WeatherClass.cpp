// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: WeatherClass.cpp
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
//   Get the nearest airport, based on our IP.
//
// --------------------------------------------------------------------------------------------

//#define GENERATE //(Airports List)


#define _CRT_SECURE_NO_WARNINGS
#include "platform.h"

#include "main.h"
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include "weatherClass.h"
#include "Math3D.h"			//sqrt
#include "TrigonometryMathClass.h"	//sim, cos table
#include "fileLoader.h"
#include "log.h"

// Internet:
#include <urlmon.h>
#pragma comment( lib, "urlmon.lib" )

#include <vector>

// --------------------------------------------------------------------------------------------
// Globals:
// --------------------------------------------------------------------------------------------

#ifdef GENERATE
#define DATAFILE TEXT("engine/data/airports.dat")
#else
#define DATAFILE TEXT("engine/data/airports.metar")
#endif

WeatherClass::WeatherClass()
{
    WomaIntegrityCheck = 1234567831;
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
    //1,"Goroka","Goroka","Papua New Guinea","GKA","AYGA",-6.081689,145.391881,5282,10,"U"
    //1638,"Lisboa","Lisbon","Portugal","LIS","LPPT",38.781311,-9.135919,374,0,"E"
    IFSTREAM fileIn(WOMA::LoadFile(DATAFILE));    //Open file for Read : cpp
	if (!fileIn) 
		{ WOMA::WomaMessageBox(DATAFILE, TEXT("File not found")); } //Warning only!

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
    float longi = (360 - longitude);
    if (longi >= 360) longi -= 360;
    if (longitude > 180) longi = -longi;

    //latitude, longi

    UINT GoodIndx = 0;
    float minDist = FLT_MAX;
    
    for (UINT i = 0; i < airportVec.size(); i++)
    {
        float dist = distHaversine(latitude, longi, airportVec[i].latitude, airportVec[i].longitude);
        if (dist < minDist)  {
            GoodIndx = i;
            minDist = dist;
        }
    }

    if (GoodIndx >= airportVec.size()) {
		WomaFatalException("Airports file not found!");
		return TEXT("");
	}

    return airportVec[GoodIndx].ICAO;
}

float WeatherClass::distHaversine(float lat1, float  lon1, float  lat2, float  lon2)
{
    float R = 6371; // earth's mean radius in km
    float dLat = (lat2 - lat1);// *0.0174532925f; //0.0174532925f (PI / 180.0f): Convert degrees to radians
    float dLon = (lon2 - lon1);// *0.0174532925f;

    float a = FAST_sin(dLat / 2) * FAST_sin(dLat / 2) + FAST_cos(lat1) * FAST_cos(lat2) * FAST_sin(dLon / 2) * FAST_sin(dLon / 2);
    float c = 2 * atan2(FAST_sqrt(a), FAST_sqrt(1 - a));
    float d = R * c;

	//Note: 
    //Use conversions only, for normal "trig. math" functions.

    return d;
}

// Get Present Weather:
//
bool WeatherClass::GetPresentWeather(TCHAR* ICAO)
{
    IFSTREAM	fin;
    TCHAR		szFileName[80] = TEXT("present.weather");

    DeleteFile(WOMA::LoadFile (szFileName));

    TCHAR downloadFilename[100] = { 0 };
    StringCchPrintf(downloadFilename, sizeof(downloadFilename), _T("http://%s=%s"), _T("www.allmetsat.com/pt/metar-taf/portugal-espanha.php?icao"), ICAO);

    HRESULT hr = URLDownloadToFile(NULL, downloadFilename, WOMA::LoadFile (szFileName), 0, NULL);
    if (hr != S_OK) {
        WomaFatalException("Could not comunicate with whatismyipaddress.com Server!");
        return false;
    }

    STRING line;
    IFSTREAM fileIn(WOMA::LoadFile (szFileName));
	if (!fileIn)
	{
#ifdef RELEASE
		STRING err = TEXT("File not found: "); err += szFileName;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "));
		WomaFatalExceptionW((TCHAR*)err.c_str());
		return false;
#else
        return true;
#endif
	}

    int i = 0; // String Index
    while (!fileIn.eof()) {
        getline(fileIn, line);
        i = (int) line.find(TEXT("METAR:")); //look for "METAR:" at html file
        if (i > 1)
            break;
    }

    fileIn.close();

    if (i > 1) {
		// Extract METAR MSG from html line:
		int end = (int) line.find(TEXT("<br><b>short"));
        Metar = line.substr(i+10, end);
		end = (int) Metar.find(TEXT("<br>"));
		Metar = Metar.substr(0, end);
	}

    return true;
}

