// --------------------------------------------------------------------------------------------
// Filename: DX11createDevice-and-SwapChain.cpp
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
#if defined DX_ENGINE
#include <d3d11.h>
#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
#include "dx11Class.h"
#include "winsystemclass.h"	// SystemHandle
#include <system_error>

/* List for each possible Direct3D11 device type */
struct
{
	D3D_DRIVER_TYPE		DX11Type;
	STRING Name;
}
DriverTypes[] =
{
	{ D3D_DRIVER_TYPE_HARDWARE,	 TEXT("hardware")},
	#if !defined USE_DX_DRIVER_FONT
	{ D3D_DRIVER_TYPE_WARP,	   TEXT("emulator")},
	{ D3D_DRIVER_TYPE_REFERENCE, TEXT("software")},
	#endif
};

UINT numElements = sizeof(DriverTypes) / sizeof(DriverTypes[0]);

namespace DirectX {

	//Init Step: 3
	// ----------------------------------------------------------------------------------------------
	bool DX11Class::createDevice_legacy()
		// ----------------------------------------------------------------------------------------------
	{
		HRESULT result = S_OK;
		IDXGIFactory* factory=nullptr;
		IDXGIAdapter* adapter = nullptr;
		IDXGIOutput* adapterOutput = nullptr;
		unsigned int numModes, i, numerator = 1, denominator = 1;
		DXGI_MODE_DESC* displayModeList = nullptr;
		DXGI_ADAPTER_DESC adapterDesc;

		#define screenWidth		SystemHandle->AppSettings->WINDOW_WIDTH
		#define screenHeight	SystemHandle->AppSettings->WINDOW_HEIGHT

#if _DEBUG
		result = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDev));
#endif

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		IF_FAILED_RETURN_FALSE(result);

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		IF_FAILED_RETURN_FALSE(result);

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		IF_FAILED_RETURN_FALSE(result);

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		IF_FAILED_RETURN_FALSE(result);

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		IF_NOT_RETURN_FALSE(displayModeList);

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		IF_FAILED_RETURN_FALSE(result);

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)screenWidth)
			{
				if (displayModeList[i].Height == (unsigned int)screenHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		IF_FAILED_RETURN_FALSE(result);

		// Store the dedicated video card memory in megabytes.
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
#if UNICODE
		wtoa(m_videoCardDescription, adapterDesc.Description, 128);
#else
		unsigned long long stringLength;
		int error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128); // convert WCHAR adapterDesc.Description --> CHAR m_videoCardDescription
		if (error != 0)
		{
			return false;
		}
#endif
		// Release the display mode list.
		delete[] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;
        
		// From most desired to least desired:
		D3D_FEATURE_LEVEL featureLevels[] = {
		#if defined USE_DX11_3
			D3D_FEATURE_LEVEL_12_2,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
		#endif
		#if defined USE_DX11_1
			D3D_FEATURE_LEVEL_11_1,
		#endif
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1, 
			D3D_FEATURE_LEVEL_10_0 
		};
		D3D_FEATURE_LEVEL featureLevelsDX9[] = { 
			D3D_FEATURE_LEVEL_9_3, 
			D3D_FEATURE_LEVEL_9_2, 
			D3D_FEATURE_LEVEL_9_1,
			D3D_FEATURE_LEVEL_1_0_CORE
		}; 

		//	More info: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476107%28v=vs.85%29.aspx
		//	NOTE: "DirectX SDK_June2010" ("DEBUG version") have to be installed, to use: "D3D11_CREATE_DEVICE_DEBUG"
		UINT DeviceFlags = 0;

		int num_levels = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);
		D3D_FEATURE_LEVEL* PtrfeatureLevels = featureLevels;

		if (dx11_force_dx9) {
			PtrfeatureLevels = featureLevelsDX9;
			num_levels = sizeof(featureLevelsDX9) / sizeof(D3D_FEATURE_LEVEL);
		} else {

	//#if !defined DX11_ALLOW_MULTI_THREAD
	//	DeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
	//#endif

	#if defined DX11_ALLOW_BGRA_SUPPORT
		DeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	#endif

	#if defined(_DEBUG) & !defined(NDEBUG)
		DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		}
		
	#undef screenWidth
	#undef screenHeight

		//DEVICE_TYPES:
		// "hardware"
		// "emulator"
		// "software"

		for (UINT device_type = 0; device_type < numElements; device_type)
		{
		retry:
			//NOTE: DX11 might fallback to "D3D_FEATURE_LEVEL_10_1" or even "D3D_FEATURE_LEVEL_10_0" if DX11 is not supported
			result = D3D11CreateDevice(
                USE_THIS_GRAPHIC_CARD_ADAPTER,		//0 = 1st: adapterGraphicCard
				DriverTypes[device_type].DX11Type,	// device_type
				NULL,								// used only by: D3D_DRIVER_TYPE_SOFTWARE
				DeviceFlags,
				PtrfeatureLevels, num_levels/*ARRAYSIZE(featureLevels)*/,
				D3D11_SDK_VERSION,
                &m_device11,
				&featureLevel_,				// OUTPUT: The address of the feature level that was selected
				&m_deviceContext);			// OUTPUT: The address for the rendering context

			if (result != S_OK  && device_type < numElements - 1)
			{
				WOMA_LOGManager_DebugMSG(TEXT("Could not create Direct3D11 device in %s mode - Levels: %d\n"), DriverTypes[device_type].Name.c_str(), num_levels);
				PtrfeatureLevels++;
				num_levels--;
				if (num_levels > 0)
					goto retry;
				WOMA_LOGManager_DebugMSG(TEXT("Could not create Direct3D11 device in %s mode; using driver: %s mode - Levels: %d\n"), DriverTypes[device_type].Name.c_str(), DriverTypes[device_type + 1].Name.c_str(), num_levels);
			}
			else
			{
				m_sCapabilities.SelectedDriverType = device_type;
				break;
			}
		}

		//m_device11 = *m_device.GetAddressOf();
		if (result != S_OK || !m_device11)
		{
			WomaFatalException(("FATAL ERROR: Could not Create DX 11 Device: D3D11CreateDevice")); //CHAR!
		}


		g_ALLOW_DX9x = dx11_force_dx9;

		// OK, We have a working DX11 API, lets find out at which level: we got it.
		if (g_ALLOW_DX9x)
		{
			// Check min. HW level required: DX9.X capabilities, if not --> FATAL
			if (featureLevel_ < D3D_FEATURE_LEVEL_9_3)
			{
				WomaFatalException(("DirectX 9.3 not supported.")); //return false;
			}
		}
		else {
			// if we cant use DX9 and dont have DX10 --> FATAL
			if (featureLevel_ < D3D_FEATURE_LEVEL_10_0)
			{
				WomaFatalException(("DirectX 10 not supported.")); //return false;
			}
		}

		// Check driver capabilities:
		// Defaults:
		m_sCapabilities.MSAAmultiSampleCount = 1;
		m_sCapabilities.MSAAquality = 1;

#if defined SET_DEVICE_CAPABILITIES
		setDeviceCapabilities(featureLevel_);

		// Check DX11 Multi-Threading Capabilities:
		// -------------------------------------------------------------------------
		D3D11_FEATURE_DATA_THREADING ThreadingOptions;
		m_device11->CheckFeatureSupport(D3D11_FEATURE_THREADING, &ThreadingOptions, sizeof(ThreadingOptions));

		WOMA_LOGManager_DebugMSGAUTO(TEXT("Driver Support Concurrent Creates: %s\n"), ThreadingOptions.DriverConcurrentCreates ? TEXT("yes") : TEXT("no"));
		WOMA_LOGManager_DebugMSGAUTO(TEXT("Driver Support Command Lists: %s\n\n"), ThreadingOptions.DriverCommandLists ? TEXT("yes") : TEXT("no"));

		// -------------------------------------------------------------------------
		// Get the best Multi Sample Quality (MSAAmultiSampleCount & MSAAquality)
		// -------------------------------------------------------------------------
		m_sCapabilities.MSAA_SUPPORTBoolean = false; // Lets check this...

		// Detect Max Capabilities:

		// Check "4X" MSAA quality support for our back buffer format.
		// All Direct3D 11 capable devices support "4X" MSAA for all render target formats, so we only need to check quality support.
		if (!FAILED(m_device11->CheckMultisampleQualityLevels(BUFFER_COLOR_FORMAT, 4, &m_sCapabilities.MSAAquality))) //WomaFatalException("Failed to check multisample support!");
		{
			if (m_sCapabilities.MSAAquality <= 0)
			{
				//SystemHandle->AppSettings->MSAA_ENABLED = FALSE;
				WomaMessageBox(TEXT("WARNING: This card don't support, MultiSample Anti-Aliasing (MSAA)"), TEXT("WARNING")); // NOTE: Don't make it fatal (just reset setting)
			}
			else
			{
				// Support at least 4:
				m_sCapabilities.MSAA_SUPPORTBoolean = true;

				if (MSAA_COUNT == 0) // 0 = Auto Detect Max!
				{
					UINT quality = 0;
					for (UINT msaaSamples_ = 1; msaaSamples_ <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; msaaSamples_++)
					{
						result = m_device11->CheckMultisampleQualityLevels(BUFFER_COLOR_FORMAT, msaaSamples_, &quality);
						if (result == S_OK && quality != 0)
						{
							m_sCapabilities.MSAAmultiSampleCount = msaaSamples_;
							m_sCapabilities.MSAAquality = quality;
							WOMA_LOGManager_DebugMSGAUTO(TEXT("DRIVER MSAAmultiSampleCount: %d\n"), m_sCapabilities.MSAAmultiSampleCount);		// Get the max Sample Count: 8
							WOMA_LOGManager_DebugMSGAUTO(TEXT("DRIVER multiSampleQuality: %d\n"), m_sCapabilities.MSAAquality);	// Get the max MsaaQuality: 32

							// Use Max Setting Supported:
							MSAA_QUALITY = m_sCapabilities.MSAAquality;
							MSAA_COUNT = MIN(4, m_sCapabilities.MSAAmultiSampleCount);

						}
					}
				}
			}
		}

		if (SystemHandle->AppSettings->MSAA_Anisotropic == false) //Setup defaults!
		{
			MSAA_QUALITY = 0;
			MSAA_COUNT = 1;
		}

		// Log It!
		if (SystemHandle->AppSettings->MSAA_Anisotropic) {
			WOMA_LOGManager_DebugMSGAUTO(TEXT("MSSA is Enabled with %d Samples\n"), MSAA_COUNT);
		}
		else
		{
			if (SystemHandle->AppSettings->MSAA_bilinear)
				WOMA_LOGManager_DebugMSGAUTO(TEXT("Antialise: bilinear\n"));
			else if (SystemHandle->AppSettings->MSAA_trilinear)
				WOMA_LOGManager_DebugMSGAUTO(TEXT("Antialise: trilinear\n"));
			else
				WOMA_LOGManager_DebugMSGAUTO(TEXT("Antialise: off\n"));
		}
																 
#endif

		return true;
}

	//Init Step: 4
	#if defined USE_DX11_1
	// ----------------------------------------------------------------------------------------------
	bool DX11Class::createSwapChainDX11device2(HWND hwnd, int screenWidth, int screenHeight, BOOL vsync,
		BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize, UINT numerator, UINT denominator)
	// ----------------------------------------------------------------------------------------------
	{
		HRESULT result = S_OK;

		if (dx11_force_dx9)
			g_UseDoubleBuffering = FALSE;

		ASSERT(hwnd); // FATAL ERROR: Create Main Window first!

		// Fill out the description of the swap chain
		// ==========================================
		// Initialize the swap chain description:
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };	// dxgi.h

		// Set to a single/double-buffering back buffer:
		swapChainDesc.BufferCount = (g_UseDoubleBuffering) ? 2 : 1; // Use double-buffering to minimize latency.

		// Set the default usage of the back buffer:
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the width and height of the back buffer:
		swapChainDesc.Width = screenWidth;
		swapChainDesc.Height = screenHeight;

		constexpr DXGI_FORMAT formats[] =
		{
			DXGI_FORMAT_R8G8B8A8_UNORM
		};
		swapChainDesc.Format = formats[0];

		// Multisample
		// Default: 0, This member is used to tell Direct3D how to perform multisample anti-aliased (MSAA) rendering	
		swapChainDesc.SampleDesc.Count = MSAA_COUNT;
		swapChainDesc.SampleDesc.Quality = MSAA_QUALITY;		// MSAA

		// Windoze
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;

		//swapChainDesc.Windowed = !fullscreen;	// Set to "Fullscreen" or "Windowed Mode":
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fulldesc{};
		fulldesc.Windowed = !fullscreen;	// Set to "Fullscreen" or "Windowed Mode":

		swapChainDesc.Flags = (g_AllowResize) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

		// Discard the back buffer contents after presenting:
	#if D3D11_SPEC_DATE_YEAR > 2009
		if (swapChainDesc.SampleDesc.Count == 1)
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		else
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	#endif

		IDXGIDevice* pDXGIDevice = NULL;
		result = m_device11->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
		if (FAILED(result)) return false;

	#if defined DXGI1_1 || defined DXGI1_2
		pDXGIDevice->SetGPUThreadPriority(7);
	#endif

		IDXGIAdapterN* pDXGIAdapter = NULL;
		result = pDXGIDevice->GetParent(__uuidof(IDXGIAdapterN), (void**)&pDXGIAdapter);
		if (FAILED(result)) return false;

		if (pDXGIAdapter)
		{
			DXGI_ADAPTER_DESC sDXGIAdapterDesc;
			pDXGIAdapter->GetDesc(&sDXGIAdapterDesc);
			m_sCapabilities.nTotalAvailableGPUMemory = sDXGIAdapterDesc.DedicatedVideoMemory; //unit: bytes
		}

		IDXGIFactoryN* pIDXGIFactory = NULL;
		result = pDXGIAdapter->GetParent(__uuidof(IDXGIFactoryN), (void**)&pIDXGIFactory);
		if (FAILED(result)) return false;

		// For each Monitor: 
		for (int i = 0; i < SystemHandle->windowsArray.size(); i++)
		{
			DXwindowDataContainer DXwindow;
			DXwindow.m_swapChain = NULL;
			DXwindow.m_swapChain1 = NULL;
			DXwindow.m_backBuffer = NULL;
			DXwindow.m_renderTargetView = NULL;
			DXwindow.m_depthStencilView = NULL;

			// Windows 10 and up:
			//swapChainDesc.OutputWindow = SystemHandle->windowsArray[i].hWnd;						// Set the handle for the window to render to.
			WOMA_LOGManager_DebugMSGAUTO(TEXT("Try CreateSwapChain settings for Windows 10:\n"));
			result = pIDXGIFactory->CreateSwapChainForHwnd(m_device11, SystemHandle->windowsArray[i].hWnd , &swapChainDesc, fulldesc.Windowed ? nullptr : &fulldesc, nullptr, &DXwindow.m_swapChain1); // Turn Screen to Black
			if (FAILED(result))
			{
				return false;
			}

			DX11windowsArray.push_back(DXwindow);
		}

		if (!g_AllowResize)
			pIDXGIFactory->MakeWindowAssociation(SystemHandle->windowsArray[0].hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER); //Prevent DXGI from responding to an alt-enter sequence.

		SAFE_RELEASE(pDXGIDevice);
		SAFE_RELEASE(pDXGIAdapter);
		SAFE_RELEASE(pIDXGIFactory);

		return true;
	}
	#endif

}

#endif
#endif
