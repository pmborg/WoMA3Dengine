// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: WomaSetupManager.h
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

#include "platform.h"

#define ButtonSetup		0
#define ButtonStart		1

////////////////////////////////////////////////////////////////////////////////
// Class name: SkyPlaneClass
////////////////////////////////////////////////////////////////////////////////
class WomaSetupManager
{
public:
	UINT WomaIntegrityCheck = 1234567890;
	WomaSetupManager();
	~WomaSetupManager();

	bool Initialize(void* g_driver);
	void Shutdown();

	// VAR:
	HWND m_setupWnd;
	HWND m_hBtnOK;
	HWND m_hBtnCancel;

	std::vector<HWND> hWndTitleLabel;	// hWndTitleLabel[11]
	std::vector<HWND> hWndComboBox;		// hWndComboBox[7]
	std::vector<HWND> hWndCheckBox;		// hWndCheckBox[4]

	std::vector<HWND> hWndTitleLabelperMonitor;	// hWndTitleLabel
	std::vector<HWND> hWndComboBoxperMonitor;	// hWndComboBox[
};

