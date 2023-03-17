// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: DX12Class.h
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
#include<D3D11.h> //To populate: D3D11_SPEC_DATE_YEAR
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 && DX_ENGINE_LEVEL >= 19
/////////////
// LINKING // First thing in the header is to specify the libraries to link when using this object module
/////////////
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//The next thing we do is include the headers for those libraries that we are linking to this object module.

//////////////
// INCLUDES //
//////////////

//#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#include <Windows.h>

//SELECT DX12 DXGI_API version:
//#define DXGI_API 3
#define DXGI_API 4
//#define DXGI_API 5	// SDK10+

// DX12 CORE:
#if DXGI_API == 3
#include <dxgi1_3.h>	// Always 1st!	(Select Driver)
#endif
#if DXGI_API == 4
#include <dxgi1_4.h>	// Always 1st!	(Select Driver)
#endif
#if DXGI_API == 5
#include <dxgi1_5.h>	// Always 1st!	(Select Driver)
#endif

#include <d3d12.h>		// DX12			(Select Device)
#include <D3Dcompiler.h>// Use Compiler
#include <DirectXMath.h>// Use Math
//#endif

#if D3D11_SPEC_DATE_YEAR == 2009
//#include <D3dx9core.h>			//D3DX_SDK_VERSION (Checks for the existance of the correct D3DX library version)
#endif

// DX12 2D:

// DX12 UTILS:
#include "d3dx12.h"

// Windows STL includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <conio.h>
#include <vector>
#include <string>
#include <array>
#include <wrl.h>

using namespace DirectX;

#include <assert.h>
#include <vector>
#include <sstream> // to allow: fileTmp = "data/" + fileTmp;

#include "womadriverclass.h"

#ifdef _DEBUG
	#include "DXGIDebug.h"
#endif

#include "ApplicationClass.h"

// ----------------------------------------------------------------------------------------------

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 // DXGI1_2 

	#include "DXcameraClass.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

namespace DirectX {

////////////////////////////////////////////////////////////////////////////////
// Class name: DX12Class
////////////////////////////////////////////////////////////////////////////////
class DX12Class : public WomaDriverClass
{
public:
	static const auto BufferCount = 2;  // "Use Double Buffer"
//	static const auto BufferCount = 3;  // "Use Tripple Buffer"

	DX12Class();
	~DX12Class();
	void Shutdown2D();
	void Shutdown();

	BOOL Check (int* Hi, int* low);
	//#if DXGI_API >= 3
	ComPtr<IDXGIAdapter3> DXGIadapter3 = NULL;
	void DX12Class::QueryVideoMemoryInfo(UINT adapterIndex, ComPtr<IDXGIAdapter1> adapter);
	//#endif
	void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter, D3D_FEATURE_LEVEL feacture_level);
	BOOL CheckAPIdriver (UINT USE_THIS_ADAPTER);

	void SelectDepthFormat(UINT depthBits, BOOL fullscreen);
	bool OnInit(	int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits, 
						float screenDepth, float screenNear, BOOL msaa, bool vsync, 
						BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize);

	bool Resize (int screenWidth, int screenHeight,float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits);
	void SetCamera2D();
	void BeginScene(UINT monitorWindow);
	void EndScene(UINT monitorWindow);
	void ClearDepthBuffer();

	void MoveToNextFrame();
	void WaitForGpu();

	void ResetViewport();
	void SetBackBufferRenderTarget();

	void Initialize3DCamera();
	bool Initialize(float* clearColor);	//bool Initialize();
	void Finalize();

	XMMATRIX* GetViewMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);
	XMMATRIX* GetProjectionMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);

	bool getModesList(int g_USE_MONITOR, int screenWidth, int screenHeight, BOOL fullscreen, UINT* numerator, UINT* denominator);

//#if ZERO && defined USE_RASTERIZER_STATE
	void SetRasterizerState(UINT cullMode, UINT fillMode);
	D3D12_RASTERIZER_DESC m_rasterState[3][2];
	//D3D12_RASTERIZER_DESC* rasterState;
//#endif

	// 3D
	XMMATRIX m_projectionMatrix;
	void GetProjectionMatrix(XMMATRIX&);

    //We need to setup our ProjectionMatrix (21) and OrthoMatrix (CH07)
	void setProjectionMatrixWorldMatrixOrthoMatrix (int screenWidth, int screenHeight,float screenNear, float screenDepth);

    //We now have two new function in the DX12Class for turning the Z buffer on and off when rendering 2D images:
    void TurnZBufferOn();
    void TurnZBufferOff();

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	ImageLoaderClass* CaptureScreenShot(int screenWidth, int screenHeight);
#endif

	// Public: ------------------------------------------------------------------------
	// VARS:
	// --------------------------------------------------------------------------------
	DXGI_FORMAT BUFFER_COLOR_FORMAT;
	DXGI_FORMAT BUFFER_DEPTH_FORMAT;

	bool ScissorEnable;

	// ---------------------------------------------------------
	bool	g_ALLOW_DX9x;
	//UINT	ShaderVersionH, ShaderVersionL;	// Basics of Refrash rate / Shaver Version:

	//DXGI_MODE_DESC* displayModeList;
	
// ---------------------------------------------------------

	DXcameraClass* m_Camera = NULL;


////////////////////////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////////////////////////
	
	bool initDX12Device (HWND hwnd);
	bool createSwapChainDX12device (HWND hwnd, int screenWidth, int screenHeight, BOOL vsync, BOOL fullscreen,
									BOOL g_UseDoubleBuffering, BOOL g_AllowResize, UINT numerator, UINT denominator);
	void getProfile ();
	bool createAllRasterizerStates (bool lineAntialiasing);

	void setViewportDevice (int screenWidth, int screenHeight);
	bool CreateRenderTargetView (int screenWidth, int screenHeight);
	bool createSetDepthStencilState (bool depthEnable);

	// ---------------------------------------------------------

#if DXGI_API == 3
	ComPtr<IDXGIFactory3>		dxgiFactory;  // Create a DXGI 1.3 Interface
#endif
//#if DXGI_API == 4
	ComPtr<IDXGIFactory4>		dxgiFactory;  // Create a DXGI 1.4 Interface
//#endif
#if DXGI_API == 5
	ComPtr<IDXGIFactory5>		dxgiFactory;  // Create a DXGI 1.5 Interface
#endif

	ComPtr<ID3D12Resource>		m_depthStencil;
	
public:
	// Pipeline objects:
	D3D12_VIEWPORT				m_viewport;
	D3D12_RECT					m_scissorRect;
	ComPtr<IDXGISwapChain>		s_PrimarySwapChain;
	ComPtr<IDXGISwapChain3>		m_swapChain;		//IDXGISwapChain3* m_swapChain;
	ID3D12Device*				m_device;

	UINT m_currentFrame;
	ComPtr<ID3D12Resource>			m_renderTargets[BufferCount];

	ComPtr<ID3D12CommandAllocator>	m_commandAllocator;
	ComPtr<ID3D12CommandQueue>		m_commandQueue;

	ComPtr<ID3D12DescriptorHeap>	m_rtvHeap;

	UINT m_rtvDescriptorSize;
	
	// Synchronization objects:
	HANDLE				m_swapChainEvent = NULL;
	ComPtr<ID3D12Fence> m_renderContextFence;
	UINT64				m_renderContextFenceValue = NULL;
	HANDLE				m_renderContextFenceEvent = NULL;
	UINT64				m_frameFenceValues[BufferCount];

	UINT				m_CullMode= NULL, m_fillMode = NULL;
	HANDLE				m_fenceEvent = NULL;
	

	// Asset objects:
	ComPtr<ID3D12Fence>					m_fence;
	//UINT64							m_fenceValue;
	UINT64								m_fenceValues[BufferCount] = {0};
	ComPtr<ID3D12GraphicsCommandList>	m_commandList=NULL;
};

}
#endif
#endif
#endif
