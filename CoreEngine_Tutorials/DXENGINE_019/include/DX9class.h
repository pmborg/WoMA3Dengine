// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DX9Class.h
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

// First thing in the header is to specify the libraries to link when using this object module

#include "platform.h"

//#include<D3D11.h>
#if defined DX9sdk
#include "DX9Class.h"
/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//////////////
// INCLUDES //
//////////////

//
// [DX9]
// FATAL ERROR: fatal error C1083: Cannot open include file: 'd3dx9.h': No such file or directory
// Install: Microsoft_DirectX_SDK_June_2010
//

#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( disable : 4324 )
#include <DirectXMath.h> //#include <xnamath.h>

#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")

#include "womadriverclass.h"

typedef IDirect3D9* (WINAPI* LPDIRECT3DCREATE9)(UINT);
typedef IDirect3D9Ex* (WINAPI* LPDIRECT3DCREATE9EX)(UINT);

////////////////////////////////////////////////////////////////////////////////
// Class name: DX9Class
////////////////////////////////////////////////////////////////////////////////

namespace DirectX {

class DX9Class : public WomaDriverClass
{
public:
	DX9Class();
	~DX9Class();

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

	BOOL Check (int* Hi, int* low);
	BOOL CheckAPIdriver (UINT USE_THIS_ADAPTER);
	bool getModesList(int screenWidth, int screenHeight, BOOL fullscreen, UINT* numerator, UINT* denominator);

	void TurnZBufferOn();
	void TurnZBufferOff();

	bool DX9Class::getModesList(int g_USE_MONITOR, int screenWidth, int screenHeight, BOOL fullscreen, UINT* numerator, UINT* denominator);
	BOOL CheckAPIdriver (UINT USE_THIS_ADAPTER);
	void SelectDepthFormat(UINT depthBits, BOOL fullscreen);
	
	
    //bool Resize (int screenWidth, int screenHeight,float screenNear, float screenDepth);
	bool Resize(int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits);
	void SetCamera2D();

	XMMATRIX* GetViewMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);
	XMMATRIX* GetProjectionMatrix(void* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix);

	HRESULT RenderD3D9();

////////////////////////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////////////////////////

	bool createDevice(UINT g_USE_MONITOR, BOOL vsync, BOOL fullscreen, BOOL g_AllowResize, int screenWidth, int screenHeight);

	void getProfile(UINT g_USE_MONITOR);
	bool createRasterizerStates(bool lineAntialiasing);

	void setViewportDevice(int screenWidth, int screenHeight); //void ResetViewport();
	bool CreateRenderTargetView(int screenWidth, int screenHeight);
	bool createSetDepthStencilState(bool depthEnable);
	bool createSetDepthStencilView();

public:

	// Public: ------------------------------------------------------------------------
	// VARS:
	// --------------------------------------------------------------------------------

	D3DFORMAT	BUFFER_DEPTH_FORMAT;

	D3DPRESENT_PARAMETERS	d3dpp;

	//IDirect3D9Ex*       m_D3D9Ex;
	//IDirect3DDevice9Ex* m_DX9deviceEx;

    IDirect3D9*         m_D3D9;
    IDirect3DDevice9*   m_device;

    //TCHAR	szShaderModel[20];
    // ---------------------------------------------------------
    UINT	ufreededicatedVideoMem;

	bool	bDeviceLost;
	//UINT	ShaderVersionH, ShaderVersionL;

	D3DVIEWPORT9 viewport;

private:
	LPD3DXFONT	m_D3D9Font;

};

}

#endif
