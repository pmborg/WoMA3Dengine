// --------------------------------------------------------------------------------------------
// Filename: DX11GetModesList.cpp
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
//WomaIntegrityCheck = 1234567831;

#include "main.h"
#include <d3d11.h>
#include "DX11Class.h"
#include "DXengine.h"
#include "winsystemclass.h"	// SystemHandle
#include "mem_leak.h"

namespace DirectX {

// |Init Step: 2|
// More: Microsoft_DirectX_SDK_June_2010\Samples\C++\Direct3D10\MultiMon10\MultiMon10.cpp
// ----------------------------------------------------------------------------------------------
bool DX11Class::getModesList(int g_USE_MONITOR, int screenWidth, int screenHeight, BOOL fullscreen,	UINT* numerator, UINT* denominator)
// ----------------------------------------------------------------------------------------------
{
	HRESULT result = S_OK;
	IDXGIFactory1* pDXGIFactory = NULL;
	IDXGIOutput* outputMonitor = NULL; // MONITOR

	if (SystemHandle->allWindowsArray.size() > 0)
		return true; // the work was done already...

	*numerator = 60;	//Setup defaults, at Start!
	*denominator = 1;

	/*******************************************************************
	* Before we can initialize Direct3D we have to get: 
		-	the refresh rate from the video card/monitor
		-	chek all adapters available
			-	chek all monitors attached to each Adapter
	*******************************************************************/

	IF_FAILED_RETURN_FALSE (CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pDXGIFactory)); // Use: dxgi.dll
    IF_NOT_RETURN_FALSE (pDXGIFactory);
	
	// "Advanced"
	// Check how many adapters we have... Loop as long as DXGI returns valid Adapters.
	int AdapterNumber = 0;	// <- Will have the number of Video Card Adapters
	IDXGIAdapter1* pAdapter = NULL;

	/*******************************************************************
	// Enumerate Adapters:	(Graphic Cards)
	// PROPOSE	Set:		adapterGraphicCard
	//			Set:		AdapterNumber
	*******************************************************************/
	while (pDXGIFactory->EnumAdapters1 (AdapterNumber, &pAdapter) == S_OK)
	{
		// ---------------------------------------------------------------------------------------------
		// Get Info. from this "Adapter" (Graphic Card) Interface:
		// ---------------------------------------------------------------------------------------------
		DXGI_ADAPTER_DESC1 adapterDesc;
		IF_FAILED_RETURN_FALSE(pAdapter->GetDesc1(&adapterDesc));

		GPU	GPUINFO;

		#if defined UNICODE
			StringCchPrintf(GPUINFO.GraphicCard, MAX_STR_LEN, TEXT("\nGraphic Card %d: %s"), AdapterNumber, adapterDesc.Description); //Note: Have to be "wchar"
		#else
			WideCharToMultiByte(CP_ACP,0,adapterDesc.Description,-1, adapterDesc_Description, MAX_STR_LEN,NULL,NULL); //Note: Cant use: wtoa
			StringCchPrintf(GPUINFO.GraphicCard, MAX_STR_LEN, TEXT("\nGraphic Card %d: %s"), AdapterNumber, adapterDesc_Description); //Note: Have to be "wchar"
		#endif
		
		WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), GPUINFO.GraphicCard);

		if ( (SystemHandle) && (SystemHandle->systemManager)  )
		{
		#if defined __SKIPIT_FOR_NOW__
			#if defined UNICODE
				StringCchPrintf(GPUINFO.AdapterDACType, MAX_STR_LEN, TEXT("AdapterDACType: %s"), SystemHandle->systemManager->wmiUtil.AdapterDACType.c_str());
			#else
				TCHAR	AdapterDACType[MAX_STR_LEN];			// Note: have to be wstring
				WideCharToMultiByte(CP_ACP,0,SystemHandle->systemManager->wmiUtil.AdapterDACType.c_str(),-1, AdapterDACType, MAX_STR_LEN,NULL,NULL); //Note: Cant use: wtoa
				StringCchPrintf(GPUINFO.AdapterDACType, MAX_STR_LEN, TEXT("AdapterDACType: %s"), AdapterDACType);
			#endif

				WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), GPUINFO.AdapterDACType);

				StringCchPrintf(GPUINFO.AdapterRAM, MAX_STR_LEN, TEXT("AdapterRAM: %d"), SystemHandle->systemManager->wmiUtil.AdapterRAM);
				WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), GPUINFO.AdapterRAM);

				ufreededicatedVideoMem = (UINT) (adapterDesc.DedicatedVideoMemory / (MBs));
				StringCchPrintf(GPUINFO.DedicatedVideoMemory, MAX_STR_LEN, TEXT("Dedicated Video Mem: %d MB"), ufreededicatedVideoMem);
				WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), GPUINFO.DedicatedVideoMemory);

				StringCchPrintf(GPUINFO.DedicatedSystemMemory, MAX_STR_LEN, TEXT("Dedicated System Memory: %d MB"), (int) (adapterDesc.DedicatedSystemMemory / (MBs)));
				WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), GPUINFO.DedicatedSystemMemory);

				StringCchPrintf(GPUINFO.SharedSystemMemory, MAX_STR_LEN, TEXT("Shared System Memory: %d MB"), (int) (adapterDesc.SharedSystemMemory / (MBs)));
				WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), GPUINFO.SharedSystemMemory);

				SystemHandle->systemDefinitions.GPUINFO.push_back (GPUINFO);
		#endif
		}

		// Tipically "GraphicCards" Sample Adapters:
		// 0: "Graphic Card" Interface.
		// 1: "Microsoft Basic Render Driver" Interface.
		//
		// SAVE: our Adapter to use later, FREE: The others.
		if (AdapterNumber == USE_THIS_GRAPHIC_CARD_ADAPTER)
			adapterGraphicCard = pAdapter;	// SAVE: our Adapter
		else 
			pAdapter->Release();			// Release the adapters that will not be used.

		AdapterNumber++;
	}

	ASSERT (AdapterNumber);

	/*******************************************************************
	// "Advanced": Check how many Monitors we have in our Adapter Card: 
	// "USE_ADAPTER" Loop as long as DXGI returns valid Adapters.
	//
	// PROPOSE	Set:		MonitorNumber
	//          Populate:	SystemHandle->windowsArray
	*******************************************************************/
	IDXGIOutput* pMonitor = NULL;
	while (adapterGraphicCard->EnumOutputs (MonitorNumber, &pMonitor) == S_OK)
	{
		pMonitor->Release (); // Do something with the Monitor object
		MonitorNumber++;
	}

	if (MonitorNumber == 0)
		WOMA_LOGManager_DebugMSGAUTO(TEXT("WARNING: We dont have any monitor, conneted to our Graphic Card (We are in Remote desktop without monitor attached)\n"));
	
	// ---------------------------------------------------------------------------------------------
	// Iterate on all Monitors (might be also only 1!) and get all possible resolutions for them:
	// ---------------------------------------------------------------------------------------------
	for (UINT mon = 0; mon < MonitorNumber; mon++)
	{
		WOMA_LOGManager_DebugMSGAUTO(TEXT("\nMonitor %d: "), mon);

		// Get the MONITOR: "mon"
		IF_FAILED_RETURN_FALSE(adapterGraphicCard->EnumOutputs(mon, &outputMonitor));// MONITOR
		if (!outputMonitor) return false;

		WOMA::WindowDataContainer screen;

		// WindowDataContainer:[1] get the MonitorName:
		DXGI_OUTPUT_DESC OutputDesc;
		if (!FAILED(outputMonitor->GetDesc(&OutputDesc)))
		{
  			{
				if (SystemHandle->systemManager) 
				{
					STRING text = SystemHandle->systemManager->wmiUtil.GetMonitorDescription(OutputDesc.Monitor);
					if (text.length() == 0)
						text = TEXT("---");
					WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), text.c_str());

					// 1
					screen.MonitorName = text;
				}
            }
		} else {
			WOMA_LOGManager_DebugMSGAUTO(TEXT("WARNING: Could not get the description of this Monitor: %d\n"), mon);
		}
		
		#if UNICODE
			std::string str = screen.deviceName;
			screen.deviceIndex = atoi(str.c_str());
		#else
			//Cant be wtoa():
			WideCharToMultiByte(CP_ACP, 0, OutputDesc.DeviceName, -1, screen.deviceName, MAX_STR_LEN, NULL, NULL);  // FORCE: "WCHAR" to "char" converter:
			STRING str = screen.deviceName;
			int char_index = (int)str.find_first_of(TEXT("DISPLAY"));
			str = str.substr(char_index+7, 1);
			screen.deviceIndex = atoi(str.c_str());
		#endif

		//This is the DX's Adapter Formats (Preferred Mode: 1st)
		DXGI_FORMAT allowedAdapterFormatArray [] = { DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB }; //*This is DXUT's preferred mod*/, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_R10G10B10A2_UNORM };
		UINT allowedAdapterFormatArrayCount = sizeof(allowedAdapterFormatArray) / sizeof(allowedAdapterFormatArray[0]);

		// Find the best FORMAT: BUFFER_COLOR_FORMAT for this GraphicCard/Monitor:
		// ---------------------------------------------------------------------------------------------
		UINT f;
		bool find = false;
		for (f = 0; f < allowedAdapterFormatArrayCount; f++)
		{
			// Get the number of modes that fit the "Preferred Mode" display format for the adapterGraphicCard output (monitor).
			result = outputMonitor->GetDisplayModeList(allowedAdapterFormatArray[f],
				NULL
				#if defined DX11_ALLOW_MODES_INTERLACED
					| DXGI_ENUM_MODES_INTERLACED 
				#endif
				#if defined DX11_ALLOW_MODES_SCALING
					| DXGI_ENUM_MODES_SCALING
				#endif
				,
				&numModes, NULL);
			if (!FAILED(result)) 
				{ find = true; break; }
		}

		if (result == DXGI_ERROR_NOT_FOUND)
			continue; // Next Monitor...

		if (find)
		{   
			// FOUND!
			// ---------------------------------------------------------------------------------------------
			BUFFER_COLOR_FORMAT = allowedAdapterFormatArray[f];

			// Create a list to hold all the possible display modes for this monitor/video card combination.
			displayModeList = NEW DXGI_MODE_DESC[numModes];
			IF_NOT_THROW_EXCEPTION (displayModeList);

			// Now GET THE LIST of display mode structures, for that specific "FormatArray":
			HRESULT HR = outputMonitor->GetDisplayModeList(	allowedAdapterFormatArray[f],
															NULL
															#if defined DX11_ALLOW_MODES_INTERLACED
																| DXGI_ENUM_MODES_INTERLACED 
															#endif
															#if defined DX11_ALLOW_MODES_SCALING
																| DXGI_ENUM_MODES_SCALING
															#endif
															,
															&numModes, 
															displayModeList);
			IF_FAILED_RETURN_FALSE (HR);
		} 
		else 
		{
			// "Remote Desktop Connection"
			// ---------------------------------------------------------------------------------------------
			BUFFER_COLOR_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
			
			if (result != MAKE_DXGI_HRESULT(34)) // Check if we are using "Remote Desktop Connection", if "YES", use defaults and get out of here...
			{
				*numerator = 60;	//Use defaults!
				*denominator = 1;
			}

			WOMA_LOGManager_DebugMSGAUTO(TEXT("Monitor Using Remote Desktop Connection\n"));
			SAFE_RELEASE(outputMonitor);		// Release the adapterGraphicCard output.
			SAFE_DELETE_ARRAY(displayModeList);

			// WindowDataContainer:[2]: "Remote Desktop Connection"
			resolutionType resolution;
			resolution.Width = GetSystemMetrics(SM_CXSCREEN);
			resolution.Height = GetSystemMetrics(SM_CYSCREEN);
			resolution.RefreshRate_Numerator = *numerator;
			resolution.RefreshRate_Denominator = *denominator;

			screen.ScreenResolution.push_back(resolution);
		}

		// ---------------------------------------------------------------------------------------------
		// Now go through all the "Display Modes" availabel and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		// ---------------------------------------------------------------------------------------------
		for (int i = (numModes - 1); i >= 0; i--) // New to need Backwards ( once better RefreshRate are the last ones! )
		{
			// This index: "i" resolution already exist or its "new"?
			bool found = false;
			for (UINT res=0; res < screen.ScreenResolution.size(); res++)
			{
				if (screen.ScreenResolution[res].Width == displayModeList[i].Width)
				{
					if (screen.ScreenResolution[res].Height == displayModeList[i].Height)
					{
						found = true;
						break;
					}
				}
			}

			// [1] If this is a "new" Resolution? Save it for alter...
			if (!found) 
			{
				// WindowDataContainer:[2] Normal Modes
				resolutionType resolution;
				resolution.Width = displayModeList[i].Width;
				resolution.Height = displayModeList[i].Height;

				resolution.RefreshRate_Numerator = displayModeList[i].RefreshRate.Numerator;
				resolution.RefreshRate_Denominator = displayModeList[i].RefreshRate.Denominator;

				screen.ScreenResolution.push_back( resolution );
			}

			// [2] If user want to go on fullscreen (XML Settings) on this monitor: 
			// Get the Best "RefreshRate" for that resolution (XML Settings)!
			if ((mon == g_USE_MONITOR && fullscreen) || (SystemHandle->AppSettings->UseAllMonitors == true))
			{
				if (((displayModeList[i].Width == (unsigned int) screenWidth)) &&
					((displayModeList[i].Height == (unsigned int) screenHeight)))
				{
					// Find the best refresh Rate
					if (displayModeList[i].RefreshRate.Numerator / displayModeList[i].RefreshRate.Denominator > (*numerator) / (*denominator)) 
					{
						*numerator = displayModeList[i].RefreshRate.Numerator;
						*denominator = displayModeList[i].RefreshRate.Denominator;

						// Save WindowDataContainer:[3] 
						screen.refreshRate_Numerator = displayModeList[i].RefreshRate.Numerator;
						screen.refreshRate_Denominator = displayModeList[i].RefreshRate.Denominator;
					}
				}
			}
		}//FOR numModes

		SAFE_RELEASE(outputMonitor);		// Release the adapterGraphicCard output.
		SAFE_DELETE_ARRAY(displayModeList);

		// WindowDataContainer:[2]:
		if (!fullscreen)
		{ 

			//MONITORINFOEX iMonitor;
			//iMonitor.cbSize = sizeof(MONITORINFOEX);
			//GetMonitorInfo(hMonitor, &iMonitor);

			double horizontalScale = 0;
			double verticalScale = 0;
			double cxLogical = 0;
			double cyLogical = 0;
			double cxPhysical = 0;
			double cyPhysical = 0;

			/*
			// Get the logical width and height of the monitor
			MONITORINFOEX monitorInfoEx;
			monitorInfoEx.cbSize = sizeof(monitorInfoEx);
			GetMonitorInfo(hMonitor, &monitorInfoEx);
			cxLogical = monitorInfoEx.rcMonitor.right - monitorInfoEx.rcMonitor.left;
			cyLogical = monitorInfoEx.rcMonitor.bottom - monitorInfoEx.rcMonitor.top;

			// Get the physical width and height of the monitor
			DEVMODE devMode;
			devMode.dmSize = sizeof(devMode);
			devMode.dmDriverExtra = 0;
			EnumDisplaySettings(monitorInfoEx.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
			cxPhysical = devMode.dmPelsWidth;
			cyPhysical = devMode.dmPelsHeight;

			// Calculate the scaling factor
			horizontalScale = ((double)cxPhysical / (double)cxLogical);
			verticalScale = ((double)cyPhysical / (double)cyLogical);

			screen.width = (UINT)cxPhysical;	// GetSystemMetrics(SM_CXSCREEN);
			screen.height = (UINT)cyPhysical;	// GetSystemMetrics(SM_CYSCREEN);
			*/
			screen.width = 0; // GetSystemMetrics(SM_CXSCREEN);
			screen.height = 0; // GetSystemMetrics(SM_CYSCREEN);
			screen.refreshRate_Numerator = *numerator;
			screen.refreshRate_Denominator = *denominator;
		}

		// DEBUG: Write all Best "RefreshRate" Resolutions for this Monitor on "report.txt":
		for (UINT res = 0; res < screen.ScreenResolution.size(); res++)
		{
			WOMA_LOGManager_DebugMSGAUTO(TEXT("%d x %d "), screen.ScreenResolution[res].Width, screen.ScreenResolution[res].Height);
			WOMA_LOGManager_DebugMSGAUTO(TEXT("(%4.2f Hz)\n"), (float)screen.ScreenResolution[res].RefreshRate_Numerator / (float)screen.ScreenResolution[res].RefreshRate_Denominator);
		}

		SystemHandle->allWindowsArray.push_back(screen);
		//if ((mon == g_USE_MONITOR) || (SystemHandle->AppSettings->UseAllMonitors == true))
		//	SystemHandle->windowsArray.push_back(screen);

	} //FOR mon (go for next monitor if exits...)

	// No monitors? Are we in a remote desktop machine, with no monitors attached?
	if (SystemHandle->windowsArray.size() < 1)
	{
		WOMA::WomaMessageBox(TEXT("Invalid uiMonitor number"), TEXT("Error: ")); 
		return false;
	}

	WOMA_LOGManager_DebugMSGAUTO(TEXT("\n"));

	// We now have the "numerator" and "denominator" for the refresh rate and the list of all Resolution modes.
	// Release the structures and interfaces used to get that information:
	SAFE_RELEASE (pDXGIFactory);			// Release the factory.

	return true;
}

}

