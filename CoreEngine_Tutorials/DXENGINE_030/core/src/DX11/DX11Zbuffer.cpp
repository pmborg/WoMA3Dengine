// --------------------------------------------------------------------------------------------
// Filename: DX11Zbuffer.cpp
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
#include <d3d11.h>

#if (defined OPENGL3 || defined OPENGL40) 
	#include "GLmathClass.h"
	#include "GLopenGLclass.h"
#endif

#if defined DX11 || defined DX9
#include "womadriverclass.h"

#include "dx11Class.h"

// SOURCE: http://www.gamedev.net/topic/650974-dx11-disabling-depth-testing/
// SOURCE: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476110%28v=vs.85%29.aspx

namespace DirectX {

// ----------------------------------------------------------------------------------------------
// DepthStencilState:
// ----------------------------------------------------------------------------------------------
bool DX11Class::createSetDepthStencilState (bool depthTestEnabled)
// ----------------------------------------------------------------------------------------------
{
	//USE:
	//TurnZBufferOn()
	//TurnZBufferOff()

	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {0}; // ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = depthTestEnabled;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;			

	depthStencilDesc.StencilEnable = false;

	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	if (depthTestEnabled)
	{
		// Create the state using the device.
		result = m_device11->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);		// true
		if(FAILED(result)){WomaFatalException(("Error: CreateDepthStencilState")); /*return false;*/}

	} else {
		// With the description filled out we can now create a depth stencil state:
		// (Create the depth stencil state)
		result = m_device11->CreateDepthStencilState(&depthStencilDesc, &m_depthDisabledStencilState);// false
		if(FAILED(result)){WomaFatalException(("Error: CreateDepthStencilState")); /*return false;*/}
	}

    if (!testStencilState)
    {
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        result = m_device11->CreateDepthStencilState(&depthStencilDesc, &testStencilState);// false
    }

	return true;
}

// -----------------------------------------------------------------
//These are the new functions for enabling and disabling the Z buffer. To turn Z buffering on we set the original depth stencil. 
//To turn Z buffering off we set the new depth stencil that has depthEnable set to false. 
//Generally the best way to use these functions is first do all your 3D rendering, then turn the Z buffer off and do your 
//2D rendering, and then turn the Z buffer on again.

//static bool g_Zbuffer = false;
// ----------------------------------------------------------------------------------------------
void DirectX::DX11Class::TurnZBufferOn(void* ctx)
// ----------------------------------------------------------------------------------------------
{
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;
    if (g_Zbuffer) return;
	pContext->OMSetDepthStencilState(m_depthStencilState, 1);
    g_Zbuffer = true;
}

// ----------------------------------------------------------------------------------------------
void DirectX::DX11Class::TurnZBufferOff(void* ctx)
// ----------------------------------------------------------------------------------------------
{
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;
    if (!g_Zbuffer) return;
	pContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
    g_Zbuffer = false;
}

// ----------------------------------------------------------------------------------------------
// Depth-Buffer:
// ----------------------------------------------------------------------------------------------
// Select the Depth-Buffer Format (16, 24, 32, 64, or default...)
//----------------------------------------------------------------------------------------------
DXGI_FORMAT DirectX::DX11Class::SelectDepthFormat(UINT depthBits, BOOL fullscreen)
//----------------------------------------------------------------------------------------------
{
	DXGI_FORMAT buffer_depth_format = DXGI_FORMAT_UNKNOWN;

	switch (depthBits)
	{
		// REF: http://developer.download.nvidia.com/presentations/2008/GDC/GDC08-D3DDay-Performance.pdf

	case 16://A buffer that contains 16-bits of depth data:
		buffer_depth_format = DXGI_FORMAT_D16_UNORM;			// Fastest!
		break;

	case 24:// A 32 bit buffer that contains 24 bits of depth data and 8 bits of stencil data:
		buffer_depth_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;

	case 32://A buffer that contains 32-bits of depth data:
		buffer_depth_format = DXGI_FORMAT_D32_FLOAT;			// High-precision but slower
		break;

	default:// Default:"Zero" value => use the Best!
	case 64:// A double 32 bit buffer that contains 32 bits of depth data and 8 bits padded with 24 zero bits of stencil data:
		buffer_depth_format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;	// Best Results but even slower
		break;
	}

	// Verify our target format is supported by the current device
	// Handles WDDM 1.0 or WDDM 1.1 device driver cases as well as DirectX 11.0 Runtime
	UINT support = 0;
	HRESULT hr = m_device11->CheckFormatSupport(buffer_depth_format, &support);

	// Not Supported? We are forced to use the Default!
	if (hr != S_OK) {
		// which is a 32-bit z-buffer format that supports 24 bits for depth buffer and 8 bits for stencil buffer
		WomaMessageBox(TEXT("WARNING: The specified Depth Buffer Size is invalid!\n Using the default"), TEXT("WARNING"));
		buffer_depth_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

	return buffer_depth_format;
}

#if !defined USE_DX11_1_SETUP
bool DX11Class::createDepthStencil(int screenWidth, int screenHeight, BOOL fullscreen, UINT depthBits)
{
	// We'll use this to create a depth buffer so that our polygons can be rendered properly in 3D space. 
	// At the same time we will attach a stencil buffer to our depth buffer. 
	// The stencil buffer can be used to achieve effects such as motion blur, volumetric shadows, and other things:
	// ============================================================================================================
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };	//ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1; //FIX ME! With 4 (createSetDepthStencilView Might FAIL!)
	depthBufferDesc.ArraySize = 1;

	BUFFER_DEPTH_FORMAT = SelectDepthFormat(depthBits, fullscreen);
	depthBufferDesc.Format = BUFFER_DEPTH_FORMAT; //DXGI_FORMAT_D24_UNORM_S8_UINT; //The formats that support stenciling are: DXGI_FORMAT_D24_UNORM_S8_UINT and DXGI_FORMAT_D32_FLOAT_S8X24_UINT.
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	depthBufferDesc.SampleDesc.Count = MSAA_COUNT;			//Anti-Aliasing: MultiSample tech.
	depthBufferDesc.SampleDesc.Quality = MSAA_QUALITY;		//Anti-Aliasing: Texture Filtering tech. (MSAA > 0)

	//depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthBufferDesc.CPUAccessFlags = 0;
	//depthBufferDesc.MiscFlags = 0;

	// Then this 2D buffer is drawn to the screen:
	// ===========================================
	// Create the texture for the depth buffer using the filled out description.
	IF_FAILED_RETURN_FALSE (m_device11->CreateTexture2D(&depthBufferDesc, NULL, &DX11windowsArray[0].m_depthStencilBuffer));

#if TUTORIAL_PRE_CHAP >= 7
	IF_FAILED_RETURN_FALSE (m_device11->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilWaterBuffer));
#endif

	return true;
}
#else

// Creates per-window depth+DSV that MATCH the swap chain's back buffer (sample count & size).
// Call this after (re)creating the swap chain and after CreateRenderTargetView().
bool DX11Class::createDepthStencil(int screenWidth, int screenHeight, BOOL fullscreen, UINT depthBits)
{
	// Choose formats
	// Use typeless so you can create DSV and SRV from the same texture if you ever need it.
	DXGI_FORMAT depthTexFormat;
	DXGI_FORMAT dsvFormat;
	if (depthBits >= 32) {
		depthTexFormat = DXGI_FORMAT_R32_TYPELESS;
		dsvFormat = DXGI_FORMAT_D32_FLOAT;
	}
	else {
		depthTexFormat = DXGI_FORMAT_R24G8_TYPELESS;
		dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

	// For each window/swap chain
	for (size_t i = 0; i < DX11windowsArray.size(); ++i)
	{
		auto& win = DX11windowsArray[i];

		// Release old (safe if null)
		SAFE_RELEASE(win.m_depthStencilView);
		SAFE_RELEASE(win.m_depthStencilBuffer);

		if (!win.m_swapChain1)
			continue;

		// Read back buffer desc to MATCH size and SampleDesc
		ComPtr<ID3D11Texture2D> bb;
		HRESULT hr = win.m_swapChain1->GetBuffer(0, IID_PPV_ARGS(&bb));
		if (FAILED(hr)) { WomaFatalException("GetBuffer(0) failed for depth creation."); return false; }

		D3D11_TEXTURE2D_DESC bbDesc{};
		bb->GetDesc(&bbDesc);

		// Build depth texture desc, matching back buffer
		D3D11_TEXTURE2D_DESC dsDesc{};
		dsDesc.Width = bbDesc.Width;   // match back buffer (you can also use screenWidth/Height)
		dsDesc.Height = bbDesc.Height;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
		dsDesc.Format = depthTexFormat; // typeless
		dsDesc.SampleDesc = bbDesc.SampleDesc; // *** MUST MATCH back buffer (flip model ? {1,0}) ***
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;

		IF_FAILED_RETURN_FALSE(m_device11->CreateTexture2D(&dsDesc, nullptr, &win.m_depthStencilBuffer));

		// Create DSV view on that texture
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = dsvFormat;
		dsvDesc.ViewDimension = (dsDesc.SampleDesc.Count > 1)
			? D3D11_DSV_DIMENSION_TEXTURE2DMS
			: D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = 0;

		IF_FAILED_RETURN_FALSE(m_device11->CreateDepthStencilView(
			win.m_depthStencilBuffer, &dsvDesc, &win.m_depthStencilView));
	}

	return true;
}
#endif

//Init Step: 7
#if !defined USE_DX11_1_SETUP
// ----------------------------------------------------------------------------------------------
bool DX11Class::createSetDepthStencilView (int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	HRESULT result;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//******************************************************************
	// Create and Set the Depth Stencil; "View"
	//******************************************************************
	// The next thing we need to create is the description of the view of the depth stencil buffer. 
	// We do this so that Direct3D knows to use the depth buffer as a depth stencil texture. 
	// After filling out the description we then call the function CreateDepthStencilView to create it. 

	// Set up the depth stencil view description:
	depthStencilViewDesc.Format = BUFFER_DEPTH_FORMAT; //DXGI_FORMAT_D24_UNORM_S8_UINT; The formats that support stenciling are: DXGI_FORMAT_D24_UNORM_S8_UINT and DXGI_FORMAT_D32_FLOAT_S8X24_UINT.
	depthStencilViewDesc.ViewDimension = (SystemHandle->AppSettings->MSAA_Anisotropic) ? D3D11_DSV_DIMENSION_TEXTURE2DMS: D3D11_DSV_DIMENSION_TEXTURE2D; //MSAA
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	//depthStencilViewDesc.Flags = 0;
 
	// For each Monitor: 
	for (int i = 0; i < DX11windowsArray.size(); i++)
	{
		result = m_device11->CreateDepthStencilView(DX11windowsArray[i].m_depthStencilBuffer, &depthStencilViewDesc, &DX11windowsArray[i].m_depthStencilView); //depthBufferDSV
		IF_FAILED_RETURN_FALSE(result);
	}

	return true;
}
#else
bool DX11Class::createSetDepthStencilView(int screenWidth, int screenHeight)
{
	for (size_t i = 0; i < DX11windowsArray.size(); ++i)
	{
		if (DX11windowsArray[i].m_depthStencilView)
		{
			// Already created in createDepthStencil()
			m_deviceContext->OMSetRenderTargets(
				1,
				&DX11windowsArray[i].m_renderTargetView,
				DX11windowsArray[i].m_depthStencilView);
		}
	}
	return true;
}

#endif


// ----------------------------------------------------------------------------------------------
void DirectX::DX11Class::ClearDepthBuffer(void* ctx)
// ----------------------------------------------------------------------------------------------
{
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;
	// For each Monitor: 
	for (int i = 0; i < DX11windowsArray.size(); i++)
		pContext->ClearDepthStencilView(DX11windowsArray[i].m_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);	// Clear the "depth buffer":
}

}

#endif


#endif