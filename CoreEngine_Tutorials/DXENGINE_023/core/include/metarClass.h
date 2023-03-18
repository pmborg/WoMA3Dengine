// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: MetarClass.h
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

#include "main.h" // STRING

typedef struct
{
    STRING icao_code;

    UINT day_of_month;
    UINT hour, minute;  // "Hour" and "minute" of the METAR release time in UTC

    bool wind_available;
    bool visibility_available;
    bool temperatures_available;
    bool hpa_available;
    bool clouds_available;
    bool rain_available;
    bool snow_available;

    //wind: wind_available
    UINT wind_bearing;  //0..359
    UINT windKMh;       //km/h

    //visibility: visibility_available
    UINT visibility;    //0..9999
    bool cavok;

    // temperatures: temperatures_available
    int temperature;
    UINT relative_humidity;
    int dew_point;

    //pressure: hpa_available
    UINT hpa;

    //clouds: clouds_available
    float cloud_coverage;   //0..1
    UINT cloud_layer;       //ft

    //weather: rain_available
    float rain; //0..1

    //snow: snow_available
    float snow_size;
    UINT snow_type;

    //special cases:
    bool thunderstorm;
    bool haze;

} Weather;

extern Weather weather;

class MetarClass
{
public:
	MetarClass();
	~MetarClass();

	bool Initialize(STRING Metar);

private:

};

//extern MetarClass* metarClass;
