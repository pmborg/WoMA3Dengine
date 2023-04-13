// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: initWorld.h
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

#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include "main.h" //STRING

class InitWorld
{
public:
    UINT WomaIntegrityCheck = 1234567830;
	InitWorld();
	~InitWorld();

	void Frame();

private:
	STRING getMyIp();
	bool getMyLocation(double* latitude, double* longitude, STRING ip);

public:
    STRING LatDir;
    STRING LonDir;

	double latitude;
	double longitude;

    // Sun:
    double h_sunrise;
    double m_sunrise;

    double h_sunset;
    double m_sunset;

    double geometricElevation, SunElevation, SunAzimuth;

    // Moon:
	double moonDistance;
    float moonScale;
    UINT moon_image_indx;
	TCHAR moonFilename[MAX_STR_LEN];

    double geometricMoonElevation, moonElevation, MoonAz;
};

