// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: default_settings_xml.cpp
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
// PURPOSE: SET DEFAULT: "Aplication Settings" (they will be overwritten by "settings.xml" in run-time)
//
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// Includes:
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "OSengine.h"

#include "log.h"

#include "default_settings_xml.h"			// Settings

// --------------------------------------------------------------------------------------------
// Global Aplication Settings: ("Release" build only, "Debug" use local files)
// --------------------------------------------------------------------------------------------
namespace WOMA
{
	ILogManager* logManager = NULL;	// Global Log Manager

	UINT ENGINE_LEVEL_USED = 0;

	// --------------------------------------------------------------------------------------------
	// Driver Settings:
	// --------------------------------------------------------------------------------------------
	Settings settings =
	{
		NULL,		// LANGID id
		//false,	// HaswellCPU
		0,			// Endian (BIG_ENDIAN 0) & (LITTLE_ENDIAN 1)

		// driver
		// -------------------------------------------------------------------------------------------
		-1,			// Use Woma Driver		{DRIVER_GL3=0, DRIVER_DX11};
		0,			// USE MONITOR
		false,		// UseAllMonitors
		true,		// Use Double Buffering?
		true,		// Start in VSYNC?		(TRUE: Benchmark | FALSE: Smooth)

		true,	// MSAA_ENABLED?
		0,		// MSAA_X: 0 = Autodetect best mode (Values: 1,2,4,8...)

		//	Texture
		// -------------------------------------------------------------------------------------------
		1024,	// maxTexture Resolution Size
		false,	// bilinear
		false,	// trilinear
		false,	// Anisotropic;
		16,		// AnisotropicLevel

		// screen
		// -------------------------------------------------------------------------------------------
		false,	// Start in FULLSCREEN?
		false, 
		0,		// PosX:		Note: 0 = (AUTO) Bottom Right Corner.
		0,		// PosY			Note: 0 = (AUTO) Bottom Right Corner.
		0,		// PosX_ori
		0,		// PosY_ori
		1280,	// WIN_WIDTH	Note: On Fullscreen WIN_WIDTH = SCREEN_RESOLUTION_WIDTH
		1024,	// WIN_HEIGHT	Note: On Fullscreen WIN_HEIGHT = SCREEN_RESOLUTION_HEIGHT
		1280,	// WIN_WIDTH_ori	
		1024,	// WIN_HEIGHT_ori	
		32,		// BITSPERPEL	16,24,32?

		// -------------------------------------------------------------------------------------------
		//  Camera:
		0.1f,	// SCR_NEAR
		512.0f, // SCR_DEPTH	(Depend of Computer Performance)

		0,	//INIT_CAMX;
		0,	//INIT_CAMY;
		0,	//INIT_CAMZ;

		0,	//INIT_ROTX;
		0,	//INIT_ROTY;
		0,	//INIT_ROTZ;

	// ------------------------------------------------------------------------------------------
	// FOG
	1,
	512,

	// ------------------------------------------------------------------------------------------
	// SOUND
	true,	// Bk Music...
	true,	// Sounding...

	// ------------------------------------------------------------------------------------------
	0,		// 800 SCREEN_RESOLUTION_WIDTH NOTE: 0 = Auto Detect
	0,		// 600 SCREEN_RESOLUTION_HEIGHT NOTE: 0 = Auto Detect

	// ------------------------------------------------------------------------------------------
	// TODO: Networking...
	false,
	"012345678912345",// char only!
	0,
	};
}
