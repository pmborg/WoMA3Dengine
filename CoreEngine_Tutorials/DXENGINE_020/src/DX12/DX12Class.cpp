// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: DX12Class.cpp
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
//  MAIN PURPOSE: Work as a DirectX 12 Driver.
// --------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------

#include "platform.h"
#include<D3D11.h>
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
#include "dx12Class.h"

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 && DX_ENGINE_LEVEL >= 19		// Initializing Engine && defined DX11
#include "winsystemclass.h"	// SystemHandle

namespace DirectX {

// ----------------------------------------------------------------------------------------------
DX12Class::DX12Class()
// ----------------------------------------------------------------------------------------------
{
	// WomaDriverClass / Public: ------------------------------------------------------
	CLASSLOADER();

	// SUPER Video Card Info:
	// ---------------------------------------------------------------------------
	_tcscpy_s(driverName, sizeof(driverName), TEXT("DX12"));
	ZeroMemory(&ShaderModel, sizeof(ShaderModel));
	ZeroMemory(&szShaderModel, sizeof(szShaderModel));
	// ------------------
	ZeroMemory(&m_videoCardDescription, sizeof(m_videoCardDescription));
	m_videoCardMemory = NULL;
	ZeroMemory(&adapterDesc_Description, sizeof(adapterDesc_Description));
	ufreededicatedVideoMem = NULL;

	// MSAA Used:
	// ----------------------------------------------------------------------------
	ReqMSAAmultiSampleCount = MAX(1,SystemHandle->AppSettings->MSAA_X);	// Req. Note: DX12 min: 1
	ReqMSAAquality = 1;													// Req. Note: default: 1

	// DX12Class()
	// Public: ------------------------------------------------------------------------
	numerator = denominator = 1;
	m_device = NULL;
	m_fenceEvent = NULL;

	ShaderVersionH = ShaderVersionL = NULL;

	m_CullMode = m_fillMode = NULL;

	BUFFER_COLOR_FORMAT = DXGI_FORMAT_B8G8R8A8_UNORM; // "Loader Image" use this format.

#if DX_ENGINE_LEVEL >= 21 || defined CLIENT_SCENE_TEXT //21
	m_Camera = NULL;
#endif

#ifdef USING_THREADS
	ZeroMemory(m_srvIndex, sizeof(m_srvIndex));
	ZeroMemory(m_frameFenceValues, sizeof(m_frameFenceValues));

	m_currentFrame = 0;

	for (int n = 0; n < ThreadCount; n++)
	{
		m_renderContextFenceValues[n] = 0;
		m_threadFenceValues[n] = 0;
	}

	float sqRootNumAsyncContexts = sqrt(static_cast<float>(ThreadCount));
	m_heightInstances = static_cast<UINT>(ceil(sqRootNumAsyncContexts));
	m_widthInstances = static_cast<UINT>(ceil(sqRootNumAsyncContexts));

	if (m_widthInstances * (m_heightInstances - 1) >= ThreadCount)
	{
		m_heightInstances--;
	}
#endif

	// Clear the previous window size specific content.
	for (UINT n = 0; n < BufferCount; n++)
		m_renderTargets[n] = nullptr;

	m_rtvHeap = nullptr;
}

DX12Class::~DX12Class() // Used for Static Classes
{
	Shutdown(); 
	CLASSDELETE();
} 

// The Shutdown function will release and clean up all the pointers used in the Initialize function
void DX12Class::Shutdown2D() {}

//----------------------------------------------------------------------------------------------
void DX12Class::Shutdown()
//----------------------------------------------------------------------------------------------
{
	if (m_device)
	{
		// Wait for the GPU to be done with all resources.
		CloseHandle(m_fenceEvent);
		m_fenceEvent = nullptr;

		for (UINT i = 0; i<BufferCount; ++i)
			m_renderTargets[i].Reset();

		m_swapChain.Reset();
		m_fence.Reset();
		m_commandList.Reset();
		m_commandAllocator.Reset();
		m_commandQueue.Reset();

		//Release the two new blending states.

		// The Last one!
		ULONG count = m_device->Release();
		m_device = NULL;

		if (count)
		{
			TCHAR str[512];
			StringCchPrintf(str, sizeof(str), TEXT("DEBUG WARNING: There are %d unreleased references left on the D3D device!\n"), count);
			WOMA::logManager->DEBUG_MSG(str);
		}
	}

}

// |Init Step: 1| This is for DIRECTX Driver only!
// ----------------------------------------------------------------------------------------------
BOOL DX12Class::CheckAPIdriver(UINT USE_THIS_ADAPTER)
// ----------------------------------------------------------------------------------------------
{
	//
	// LoadPipeline() - PART 1
	//
	UINT FactoryFlags = 0;

#ifdef _DEBUG // Enable the D3D12 debug layer.
	ComPtr<ID3D12Debug> debugController = NULL;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();

		// Enable additional debug layers:
		FactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

/*
		LAYERS:
		-----------------------
		| WOMA APPLICATION
		V----------------------
		| DX		12.x
		V----------------------
		| DXGI		1.x
		V----------------------
		| WDDM		2.x
		V----------------------
		| WINDOWS	10

		DX Low Level "Display Driver" API version:
		------------------------------------------
		Win Display Driver Model Versions: SOURCE	https://en.wikipedia.org/wiki/Windows_Display_Driver_Model#History
		------------------------------------------
		WDDM 1.0->Windows Vista	CreateDXGIFactory	Creates a DXGI 1.0 Add DX10 device driver
		WDDM 1.1->Windows 7		CreateDXGIFactory1	Creates a DXGI 1.1 Add DX11 device driver interface(DDI), Support multiple drivers in a multi-adapter and multi-monitor setup
		WDDM 1.2->Windows 8							Creates a DXGI 1.2 Include preemptive multitasking of the GPU with finer granularity(DMA buffer, primitive, triangle, pixel, or instruction - level)
		WDDM 1.3->Windows 8.1	CreateDXGIFactory2	Creates a DXGI 1.3 Ability to trim DXGI adapter memory usage, multi-plane overlays, overlapping swap chains and swap chain scaling
		WDDM 2.0->Windows 10						Creates a DXGI 1.4 SHADER MODEL: 5.1 Add DX12 device driver, support virtual memory addressing, to allow multithreading parallelism
		WDDM 2.1->Windows 10 (version 1607)			Creates a DXGI 1.5 SHADER MODEL: 6.0 Mandatory for feature levels 12_0 and 12_1
		WDDM 2.2->Windows 10 (version 1703)			Creates a DXGI 1.6 SHADER MODEL: 6.0 With stereoscopic rendering for the Windows Holographic platform

		DX Low Level "Card Driver" API versions:
		----------------------------------------
		DXGI API Version: SOURCE					https://msdn.microsoft.com/en-us/library/windows/desktop/ff471322(v=vs.85).aspx
		----------------------------------------
			IDXGIAdapter	DXGI 1.0	IDXGIFactory	DX10 : Vista
			IDXGIAdapter1	DXGI 1.1	IDXGIFactory1	DX11 : Vista + SP2(KB 971644) & Win7
			IDXGIAdapter2	DXGI 1.2	IDXGIFactory2	DX11 : Win7 + (Platform Update) & Win8.0
			"(IDXGIFactory2::CreateSwapChainForComposition) Add DirectComposition"
			"(IDXGIFactory2::IsWindowedStereoEnabled) Stereo Mode"
			"(IDXGIFactory2::RegisterOcclusionStatusEvent) Receive notification messages of changes of occlusion status"
			MORE: https://msdn.microsoft.com/en-us/library/windows/desktop/hh404556(v=vs.85).aspx
			DXGI 1.3	IDXGIFactory3	DX12 : Win8.1"(IDXGIFactory3::GetCreationFlags) Gets the flags that were used when a (DXGI) object was created.
			IDXGIAdapter3								DX12 : Win10 "(IDXGIAdapter3::QueryVideoMemoryInfo) Adds some memory residency methods, for budgeting and reserving physical memory"
			DXGI 1.4	IDXGIFactory4	DX12 : Win10 "(IDXGIFactory4::EnumAdapterByLuid) Adds Outputs the IDXGIAdapter for the specified LUID"
			"(IDXGIFactory4::EnumWarpAdapter) Provides an adapter which can be provided to D3D12CreateDevice to use the WARP renderer. 
			DXGI 1.5	IDXGIFactory5	DX12 : Win10 "(IDXGIFactory5::CheckFeatureSupport) Enables a single method to support variable refresh rate displays"
			IDXGIAdapter4	DXGI 1.6					DX12 : Win10 "(IDXGIAdapter4::GetDesc3) Adds a method to check for an adapter's compatibility with Arbitrary Code Guard (ACG)."
*/
			ThrowIfFailed(CreateDXGIFactory2(FactoryFlags, IID_PPV_ARGS(&dxgiFactory)));// Create a DXGI 1.3 / 1.4 /1.5 factory Interface with Factory:2

	//
	// EnumAdapters
	//
	ComPtr<IDXGIAdapter1> hardwareAdapter;
	D3D_FEATURE_LEVEL feacture_level = D3D_FEATURE_LEVEL_12_1;					// Target DX 12.1
	GetHardwareAdapter(dxgiFactory.Get(), &hardwareAdapter, feacture_level);

	// ------------------------------------------------------------------------------------------
	// Try: "CreateDevice" for: LEVEL_12_1
	// ------------------------------------------------------------------------------------------
	m_sCapabilities.CapDX12 = true;
	if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), feacture_level, IID_PPV_ARGS(&m_device))))
	{
		m_sCapabilities.CapDX12_1 = false;

		D3D_FEATURE_LEVEL feacture_level = D3D_FEATURE_LEVEL_12_0;				// Target DX 12.0
		GetHardwareAdapter(dxgiFactory.Get(), &hardwareAdapter, feacture_level);

		// ------------------------------------------------------------------------------------------
		// Try: CreateDevice for: LEVEL_12_0
		// ------------------------------------------------------------------------------------------
		if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), feacture_level, IID_PPV_ARGS(&m_device))))
		{
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
			ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), feacture_level, IID_PPV_ARGS(&m_device)));
			m_sCapabilities.CapDX12 = false;
		}
		m_sCapabilities.CapDX12 = true;
	}
	else
		m_sCapabilities.CapDX12_1 = true;

	//m_device->SetStablePowerState(TRUE);	// Prevent the GPU from overclocking or underclocking to get consistent timings

	// USE DXGI 1.2 API: Are we Rendering in Stereo for 3D Monitors?
	m_sCapabilities.inStereoAdapterMode = dxgiFactory->IsWindowedStereoEnabled();

	// Check [1] D3D12_FEATURE_DATA_D3D12_OPTIONS
	{
		UINT DX12_TIER = 0;
		if (m_device)
		{
			D3D12_FEATURE_DATA_D3D12_OPTIONS options;
			m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options));

			switch (options.ResourceBindingTier)
			{
			case D3D12_RESOURCE_BINDING_TIER_1:
				DX12_TIER = 1; // Tier 1 is supported
				break;

			case D3D12_RESOURCE_BINDING_TIER_2:
				DX12_TIER = 2; // Tier 1 and 2 are supported
				break;

			case D3D12_RESOURCE_BINDING_TIER_3:
				DX12_TIER = 3; // Tier 1,2 and 3 are supported
				break;
			}
		}
	}

	// In multi: Adapter operation:
	{
		uint32_t numNodes = m_device->GetNodeCount();
		for (uint32_t i = 0; i < numNodes; ++i)
		{
			D3D12_FEATURE_DATA_ARCHITECTURE arch;
			arch.NodeIndex = i;
			if (FAILED(/*adapterList[i].Device*/m_device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &arch, sizeof(arch))))
			{
				m_sCapabilities.multiAdapterSupport = false;
				WOMA::logManager->DEBUG_MSG(TEXT("Failed to query (the architecture) for Adapter: 1\n"));
			}
			else {
				m_sCapabilities.multiAdapterSupport = true;
				WOMA::logManager->DEBUG_MSG("Node % 2d: TileBasedRenderer %d, UMA %d, CacheCoherentUMA %d\n"
					, i
					, arch.TileBasedRenderer
					, arch.UMA
					, arch.CacheCoherentUMA
				);
			}
		}
	}

	//
	// Check [3] D3D12_FEATURE_DATA_FEATURE_LEVELS
	{
		D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelsInfo;

		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_12_1, // check D3D 12.1 support 
												D3D_FEATURE_LEVEL_12_0, // check D3D 12 support 
												D3D_FEATURE_LEVEL_11_1, // check D3D 11.1 support 
												D3D_FEATURE_LEVEL_11_0, // check D3D 11 support 
												D3D_FEATURE_LEVEL_10_1, // check D3D 10.1 support 
												D3D_FEATURE_LEVEL_10_0, // check D3D 10 support 
												D3D_FEATURE_LEVEL_9_3	// Finally, check D3D 9.3 support 
		};

		featureLevelsInfo.NumFeatureLevels = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);
		featureLevelsInfo.pFeatureLevelsRequested = featureLevels;

		if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelsInfo, sizeof(featureLevelsInfo))))
		{
			WOMA::logManager->DEBUG_MSG(TEXT("Failed to query (featureLevelsInfo) for Adapter: 1\n"));
		}
		else {
			WORD MaxLevel = featureLevelsInfo.MaxSupportedFeatureLevel;
			m_sCapabilities.featureLevelsHI = (MaxLevel >> 8) >> 4;
			m_sCapabilities.featureLevelsLO = (MaxLevel >> 8) & 0xF;
			WOMA::logManager->DEBUG_MSG("(featureLevelsInfo) query for Adapter: 1 =  %d.%d\n", m_sCapabilities.featureLevelsHI, m_sCapabilities.featureLevelsLO);

			//printf ("(featureLevelsInfo) query for Adapter: 1 =  %04x\n", MaxLevel);
		}
	}

	//
	// Check [4] D3D12_FEATURE_DATA_FORMAT_SUPPORT

	{
		D3D12_FEATURE_DATA_FORMAT_SUPPORT data;

		// TODO: Make a for... check them all
		for (size_t i = 0; i < 115; i++)
		{
			data.Format = (DXGI_FORMAT)i /*DXGI_FORMAT_R8G8B8A8_UNORM*/;
			if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &data, sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT))))
			{
				m_sCapabilities.Formats[i] = false;
			}
			else
			{
				m_sCapabilities.Formats[i] = true;
			}
		}
	}

	//
	// Check [5] D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS
	{
		if (ReqMSAAmultiSampleCount > 0)
		{
			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS data;
			data.Format = BUFFER_COLOR_FORMAT;
			data.SampleCount = ReqMSAAmultiSampleCount;
			data.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;

			if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &data, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS))))
			{
				WOMA::logManager->DEBUG_MSG(TEXT("Failed to query (MSAA) for Adapter: 1\n"));
			}
			else
			{
				WOMA::logManager->DEBUG_MSG("(MSAA) query for Adapter: 1 =  %dX Quality: %d\n", data.SampleCount, data.NumQualityLevels);
			}
		}
		else
		{
			WOMA::logManager->DEBUG_MSG(TEXT("Requested Settings for MSAA: Disabled for Adapter: 1\n"));
		}
	}

	//
	// Check [6] D3D12_FEATURE_DATA_FORMAT_INFO
	{
		D3D12_FEATURE_DATA_FORMAT_INFO formatInfo = { BUFFER_COLOR_FORMAT };
		if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &formatInfo, sizeof(formatInfo))))
		{
			WOMA::logManager->DEBUG_MSG(TEXT("Failed to query (Format Info) for Adapter: 1\n"));
		}
		else
		{
			WOMA::logManager->DEBUG_MSG("(Format Info) query for Adapter: 1 PlaneCount: %d\n", formatInfo.PlaneCount);
		}
	}

	//
	// Check [7] D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT
	{
		D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT m_GpuVaSupport;

		if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT, &m_GpuVaSupport, sizeof(m_GpuVaSupport))))
		{
			WOMA::logManager->DEBUG_MSG("Failed to acquire GPU virtual address support for Adapter: 1");
		}
		else
		{
			WOMA::logManager->DEBUG_MSG("MaxGPUVirtualAddressBitsPerResource: %d\nMaxGPUVirtualAddressBitsPerProcess: %d\n",
				m_GpuVaSupport.MaxGPUVirtualAddressBitsPerResource,
				m_GpuVaSupport.MaxGPUVirtualAddressBitsPerProcess);
		}
	}

	//
	// Check [8] D3D12_FEATURE_DATA_SHADER_MODEL
	getProfile();


	//
	// Check [9] D3D12_FEATURE_DATA_D3D12_OPTIONS1
	D3D12_FEATURE_DATA_D3D12_OPTIONS1 d3d12Options;
	if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &d3d12Options, sizeof(d3d12Options))))
	{
		WOMA::logManager->DEBUG_MSG(TEXT("Failed to query (d3d12Options) for Adapter: 1\n"));
	}
	//
	// Check [10] D3D12_FEATURE_DATA_ROOT_SIGNATURE
	D3D12_FEATURE_DATA_ROOT_SIGNATURE rootSignature;
	rootSignature.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &rootSignature, sizeof(rootSignature))))
	{
		WOMA::logManager->DEBUG_MSG(TEXT("Failed to query (rootSignature) for Adapter: 1\n"));
	}
	else
	{
		// D3D_ROOT_SIGNATURE_VERSION_1_0 = 0x1,
		// D3D_ROOT_SIGNATURE_VERSION_1_1 = 0x2
		if (rootSignature.HighestVersion == 1)
			WOMA::logManager->DEBUG_MSG("(rootSignature) query for Adapter: 1 HighestVersion: %d.%d\n", 1, 0);
		if (rootSignature.HighestVersion == 2)
			WOMA::logManager->DEBUG_MSG("(rootSignature) query for Adapter: 1 HighestVersion: %d.%d\n", 1, 1);
	}

	return TRUE;
}


// MAIN INIT - createSwapChain and also "Get the best MultiSampleQuality":
//----------------------------------------------------------------------------------------------
bool DX12Class::OnInit(int g_USE_MONITOR, /*HWND*/void* hwnd, int screenWidth, int screenHeight,
	UINT depthBits, float screenDepth, float screenNear, BOOL msaa, bool vsync,
	BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize)
	//----------------------------------------------------------------------------------------------
{
	//mEnable4xMsaa = msaa;
	m_VSYNC_ENABLED = vsync;

	WOMA::logManager->DEBUG_MSG(TEXT("-------------------------\n"));
	WOMA::logManager->DEBUG_MSG(TEXT("Initializing Driver: %s\n"), driverName);
	WOMA::logManager->DEBUG_MSG(TEXT("-------------------------\n"));
	
	//
	// CreateDXGIFactory2
	//
	// CreateDevice() - Init Step: 0 - Check for DX9, DX10, DX11, DX12 & DX12_1 API
	//
	CheckAPIdriver(USE_THIS_GRAPHIC_CARD_ADAPTER);	//USE_THIS_ADAPTER

	// Get BUFFER_COLOR_FORMAT: Init Step: 1 - [NOT IMPLEMENTED YET FOR DX12] Create Factory: Get list of all MODES for all MONITORS & Get Refresh Rate:
	////IF_NOT_RETURN_FALSE(getModesList(g_USE_MONITOR, screenWidth, screenHeight, fullscreen, &numerator, &denominator)); //TODO! DX12

	// 
	// CreateCommandQueue()
	//
	// CreateDescriptorHeap() - (RTV) - (CBV)
	//
	// CreateCommandAllocator()
	//
	// CreateCommandList()
	//
	IF_NOT_RETURN_FALSE(initDX12Device((HWND) hwnd));	// Init Step: 2 - Init DX12 Device

	// SelectDepthFormat(depthBits, fullscreen);	// Init Step: 3 Note: need device

	//Init Step: 9 - CreateTexture2D:
	IF_NOT_RETURN_FALSE(CreateRenderTargetView(screenWidth, screenHeight));

	//Init Step: 6 Before Resize (SetCamera2D & SetCamera3D)!

	//
	//DO: setViewportDevice(screenWidth, screenHeight);
	//DO: setProjectionMatrixWorldMatrixOrthoMatrix(screenWidth, screenHeight, screenNear, screenDepth);
	//
	Resize(screenWidth, screenHeight, screenNear, screenDepth, fullscreen, depthBits);		//Init Step: 7 (Include: 8,9,10,11,12) // Get Projection Matrix!

	//Init Step: 6 - Cull Back / Front:
#if defined USE_RASTERIZER_STATE
	IF_NOT_RETURN_FALSE(createAllRasterizerStates(false)); // Only applies: if doing "line drawing" and "MultisampleEnable" is false.
	SetRasterizerState(CULL_NONE, FILL_SOLID);	//Set Default
#endif

	// Create the command list.
	ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
	return true;
}

// This is for OPENGL Driver only!
// ----------------------------------------------------------------------------------------------
BOOL DX12Class::Check (int* Hi, int* low){return TRUE;}
// ----------------------------------------------------------------------------------------------

inline UINT8 D3D12GetFormatPlaneCount(_In_ ID3D12Device* pDevice, DXGI_FORMAT Format)
{
	D3D12_FEATURE_DATA_FORMAT_INFO formatInfo = { Format };
	if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &formatInfo, sizeof(formatInfo))))
	{
		return 0;
	}
	return formatInfo.PlaneCount;
}

void DX12Class::QueryVideoMemoryInfo(UINT adapterIndex, ComPtr<IDXGIAdapter1> adapter)
{
	// USE DXGI 1.3 API: QueryVideoMemoryInfo
	#if DXGI_API >= 3	
	
	if (adapter->QueryInterface(IID_PPV_ARGS(&DXGIadapter3)) == S_OK)		// Try to Get: IDXGIAdapter3 from IDXGIAdapter1
	{
		DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
		DXGIadapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

		WOMA::logManager->DEBUG_MSG("videoMemory Budget for Adapter %d: (%d) MB\n", adapterIndex, (videoMemoryInfo.Budget / MBs));
		WOMA::logManager->DEBUG_MSG("videoMemory CurrentReservation for Adapter (%d): %d MB\n", adapterIndex, (videoMemoryInfo.CurrentReservation / MBs));
		WOMA::logManager->DEBUG_MSG("videoMemory CurrentUsage for Adapter (%d): %d MB\n", adapterIndex, (videoMemoryInfo.CurrentUsage / MBs));
	}
	#endif
}

void DX12Class::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter, D3D_FEATURE_LEVEL feacture_level)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			WOMA::logManager->DEBUG_MSG(L"\nD3D12-Capable Software Adapter Adapter (%d) found: %s (%u MB)\n", adapterIndex, desc.Description, desc.DedicatedVideoMemory >> 20);
			QueryVideoMemoryInfo(adapterIndex, adapter);
		}
		else
		{
			// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), feacture_level, _uuidof(ID3D12Device), nullptr)))
			{
				WOMA::logManager->DEBUG_MSG(L"\nD3D12-Capable Hardware Adapter (%d) found: %s (%u MB)\n", adapterIndex, desc.Description, desc.DedicatedVideoMemory >> 20);
				QueryVideoMemoryInfo(adapterIndex, adapter);
				*ppAdapter = adapter.Detach();
			}
		}
	}

	ASSERT (*ppAdapter);
}

// Select the Depth Buffer Format (16, 24, 32, 64, or default...)
//----------------------------------------------------------------------------------------------
void DX12Class::SelectDepthFormat(UINT depthBits, BOOL fullscreen) 
//----------------------------------------------------------------------------------------------
{
}


//----------------------------------------------------------------------------------------------
bool DX12Class::Resize (int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits)
//----------------------------------------------------------------------------------------------
{
HRESULT result = S_OK;

	RenderfirstTime = true;	 // Used on SPRITES!

	if (m_device && m_swapChain) 
	{

		//Init Step: 11 - RSSetViewports: Map Screen clip space coordinates to the render target space:
		setViewportDevice(screenWidth, screenHeight);

        //Init Step: 12 - Set ProjectionMatrix (CH06) and OrthoMatrix (CH07)
	}

#if defined CLIENT_SCENE_TEXT || defined USE_VIEW2D_SPRITES // 26
	//SetCamera2D(); //AQUI
	Initialize3DCamera();
#endif

	return true;
}




// |Init Step: 2|
// ----------------------------------------------------------------------------------------------
bool DX12Class::getModesList(int g_USE_MONITOR, int screenWidth, int screenHeight, BOOL fullscreen, UINT* numerator, UINT* denominator)
// ----------------------------------------------------------------------------------------------
{
	HRESULT result = S_OK;

return true;
}

//Init Step: 3
// ----------------------------------------------------------------------------------------------
bool DX12Class::initDX12Device (HWND hwnd)
// ----------------------------------------------------------------------------------------------
{
	HRESULT result = S_OK;

	//
	// LoadAssets()- PARTE 2
	//
	// Describe and create the command queue.
	// ==========================================
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
	
	ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));

	// Create Descriptor Heaps:
	{
		// render target view: "Describe" the render target view (RTV) descriptor heap:
		// ----------------------------------------------------------------------------
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = BufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));	// Create a render target view of the swap chain back buffer:

		m_rtvHeap->SetName(L"Render Target View Descriptor Heap");

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	}

	//
	// LoadAssets()- PARTE 3
	//


	//
	// createSwapChain()
	//
	// ----------------------------------------------------------------------------------------------
	IF_NOT_RETURN_FALSE(createSwapChainDX12device(hwnd, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT, SystemHandle->AppSettings->VSYNC_ENABLED,
		SystemHandle->AppSettings->FULL_SCREEN, SystemHandle->AppSettings->UseDoubleBuffering, SystemHandle->AppSettings->AllowResize,
		numerator, denominator));
	if (SystemHandle->AppSettings->MSAA_ENABLED)
		WOMA::logManager->DEBUG_MSG(TEXT("MSSA setting: Enabled\n"));
	else
		WOMA::logManager->DEBUG_MSG(TEXT("MSSA setting: Disabled\n"));

	return true;
}

//Init Step: 4
// ----------------------------------------------------------------------------------------------
bool DX12Class::createSwapChainDX12device (	HWND hwnd, int screenWidth, int screenHeight, BOOL vsync, 
											BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize, 
											UINT numerator, UINT denominator)
// ----------------------------------------------------------------------------------------------
{
	HRESULT result = S_OK;

	//
	// LoadPipeline() - PART 2
	//
	ASSERT (hwnd);			// FATAL ERROR: Create "Main Window" first!
	ASSERT(!m_swapChain);	// FATAL ERROR: Reset "m_swapChain" first!

	// Describe the swap chain:
	// ==========================================

	// Initialize the swap chain description:
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};	// ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set the width and height of the back buffer:
	swapChainDesc.BufferDesc.Width = screenWidth;	
	swapChainDesc.BufferDesc.Height = screenHeight;

	// The default: 32-bit surface for the back buffer: (RGB + A) 8 bits each (This is the most common swap chain format)
	swapChainDesc.BufferDesc.Format = BUFFER_COLOR_FORMAT /*DXGI_FORMAT_B8G8R8A8_UNORM*/; // Default: DXGI_FORMAT_R8G8B8A8_UNORM

	// Set the refresh rate of the back buffer:
	swapChainDesc.BufferDesc.RefreshRate.Numerator = (vsync) ? numerator : 0;		// 0
	swapChainDesc.BufferDesc.RefreshRate.Denominator = (vsync) ? denominator : 0;	// 1

	// Set the default usage of the back buffer:
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	// Set to a single/double-buffering back buffer:  "Frame Buffer"
	swapChainDesc.BufferCount = BufferCount; // (g_UseDoubleBuffering) ? 2 : 1; // Use double-buffering to minimize latency.
	//swapChainDesc.BufferCount = BufferCount; // Use triple-buffering to minimize latency.

	// Set MSAA Quality:
	swapChainDesc.SampleDesc.Quality = ReqMSAAquality-1;
	swapChainDesc.SampleDesc.Count = ReqMSAAmultiSampleCount;	// Default: 0, This member is used to tell Direct3D how to perform multisample anti-aliased (MSAA) rendering

	swapChainDesc.OutputWindow = hwnd;		// Set the handle for the window to render to.
	swapChainDesc.Windowed = !fullscreen;	// Set to "Fullscreen" or "Windowed Mode":

	swapChainDesc.Flags = (g_AllowResize) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

	// Set the scan line ordering and scaling to unspecified:
	//swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//swapChainDesc.BufferDesc.Scaling = DXGI_SCALING_STRETCH;

	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;		// DX11
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   // DX12

	ThrowIfFailed(dxgiFactory->CreateSwapChain(m_commandQueue.Get(), &swapChainDesc, &s_PrimarySwapChain));

	// Get IDXGISwapChain3: m_swapChain
	ThrowIfFailed(s_PrimarySwapChain.As(&m_swapChain)); // m_swapChain = (IDXGISwapChain3*)s_PrimarySwapChain;

	//m_swapChain->SetMaximumFrameLatency(3);

	if (!g_AllowResize)
		ThrowIfFailed(dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));


	m_currentFrame = m_swapChain->GetCurrentBackBufferIndex(); // 0

	return true;
}

//Init Step: 7


// ----------------------------------------------------------------------------------------------
bool DX12Class::Initialize(float* clearColor) //void D3D12HelloWindow::LoadAssets()
// ----------------------------------------------------------------------------------------------
{
	WOMA::logManager->DEBUG_MSG("DX12Class::Initialize()\n");

	// Set default clear color:
	ClearColor[0] = *clearColor++;
	ClearColor[1] = *clearColor++;
	ClearColor[2] = *clearColor++;
	ClearColor[3] = *clearColor;

	return true;
}

void DX12Class::Finalize() //::LoadAssets()
// ----------------------------------------------------------------------------------------------
{
	// [3]
	//
	// Create synchronization objects:
	for (UINT n = 0; n < BufferCount; n++)
	{
		ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValues[n] = 1; // ++;
	}

	// Create an event handle to use for frame synchronization.
	m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	ASSERT(m_fenceEvent);

	// [1]
	// Close the command list and execute it to begin the vertex buffer copy into
	// the default heap.
	ThrowIfFailed(m_commandList->Close());

	// [2]
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// [4]
	// Wait for the command list to execute; we are reusing the same command 
	// list in our main loop but for now, we just want to wait for setup to 
	// complete before continuing.
	WaitForGpu();
}

// ----------------------------------------------------------------------------------------------
void DX12Class::BeginScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress:

	ThrowIfFailed(m_commandAllocator->Reset());
	
	ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), nullptr /*m_pipelineInitialState.Get()*/));
	
	// Indicate that the back buffer will be used as a render target.
	//																		m_deviceResources->GetRenderTarget()	
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentFrame].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	// Record drawing commands:
	// ------------------------
	// Set necessary state.
	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrame, m_rtvDescriptorSize);

	// EQUAL TO:
	//auto handleRTV = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	//rtvHandle.ptr += (m_currentFrame * m_rtvDescriptorSize);

	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Clear Screen - with "ClearColor" defined: DX11Class::Initialize
	m_commandList->ClearRenderTargetView(rtvHandle, ClearColor, 0, nullptr);

}


// ----------------------------------------------------------------------------------------------
void DX12Class::ClearDepthBuffer()
// ----------------------------------------------------------------------------------------------
{
}

#define USE_PRESENT_DXGI_1_0

// ----------------------------------------------------------------------------------------------
void DX12Class::EndScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	// Indicate that the "back-buffer" will now be used to present.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentFrame].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Command lists are created in the recording state, but there is nothing
	// to record yet. The main loop expects it to be closed, so close it now.
	ThrowIfFailed(m_commandList->Close());

	// Execute all the command list -> GPU will now RENDER ALL SCREEN: on "back-buffer"(m_currentFrame)
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue.Get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// The first argument instructs DXGI to block execution, until VSync (if VSync ON), putting the application to sleep until the next VSync. 
	// This ensures we don't waste any cycles rendering frames that will never be displayed to the screen.
#if defined USE_PRESENT_DXGI_1_0
	// Direct3D 11++
	HRESULT hr = m_swapChain->Present(m_VSYNC_ENABLED, 0); // if (m_VSYNC_ENABLED) Sleep a bit, until next motinor Hz
#else
	// Direct3D 11.1++
	DXGI_PRESENT_PARAMETERS PresentDesc = { 0 };
	UINT Flags = DXGI_PRESENT_DO_NOT_WAIT;
	HRESULT hr = m_swapChain->Present1(m_VSYNC_ENABLED, Flags, &PresentDesc);
#endif

	// If the device was removed either by a "Full-Screen Switch" or a "driver upgrade", 
	// We must wait for all device resources re-load:
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		return;

	//m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	MoveToNextFrame();
}

// Using Double Buffer: (BufferCount = 2)
// | 0 | --> | 1 | --> | 0 | ...

// Using Tripple Buffer: (BufferCount = 3)
// | 0 | --> | 1 | --> | 2 | --> | 0 | ...

void DX12Class::MoveToNextFrame()
{

	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = m_fenceValues[m_currentFrame];
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

	// Check to see if the next frame is ready to start.
	if (m_fence->GetCompletedValue() < currentFenceValue)
	{
		ThrowIfFailed(m_fence->SetEventOnCompletion(currentFenceValue, m_fenceEvent));
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
	}

	// Advance the frame index:
	m_currentFrame = (m_currentFrame + 1) % BufferCount;

	// Set the fence value for the next frame.
	m_fenceValues[m_currentFrame] = currentFenceValue + 1;

}

//
// Wait for GPU to Load all Objects/Textures and so on, before start rendering:
//
void DX12Class::WaitForGpu()
{

	// Schedule a Signal command in the queue.
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_currentFrame]));
	ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));

	// Wait until the fence has been processed.
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	m_fenceValues[m_currentFrame]++;

}

//Init Step: 5 - Set the best shader availabel: MORE INFO: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476876%28v=vs.85%29.aspx
// ----------------------------------------------------------------------------------------------
void DX12Class::getProfile ()
// ----------------------------------------------------------------------------------------------
{
	// NOTE: DX12 = 5.1 (min.)

	D3D12_FEATURE_DATA_SHADER_MODEL shaderModel;
	shaderModel.HighestShaderModel = D3D_HIGHEST_SHADER_MODEL;

	#ifdef FOR_DX_12_NOTES_21_FEV_2023
	D3D_SHADER_MODEL_5_1 = 0x51,
	D3D_SHADER_MODEL_6_0 = 0x60,
	D3D_SHADER_MODEL_6_1 = 0x61,
	D3D_SHADER_MODEL_6_2 = 0x62,
	D3D_SHADER_MODEL_6_3 = 0x63,
	D3D_SHADER_MODEL_6_4 = 0x64,
	D3D_SHADER_MODEL_6_5 = 0x65,
	D3D_SHADER_MODEL_6_6 = 0x66,
	D3D_SHADER_MODEL_6_7 = 0x67,
	#endif
	while (true) 
	{
		if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel))))
		{
			ShaderVersionH = shaderModel.HighestShaderModel >> 4;
			ShaderVersionL = shaderModel.HighestShaderModel & 0xF;
			WOMA::logManager->DEBUG_MSG(TEXT("Failed to query (shaderModel: %d.%d) for Adapter: 1\n"), ShaderVersionH, ShaderVersionL);
			//WOMA::WomaMessageBox(TEXT("Failed to query (shaderModel) for Adapter: 1\n"), TEXT("FATAL"), MB_OK);
			int i = shaderModel.HighestShaderModel;
			i--;
			shaderModel.HighestShaderModel = (D3D_SHADER_MODEL)i;
		}
		else
			break;
	}

	ShaderVersionH = shaderModel.HighestShaderModel >> 4;
	ShaderVersionL = shaderModel.HighestShaderModel & 0xF;
	WOMA::logManager->DEBUG_MSG("(shaderModel) query for Adapter: 1 HighestShaderModel: %d.%d\n", ShaderVersionH, ShaderVersionL);

	szShaderModel[0] = '0' + ShaderVersionH;
	szShaderModel[1] = '.';
	szShaderModel[2] = '0' + ShaderVersionL;
}

//Init Step: 4
#if defined USE_RASTERIZER_STATE
// ----------------------------------------------------------------------------------------------
bool DX12Class::createAllRasterizerStates(bool lineAntialiasing)
// ----------------------------------------------------------------------------------------------
{
	for (UINT cullMode = 0; cullMode < 3; cullMode++)			//0..2
	{
		for (UINT fillMode = 0; fillMode < 2; fillMode++)		//0..1
		{
			if (cullMode == CULL_FRONT)
				m_rasterState[cullMode][fillMode].CullMode = D3D12_CULL_MODE_FRONT;
			if (cullMode == CULL_BACK)
				m_rasterState[cullMode][fillMode].CullMode = D3D12_CULL_MODE_BACK;
			if (cullMode == CULL_NONE)
				m_rasterState[cullMode][fillMode].CullMode = D3D12_CULL_MODE_NONE;

			if (fillMode == FILL_SOLID)
				m_rasterState[cullMode][fillMode].FillMode = D3D12_FILL_MODE_SOLID;
			if (fillMode == FILL_WIRE)
				m_rasterState[cullMode][fillMode].FillMode = D3D12_FILL_MODE_WIREFRAME;

			m_rasterState[cullMode][fillMode].FrontCounterClockwise = FALSE;
			m_rasterState[cullMode][fillMode].DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			m_rasterState[cullMode][fillMode].DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			m_rasterState[cullMode][fillMode].SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;

			m_rasterState[cullMode][fillMode].DepthClipEnable = TRUE;	// Enable clipping based on distance: http://technet.microsoft.com/de-de/subscriptions/ff476198%28v=vs.85%29.aspx

			m_rasterState[cullMode][fillMode].MultisampleEnable = (SystemHandle->AppSettings->MSAA_ENABLED) ? true : false;	// Enable/Disable multisample antialiasing.
			m_rasterState[cullMode][fillMode].AntialiasedLineEnable = (SystemHandle->AppSettings->MSAA_ENABLED) ? false : true;
			//rasterizerState[cullMode][fillMode].AntialiasedLineEnable = FALSE; //lineAntialiasing;				// Enable line antialiasing; only applies if doing line drawing and MultisampleEnable is false.

			m_rasterState[cullMode][fillMode].ForcedSampleCount = 0;
			m_rasterState[cullMode][fillMode].ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		}
	}

    return true;
}
#endif

// REVIEW - Init Step: 5 - Rasterizer State: Set the Viewport for rendering
// ----------------------------------------------------------------------------------------------
void DX12Class::setViewportDevice(int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	// Create the viewport:
	ASSERT(screenWidth > 0 && screenHeight > 0);

	// The viewport also needs to be setup so that Direct3D can map clip space coordinates to the render target space. 
	// Set this to be the entire size of the window. 
	m_viewport.TopLeftX = 0.0f;	// MinX
	m_viewport.TopLeftY = 0.0f;	// MinY
	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;

	m_viewport.MinDepth = 0.0f;	// MinZ
	m_viewport.MaxDepth = 1.0f;	// MaxZ


	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = static_cast<LONG>(screenWidth);
	m_scissorRect.bottom = static_cast<LONG>(screenHeight);

}

//Init Step: 6 [Create] render target views (RTVs).
// ----------------------------------------------------------------------------------------------
bool DX12Class::CreateRenderTargetView(int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	//EQUAL: auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

	// Create a RTV for each "Frame Buffer".
	for (UINT n = 0; n < BufferCount; n++)
	{
		ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
		m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);

		//EQUAL: rtvHandle.ptr += 1 * m_rtvDescriptorSize;
		rtvHandle.Offset(1, m_rtvDescriptorSize);
			
		WCHAR name[25];
		swprintf_s(name, L"Render Target View: %d", n);
		m_renderTargets[n]->SetName(name);
	}

	return true;
}


// Bind the render target view and depth stencil buffer to the output render pipeline.
// ----------------------------------------------------------------------------------------------
void DX12Class::SetBackBufferRenderTarget()	
// ----------------------------------------------------------------------------------------------
{
}
#if defined USE_RASTERIZER_STATE
// ----------------------------------------------------------------------------------------------
void DX12Class::SetRasterizerState(UINT CullMode, UINT fillMode) 
// ----------------------------------------------------------------------------------------------
{
	m_CullMode = CullMode;
	m_fillMode = fillMode;

	//rasterState = &m_rasterState[CullMode][fillMode];
}
#endif

// ----------------------------------------------------------------------------------------------
bool DX12Class::createSetDepthStencilState (bool depthTestEnabled)
// ----------------------------------------------------------------------------------------------
{
	return true;
}



// -----------------------------------------------------------------
//These are the new functions for enabling and disabling the Z buffer. To turn Z buffering on we set the original depth stencil. 
//To turn Z buffering off we set the new depth stencil that has depthEnable set to false. 
//Generally the best way to use these functions is first do all your 3D rendering, then turn the Z buffer off and do your 
//2D rendering, and then turn the Z buffer on again.

static bool g_Zbuffer = false;
// ----------------------------------------------------------------------------------------------
void DX12Class::TurnZBufferOn()
// ----------------------------------------------------------------------------------------------
{
    if (g_Zbuffer) return;
    g_Zbuffer = true;
}

// ----------------------------------------------------------------------------------------------
void DX12Class::TurnZBufferOff()
// ----------------------------------------------------------------------------------------------
{
    if (!g_Zbuffer) return;
    g_Zbuffer = false;
}

// TODO: 

// Set CAMERA for "2D" (Once per Re-Size only)
//-----------------------------------------------------------------------------------------
void DX12Class::SetCamera2D()
{
}

// ----------------------------------------------------------------------------------------------
void DX12Class::Initialize3DCamera()
// ----------------------------------------------------------------------------------------------
{

	// Normal Camera:

}


// TODO: go to Virtual Class?

}

#endif
#endif

