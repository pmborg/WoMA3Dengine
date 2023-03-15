// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: astroClass.cpp
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
//
//   This file will Calculate: (Based on longitude, latitude )
//   - The real moon phases (MoonAge)
//   - the positions of the "Moon" ("Moon Elevation" and "Moon Azimuth") 
//   - the positions of the "Sun" ("Geometric Elevation") in the sky.
//   - Calculate: "SunSet" "SunRise"
//   - Calculate "Moon Distance": (ans the aparent Size correction: "Moon Scale")
//
// --------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS
#include "platform.h"

#include "astroClass.h"

AstroClass::AstroClass() 
{
	CLASSLOADER();

	//public:
	UTC_adjustment=0; //in minutes

	//Values:
	GeometricElevation = SunElevation = SunAzimuth=NULL;
	GeometricMoonElevation = moonElevation = MoonAz=NULL;

	second=NULL;
	minute=NULL;
	hour=NULL;

#if defined USE_FORCE_MIN
	initial_force_minute = USE_FORCE_MIN;
#else
	initial_force_minute = 0;
#endif

#if defined USE_FORCE_HOUR
	initial_force_hour = USE_FORCE_HOUR;
#else
	initial_force_hour = 0;
#endif

	day=NULL;
	month=NULL;
	year=NULL;

	//private:
	JD = DP = IP=NULL;
	declin = eqTime=NULL;
	offset = time_offset=NULL;
	m_LocalTime=NULL;

	latitude = NULL;
	longitude = NULL;

	ASSERT (MoonAge( 5, 9, 2013) == 30); // ag = 30: NEW MOON
	ASSERT (MoonAge(20, 9, 2013) == 15); // ag = 15: FULL MOON
}

void AstroClass::Initialize(double lati, double longi)
{
	// Input Values:
	latitude = lati;
	longitude= -longi;	// Fix longitude:

	updateTime ();
}

AstroClass::~AstroClass(){CLASSDELETE();}

// ============================================================================================================
// SUN:
// ============================================================================================================

// solar azimuth angle for sunrise and sunset corrected for atmospheric refraction (in degrees),
double AstroClass::hourAngleRiseSet() 
{
 double K = PI/180;
 double hars = cos(K*90.833)/(cos(K*latitude) * cos(K*declin)) - tan(K*latitude) * tan(K*declin);
 hars = acos(hars)/K;

 return hars;
}

// normalize values to range 0...1    
double normalize( double v_ )
{
    double v = v_ - floor( v_  ); 
    if( v < 0 )
        v = v + 1;
        
    return v;
}

void AstroClass::updateTime() 
{

	// Calculate the current time offset:
	time_t currentTime = time(NULL);
	m_LocalTime = localtime(&currentTime);

	second = m_LocalTime->tm_sec;

#if defined USE_FORCE_MIN
	minute = initial_force_minute;
#else
	minute = m_LocalTime->tm_min;
#endif

#if defined USE_FORCE_HOUR
	hour = initial_force_hour;
#else
	hour = m_LocalTime->tm_hour;
#endif

	day = m_LocalTime->tm_mday;
	month = m_LocalTime->tm_mon + 1;
	year = 1900 + m_LocalTime->tm_year;

    TIME_ZONE_INFORMATION TimeZoneInfo;
    DWORD rcd = GetTimeZoneInformation(&TimeZoneInfo);

	if (rcd == TIME_ZONE_ID_DAYLIGHT) // are we in summer time?
		UTC_adjustment = TimeZoneInfo.DaylightBias; //UTC = GMT + UTC_adjustment (in minutes);

	// Get TimeZone on GMT
	offset = -(double(UTC_adjustment) / 60);

	eqTime = equationTime();									// Calculate: equationTime: need (offset)
    time_offset = eqTime - (4 * longitude) - (60 * offset);		// Calculate: time_offset: need (eqTime and offset)
	declin = declination();										// Calculate: declination: need (offset not time_offset!)
}

// Sunrise (local time)
double AstroClass::SunRise() 
{
 double sun_rise = 720 + 4*(longitude-hourAngleRiseSet() ) - eqTime;
 sun_rise = sun_rise/60 + offset;

 return sun_rise;
}

// Sunset (local time)
double AstroClass::SunSet() 
{
 double sun_set = 720 + 4*(longitude+hourAngleRiseSet() ) - eqTime;
 sun_set = sun_set/60 + offset;

 return sun_set;
}

// declination (in degrees)
double AstroClass::declination() 
{
 double x = doy()*2 * PI/365; // fractional year in radians
 declin = 0.006918-0.399912 * cos(x)+0.070257 * sin(x)-0.006758 * cos(2*x);
 declin = declin+0.000907 * sin(2*x)-0.002697 * cos(3*x)+0.00148 * sin(3*x);
 declin = declin*180/PI;

 return declin;
}

//Calc DayOfYear
double AstroClass::doy()
{
	//EXAMPLE: 1/1/2013: N=1
	double N1 = floor((double)275 * month / 9);
	double N2 = floor((double)(month + 9) / 12);
	double N3 = (1 + floor((double) (year - 4 * floor((double) year / 4) + 2) / 3));
	double N = N1 - (N2 * N3) + day - 30.0;

	//add time to DoY
	double h= m_LocalTime->tm_hour + offset + m_LocalTime->tm_min/60;
	N = N - 1+(h-12)/24;

	return N;
}

// eqtime: equation of time (in minutes)
double AstroClass::equationTime() {
    double x = doy() * 2 * PI / 365;
    double eqtime = 229.18*(0.000075 + 0.001868 * cos(x) - 0.032077 * sin(x) - 0.014615 * cos(2 * x) - 0.040849 * sin(2 * x));

	return eqtime;
}

// tst: true solar time (in hours)
double AstroClass::solarTime() {

    return m_LocalTime->tm_hour * 60 + m_LocalTime->tm_min + time_offset;
}

//ha
double AstroClass::hourAngle() {
    return solarTime() / 4 - 180;
}

// solar zenith angle (in degrees)
double Phi;
double AstroClass::altitudeAngle()
{
    double K = PI / 180;
    Phi = sin(K*latitude) * sin(K*declin) + cos(K*latitude) * cos(K*declin) * cos(K * hourAngle());
    Phi = acos(Phi) / K;	//zenithAngle
    return 90 - Phi;		// altitude
}

// solar azimuth angle (in degrees, clockwise from north)
double AstroClass::azimuthAngle()
{
    double K = PI / 180;
    double Theta = -(sin(K*latitude) * cos(K*Phi) - sin(K*declin)) / (cos(K*latitude) * sin(K*Phi));
    Theta = acos(Theta) / K;
    if (hourAngle() < 0) Theta = Theta;
    else Theta = 360 - Theta;

	return Theta;
}

// ============================================================================================================
// MOON:
// ============================================================================================================
double AstroClass::JulianDate(int d, int m, int y)
{
	int mm, yy;
	int k1, k2, k3;
	int j;

	yy = y - (int)((12 - m) / 10);
	mm = m + 9;
	if (mm >= 12)
	{
		mm = mm - 12;
	}
	k1 = (int)(365.25 * (yy + 4712));
	k2 = (int)(30.6001 * mm + 0.5);
	k3 = (int)((int)((yy / 100) + 49) * 0.75) - 38;
	// 'j' for dates in Julian calendar:
	j = k1 + k2 + d + 59;
	if (j > 2299160)
	{
		// For Gregorian calendar:
		j = j - k3;  // 'j' is the Julian date at 12h UT (Universal Time)
	}

	JD = j;
	return JD;
}

double AstroClass::MoonAge(int day, int month, int year)
{        
	double ag;
	int j = (int)JulianDate(day, month, year);

	//Calculate the approximate phase of the moon
	double ip = (j + 4.867) / 29.530588853;
	ip = ip - floor(ip);
	IP = ip * 2 * PI;       // Convert phase to radians

	if(ip < 0.5)
		ag = ip * 29.530588853 + 29.530588853 / 2;
	else
		ag = ip * 29.530588853 - 29.530588853 / 2;

	// Moon's age in days
	ag = floor(ag) + 1;		// convert ag from :"true double age" to "int" age
    WOMA_LOGManager_DebugMSGAUTO(TEXT("Moon Age test: %f\n"), (float)ag);
	return ag; // int. value between 1 and 30
}

double AstroClass::calculateMoonDistance()
{
    // calculate moon's distance
    DP = 2*PI * normalize( ( JD - 2451562.2 ) / 27.55454988 );
    double DI = 60.4 - 3.3*cos( DP ) - 0.6 * cos( 2*IP - DP ) - 0.5 * cos( 2*IP );

	return DI;
}

double Radians(double number)
{
    return number * PI / 180;
}

double Deg(double number)
{
    return number * 180 / PI;
}

double ElevationRefraction(double El_geometric)
{

    double El_observed;
    double x, a0, a1, a2, a3, a4;

    a0 = 0.58804392;
    a1 = -0.17941557;
    a2 = 0.29906946E-1;
    a3 = -0.25187400E-2;
    a4 = 0.82622101E-4;

    El_observed = El_geometric;

    x = abs(El_geometric + 0.589);
    double refraction = abs(a0 + a1*x + a2*x*x + a3*x*x*x + a4*x*x*x*x);

    if (El_geometric > 10.2)
        El_observed = El_geometric + 0.01617*(cos(Radians(abs(El_geometric))) / sin(Radians(abs(El_geometric))));
    else
        El_observed = El_geometric + refraction;

    return(El_observed);
}

double Rev(double number)
{
    return number - floor(number / 360.0) * 360;
}


double Rev2(double number)
{
    double x = number - floor(number / 360.0) * 360;
    if (x > 180) x = x - 360;

    return x;
}

double sunangles[24];

/*
sun_angles:
document.Angles.SunAzimuth.value=formatvalue(output_angles[1],8);

sunangles[2] = Decl;
sunangles[3] = sunlon;
sunangles[4] = RA;
sunangles[5] = Rev(GMST0);
sunangles[6] = Rev(M);
sunangles[7] = Rev(w);
sunangles[8] = Rev(e);
sunangles[9] = Rev(oblecl);
sunangles[10] = GeometricElevation;
sunangles[11] = L;

document.Angles.SunDeclination.value=formatnumber(output_angles[2],3);
document.Angles.SunLongitude.value=formatnumber(output_angles[3],3);
document.Angles.RA.value=formatnumber(output_angles[4],3);
document.Angles.GMST0.value=formatnumber(output_angles[5],3);
document.Angles.MeanAnomaly.value=formatnumber(output_angles[6],3);
document.Angles.Perihelion.value=formatnumber(output_angles[7],3);
document.Angles.Eccentricity.value=formatnumber(output_angles[8],3);
document.Angles.Obliquity.value=formatnumber(output_angles[9],3);
document.Angles.SunGeometricElevation.value=formatnumber(output_angles[10],3);
*/

void AstroClass::sun_angles(double d, double SiteLon, double SiteLat)
{
    double HourAngle, SIDEREALTIME;
    double w, a, e, M, L, oblecl, E, x, y, r, v, sunlon, z, xequat, yequat, zequat, RA, Decl, GMST0, UT;
    double xhor, yhor, zhor;

    if (SiteLat < 0) SiteLat = -SiteLat;
    if (SiteLon < 0) SiteLon = 360 - SiteLon;

    //*********CALCULATE SUN DATA *********************
    w = 282.9404 + 4.70935E-5 * d;   //Sun Perihelion: "sunangles[7]"...ok
    a = 1;

    e = 0.016709 - 1.151E-9 *d;
    M = 356.0470 + 0.9856002585 * d; //Sun Mean Anomaly [6]...ok
    oblecl = 23.4393 - 3.563E-7 * d; //Obliquity: sunangles[9]...ok
    L = w + Rev(M);

    L = Rev(L);

    E = M + (180 / PI)*e * sin(Radians(M))*(1 + e * cos(Radians(M)));
    E = Rev(E);  // OK
    x = a*cos(Radians(E)) - e;
    y = a*sin(Radians(Rev(E)))*sqrt(1 - e*e);
    r = sqrt(x*x + y*y);
    v = Deg(atan2(y, x));
    sunlon = Rev(v + w);  // SunLongitude: sunangles[3]...ok (170.9xx)

    x = r*cos(Radians(sunlon));
    y = r*sin(Radians(sunlon));
    z = 0;

    xequat = x;
    yequat = y*cos(Radians(oblecl)) + z*sin(Radians(oblecl));
    zequat = y*sin(Radians(oblecl)) + z*cos(Radians(oblecl));

    RA = Rev(Deg(atan2(yequat, xequat)));							 // Right Ascention
    Decl = Deg(atan2(zequat, sqrt(xequat*xequat + yequat*yequat)));  // SunDeclination

    GMST0 = (L + 180);

    //*********CALCULATE TIME *********************
    UT = d - floor(d);  //ok...

    SIDEREALTIME = GMST0 + UT * 360 + SiteLon;  // !!!!!!!! SiteLon !!!!!!!!!!!!!
    HourAngle = SIDEREALTIME - RA;  


    x = cos(HourAngle*PI / 180) * cos(Decl*PI / 180);
    y = sin(HourAngle*PI / 180) * cos(Decl*PI / 180);
    z = sin(Decl*PI / 180);

    xhor = x*sin(SiteLat*PI / 180) - z*cos(SiteLat*PI / 180);
    yhor = y;
    zhor = x*cos(SiteLat*PI / 180) + z*sin(SiteLat*PI / 180);

    // Geocentric coordinates ??
    double EarthLon, EarthLat;
    EarthLat = Deg(atan2(z, sqrt(x*x + y*y)));  // trolig OK

    EarthLon = Rev(0 * 180 + RA - GMST0 - UT * 360);

    //1st:
    SunElevation = Deg(asin(zhor));  // ok
    GeometricElevation = SunElevation;
    SunElevation = ElevationRefraction(SunElevation); // atmospheric refraction

    //2nd:
    SunAzimuth = Deg(atan2(yhor, xhor));

    sunangles[0] = SunElevation;

    if (SiteLat < 0) sunangles[1] = SunAzimuth + 180;  // added 1 May 2008
    else sunangles[1] = SunAzimuth + 180;

    sunangles[2] = Decl;        //SunDeclination...ok
    sunangles[3] = sunlon;      //SunLongitude...ok
    sunangles[4] = RA;          //Sun Right Ascension...ok
    sunangles[5] = Rev(GMST0);  //Greenwich Mean Sidereal Time 
    sunangles[6] = Rev(M);      //MeanAnomaly
    sunangles[7] = Rev(w);      //Perihelion
    sunangles[8] = Rev(e);      //Eccentricity
    sunangles[9] = Rev(oblecl); //Obliquity
    sunangles[10] = GeometricElevation; //SunGeometricElevation
    sunangles[11] = L;           // Suns mean longitude

    if (SiteLat < 0) sunangles[14] = Rev(360 - HourAngle);
    else sunangles[14] = Rev(HourAngle - 180);
    /*
    sunangles[15]=Rev( ((gcHA)*180/PI)-180);

    sunangles[16]=topRA*180/PI;

    sunangles[17]=topDecl*180/PI;
    sunangles[18]=gclat*180/PI;
    */
    sunangles[19] = Rev(EarthLon);
    sunangles[20] = EarthLat;

    sunangles[21] = Rev2(HourAngle);

	SunAzimuth = sunangles[1];
}

double Div(int a, int b)
{
    return((a - a%b) / b);  //OK
}

double daynumber(double Day,double Month,double Year,double Hour,double Minute,double Second)
{
    double d = 367 * Year - Div((int) (7 * (Year + Div((int) (Month + 9), 12))), 4) + Div((int) (275 * Month), 9) + Day - 730530;
	d=d+ Hour/24 + Minute/(60*24) + Second/(24*60*60);   // OK

	return d;
}


double AstroClass::AstroCalculations(double SiteLat, double SiteLon)
{
    /*
    day = 13;
    month = 9;
    year = 2013;
    hour = 29;
    minute = 20;
    second = 0;
    */
    double d = daynumber(day, month, year, hour+UTC_adjustment/60, minute, second);

    //*********CALCULATE Moon DATA *********************
    double N = 125.1228 - 0.0529538083*d;
    double i = 5.1454;

    double w = 318.0634 + 0.1643573223 * d;   //OK
    double a = 60.2666;

    double e = 0.054900;
    double M = 115.3654 + 13.0649929509 * d;

    w = Rev(w); //Perihelion
    M = Rev(M);	//MeanAnomaly
    N = Rev(N);

    double E = M + (180 / PI)*e * sin(Radians(M))*(1 + e * cos(Radians(M)));
    E = Rev(E);  // OK

    double Ebeforeit = E;
    // now iterate until difference between E0 and E1 is less than 0.005_deg
    // use E0, calculate E1

    double Iterations = 0;
    double E_error = 9;

    double E0, E1, Eafterit, E_ErrorBefore;
    while ((E_error > 0.0005) && (Iterations < 20))  // ok - itererer korrekt
    {
        Iterations = Iterations + 1;
        E0 = E;
        E1 = E0 - (E0 - (180 / PI)*e * sin(Radians(E0)) - M) / (1 - e * cos(Radians(E0)));
        E = Rev(E1);

        Eafterit = E;

        if (E < E0) E_error = E0 - E;
        else E_error = E - E0;

        if (E < Ebeforeit) E_ErrorBefore = Ebeforeit - E;
        else E_ErrorBefore = E - Ebeforeit;

        //window.status = "Iterations=" + Iterations + " Moon eccentric anomaly error before iterations=" + formatvalue(E_ErrorBefore, 7) + "_deg after iterations=" + E_error + "_deg";
        //if (Iterations > 10) window.status = "Number of Iterations more than 10=" + Iterations + " Ebefore=" + Ebeforeit + " Eafter=" + Eafterit + " E_errorbefore=" + formatvalue(E_ErrorBefore, 7) + " E_errorafter" + E_error;
    }

    double x = a*(cos(Radians(E)) - e);
    double y = a*sin(Radians(Rev(E)))*sqrt(1 - e*e);
    double r = sqrt(x*x + y*y);
    double v = Deg(atan2(y, x));

    //double sunlon = Rev(v + w);  // will be ignored?

    //x = r*cos(Radians(sunlon));
    //y = r*sin(Radians(sunlon));
    double z = 0;

    double xeclip = r*(cos(Radians(N))*cos(Radians(v + w)) - sin(Radians(N)) * sin(Radians(v + w))*cos(Radians(i)));
    double yeclip = r*(sin(Radians(N))*cos(Radians(v + w)) + cos(Radians(N)) * sin(Radians(v + w))*cos(Radians(i)));
    double zeclip = r*sin(Radians(v + w))*sin(Radians(i));

    double moon_longitude = Rev(Deg(atan2(yeclip, xeclip)));   // OK
    double moon_latitude = Deg(atan2(zeclip, sqrt(xeclip*xeclip + yeclip*yeclip)));  // trolig OK

    // Now add Perbutations, we actually need Sun mean longitude and Suns mean anomaly
    sun_angles(d, SiteLon, SiteLat);

    double Ls = sunangles[11]; // Suns mean longitude  er feil
    double Ms = sunangles[6]; // Suns mean anomaly
    double Mm = Rev(M);            // Moons mean anomaly
    double Lm = Rev(N + w + M); // moon mean longitude
    double D = Rev(Lm - Ls); //Moons mean elongation
    double F = Rev(Lm - N);  //Moons argument of latitude

    // Perbutations Moons Longitude
    double P_lon1 = -1.274 * sin(Radians(Mm - 2 * D));  //  (Evection)
    double P_lon2 = +0.658 * sin(Radians(2 * D));     //    (Variation)
    double P_lon3 = -0.186 * sin(Radians(Ms));    //    (Yearly equation)
    double P_lon4 = -0.059 * sin(Radians(2 * Mm - 2 * D));
    double P_lon5 = -0.057 * sin(Radians(Mm - 2 * D + Ms));
    double P_lon6 = +0.053 * sin(Radians(Mm + 2 * D));
    double P_lon7 = +0.046 * sin(Radians(2 * D - Ms));
    double P_lon8 = +0.041 * sin(Radians(Mm - Ms));
    double P_lon9 = -0.035 * sin(Radians(D));     //      (Parallactic equation)
    double P_lon10 = -0.031 * sin(Radians(Mm + Ms));
    double P_lon11 = -0.015 * sin(Radians(2 * F - 2 * D));
    double P_lon12 = +0.011 * sin(Radians(Mm - 4 * D));

    // Perbutations Moons Latitude
    double P_lat1 = -0.173 * sin(Radians(F - 2 * D));
    double P_lat2 = -0.055 * sin(Radians(Mm - F - 2 * D));
    double P_lat3 = -0.046 * sin(Radians(Mm + F - 2 * D));
    double P_lat4 = +0.033 * sin(Radians(F + 2 * D));
    double P_lat5 = +0.017 * sin(Radians(2 * Mm + F));

    double P_lon = P_lon1 + P_lon2 + P_lon3 + P_lon4 + P_lon5 + P_lon6 + P_lon7 + P_lon8 + P_lon9 + P_lon10 + P_lon11 + P_lon12;
    double P_lat = P_lat1 + P_lat2 + P_lat3 + P_lat4 + P_lat5;
    double P_moondistance = -0.58 * cos(Radians(Mm - 2 * D)) - 0.46 * cos(Radians(2 * D));

    moon_longitude = moon_longitude + P_lon;
    moon_latitude = moon_latitude + P_lat;
    r = r + P_moondistance;

    // get the Eliptic coordinates
    double xh = r*cos(Radians(moon_longitude))*cos(Radians(moon_latitude));
    double yh = r*sin(Radians(moon_longitude))*cos(Radians(moon_latitude));
    double zh = r*sin(Radians(moon_latitude));

    // rotate to rectangular equatorial coordinates
    double xequat = xh;
    double yequat = yh*cos(Radians(sunangles[9])) - zh*sin(Radians(sunangles[9]));
    double zequat = yh*sin(Radians(sunangles[9])) + zh*cos(Radians(sunangles[9]));

    // now calculate RA & Decl
    double Moon_RA = Rev(Deg(atan2(yequat, xequat)));   // OK
    double Moon_Decl = Deg(atan2(zequat, sqrt(xequat*xequat + yequat*yequat)));  // trolig OK

    double GMST0 = (Ls + 180); //...ok

    //*********CALCULATE TIME *********************
    double UT = d - floor(d);

    double SIDEREALTIME = GMST0 + UT * 360 + SiteLon;  // ok 
    double HourAngle = SIDEREALTIME - Moon_RA;  // trolig ok

    x = cos(HourAngle * PI / 180) * cos(Moon_Decl * PI / 180);
    y = sin(HourAngle * PI / 180) * cos(Moon_Decl * PI / 180);
    z = sin(Moon_Decl * PI / 180);

    double xhor = x * sin(SiteLat*PI / 180) - z * cos(SiteLat*PI / 180);
    double yhor = y;
    double zhor = x * (SiteLat * PI / 180) + z * sin(SiteLat*PI / 180);

    moonElevation = Deg(asin(zhor));  

    moonElevation = moonElevation - Deg(asin(1 / r * cos(Radians(moonElevation))));

    //1st:
    GeometricMoonElevation = moonElevation;
    moonElevation = ElevationRefraction(moonElevation); // atmospheric refraction

    //2nd
    double MoonAzimuth = Deg(atan2(yhor, xhor));

    if (SiteLat < 0) MoonAz = MoonAzimuth + 180; // added 180 deg 
    else
    MoonAz = MoonAzimuth + 180;

    return moonElevation;
}

