// --------------------------------------------------------------------------------------------
// Filename: DX11createDevice-and-SwapChain.cpp
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
	{ D3D_DRIVER_TYPE_WARP,	   TEXT("emulator")},
	{ D3D_DRIVER_TYPE_REFERENCE, TEXT("software")},
};

UINT numElements = sizeof(DriverTypes) / sizeof(DriverTypes[0]);

D3D_FEATURE_LEVEL featureLevel_ = D3D_FEATURE_LEVEL_11_0;	// OUTPUT: (createDevice) The address of the feature level that was selected

namespace DirectX {
	//Init Step: 3
	// ----------------------------------------------------------------------------------------------
	bool DX11Class::createDevice()
		// ----------------------------------------------------------------------------------------------
	{
		HRESULT result = S_OK;

		// From most desired to least desired:
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		D3D_FEATURE_LEVEL featureLevelsDX9[] = { D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1 }; 

		//	More info: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476107%28v=vs.85%29.aspx
		//	NOTE: "DirectX SDK_June2010" ("DEBUG version") have to be installed, to use: "D3D11_CREATE_DEVICE_DEBUG"
		UINT DeviceFlags = 0;

		D3D_FEATURE_LEVEL* PtrfeatureLevels = featureLevels;
		if (dx11_force_dx9) {
			PtrfeatureLevels = featureLevelsDX9;
		} else {
	#if !defined DX11_ALLOW_MULTI_THREAD
		DeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
	#endif

	#if defined DX11_ALLOW_BGRA_SUPPORT
		DeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	#endif

	#if defined(_DEBUG) & !defined(NDEBUG)
		DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		}

		for (UINT i = 0; i < numElements; ++i)
		{
			//NOTE: DX11 might fallback to "D3D_FEATURE_LEVEL_10_1" or 
			//		even "D3D_FEATURE_LEVEL_10_0" if DX11 is not supported

			result = D3D11CreateDevice(0,						//1st: adapterGraphicCard
				DriverTypes[i].DX11Type,// Driver type
				NULL,					//used only by: D3D_DRIVER_TYPE_SOFTWARE
				DeviceFlags,
				PtrfeatureLevels, 3		/*ARRAYSIZE(featureLevels)*/,
				D3D11_SDK_VERSION,
				&m_device,
				&featureLevel_,			// OUTPUT: The address of the feature level that was selected
				&m_deviceContext);		// OUTPUT: The address for the rendering context

			if (result && i < numElements - 1)
			{
				WOMA_LOGManager_DebugMSGAUTO(TEXT("Could not create Direct3D11 device in %s mode; using driver: %s mode"),
					DriverTypes[i].Name, DriverTypes[i + 1].Name);
			}
			else
			{
				m_sCapabilities.SelectedDriverType = i;
				break;
			}
		}

		if (result || !m_device)
		{
			WomaFatalExceptionW(TEXT("FATAL ERROR: Could not Create DX 11 Device: D3D11CreateDevice")); return false;
		}

		g_deviceContext = m_deviceContext; // Save the context in a global variable to be used and abused! :)

		g_ALLOW_DX9x = dx11_force_dx9;

		// OK, We have a working DX11 API, lets find out at which level: we got it.
		if (g_ALLOW_DX9x)
		{
			// Check min. HW level required: DX9.X capabilities, if not --> FATAL
			if (featureLevel_ < D3D_FEATURE_LEVEL_9_3)
			{
				WomaFatalExceptionW(TEXT("DirectX 9.3 not supported.")); return false;
			}
		}
		else {
			// if we cant use DX9 and dont have DX10 --> FATAL
			if (featureLevel_ < D3D_FEATURE_LEVEL_10_0)
			{
				WomaFatalExceptionW(TEXT("DirectX 10 not supported.")); return false;
			}
		}

		// Check driver capabilities:
		setDeviceCapabilities(featureLevel_);

		// Check DX11 Multi-Threading Capabilities:
		// -------------------------------------------------------------------------
		D3D11_FEATURE_DATA_THREADING ThreadingOptions;
		m_device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &ThreadingOptions, sizeof(ThreadingOptions));

		WOMA_LOGManager_DebugMSGAUTO(TEXT("Driver Support Concurrent Creates: %s\n"), ThreadingOptions.DriverConcurrentCreates ? TEXT("yes") : TEXT("no"));
		WOMA_LOGManager_DebugMSGAUTO(TEXT("Driver Support Command Lists: %s\n\n"), ThreadingOptions.DriverCommandLists ? TEXT("yes") : TEXT("no"));

		// SOURCE INFO: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476330(v=vs.85).aspx
		D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hwopts = { 0 };
		m_device->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hwopts, sizeof(hwopts));

		// -------------------------------------------------------------------------
		// Get the best Multi Sample Quality (MSAAmultiSampleCount & MSAAquality)
		// -------------------------------------------------------------------------
		m_sCapabilities.MSAA_SUPPORTBoolean = false; // Lets check this...

		// Defaults:
		m_sCapabilities.MSAAmultiSampleCount = 1;
		m_sCapabilities.MSAAquality = 1;

		// Detect Max Capabilities:


		// Check "4X" MSAA quality support for our back buffer format.
		// All Direct3D 11 capable devices support "4X" MSAA for all render target formats, so we only need to check quality support.
		if (!FAILED(m_device->CheckMultisampleQualityLevels(BUFFER_COLOR_FORMAT, 4, &m_sCapabilities.MSAAquality))) //WomaFatalException("Failed to check multisample support!");
		{
			if (m_sCapabilities.MSAAquality <= 0)
			{
				SystemHandle->AppSettings->MSAA_ENABLED = FALSE;
				WOMA::WomaMessageBox(TEXT("WARNING: This card don't support, Multisample anti-aliasing")); // NOTE: Don't make it fatal (just reset setting)
			}
			else
			{
				m_sCapabilities.MSAA_SUPPORTBoolean = true;

				if (ReqMSAAmultiSampleCount == 0) // 0 = Detect Max, other setting: Use it!
				{
					UINT quality = 0;
					for (UINT msaaSamples_ = 1; msaaSamples_ <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; msaaSamples_++)
					{
						result = m_device->CheckMultisampleQualityLevels(BUFFER_COLOR_FORMAT, msaaSamples_, &quality);
						if (result == S_OK && quality != 0)
						{
							m_sCapabilities.MSAAmultiSampleCount = msaaSamples_;
							m_sCapabilities.MSAAquality = quality;
							WOMA_LOGManager_DebugMSGAUTO(TEXT("DRIVER MSAAmultiSampleCount: %d\n"), m_sCapabilities.MSAAmultiSampleCount);		// Get the max Sample Count: 8
							WOMA_LOGManager_DebugMSGAUTO(TEXT("DRIVER multiSampleQuality: %d\n"), m_sCapabilities.MSAAquality - 1);	// Get the max MsaaQuality: 32

							// Use Max Setting Supported:
							ReqMSAAquality = m_sCapabilities.MSAAquality;
							ReqMSAAmultiSampleCount = m_sCapabilities.MSAAmultiSampleCount;

							//if (ReqMSAAmultiSampleCount >0 && ReqMSAAmultiSampleCount == m_sCapabilities.MSAAmultiSampleCount)
							//	break;
						}
					}
				}
			}
		}
		if (!SystemHandle->AppSettings->MSAA_ENABLED)
		{
			ReqMSAAquality = 1;
			ReqMSAAmultiSampleCount = 1;
		}

		// Log It!
		if (SystemHandle->AppSettings->MSAA_ENABLED)
			WOMA_LOGManager_DebugMSGAUTO(TEXT("MSSA setting is Enabled (%d Samples)\n"), ReqMSAAmultiSampleCount);
		else
			WOMA_LOGManager_DebugMSGAUTO(TEXT("MSSA setting is Disabled\n"));

		return true;
	}

	//Init Step: 4
	// ----------------------------------------------------------------------------------------------
	bool DX11Class::createSwapChainDX11device(HWND hwnd, int screenWidth, int screenHeight, BOOL vsync,
		BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize,
		UINT numerator, UINT denominator)
		// ----------------------------------------------------------------------------------------------
	{
		HRESULT result = S_OK;

		if (dx11_force_dx9)
			g_UseDoubleBuffering = FALSE;

		ASSERT(hwnd); // FATAL ERROR: Create Main Window first!

		// Fill out the description of the swap chain
		// ==========================================
		// Initialize the swap chain description:
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };	// ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single/double-buffering back buffer:
		swapChainDesc.BufferCount = (g_UseDoubleBuffering) ? 2 : 1; // Use double-buffering to minimize latency.

		// Set the width and height of the back buffer:
		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;

		// The default: 32-bit surface for the back buffer: (RGB + A) 8 bits each (This is the most common swap chain format)
		swapChainDesc.BufferDesc.Format = BUFFER_COLOR_FORMAT; // Default: DXGI_FORMAT_R8G8B8A8_UNORM

		// Set the refresh rate of the back buffer:
		swapChainDesc.BufferDesc.RefreshRate.Numerator = (vsync) ? numerator : 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = (vsync) ? denominator : 1;

		// Set the default usage of the back buffer:
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapChainDesc.Windowed = !fullscreen;	// Set to "Fullscreen" or "Windowed Mode":

		// Default: 0, This member is used to tell Direct3D how to perform multisample anti-aliased (MSAA) rendering	
		swapChainDesc.SampleDesc.Count = ReqMSAAmultiSampleCount;
		swapChainDesc.SampleDesc.Quality = ReqMSAAquality - 1;		// MSAA

		swapChainDesc.Flags = (g_AllowResize) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

		// Set the scan line ordering and scaling to unspecified:
		//swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting:
#if D3D11_SPEC_DATE_YEAR > 2009
		if (swapChainDesc.SampleDesc.Count == 1)
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		else
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
#endif

		IDXGIDevice* pDXGIDevice = NULL;
		result = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
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
			DXwindow.m_backBuffer = NULL;
			DXwindow.m_renderTargetView = NULL;
			DXwindow.m_depthStencilView = NULL;

			// Windows 10 and up:
			swapChainDesc.OutputWindow = SystemHandle->windowsArray[i].hWnd;						// Set the handle for the window to render to.
			WOMA_LOGManager_DebugMSGAUTO(TEXT("Try CreateSwapChain settings for Windows 10:\n"));
			result = pIDXGIFactory->CreateSwapChain(m_device, &swapChainDesc, &DXwindow.m_swapChain); // Turn Screen to Black
			if (FAILED(result))
			{
				//WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("DX11 ERROR - CreateSwapChain(): %s\n"), std::system_category().message(result));
				// Windows 8.1:
#if D3D11_SPEC_DATE_YEAR > 2009
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
				swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
#endif
				swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				WOMA_LOGManager_DebugMSGAUTO(TEXT("Try CreateSwapChain settings for Windows 8.1:\n"));
				result = pIDXGIFactory->CreateSwapChain(m_device, &swapChainDesc, &DXwindow.m_swapChain); // Are we ok now?
				if (FAILED(result)) {
					//WOMA_LOGManager_DebugMSGAUTO(TEXT("DX11 ERROR - CreateSwapChain(): %s\n"), std::system_category().message(result));
					// older Windows
					swapChainDesc.BufferCount = 1;
					swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
					swapChainDesc.Flags = 0;
					WOMA_LOGManager_DebugMSGAUTO(TEXT("Try CreateSwapChain settings for Windows legacy:\n"));
					result = pIDXGIFactory->CreateSwapChain(m_device, &swapChainDesc, &DXwindow.m_swapChain); // Are we ok now?
					if (FAILED(result)) {
						//WOMA_LOGManager_DebugMSGAUTO(TEXT("DX11 FATAL ERROR - CreateSwapChain(): %s\n"), std::system_category().message(result));
						return false;
					}
				}
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

}

