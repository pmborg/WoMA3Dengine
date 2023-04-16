// --------------------------------------------------------------------------------------------
// Filename: DX9Class.cpp
// --------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : woma.no-ip.org
//
//  MAIN PURPOSE: Work as a DirectX 9 Driver.
//
// ----------------------------------------------------------------------------------------------

#include "main.h"
#include "mem_leak.h"
//#if !defined (DX9)
//	#error "WOMA COMPILATION ERROR: DX9 should be ON to compile DX09 Driver, use: lvl_000_xp"
//#endif

//#if defined DX9
//#include <d3d11.h>
#if defined DX9sdk

#include "DX9Class.h"
#include "winsystemclass.h"	// SystemHandle


namespace DirectX {

DX9Class::~DX9Class() {
	Shutdown();
	CLASSDELETE(); 
}

DX9Class::DX9Class() 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567830;
	// SUPER: 
	// Video Card Info:
	// ---------------------------------------------------------------------------
	_tcscpy_s(driverName, TEXT("DX9")); 
	ZeroMemory( &ShaderModel, sizeof(ShaderModel) );			// ShaderModel = TEXT(""); //
	ZeroMemory( &szShaderModel, sizeof(szShaderModel) );		// ShaderModel = TEXT(""); //
	// ------------------
	ZeroMemory( &m_videoCardDescription, sizeof(m_videoCardDescription) );
	m_videoCardMemory = NULL;
	ZeroMemory( &adapterDesc_Description, sizeof(adapterDesc_Description) );
	ufreededicatedVideoMem = NULL;

	// List of resoltions availabel to Use
	// ---------------------------------------------------------------------------
	numerator = denominator = 1;
	//MonitorIndex = 0;	
	numModes = NULL;

	// MSAA Used:
	// ----------------------------------------------------------------------------
	//mEnable4xMsaa = true;					// Note: g_MSAA_ENABLED Might not be populated
	//g_MSAA_X = NULL;						// Req: With this X Samples
	// ------------------
	//g_MSAA_SUPPORT = NULL;					// Available?
	//multiSampleCount = m4xMsaaQuality = 1;	// Available like this!	


	// DX9Class()
	// Public: ------------------------------------------------------------------------
    m_D3D9 = NULL;
    //m_D3D9Ex = NULL;

	m_device = NULL;

	// Private: ------------------------------------------------------------------------
	bDeviceLost = false;
	ShaderVersionH = ShaderVersionL = 0;

		m_Camera = NULL;
		m_CameraSKY = NULL;

	#if defined USE_DRIVER_FONTS
		g_pFont = NULL;
	#endif
}

void DX9Class::Finalize(void){} //not used on DX9


//----------------------------------------------------------------------------------------------
bool DX9Class::OnInit(int g_USE_MONITOR, /*HWND*/void* hwnd, int screenWidth, int screenHeight,
							  UINT depthBits, float screenDepth, float screenNear, BOOL msaa, bool vsync, 
							  BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize) 
//----------------------------------------------------------------------------------------------
{
	//mEnable4xMsaa = msaa;
	m_VSYNC_ENABLED = vsync;

	WOMA::logManager->DEBUG_MSG(TEXT("-------------------------\n"));
	WOMA::logManager->DEBUG_MSG(TEXT("Initializing Driver: %s\n"), driverName);
	WOMA::logManager->DEBUG_MSG(TEXT("-------------------------\n"));

	//Init Step: 0 - Check Driver:
	CheckAPIdriver(USE_THIS_GRAPHIC_CARD_ADAPTER);

	//Init Step: 1 - Create Factory:
	IF_NOT_RETURN_FALSE (getModesList( g_USE_MONITOR, screenWidth, screenHeight, fullscreen, &numerator, &denominator));

	//Init Step: 3 Note: BEFORE device (DX9)
	SelectDepthFormat(depthBits, fullscreen);

	//Init Step: 2 - Create DX11 Device:
	IF_NOT_RETURN_FALSE (createDevice (g_USE_MONITOR, vsync, fullscreen, g_AllowResize, screenWidth, screenHeight));		//Init Step: 2

	//Init Step: 5 - Get Best Shader of this Graphic Card: dx10,dx10.1,dx11,etc... OUTPUT: ShaderModel
	getProfile(g_USE_MONITOR);

	//Init Step: 6 Before Resize!
		Initialize3DCamera();

	//Init Step: 7 (Include: 8,9,10,11,12)
	//Resize (screenWidth, screenHeight, screenNear, screenDepth);
	IF_NOT_RETURN_FALSE(Resize(screenWidth, screenHeight, screenNear, screenDepth, fullscreen, depthBits));

	//Init Step: 8 - Cull Back / Front:
	//IF_NOT_RETURN_FALSE(createSetRasterizerStates(/*lineAntialiasing*/ false)); // Only applies: if doing "line drawing" and "MultisampleEnable" is false.
	//SetRasterizerState(CULL_NONE, FILL_SOLID);	//Set Default

	frustum = NEW DXfrustumClass;	// Create Frustum

#if defined INTRO_DEMO || defined USE_BLENDING // 26
	//Init Step: 13 Create and Set the depth stencil state: With the created depth stencil state we can now set it so that it takes effect:
	IF_NOT_RETURN_FALSE(createSetDepthStencilState(true));	// Zbuffer on state
	IF_NOT_RETURN_FALSE(createSetDepthStencilState(false));	// Zbuffer off state
	// TurnZBufferOn();

	//Init Step: 14 - Transparency: To render text on top of 3D
	IF_NOT_RETURN_FALSE(CreateBlendState());
#endif//

	return true;
}

// ----------------------------------------------------------------------------------------------
//bool DX9Class::Resize(int screenWidth, int screenHeight, float screenNear, float screenDepth)
bool DX9Class::Resize(int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits)
// ----------------------------------------------------------------------------------------------
{
	if (m_device)
	{
#if defined USE_DRIVER_FONTS //defined INTRO_DEMO || ENGINE_LEVEL >= 29	// FONT v2
		InitD2DScreenTexture();
#endif

		// SAMPLE!? http://pastebin.com/VvmUZNXF

		// TODO: DX9
		// invalidateDeviceObjects();
		if (!RenderfirstTime)
		{
			d3dpp.BackBufferHeight = screenHeight;
			d3dpp.BackBufferWidth = screenWidth;

			HRESULT hr = m_device->Reset(&d3dpp);
			if (hr != D3D_OK)
			{
				WOMA::logManager->DEBUG_MSG(TEXT("Error: %s error description: %s\n"), DXGetErrorString(hr), DXGetErrorDescription(hr));
				WOMA::WomaMessageBox(TEXT("Call to Reset() failed!"));
				return false;
			}
		}
		// restoreDeviceObjects();

		// DX11 CODE WAY:

		////m_deviceContext->OMSetRenderTargets(0, NULL, NULL);

		////SAFE_RELEASE (m_backBuffer);					// Release pointer to the back buffer
		////SAFE_RELEASE (m_backBufferRenderTargetView);	// Init Step: 9	(backBufferRTV)
		////SAFE_RELEASE (m_depthBuffer);					// Init Step: 9
		////SAFE_RELEASE (m_depthBufferDepthStencilView);	// Init Step: 10

		//Init Step: 8 - Resize internal Buffers for new Window size:
		////result = (m_swapChain->ResizeBuffers(1, screenWidth, screenHeight, BUFFER_COLOR_FORMAT, 0));
		////if( FAILED( result ) ) return false;

		//Init Step: 9 - CreateTexture2D:
		////IF_NOT_RETURN_FALSE (createRenderingTargetView (screenWidth, screenHeight));

		//Init Step: 10 - - CreateDepthStencilView / OMSetRenderTargets:
		////IF_NOT_RETURN_FALSE (createSetDepthStencilView ());

		//Init Step: 11 - RSSetViewports: Map Screen clip space coordinates to the render target space:
		setViewportDevice(screenWidth, screenHeight);

		//Init Step: 12 - Set ProjectionMatrix (CH06) and OrthoMatrix (CH07)
		setProjectionMatrixWorldMatrixOrthoMatrix(screenWidth, screenHeight, screenNear, screenDepth);
	}

	RenderfirstTime = true;	 // Used to render Once the "Mini-Map" and Sprites

	//#if defined INTRO_DEMO || defined CLIENT_SCENE_TEXT // 26
	//SetCamera2D();
	//#endif

	return true;
}

// ----------------------------------------------------------------------------------------------
bool DX9Class::createSetDepthStencilState(bool depthTestEnabled)
// ----------------------------------------------------------------------------------------------
{
	//USE:
	//TurnZBufferOn()
	//TurnZBufferOff()

	return true;
}
/*
// ----------------------------------------------------------------------------------------------
bool DX9Class::createSetRasterizerStates(bool lineAntialiasing)
// ----------------------------------------------------------------------------------------------
{
	//USE:
	//SetRasterizerState(UINT cullMode, UINT fillMode)

	return true;
}
*/
#if defined INTRO_DEMO || defined USE_BLENDING // 26
// ----------------------------------------------------------------------------------------------
bool DX9Class::CreateBlendState()
// ----------------------------------------------------------------------------------------------
{
	//USE:
	//DX9Class::TurnOnAlphaBlending()
	//DX9Class::TurnOffAlphaBlending()

	return true;
}
#endif

// ----------------------------------------------------------------------------------------------
bool DX9Class::Initialize(float* clearColor)
// ----------------------------------------------------------------------------------------------
{
	WOMA::logManager->DEBUG_MSG ("DX11Class::Initialize()\n");

	// Set default clear color:
	ClearColor[0] = *clearColor++;
	ClearColor[1] = *clearColor++;
	ClearColor[2] = *clearColor++;
	ClearColor[3] = *clearColor;

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_device->SetRenderState( D3DRS_ZENABLE, TRUE );

	return true;
}

//Init Step: 5 - Set the best shader availabel
// ----------------------------------------------------------------------------------------------
void DX9Class::getProfile( UINT g_USE_MONITOR )
// ----------------------------------------------------------------------------------------------
{
    // Check for Hardware Vertex Processing. or Software vp.
	D3DCAPS9 caps;
    ThrowIfFailed( m_D3D9->GetDeviceCaps(g_USE_MONITOR, D3DDEVTYPE_HAL, &caps) );

	// TODO: make it more clever...
	ShaderVersionH = min ((caps.VertexShaderVersion & 0x0000FFFF) >>8, (caps.PixelShaderVersion & 0x0000FFFF) >>8);
	ShaderVersionL = min ((caps.VertexShaderVersion & 0x000000FF), (caps.PixelShaderVersion & 0x000000FF));

	_tcscpy_s(ShaderModel, TEXT("X_X"));	// Used later for shaders:
	ShaderModel[0] = '0'+ShaderVersionH;
	ShaderModel[2] = '0'+ShaderVersionL;

	_tcscpy_s(szShaderModel, ShaderModel);	// Human readable:
	szShaderModel[1] = '.';

	WOMA::logManager->DEBUG_MSG (TEXT("DX9 VS & PS are at least: %s\n"), szShaderModel);



	/*
	_tcscpy_s(ShaderModel, TEXT("5_0")); // ShaderModel = TEXT("5_0"); // 
	ShaderVersionH = 5; ShaderVersionL = 0;
	WOMA_LOGManager_DebugMSG("\nBest Shader Model 5.0 (Best API: DX9)\n");
	_tcscpy_s(szShaderModel, ShaderModel); szShaderModel[1] = '.';	// Convert: "x_y" to" x.y
	*/



}

// ----------------------------------------------------------------------------------------------
void DX9Class::Initialize3DCamera()
// ----------------------------------------------------------------------------------------------
{
	if (!m_Camera) {
		m_Camera = NEW DirectX::DXcameraClass; // DX Implementation
		IF_NOT_THROW_EXCEPTION (m_Camera);
	}

#if defined USE_VIEW2D // 26
	SetCamera2D();
#endif

	// Normal Camera:
	m_Camera->SetPosition(	SystemHandle->AppSettings->INIT_CAMX, SystemHandle->AppSettings->INIT_CAMY, 
							SystemHandle->AppSettings->INIT_CAMZ);

	m_Camera->SetRotation(	SystemHandle->AppSettings->INIT_ROTX, SystemHandle->AppSettings->INIT_ROTY, 
							SystemHandle->AppSettings->INIT_ROTZ);

	m_Camera->Render();

	if (!m_CameraSKY) {
		m_CameraSKY = NEW DXcameraClass; // DX Implementation
		IF_NOT_THROW_EXCEPTION (m_CameraSKY);
	}

	m_CameraSKY->SetPosition(0.0f, 0.0f, 0.0f);
	m_CameraSKY->SetRotation(	SystemHandle->AppSettings->INIT_ROTX, SystemHandle->AppSettings->INIT_ROTY, 
								SystemHandle->AppSettings->INIT_ROTZ);

	m_CameraSKY->Render();
}


// ----------------------------------------------------------------------------------------------
void DX9Class::SetRasterizerState(UINT cullMode, UINT fillMode) 
// ----------------------------------------------------------------------------------------------
{
	if (cullMode == CULL_FRONT)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//NEED TO CONFIRM!!

	if (cullMode == CULL_BACK)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	//NEED TO CONFIRM!!

	if (cullMode == CULL_NONE)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//NEED TO CONFIRM!!

	m_device->SetRenderState(D3DRS_FILLMODE, (fillMode == FILL_WIRE) ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
}


// Set CAMERA for "2D" (Once per Re-Size only)
//-----------------------------------------------------------------------------------------
void DX9Class::SetCamera2D()
{
	// Set Fixed Camera for 2D:
	m_Camera->SetRotation(0, 0, 0);			// NOTE: On 2D This values have always these values!
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);	// NOTE: On 2D This values have always these values!

												// Calculate: 2D ViewMatrix
	m_Camera->Render();				// ((OpenGLClass*)m_Driver)->m_Camera->Render(); || ((DX_CLASS*)m_Driver)->m_Camera->Render();
	m_Camera->Set2DViewMatrix();	// SAVE the 2D Calculated View Matrix: (m_viewmatrix2D = m_viewMatrix;)
}

//bool DX9Class::getModesList(int screenWidth, int screenHeight, BOOL fullscreen, UINT* numerator, UINT* denominator) {return true;}



//Init Step: 2
// ----------------------------------------------------------------------------------------------
bool DX9Class::getModesList(int g_USE_MONITOR, int screenWidth, int screenHeight, BOOL fullscreen, UINT* numerator, UINT* denominator)
// ----------------------------------------------------------------------------------------------
{
/*
	// Adap RAM ?

	// Get N. of monitors? MonitorIndex

	UINT AdapterIndex = g_USE_MONITOR; //D3DADAPTER_DEFAULT;
	UINT ModeCount = m_D3D9->GetAdapterModeCount(AdapterIndex, D3DFMT_X8R8G8B8); // Get N. of modes for this monitor

	for (UINT idx = 0; idx < ModeCount; idx++)
	{
		D3DDISPLAYMODE DisplayMode;
		HRESULT d3dResult = m_D3D9->EnumAdapterModes(AdapterIndex, D3DFMT_X8R8G8B8, idx, &DisplayMode);
		if (d3dResult == D3D_OK)
		{
			resolutionType resolution;
			resolution.Width = (INT)DisplayMode.Width;
			resolution.Height = (INT)DisplayMode.Height;

			resolution.RefreshRate_Numerator = (INT)DisplayMode.RefreshRate;
			resolution.RefreshRate_Denominator = 1;

			ScreenResolution.push_back(resolution);
		}
	}

	// DEBUG: Write all Best "RefreshRate" Resolutions for this Monitor on "report.txt":
	for (UINT res = 0; res < ScreenResolution.size(); res++)
	{
		WOMA::logManager->DEBUG_MSG(TEXT("%d x %d "), ScreenResolution[res].Width, ScreenResolution[res].Height);
		WOMA::logManager->DEBUG_MSG(TEXT("(%4.2f Hz)\n"), (float) ScreenResolution[res].RefreshRate_Numerator / (float) ScreenResolution[res].RefreshRate_Denominator);
	}
*/
	WOMA::logManager->DEBUG_MSG(TEXT("\n"));

	return true;
}

// ----------------------------------------------------------------------------------------------
BOOL DX9Class::CheckAPIdriver (UINT USE_THIS_ADAPTER)
// ----------------------------------------------------------------------------------------------
{
	/*******************************************************************
    // Checks for the for the proper RunTime D3DX library version: (D3DX_SDK_VERSION)
    /******************************************************************/
	TCHAR name[MAX_STR_LEN];
	StringCchPrintf(name, MAX_STR_LEN, TEXT("D3DX9_%d.dll"), D3DX_SDK_VERSION);  //L"D3DX9_43.dll"

    HINSTANCE hinstLib = LoadLibrary(name);
    if (!hinstLib)
    {
		TCHAR error[MAX_STR_LEN];
		StringCchPrintf(error, MAX_STR_LEN, TEXT("Fatal Error: Wrong D3DX_SDK_VERSION, Could not load: %d"), D3DX_SDK_VERSION);
		WOMA::WomaMessageBox(error);
        return FALSE;
    }

    FreeLibrary(hinstLib);

	/*******************************************************************
    // Check for DX9, Load DX 9 DLL if is installed...
	/******************************************************************/
    if (LoadLibraryA("d3d9.dll"))
		*CapDX9 = TRUE;

	// OLDWAY:
#ifdef OLDWAY
    m_D3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_D3D9 == NULL)
        return FALSE;
#else
	// This is for DX: DX 9.0
    LPDIRECT3DCREATE9   s_DynamicDirect3DCreate9 = NULL;
    HMODULE s_hModD3D9 = LoadLibrary(TEXT("d3d9.dll"));
    if (s_hModD3D9 != NULL)
        s_DynamicDirect3DCreate9 = (LPDIRECT3DCREATE9) GetProcAddress(s_hModD3D9, "Direct3DCreate9");

    m_D3D9 = s_DynamicDirect3DCreate9(D3D_SDK_VERSION);
#endif

	ASSERT (m_D3D9);

	// This is for DX: 9.3

    /*
    LPDIRECT3DCREATE9EX   s_DynamicDirect3DCreate9Ex = NULL;
    //s_hModD3D9 = LoadLibrary(L"d3d9.dll");
    if (s_hModD3D9 != NULL)
        s_DynamicDirect3DCreate9Ex = (LPDIRECT3DCREATE9EX) GetProcAddress(s_hModD3D9, "Direct3DCreate9Ex");

    IDirect3D9Ex* pD3DEx = s_DynamicDirect3DCreate9Ex(D3D_SDK_VERSION);
    */
/*
    try {
        Direct3DCreate9Ex(D3D_SDK_VERSION, &m_D3D9Ex);
        if (!m_D3D9Ex) return false;
    }
    catch (...)	 
    {
    }
    */

	return TRUE;
}

#define checkDepthStencilFormat() \
{ \
	return !D3DInstance_->CheckDeviceFormat( \
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Presenter_.BackBufferFormat, \
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, Presenter_.AutoDepthStencilFormat \
		); \
}

// Select the Depth Buffer Format (16, 24, 32, 64, or default...)
//----------------------------------------------------------------------------------------------
void DX9Class::SelectDepthFormat(UINT depthBits, BOOL fullscreen)
//----------------------------------------------------------------------------------------------
{
	// REF: https://msdn.microsoft.com/en-us/library/windows/desktop/cc308051%28v=vs.85%29.aspx

	BUFFER_DEPTH_FORMAT = D3DFMT_UNKNOWN;

	switch (depthBits)
	{
	case 16://A buffer that contains 16-bits of depth data:
		BUFFER_DEPTH_FORMAT = D3DFMT_D16;	// DXGI_FORMAT_D16_UNORM;			// Fastest!
		break;

	case 24:// A 32 bit buffer that contains 24 bits of depth data and 8 bits of stencil data:
		BUFFER_DEPTH_FORMAT = D3DFMT_D24X8; // DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;

	default:// Default like a "zero" value use the Best!
	case 32://A buffer that contains 32-bits of depth data:
		BUFFER_DEPTH_FORMAT = D3DFMT_D32; // DXGI_FORMAT_D32_FLOAT;			// High-precision but slower
		break;
	}

}


// ----------------------------------------------------------------------------------------------
bool DX9Class::createDevice(UINT g_USE_MONITOR, BOOL vsync, BOOL fullscreen, BOOL g_AllowResize, int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	HRESULT result;

	// --- CREATE DEVICE 
	//Get the current display mode
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_D3D9->GetAdapterDisplayMode(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, &d3ddm)))
	{
		WOMA::WomaMessageBox(TEXT("GetAdapterDisplayMode NOK"));
		return FALSE;
	}

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//Fill the structure. 
	//We want our program to be windowed, and set the back buffer to a format
	//that matches our current display mode
	d3dpp.BackBufferCount = 1;
	d3dpp.hDeviceWindow = SystemHandle->m_hWnd;
	d3dpp.Windowed = !fullscreen;
	d3dpp.BackBufferFormat = d3ddm.Format; // DEFUALT: D3DFMT_X8R8G8B8;// Set 32 Bits

	//TODO:
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	if (fullscreen) {
		// FULL SCREEN 
		// ------------------------------
		if (g_AllowResize)
		{
			// Force Change Resolution
			d3dpp.BackBufferWidth = screenWidth;
			d3dpp.BackBufferHeight = screenHeight;
		}
		else {
			d3dpp.BackBufferWidth = d3ddm.Width;
			d3dpp.BackBufferHeight = d3ddm.Height;
		}

		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	}
	else {
		// WINDOWED SCREEN 
		// ------------------------------
		d3dpp.BackBufferWidth = screenWidth;
		d3dpp.BackBufferHeight = screenHeight;

		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	}

	// Wait for V SYNC:
	// ------------------------------
	if (vsync) {
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // What we want!
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else {
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// ------------------------------
	// Automatically run the z-buffer: Check-feacture...
	// -------------------------------
	//#ifdef ENABLE_FACE_CULLING
	//m_pD3D.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

	
	// 
	if (m_D3D9->CheckDeviceFormat(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, D3DDEVTYPE_HAL, d3ddm.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, BUFFER_DEPTH_FORMAT) == D3D_OK)
	{
		d3dpp.AutoDepthStencilFormat = BUFFER_DEPTH_FORMAT;
		d3dpp.EnableAutoDepthStencil = TRUE;

		//WriteToLog("\t32bit depth buffer selected");
	}

	//Select the best depth buffer, select 32, 24 or 16 bit
	else if (m_D3D9->CheckDeviceFormat(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, D3DDEVTYPE_HAL, d3ddm.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32) == D3D_OK)
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
		d3dpp.EnableAutoDepthStencil = TRUE;

		//WriteToLog("\t32bit depth buffer selected");
	}
	else if (m_D3D9->CheckDeviceFormat(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, D3DDEVTYPE_HAL, d3ddm.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8) == D3D_OK)
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
		d3dpp.EnableAutoDepthStencil = TRUE;

		//WriteToLog("\t24bit depth buffer selected");
	}
	else if (m_D3D9->CheckDeviceFormat(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, D3DDEVTYPE_HAL, d3ddm.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16) == D3D_OK)
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil = TRUE;

		//WriteToLog("\t16bit depth buffer selected");
	}
	else
	{
		d3dpp.EnableAutoDepthStencil = FALSE;
		WOMA::logManager->DEBUG_MSG(TEXT("\tUnable to select depth buffer."));
	}
	//#endif

	// Need to confirm this:
	
	if (SystemHandle->AppSettings->MSAA_ENABLED)
		d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow			= SystemHandle->m_hWnd;
	d3dpp.PresentationInterval	= (vsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;


	// --- CREATE DEVICE 
	/*
	if (m_D3D9Ex) {
		result = m_D3D9Ex->CreateDeviceEx(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hwnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				NULL,
				&m_DX9deviceEx);

		if (FAILED(result)) return false;
	}
	*/

	// Step 3: Check for Hardware Vertex Processing. or Software vp.
	D3DCAPS9 caps;
	//m_device->GetDeviceCaps( &Caps );

	if (FAILED(m_D3D9->GetDeviceCaps(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, D3DDEVTYPE_HAL, &caps)))
	{
		WOMA::WomaMessageBox(TEXT("Directx Device: GetDeviceCaps")); return FALSE;
	}

	int hardwareVertexProcessing = (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_MIXED_VERTEXPROCESSING;

	if (m_D3D9)
	{
		// Try HW Driver first:
		result = m_D3D9->CreateDevice(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, D3DDEVTYPE_HAL, SystemHandle->m_hWnd,
			hardwareVertexProcessing, &d3dpp, &m_device);
		if (FAILED(result))
		{
			// Try SW Driver as last attempt:
			result = m_D3D9->CreateDevice(g_USE_MONITOR /*D3DADAPTER_DEFAULT*/, D3DDEVTYPE_HAL, SystemHandle->m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_device);
			if (FAILED(result))
				return false;

			WOMA::logManager->DEBUG_MSG(L"HARDWARE VERTEXPROCESSING: Not Supported\n");
		}
		else
		{
			WOMA::logManager->DEBUG_MSG(L"HARDWARE VERTEXPROCESSING: Supported\n");
		}
	}

	m_device->Reset(&d3dpp);

	// If device doesn't support 3.0 vertex shaders in HW, abort
	// Note: Normally DX9 is until VS 3.0
	if (caps.VertexShaderVersion < D3DVS_VERSION(3, 0))
	{
		{WOMA::WomaMessageBox(TEXT("This Graphics Card dont Support the basic Vertix Shader")); return false; }
	}
	//WOMA::logManager->DEBUG_MSG (L"DX9 is at least VS 3.0\n");

	if (caps.PixelShaderVersion < D3DPS_VERSION(3, 0))
	{
		WOMA::WomaMessageBox(TEXT("This Graphics Card dont Support the basic Vertix Shader")); return false;
	}


	// -------------------------------------------------------------------------
	// Get the best Multi Sample Quality (multiSampleCount & m4xMsaaQuality)
	// -------------------------------------------------------------------------
	DWORD QualityLevels = 0;

	HRESULT Result = m_D3D9->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.BackBufferFormat,
		d3dpp.Windowed, d3dpp.MultiSampleType, &QualityLevels);

	bool g_MSAA_SUPPORT=false;
	if (Result == S_OK)
	{
		g_MSAA_SUPPORT = true;
		WOMA::logManager->DEBUG_MSG(TEXT("DRIVER multiSampleQuality: %d\n"), QualityLevels - 1);	// Get the max MsaaQuality: 32
	} else {
		g_MSAA_SUPPORT = false;
		WOMA::WomaMessageBox(TEXT("WARNING: This card don't support, Multisample anti-aliasing")); // NOTE: Don't make it fatal (just reset setting)
	}

	// Log It!
	if (g_MSAA_SUPPORT)
		WOMA::logManager->DEBUG_MSG(TEXT("MSSA setting: Enabled\n"));
	else
		WOMA::logManager->DEBUG_MSG(TEXT("MSSA setting: Disabled\n"));

	// Make sence? After create device?


	return true;
}

/*
// ----------------------------------------------------------------------------------------------
bool DX9Class::InitializeFontDemo(int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL msaa, BOOL vsync, BOOL fullscreen)
// ----------------------------------------------------------------------------------------------
{
	HRESULT result;
	IF_NOT_RETURN_FALSE (createDevice (vsync, fullscreen, true, screenWidth, screenHeight));

    result = D3DXCreateFont(m_device, -15, 0, 0, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_D3D9Font);
	if (FAILED(result)) return false;

	return true;
}
*/

// ----------------------------------------------------------------------------------------------
void DX9Class::Shutdown2D()
// ----------------------------------------------------------------------------------------------
{
#if defined USE_DRIVER_FONTS
	SAFE_RELEASE(g_pFont);
#endif
}

// The Shutdown function will release and clean up all the pointers used in the Initialize function
// ----------------------------------------------------------------------------------------------
void DX9Class::Shutdown()
// ----------------------------------------------------------------------------------------------
{
		if(m_CameraSKY) { 
			delete ((DirectX::DXcameraClass*)m_CameraSKY); 
			m_CameraSKY=NULL; 
		}

		if(m_Camera) { 
			delete ((DirectX::DXcameraClass*)m_Camera); 
			m_Camera=NULL; 
		}	//SAFE_DELETE (m_Camera);

	SAFE_RELEASE(m_device);

	SAFE_RELEASE(m_D3D9);
}

// ----------------------------------------------------------------------------------------------
void DX9Class::setProjectionMatrixWorldMatrixOrthoMatrix (int screenWidth, int screenHeight, float screenNear, float screenDepth)
// ----------------------------------------------------------------------------------------------
{
	float fieldOfView, screenAspect;

	ASSERT (screenWidth > 0);
	ASSERT (screenHeight > 0);
	ASSERT (screenNear > 0);
	ASSERT (screenDepth > 0);

	// Create the projection matrix:
	fieldOfView = (float)PI / 4.0f; // Or... fieldOfView = (90 / 2) * 0,0174532925f; (which is equal) for 45 degrees...
	screenAspect = (float)screenWidth / (float)screenHeight;

	D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, fieldOfView, screenAspect, screenNear, screenDepth );
    m_device->SetTransform( D3DTS_PROJECTION, &matProj );
}


#define CleanColor D3DCOLOR_XRGB((int)(ClearColor[0]*255), (int)(ClearColor[1]*255), (int)(ClearColor[2]*255))

// ----------------------------------------------------------------------------------------------
void DX9Class::BeginScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	//m_device === CONVERTED ===> m_pd3dDevice

	//Clear the backbuffer
	m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CleanColor, 1.0f, 0);

	ClearDepthBuffer();

	// Begin the scene:
	m_device->BeginScene();
	//----------------------------------------------------------------------------

}

// ---------------------------------------------------------
// ----------------------------------------------------------------------------------------------
void DX9Class::GetProjectionMatrix(XMMATRIX& projectionMatrix)
// ----------------------------------------------------------------------------------------------
{
	projectionMatrix = m_projectionMatrix;
}


/*
void DX9Class::setRasterStateCullFront(UINT fillMode){}
void DX9Class::setRasterStateCullBack(UINT fillMode){}
void DX9Class::setRasterStateCullNone(UINT fillMode){}
*/

void DX9Class::setViewportDevice(int screenWidth, int screenHeight)
{
	viewport.X = 0.0f;	// MinX
	viewport.Y = 0.0f;	// MinY
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;

	viewport.MinZ = 0.0f;	// MinZ
	viewport.MaxZ = 1.0f;	// MaxZ

	// Create the viewport:
	ASSERT(viewport.Width > 0 && viewport.Height > 0);
	ThrowIfFailed(m_device->SetViewport(&viewport));
}

//static bool g_Zbuffer = false;
// ----------------------------------------------------------------------------------------------
void DX9Class::TurnZBufferOn()
// ----------------------------------------------------------------------------------------------
{
    if (g_Zbuffer) return;
    //m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
    g_Zbuffer = true;
}

// ----------------------------------------------------------------------------------------------
void DX9Class::TurnZBufferOff()
// ----------------------------------------------------------------------------------------------
{
    if (!g_Zbuffer) return;
    //m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
    g_Zbuffer = false;
}


// ----------------------------------------------------------------------------------------------
HRESULT DX9Class::RenderD3D9()
// ----------------------------------------------------------------------------------------------
{
    HRESULT hr = E_FAIL;

    RECT rect = {0, 0, 200, 15};
    
    if (FAILED(hr = m_device->BeginScene()))
        return hr;

    float FPS = 0; //UpdateFPS();

    TCHAR fpsBuffer[1024];
	StringCchPrintf(fpsBuffer, 1024, TEXT("FPS: %f"), FPS);
    m_D3D9Font->DrawText(NULL, fpsBuffer, -1, &rect, 0, D3DCOLOR_XRGB(255, 255, 255));
   
    rect.top = 15; 
    rect.bottom = 30;
	StringCchPrintf(fpsBuffer, 1024, TEXT("Render time: %f"), 0);
    m_D3D9Font->DrawText(NULL, fpsBuffer, -1, &rect, 0, D3DCOLOR_XRGB(255, 255, 255));

	hr = m_device->EndScene();

    return hr;
}

// ----------------------------------------------------------------------------------------------
void DX9Class::ClearDepthBuffer()
// ----------------------------------------------------------------------------------------------
{
	//d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//m_deviceContext->ClearDepthStencilView(m_depthBufferDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);	// Clear the "depth buffer":
}

// ----------------------------------------------------------------------------------------------
void DX9Class::EndScene(UINT monitorWindow)
// ----------------------------------------------------------------------------------------------
{
	// <PRINT THE 3D SCENE TO SCREEN> to Swap Chain (wait from VSYNC refresh rate, if it is the case)
	//m_swapChain->Present(m_VSYNC_ENABLED, 0);

	//-------------------------------------------------------------------------------
	//End the scene:
	m_device->EndScene();

	//Filp the back and front buffers so that whatever has been rendered on the back buffer
	//will now be visible on screen (front buffer).
	if( D3DERR_DEVICELOST == m_device->Present(NULL, NULL, NULL, NULL) )
		bDeviceLost = true; // Full Screen lost focus.
}



// These next functions simply get pointers to the Direct3D device and the Direct3D device context.
/*IDirect3DDevice9Ex* DX9Class::GetDevice()
{
	return m_device;
}*/


#if defined INTRO_DEMO || defined USE_BLENDING // OLD:ENGINE_LEVEL >= 26
void  DX9Class::TurnOnAlphaBlending() {}
void  DX9Class::TurnOffAlphaBlending() {}
#endif

#if defined USE_DRIVER_FONTS //defined INTRO_DEMO || ENGINE_LEVEL >= 29	// FONT v2

void DX9Class::InitD2DScreenTexture()
{
	HRESULT hr;
	TCHAR g_strFont[LF_FACESIZE];
	HDC hDC = GetDC(NULL);
	int nLogPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
	ReleaseDC(NULL, hDC);

	wcscpy_s(g_strFont, 32, L"Arial");
	#define g_nFontSize 8


	int nHeight = -g_nFontSize * nLogPixelsY / 72;
	hr = D3DXCreateFont(m_device,	// D3D device
		nHeight,					// Height
		0,							// Width
		FW_BOLD,					// Weight
		1,							// MipLevels, 0 = autogen mipmaps
		FALSE,						// Italic
		DEFAULT_CHARSET,			// CharSet
		OUT_DEFAULT_PRECIS,			// OutputPrecision
		DEFAULT_QUALITY,			// Quality
		DEFAULT_PITCH | FF_DONTCARE,// PitchAndFamily
		g_strFont,					// pFaceName
		&g_pFont);					// ppFont

	ThrowIfFailed(hr);
}

//
// Source: Based on SDK D:\DirectXSamples\Text3D
//
void DX9Class::RenderDriverText(int Xpos, int Ypos, WCHAR* text, int Width, int Height, float R, float G, float B)
{
	RECT rc;

	// If you wanted to calc the bottom/rect of the rect make these 2 calls

	// Demonstration 1:
	// Draw a simple line using ID3DXFont::DrawText
	//SetRect(&rc, 150, 200, 0, 0);
	SetRect(&rc, Xpos, Ypos, Width, Height);

	g_pFont->DrawText(NULL, text, -1, &rc, DT_CALCRECT, D3DXCOLOR(R, G, B, 1.0f));
	g_pFont->DrawText(NULL, text, -1, &rc, 0, D3DXCOLOR(R, G, B, 1.0f));
}
#endif

}
#endif
//#endif