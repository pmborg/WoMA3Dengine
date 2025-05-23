// --------------------------------------------------------------------------------------------
// Filename: winWomaSetupManager.cpp
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;

#include "OSengine.h"
#if defined CLIENT_SCENE_SETUP
#include "StateMachine.h"
#include "WomaSetupManager.h"
#include <algorithm> 

WomaSetupManager::WomaSetupManager() 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567155;

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

UINT intMonitorcounter = 0;
void ListDisplayModes(TCHAR* deviceName) 
{
    DEVMODE dm = { 0 }; //ZeroMemory(&dm, sizeof(dm));
    int modeNum = 0;
    
    std::wcout << L"Supported display modes for " << deviceName << L":" << std::endl;

    UINT lastID = 0;
    UINT lastW = 0;
    UINT lastH = 0;
    WOMA::WindowDataContainer scr;
    while (true) 
    {
        dm.dmSize = sizeof(dm);
        if (!EnumDisplaySettings(deviceName, modeNum, &dm)) {
            break; // No more modes
        }

        if (dm.dmPelsWidth != lastW || dm.dmPelsHeight != lastH) // New Reswolution?
        {
            lastW = dm.dmPelsWidth;
            lastH = dm.dmPelsHeight;
            resolutionType res = {0};
            res.Width = dm.dmPelsWidth;
            res.Height = dm.dmPelsHeight;
            if (res.Width > MIN_WIDTH_RESOLUTION && res.Height > MIN_HEIGHT_RESOLUTION)
                scr.ScreenResolution.push_back(res);
            else
                std::wcout << L"ignored: ";
            std::wcout << L"[" << modeNum << L"] "
                << dm.dmPelsWidth << L"x" << dm.dmPelsHeight
                << L" @ " << dm.dmDisplayFrequency << L"Hz"
                << L" (" << ((dm.dmFields & DM_DISPLAYFLAGS) ? dm.dmDisplayFlags : 0) << L")"
                << std::endl;
        }

        modeNum++;
    }

    // Sort by Width before storing
    std::sort(scr.ScreenResolution.begin(), scr.ScreenResolution.end(),
        [](const resolutionType& a, const resolutionType& b) {
        if (a.Width == b.Width)
            return a.Height > b.Height;
        return a.Width > b.Width;
    });

    SystemHandle->allWindowsArray.push_back(scr);
    intMonitorcounter++;
    std::wcout << L"----------------------------------------" << std::endl;
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT lprc, LPARAM dwData) {
    intMonitorcounter=0;
    MONITORINFOEX mi;
    mi.cbSize = sizeof(mi);

    if (GetMonitorInfo(hMonitor, &mi)) {
        ListDisplayModes(mi.szDevice);
    }

    return TRUE;
}

bool WomaSetupManager::Initialize(void* Driver)
{
	static int yPos_initial = 100;// 250;
	int xPos = 20, yPos = yPos_initial;

	m_setupWnd = SystemHandle->m_hWnd;
	HWND windownTOP = m_setupWnd;
    UINT hWndTitleLabelIdx = 0;

	// hWndTitleLabel[1] "Display Mode": ComboBox (lvl:5)
    // hWndComboBox[0]
	// ---------------------------------------------------------------------------------------------
	DWORD Style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL | WS_CLIPCHILDREN;
	
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Display Mode:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
		xPos, yPos, 130, 22, windownTOP, HMENU(100), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[0] = CreateWindow(TEXT("COMBOBOX"), TEXT("Display Mode"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(200), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[0], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("Windowed")));
	SendMessage(hWndComboBox[0], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Full Screen")));
	SendMessage(hWndComboBox[0], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Windowed Fullscreen")));

    if (SystemHandle->AppSettings->FULLSCREEN_ON_WINDOWED)
        SendMessage(hWndComboBox[0], CB_SETCURSEL, 2, NULL); //Default Value
    else if (SystemHandle->AppSettings->FULL_SCREEN)
		SendMessage(hWndComboBox[0], CB_SETCURSEL, 1, NULL); //Default Value
	else
		SendMessage(hWndComboBox[0], CB_SETCURSEL, 0, NULL); //Default Value
	
    //#if !defined USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE && CORE_ENGINE_LEVEL >= 10
	//EnableWindow(hWndComboBox[0], false);
	//#endif
    // ---------------------------------------------------------------------------------------------
	yPos += 30;

	// hWndTitleLabel[2] "UI Monitor:": ComboBox (lvl:5)
    // hWndComboBox[1]
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("UI Monitor:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(101), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[1] = CreateWindow(TEXT("COMBOBOX"), TEXT("UI Monitor"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(201), SystemHandle->m_hinstance, NULL);

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		EnableWindow(hWndComboBox[1], false);

	TCHAR str[50];
	for (int mon = 0; mon < SystemHandle->info.Count; mon++)
	{
		//float Refresh = (float)m_driver->ScreenResolution[res].RefreshRate_Numerator / (float)m_driver->ScreenResolution[res].RefreshRate_Denominator;
		//StringCchPrintf(str, sizeof(str), TEXT("%d %s"), mon, SystemHandle->monitorArray[mon].szDevice);
        StringCchPrintf(str, sizeof(str), TEXT("%d"), mon);
		SendMessage(hWndComboBox[1], CB_ADDSTRING, mon, reinterpret_cast<LPARAM>(str));
	}

	SendMessage(hWndComboBox[1], CB_SETCURSEL, SystemHandle->AppSettings->UI_MONITOR, NULL); //Default Value
    // ---------------------------------------------------------------------------------------------

	{
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
			xPos, yPos, 130, 22, windownTOP, HMENU((HMENU)(UINT_PTR)(mon * 100 + 1010)), SystemHandle->m_hinstance, NULL); //Note: https://stackoverflow.com/questions/3569859/questions-regarding-warning-c4312-type-cast

		hWndComboBoxperMonitor.push_back(NULL);
		hWndComboBoxperMonitor[mon] = CreateWindow(TEXT("COMBOBOX"), TEXT("Resolution"), Style,
			xPos + 150, yPos, 200, 6*40/*80*/, windownTOP, HMENU((HMENU)(UINT_PTR)(mon * 100 + 2010)), SystemHandle->m_hinstance, NULL);

		TCHAR str[50];
#if _NOT //CORE_ENGINE_LEVEL >= 10
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
            for (size_t i = 0; i < SystemHandle->allWindowsArray[mon].ScreenResolution.size(); i++)
            {
                int width = SystemHandle->allWindowsArray[mon].ScreenResolution[i].Width;
                int heigth = SystemHandle->allWindowsArray[mon].ScreenResolution[i].Height;
                StringCchPrintf(str, sizeof(str), TEXT("%d x %d"), width, heigth);
                SendMessage(hWndComboBoxperMonitor[mon], CB_ADDSTRING, i, reinterpret_cast<LPARAM>(str));
            }
			
			SendMessage(hWndComboBoxperMonitor[mon], CB_SETCURSEL, 0, NULL); //Default Value: index: 0
		}
#endif
#if _NOT !defined DX_ENGINE
		if (!SystemHandle->AppSettings->FULL_SCREEN)
		{
		EnableWindow(hWndTitleLabelperMonitor[mon], false);
		EnableWindow(hWndComboBoxperMonitor[mon], false);
		}
#endif
	  }
	}
//#endif
	// ---------------------------------------------------------------------------------------------
    yPos += 30*2;

    // ---------------------------------------------------------------------------------------------
    // hWndTitleLabel[0] "Monitor Vsync:": CheckBox (lvl10)
    // hWndCheckBox[0]
    // ---------------------------------------------------------------------------------------------
    hWndTitleLabel.push_back(NULL);

    hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Monitor Vsync:"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
        xPos, yPos, 130, 22, windownTOP, HMENU(107), SystemHandle->m_hinstance, NULL);

    hWndCheckBox.push_back(NULL);
    hWndCheckBox[0] = CreateWindow(TEXT("BUTTON"), TEXT("Monitor Vsync"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        xPos + 150, yPos + 4, 12, 12, windownTOP, HMENU(300), SystemHandle->m_hinstance, NULL);

    //SET default value:
    if (SystemHandle->AppSettings->VSYNC_ENABLED)
        SendMessage(hWndCheckBox[0], BM_SETCHECK, BST_CHECKED, 0);

#if CORE_ENGINE_LEVEL < 10
    EnableWindow(hWndTitleLabel[hWndTitleLabelIdx - 1], false);
    EnableWindow(hWndCheckBox[0], false);
#endif

    // ---------------------------------------------------------------------------------------------
    yPos += 30;

    // 7 DRIVER ComboBox
    // ---------------------------------------------------------------------------------------------
    hWndTitleLabel.push_back(NULL);
    hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Driver:"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
        xPos, yPos, 130, 22, windownTOP, HMENU(107), SystemHandle->m_hinstance, NULL);

    hWndComboBox.push_back(NULL);
    hWndComboBox[2] = CreateWindow(TEXT("COMBOBOX"), TEXT("Driver"), Style,
        xPos + 150, yPos, 200, 3 * 40/*80=1+3 80=1+2*/, windownTOP, HMENU(207), SystemHandle->m_hinstance, NULL);

#if defined DX11 //Pure DX11
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("DX11")));
#else
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("DX11 (not suppported)")));
#endif
#if (defined OPENGL3 || defined OPENGL40) 
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("GL3")));
#else
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("GL3 (not suppported)")));
#endif
#if defined DX9sdk
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("DX9")));
#elif defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("DX9(onDX11)")));
#else
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("DX9(not suppported)")));
#endif
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("DX12")));
#else
    SendMessage(hWndComboBox[2], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("DX12 (not suppported)")));
#endif

    SendMessage(hWndComboBox[2], CB_SETCURSEL, SystemHandle->AppSettings->DRIVER, NULL); //Default Value
#if !defined DX_ENGINE
    EnableWindow(hWndTitleLabel[hWndTitleLabelIdx - 1], false);
    EnableWindow(hWndComboBox[2], false);
#endif

    // ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 0 "Multisampling:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("MSAA:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(102), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[3] = CreateWindow(TEXT("COMBOBOX"), TEXT("MSAA"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(202), SystemHandle->m_hinstance, NULL);

	//SendMessage(hWndComboBox[3], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("disable")));

	StringCchPrintf(str, sizeof(str), TEXT("Disabled" ));
	SendMessage(hWndComboBox[3], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str));
	SendMessage(hWndComboBox[3], CB_SETCURSEL, 0, NULL); //Default Value

	EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
	EnableWindow(hWndComboBox[3], false);

	//if (!m_driver->mEnable4xMsaa) 
	//	EnableWindow(hWndComboBox[3], FALSE); // Disable Options if mEnable4xMsaa is false.
	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 1 "Texture Resolution:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Texture Resolution:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(103), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[4] = CreateWindow(TEXT("COMBOBOX"), TEXT("Texture Resolution"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(203), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[4], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("High")));
	SendMessage(hWndComboBox[4], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Good")));
	SendMessage(hWndComboBox[4], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Fair")));
	SendMessage(hWndComboBox[4], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("Low")));

	SendMessage(hWndComboBox[4], CB_SETCURSEL, 0, NULL); //Default Value

	EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
	EnableWindow(hWndComboBox[4], false);

	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 2 "View Distance:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("View Distance:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(104), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[5] = CreateWindow(TEXT("COMBOBOX"), TEXT("View Distance"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(204), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[5], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("High")));
	SendMessage(hWndComboBox[5], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Good")));
	SendMessage(hWndComboBox[5], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Fair")));
	SendMessage(hWndComboBox[5], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("Low")));

	EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
	EnableWindow(hWndComboBox[5], false);

	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 5 "Particle Density:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Particle Density:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(105), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[6] = CreateWindow(TEXT("COMBOBOX"), TEXT("Particle Density"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(205), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[6], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("High")));
	SendMessage(hWndComboBox[6], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("Good")));
	SendMessage(hWndComboBox[6], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("Fair")));
	SendMessage(hWndComboBox[6], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("Low")));
	EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
	EnableWindow(hWndComboBox[6], false);
	// ---------------------------------------------------------------------------------------------
	yPos += 30;

	// 6 "Network Messages:": ComboBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Network Messages:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(106), SystemHandle->m_hinstance, NULL);

	hWndComboBox.push_back(NULL);
	hWndComboBox[7] = CreateWindow(TEXT("COMBOBOX"), TEXT("Network Messages"), Style,
									xPos + 150, yPos, 200, 80, windownTOP, HMENU(206), SystemHandle->m_hinstance, NULL);

	SendMessage(hWndComboBox[7], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("60 msg/s High")));
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 1, reinterpret_cast<LPARAM>(TEXT("30 msg/s Good")));
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 2, reinterpret_cast<LPARAM>(TEXT("20 msg/s Fair")));
	SendMessage(hWndComboBox[7], CB_ADDSTRING, 3, reinterpret_cast<LPARAM>(TEXT("10 msg/s Low")));
	EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
	EnableWindow(hWndComboBox[7], false);


	yPos += 30*2;



	// B3 "Sound Effects:": CheckBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	//10
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Sound Effects:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(109), SystemHandle->m_hinstance, NULL);

	hWndCheckBox.push_back(NULL);
	hWndCheckBox[1] = CreateWindow(TEXT("BUTTON"), TEXT("Sound Effects"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
						 xPos + 150, yPos+4, 12, 12, windownTOP, HMENU(302), SystemHandle->m_hinstance, NULL);
	EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
	EnableWindow(hWndCheckBox[1], false);
	// ---------------------------------------------------------------------------------------------

	yPos += 30;

	// B4 "Background Music:": CheckBox
	// ---------------------------------------------------------------------------------------------
	hWndTitleLabel.push_back(NULL);
	//11
	hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Music:"),  WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
									xPos, yPos, 130, 22, windownTOP, HMENU(110), SystemHandle->m_hinstance, NULL);

	hWndCheckBox.push_back(NULL);
	hWndCheckBox[2] = CreateWindow(TEXT("BUTTON"), TEXT("Music"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
						 xPos + 150, yPos+4, 12, 12, windownTOP, HMENU(303), SystemHandle->m_hinstance, NULL);
	EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
	EnableWindow(hWndCheckBox[2], false);

    yPos += 30 * 2;

    // B2 "Network:": CheckBox
    // ---------------------------------------------------------------------------------------------
    hWndTitleLabel.push_back(NULL);
    //9
    hWndTitleLabel[hWndTitleLabelIdx++] = CreateWindow(TEXT("STATIC"), TEXT("Network:"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER,
        xPos, yPos, 130, 22, windownTOP, HMENU(108), SystemHandle->m_hinstance, NULL);

    hWndCheckBox.push_back(NULL);
    hWndCheckBox[3] = CreateWindow(TEXT("BUTTON"), TEXT("Network"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        xPos + 150, yPos + 4, 12, 12, windownTOP, HMENU(301), SystemHandle->m_hinstance, NULL);
    EnableWindow(hWndTitleLabel[hWndTitleLabelIdx-1], false);
    EnableWindow(hWndCheckBox[3], false);
    // ---------------------------------------------------------------------------------------------

    

	yPos += 30*2;
	// ---------------------------------------------------------------------------------------------
	// Button ON / CANCEL:
	// ---------------------------------------------------------------------------------------------
	m_hBtnOK = CreateWindow(TEXT("BUTTON"), TEXT("OK"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
								xPos, yPos, 80, 22, windownTOP, (HMENU) ButtonStart, SystemHandle->m_hinstance, NULL);
	m_hBtnCancel = CreateWindow(TEXT("BUTTON"), TEXT("CANCEL"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
		85+xPos, yPos, 80, 22, windownTOP, (HMENU)ButtonStart, SystemHandle->m_hinstance, NULL);
	// ---------------------------------------------------------------------------------------------

	// Show Setup Window:
	ShowWindow(windownTOP, WOMA::Cmdshow);
	RedrawWindow(windownTOP, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE | RDW_ERASE);  // Invoke: Window PAINT (with ERASE)

	return true;
}


#endif