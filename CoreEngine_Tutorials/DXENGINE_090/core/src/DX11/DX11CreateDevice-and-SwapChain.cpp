// --------------------------------------------------------------------------------------------
// Filename: DX11createDevice-and-SwapChain.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"

#if defined DX_ENGINE

#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
#include "dx11Class.h"
#include "winsystemclass.h"	// SystemHandle
#include <system_error>

namespace DirectX {

#if defined USE_DX11_1_SETUP
	static std::string HRHex(HRESULT hr) {
		std::ostringstream oss;
		oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << static_cast<unsigned long>(hr);
		return oss.str();
	}

	std::wstring DX11Class::HRtoStr(HRESULT hr) {
		wchar_t* buf = nullptr;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, 0, (LPWSTR)&buf, 0, nullptr);
		std::wstring s = buf ? buf : L"(unknown)";
		if (buf) LocalFree(buf);
		return s;
	}

	static bool IsSoftwareAdapter(IDXGIAdapter* a) {
		DXGI_ADAPTER_DESC desc;
		if (FAILED(a->GetDesc(&desc))) return false;
		// Microsoft Basic Render Driver (software)
		return (desc.VendorId == 0x1414 && desc.DeviceId == 0x008C);
	}

	static void EnumerateAdaptersFactory1(IDXGIFactory1* f1, std::vector<ComPtr<IDXGIAdapter> >& out) {
		for (UINT i = 0;; ++i) {
			ComPtr<IDXGIAdapter> a;
			if (f1->EnumAdapters(i, &a) == DXGI_ERROR_NOT_FOUND) break;
			if (!IsSoftwareAdapter(a.Get())) out.push_back(a);
		}
	}

	static void EnumerateAdaptersFactory6(IDXGIFactory6* f6, std::vector<ComPtr<IDXGIAdapter> >& out) {
		// Prefer HIGH_PERFORMANCE, then MINIMUM_POWER
		for (UINT pass = 0; pass < 2; ++pass) {
			DXGI_GPU_PREFERENCE pref = (pass == 0) ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_MINIMUM_POWER;
			for (UINT i = 0;; ++i) {
				ComPtr<IDXGIAdapter1> a1;
				HRESULT hr = f6->EnumAdapterByGpuPreference(i, pref, IID_PPV_ARGS(&a1));
				if (FAILED(hr)) break;
				if (!IsSoftwareAdapter(a1.Get())) out.push_back(a1);
			}
			if (!out.empty()) return;
		}
	}
#endif

bool DX11Class::list_resolutions()
{
		HRESULT result = S_OK;
		IDXGIFactory* factory = nullptr;
		IDXGIAdapter* tstadapter = nullptr;
		IDXGIOutput* adapterOutput = nullptr;
		unsigned int numModes, i, numerator = 1, denominator = 1;
		DXGI_MODE_DESC* displayModeList = nullptr;
		DXGI_ADAPTER_DESC adapterDesc;

		#define screenWidth		SystemHandle->AppSettings->WINDOW_WIDTH
		#define screenHeight	SystemHandle->AppSettings->WINDOW_HEIGHT

#if _DEBUG
		result = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDev));
#endif

		// ==============================
		// LIST ALL POSSIBLE RESOLUTIONS:
		// ==============================
		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		IF_FAILED_RETURN_FALSE(result);

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &tstadapter);
		IF_FAILED_RETURN_FALSE(result);

		// Enumerate the primary adapter output (monitor).
		result = tstadapter->EnumOutputs(0, &adapterOutput);
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
		result = tstadapter->GetDesc(&adapterDesc);
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
		SAFE_RELEASE(adapterOutput);

		// Release the adapter.
		SAFE_RELEASE(tstadapter);

		// Release the factory.
		SAFE_RELEASE(factory);

		#undef screenWidth
		#undef screenHeight
		return true;
}

#if defined(_DEBUG)
	// Check for SDK Layer support.
	inline bool SdkLayersAvailable() noexcept
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
		);

		return SUCCEEDED(hr);
	}
#endif

	// ==============================
	// CREATE DEVICE:
	// ==============================
	// ----------------------------------------------------------------------------------------------
	bool DX11Class::createDevice()
	// ----------------------------------------------------------------------------------------------
	{
		HRESULT result = S_OK;
		UINT DeviceFlags = 0;			//auto-detect
		bool enableDebugLayer = false;	//auto-detect

		// 1) TEST ALL POSSIBLE LEVELS:
		
		// From most desired to least desired:
		D3D_FEATURE_LEVEL featureLevels[] = {
			//D3D_FEATURE_LEVEL_12_2,   //not supported on DX11.x
			D3D_FEATURE_LEVEL_12_1,		//DX11_3 (Win-10) A GPU with feature level 12_1 supports all the features of DirectX 11 and also includes features that are available in DirectX 12.1 (though not all DirectX 12.1 features are necessarily supported). 
			D3D_FEATURE_LEVEL_12_0,		//DX11_3 (Win-10) A GPU with feature level 12_0 supports all the features of DirectX 11 and also includes features that are available in DirectX 12 (though not all DirectX 12 features are necessarily supported). 

			D3D_FEATURE_LEVEL_11_1,		//DX11_1 (Win-8)  Windows Display Driver Model (WDDM) 1.2

			D3D_FEATURE_LEVEL_11_0,		//DX11   (Win-7)
			D3D_FEATURE_LEVEL_10_1, 
			D3D_FEATURE_LEVEL_10_0, 

			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};
		D3D_FEATURE_LEVEL featureLevelsDX9[] = { 
			D3D_FEATURE_LEVEL_9_3, 
			D3D_FEATURE_LEVEL_9_2, 
			D3D_FEATURE_LEVEL_9_1,
		}; 

		int num_levels = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);
		D3D_FEATURE_LEVEL* PtrfeatureLevels = featureLevels;

		if (dx11_force_dx9)
		{
			//USE DX9 in DX11
			PtrfeatureLevels = featureLevelsDX9;
			num_levels = sizeof(featureLevelsDX9) / sizeof(D3D_FEATURE_LEVEL);
		} 
		
		// 2) Setup Device Flags
		if (!dx11_force_dx9) 
		{
			//USE DX11
		#if defined DX11_ALLOW_BGRA_SUPPORT
			DeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		#endif

			// 1) CHECK IF WE WILL RUN SHADERS ON DEBUG:
		#if defined(_DEBUG) & !defined(NDEBUG)
			if (SdkLayersAvailable())
			{
				// If the project is in a debug build, enable debugging via SDK Layers with this flag.
				DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
				enableDebugLayer = true;
			}
			else
			{
				womalog("WARNING: Direct3D Debug Device is not available\n");
			}
		#endif
		}

#if defined USE_DX11_1_SETUP
		D3D_DRIVER_TYPE dt = WOMA::UseWarpDevice ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_UNKNOWN;

		// 3) CREATE DXGI FACTORY: factory1 & factory6
		ComPtr<IDXGIFactory1> factory1;
		ComPtr<IDXGIFactory6> factory6; // optional

		UINT dxgiFlags = enableDebugLayer ? DXGI_CREATE_FACTORY_DEBUG : 0;

		// Try CreateDXGIFactory2 (gives us Factory6 on newer OS/SDK), else fall-back to CreateDXGIFactory1.
		HRESULT hr = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&factory6));
		if (FAILED(hr) || !factory6) {
			// Fall-back
			factory6.Reset();
			hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory1));
			if (FAILED(hr) || !factory1) {
				throw std::runtime_error("DXGI factory creation failed: " + HRHex(hr));
			}
		}
		else {
			// We also want a Factory1 pointer for EnumAdapters fall-back if needed
			factory6.As(&factory1);
		}

		// 4) ENUMERATE HARDWARE ADAPTERS: with factory6 if fail try with legacy factory1
		std::vector<ComPtr<IDXGIAdapter> > adapters;
		if (factory6) EnumerateAdaptersFactory6(factory6.Get(), adapters);
		if (adapters.empty()) EnumerateAdaptersFactory1(factory1.Get(), adapters);

		// 5) CreateDevice: TRY HARDWARE FIRST for DX11.0 interfaces
		hr = E_FAIL;
		for (size_t i = 0; i < adapters.size(); ++i) 
		{
			hr = D3D11CreateDevice(
				adapters[i].Get(),
				dt,
				NULL,
				DeviceFlags,
				PtrfeatureLevels, num_levels,			// ARRAYSIZE
				D3D11_SDK_VERSION,
				m_device.ReleaseAndGetAddressOf(),
				&featureLevel_,							// OUTPUT: The address of the feature level that was selected
				m_Context.ReleaseAndGetAddressOf());	// OUTPUT: The address for the rendering context

			if (SUCCEEDED(hr)) break;
		}

		// 6) Upgrade to DX11.1 interfaces if available
		(void)m_device.As(&m_device1);
		(void)m_Context.As(&m_Context1);

		// 7) Upgrade to DX11.2 interfaces if available
		(void)m_Context.As(&m_Context2);

		// 7.1) Upgrade to DX11.3 interfaces if available
		(void)m_Context.As(&m_Context3);

		m_device11 = m_device.Get();
		m_deviceContext = m_Context.Get();

		// 8) Make the immediate context thread-safe (recommended for multi-threaded engines)
		ComPtr<ID3D11Multithread> mt;
		if (SUCCEEDED(m_Context->QueryInterface(IID_PPV_ARGS(&mt)))) {
			mt->SetMultithreadProtected(TRUE);
		}
		
		// 9) Check Hardware Requirements
		D3D11_FEATURE_DATA_D3D11_OPTIONS options = {};
		hr = m_device->CheckFeatureSupport(
			D3D11_FEATURE_D3D11_OPTIONS,
			&options,
			sizeof(options)
		);

		// OutputMergerLogicOp	1
		// UAVOnlyRenderingForcedSampleCount	1
		// DiscardAPIsSeenByDriver	1
		// FlagsForUpdateAndCopySeenByDriver	1
		// ClearView	1
		// CopyWithOverlap	1
		// ConstantBufferPartialUpdate	1
		// ConstantBufferOffsetting	1
		// MapNoOverwriteOnDynamicConstantBuffer	1
		// MapNoOverwriteOnDynamicBufferSRV	1
		// MultisampleRTVWithForcedSampleCountOne	1
		// SAD4ShaderInstructions	1
		// ExtendedDoublesShaderInstructions	1
		// ExtendedResourceSharing	1

		D3D11_FEATURE_DATA_D3D11_OPTIONS1 options1 = {};
		hr = m_device->CheckFeatureSupport(
			D3D11_FEATURE_D3D11_OPTIONS1,
			&options1,
			sizeof(options1)
		);

		// TiledResourcesTier - D3D11_TILED_RESOURCES_TIER_2
		// MinMaxFiltering - 1
		// ClearViewAlsoSupportsDepthOnlyFormats - 1
		// MapOnDefaultBuffers - 1

		D3D11_FEATURE_DATA_D3D11_OPTIONS2 options2 = {};
		hr = m_device->CheckFeatureSupport(
			D3D11_FEATURE_D3D11_OPTIONS2,
			&options2,
			sizeof(options2)
		);

		// PSSpecifiedStencilRefSupported	0
		// TypedUAVLoadAdditionalFormats	1
		// ROVsSupported	1
		// ConservativeRasterizationTier	D3D11_CONSERVATIVE_RASTERIZATION_TIER_3
		// TiledResourcesTier	D3D11_TILED_RESOURCES_TIER_3
		// MapOnDefaultTextures	1
		// StandardSwizzle	0
		// UnifiedMemoryArchitecture	0

		D3D11_FEATURE_DATA_D3D11_OPTIONS3 options3 = {};
		hr = m_device->CheckFeatureSupport(
			D3D11_FEATURE_D3D11_OPTIONS3,
			&options3,
			sizeof(options3)
		);

		// VPAndRTArrayIndexFromAnyShaderFeedingRasterizer	1

		if (SUCCEEDED(hr)) {
			if (options1.TiledResourcesTier == D3D11_TILED_RESOURCES_NOT_SUPPORTED) {
				womalog(TEXT("[INFO] Tiled Resources: NOT SUPPORTED on this GPU.\n"));
			}
			else {
				womalog(TEXT("[INFO] Tiled Resources: SUPPORTED - Tier %d\n"), options1.TiledResourcesTier);
			}
		}
		else {
			womalog(TEXT("[INFO] Tiled Resources: Query failed (old SDK or GPU)\n"));
		}

#ifdef _DEBUG
		// 10) Debug info queue setup when debug layer is active
		if (DeviceFlags & D3D11_CREATE_DEVICE_DEBUG) {
			ComPtr<ID3D11Debug> dbg;
			if (SUCCEEDED(m_device.As(&dbg))) {
				ComPtr<ID3D11InfoQueue> iq;
				if (SUCCEEDED(dbg.As(&iq))) {
					D3D11_INFO_QUEUE_FILTER filter;
					ZeroMemory(&filter, sizeof(filter));
					D3D11_MESSAGE_SEVERITY denySev[] = { D3D11_MESSAGE_SEVERITY_INFO };
					filter.DenyList.NumSeverities = _countof(denySev);
					filter.DenyList.pSeverityList = denySev;
					iq->PushStorageFilter(&filter);

					iq->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
					iq->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
				}
			}
		}
#endif
#else
		D3D_DRIVER_TYPE dt = WOMA::UseWarpDevice ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE;
		result = D3D11CreateDevice(
				NULL,
				dt /*DriverTypes[0].DX11Type*/,
				NULL,						// used only by: D3D_DRIVER_TYPE_SOFTWARE
				DeviceFlags,
				PtrfeatureLevels, num_levels/*ARRAYSIZE*/,
				D3D11_SDK_VERSION,
                &m_device11,
				&featureLevel_,				// OUTPUT: The address of the feature level that was selected
				&m_deviceContext);			// OUTPUT: The address for the rendering context
		
		if (result != S_OK || !m_device11)
		{
			WomaFatalException(("FATAL ERROR: Could not Create DX 11 Device: D3D11CreateDevice")); //CHAR!
		}
#endif


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
			// if we cant use DX9 and don't have DX10 --> FATAL
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

		womalogauto(TEXT("Driver Support Concurrent Creates: %s\n"), ThreadingOptions.DriverConcurrentCreates ? TEXT("yes") : TEXT("no"));
		womalogauto(TEXT("Driver Support Command Lists: %s\n\n"), ThreadingOptions.DriverCommandLists ? TEXT("yes") : TEXT("no"));

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
				
				UINT quality = 0;
				for (UINT msaaSamples_ = 1; msaaSamples_ <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; msaaSamples_++)
				{
					result = m_device11->CheckMultisampleQualityLevels(BUFFER_COLOR_FORMAT, msaaSamples_, &quality);
					if (result == S_OK && quality != 0)
					{
						m_sCapabilities.MSAAmultiSampleCount = msaaSamples_;
						m_sCapabilities.MSAAquality = quality;
						womalogauto(TEXT("DRIVER MSAAmultiSampleCount Supported: x%d\n"), m_sCapabilities.MSAAmultiSampleCount);		// Get the max Sample Count: 8
						FSAA_possibleValues.push_back(m_sCapabilities.MSAAmultiSampleCount);
						womalogauto(TEXT("DRIVER multiSampleQuality: %d\n"), m_sCapabilities.MSAAquality);	// Get the max MsaaQuality: 32

						// Use Max Setting Supported:
						if (MSAA_COUNT == 0) // 0 = Auto Detect Max!
						{
							MSAA_QUALITY = m_sCapabilities.MSAAquality;
							MSAA_COUNT = MIN(4, m_sCapabilities.MSAAmultiSampleCount);
						}
					}
				}
				
			}
		}

		if (SystemHandle->AppSettings->MSAA_Anisotropic == true && MSAA_COUNT > 1) //Setup defaults!
		{
			MSAA_QUALITY = 1;
		}
		if (SystemHandle->AppSettings->MSAA_Anisotropic == false) //Setup defaults!
		{
			MSAA_QUALITY = 0;
			MSAA_COUNT = 1;
		}

		// Log It!
		if (SystemHandle->AppSettings->MSAA_Anisotropic) {
			womalogauto(TEXT("MSSA is Enabled with %d Samples\n"), MSAA_COUNT);
		}
		else
		{
			if (SystemHandle->AppSettings->MSAA_bilinear)
				womalogauto(TEXT("Antialise: bilinear\n"));
			else if (SystemHandle->AppSettings->MSAA_trilinear)
				womalogauto(TEXT("Antialise: trilinear\n"));
			else
				womalogauto(TEXT("Antialise: off\n"));
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

		// Window Scaling
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
			womalogauto(TEXT("Try CreateSwapChain settings for Windows 10:\n"));
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
