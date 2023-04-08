// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: MetarClass.cpp
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
// Decode the Dynamic METAR info, from the nearest airport.
// Source Code: (C) 2000-2016 http://git.xcsoar.org/cgit/jmw/xcsoar.git/plain/src/Weather/METARParser.cpp
// --------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS
#include "platform.h"

#include "metarClass.h"
#include <math.h>	/* exp */

Weather weather;

MetarClass::MetarClass()
{
    CLASSLOADER();
    WomaIntegrityCheck = 1234567829;
}

MetarClass::~MetarClass() { CLASSDELETE(); }

bool IsDigitASCII(TCHAR ch)
{
    return ch >= '0' && ch <= '9';
}

// Detects a token with exactly 4 letters
bool DetectICAOCodeToken(const TCHAR* token)
{
    if (_tcslen(token) != 4)
        return false;

    for (unsigned i = 0; i < 4; i++) {
        if ((token[i] >= 'A' && token[i] <= 'Z') ||
            (token[i] >= 'a' && token[i] <= 'z')) {
            // okay
        }
        else
            return false;
    }

    return true;
}

// Detects a token with exactly 6 digits and a Z (Zulu = UTC) at the end
bool DetectTimeCodeToken(const TCHAR* token)
{
    if (_tcslen(token) != 7)
        return false;

    return token[6] == TEXT('Z') || token[6] == TEXT('z');
}

bool ParseTimeCode(const TCHAR* token)
{
    TCHAR* endptr;
    unsigned time_code = (unsigned)_tcstod(token, &endptr);
    if (endptr == NULL || endptr == token)
        return false;

    weather.day_of_month = (int)(time_code / 10000);
    time_code -= weather.day_of_month * 10000;
    weather.hour = (int)(time_code / 100);
    time_code -= weather.hour * 100;
    weather.minute = time_code;

    return true;
}


// Detects a token with exactly 5 digits and MPS or KT at the end.
// If the wind direction varies VRB is also valid.
bool DetectWindToken(const TCHAR* token)
{
    unsigned length = (unsigned)_tcslen(token);

    if (length != 8 && length != 7)
        return false;

    if (!_tcsicmp(token + 5, TEXT("MPS")) == 0 && !_tcsicmp(token + 5, TEXT("KT")) == 0)
        return false;

    bool variable = (_tcsncicmp(token, TEXT("VRB"), 3) == 0);

    for (unsigned i = variable ? 3 : 0; i < 5; ++i)
        if (!IsDigitASCII(token[i]))
            return false;

    return true;
}

bool ParseWind(const TCHAR* token)
{
    //assert(DetectWindToken(token));

    // variable wind directions
    if (_tcsncicmp(token, TEXT("VRB"), 3) == 0)
        // parsing okay but don't provide wind
        return true;

    TCHAR* endptr;
    unsigned wind_code = (unsigned)_tcstod(token, &endptr);
    if (endptr == NULL || endptr == token)
        return false;

    unsigned bearing = (int)(wind_code / 100);
    wind_code -= bearing * 100;

    if (_tcsicmp(endptr, TEXT("MPS")) == 0)
        weather.windKMh = wind_code * 3600 / 1000;
    //parsed.wind.norm = fixed(wind_code);
    else if (_tcsicmp(endptr, TEXT("KT")) == 0)
        weather.windKMh = (UINT)(wind_code * 1.852);
    //parsed.wind.norm = Units::ToSysUnit(fixed(wind_code), Unit::KNOTS);
    else
        return false;

    weather.wind_bearing = bearing; // Angle::Degrees(bearing);
    weather.wind_available = true;
    return true;
}

// Detects a token with exactly 5 digits
bool DetectVisibilityToken(const TCHAR* token)
{
    if (_tcslen(token) != 4)
        return false;

    for (unsigned i = 0; i < 4; ++i)
        if (!IsDigitASCII(token[i]))
            return false;

    return true;
}

bool ParseVisibility(const TCHAR* token)
{
    TCHAR* endptr;
    weather.visibility = _tcstol(token, &endptr, 10);
    if (endptr == NULL || endptr == token)
        return false;

    weather.visibility_available = true;
    return true;
}

// Detects a CAVOK token
bool DetectCAVOK(const TCHAR* token)
{
    return (_tcslen(token) == 5 && _tcsicmp(token, TEXT("CAVOK")) == 0);
}


// Detects a token with two pairs of two digits seperated by a slash.
// If the temperatures are negative a 'M' is a valid prefix.

bool DetectTemperaturesToken(const TCHAR* token)
{
    unsigned length = (unsigned)_tcslen(token);

    bool minus_possible = true;
    bool divider_found = false;

    for (unsigned i = 0; i < length; i++) {
        if (IsDigitASCII(token[i]))
            continue;

        if (token[i] == TEXT('/')) {
            divider_found = true;
            minus_possible = true;
            continue;
        }

        if (minus_possible && (token[i] == TEXT('M') || token[i] == TEXT('m')))
            continue;

        return false;
    }

    return divider_found;
}

const TCHAR* ParseTemperature(const TCHAR* token, int& temperature)
{
    bool negative = (token[0] == TEXT('M') || token[0] == TEXT('m'));
    if (negative)
        token++;

    TCHAR* endptr;
    int _temperature = (int)_tcstod(token, &endptr);
    if (endptr == NULL || endptr == token)
        return NULL;

    if (negative)
        _temperature = -_temperature;

    temperature = _temperature;
    return endptr;
}

bool ParseTemperatures(const TCHAR* token)
{
    if ((token = ParseTemperature(token, weather.temperature)) == NULL)
        return false;

    if (*token != TEXT('/'))
        return false;

    token++;

    if ((token = ParseTemperature(token, weather.dew_point)) == NULL)
        return false;

    weather.temperatures_available = true;
    return true;
}

// Detects a token beginning with a 'T' and followed by 8 digits
static bool
DetectAdditionalTemperaturesToken(const TCHAR* token)
{
    if (_tcslen(token) != 9)
        return false;

    if (token[0] != TEXT('T') && token[0] != TEXT('t'))
        return false;

    for (unsigned i = 1; i < 9; ++i) {
        if (!IsDigitASCII(token[i]))
            return false;
    }

    return true;
}

bool ParseAdditionalTemperatures(const TCHAR* token)
{
    // Skip 'T'
    token++;

    TCHAR* endptr;
    long temperature_code = _tcstol(token, &endptr, 10);
    if (endptr == NULL || endptr == token)
        return false;

    int temperature = (int)(temperature_code / 10000);
    int dew_point = temperature_code - temperature * 10000;

    if (temperature >= 1000)
        temperature = -temperature + 1000;

    if (dew_point >= 1000)
        dew_point = -dew_point + 1000;

    weather.temperature = temperature / 10;
    weather.dew_point = dew_point / 10;
    weather.temperatures_available = true;
    return true;
}

// Detects a token beginning with either 'Q' or 'A' and followed by 4 digits 
static bool
DetectQNHToken(const TCHAR* token)
{
    unsigned length = (unsigned)_tcslen(token);

    // International style
    if (token[0] == TEXT('Q') || token[0] == TEXT('q'))
        return length <= 5 && length >= 4;

    // American style
    if (token[0] == TEXT('A') || token[0] == TEXT('a'))
        return length == 5;

    return false;
}

bool ParseQNH(const TCHAR* token)
{
    // International style (hPa)
    if (token[0] == TEXT('Q') || token[0] == TEXT('q')) {
        token++;

        TCHAR* endptr;
        unsigned hpa = (unsigned)_tcstod(token, &endptr);
        if (endptr == NULL || endptr == token)
            return false;

        weather.hpa = hpa;
        weather.hpa_available = true;
        return true;
    }

    // American style (inHg)
    if (token[0] == TEXT('A') || token[0] == TEXT('a')) {
        token++;

        TCHAR* endptr;
        unsigned inch_hg = (unsigned)_tcstod(token, &endptr);
        if (endptr == NULL || endptr == token)
            return false;


        weather.hpa = (UINT)(inch_hg / 0.0296133971008484);
        weather.hpa_available = true;

        return true;
    }

    return false;
}

// Detects a token beginning with 
bool DetectCLOUDToken(const TCHAR* token)
{
    unsigned length = (unsigned)_tcslen(token);

    if (
        (_tcsncicmp(token, TEXT("FEW"), 3) == 0) ||
        (_tcsncicmp(token, TEXT("SCT"), 3) == 0) ||
        (_tcsncicmp(token, TEXT("BKN"), 3) == 0) ||
        (_tcsncicmp(token, TEXT("OVC"), 3) == 0)
        )
        return length == 6;

    if (
        (_tcsncicmp(token, TEXT("VV"), 2) == 0)
        )
        return length == 5;

    return false;
}

bool ParseCLOUD(const TCHAR* token)
{
    if (_tcsncicmp(token, TEXT("FEW"), 3) == 0)
        weather.cloud_coverage = MAX(weather.cloud_coverage, (float)1 / 8);

    if (_tcsncicmp(token, TEXT("SCT"), 3) == 0)
        weather.cloud_coverage = MAX(weather.cloud_coverage, (float)3 / 8);

    if (_tcsncicmp(token, TEXT("BKN"), 3) == 0)
        weather.cloud_coverage = MAX(weather.cloud_coverage, (float)5 / 8);

    if (_tcsncicmp(token, TEXT("OVC"), 3) == 0)
        weather.cloud_coverage = 1;

    if (_tcsncicmp(token, TEXT("VV"), 2) == 0)
        weather.cloud_coverage = 1;

    if (
        (_tcsncicmp(token, TEXT("VV"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("FEW"), 3) == 0) ||
        (_tcsncicmp(token, TEXT("SCT"), 3) == 0) ||
        (_tcsncicmp(token, TEXT("BKN"), 3) == 0) ||
        (_tcsncicmp(token, TEXT("OVC"), 3) == 0)
        )
    {
        if (_tcsncicmp(token, TEXT("VV"), 2) == 0)
            token += 2;
        else
            token += 3;

        TCHAR* endptr;
        unsigned layer = (unsigned)_tcstod(token, &endptr);
        if (endptr == NULL || endptr == token)
            return false;

        if (weather.cloud_layer == 0)
            weather.cloud_layer = layer * 100;
        else
            weather.cloud_layer = MIN(weather.cloud_layer, layer * 100);
    }

    weather.clouds_available = true;

    return true;
}

// Detects a token beginning with 
bool DetectWEATHERToken(const TCHAR* token)
{
    //detail: http://www.met.tamu.edu/class/metar/metar-pg9-ww.html

    if (_tcsncicmp(token, TEXT("TS"), 2) == 0)
        return true;

    if (_tcsncicmp(token, TEXT("+"), 1) == 0)
        return true;

    if (_tcsncicmp(token, TEXT("-"), 1) == 0)
        return true;

    //PRECIPITATION:
    if (
        //RAIN
        (_tcsncicmp(token, TEXT("DZ"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("RA"), 2) == 0) ||

        //SNOW
        (_tcsncicmp(token, TEXT("SN"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("SG"), 2) == 0) ||

        (_tcsncicmp(token, TEXT("IC"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("PL"), 2) == 0) ||

        (_tcsncicmp(token, TEXT("GR"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("GS"), 2) == 0)
        )
        return true;

    //OBSCURATION
    if (
        /*
        (_wcsnicmp(token, _T("FU"), 2) == 0) ||
        (_wcsnicmp(token, _T("VA"), 2) == 0) ||
        (_wcsnicmp(token, _T("DU"), 2) == 0) ||
        (_wcsnicmp(token, _T("SA"), 2) == 0) ||
        (_wcsnicmp(token, _T("PY"), 2) == 0) ||
        */

        //http://pt.wikipedia.org/wiki/N%C3%A9voa_seca
        (_tcsncicmp(token, TEXT("HZ"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("PR"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("BR"), 2) == 0) ||
        (_tcsncicmp(token, TEXT("FG"), 2) == 0)
        )
        return true;

    //BAD WEATHER:
    /*
    if (
        (_wcsnicmp(token, _T("PO"), 2) == 0) ||
        (_wcsnicmp(token, _T("SQ"), 2) == 0) ||
        (_wcsnicmp(token, _T("FC"), 2) == 0) ||
        (_wcsnicmp(token, _T("SS"), 2) == 0) ||
        (_wcsnicmp(token, _T("DS"), 2) == 0)
        )
        return true;
    */

    return false;
}

bool ParseWEATHER(const TCHAR* token)
{
    bool plus = false, minus = false;

    if (_tcsncicmp(token, TEXT("PR"), 2) == 0) {
        token += 2;
    }

    if (_tcsncicmp(token, TEXT("+"), 1) == 0) {
        plus = true;
        token++;
    }

    if (_tcsncicmp(token, TEXT("-"), 1) == 0) {
        minus = true;
        token++;
    }

    // TS / TSRA / TSSN / TSPL / TSGS / 
    if (_tcsncicmp(token, TEXT("TS"), 2) == 0) {
        weather.thunderstorm = true;
        token += 2;
    }

    // SH / SHRA / SHSN / SHPL / SHGS / RADZ
    if ((_tcsncicmp(token, TEXT("SH"), 2) == 0) || (_tcsncicmp(token, TEXT("RA"), 2) == 0))
    {
        weather.rain_available = true;
        token += 2;
    }

    //PRECIPITATION:
    if (_tcsncicmp(token, TEXT("DZ"), 2) == 0)
        weather.rain = (float)2 / 6;

    if (_tcsncicmp(token, TEXT("RA"), 2) == 0)
        weather.rain = (float)5 / 6;

    if (weather.rain > 0)
    {
        if (plus)
            weather.rain += (float)1 / 6;

        if (minus)
            weather.rain -= (float)1 / 6;

        weather.rain_available = true;
    }


    //SNOW type:
    //1 snow grains.
    //2 flake
    //3 ice cristals
    //4 ice pellets: Ice pellets are a form of precipitation consisting of small, translucent balls of ice
    //5 Small Hail
    //6 hail

    // Note:
    // rand() % 100;         // v1 in the range 0 to 99

    //snow grains: very small(< 1 mm)
    if (_tcsncicmp(token, TEXT("SG"), 2) == 0) {
        weather.snow_size = (float)((rand() % 10) / 10);
        weather.snow_type = 1;
    }

    //flake
    if (_tcsncicmp(token, TEXT("SN"), 2) == 0) {
        weather.snow_size = 1;
        weather.snow_type = 2;
    }

    //ice cristals: 0.1-5 mm
    if (_tcsncicmp(token, TEXT("IC"), 2) == 0) {
        weather.snow_size = (float)((rand() % 51) / 10); //0.1 to 5
        weather.snow_type = 3;
    }

    //ice pellets: 2-5 mm
    if (_tcsncicmp(token, TEXT("PL"), 2) == 0) {
        weather.snow_size = (float)(2 + rand() % 4); //2 + 0..3
        weather.snow_type = 4;
    }

    //Small Hail: 2-5 mm
    if (_tcsncicmp(token, TEXT("GS"), 2) == 0) {
        weather.snow_size = (float)(2 + rand() % 4); //2 + 0..3
        weather.snow_type = 5;
    }

    //Hail: 5-150 mm
    if (_tcsncicmp(token, TEXT("GR"), 2) == 0) {
        weather.snow_size = (float)(5 + rand() % 146); //5 + 0..145
        weather.snow_type = 6;
    }

    if (weather.snow_type > 0)
        weather.snow_available = true;

    //OBSCURATION
    //http://pt.wikipedia.org/wiki/N%C3%A9voa_seca
    if (_tcsncicmp(token, TEXT("HZ"), 2) == 0)
        weather.haze = true;

    if (_tcsncicmp(token, TEXT("BR"), 2) == 0) //H visibility: > one kilometre
        weather.visibility = MAX(1000, weather.visibility);

    if (_tcsncicmp(token, TEXT("FG"), 2) == 0) //H visibility: < one kilometre
        weather.visibility = MIN(1000, weather.visibility);

    return true;
}


bool parseToken(TCHAR* token)
{
    // Parse day of month, hour and minute
    if (DetectTimeCodeToken(token)) {
        ParseTimeCode(token);
        return true;
    }

    // Parse Wind
    if (DetectWindToken(token)) {
        if (ParseWind(token))
            return true;
    }

    // Parse Visibility
    if (DetectVisibilityToken(token))
        if (ParseVisibility(token))
            return true;

    if (DetectCAVOK(token))
        weather.cavok = true;

    // Parse Temperatures: M02/M02
    if (DetectTemperaturesToken(token)) {
        if (ParseTemperatures(token))
            return true;

    }

    // Parse Temperatures: T10171017
    if (DetectAdditionalTemperaturesToken(token)) {
        if (ParseAdditionalTemperatures(token))
            return true;
    }

    // Parse QNH
    if (DetectQNHToken(token)) {
        if (ParseQNH(token))
            return true;
    }

    // Parse CLOUDs
    if (DetectCLOUDToken(token)) {
        if (ParseCLOUD(token))
            return true;
    }

    if (DetectWEATHERToken(token)) {
        if (ParseWEATHER(token))
            return true;
    }

    // Parse four-letter ICAO code
    if (DetectICAOCodeToken(token))
    {
        weather.icao_code = token;
        return true;
    }

    return false;
}

void resetWeather()
{
    weather.icao_code = TEXT("");

    weather.day_of_month = 0;
    weather.hour = 0;
    weather.minute = 0;  // Hour and minute of the METAR release time in UTC

    weather.wind_available = false;
    weather.visibility_available = false;
    weather.temperatures_available = false;
    weather.hpa_available = false;
    weather.clouds_available = false;
    weather.rain_available = false;
    weather.snow_available = false;

    //wind:
    weather.wind_bearing = 0;  //0..359
    weather.windKMh = 0;       //km/h

    //visibility:
    weather.visibility = 10000;    //0..9999
    weather.cavok = false;

    // temperatures:
    weather.temperature = 0;
    weather.dew_point = 0;

    //pressure:
    weather.hpa = 0;

    weather.thunderstorm = false;
    weather.haze = false;

    //clouds:
    weather.cloud_coverage = 0;   //0..1
    weather.cloud_layer = 0;       //ft

    //weather:
    weather.rain = 0;       //0..1

    //snow:
    weather.snow_size = 0;
    weather.snow_type = 0;    //1..6
}

bool MetarClass::Initialize(STRING Metar)
{
    resetWeather();

    // Setup the ID string.
    CHAR metar[100] = { 0 };
    wtoa(metar, Metar.c_str(), 100); // wchar ==> char

    // get ip from xml format:
    char* token;
    int i = 0;
    for (token = strtok(metar, " "); token != 0; token = strtok(NULL, " "), i++) {

        TCHAR metarToken[100] = { 0 }; atow(metarToken, token, 100);
        if (!parseToken(metarToken)) {
            //something else
        }
    }

    // Formula:
    // Temperature T / Dewpoint Td / Relative Humidity RH     
    // RH: =100*(EXP((17.625*TD)/(243.04+TD))/EXP((17.625*T)/(243.04+T))) 
    weather.relative_humidity = (UINT)(100 * (exp((17.625 * weather.dew_point) / (243.04 + weather.dew_point)) / exp((17.625 * weather.temperature) / (243.04 + weather.temperature))));

    return true;
}


