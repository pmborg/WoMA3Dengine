// ----------------------------------------------------------------------------------------------
// Filename: DX11Class.cpp
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
//
// PURPOSE: MAIN PURPOSE: Work as a DirectX 11 Driver.
//
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#include "mem_leak.h"
#if defined DX_ENGINE
#include "OSmain_dir.h"
#include <d3d11.h>
#if defined DX11 || defined DX9

#include "InputClass.h"

#include "Math3D.h"
#include "DXcameraClass.h"

#include "dxWinSystemClass.h"	// SystemHandle
#include "dx11Class.h"

#if D3D11_SPEC_DATE_YEAR == 2009 //defined DX9 
#include <D3dx9core.h>		//D3DX_SDK_VERSION (Checks for the existance of the correct D3DX library version)
#endif

// ----------------------------------------------------------------------------------------------
// Gloabals:
// ----------------------------------------------------------------------------------------------

namespace DirectX {

void LOAD_TEXTURES(std::vector<TCHAR*> file, std::vector<ID3D11ShaderResourceView**> ppShaderResourceView) {}


DX11Class::~DX11Class() // Used for Static Classes
{
    Shutdown();
	CLASSDELETE();
}

// ----------------------------------------------------------------------------------------------
DX11Class::DX11Class()
// ----------------------------------------------------------------------------------------------
{
	// WomaDriverClass / Public: ------------------------------------------------------
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

	// SUPER: 
	dx11_force_dx9 = false;

	// Video Card Info:
	// ---------------------------------------------------------------------------
	if (!WOMA::UseWarpDevice)
		_tcscpy_s(driverName, sizeof(driverName), TEXT("DX11"));
	else
		_tcscpy_s(driverName, sizeof(driverName), TEXT("DX11 WARP"));

	ZeroMemory( &ShaderModel, sizeof(ShaderModel) );					
	ZeroMemory( &szShaderModel, sizeof(szShaderModel) );				

	ZeroMemory( &m_videoCardDescription, sizeof(m_videoCardDescription) );
	m_videoCardMemory = NULL;
	ZeroMemory( &adapterDesc_Description, sizeof(adapterDesc_Description) );
	ufreededicatedVideoMem = NULL;

	// List of resoltions availabel to Use
	// ---------------------------------------------------------------------------
	numerator = denominator = 1;
	MonitorNumber = 0;	// <- Will have the number of Monitors
	numModes = NULL;

	// MSAA Used:
	// ----------------------------------------------------------------------------
	if (SystemHandle->AppSettings->MSAA_Anisotropic)
		MSAA_COUNT = MAX(1, SystemHandle->AppSettings->MSAA_AnisotropicLevel);	// Req. Note: DX12 min: 1
	else
		MSAA_COUNT = 1;
	MSAA_QUALITY = 0;																	// Req. Note: default: 1

	// DX11Class()
	// Public: ------------------------------------------------------------------------
	m_device11 = NULL;

	adapterGraphicCard = NULL;
	m_deviceContext = NULL;

	//m_backBuffer = NULL;
	ScissorEnable = false;

	mCurRasterState = 0;

	// ---------------------------------------------------------
	g_ALLOW_DX9x = false;
	ShaderVersionH = ShaderVersionL = 0;

	displayModeList = NULL;
	// ---------------------------------------------------------
	#if defined USE_DX_DRIVER_FONT
	CWcullMode = NULL;

	d3d101Device = NULL;
	//keyedMutex11 = NULL;
	//keyedMutex10 = NULL;
	D2DRenderTarget = NULL;	
	Brush = NULL;
	sharedTex11 = NULL;	
	d2dVertBuffer = NULL;
	d2dIndexBuffer = NULL;
	d2dTexture = NULL;
	DWriteFactory = NULL;

	TextFormat = NULL;

	cbPerObjectBuffer = NULL;
	Transparency = NULL;

	//m_FontV2Shader = NULL;
	#endif


	#if defined USE_FRUSTRUM
		frustum				= NULL;
	#endif

	#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING // 26
		//Set the two new blending states to null.
		m_alphaEnableBlendingState = NULL;
		m_alphaDisableBlendingState = NULL;
	#endif

	// Private: ------------------------------------------------------------------------
	m_depthStencilBuffer = NULL;

	//Initialize the new depth stencil state to null in the class constructor.
	m_depthStencilState = NULL;
	m_depthDisabledStencilState = NULL;

#if defined USE_RASTERIZER_STATE
	ZeroMemory( &m_rasterState, sizeof(m_rasterState) );
#endif

	#if defined USE_RASTERTEK_TEXT_FONT && D3D11_SPEC_DATE_YEAR == 2009
		loadInfo.Width = D3DX11_DEFAULT;
		loadInfo.Height = D3DX11_DEFAULT;
		loadInfo.Depth = D3DX11_DEFAULT;
		loadInfo.FirstMipLevel = D3DX11_DEFAULT;
		loadInfo.MipLevels = D3DX11_DEFAULT;

		loadInfo.Usage = D3D11_USAGE_IMMUTABLE;				// Resource will be read-only by the GPU, so Stay inside GPU if possible!
		loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// RESOURCE is a "Source" not a "Target"

		loadInfo.CpuAccessFlags = D3DX11_DEFAULT;
		loadInfo.MiscFlags = D3DX11_DEFAULT;
		loadInfo.Format = DXGI_FORMAT_FROM_FILE;			//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // COMPRESS HERE: DXGI_FORMAT_BC3_UNORM
		loadInfo.Filter = D3DX11_DEFAULT;					//D3DX11_FILTER_SRGB_IN  | D3DX11_FILTER_SRGB_OUT | D3DX11_FILTER_NONE ;
		loadInfo.MipFilter = D3DX11_DEFAULT;
		loadInfo.pSrcInfo = NULL;  
	#endif


}


// The Shutdown function will release and clean up all the pointers used in the Initialize function

//----------------------------------------------------------------------------------------------
void DX11Class::Shutdown2D()
//----------------------------------------------------------------------------------------------
{
#if defined USE_DX_DRIVER_FONT
	if (SystemHandle->m_Application) {
		SAFE_SHUTDOWN(SystemHandle->m_Application->m_FontV2Shader);	// Release previews Font Size (on Window Re-size)
		SAFE_RELEASE(SystemHandle->m_Application->keyedMutex11);
		SAFE_RELEASE(SystemHandle->m_Application->keyedMutex10);
	}

	SAFE_RELEASE(CWcullMode);

	SAFE_RELEASE(D2DRenderTarget);
	SAFE_RELEASE(Brush);

	SAFE_RELEASE(sharedTex11);
	SAFE_RELEASE(d2dVertBuffer);
	SAFE_RELEASE(d2dIndexBuffer);

	SAFE_RELEASE(d2dTexture);
	SAFE_RELEASE(DWriteFactory);

	SAFE_RELEASE(TextFormat);

	SAFE_RELEASE(cbPerObjectBuffer);
	SAFE_RELEASE(Transparency);

	SAFE_RELEASE(d3d101Device);
#endif
}


//----------------------------------------------------------------------------------------------
void DX11Class::Shutdown()
//----------------------------------------------------------------------------------------------
{
	if (m_deviceContext)
	{
		//m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		DeleteViewBuffers();
	}

	Shutdown2D();

#if defined USE_RASTERIZER_STATE
//CreateAllRasterizerStates:
for (UINT i = 0; i < 3; i++)
	for (UINT j = 0; j < 2; j++)
		SAFE_RELEASE(m_rasterState[i][j]);
#endif

//Release the two new blending states.
#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
	SAFE_RELEASE (m_alphaEnableBlendingState);
	SAFE_RELEASE (m_alphaDisableBlendingState);
#endif

	womalogauto (TEXT("Number of Textures: %d\n"), allTextureNameArray.size());

	for (UINT i=0; i < allTextureNameArray.size(); i++)
		SAFE_RELEASE (allTexturePointerArray[i]);	// Free All Textures from our Texture manager

#if defined USE_FRUSTRUM
	SAFE_DELETE(frustum);
#endif

	// createSetDepthStencilState() - Here we release the new depth stencil during the Shutdown function.
	SAFE_RELEASE (m_depthDisabledStencilState);
	SAFE_RELEASE (m_depthStencilState);

	SAFE_RELEASE (adapterGraphicCard);
    if (m_deviceContext) {
        m_deviceContext->Flush();
        SAFE_RELEASE(m_deviceContext);
    }

	// For each Monitor: 
	for (int i = 0; i < DX11windowsArray.size(); i++) {
		if (DX11windowsArray[i].m_swapChain1)
			SAFE_RELEASE(DX11windowsArray[i].m_swapChain1);
	}

#ifdef USE_DX11_3
	SAFE_RELEASE(pDevice3);
#endif

    ULONG count = 0;
    if (m_device11)
    {
        count = m_device11->Release();
        m_device11 = nullptr;
    }

#ifdef _DEBUG
    if (count && debugDev)
    {
        debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
        womalog("WOMA WARNING: There are %d unreleased references left on the D3D device!\n", count);
    }
    SAFE_RELEASE(debugDev);
#endif
    
}

// |Init Step: 1| This is for DIRECTX Driver only!
// ----------------------------------------------------------------------------------------------
BOOL DX11Class::CheckAPIdriver(UINT USE_THIS_ADAPTER_CARD)
// ----------------------------------------------------------------------------------------------
{
	IDXGIFactory1* pDXGIFactory1 = NULL;

	/*******************************************************************************
	 Checks for the for the proper RunTime D3DX library version: (D3DX_SDK_VERSION)
	*******************************************************************************/
#if D3D11_SPEC_DATE_YEAR == 2009
	TCHAR name[MAX_STR_LEN];
	StringCchPrintf(name, MAX_STR_LEN, TEXT("D3DX9_%d.dll"), D3DX_SDK_VERSION);

	HINSTANCE hinstLib = LoadLibrary(name);
	if (!hinstLib)
	{
		TCHAR error[MAX_STR_LEN];
		StringCchPrintf(error, MAX_STR_LEN, TEXT("Fatal Error: Wrong D3DX_SDK_VERSION, Could not load: %d"), D3DX_SDK_VERSION);
		WomaFatalExceptionW(error);
		return FALSE;
	}

	FreeLibrary(hinstLib);
#endif

	/*******************************************************************
	// Check for DX9, Load DX 9 DLL if is installed...
	/******************************************************************/
if (dx11_force_dx9) 
{
	_tcscpy_s(driverName, sizeof(driverName), TEXT("DX9(API_DX11)"));	// driverName = TEXT ("DX11"); // 
	if (LoadLibrary(TEXT("d3d9.dll")))
		m_sCapabilities.CapDX9 = TRUE;
	else
		if (g_ALLOW_DX9x)
		{
			WomaFatalException( "FATAL ERROR: DX9 Device, not supported. Could not load d3d9.dll");
			return FALSE;
		}
}//#endif

	/*******************************************************************
	// Check for a DX10/11 Instalation (Need to have: dxgi.dll) not present at Window XP
	/******************************************************************/
	if (!LoadLibrary(TEXT("dxgi.dll"))) // NOTE: Windows XP Can't do this (SO WINDOWS XP NOT SUPPORTED!)
		{ WomaMessageBox(TEXT("dxgi.dll"), TEXT("Error, Could not load: ")); return FALSE; }

	/******************************************************************/
	// Create a DirectX 10/11 graphics interface factory.
	/******************************************************************/
	IF_FAILED_RETURN_FALSE(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pDXGIFactory1));	// Use: dxgi.dll
	if (!pDXGIFactory1)
	{
		WomaMessageBox(TEXT("Could not create DXGIFactory1"), TEXT("DX11Class")); 
		return FALSE;
	}

	/******************************************************************/
	// Check DX APIs, that can be used on this: "USE_THIS_ADAPTER_CARD"
	/******************************************************************/
	IDXGIAdapter1* pAdapter;

	if (pDXGIFactory1->EnumAdapters1(USE_THIS_ADAPTER_CARD, &pAdapter) == S_OK)
	{
		if (dx11_force_dx9)
		{
			if (&m_sCapabilities.CapDX9)			 // Can Use DX9?
				g_ALLOW_DX9x = TRUE; // use it!
			m_sCapabilities.CapDX10_11 = FALSE;
		} else {//#else
			m_sCapabilities.DXGI10 = false;
			if (!FAILED(pAdapter->CheckInterfaceSupport(__uuidof (ID3D10Device), NULL)))
			{
				m_sCapabilities.CapDX10_11 = TRUE;
				m_sCapabilities.DXGI10 = true;
				womalog("DXGI1.0: Available\n");
			}
		}//#endif
	}

    SAFE_RELEASE(pAdapter);			
	SAFE_RELEASE(pDXGIFactory1);	

	return TRUE;
}

// |Init Step: 3| This is for OPENGL Driver only!
// ----------------------------------------------------------------------------------------------
BOOL DX11Class::Check (int* Hi, int* low){return TRUE;}
// ----------------------------------------------------------------------------------------------


// MAIN INIT - CreateDevice and CreateSwapChain and also "Get the best MultiSampleQuality":
//----------------------------------------------------------------------------------------------
bool DX11Class::OnInit(int g_USE_MONITOR, /*HWND*/void* hwnd, int screenWidth, int screenHeight, 
							  UINT depthBits, float screenDepth, float screenNear, BOOL msaa, bool vsync, 
							  BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize) 
//----------------------------------------------------------------------------------------------
{
	m_VSYNC_ENABLED = vsync;

	womalog("-------------------------\n");
	womalogauto(TEXT("INITIALIZING DRIVER: %s\n"), driverName);
    womalog("-------------------------\n");

    //Init Step: 1 - Check Driver for DX9 and DX10 and DX12(=false) on DX11 API
    ASSERT(CheckAPIdriver(/* Use Graph Card 1 */ USE_THIS_GRAPHIC_CARD_ADAPTER));
   
    /*Init Step: 2 - Create Factory
    Get list of all MODES for all MONITORS
    Get Refresh Rate
    Get BUFFER_COLOR_FORMAT
    */
    getModesList(SystemHandle->AppSettings->UI_MONITOR,
        SystemHandle->AppSettings->WINDOW_WIDTH,
        SystemHandle->AppSettings->WINDOW_HEIGHT,
        SystemHandle->AppSettings->FULL_SCREEN,
        &numerator, &denominator);
    
	//Init Step: 3, 4
	ASSERT(createDevice_legacy());
    

#ifdef USE_DX11_3
	//https://learn.microsoft.com/en-us/windows/win32/api/d3d11_3/nn-d3d11_3-id3d11device3
	m_device11->QueryInterface(__uuidof(ID3D11Device3), reinterpret_cast<void**>(&pDevice3));
#endif

#if defined USE_DX11_1
	if (featureLevel_ > D3D_FEATURE_LEVEL_11_1) {
		printf("LEVEL_11_1 createSwapChainDX11device2()\n");
		if (!createSwapChainDX11device2((HWND)hwnd, screenWidth, screenHeight, vsync, fullscreen, g_UseDoubleBuffering, g_AllowResize, numerator, denominator))
		{
			printf("LEVEL_11_1 retry on: createSwapChainDX11device()\n");
			IF_NOT_RETURN_FALSE(createSwapChainDX11device((HWND)hwnd, screenWidth, screenHeight, vsync, fullscreen, g_UseDoubleBuffering, g_AllowResize, numerator, denominator));
		}
	} else 
#endif
	{
        ASSERT(true);
	}

	//Init Step: 5 - Get Best Shader of this Graphic Card: dx10,dx10.1,dx11,etc... OUTPUT: ShaderModel
	getProfile (g_USE_MONITOR);					

	//Init Step: 6 Before Resize (SetCamera2D & SetCamera3D)!
		Initialize3DCamera();

	//Init Step: 7 (Include: 8,9,10,11,12)
	// Creates a render target view and depth stencil surface/view per swapchain
	ASSERT (Resize (screenWidth, screenHeight, screenNear, screenDepth, fullscreen, depthBits));
   
#if defined USE_RASTERIZER_STATE
	//Init Step: 8 - Cull Back / Front:
	ASSERT( createRasterizerStates (/*lineAntialiasing*/ false)); // Only applies: if doing "line drawing" and "MultisampleEnable" is false.
	SetRasterizerState (CULL_NONE, FILL_SOLID);	//Set Default
#endif

  #if defined USE_FRUSTRUM
	frustum = NEW DXfrustumClass;	// Create Frustum
  #endif

  #if defined INTRO_DEMO || defined USE_ALPHA_BLENDING // 26
	//Init Step: 13 Create and Set the depth stencil state: With the created depth stencil state we can now set it so that it takes effect:
	ASSERT( createSetDepthStencilState (true) );	// Zbuffer on state
	ASSERT( createSetDepthStencilState (false) );	// Zbuffer off state

	//Init Step: 14 - Transparency: To render text on top of 3D
	ASSERT(CreateBlendState());
  #endif

	return true;
}

void DX11Class::DeleteViewBuffers()
{
	m_deviceContext->OMSetRenderTargets(0, NULL, NULL);

    // For each Monitor: 
    for (int i = 0; i < DX11windowsArray.size(); i++)
    {
        SAFE_RELEASE(DX11windowsArray[i].m_backBuffer);			// Release pointer to the back buffer
        SAFE_RELEASE(DX11windowsArray[i].m_renderTargetView);	// Init Step: 9	(backBufferRTV)
    }

    SAFE_RELEASE(m_depthStencilBuffer);								// Init Step: 9

    for (int i = 0; i < DX11windowsArray.size(); i++)
    {
        SAFE_RELEASE(DX11windowsArray[i].m_depthStencilView);	// Init Step: 10
    }

    m_deviceContext->Flush();

	// For each Monitor, Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	for (int i = 0; i < DX11windowsArray.size(); i++)
	{
		//if (DX11windowsArray[i].m_swapChain)
		//	DX11windowsArray[i].m_swapChain->SetFullscreenState(false, NULL);

		if (DX11windowsArray[i].m_swapChain1)
			DX11windowsArray[i].m_swapChain1->SetFullscreenState(false, NULL);
	}


}

//----------------------------------------------------------------------------------------------
bool DX11Class::Resize (int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits)
//----------------------------------------------------------------------------------------------
{
HRESULT result = S_OK;

	RenderfirstTime = true;	 // Used on SPRITES!

	if (m_deviceContext)
	{
		DeleteViewBuffers();

		for (int i = 0; i < DX11windowsArray.size(); i++)
		{
			// #Resize: Init Step: 8 - Resize internal Buffers for new Window size:
			if (DX11windowsArray[i].m_swapChain1)
			{
				result = (DX11windowsArray[i].m_swapChain1->ResizeBuffers((SystemHandle->AppSettings->UseDoubleBuffering) ? 2 : 1 , screenWidth, screenHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0));
				// If the device was reset we must completely reinitialize the renderer.
				if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
				{
					OnDeviceLost();
                    return false;
				}
				else
				{
					{ if (FAILED(result)) { WomaFatalException("FATAL: ResizeBuffers() error!"); } }
				}
			}
		}

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 }; //ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		//swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferCount = (SystemHandle->AppSettings->UseDoubleBuffering) ? 2 : 1; // Use double-buffering to minimize latency.

		// Set the width and height of the back buffer.
		swapChainDesc.Width = screenWidth;
		swapChainDesc.Height = screenHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		//swapChainDesc.OutputWindow = SystemHandle->m_hWnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = MSAA_COUNT;
		swapChainDesc.SampleDesc.Quality = MSAA_QUALITY;

		// Discard the back buffer contents after presenting.
        swapChainDesc.SwapEffect =  DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
#if defined USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#else
		swapChainDesc.Flags = 0;
#endif

        DXwindowDataContainer DXwindow = {};

		// First, retrieve the underlying DXGI Device from the D3D Device.
        IDXGIDevice1* dxgiDevice;
        HRESULT hr = m_device11->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
        if (FAILED(hr)) {
            WomaFatalException("Failed to query IDXGIDevice1 interface.");
            return false;
        }

		// Identify the physical adapter (GPU or card) this device is running on.
        IDXGIAdapter* dxgiAdapter;
        hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
        if (FAILED(hr)) {
            SAFE_RELEASE(dxgiDevice);
            WomaFatalException("Failed to get IDXGIAdapter from IDXGIDevice1.");
            return false;
        }

		// And obtain the factory object that created it.
        IDXGIFactory2* dxgiFactory;
        hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
        if (FAILED(hr)) {
            SAFE_RELEASE(dxgiAdapter);
            SAFE_RELEASE(dxgiDevice);
            WomaFatalException("Failed to get IDXGIFactory2 from IDXGIAdapter.");
            return false;
        }

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		if (WOMA::game_state == GAME_LOADING || WOMA::game_state == GAME_SETUP || DX11windowsArray.size() == 0)
			DX11windowsArray.push_back(DXwindow);

		// Create a SwapChain from a Win32 window.
        hr = dxgiFactory->CreateSwapChainForHwnd(
			m_device11,
			SystemHandle->m_hWnd,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,
			&DX11windowsArray[DX11windowsArray.size()-1].m_swapChain1
		);

        if (FAILED(hr)) {
            SAFE_RELEASE(dxgiFactory);
            SAFE_RELEASE(dxgiAdapter);
            SAFE_RELEASE(dxgiDevice);
            WomaFatalException("Failed to create swap chain for HWND.");
            return false;
        }

        // Release resources.
        SAFE_RELEASE(dxgiFactory);
        SAFE_RELEASE(dxgiAdapter);
        SAFE_RELEASE(dxgiDevice);

		// #CreateViewBuffers:
		// -------------------

		// [1] CreateRenderTargetView
		IF_NOT_RETURN_FALSE (CreateRenderTargetView (screenWidth, screenHeight));
		
		// [2] CreateTexture2D
		IF_FAILED_RETURN_FALSE (createDepthStencil(screenWidth, screenHeight, fullscreen, depthBits));

		// CreateDepthStencilView:
		// -----------------------
		// NOTE: need to be before createSetDepthStencilView()
		for (int i = 0; i < DX11windowsArray.size(); i++)
			setViewportDevice(i, screenWidth, screenHeight);  // RSSetViewports: Map Screen clip space coordinates to the render target space

#if defined SET_DEVICE_CAPABILITIES
		// [3] CreateDepthStencilView / OMSetRenderTargets:
		ASSERT (createSetDepthStencilView (screenWidth, screenHeight));
#endif

		// This will bind the render target view and the depth stencil buffer to the output render pipeline. 
		// This way the graphics that the pipeline renders will get drawn to our back buffer that we previously created. 
		// With the graphics written to the back buffer we can then swap it to the front and display our graphics on the user's screen. 
		//
		// OMSetRenderTargets: NOTE: Need to be After [1], [2] and [3]
		// For each Monitor: 
		for (int i = 0; i < DX11windowsArray.size(); i++)
			SetBackBufferRenderTarget(i);

		// #Generate new "ProjectionMatrix" and "OrthoMatrix"
		// --------------------------------------------------
		setProjectionMatrixWorldMatrixOrthoMatrix ( screenWidth, screenHeight, screenNear, screenDepth);
	}

#if defined CLIENT_SCENE_TEXT || defined USE_VIEW2D_SPRITES
	Initialize3DCamera();
#endif

	#if defined USE_DX_DRIVER_FONT
	m_sCapabilities.USE_DXDRIVER_FONTSBoolean = InitD2D_D3D101_DWrite(adapterGraphicCard, L"Consolas", screenWidth, screenHeight, 1, 1, 1); // RGB
	#endif

	return true;
}

//Init Step: 5 - Set the best shader availabel: MORE INFO: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476876%28v=vs.85%29.aspx
// ----------------------------------------------------------------------------------------------
void DX11Class::getProfile ( UINT g_USE_MONITOR )
// ----------------------------------------------------------------------------------------------
{
	// Get Best FeatureLevel Availabel:
	D3D_FEATURE_LEVEL featureLevel = m_device11->GetFeatureLevel();
	switch (featureLevel) 
	{
		//DX 9.x
		case D3D_FEATURE_LEVEL_9_1:	// To Support old Graphic Cards
			_tcscpy_s(ShaderModel, TEXT("4_0_level_9_1")); // ShaderModel = TEXT("2_0"); // 
			ShaderVersionH = 2; ShaderVersionL = 0;
			womalogauto(TEXT("\nBest Shader Model 2.0 (Best API: DX9.1)\n"));
		break;

		case D3D_FEATURE_LEVEL_9_2:	// To Support old Graphic Cards
			_tcscpy_s(ShaderModel, TEXT("4_0_level_9_2")); //ShaderModel = TEXT("2_0"); // 
            ShaderVersionH = 2; ShaderVersionL = 0;
			womalogauto(TEXT("\nBest Shader Model 2.0 (Best API: DX9.2)\n"));
		break;

		case D3D_FEATURE_LEVEL_9_3:	// To Support old Graphic Cards
            _tcscpy_s(ShaderModel, TEXT("4_0_level_9_3")); // ShaderModel = TEXT("2_0"); // 
            ShaderVersionH = 2; ShaderVersionL = 0;
			womalogauto(TEXT("\nBest Shader Model 2.0 (Best API: DX9.3)\n"));
		break;

		//DX 10
		case D3D_FEATURE_LEVEL_10_0:	// To Support old Graphic Cards
            _tcscpy_s(ShaderModel, TEXT("4_0")); // ShaderModel = TEXT("4_0"); // 
            ShaderVersionH = 4; ShaderVersionL = 0;
			womalog("\nBest Shader Model 4.0 (Best API: DX10)\n");
		break;

		// DX10.1
		case D3D_FEATURE_LEVEL_10_1:	// To Support old Graphic Cards
            _tcscpy_s(ShaderModel, TEXT("4_1")); // ShaderModel = TEXT("4_1"); // 
            ShaderVersionH = 4; ShaderVersionL = 1;
			womalog("\nBest Shader Model 4.1 (Best API: DX10.1)\n");
		break;

		case D3D_FEATURE_LEVEL_11_0:
			_tcscpy_s(ShaderModel, TEXT("5_0")); // ShaderModel = TEXT("5_0"); // 
			ShaderVersionH = 5; ShaderVersionL = 0;
			womalog("\nBest Shader Model 5.0 (Best API: DX11)\n");
		break;
		case D3D_FEATURE_LEVEL_11_1:
			_tcscpy_s(ShaderModel, TEXT("5_0")); // ShaderModel = TEXT("5_0"); // 
			ShaderVersionH = 5; ShaderVersionL = 0;
			womalog("\nBest Shader Model 5.0 (Best API: DX11.1)\n");
		break;
		case D3D_FEATURE_LEVEL_12_0:
			_tcscpy_s(ShaderModel, TEXT("5_0")); // ShaderModel = TEXT("5_0"); // 
			ShaderVersionH = 5; ShaderVersionL = 0;
			womalog("\nBest Shader Model 5.0 (Best API: DX11_level_12_0)\n");
		break;

		default: //For Future DX Versions!	
		case D3D_FEATURE_LEVEL_12_1:
			_tcscpy_s(ShaderModel, TEXT("5_0")); // ShaderModel = TEXT("5_0"); // 
			ShaderVersionH = 5; ShaderVersionL = 0;
			womalog("\nBest Shader Model 5.0 (Best API: DX11_level_12_1)\n");
			break;

		break;
	}
	womalog("\n");

	_tcscpy_s(szShaderModel, ShaderModel); szShaderModel[1] = '.';	// Convert: "x_y" to" x.y"
}

// Bind the render target view and depth stencil buffer to the output render pipeline.
// ----------------------------------------------------------------------------------------------
void DX11Class::SetBackBufferRenderTarget(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	m_deviceContext->OMSetRenderTargets(1, &DX11windowsArray[monitorWindow].m_renderTargetView,
											DX11windowsArray[monitorWindow].m_depthStencilView);
	m_deviceContext->RSSetViewports(1, &DX11windowsArray[monitorWindow].viewport);
}

//Init Step: 6 - Create Rendering Target
// ----------------------------------------------------------------------------------------------
bool DX11Class::CreateRenderTargetView (int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	// For each Monitor: 
	for (int i = 0; i < DX11windowsArray.size(); i++)
	{
		if (DX11windowsArray[i].m_swapChain1) {
			IF_FAILED_RETURN_FALSE(DX11windowsArray[i].m_swapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&DX11windowsArray[i].m_backBuffer));
		  
																																				 
		} 
		
		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescMSAA(D3D11_RTV_DIMENSION_TEXTURE2DMS);			//MSAA_on
		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D);					//MSAA_off
		IF_FAILED_RETURN_FALSE(m_device11->CreateRenderTargetView	(DX11windowsArray[i].m_backBuffer, 
																	(SystemHandle->AppSettings->MSAA_Anisotropic) ? &renderTargetViewDescMSAA:&renderTargetViewDesc,
																	&DX11windowsArray[i].m_renderTargetView));	//backBufferRTV
	}

	return true;
}

// ----------------------------------------------------------------------------------------------
bool DX11Class::Initialize(float* clearColor)
// ----------------------------------------------------------------------------------------------
{
	womalog("DX11Class::Initialize()\n");

	// Set default clear color:
	driver_ClearColor[0] = *clearColor++;
	driver_ClearColor[1] = *clearColor++;
	driver_ClearColor[2] = *clearColor++;
	driver_ClearColor[3] = *clearColor;

	return true;
}

void DX11Class::Finalize() {} //not used on DX11

// ----------------------------------------------------------------------------------------------
void DX11Class::BeginScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
#if !defined ANDROID_PLATFORM && defined DX_ENGINE && defined USE_MULTI_MONITOR
	if (SystemHandle->windowsArray.size() == 3)
	{
		// TODO: settings.xml define: LEFT/RIGTH: Monitor
		// Monitors Index:
		// | 1 | 0 | 2 |
		if (monitorWindow == 1) DXsystemHandle->m_Camera->m_rotationY -= (90 / 3); //  90:3 = 30deg
		if (monitorWindow == 0) DXsystemHandle->m_Camera->m_rotationY -= 0;
		if (monitorWindow == 2) DXsystemHandle->m_Camera->m_rotationY += (90 / 3);	//  90:3 = 30deg
	}
#endif

	// Clear Screen
	m_deviceContext->ClearRenderTargetView(DX11windowsArray[monitorWindow].m_renderTargetView, driver_ClearColor);	// Clear the "back buffer":
#if defined SET_DEVICE_CAPABILITIES
	ClearDepthBuffer();
#endif

	SetBackBufferRenderTarget(monitorWindow);
}

//Steps to Handle Device Loss:
//    1.	Release All Resources Tied to the Device
//    •	Release all Direct3D resources(e.g., buffers, textures, shaders, views) that are tied to the device.
//    •	This includes render targets, depth - stencil views, and swap chains.
//    2.	Reset the Device
//    •	Recreate the Direct3D device and its associated context.
//    •	Reinitialize the swap chain and other resources.
//    3.	Reinitialize Resources
//    •	Recreate all resources that were released earlier.
//    •	This includes shaders, textures, buffers, and any other GPU resources.
//    4.	Log the Event
//    •	Log the device loss and recovery process for debugging purposes.

void DX11Class::OnDeviceLost() // Our Driver in use was re-installed??
{
    womalog("Device lost!\n");

    WOMA::game_state = ENGINE_RESTART;
}

void DX11Class::ResetResource(UINT monitorWindow)
{
    //RESET ShaderResources! to avoid HLSL WARNINGS: Resource being set to OM RenderTarget slot 0 is still bound on input!
    //WHY 3,4,5...? hlsl use 4 registers: MAX
    //Texture2D shaderTexture			: register(t0);	// 21: Texture
    //Texture2D AlfaMapTexture			: register(t1);	// 33: AlfaMap
    //Texture2D ShadowMapTextureTexture : register(t2);	// 36: ShadowMap
    //									: register(t3);  t number...

    ID3D11ShaderResourceView* const pSRV[15] = { NULL };
    m_deviceContext->PSSetShaderResources(0, 15, pSRV);
}

// ----------------------------------------------------------------------------------------------
void DX11Class::EndScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	HRESULT hr=S_OK;

	// <PRINT THE 3D SCENE TO SCREEN> to Swap Chain (wait from VSYNC refresh rate, if it is the case)
	if (DX11windowsArray[monitorWindow].m_swapChain1)
		hr = DX11windowsArray[monitorWindow].m_swapChain1->Present(m_VSYNC_ENABLED, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		{ if (FAILED(hr)) { WomaFatalException("FATAL: swapChain->Present() error!"); } }
	}

    ResetResource(monitorWindow);
}


// ----------------------------------------------------------------------------------------------
void DX11Class::GetProjectionMatrix(XMMATRIX& projectionMatrix)
// ----------------------------------------------------------------------------------------------
{
	projectionMatrix = m_projectionMatrix;
}

// ----------------------------------------------------------------------------------------------
void DX11Class::setProjectionMatrixWorldMatrixOrthoMatrix (int screenWidth, int screenHeight,float screenNear, float screenDepth)
// ----------------------------------------------------------------------------------------------
{
	float fieldOfView, screenAspect;

	ASSERT(screenWidth * screenHeight);
	ASSERT(screenNear > 0);
	ASSERT(screenDepth > screenNear);

	/*******************************************************************
	* Set up: m_projectionMatrix and m_orthoMatrix
	*******************************************************************/
	// The projection matrix is used to translate the 3D scene into the 2D viewport space that we previously created. 
	// We will need to keep a copy of this matrix so that we can pass it to our shaders that will be used to render our scenes. 

	// Create the projection matrix:
	UINT num_monitors = (UINT)SystemHandle->windowsArray.size();

	fieldOfView =	(float)(PI / 4.0f) / // Or... 90deg => fieldOfView = (90 / 2) * 0,0174532925f;
					num_monitors;		 // 90: 3(num "Impar" monitors)
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for "3D" rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, screenNear, screenDepth);		// 3D PROJECTION
    m_projectionMatrix_sky = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, 512);			// SKY 3D PROJECTION

#if defined CLIENT_SCENE_TEXT || defined USE_VIEW2D_SPRITES
	// And the final thing we will setup in the Initialize function is an orthographic projection matrix. 
	//This matrix is used for rendering 2D elements like user interfaces on the screen
	// (Create an orthographic projection matrix for 2D rendering)
	m_orthoMatrix = XMMatrixOrthographicLH ((float) screenWidth, (float) screenHeight, screenNear, screenDepth);// 2D PROJECTION
#endif

	// MINI-MAP:
}

#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
// ----------------------------------------------------------------------------------------------
void DX11Class::GetOrthoMatrix(XMMATRIX& orthoMatrix)
// ----------------------------------------------------------------------------------------------
{
	orthoMatrix = m_orthoMatrix;
}
#endif

// Set CAMERA for "2D" (Once per Re-Size only)
//-----------------------------------------------------------------------------------------
void DX11Class::SetCamera2D()
{
	DXcameraClass DX11m_Camera2D;
	RenderfirstTime = true;

	// Set Fixed Camera for 2D:
	DX11m_Camera2D.SetRotation(0, 0, 0);			// NOTE: On 2D This values have always these values!
	DX11m_Camera2D.SetPosition(0.0f, 0.0f, -1.0f);	// NOTE: On 2D This values have always these values!

	// Calculate: 2D ViewMatrix
	DX11m_Camera2D.CalculateViewMatrix();						// ((OpenGLClass*)m_Driver)->m_Camera->Render(); || ((DX_CLASS*)m_Driver)->m_Camera->Render();
	DX11m_Camera2D.Use2DViewMatrix();				// Get the view from the camera and 2D objects.

	DXsystemHandle->m_Camera->m_viewmatrix2D = DX11m_Camera2D.m_viewMatrix;
}
// TODO: go to Virtual Class?
// ----------------------------------------------------------------------------------------------
void DX11Class::Initialize3DCamera()
// ----------------------------------------------------------------------------------------------
{
	if (DXsystemHandle->m_Camera) 
	{

#if defined USE_VIEW2D_SPRITES
	SetCamera2D();
#endif

	// Normal Camera: ( After: SetCamera2D() )
	DXsystemHandle->m_Camera->SetPosition(	SystemHandle->AppSettings->INIT_CAMX, SystemHandle->AppSettings->INIT_CAMY, SystemHandle->AppSettings->INIT_CAMZ);

	DXsystemHandle->m_Camera->SetRotation(	SystemHandle->AppSettings->INIT_ROTX, SystemHandle->AppSettings->INIT_ROTY, SystemHandle->AppSettings->INIT_ROTZ);
	}

}

// TODO: go to Virtual Class?
XMMATRIX* DX11Class::GetViewMatrix( UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
	{
		switch (projection)
		{
			default:
			case PROJECTION_PERSPECTIVE:
				if (camera == CAMERA_NORMAL)
					return &DXsystemHandle->m_Camera->m_viewMatrix;		// TODO: Use a global matrix, one per frame
				break;

			#if defined CLIENT_SCENE_TEXT || defined USE_VIEW2D_SPRITES
			case PROJECTION_ORTHOGRAPH:
				 return &DXsystemHandle->m_Camera->m_viewmatrix2D;		// TODO: Use a global matrix, one per frame
				break;
			#endif
		}
	} 

	ASSERT(FALSE);
	return NULL; //Return ERROR
}

XMMATRIX* DX11Class::GetProjectionMatrix( UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
    if (camera == CAMERA_SKY)
        return &m_projectionMatrix_sky;

	{
		switch (projection)
		{
			default:
			case PROJECTION_PERSPECTIVE:
				return &m_projectionMatrix;	
				break;
			#if defined CLIENT_SCENE_TEXT  || defined USE_VIEW2D_SPRITES
			case PROJECTION_ORTHOGRAPH:
				return &m_orthoMatrix;
				break;
			#endif
		}
	} 

	ASSERT(FALSE);
}


}

#endif
#endif
