// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: 6OperatingSystem.cpp
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
//WomaIntegrityCheck = 1234567829;

#define _CRT_SECURE_NO_WARNINGS

#include "WinSystemClass.h"

#define _WIN32_DCOM
using namespace std;

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <wincred.h>
#include <strsafe.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "credui.lib")
#pragma comment(lib, "comsuppw.lib")

// Macro for cleanup (changed to exit to imitate original code more closely)
#define EXIT(x) { ret = x; goto out; }

int GETOS(void)
{
    IWbemLocator* pLoc = NULL;
    IWbemServices* pSvc = NULL;
    IWbemClassObject* pclsObj = NULL;
    IEnumWbemClassObject* pEnum = NULL;
    HRESULT hres;
    int ret = 0;
    // Obtain the initial locator to WMI 
    hres = CoCreateInstance(
        CLSID_WbemLocator,             
        0, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator, 
        (LPVOID*)&pLoc); 

    if (FAILED(hres))
    {
        cout << "Failed to create IWbemLocator object." << hex << hres << endl;
        //CoUninitialize();
        EXIT(1);
    }

    // Connect to WMI through the IWbemLocator::ConnectServer method...


    // First, prompt for the user name and password for the remote computer
    
    bool useToken = false;
    bool useNTLM = true;
    // Connect to the remote root\cimv2 namespace and obtain pointer pSvc to make IWbemServices calls.
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL/*_bstr_t(useToken?NULL:pszName)*/,     // User name
        NULL/*_bstr_t(useToken?NULL:pszPwd)*/,      // User password
        NULL,                                       // Locale             
        NULL,                                       // Security flags
        NULL/*_bstr_t(useNTLM?NULL:pszAuthority)*/, // Authority        
        NULL,                                       // Context object
        &pSvc                                       // IWbemServices proxy
        );

    if (FAILED(hres))
    {
        cout << "Could not connect. Error code = 0x" << hex << hres << endl;
        pLoc->Release();
        //CoUninitialize();
        EXIT(1);
    }
    cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;
    // Set security levels on a WMI connection
    hres = CoSetProxyBlanket(
       pSvc,                        // Indicates the proxy to set
	   RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
	   RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
	   NULL,                        // Server principal name 
	   RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
	   RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
       NULL/*userAcct*/,            // client identity
       EOAC_NONE                    // proxy capabilities 
    );

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket. Error code = 0x" << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        //CoUninitialize();
        EXIT(1);
    }

    // Use the IWbemServices pointer to make requests of WMI...
    // Get the name of the operating system

    hres = pSvc->ExecQuery(
        bstr_t("WQL"), 
        bstr_t("Select * from Win32_OperatingSystem"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
        NULL,
        &pEnum);

    if (FAILED(hres))
    {
        cout << "Query for operating system name failed." << " Error code = 0x" << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        //CoUninitialize();
        EXIT(1);
    }

    // Secure the enumerator proxy
    hres = CoSetProxyBlanket(
        pEnum,                          // Indicates the proxy to set
        RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
        COLE_DEFAULT_PRINCIPAL,         // Server principal name 
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
        RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
        NULL/*userAcct*/,               // client identity
        EOAC_NONE                       // proxy capabilities 
        );

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket on enumerator. Error code = 0x" << hex << hres << endl;
        pEnum->Release();
        pSvc->Release();
        pLoc->Release();
        //CoUninitialize();
        EXIT(1);
    }
    // Get the data from the OS query

    ULONG uReturn = 0;

    while (pEnum)
    {
        // DEBUG:
        //cout << "beginning of loop..." << endl;

        // Get the result of the query
        HRESULT hr = pEnum->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);  

        // DEBUG:
#ifdef _DEBUG
        cout << "IEnumWbemClassObject::Next returned 0x" << hex << hr << endl;
#endif

        if (0 == uReturn)
            continue;

		// MORE INFO: https://msdn.microsoft.com/en-us/library/aa394239%28v=vs.85%29.aspx
        VARIANT vtProp;

		{
			//
			if (SUCCEEDED(pclsObj->Get(L"Name", 0, &vtProp, 0, 0)))
				WOMA_LOGManager_DebugMSGW(L"OS Name: %s\n", vtProp.bstrVal);

			std::wstring result = vtProp.bstrVal;
			size_t pos = result.find('|');
			result = result.substr(0, pos);

		#if defined UNICODE
			wcscpy_s(SystemHandle->systemManager->pszOS, BUFSIZE, result.c_str());
		#else
			WideCharToMultiByte(CP_ACP, 0, result.c_str(), -1, SystemHandle->systemManager->pszOS, BUFSIZE, NULL, NULL);  // "WCHAR" to "char" converter:
		#endif		

			//
			if (SUCCEEDED(pclsObj->Get(L"version", 0, &vtProp, 0, 0)))
				WOMA_LOGManager_DebugMSGW(L"version: %s\n", vtProp.bstrVal);

			swscanf(vtProp.bstrVal, L"%d.%d", &SystemHandle->systemManager->MajorVersion, &SystemHandle->systemManager->MinorVersion); // Note: Cant use TEXT()

			//
			if (SUCCEEDED(pclsObj->Get(L"BuildNumber", 0, &vtProp, 0, 0)))
				WOMA_LOGManager_DebugMSGW(L"BuildNumber: %s\n", vtProp.bstrVal);

			SystemHandle->systemManager->BuildVersion = _wtoi(vtProp.bstrVal);
		}
		if (SUCCEEDED(pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0)))
			WOMA_LOGManager_DebugMSGW(L"Manufacturer: %s\n", vtProp.bstrVal);

		if (SUCCEEDED(pclsObj->Get(L"OSLanguage", 0, &vtProp, 0, 0)))
			WOMA_LOGManager_DebugMSGW(L"OSLanguage: %d\n", vtProp.uintVal);

		if (SUCCEEDED(pclsObj->Get(L"Locale", 0, &vtProp, 0, 0)))
			WOMA_LOGManager_DebugMSGW(L"Locale: %s\n", vtProp.bstrVal);

		// etc...

        VariantClear(&vtProp);
        pclsObj->Release();
        pclsObj = NULL;

        // KEEP THIS ONLY IF USING PATCH SOLN
        break;
    }

out:
    // Clean up
	if (pSvc)
		pSvc->Release();
	if (pLoc)
		pLoc->Release();
	if (pEnum)
		pEnum->Release();
    if (pclsObj) 
        pclsObj->Release(); 

    return ret;
}

