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
#pragma once

//#include "dx_engine_level.h"
#include "main.h"

//#include "main.h" // TCHAR
// Define the Rendering States for textures:
#define CULL_FRONT	0
#define CULL_BACK	1
#define CULL_NONE	2

#define FILL_SOLID	0
#define FILL_WIRE	1

//#if defined DX12 || defined DX11 || defined DX9
#include <d3d11.h>
#if (D3D11_SPEC_DATE_YEAR > 2009)
	#define SUPPORT_TGA
#endif
//#endif

#if defined USE_FRUSTRUM
	#include "DXfrustumClass.h"

	#define FrustumClass DXfrustumClass
#endif


#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	#include "ImageLoaderClass.h"
#endif

// ----------------------------------------------------------------------------------------------
enum CAMERA_TYPE
{
	CAMERA_NORMAL = 0, 
	CAMERA_SKY
};

enum PROJECTION_TYPE
{
	PROJECTION_PERSPECTIVE = 0, 
	PROJECTION_ORTHOGRAPH
};

enum SHADER_TYPE
{
	SHADER_AUTO = -1, 
	SHADER_COLOR,						//21: M3D v1.0	public MAIN (Used by: 21 & Sun Ray & 3D Obj)
	SHADER_TEXTURE,						//22: M3D v1.1	public MAIN (Used by: 22 & Banner & Sky2D & SplashIntro & UnderWater & Font & 3D Obj)
	SHADER_TEXTURE_FONT,				//27:
	SHADER_TEXTURE_LIGHT,				//23: M3D v1.2	public MAIN + Pass2: Shadows (Used by: 23 & Sky3D & Sun & Moon & 3D Obj)
	SHADER_TEXTURE_LIGHT_RENDERSHADOW,  //45:			public "Render Shadows"
	SHADER_TEXTURE_LIGHT_CASTSHADOW,	//45:			private "Pass1: Shadows" (add auxiliar Shader)
	SHADER_NORMAL_BUMP,					//47: M3D v1.5	public MAIN (Used by: 47 & 3D Obj)

	// Have to be last:
	SHADER_TEXTURE_LIGHT_INSTANCED,				//public   51: INST 23 light with Instances    (pass2) 
	SHADER_TEXTURE_LIGHT_CASTSHADOW_INSTANCED,	//private  51: INST 45 shadows with Instances  (pass1) 
	SHADER_NORMAL_BUMP_INSTANCED,				//private  51: INST 47 bump with Instances ... is working!?!? check: 051LightInstance.hlsl

	// TERRAINS:
	SHADER_Double_Color_Terrain,
	SHADER_Double_Color_Slop_Terrain,
	SHADER_Double_Color_Slop_Detail_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMapping_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMappingDouble_Terrain,
	SHADER_Double_Color_Slop_Detail_TexMappingDouble_Bump_Terrain,

	//MY_SHADER_SKYTEXTURE,		//90:
	//MY_SHADER_REALSKYTEXTURE,	//91:
};


struct Capabilities 
{
	// DX Capabilities:
	bool CapDX9 = false;
	bool CapDX10_11 = false;
	bool CapDX12 = false;
	bool CapDX12_1 = false;
	STRING SHADER_TYPE_NAME;

	BOOL   inStereoAdapterMode;

	size_t nTotalAvailableGPUMemory;		/**< Total available GPU memory in kilobytes, 0 if it was not possible to determine this value, this value may not match your graphics card specification (e.g. "512 MiB" may get you "480 MiB" in here) */
	UINT   SelectedDriverType;
	bool   USE_DXDRIVER_FONTSBoolean;

	bool   DXGI10;
	bool   DXGI11;
	bool   DXGI12;

	BOOL   MSAA_SUPPORTBoolean;		// MSAA Available?
	UINT   MSAAmultiSampleCount;	// HW: Max.Available
	UINT   MSAAquality;				// HW: Max.Available

	// ----------------------------------------------------------------------------
	// INSPIRED:	EXTRA\_TO LEARN FREE\angle-chromium_2423\src\libANGLE\renderer\d3d\d3d11\renderer11_utils.cpp
	//				EXTRA\_TO LEARN FREE\angle - chromium_2423\src\libANGLE\renderer\d3d\d3d9\renderer9_utils.cpp
	//				EXTRA\_TO LEARN FREE\angle - chromium_2423\src\libANGLE\renderer\gl\renderergl_utils.cpp

	float  maxTextureAnisotropy;
	bool   occlusionQueryBoolean;
	bool   instancedArraysBoolean;
	bool   framebufferMultisampleBoolean;
	bool   shaderTextureLODBoolean;

	size_t MaximumSimultaneousRenderTargets;

	size_t max2DTextureSize;
	size_t maxArrayTextureLayers;

	size_t max3DTextureSize;
	size_t maxCubeMapTextureSize;

	size_t maxViewportWidth;
	size_t maxViewportHeight;

	size_t maxElementsIndices;
	size_t maxElementsVertices;

	size_t maxVertexAttributes;
	size_t MaximumConstantBufferSize;

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
	UINT featureLevelsHI;
	UINT featureLevelsLO;

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

#if defined USE_RASTERIZER_STATE
	virtual void SetRasterizerState(UINT cullMode, UINT fillMode) = 0;
#endif
	virtual void TurnZBufferOn() = 0;
	virtual void TurnZBufferOff() = 0;

	#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	virtual ImageLoaderClass* CaptureScreenShot(int screenWidth, int screenHeight) = 0;
	#endif

	UINT	ShaderVersionH, ShaderVersionL;	// Basics of Refrash rate / Shaver Version:
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
	int		m_videoCardMemory;
	TCHAR	adapterDesc_Description[MAX_STR_LEN];	// Note: have to be wstring
	UINT	ufreededicatedVideoMem;

	// List of resoltions availabel to Use
	// ----------------------------------------------------------------------------
	UINT numerator, denominator;
	UINT MonitorNumber;					// Total number of Monitors
	UINT numModes;						// Number ResolutionModes

	BOOL	m_VSYNC_ENABLED;
	float	ClearColor[4];

	// MSAA Used:
	// ----------------------------------------------------------------------------
	UINT	ReqMSAAmultiSampleCount;	// Req. to Use
	UINT	ReqMSAAquality;				// Req. to Use

#if defined USE_FRUSTRUM
	FrustumClass* frustum;
#endif

private:
	
};

#define msaaSamples (ReqMSAAquality - 1)	// MSAA:

