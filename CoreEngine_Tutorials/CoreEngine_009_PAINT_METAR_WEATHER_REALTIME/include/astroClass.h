// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: astroClass.h
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

#include "platform.h"
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include "Math3D.h"
#include "TrigonometryMathClass.h" //sim, cos table

#include <cmath>
#include <time.h>

class AstroClass
{
public:
	UINT WomaIntegrityCheck = 1234567890;
	AstroClass();
	~AstroClass();

	void Initialize(double lati, double longi);

	// Update
	void updateTime();

	// Sun Positions:
	double altitudeAngle();
	double azimuthAngle();

	// Sun Time Events:
	double SunRise();
	double SunSet();

	// Moon Phase:
	double MoonAge(int day, int month, int year);

	// Moon Position:
	double calculateMoonDistance();

	//MAIN:
    double AstroCalculations(double SiteLat, double SiteLon);

	double calculateMoonEclipticLatitude();
	double calculateMoonEclipticLongitude();
	double doy();

private:
	void sun_angles(double d, double SiteLon, double SiteLat);

	double hourAngleRiseSet();
	double declination();
	double equationTime();
	double solarTime();
	double hourAngle();

	double JulianDate(int d, int m, int y);

public:
	LONG UTC_adjustment; //in minutes

	//Values:
	double GeometricElevation, SunElevation, SunAzimuth;
	double GeometricMoonElevation, moonElevation, MoonAz;

	int second;
	int minute;
	int hour;

	int initial_force_minute;
	int initial_force_hour;

	int day;
	int month;
	int year;

	private:
	double JD, DP, IP;
	double declin, eqTime;
	double offset, time_offset;
	tm* m_LocalTime;

	double latitude;
	double longitude;

};

extern AstroClass* astroClass;

#if defined NOTES
double AstroClass::Calculate_UTC_SunEvent (bool rising)
{

	// sunrise_sunset_algorithm:
	// "rising": true: sunrise / false: sunset
	The Solar Zenith Angle, "θs" is estimated using results from spherical trigonometry
	Formula:		cos θs = sin φ                   * sin δ  + cos φ                   * cos δ  * cos h 
	Implementaion:	cos θs = sin (latitude/(180/PI)) * sinDec + sin (latitude/(180/PI)) * cosDec * cosH

	Where:
    θs is the solar zenith angle
    h is the "hour angle", in the local solar time.
    δ is the current declination of the Sun
    φ is the local latitude.
#endif

