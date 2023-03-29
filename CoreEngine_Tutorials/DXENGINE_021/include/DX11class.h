// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: DX11Class.h
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
#pragma once

#include "platform.h"
#include<D3D11.h>
//SELECT DXGI DX11 version:
	#include<D3D11.h>
	#if defined WIN6x
		#define DXGI1_3	// DX10: Target For: Vista (for drivers WDDM 1.0 specification)
	#elif D3D11_SPEC_DATE_YEAR == 2009
		#define DXGI1_1				//SDK: C:\WoMAengine2014\Microsoft_DirectX_SDK_June_2010
			// WINDOWS VISTA (SP2):
			//- Support for synchronized shared surfaces
			//- Support for high color
			//- Support for maximum frame latency control
			//- Support for adapter enumeration ordering: IDXGIFactory1::EnumAdapters1
			//- Support for BGRA format

	#else
		//https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads
		//https://github.com/toivjon/dxgi-sandbox
		//https://github.com/apitrace/dxsdk
		//SDK10+: C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um\d3d11.h

		#define DXGI1_2 //Nov 29, 2013 -  WINDOWS 7 (PLATFORM UPDATE) / WINDOWS 8.0:
			// Support for stereo swap chain
			// Support for flip // model swap chain
			// Support for desktop duplication
			// Support for 16 bpp formats
			// Support for debugging APIs
			// Optimization of the use of video memory
			// Optimization for presentations
		
		//#define DXGI1_3 //Sep 3, 2014 -  WINDOWS 8.1 SDK
			// Support for DXGI adapter memory trim
			// Support for multi // plane overlays
			// Support for overlappingand scaled swap chains
			// Support for selecting backbuffer subregion for swap chain
			// Support for lower - latency swap chain presentation

		//#define DXGI1_4 //Aug 11, 2015 - WINDOWS 10.0.10240.0
			// Support for cheaper adapter enumeration
			// Support for video memory budget tracking
			// Support for invariant backbuffer identity
			// Support for alternate frame rendering(AFR)
			// Fullscreen state no longer owns the display

		//#define DXGI1_5 //Oct 5, 2017 - WINDOWS 10.0.14393:
			// Support for Hich Dynamic Range(HDR)
			// Support for Wide Color Gamut(WCG)
			// Support for flexibleand performant output duplication
			// Support for offeringand reclaiming resources

		//#define DXGI1_6 //Oct 10, 2017 - WINDOWS 10.0.15063:
			// Support for listening adapter enumeration change events
			// Support for adapter enumeration ordering prefence
			// Support for checking adapter synchronization object features
			// Support for detecting HDR display detection
	#endif

#include "main.h"
#if _DEBUG
#pragma comment(lib, "dxguid.lib")
#endif

/////////////
// LINKING // First thing in the header is to specify the libraries to link when using this object module
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#if _DEBUG
#pragma comment(lib, "dxguid.lib")
#endif

//////////////
// INCLUDES //
//////////////
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#pragma warning( disable : 4067 )	//warning C4067: unexpected tokens following preprocessor directive - expected a newline

// --------------------------------------------------------------------------------------------
#ifdef notes
Low Level Display Drivers API version :
--------------------------------------
WDDM 1.0->Windows Vista				Display Drivers CreateDXGIFactory	Creates a DXGI 1.0
WDDM 1.1->Vista * (Up)&Windows 7	Display Drivers CreateDXGIFactory1	Creates a DXGI 1.1
WDDM 1.2->Windows 8					Display Drivers or Creates a DXGI 1.2
WDDM 1.3->Windows 8.1				Display Drivers CreateDXGIFactory2	Creates a DXGI 1.3
WDDM 2.0->Windows 10				Display Drivers or Creates a DXGI 1.4
#endif

#ifdef DXGI1_6
	#include <dxgi1_6.h>
#elif defined DXGI1_5
	#include <dxgi1_5.h>
#elif defined DXGI1_4
	#include <dxgi1_4.h>
#elif defined DXGI1_3
	#include <dxgi1_3.h>
#elif defined DXGI1_2	
	#include <dxgi1_2.h>
#else
	#include <dxgi.h>
#endif

// ----------------------------------------------------------------------------------------------
//Check: DXGI_H = 1; DXGI_L = 3;

// (WINDOWS == 6.0) // WINDOWS Vista									==> SUPPORT: DX10	/ DRIVER MODEL: WDDM 1.0 ==> (DXGI 1.0) = 
// (WINDOWS == 6.1) // WINDOWS 7 & Windows Vista + Update: KB971644		==> SUPPORT: DX11.0 / DRIVER MODEL: WDDM 1.1
// (WINDOWS == 6.2) // WINDOWS 8	==> SUPPORT: DX11.1 / DRIVER MODEL: WDDM 1.2 ==> Added: StereoScopic 3D
// (WINDOWS >= 6.3) // WINDOWS 8.1 	==> SUPPORT: DX11.2 / DRIVER MODEL: WDDM 1.3 (Win8.1 ONLY!)
// (WINDOWS >= 10)  // WINDOWS 10	==> SUPPORT: DX12 / DRIVER MODEL: WDDM 2.0

//DXGI1_0 // DX10: For: Vista
//DXGI1_1 // DX11: For: Vista / Win7 / Win8.0 (WDDM 1.1 drivers are backward compatible with WDDM 1.0 specification)
//DXGI1_2 // DX11: For: Win8.0 Only!

#if defined DXGI1_6
#define IDXGIAdapterN IDXGIAdapter4
#define IDXGIFactoryN IDXGIFactory6
#define CreateDXGIFactoryN CreateDXGIFactory2
#define EnumAdaptersN pDXGIFactory->EnumAdapters1
#define DXGI_ADAPTER_DESCN DXGI_ADAPTER_DESC2
#define GetDescN pAdapter->GetDesc2

#elif defined DXGI1_5
#define IDXGIAdapterN IDXGIAdapter3
#define IDXGIFactoryN IDXGIFactory5
#define CreateDXGIFactoryN CreateDXGIFactory2
#define EnumAdaptersN pDXGIFactory->EnumAdapters1
#define DXGI_ADAPTER_DESCN DXGI_ADAPTER_DESC2
#define GetDescN pAdapter->GetDesc2

#elif defined DXGI1_4
#define IDXGIAdapterN IDXGIAdapter3
#define IDXGIFactoryN IDXGIFactory4
#define CreateDXGIFactoryN CreateDXGIFactory2
#define EnumAdaptersN pDXGIFactory->EnumAdapters1
#define DXGI_ADAPTER_DESCN DXGI_ADAPTER_DESC2
#define GetDescN pAdapter->GetDesc2

#elif defined DXGI1_3
#define IDXGIAdapterN IDXGIAdapter2
#define IDXGIFactoryN IDXGIFactory3
#define CreateDXGIFactoryN CreateDXGIFactory2
#define EnumAdaptersN pDXGIFactory->EnumAdapters1
#define DXGI_ADAPTER_DESCN DXGI_ADAPTER_DESC2
#define GetDescN pAdapter->GetDesc2

#elif defined DXGI1_2 
#define IDXGIAdapterN IDXGIAdapter2
#define IDXGIFactoryN IDXGIFactory2
#define CreateDXGIFactoryN CreateDXGIFactory1
#define EnumAdaptersN pDXGIFactory->EnumAdapters1
#define DXGI_ADAPTER_DESCN DXGI_ADAPTER_DESC2
#define GetDescN pAdapter->GetDesc2

#elif defined DXGI1_1 //available on Windows 7, Windows Server 2008 R2, and as an update to Windows Vista with Service Pack 2 (SP2) (KB 971644) and Windows Server 2008 (KB 971512).
//SDK: C:\DirectXSDK_June2010
#define IDXGIAdapterN IDXGIAdapter1
#define IDXGIFactoryN IDXGIFactory1
#define CreateDXGIFactoryN CreateDXGIFactory1
#define EnumAdaptersN EnumAdapters1
#define DXGI_ADAPTER_DESCN DXGI_ADAPTER_DESC1
#define GetDescN pAdapter->GetDesc1

#else //DXGI1_0
// For: Vista / Win7 / Win8
#define IDXGIAdapterN IDXGIAdapter
#define IDXGIFactoryN IDXGIFactory
#define CreateDXGIFactoryN CreateDXGIFactory
#define EnumAdaptersN factory->EnumAdapters
#define DXGI_ADAPTER_DESCN DXGI_ADAPTER_DESC
#define GetDescN pAdapter->GetDesc
#endif

#include <d3dcommon.h>
#include <d3d11.h>

#if D3D11_SPEC_DATE_YEAR == 2009
	//ON [VS2015 or VS2017 or VS2019] We will use the win10SDK (ALERT SYSTEM ONLY!) --> "D3D11_SPEC_DATE_YEAR = 2011"
	#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
	#pragma warning( disable : 4838 )
	#include <DirectXMath.h> //#include <xnamath.h>				//#include <d3dx10math.h>
	#include <d3dx11tex.h>
	#pragma comment(lib, "/WoMA3Dengine/ThirdParty/Microsoft_DirectX_SDK_June_2010\\Lib\\x64\\d3dx11.lib")	// DONT NEED THIS AT WIN8 SDK BUT NEED WITH OLD JUN 2010 SDK
	#pragma comment(lib, "/WoMA3Dengine/ThirdParty/Microsoft_DirectX_SDK_June_2010\\Lib\\x64\\d3dx10.lib")	// DONT NEED THIS AT WIN8 SDK BUT NEED WITH OLD JUN 2010 SDK
#else
	#include <DirectXMath.h> 
	using namespace DirectX;
#endif

#include <assert.h>
#include <vector>
#include <sstream> // to allow: fileTmp = "data/" + fileTmp;

#include "womadriverclass.h"

#ifdef _DEBUG
	#include "DXGIDebug.h"
#endif



extern ID3D11DeviceContext* g_deviceContext; // Also to Comunicate with DXUT

	#include "DXcameraClass.h"

#define	MaxTextSizes 24

//extern UINT g_MSAA_X;

namespace DirectX {

struct DXTextLine
{
	int Xpos;
	int Ypos;
	STRING printText;
	float R;
	float G;
	float B;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: DX11Class
////////////////////////////////////////////////////////////////////////////////
class DX11Class : public WomaDriverClass
{
public:
	UINT WomaIntegrityCheck = 1234567890;
	DX11Class();
	~DX11Class();

#if _DEBUG
	IDXGIDebug* debugDev;
#endif

	bool dx11_force_dx9;
	void Initialize3DCamera();
	bool Initialize(float* clearColor);	//bool Initialize();
	void Finalize();

	bool OnInit(int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits,
				float screenDepth, float screenNear, BOOL msaa, bool vsync,
				BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize);
	void Shutdown();
	void Shutdown2D();

	void BeginScene(UINT monitorWindow);
	void EndScene(UINT monitorWindow);
	void ClearDepthBuffer();

	void SetRasterizerState(UINT cullMode, UINT fillMode);
	//We now have two new function in the DX11Class for turning the Z buffer on and off when rendering 2D images:
	void TurnZBufferOn();
	void TurnZBufferOff();

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	ImageLoaderClass* CaptureScreenShot(int screenWidth, int screenHeight);
#endif

	BOOL Check (int* Hi, int* low);
	BOOL CheckAPIdriver (UINT USE_THIS_ADAPTER);
	bool getModesList(int g_USE_MONITOR, int screenWidth, int screenHeight, BOOL fullscreen, UINT* numerator, UINT* denominator);

	void SelectDepthFormat(UINT depthBits, BOOL fullscreen);

	bool Resize (int screenWidth, int screenHeight,float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits);
	void SetCamera2D();

	void ResetViewport();
	void SetBackBufferRenderTarget(UINT monitorWindow);

	XMMATRIX* GetViewMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);
	XMMATRIX* GetProjectionMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);

	// 3D
	XMMATRIX m_projectionMatrix;
	void GetProjectionMatrix(XMMATRIX&);

    //We need to setup our ProjectionMatrix (21) and OrthoMatrix (CH07)
	void setProjectionMatrixWorldMatrixOrthoMatrix (int screenWidth, int screenHeight,float screenNear, float screenDepth);

	// Public: ------------------------------------------------------------------------
	// VARS:
	// --------------------------------------------------------------------------------
	DXGI_FORMAT BUFFER_COLOR_FORMAT= DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT BUFFER_DEPTH_FORMAT;

	// For each DX11 Adapter:
	IDXGIAdapter1* adapterGraphicCard = NULL;
	ID3D11Device* m_device = NULL;
	ID3D11DeviceContext* m_deviceContext = NULL;

	// For each DX11 Monitor:
	struct DXwindowDataContainer
	{
		IDXGISwapChain* m_swapChain;
		ID3D11Texture2D* m_backBuffer;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11DepthStencilView* m_depthStencilView;
		D3D11_VIEWPORT viewport;
	};
	std::vector<DXwindowDataContainer> DX11windowsArray;

	// TODO: Go inside DXwindowDataContainer
	bool ScissorEnable;

	UINT mCurRasterState;

	// ---------------------------------------------------------
	bool	g_ALLOW_DX9x;

	// Aux. Used by all monitors:
	// Used in: resolutionType resolution;
	// SystemHandle->windowsArray.push_back(screen);
	DXGI_MODE_DESC* displayModeList = NULL;
	
#if zero
	ID3D11DepthStencilView* m_depthStencilViewWater = NULL;
#endif

// ---------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////////////////////////
	
	bool createDevice ();
	void setDeviceCapabilities(D3D_FEATURE_LEVEL featureLevel);
	bool createSwapChainDX11device (HWND hwnd, int screenWidth, int screenHeight, BOOL vsync, BOOL fullscreen,
									BOOL g_UseDoubleBuffering, BOOL g_AllowResize, UINT numerator, UINT denominator);
	void getProfile ( UINT g_USE_MONITOR );

	bool createRasterizerStates (bool lineAntialiasing);

	void setViewportDevice(UINT monitorWindow, int screenWidth, int screenHeight);
	void setScissorRectangle(UINT left, UINT right, UINT top, UINT bottom, bool enabled);

	bool CreateRenderTargetView (int screenWidth, int screenHeight);
	bool createDepthStencil(int screenWidth, int screenHeight, BOOL fullscreen, UINT depthBits);
	bool createSetDepthStencilState (bool depthEnable);
	bool createSetDepthStencilView (int screenWidth, int screenHeight);

	// ---------------------------------------------------------
	// ---------------------------------------------------------
	BYTE dummybuff1[128] = { 0 };
	ID3D11Texture2D*		 m_depthBuffer=NULL;
	ID3D11DepthStencilState* m_depthStencilState = NULL;
	ID3D11DepthStencilState* m_depthDisabledStencilState = NULL;
	BYTE dummybuff2[128] = { 0 };
	ID3D11RasterizerState* m_rasterState[3][2] = { 0 };

public:

};

}

