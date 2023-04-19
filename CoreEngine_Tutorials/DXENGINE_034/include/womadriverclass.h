// --------------------------------------------------------------------------------------------
// Filename: womadriverclass.h
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

#pragma once

#include "main.h"

// Define the Rendering States for textures:
#define CULL_FRONT	0
#define CULL_BACK	1
#define CULL_NONE	2

#define FILL_SOLID	0
#define FILL_WIRE	1

#include <d3d11.h>
#if (D3D11_SPEC_DATE_YEAR > 2009)
	#define SUPPORT_TGA
#endif

	#include "virtualCameraClass.h"

	#include "DXfrustumClass.h"

	#define FrustumClass DXfrustumClass


#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
	#include "ImageLoaderClass.h"
#endif

// ----------------------------------------------------------------------------------------------
enum CAMERA_TYPE
{
	CAMERA_NORMAL = 0,			// PLAYER CAMERA
	CAMERA_SKY					// FIXED CAMERA AT 0,0,0
};

enum PROJECTION_TYPE
{
	PROJECTION_PERSPECTIVE = 0, //3D
	PROJECTION_ORTHOGRAPH		//2D
};

enum SHADER_TYPE
{
	SHADER_AUTO = -1, 
	SHADER_COLOR,						//21: M3D v1.0	public MAIN (Used by: 21 & Sun Ray & 3D Obj)
	SHADER_TEXTURE,						//22: M3D v1.1	public MAIN (Used by: 22 & Banner & Sky2D & SplashIntro & UnderWater & Font & 3D Obj)

	SHADER_TEXTURE_FONT,				//27: fade: using alfa color
	SHADER_TEXTURE_LIGHT,				//23: M3D v1.2	public MAIN + Pass2: Shadows (Used by: 23 & Sky3D & Sun & Moon & 3D Obj)
	SHADER_TEXTURE_LIGHT_RENDERSHADOW,  //45:			public "Render Shadows"
	SHADER_TEXTURE_LIGHT_CASTSHADOW,	//45:			private "Pass1: Shadows" (add auxiliar Shader)
	SHADER_NORMAL_BUMP,					//47: M3D v1.5	public MAIN (Used by: 47 & 3D Obj)

	// Have to be last:
	SHADER_TEXTURE_LIGHT_INSTANCED,				//public   51: INST 23 light with Instances    (pass2) 
	SHADER_TEXTURE_LIGHT_CASTSHADOW_INSTANCED,	//private  51: INST 45 shadows with Instances  (pass1) 
	SHADER_NORMAL_BUMP_INSTANCED,				//private  51: INST 47 bump with Instances ... check: 051LightInstance.hlsl

	// TERRAINS:
	SHADER_Double_Color_Terrain,
	SHADER_Double_Color_Slop_Terrain,
	SHADER_Double_Color_Slop_Detail_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMapping_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMappingDouble_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMappingDouble_Bump_Terrain,
};


struct Capabilities 
{
	// DX Capabilities:
	bool CapDX9 = false;
	bool CapDX10_11 = false;
	bool CapDX12 = false;
	bool CapDX12_1 = false;

	STRING SHADER_TYPE_NAME;

	BOOL   inStereoAdapterMode=false;

	size_t nTotalAvailableGPUMemory;		//< Total available GPU memory in kilobytes, 0 if it was not possible to determine this value, this value may not match your graphics card specification (e.g. "512 MiB" may get you "480 MiB" in here)
	UINT   SelectedDriverType = 0;
	bool   USE_DXDRIVER_FONTSBoolean;

	bool   DXGI10;
	bool   DXGI11;
	bool   DXGI12;

	BOOL   MSAA_SUPPORTBoolean;			// MSAA Available?
	UINT   MSAAmultiSampleCount = 0;	// HW: Max.Available
	UINT   MSAAquality = 0;				// HW: Max.Available

	// ----------------------------------------------------------------------------

	float  maxTextureAnisotropy;
	bool   occlusionQueryBoolean;
	bool   instancedArraysBoolean;
	bool   framebufferMultisampleBoolean;
	bool   shaderTextureLODBoolean;

	size_t MaximumSimultaneousRenderTargets = 0;

	size_t max2DTextureSize = 0;
	size_t maxArrayTextureLayers = 0;

	size_t max3DTextureSize = 0;
	size_t maxCubeMapTextureSize = 0;

	size_t maxViewportWidth = 0;
	size_t maxViewportHeight = 0;

	size_t maxElementsIndices = 0;
	size_t maxElementsVertices = 0;

	size_t maxVertexAttributes = 0;
	size_t MaximumConstantBufferSize = 0;

	// ----------------------------------------------------------------------------
	// https://en.wikipedia.org/wiki/Feature_levels_in_Direct3D#Support_matrix
};

////////////////////////////////////////////////////////////////////////////////
// Class name: womadriverclass
////////////////////////////////////////////////////////////////////////////////
class WomaDriverClass 
{
public:
	virtual void Initialize3DCamera()=0;
	virtual bool Initialize(float* clearColor)=0;
	virtual void Finalize()=0;

	virtual bool OnInit(int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits, 
						float screenDepth, float screenNear, BOOL msaa, bool vsync, 
						BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize) = 0;
	virtual void Shutdown()=0;
	virtual void Shutdown2D()=0;

	virtual void BeginScene(UINT monitorWindow) = 0;
	virtual void EndScene(UINT monitorWindow) = 0;
	virtual void ClearDepthBuffer() = 0;

	virtual void SetRasterizerState(UINT cullMode, UINT fillMode) = 0;
	virtual void TurnZBufferOn() = 0;
	virtual void TurnZBufferOff() = 0;

	virtual void TurnOnAlphaBlending() = 0;
	virtual void TurnOffAlphaBlending() = 0;

	UINT	ShaderVersionH = 0, ShaderVersionL = 0;	// Basics of Refrash rate / Shaver Version:
	bool	RenderfirstTime = true;

	// Video Card Capabilities:
	// ----------------------------------------------------------------------------
	Capabilities m_sCapabilities;

	float blendFactor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	bool g_AlphaBlend = false;
	bool g_Zbuffer = false;

	// Video Card Info:
	// ----------------------------------------------------------------------------
	TCHAR driverName[MAX_STR_LEN];		// STRING driverName;
	TCHAR ShaderModel[MAX_STR_LEN];		// STRING ShaderModel;   // "x_y"
	TCHAR szShaderModel[MAX_STR_LEN];	// STRING szShaderModel; // "x.y"
	// ------------------
	
	TCHAR	m_videoCardDescription[MAX_STR_LEN];	//STRING m_videoCardDescription; 
	int		m_videoCardMemory = 0;
	TCHAR	adapterDesc_Description[MAX_STR_LEN];	// Note: have to be wstring
	UINT	ufreededicatedVideoMem = 0;

	// List of resoltions availabel to Use
	// ----------------------------------------------------------------------------
	UINT numerator, denominator = 0;
	UINT MonitorNumber = 0;					// Total number of Monitors
	UINT numModes = 0;						// Number ResolutionModes

	BOOL	m_VSYNC_ENABLED;
	float	ClearColor[4];

	// MSAA Used:
	// ----------------------------------------------------------------------------
	UINT	ReqMSAAmultiSampleCount = 0;	// Req. to Use
	UINT	ReqMSAAquality=0;				// Req. to Use

	FrustumClass* frustum=NULL;
};

#define msaaSamples (ReqMSAAquality - 1)	// MSAA

