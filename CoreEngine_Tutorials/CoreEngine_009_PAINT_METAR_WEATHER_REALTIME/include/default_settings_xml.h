// --------------------------------------------------------------------------------------------
// Filename: default_settings_xml.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: Define the structure of default settings when settings.xml file is missing.
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#pragma once
#ifndef __DEFAULTSETTINGS_XML_H__

#include "platform.h"
#include "standard_platform.h"

#if defined WINDOWS_PLATFORM
#include <objbase.h>
#include <io.h>
#include <Shellapi.h>	// CommandLineToArgv()
#include <Wincon.h>		// SetConsoleTitle()
#endif

//NOTE: Sync With: SystemClass::LoadAllDrivers() (keep this order)
enum WomaDriver {	DRIVER_DX11=0,	// DRIVER_DX11  = DX10 or DX10.1 or DX11
					DRIVER_GL3,		// DRIVER_GL3   = OPENGL 3.3+ / OPENGL 4.0
					DRIVER_DX9,		// DRIVER_DX9   = DX11 with API for DX9
					DRIVER_DX12		// DRIVER_DX12  = DX12
};

//NOTE: The defaults settings, are beeing populated on: "main_settings.cpp" (SYNC)
namespace WOMA
{
	extern int				main_loop_state;

	typedef struct {

	#if defined WINDOWS_PLATFORM
		LANGID	id;
	#endif
		int		Endian;

		// driver
		// -------------------------------------------------------------------------------------------
		int		DRIVER;
		int		UI_MONITOR;
		// -------------------------------------------------------------------------------------------
		bool	UseAllMonitors;
		bool	UseDoubleBuffering;
		bool 	VSYNC_ENABLED;

		// Texture
		// ------------------------------------------------------------------------------------------
		UINT	MaxTextureSize;
		// -------------------------------------------------------------------------------------------
		bool	MSAA_bilinear;			//rasterDesc.AntialiasedLineEnable = true;
		bool	MSAA_trilinear;			//rasterDesc.AntialiasedLineEnable = true;
		bool	MSAA_Anisotropic;		//rasterDesc.MultisampleEnable = true;
		int		MSAA_AnisotropicLevel;	//4,8,16

		// screen
		// -------------------------------------------------------------------------------------------
		bool	FULL_SCREEN;
		bool	AllowResize;
		// -------------------------------------------------------------------------------------------
		int		WINDOW_Xpos;	//NOTE: Have to be "int" not UINT due other / used later on window position Calculations
		int		WINDOW_Ypos;
		int		WINDOW_Xpos_ori;
		int		WINDOW_Ypos_ori;
		// -------------------------------------------------------------------------------------------
		int		WINDOW_WIDTH;	//NOTE: Have to be "int" not UINT due other / used later on window position Calculations
		int		WINDOW_HEIGHT;
		int		WINDOW_WIDTH_ori;
		int		WINDOW_HEIGHT_ori;
		int		BITSPERPEL;		// 15, 16, 32?

		// -----------------------------------------------------------------------------------------
		// Camera:
		float 	SCREEN_NEAR;
		float 	SCREEN_DEPTH;

		float	INIT_CAMX;
		float	INIT_CAMY;
		float	INIT_CAMZ;

		float	INIT_ROTX;
		float	INIT_ROTY;
		float	INIT_ROTZ;

		// ------------------------------------------------------------------------------------------
		// FOG
		float	START_FOG;
		float	END_FOG;

		// ------------------------------------------------------------------------------------------
		// SOUND
		bool	MUSIC_ENABLED;	// Background Music...
		bool	SOUND_ENABLED;	// Sound: Effects...

		// Auto Detect:
		// ------------------------------------------------------------------------------------------
		int		SCREEN_RESOLUTION_WIDTH;  // Note: 0 eq. Auto detect Current Resolution
		int		SCREEN_RESOLUTION_HEIGHT; // Note: 0 eq. Auto detect Current Resolution

		// ------------------------------------------------------------------------------------------
		// Networking...
		bool			NETWORK_ENABLED;
		char			SERVER_ADDRESS[16]; //Allocate max string lenght possible: ___.___.___.___ 15+1
		unsigned short	SERVER_PORT;

	} Settings;
}

#endif