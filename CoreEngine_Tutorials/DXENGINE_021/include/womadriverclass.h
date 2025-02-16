// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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
	SHADER_AUTO = 0, 
	SHADER_COLOR,						//21: M3D v1.0	public MAIN (Used by: 21 & Sun Ray & 3D Obj)
	SHADER_TEXTURE,						//22: M3D v1.1	public MAIN (Used by: 22 & Banner & Sky2D & SplashIntro & UnderWater & Font & 3D Obj)

	SHADER_TEXTURE_FONT,				//27: fade: using alfa color
	SHADER_TEXTURE_LIGHT,				//23: M3D v1.2	public MAIN + Pass2: Shadows (Used by: 23 & Sky3D & Sun & Moon & 3D Obj)
	SHADER_NORMAL_BUMP,					//35: M3D v1.3	public MAIN (Used by: 47 & 3D Obj)

	SHADER_TEXTURE_LIGHT_RENDERSHADOW,  //36: Draw Shadows
	SHADER_TEXTURE_LIGHT_CASTSHADOW,	//36: Aux. Shader (render in texture)

	// Have to be last:
	SHADER_TEXTURE_LIGHT_INSTANCED,				//40: INSTANCED like 23 light, but using Instances
	SHADER_TEXTURE_LIGHT_CASTSHADOW_INSTANCED,	//41: Aux. Shader (render in texture), but using Instances (used on 41,42)
	SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED,	//41: INSTANCED like 36 shadow, but using Instances
	SHADER_NORMAL_BUMP_INSTANCED,				//99: INSTANCED like 35 bump, but using Instances

	// TERRAINS:
	SHADER_Double_Color_Terrain,
	SHADER_Double_Color_Slop_Terrain,
	SHADER_Double_Color_Slop_Detail_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMapping_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMappingDouble_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMappingDouble_Bump_Terrain,

	SHADER_SKYTEXTURE,		//90:
	SHADER_REALSKYTEXTURE,	//91:
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
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	// Check [1] D3D12_FEATURE_DATA_D3D12_OPTIONS
	/* Resource binding*---------------------------------- */ /*D3D12_RESOURCE_BINDING_TIER*/ UINT ResourceBindingTier;
	/* Tiled resources*----------------------------------- */ /*D3D12_TILED_RESOURCES_TIER*/ UINT TiledResourcesTier;
	/* Typed UAV loads for additional formats ------------ */ BOOL TypedUAVLoadAdditionalFormats;
	/* Conservative rasterization*------------------------ */ /*D3D12_CONSERVATIVE_RASTERIZATION_TIER*/ UINT ConservativeRasterizationTier;
	/* Rasterizer - ordered views ------------------------ */ BOOL ROVsSupported;
	/* Stencil reference value from Pixel Shader --------- */ BOOL PSSpecifiedStencilRefSupported;
	/* UAV slots for all stages -------------------------- */
	/* Logical blend operations -------------------------- */ BOOL OutputMergerLogicOp;
	/* Double precision (64-bit) floating point operations */ BOOL DoublePrecisionFloatShaderOps;
	/* Minimum floating point precision ------------------ */ /*D3D12_SHADER_MIN_PRECISION_SUPPORT*/ UINT MinPrecisionSupport;
	/* Resource heap ------------------------------------- */ /*D3D12_RESOURCE_HEAP_TIER*/ UINT ResourceHeapTier;
	/* Per - resource virtual addressing ----------------- */ UINT MaxGPUVirtualAddressBitsPerResource;
	/* Per - process virtual addressing ------------------ */ 
	BOOL StandardSwizzle64KBSupported;
	BOOL CrossAdapterRowMajorTextureSupported;
	BOOL VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation;
	/*D3D12_CROSS_NODE_SHARING_TIER*/ UINT CrossNodeSharingTier;

	// Check [2] D3D12_FEATURE_DATA_ARCHITECTURE
	BOOL multiAdapterSupport;

	// Check [3] D3D12_FEATURE_DATA_FEATURE_LEVELS
	UINT featureLevelsHI = 0;
	UINT featureLevelsLO = 0;

	// Check[4] D3D12_FEATURE_DATA_FORMAT_SUPPORT
	bool Formats[115] = { 0 };
#endif
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

	UINT	ShaderVersionH = 0, ShaderVersionL = 0;	// Basics of Refrash rate / Shaver Version:
	bool	RenderfirstTime = true;

	// Video Card Capabilities:
	// ----------------------------------------------------------------------------
	Capabilities m_sCapabilities;

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

};

#define msaaSamples (ReqMSAAquality - 1)	// MSAA

