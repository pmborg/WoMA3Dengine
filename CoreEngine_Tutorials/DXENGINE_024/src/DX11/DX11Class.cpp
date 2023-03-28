// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: DX11Class.cpp
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
//
// PURPOSE: MAIN PURPOSE: Work as a DirectX 11 Driver.
//
// ----------------------------------------------------------------------------------------------

/*
**Displaying Graphics**

Rendering 2D with Sprites and Text
	Describes step-by-step procedures for using 2D graphics techniques.
	https://msdn.microsoft.com/en-us/library/ff634505.aspx
// ----------------------------------------------------------------------------------------------
Rendering 3D with Effects
	Describes step-by-step procedures for using 3D graphics techniques.
	https://msdn.microsoft.com/en-us/library/bb975154.aspx
// ----------------------------------------------------------------------------------------------
"What Is" Articles:
	// SOURCE: https://msdn.microsoft.com/en-us/library/dd282490.aspx


|Antialiasing| MSAA: off/X1/X2/X4/X8
	Antialiasing is a technique for softening or blurring sharp edges so they appear less jagged when rendered.
	Enabling AntiAliasing (MultiSampling) --> MSAA --> BOOL VAR: mEnable4xMsaa

	-> depth buffers must have the same "sample counts" and "quality levels"
	-> depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	-> Samplers: DXshaderClass::InitializeShader()
	http://www.tweakguides.com/Graphics_11.html

|MULTIPLE SCREENS| https://stackoverflow.com/questions/10744788/display-different-images-per-monitor-directx-10?answertab=active#tab-top

|Back-Buffer| (DX11windowsArray[0].m_backBuffer)
	A back buffer is a render target whose contents will be sent to the device when GraphicsDevice.Present is called.

|Blend-State| (#defined USE_ALPHA_BLENDING)
|Alpha-Blending|
	Blend state controls how color and alpha values are blended when combining rendered data with existing render target data.

|Color-Blending| (#defined USE_DX10DRIVER_FONTS)
	Color blending mixes two colors together to produce a third color.

|Depth-Buffer| (m_depthBuffer)
	A depth buffer contains per-pixel floating-point data for the z depth of each pixel rendered. 
	A depth buffer may also contain stencil data which can be used to do more complex rendering such as simple shadows or outlines

|Depth-Stencil-State|  (m_depthStencilState)
	Depth stencil state controls how the depth buffer and the stencil buffer are used.

|Depth-Texture| (#define USE_SHADOW_MAP)
	A depth texture, also known as a shadow map, is a texture that contains the data from the depth buffer for a particular scene.

|Effect/HLSL Loader|
|Model|
|Model-Bone|
	A model bone is a matrix that represents the position of a mesh as it relates to other meshes in a 3D model.

|Profile| getProfile ( UINT g_USE_MONITOR );
	A profile is a feature set that is implemented in hardware. Like:	
	vs1.1,vs2.1 ps1.1,ps1.2,ps1.3,ps1.4,ps2.0 (DX9.x)
	4.0(DX10), 4.1(DX10.1), 5.0 (DX11), 5.1 (DX12)

|Rasterizer-State|	(#define defined USE_RASTERIZER_STATE)
	Rasterizer state determines how to render 3D data such as position, color, and texture onto a 2D surface.

|Render-Target| (SetBackBufferRenderTarget(UINT monitorWindow))
	A render target is a memory buffer for rendering pixels. One common use for a render target is offscreen rendering.

|Sampler-State|	(m_sampleState)
	Sampler state determines how texture data is sampled using texture addressing modes, filtering, and level of detail.

|Stencil-Buffer| (m_depthStencilView)
				ID3D11DepthStencilState* m_depthStencilState;
				ID3D11DepthStencilState* m_depthDisabledStencilState;
	A stencil buffer contains per-pixel integer data which is used to add more control over which pixels are rendered. 
	A stencil buffer can also be used in combination with a depth buffer to do more complex rendering such as simple shadows or outlines.

|Vectors, Matrices, and Quaternions|
	Presents an overview of the math-related functionality provided by the XNA Framework.

|View-Frustum|	(#define USE_FRUSTRUM)
	A view frustum is a 3D volume that defines how models are projected from camera space to projection space. 
	Objects must be positioned within the 3D volume to be visible.

|Viewport|		(D3D11_VIEWPORT viewport;)
	A viewport is a 2D rectangle that defines the size of the rendering surface onto which a 3D scene is projected.
*/

// Introduction to: Programming DirectCompute & DirectX 11 Compute Shaders:
// http://openvidia.sourceforge.net/index.php/DirectCompute

#include "platform.h"
#include "OSmain_dir.h"
#include <d3d11.h>

#include "win32InputClass.h"
#include "mem_leak.h"

#include "dxWinSystemClass.h"	// SystemHandle
#include "dx11Class.h"

#if D3D11_SPEC_DATE_YEAR == 2009 //defined DX9 
#include <D3dx9core.h>		//D3DX_SDK_VERSION (Checks for the existance of the correct D3DX library version)
#endif

#include "Math3D.h"

// ----------------------------------------------------------------------------------------------
// Gloabals:
// ----------------------------------------------------------------------------------------------
ID3D11DeviceContext* g_deviceContext = NULL;				// Also to Comunicate with DXUT

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
	WomaIntegrityCheck = 1234567890;

	// SUPER: 
	dx11_force_dx9 = false;

	// Video Card Info:
	// ---------------------------------------------------------------------------
	_tcscpy_s(driverName, sizeof(driverName), TEXT("DX11"));	// driverName = TEXT ("DX11"); // 
	ZeroMemory( &ShaderModel, sizeof(ShaderModel) );			// ShaderModel = TEXT(""); //
	ZeroMemory( &szShaderModel, sizeof(szShaderModel) );		// ShaderModel = TEXT(""); //

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
	ReqMSAAmultiSampleCount = SystemHandle->AppSettings->MSAA_X;// Req.
	ReqMSAAquality = 1;											// Req.

	// DX11Class()
	// Public: ------------------------------------------------------------------------
	m_device = NULL;

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


		//Set the two new blending states to null.
		m_alphaEnableBlendingState = NULL;
		m_alphaDisableBlendingState = NULL;

	// Private: ------------------------------------------------------------------------
	m_depthBuffer = NULL;

	//Initialize the new depth stencil state to null in the class constructor.
	m_depthStencilState = NULL;
	m_depthDisabledStencilState = NULL;

	ZeroMemory( &m_rasterState, sizeof(m_rasterState) );

	//Init Step: 1 - Check Driver for DX9 and DX10 and DX12(=false) on DX11 API
	ASSERT (CheckAPIdriver(/* Use Graph Card 1 */ USE_THIS_GRAPHIC_CARD_ADAPTER));

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
}


// The Shutdown function will release and clean up all the pointers used in the Initialize function

//----------------------------------------------------------------------------------------------
void DX11Class::Shutdown2D()
//----------------------------------------------------------------------------------------------
{
}


//----------------------------------------------------------------------------------------------
void DX11Class::Shutdown()
//----------------------------------------------------------------------------------------------
{


	if (m_device) 
	{

	Shutdown2D();

	//createAllRasterizerStates:
	for (UINT i = 0; i < 3; i++)
		for (UINT j = 0; j < 2; j++)
			SAFE_RELEASE(m_rasterState[i][j]);

	//Release the two new blending states.
		SAFE_RELEASE (m_alphaEnableBlendingState);
		SAFE_RELEASE (m_alphaDisableBlendingState);

		WOMA_LOGManager_DebugMSGAUTO (TEXT("Number of Textures: %d\n"), allTextureNameArray.size());

		for (UINT i=0; i < allTextureNameArray.size(); i++)
			SAFE_RELEASE (allTexturePointerArray[i]);	// Free All Textures from our Texture manager

	//#if defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21 // Color Shader
	//	if(m_Camera) 
	//		{ delete ((DirectX::DXcameraClass*)m_Camera); m_Camera=NULL; }	//SAFE_DELETE (m_Camera);
	//#endif

		// For each Monitor: 
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		for (int i = 0; i < DX11windowsArray.size(); i++)
		{
			if (DX11windowsArray[i].m_swapChain)
				DX11windowsArray[i].m_swapChain->SetFullscreenState(false, NULL);

				SAFE_RELEASE(DX11windowsArray[i].m_backBuffer);				// Release pointer to the back buffer
		}

		// createSetDepthStencilState() - Here we release the new depth stencil during the Shutdown function.
		SAFE_RELEASE (m_depthDisabledStencilState);
		SAFE_RELEASE (m_depthStencilState);

		// For each Monitor: 
		for (int i = 0; i < DX11windowsArray.size(); i++)
			SAFE_RELEASE(DX11windowsArray[i].m_depthStencilView);		// CreateSetDepthStencilView

		SAFE_RELEASE (m_depthBuffer);

		// For each Monitor: 
		for (int i = 0; i < DX11windowsArray.size(); i++)
			SAFE_RELEASE(DX11windowsArray[i].m_renderTargetView);		// CreateRenderTargetView

		SAFE_RELEASE (adapterGraphicCard);
		SAFE_RELEASE (m_deviceContext);

		// For each Monitor: 
		for (int i = 0; i < DX11windowsArray.size(); i++)
			SAFE_RELEASE(DX11windowsArray[i].m_swapChain);

		// The Last one!
		#if _DEBUG
		HRESULT hr = debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		#endif

		ULONG count = m_device->Release();
		m_device = NULL;

		#ifdef _DEBUG
		if (count)
			WOMA_LOGManager_DebugMSG("DEBUG WARNING: There are %d unreleased references left on the D3D device!\n", count);
		#endif

		ASSERT (!count);
		
	}
}


// |Init Step: 1| This is for DIRECTX Driver only!
// ----------------------------------------------------------------------------------------------
BOOL DX11Class::CheckAPIdriver(UINT USE_THIS_ADAPTER_CARD)
// ----------------------------------------------------------------------------------------------
{
	IDXGIFactoryN* pDXGIFactory = NULL;
	IDXGIFactory1* pDXGIFactory1 = NULL;

	/*******************************************************************
	// Checks for the for the proper RunTime D3DX library version: (D3DX_SDK_VERSION)
	/******************************************************************/
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

#if _DEBUG
	HRESULT hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDev));
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
			WomaFatalExceptionW(TEXT("FATAL ERROR: DX9 Device, not supported. Could not load d3d9.dll"));
			return FALSE;
		}
}//#endif

	/*******************************************************************
	// Check for a DX10/11 Instalation (Need to have: dxgi.dll) not present at Window XP
	/******************************************************************/

	if (!LoadLibrary(TEXT("dxgi.dll"))) // NOTE: Windows XP Can't do this (SO WINDOWS XP NOT SUPPORTED!)
	{
		WOMA::WomaMessageBox(TEXT("dxgi.dll"), TEXT("Error, Could not load: ")); 
		return FALSE;
	}

	/******************************************************************/
	// Create a DirectX 10/11 graphics interface factory.
	/******************************************************************/
	IF_FAILED_RETURN_FALSE(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pDXGIFactory1));	// Use: dxgi.dll
	if (!pDXGIFactory1)
	{
		WOMA::WomaMessageBox(TEXT("Could not create DXGIFactory1"), TEXT("DX11Class")); 
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
				WOMA_LOGManager_DebugMSG("DXGI1.0: Available\n");
			}
		}//#endif
	}

	pAdapter->Release();			// Don't forget to release when you are done
	SAFE_RELEASE(pDXGIFactory1);	// Release the factory, Lancher.exe dont use more :)
	/******************************************************************/
	// Check Math Library:
	/******************************************************************/
	if (!XMVerifyCPUSupport())
		MessageBox(NULL, TEXT("WARNING: Failed to verify DirectX Math library support."), TEXT("Error"), MB_OK);

	/******************************************************************/
	// Check DX Driver Multithread
	/******************************************************************/
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

	WOMA_LOGManager_DebugMSG("-------------------------\n");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("Initializing Driver: %s\n"), driverName);
    WOMA_LOGManager_DebugMSG("-------------------------\n");

	//Init Step: 3, 4
	IF_NOT_RETURN_FALSE (createDevice ());
	IF_NOT_RETURN_FALSE (createSwapChainDX11device ((HWND)hwnd, screenWidth, screenHeight, vsync, fullscreen, g_UseDoubleBuffering, g_AllowResize, numerator,  denominator));

	//Init Step: 5 - Get Best Shader of this Graphic Card: dx10,dx10.1,dx11,etc... OUTPUT: ShaderModel
	getProfile (g_USE_MONITOR);					

	//Init Step: 6 Before Resize (SetCamera2D & SetCamera3D)!
		Initialize3DCamera();

	//Init Step: 7 (Include: 8,9,10,11,12)
	// Creates a render target view and depth stencil surface/view per swapchain
	IF_NOT_RETURN_FALSE (Resize (screenWidth, screenHeight, screenNear, screenDepth, fullscreen, depthBits));

	//Init Step: 8 - Cull Back / Front:
	IF_NOT_RETURN_FALSE ( createRasterizerStates (/*lineAntialiasing*/ false)); // Only applies: if doing "line drawing" and "MultisampleEnable" is false.
	SetRasterizerState (CULL_NONE, FILL_SOLID);	//Set Default

	//Init Step: 13 Create and Set the depth stencil state: With the created depth stencil state we can now set it so that it takes effect:
	IF_NOT_RETURN_FALSE ( createSetDepthStencilState (true) );	// Zbuffer on state
	IF_NOT_RETURN_FALSE ( createSetDepthStencilState (false) );	// Zbuffer off state
	//TurnZBufferOn();

	//Init Step: 14 - Transparency: To render text on top of 3D
	IF_NOT_RETURN_FALSE(CreateBlendState());

	return true;
}


//----------------------------------------------------------------------------------------------
bool DX11Class::Resize (int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits)
//----------------------------------------------------------------------------------------------
{
HRESULT result = S_OK;

	RenderfirstTime = true;	 // Used on SPRITES!

	if (m_deviceContext && m_device) 
	{

		m_deviceContext->OMSetRenderTargets(0, NULL, NULL);

		// #DeleteViewBuffers:
		// -------------------

		// For each Monitor: 
		for (int i = 0; i < DX11windowsArray.size(); i++)
		{
			SAFE_RELEASE(DX11windowsArray[i].m_backBuffer);			// Release pointer to the back buffer
			SAFE_RELEASE(DX11windowsArray[i].m_renderTargetView);	// Init Step: 9	(backBufferRTV)
		}

		SAFE_RELEASE(m_depthBuffer);					// Init Step: 9

		for (int i = 0; i < DX11windowsArray.size(); i++)
		{
			SAFE_RELEASE(DX11windowsArray[i].m_depthStencilView);		// Init Step: 10
			
		#if D3D11_SPEC_DATE_YEAR == 2009
			// #Resize: Init Step: 8 - Resize internal Buffers for new Window size:
			result = (DX11windowsArray[i].m_swapChain->ResizeBuffers(1, screenWidth, screenHeight, BUFFER_COLOR_FORMAT, 0));
			if (FAILED(result)) return false;
		#endif
		}

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

		// [3] CreateDepthStencilView / OMSetRenderTargets:
		IF_NOT_RETURN_FALSE (createSetDepthStencilView (screenWidth, screenHeight));

		// This will bind the render target view and the depth stencil buffer to the output render pipeline. 
		// This way the graphics that the pipeline renders will get drawn to our back buffer that we previously created. 
		// With the graphics written to the back buffer we can then swap it to the front and display our graphics on the user's screen. 
		//
		// OMSetRenderTargets: NOTE: Need to be After [1], [2] and [3]
		// #Generate new "ProjectionMatrix" and "OrthoMatrix"
		// --------------------------------------------------
		setProjectionMatrixWorldMatrixOrthoMatrix ( screenWidth, screenHeight, screenNear, screenDepth);
	}

	//SetCamera2D(); //AQUI
	Initialize3DCamera();

	return true;
}

//Init Step: 5 - Set the best shader availabel: MORE INFO: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476876%28v=vs.85%29.aspx
// ----------------------------------------------------------------------------------------------
void DX11Class::getProfile ( UINT g_USE_MONITOR )
// ----------------------------------------------------------------------------------------------
{
	// Get Best FeatureLevel Availabel:
	D3D_FEATURE_LEVEL featureLevel = m_device->GetFeatureLevel();
	switch (featureLevel) 
	{
		//DX 9.x
		case D3D_FEATURE_LEVEL_9_1:	// To Support old Graphic Cards
			_tcscpy_s(ShaderModel, TEXT("4_0_level_9_1")); // ShaderModel = TEXT("2_0"); // 
			ShaderVersionH = 2; ShaderVersionL = 0;
			WOMA_LOGManager_DebugMSGAUTO(TEXT("\nBest Shader Model 2.0 (Best API: DX9.1)\n"));
		break;

		case D3D_FEATURE_LEVEL_9_2:	// To Support old Graphic Cards
			_tcscpy_s(ShaderModel, TEXT("4_0_level_9_2")); //ShaderModel = TEXT("2_0"); // 
            ShaderVersionH = 2; ShaderVersionL = 0;
			WOMA_LOGManager_DebugMSGAUTO(TEXT("\nBest Shader Model 2.0 (Best API: DX9.2)\n"));
		break;

		case D3D_FEATURE_LEVEL_9_3:	// To Support old Graphic Cards
            _tcscpy_s(ShaderModel, TEXT("4_0_level_9_3")); // ShaderModel = TEXT("2_0"); // 
            ShaderVersionH = 2; ShaderVersionL = 0;
			WOMA_LOGManager_DebugMSGAUTO(TEXT("\nBest Shader Model 2.0 (Best API: DX9.3)\n"));
		break;

		//DX 10
		case D3D_FEATURE_LEVEL_10_0:	// To Support old Graphic Cards
            _tcscpy_s(ShaderModel, TEXT("4_0")); // ShaderModel = TEXT("4_0"); // 
            ShaderVersionH = 4; ShaderVersionL = 0;
			WOMA_LOGManager_DebugMSG("\nBest Shader Model 4.0 (Best API: DX10)\n");
		break;

		// DX10.1
		case D3D_FEATURE_LEVEL_10_1:	// To Support old Graphic Cards
            _tcscpy_s(ShaderModel, TEXT("4_1")); // ShaderModel = TEXT("4_1"); // 
            ShaderVersionH = 4; ShaderVersionL = 1;
			WOMA_LOGManager_DebugMSG("\nBest Shader Model 4.1 (Best API: DX10.1)\n");
		break;

		//DX 11
		default: //For Future DX Versions!	
				// case D3D_FEATURE_LEVEL_11_1
				// case D3D_FEATURE_LEVEL_11_2
				// case D3D_FEATURE_LEVEL_12

		case D3D_FEATURE_LEVEL_11_0:
            _tcscpy_s(ShaderModel, TEXT("5_0")); // ShaderModel = TEXT("5_0"); // 
            ShaderVersionH = 5; ShaderVersionL = 0;
			WOMA_LOGManager_DebugMSG("\nBest Shader Model 5.0 (Best API: DX11)\n");
		break;
	}
	WOMA_LOGManager_DebugMSG("\n");

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
		// Get the pointer to the back buffer.
		if ((!DX11windowsArray[i].m_swapChain) || (!m_device))
			return false;

		IF_FAILED_RETURN_FALSE(DX11windowsArray[i].m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&DX11windowsArray[i].m_backBuffer));
		
		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescMSAA(D3D11_RTV_DIMENSION_TEXTURE2DMS);	//MSAA_on
		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D);			//MSAA_off
		IF_FAILED_RETURN_FALSE(m_device->CreateRenderTargetView	(DX11windowsArray[i].m_backBuffer, 
																(ReqMSAAmultiSampleCount > 1) ? &renderTargetViewDescMSAA:&renderTargetViewDesc,
																&DX11windowsArray[i].m_renderTargetView)); //backBufferRTV
	}

	return true;
}


// ----------------------------------------------------------------------------------------------
bool DX11Class::Initialize(float* clearColor)
// ----------------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("DX11Class::Initialize()\n");

	// Set default clear color:
	ClearColor[0] = *clearColor++;
	ClearColor[1] = *clearColor++;
	ClearColor[2] = *clearColor++;
	ClearColor[3] = *clearColor;

	return true;
}

void DX11Class::Finalize() {} //not used on DX11

// ----------------------------------------------------------------------------------------------
void DX11Class::BeginScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	SetBackBufferRenderTarget(monitorWindow);

	// Clear Screen - with "ClearColor" defined: DX11Class::Initialize
	m_deviceContext->ClearRenderTargetView(DX11windowsArray[monitorWindow].m_renderTargetView, ClearColor);					// Clear the "back buffer":
	ClearDepthBuffer();
}

// ----------------------------------------------------------------------------------------------
void DX11Class::EndScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	// <PRINT THE 3D SCENE TO SCREEN> to Swap Chain (wait from VSYNC refresh rate, if it is the case)
	DX11windowsArray[monitorWindow].m_swapChain->Present(m_VSYNC_ENABLED, 0);

	#if defined USE_SHADOW_MAP
	//RESET ShaderResources! to avoid HLSL WARNINGS: Resource being set to OM RenderTarget slot 0 is still bound on input!
	// 
	//WHY 3? Because: 045LightRenderShadow.hlsl use 3 registers:
	//Texture2D shaderTexture : register(t0);			// 21:
	//Texture2D AlfaMapTexture : register(t1);			// 43: AlfaMap
	//Texture2D ShadowMapTextureTexture : register(t2);	// 45: ShadowMap
	//
	ID3D11ShaderResourceView* const pSRV[3] = { NULL };
	g_deviceContext->PSSetShaderResources(0, 3, pSRV);
	#endif
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

	fieldOfView =	(float)(PI / 4.0f) /				// Or... 90deg => fieldOfView = (90 / 2) * 0,0174532925f;
					num_monitors;	// 90: 3(num "Impar" monitors)
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for "3D" rendering.
	//D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);		// 3D PROJECTION
	m_projectionMatrix = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, screenNear, screenDepth);			// 3D PROJECTION

	// And the final thing we will setup in the Initialize function is an orthographic projection matrix. 
	//This matrix is used for rendering 2D elements like user interfaces on the screen
	// (Create an orthographic projection matrix for 2D rendering)

    //D3DXMatrixOrthoLH(&m_orthoMatrix, (float) screenWidth, (float) screenHeight, screenNear, screenDepth);	// 2D PROJECTION
	m_orthoMatrix = XMMatrixOrthographicLH ((float) screenWidth, (float) screenHeight, screenNear, screenDepth);// 2D PROJECTION

}

// ----------------------------------------------------------------------------------------------
void DX11Class::GetOrthoMatrix(XMMATRIX& orthoMatrix)
// ----------------------------------------------------------------------------------------------
{
	orthoMatrix = m_orthoMatrix;
}

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
	DX11m_Camera2D.Render();						// ((OpenGLClass*)m_Driver)->m_Camera->Render(); || ((DX_CLASS*)m_Driver)->m_Camera->Render();
	DX11m_Camera2D.Set2DViewMatrix();				// Get the view from the camera and 2D objects.

	DXsystemHandle->m_Camera->m_viewmatrix2D = DX11m_Camera2D.m_viewMatrix;
}
// TODO: go to Virtual Class?
// ----------------------------------------------------------------------------------------------
void DX11Class::Initialize3DCamera()
// ----------------------------------------------------------------------------------------------
{
	if (DXsystemHandle->m_Camera) {

	SetCamera2D();

	// Normal Camera: ( After: SetCamera2D() )
	DXsystemHandle->m_Camera->SetPosition(	SystemHandle->AppSettings->INIT_CAMX, SystemHandle->AppSettings->INIT_CAMY,
							SystemHandle->AppSettings->INIT_CAMZ);

	DXsystemHandle->m_Camera->SetRotation(	SystemHandle->AppSettings->INIT_ROTX, SystemHandle->AppSettings->INIT_ROTY,
							SystemHandle->AppSettings->INIT_ROTZ);

	DXsystemHandle->m_Camera->Render();
	}

}

// TODO: go to Virtual Class?
XMMATRIX* DX11Class::GetViewMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
	{
		switch (projection)
		{
			default:
			case PROJECTION_PERSPECTIVE:
				if (camera == CAMERA_NORMAL)
					return &DXsystemHandle->m_Camera->m_viewMatrix;		// TODO: Use a global matrix, one per frame
				break;

			case PROJECTION_ORTHOGRAPH:
				 return &DXsystemHandle->m_Camera->m_viewmatrix2D;		// TODO: Use a global matrix, one per frame
				break;
		}
	} 

	ASSERT(FALSE);
	return NULL; //Return ERROR
}

// TODO: go to Virtual Class?
XMMATRIX* DX11Class::GetProjectionMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
	{
		switch (projection)
		{
			default:
			case PROJECTION_PERSPECTIVE:
				return &m_projectionMatrix;	
				break;

			case PROJECTION_ORTHOGRAPH:
				return &m_orthoMatrix;
				break;
		}
	} 

	ASSERT(FALSE);
}


}

