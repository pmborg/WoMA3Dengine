// --------------------------------------------------------------------------------------------
// Filename: SystemClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Export APIs for systemclass.cpp which is the common OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once
#ifndef __SYSTEMCLASS_H__

#include "main.h" 
#include "default_settings_xml.h"

#if (defined LINUX_PLATFORM) || (defined ANDROID_PLATFORM)
#include <sys/utsname.h>
#endif

const int MAX_LANG_LEN = 50;

struct resolutionType
{
	UINT Width;
	UINT Height;
	UINT RefreshRate_Numerator;
	UINT RefreshRate_Denominator;
};

namespace WOMA
{
	struct WindowDataContainer
	{
		// 1
		STRING MonitorName = TEXT("");
		CHAR deviceName[MAX_STR_LEN] = {};
		UINT deviceIndex = 0;
		// 2
		std::vector<resolutionType> ScreenResolution = {};
		// 3
		UINT width = 0;
		UINT height = 0;
		UINT refreshRate_Numerator = 0;
		UINT refreshRate_Denominator = 0;
#if defined WINDOWS_PLATFORM
		// 4
		HWND hWnd = 0;
		// Window sizes:
		RECT m_rcWindowBounds;    // Saved window bounds for mode switches
		RECT m_rcWindowClient;    // Saved client area size for mode switches
#endif

#if defined LINUX_PLATFORM
		Display* display = nullptr;
		::Window window = 0;
#endif
	};

	extern std::vector<WOMA::WindowDataContainer> windowsToRenderArray;	// All Monitors, that will be used to render, 1 for example.
	extern std::vector<WOMA::WindowDataContainer> allWindowsHwArray; // All hardware Monitors available on system, 3 for example.
}


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	~SystemClass();

	void Shutdown();

	bool			mMaximized;
	void FrameUpdate();
    void ParseCommandLineAndApplySettings(int argc, char* argv[]);

	resolutionType		resolution;

#if CORE_ENGINE_LEVEL >= 2 && (defined USE_PROCESS_OS_KEYS || defined INTRO_DEMO)
	bool InitOsInput();
#endif

#if CORE_ENGINE_LEVEL >= 2 && defined WINDOWS_PLATFORM	
	DISPLAY_DEVICE displayDevice;
#endif

public:

};

#endif
