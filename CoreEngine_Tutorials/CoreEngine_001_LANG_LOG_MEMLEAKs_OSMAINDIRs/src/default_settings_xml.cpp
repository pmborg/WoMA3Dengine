// --------------------------------------------------------------------------------------------
// Filename: default_settings_xml.cpp
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
// PURPOSE: SET DEFAULT: "Aplication Settings" (they will be overwritten by "settings.xml" in run-time)
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#define _CRT_SECURE_NO_WARNINGS
#include "OSengine.h"
#include "main.h"
#if defined USE_LOG_MANAGER
#include "log.h"
#endif
#include "default_settings_xml.h"			// Settings

// --------------------------------------------------------------------------------------------
// Global Aplication Settings: ("Release" build only, "Debug" use local files)
// --------------------------------------------------------------------------------------------
namespace WOMA
{
	// --------------------------------------------------------------------------------------------
	// Driver Settings:
	// --------------------------------------------------------------------------------------------
	Settings settings =
	{
	#if defined WINDOWS_PLATFORM	
		NULL,		// LANGID id
	#endif
		0,			// Endian (BIG_ENDIAN 0) & (LITTLE_ENDIAN 1)

		// driver
		// -------------------------------------------------------------------------------------------
	#if defined USE_TINYXML_LOADER
		-1,			// Use Woma Driver	-1 (auto)	{DRIVER_DX11=0, DRIVER_GL3=1, DRIVER_DX11_DX9=2, DRIVER_DX12=3};
	#else
		DRIVER_DX11,
	#endif
		0,			// USE MONITOR
		// -------------------------------------------------------------------------------------------
		false,		// UseAllMonitors
		true,		// Use Double Buffering?
		true,		// Start in VSYNC?		(TRUE: Benchmark | FALSE: Smooth)


		//	Texture
		// -------------------------------------------------------------------------------------------
		1024,	// maxTexture Resolution Size
		// -------------------------------------------------------------------------------------------
		false,	// bilinear
		false,	// trilinear
		false,	// Anisotropic;
		4,		// AnisotropicLevel

		// screen
		// -------------------------------------------------------------------------------------------
		false,	// Start in FULLSCREEN?
		#if CORE_ENGINE_LEVEL < 10
		false, 
		#else
		true,	// AllowResize and FULLSCREEN Switch?
		#endif
		// -------------------------------------------------------------------------------------------
	#if defined LINUX_PLATFORM
		600,	// PosX:		Note: 0 = (AUTO) Bottom Right Corner.
		20,		// PosY			Note: 0 = (AUTO) Bottom Right Corner.
	#else
		0,		// PosX:		Note: 0 = (AUTO) Bottom Right Corner.
		0,		// PosY			Note: 0 = (AUTO) Bottom Right Corner.
	#endif
		0,		// PosX_ori
		0,		// PosY_ori
		// -------------------------------------------------------------------------------------------
	#if defined ANDROID_PLATFORM
		1280,	// WIN_WIDTH	Note: On Fullscreen WIN_WIDTH = SCREEN_RESOLUTION_WIDTH
		1024,	// WIN_HEIGHT	Note: On Fullscreen WIN_HEIGHT = SCREEN_RESOLUTION_HEIGHT
		1280,	// WIN_WIDTH_ori	
		1024,	// WIN_HEIGHT_ori	
	#else //LINUX:
		1920, //550,	// WIN_WIDTH	Note: On Fullscreen WIN_WIDTH = SCREEN_RESOLUTION_WIDTH
		1080, //940,	// WIN_HEIGHT	Note: On Fullscreen WIN_HEIGHT = SCREEN_RESOLUTION_HEIGHT
		1920, //550,	// WIN_WIDTH_ori	
		1080, //940,	// WIN_HEIGHT_ori	
	#endif
		32,		// BITSPERPEL	16,24,32?

		// -------------------------------------------------------------------------------------------
		//  Camera:
		0.1f,	// SCR_NEAR
		512.0f, // SCR_DEPTH	(Depend of Computer Performance)

		0,		// INIT_CAMX;
		2,		// INIT_CAMY;
	#if defined ANDROID_PLATFORM
		-29,	// INIT_CAMZ;
	#else
		- 28,	// INIT_CAMZ;
	#endif

		0,		// INIT_ROTX;
		0,		// INIT_ROTY;
		0,		// INIT_ROTZ;

	// ------------------------------------------------------------------------------------------
		// FOG
		1,		//nofog
		1024,	//nofog

	// ------------------------------------------------------------------------------------------
		// SOUND
		true,	// Bk Music...
		true,	// Sounding...

	// ------------------------------------------------------------------------------------------
		0,		// 800 SCREEN_RESOLUTION_WIDTH NOTE: 0 = Auto Detect
		0,		// 600 SCREEN_RESOLUTION_HEIGHT NOTE: 0 = Auto Detect

	// ------------------------------------------------------------------------------------------
		// Networking...
		false,
		TEXT ("___.___.___.___"),// char only!
		0,
	};

#if CORE_ENGINE_LEVEL >= 4
	STRING	filename;					
#endif
}
