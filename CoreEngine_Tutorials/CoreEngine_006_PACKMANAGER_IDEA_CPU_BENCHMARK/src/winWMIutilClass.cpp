// --------------------------------------------------------------------------------------------
// Filename: wmiUtilClass.cpp
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
// PURPOSE:
// --------------------------------------------------------------------------------------------

#include "platform.h"
#if CORE_ENGINE_LEVEL >= 4

#include "wmiUtilClass.h"

#if defined WINDOWS_PLATFORM

using namespace std;	//endl
#include <sstream>		//wstring

wmiUtilClass::wmiUtilClass() 
{
	CLASSLOADER();
    WomaIntegrityCheck = 1234567831;

	//public:
	description = L"";
	AdapterRAM	= NULL;
	AdapterDACType = L"";

	totalMemoryCapacity = NULL;

	//private:
    pLocator = NULL;
    pServices = NULL;
    
	#if DX_ENGINE_LEVEL >= 20 // Initializing Engine
    try {
        GetVideoControllerInfoFromWMI();
    }
    catch (...) 
    {
        WOMA_LOGManager_DebugMSGAUTO(TEXT("ERROR ON: GetVideoControllerInfoFromWMI()"));
    }
	#endif
}

wmiUtilClass::~wmiUtilClass() 
{
    SAFE_RELEASE (pServices);
    SAFE_RELEASE(pLocator);
    CoUninitialize();

	CLASSDELETE();
}

// serialize constant value T to STRING
template<typename T> inline STRING ToWideString(const T &X)
{
  #ifdef UNICODE
    std::wstringstream stream;
  #else
	std::stringstream stream;
  #endif

    stream << X;
    return stream.str();
}

bool wmiUtilClass::initWMI(STRING connectTo)
{
    // If anything fails down the line I just return an empty string and apply a fallback mechanism.
    // This type of function should never fail unless you're probing a non-existent piece of harwdare.

    // Initialize COM.
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
        return false;

    // Obtain initial locator to WMI.
    if (FAILED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&pLocator))))
    {
        CoUninitialize();
        return false;
    }

    // Connect to WMI.
	if (FAILED(pLocator->ConnectServer(_bstr_t(connectTo.c_str()), NULL, NULL, NULL, NULL, NULL, NULL, &pServices)))
    {
        SAFE_RELEASE(pLocator);
        CoUninitialize();
        return false;
    }

    // Set security levels on the proxy.
    if (FAILED(CoSetProxyBlanket(
        pServices,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE)))
    {
        SAFE_RELEASE(pServices);
        SAFE_RELEASE(pLocator);
        CoUninitialize();
        return false;
    }

    return true;
}

#define ConvertToCelsius(x) (x / 10 - 273.15)


//	-------------------------------------------------------------------------------------------
// https://msdn.microsoft.com/en-us/library/aa394493%28VS.85%29.aspx
//
// Note: Aplication MSVS need to run as admin: 
bool wmiUtilClass::GetCpuTemperature()
{
    CoUninitialize();

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) WomaFatalException("CoInitializeEx Failed!");

    hr = CoInitializeSecurity(
        NULL,
        -1,      // COM negotiates service                  
        NULL,    // Authentication services
        NULL,    // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication
        RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation
        NULL,             // Authentication info 
        EOAC_NONE,        // Additional capabilities
        NULL              // Reserved
    );

    if (!pServices) 
        if (!initWMI(TEXT("ROOT\\WMI"))) // cimv2
			{ WOMA::WomaMessageBox(TEXT("WARNING: GetCpuTemperature, failed!")); return false; }

	//WriteConsoleOutput(TEXT(""));

	// You can use also:
	// CMD LINE:	C:\WINDOWS\system32>wmic /namespace:\\root\wmi PATH MSAcpi_ThermalZoneTemperature get CurrentTemperature /value
	// WIN APP:		%windir%\system32\wbem\wbemtest.exe

    // Request WMI data:
    IEnumWbemClassObject* pEnumerator = NULL;
    if (FAILED(pServices->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM MSAcpi_ThermalZoneTemperature"),  // or Win32_TemperatureProbe
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,	// 32+16 = 48
        NULL,
        &pEnumerator)))
    {
        SAFE_RELEASE(pServices);
        SAFE_RELEASE(pLocator);
        CoUninitialize();
        WOMA::WomaMessageBox(TEXT("WARNING: GetCpuTemperature, failed!")); 
		return false;
    }

    VARIANT varProp;
	ULONG uReturn = 0;
    IWbemClassObject *pClassObj = NULL;

	while (pEnumerator != NULL)
    {
		pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &uReturn);
		if (uReturn == 0)
			break;

/*		[1]											  [2]	([3]	/[4] )
		--------------------------------------------------------------------------------------------
		ACPI\ThermalZone\CPUZ_0 | CurrentTemperature: 59.00 (59.00	/59.00)	<-- CPU TEMP
		ACPI\ThermalZone\GFXZ_0 | CurrentTemperature: 0.00	(0.00	/0.00)	<-- GPU TEMP
		ACPI\ThermalZone\EXTZ_0 | CurrentTemperature: 37.00 (37.00	/37.00)	<-- External FAN TEMP
		ACPI\ThermalZone\LOCZ_0 | CurrentTemperature: 44.00 (44.00	/44.00)	<-- 
		ACPI\ThermalZone\BATZ_0 | CurrentTemperature: 30.00 (30.00	/30.00) <-- Baterry TEMP
		ACPI\ThermalZone\PCHZ_0 | CurrentTemperature: 114.00(114.00	/114.00)<-- Chipset motherboard Component TEMP 
		PCH is the chipset component of your motherboard. 
		It tends to run a bit hotter than processors, especially at idle. 
		Anything above 80 Celsius is considered high for chipsets
		and beyond 90 Celsius is considered critical.
*/

		pClassObj->Get(L"InstanceName", 0, &varProp, NULL, NULL);			// 1 Teperature Description
		std::wstring instanceName = varProp.bstrVal;

		pClassObj->Get(L"CurrentTemperature", 0, &varProp, NULL, NULL);		// 2 CPU Temperature
		int temp = (int)ConvertToCelsius(varProp.intVal);

		pClassObj->Get(L"MinReadable", 0, &varProp, NULL, NULL);			// 3 Min Read
        int min_read = (int)ConvertToCelsius(varProp.intVal);

		pClassObj->Get(L"MaxReadable", 0, &varProp, NULL, NULL);			// 4 Max read
        int max_read = (int)ConvertToCelsius(varProp.intVal);

		WOMA_LOGManager_DebugMSGW(L"%s | CurrentTemperature: %02.2f (Min: %02.2f / Max: %02.2f)\n",
		instanceName.c_str(), temp, min_read, max_read);
	}

    SAFE_RELEASE(pServices);
    SAFE_RELEASE(pLocator);
    CoUninitialize();

    return true;
}


//	-------------------------------------------------------------------------------------------
// http://msdn.microsoft.com/en-us/library/aa394347%28v=vs.85%29.aspx
//
bool wmiUtilClass::GetTotalPhysicalMemory()
{
    if (!pServices) 
        if (!initWMI(TEXT("ROOT\\CIMV2"))) 
			{ WOMA::WomaMessageBox(TEXT("WARNING: GetTotalPhysicalMemory, failed!")); return false; }

    // Request WMI data.
    IEnumWbemClassObject* pEnumerator = NULL;
    if (FAILED(pServices->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_PhysicalMemory"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator)))
    {
        SAFE_RELEASE(pServices);
        SAFE_RELEASE(pLocator);
        CoUninitialize();
        WOMA::WomaMessageBox(TEXT("WARNING: GetTotalPhysicalMemory, failed!")); 
		return false;
    }

    VARIANT varProp;

    IWbemClassObject *pClassObj = NULL;
    ULONG uReturn = 0;

	while (pEnumerator != NULL)
    {
		pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &uReturn);
		if (uReturn == 0)
			break;

		pClassObj->Get(L"Capacity", 0, &varProp, NULL, NULL);
		std::wstring mem = varProp.bstrVal;
        totalMemoryCapacity += (float) _wtof(mem.c_str()) / ((float)GBs);
	}

    SAFE_RELEASE(pServices);
    SAFE_RELEASE(pLocator);
    CoUninitialize();

	if (totalMemoryCapacity == 0)
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		totalMemoryCapacity = memInfo.ullTotalPhys / ((float)GBs);
	}

    return true;
}
//#endif

bool wmiUtilClass::GetSystemInfo()
{
    if (!pServices) 
        if (!initWMI(TEXT("ROOT\\CIMV2"))) 
			{ WOMA::WomaMessageBox(TEXT("WARNING: GetTotalPhysicalMemory, failed!")); return false; }

    // Request WMI data.
    IEnumWbemClassObject* pEnumerator = NULL;
    if (FAILED(pServices->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_ComputerSystem"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator)))
    {
        SAFE_RELEASE(pServices);
        SAFE_RELEASE(pLocator);
        CoUninitialize();
        WOMA::WomaMessageBox(TEXT("WARNING: GetSystemInfo, failed!")); 
		return false;
    }

    VARIANT varProp;

    IWbemClassObject *pClassObj = NULL;
    ULONG uReturn = 0;

	while (pEnumerator != NULL)
    {
		pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &uReturn);
		if (uReturn == 0)
			break;

		pClassObj->Get(L"manufacturer", 0, &varProp, NULL, NULL);
		std::wstring computerManufacturer = varProp.bstrVal;

		pClassObj->Get(L"model", 0, &varProp, NULL, NULL);
		std::wstring computerModel = varProp.bstrVal;

		WOMA_LOGManager_DebugMSGW(L"Motherboard Manufacture: %s\n", computerManufacturer.c_str());
		WOMA_LOGManager_DebugMSGW(L"Motherboard Model: %s\n", computerModel.c_str());
	}

    SAFE_RELEASE(pServices);
    SAFE_RELEASE(pLocator);
    CoUninitialize();

    return true;
}
//#endif



//#ifdef TUTORIAL_PRE_CHAP// >= 5
//	-------------------------------------------------------------------------------------------
bool wmiUtilClass::GetVideoControllerInfoFromWMI()
{
    if (!pServices)
        if (!initWMI(TEXT("ROOT\\CIMV2"))) 
            return false;

    // Request WMI data.
    IEnumWbemClassObject* pEnumerator = NULL;
    if (FAILED(pServices->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_VideoController where DeviceID='VideoController1'"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator)))
    {
        SAFE_RELEASE(pServices);
        SAFE_RELEASE(pLocator);
        CoUninitialize();
        return false;
    }

    // MORE INFO: http://msdn.microsoft.com/en-us/library/aa394512%28v=vs.85%29.aspx
    VARIANT varProp;

    //QUERY: Win32_VideoController
    IWbemClassObject *pClassObj = NULL;
    ULONG uReturn = 0;
    pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &uReturn);
	pEnumerator->Release();

	HRESULT hr;
	if (pClassObj)
	{
		hr = pClassObj->Get(L"Description", 0, &varProp, NULL, NULL);
		description = varProp.bstrVal;

		hr = pClassObj->Get(L"AdapterRAM", 0, &varProp, NULL, NULL);
		if (varProp.intVal > 0)
			AdapterRAM = varProp.intVal / (MBs);
		else
			AdapterRAM = 0;

        // NOTE: In some old computers generate an exception:
		//hr = pClassObj->Get(L"AdapterDACType", 0, &varProp, NULL, NULL);
		//AdapterDACType = varProp.bstrVal;
	}

    SAFE_RELEASE(pClassObj);

    return true;
}

// Original Source code :
// http://stackoverflow.com/questions/6834156/descriptive-monitor-name-from-d3d-display-adapter-id
//
// Bug Fixed at iLoop by: Pedro Borges
STRING wmiUtilClass::GetMonitorDescriptonFromWMI(DWORD iMonitor)
{
    if (!pServices)
        initWMI(TEXT("ROOT\\CIMV2"));

    // Request WMI data.
    IEnumWbemClassObject* pEnumerator = NULL;
    if (FAILED(pServices->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_DesktopMonitor"),

        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator)))
    {
        SAFE_RELEASE(pServices);
        SAFE_RELEASE(pLocator);
        CoUninitialize();
        return TEXT("");
    }

    // Try to compile a correct description.
    std::wstring description;
    DWORD iLoop = 0; // Monitor index is 1-based.
    IWbemClassObject *pClassObj = NULL;
    while (pEnumerator != NULL)
    {
        ULONG uReturn = 0;
        if (pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &uReturn) == 0)
            break;  // Done (pClassObj remains NULL).

        // Is this the one we're looking for?
        if (iMonitor == iLoop)
        {
            VARIANT varProp;
            pClassObj->Get(L"Description", 0, &varProp, NULL, NULL); // Check the MSDN for Win32_DesktopMonitor to see what your options are!
            description = varProp.bstrVal;
            VariantClear(&varProp);
            SAFE_RELEASE(pClassObj);

            break;  // Done.
        }
        else
            SAFE_RELEASE(pClassObj);
    }

    SAFE_RELEASE(pServices);
    SAFE_RELEASE(pLocator);
    CoUninitialize();

    // With a bit of luck this string was just built.
	#ifdef UNICODE
		return description;
	#else
		CHAR desc[MAX_STR_LEN] = { 0 };
		WideCharToMultiByte(CP_ACP,0, description.c_str(),-1, desc, MAX_STR_LEN,NULL,NULL); //Note: Cant use: wtoa
		return desc;
	#endif
}

//	-------------------------------------------------------------------------------------------
STRING wmiUtilClass::GetMonitorDescription(HMONITOR hMonitor)
{
    MONITORINFOEX monInfoEx;
    monInfoEx.cbSize = sizeof(MONITORINFOEX);
    if (GetMonitorInfo(hMonitor, &monInfoEx))
    {
        // Get monitor index by matching ID.
        DWORD iDevNum = 0;
        DISPLAY_DEVICE dispDev;
        do
        {
            dispDev.cb = sizeof(DISPLAY_DEVICE);
            EnumDisplayDevices(NULL, iDevNum, &dispDev, 0);
            ++iDevNum; // Incrementing here is right since we want a 1-based display.
        } while (0 != _tcscmp (dispDev.DeviceName, monInfoEx.szDevice));

        // Attempt to get the description from WMI.
        // If it's empty, carry on.
        const STRING descriptionFromWMI = GetMonitorDescriptonFromWMI(iDevNum);
        if (!descriptionFromWMI.empty())
            return descriptionFromWMI;

        // Enumerate again, since doing it by string instead of index yields a different (more usable) DeviceString.
        dispDev.cb = sizeof(DISPLAY_DEVICE);
        EnumDisplayDevices(monInfoEx.szDevice, 0, &dispDev, 0);

        // WMI approach failed so we rely on EnumDisplayDevices() for an acceptable result.
        STRING description(dispDev.DeviceString);

        return description;
    }
    else return TEXT("Unknown Monitor");
}
// GET MONITOR NAME.
//	-------------------------------------------------------------------------------------------

#endif

#endif