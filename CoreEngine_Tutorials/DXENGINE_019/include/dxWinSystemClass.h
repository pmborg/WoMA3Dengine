// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: dxWinSystemClass.h
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
#pragma once

#pragma warning( disable : 4065 )	// warning C4065: switch statement contains 'default' but no 'case' labels
#define _CRT_SECURE_NO_WARNINGS

#include "WinSystemClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: WinSystemClass
////////////////////////////////////////////////////////////////////////////////
class dxWinSystemClass : public WinSystemClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	dxWinSystemClass(WOMA::Settings* AppSettings);
	~dxWinSystemClass();

	bool InitSelectedDriver();
	bool InitializeSystem();
	int	 ApplicationMainLoop();
	void ProcessFrame();

	void ProcessOSInput();

	bool LoadWorldXmlSettings();

	bool ApplicationInitMainWindow();

	void PAUSE();
	void UNPAUSE();
	void GPH_RESIZE();

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
	bool SaveScreenshot();
#endif

	//private:
	void Shutdown();
};

extern dxWinSystemClass* DXsystemHandle;

