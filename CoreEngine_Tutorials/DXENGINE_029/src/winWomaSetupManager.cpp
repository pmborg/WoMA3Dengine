// --------------------------------------------------------------------------------------------
// Filename: winWomaSetupManager.cpp
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

#include "WinSystemClass.h"
#if defined CLIENT_SCENE_SETUP

#include "main.h"
#include "StateMachine.h"
#include "WomaSetupManager.h"

WomaSetupManager::WomaSetupManager() 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567829;

	m_setupWnd = NULL;
}
WomaSetupManager::~WomaSetupManager(){CLASSDELETE();}

void WomaSetupManager::Shutdown()
{
	for (UINT i=0; i < hWndTitleLabel.size(); i++) {
		DestroyWindow(hWndTitleLabel[i]);
	}

	for (UINT i=0; i < hWndComboBox.size(); i++) {
		DestroyWindow (hWndComboBox[i]);
	}

	for (UINT i=0; i < hWndCheckBox.size(); i++){
		DestroyWindow (hWndCheckBox[i]);
	}

	for (int mon = 0; mon < SystemHandle->info.Count; mon++)
	{
		DestroyWindow(hWndTitleLabelperMonitor[mon]);
		DestroyWindow(hWndComboBoxperMonitor[mon]);
	}

	DestroyWindow(m_hBtnOK);
	DestroyWindow(m_hBtnCancel);

	RedrawWindow(SystemHandle->m_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_ALLCHILDREN | RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_ERASE);  // Invoke: Window PAINT (with ERASE)
}

bool WomaSetupManager::Initialize(void* Driver)
{
	static int yPos_initial = 250;
	int xPos = 20, yPos = yPos_initial;
#if defined _NOT
	m_driver = (DX_CLASS*)Driver;
#endif

	m_setupWnd = SystemHandle->m_hWnd;
	HWND windownTOP = m_setupWnd;

	// 0 "Display Mode": ComboBox (lvl:5)
	// ---------------------------------------------------------------------------------------------
	DWORD Style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL | WS_CLIPCHILDREN;

	UINT hWndTitleLabelIdx = 0;
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Display Mode:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
		xPos, yPos, 130, 20, windownTOP, HMENU(100), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[0] = CreateWindow(TEXT("COMBOBOX"), TEXT("Display Mode"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(200), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[0], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("Windowed")));
	SendMessage(hWndComboBox[0], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Full Screen")));
	//SendMessage(hWndComboBox[0], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Windowed Fullscreen")));

	if (SystemHandle->AppSettings->FULL_SCREEN)
		SendMessage(hWndComboBox[0], CB_SETCURSEL, 1, NULL); //Default Value
	else
		SendMessage(hWndComboBox[0], CB_SETCURSEL, 0, NULL); //Default Value
	// ---------------------------------------------------------------------------------------------

	yPos += 30;

	// 1 "UI Monitor:": ComboBox (lvl:5)
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("UI Monitor:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(101), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[1] = CreateWindow(TEXT("COMBOBOX"), TEXT("UI Monitor"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(201), SystemHandle->m_hinstance, NULL);

	TCHAR str[50];
	for (int mon = 0; mon < SystemHandle->info.Count; mon++)
	{
		//float Refresh = (float)m_driver->ScreenResolution[res].RefreshRate_Numerator / (float)m_driver->ScreenResolution[res].RefreshRate_Denominator;
		StringCchPrintf(str, sizeof(str), TEXT("%d %s"), mon, SystemHandle->monitorArray[mon].szDevice);
		SendMessage(hWndComboBox[1], CB_ADDSTRING, mon, reinterpret_cast<LPARAM>(str));
	}

	SendMessage(hWndComboBox[1], CB_SETCURSEL, SystemHandle->AppSettings->UI_MONITOR, NULL); //Default Value

	// 2*
	// ---------------------------------------------------------------------------------------------
//#if defined DX11 || defined DX9
	{
	  //displayDevice info (lvl 10)
      for (int mon = 0; mon < SystemHandle->info.Count; mon++)
	  {
		yPos += 30;

		hWndTitleLabelperMonitor.push_back(NULL);

		TCHAR MONITOR[25] = { 0 };
#if CORE_ENGINE_LEVEL >= 10
		//Convert HW monitor list to windows orderder list:
		{
		int deviceIndex;
		if (SystemHandle->allWindowsArray.size() == 0)
			deviceIndex = 0;
		else
			deviceIndex = (SystemHandle->allWindowsArray[mon].deviceIndex) - 1;

		StringCchPrintf(MONITOR, sizeof(MONITOR), TEXT("Resolution Mon: %d"), deviceIndex);
		}
#else
		StringCchPrintf(MONITOR, sizeof(MONITOR), TEXT("Resolution Mon: %d"), mon);
#endif
		hWndTitleLabelperMonitor[mon] = CreateWindow(TEXT("STATIC"), MONITOR, WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
			xPos, yPos, 130, 20, windownTOP, HMENU((HMENU)(UINT_PTR)(mon * 100 + 1010)), SystemHandle->m_hinstance, NULL); //Note: https://stackoverflow.com/questions/3569859/questions-regarding-warning-c4312-type-cast

		hWndComboBoxperMonitor.push_back(NULL);
		hWndComboBoxperMonitor[mon] = CreateWindow(TEXT("COMBOBOX"), TEXT("Resolution"), Style,
			xPos + 150, yPos, 200, 6*40/*80*/, windownTOP, HMENU((HMENU)(UINT_PTR)(mon * 100 + 2010)), SystemHandle->m_hinstance, NULL);

		TCHAR str[50];
#if CORE_ENGINE_LEVEL >= 10
		UINT resolutionWidth = 0;
		UINT resolutionHeight = 0;

		//Convert HW monitor list to windows orderder list:
		//int deviceIndex=(SystemHandle->allWindowsArray[mon].deviceIndex)-1;
		int deviceIndex;
		if (SystemHandle->allWindowsArray.size() == 0)
			deviceIndex = 0;
		else
			deviceIndex = (SystemHandle->allWindowsArray[mon].deviceIndex) - 1;

		if ((SystemHandle->allWindowsArray.size() > 0) && (SystemHandle->allWindowsArray[mon].ScreenResolution.size() > 1))
		{
			//int cuurent_width = SystemHandle->monitorArray[mon].rcMonitor.right - SystemHandle->monitorArray[mon].rcMonitor.left;
			//int cuurent_heigth = SystemHandle->monitorArray[mon].rcMonitor.bottom - SystemHandle->monitorArray[mon].rcMonitor.top;

			// Get the physical width and height of the monitor
			DEVMODE devMode;
			devMode.dmSize = sizeof(devMode);
			devMode.dmDriverExtra = 0;
			EnumDisplaySettings(SystemHandle->monitorArray[deviceIndex].szDevice, ENUM_CURRENT_SETTINGS, &devMode);
			int cuurent_width = devMode.dmPelsWidth;
			int cuurent_heigth = devMode.dmPelsHeight;

			for (UINT res = 0; res < SystemHandle->allWindowsArray[mon].ScreenResolution.size(); res++)
			{
				float Refresh = (float)SystemHandle->allWindowsArray[mon].ScreenResolution[res].RefreshRate_Numerator
					/ (float)SystemHandle->allWindowsArray[mon].ScreenResolution[res].RefreshRate_Denominator;

				resolutionWidth = SystemHandle->allWindowsArray[mon].ScreenResolution[res].Width;
				resolutionHeight = SystemHandle->allWindowsArray[mon].ScreenResolution[res].Height;
				StringCchPrintf(str, sizeof(str), TEXT("%d x %d %4.2f Hz"), resolutionWidth, resolutionHeight, round(Refresh));

				SendMessage(hWndComboBoxperMonitor[mon], CB_ADDSTRING, res, reinterpret_cast<LPARAM>(str));

				if ((resolutionWidth == cuurent_width) && (resolutionHeight == cuurent_heigth))
				{
					SendMessage(hWndComboBoxperMonitor[mon], CB_SETCURSEL, res, NULL); //Default Value
				}
			}
		} 
#else 
		{
			int width  = SystemHandle->monitorArray[mon].rcMonitor.right - SystemHandle->monitorArray[mon].rcMonitor.left;
			int heigth = SystemHandle->monitorArray[mon].rcMonitor.bottom - SystemHandle->monitorArray[mon].rcMonitor.top;
			StringCchPrintf(str, sizeof(str), TEXT("%d x %d"), width, heigth);

			SendMessage(hWndComboBoxperMonitor[mon], CB_ADDSTRING, 0/*Only 1*/, reinterpret_cast<LPARAM>(str));
			SendMessage(hWndComboBoxperMonitor[mon], CB_SETCURSEL, 0, NULL); //Default Value
		}
#endif
#if !defined DX_ENGINE
		EnableWindow(hWndTitleLabelperMonitor[mon], false);
		EnableWindow(hWndComboBoxperMonitor[mon], false);
#endif
	  }
	}
//#endif
	// ---------------------------------------------------------------------------------------------

	yPos += 30;

	// 2 "Multisampling:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("MSAA:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(102), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[2] = CreateWindow(TEXT("COMBOBOX"), TEXT("MSAA"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(202), SystemHandle->m_hinstance, NULL);

	//SendMessage(hWndComboBox[2], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("disable")));

	StringCchPrintf(str, sizeof(str), TEXT("Disabled" ));
	SendMessage(hWndComboBox[2], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str));
	SendMessage(hWndComboBox[2], CB_SETCURSEL, 0, NULL); //Default Value
//#if CORE_ENGINE_LEVEL < 10
	EnableWindow(hWndTitleLabel[2], false);
	EnableWindow(hWndComboBox[2], false);
//#endif
	//if (!m_driver->mEnable4xMsaa) 
	//	EnableWindow(hWndComboBox[2], FALSE); // Disable Options if mEnable4xMsaa is false.
	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 3 "Texture Resolution:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Texture Resolution:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(103), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[3] = CreateWindow(TEXT("COMBOBOX"), TEXT("Texture Resolution"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(203), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[3], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("High")));
	SendMessage(hWndComboBox[3], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Good")));
	SendMessage(hWndComboBox[3], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Fair")));
	SendMessage(hWndComboBox[3], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("Low")));

	SendMessage(hWndComboBox[3], CB_SETCURSEL, 0, NULL); //Default Value
//#if CORE_ENGINE_LEVEL < 10
	EnableWindow(hWndTitleLabel[3], false);
	EnableWindow(hWndComboBox[3], false);
//#endif 
	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 4 "View Distance:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("View Distance:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(104), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[4] = CreateWindow(TEXT("COMBOBOX"), TEXT("View Distance"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(204), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[4], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("High")));
	SendMessage(hWndComboBox[4], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Good")));
	SendMessage(hWndComboBox[4], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Fair")));
	SendMessage(hWndComboBox[4], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("Low")));
//#if CORE_ENGINE_LEVEL < 10
	EnableWindow(hWndTitleLabel[4], false);
	EnableWindow(hWndComboBox[4], false);
//#endif
	// ---------------------------------------------------------------------------------------------

	yPos += 30;

	// 5 "Particle Density:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Particle Density:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(105), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[5] = CreateWindow(TEXT("COMBOBOX"), TEXT("Particle Density"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(205), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[5], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("High")));
	SendMessage(hWndComboBox[5], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Good")));
	SendMessage(hWndComboBox[5], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Fair")));
	SendMessage(hWndComboBox[5], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("Low")));
	EnableWindow(hWndTitleLabel[5], false);
	EnableWindow(hWndComboBox[5], false);
	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 6 "Network Messages:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Network Messages:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(106), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[6] = CreateWindow(TEXT("COMBOBOX"), TEXT("Network Messages"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(206), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[6], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("60 msg/s High")));
	SendMessage(hWndComboBox[6], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("30 msg/s Good")));
	SendMessage(hWndComboBox[6], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("20 msg/s Fair")));
	SendMessage(hWndComboBox[6], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("10 msg/s Low")));
	EnableWindow(hWndTitleLabel[6], false);
	EnableWindow(hWndComboBox[6], false);

	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 7 DRIVER ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Driver:"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
		xPos, yPos, 130, 20, windownTOP, HMENU(107), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[7] = CreateWindow(TEXT("COMBOBOX"), TEXT("Driver"), Style,
		xPos + 150, yPos, 200, 3*40/*80=1+3 80=1+2*/, windownTOP, HMENU(207), SystemHandle->m_hinstance, NULL);

#if defined DX11 //Pure DX11
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("DX11")));
#else
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("DX11 (not suppported)")));
#endif
#if defined OPENGL3
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("GL3")));
#else
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("GL3 (not suppported)")));
#endif
#if defined DX9sdk
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("DX9")));
#elif defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("DX9(onDX11)")));
#else
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("DX9(not suppported)")));
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("DX12")));
#else
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("DX12 (not suppported)")));
#endif

	SendMessage(hWndComboBox[7], CB_SETCURSEL, SystemHandle->AppSettings->DRIVER, NULL); //Default Value
#if !defined DX_ENGINE
	EnableWindow(hWndTitleLabel[7], false);
	EnableWindow(hWndComboBox[7], false);
#endif


	// ---------------------------------------------------------------------------------------------
	xPos = 500; yPos = yPos_initial;

	// B1 "Monitor Vsync:": CheckBox (lvl10)
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	//8
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Monitor Vsync:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(107), SystemHandle->m_hinstance, NULL);

	hWndCheckBox.push_back(NULL);
	hWndCheckBox[0] = CreateWindow(TEXT("BUTTON"), TEXT("Monitor Vsync"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
						 xPos + 150, yPos+4, 12, 12, windownTOP, HMENU(300), SystemHandle->m_hinstance, NULL);

	//SET default value:
	if (SystemHandle->AppSettings->VSYNC_ENABLED)
		SendMessage(hWndCheckBox[0], BM_SETCHECK, BST_CHECKED, 0);

#if CORE_ENGINE_LEVEL < 10
	EnableWindow(hWndTitleLabel[8], false);
	EnableWindow(hWndCheckBox[0], false);
#endif
	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// B2 "Network:": CheckBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	//9
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Network:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(108), SystemHandle->m_hinstance, NULL);

	hWndCheckBox.push_back(NULL);
	hWndCheckBox[1] = CreateWindow(TEXT("BUTTON"), TEXT("Network"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
						 xPos + 150, yPos+4, 12, 12, windownTOP, HMENU(301), SystemHandle->m_hinstance, NULL);
	EnableWindow(hWndTitleLabel[9], false);
	EnableWindow(hWndCheckBox[1], false);
	// ---------------------------------------------------------------------------------------------

	yPos += 30;

	// B3 "Sound Effects:": CheckBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	//10
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Sound Effects:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(109), SystemHandle->m_hinstance, NULL);

	hWndCheckBox.push_back(NULL);
	hWndCheckBox[2] = CreateWindow(TEXT("BUTTON"), TEXT("Sound Effects"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
						 xPos + 150, yPos+4, 12, 12, windownTOP, HMENU(302), SystemHandle->m_hinstance, NULL);
	EnableWindow(hWndTitleLabel[10], false);
	EnableWindow(hWndCheckBox[2], false);
	// ---------------------------------------------------------------------------------------------

	yPos += 30;

	// B4 "Background Music:": CheckBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	//11
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Background Music:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 20, windownTOP, HMENU(110), SystemHandle->m_hinstance, NULL);

	hWndCheckBox.push_back(NULL);
	hWndCheckBox[3] = CreateWindow(TEXT("BUTTON"), TEXT("Background Music"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
						 xPos + 150, yPos+4, 12, 12, windownTOP, HMENU(303), SystemHandle->m_hinstance, NULL);
	EnableWindow(hWndTitleLabel[10], false);
	EnableWindow(hWndCheckBox[3], false);

	yPos += 30*4;
	// ---------------------------------------------------------------------------------------------
	// Button START:
	// ---------------------------------------------------------------------------------------------
	m_hBtnOK = CreateWindow(TEXT("BUTTON"), TEXT("OK"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
								xPos, yPos, 80, 20, windownTOP, (HMENU) ButtonStart, SystemHandle->m_hinstance, NULL);
	m_hBtnCancel = CreateWindow(TEXT("BUTTON"), TEXT("CANCEL"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
		85+xPos, yPos, 80, 20, windownTOP, (HMENU)ButtonStart, SystemHandle->m_hinstance, NULL);
	// ---------------------------------------------------------------------------------------------

	// Show Setup Window:
	ShowWindow(windownTOP, WOMA::Cmdshow);
	RedrawWindow(windownTOP, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE | RDW_ERASE);  // Invoke: Window PAINT (with ERASE)

	return true;
}


#endif