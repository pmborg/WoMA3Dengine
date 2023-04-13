// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: WeatherClass.h
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
#pragma once

#include "main.h" // TCHAR

class WeatherClass
{
public:
    UINT WomaIntegrityCheck = 1234567830;
    WeatherClass();
    ~WeatherClass();

	void	Initialize();
    TCHAR*	findClosestAirport(float latitude, float longitude);
    bool	GetPresentWeather(TCHAR* ICAO);

private:
    float distHaversine(float lat1, float  lon1, float  lat2, float  lon2);

public:
    STRING	Metar;


};


