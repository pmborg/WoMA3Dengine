// --------------------------------------------------------------------------------------------
// Filename: womadriverclass.h
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

#pragma once

#include "main.h"

#if defined LINUX_PLATFORM
#include <EGL/egl.h>
#include <EGL/eglext.h>

#if defined GLES2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#elif defined GLES3
#include <GLES3/gl3.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#endif

// Define the Rendering States for textures:
#define CULL_FRONT	0
#define CULL_BACK	1
#define CULL_NONE	2

#define FILL_SOLID	0
#define FILL_WIRE	1

#if defined (WINDOWS_PLATFORM) && defined DX_ENGINE
#include <d3d11.h>
#endif
#if (D3D11_SPEC_DATE_YEAR > 2009) || !defined (WINDOWS_PLATFORM)
	#define SUPPORT_TGA
#endif

#if defined USE_FRUSTUM // frustum
	#if defined DX11 || defined DX9 || defined DX12
	#include "DXfrustumClass.h"

	#define FrustumClass DXfrustumClass
	#endif
#endif

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
	#include "ImageLoaderClass.h"
#endif

// ----------------------------------------------------------------------------------------------
enum CAMERA_TYPE
{
	CAMERA_NORMAL = 0,			// PLAYER CAMERA
	CAMERA_SKY,					// FIXED CAMERA AT 0,0,0
	CAMERA_MINIMAP
};

enum PROJECTION_TYPE
{
	PROJECTION_PERSPECTIVE = 0, //3D
	PROJECTION_ORTHOGRAPH,		//2D
	PROJECTION_MINIMAP
};

enum SHADER_TYPE
{
		SHADER_AUTO = 0, 
/*1*/	SHADER_COLOR,								//021 Color.hlsl		: W3D v1.0	public MAIN (Used by: 21 & Sun Ray & 3D Obj)
/*2*/	SHADER_TEXTURE,								//022 Texture.hlsl		: W3D v1.1	public MAIN (Used by: 22 & Banner & Sky2D & SplashIntro & UnderWater & Font & 3D Obj)

		// Use Material Light
/*3*/	SHADER_TEXTURE_FONT,						//027 Texture.hlsl		: fade: using alfa-color
/*4*/	SHADER_TEXTURE_LIGHT,						//023 Light.hlsl		: W3D v1.2	public MAIN + Pass2: Shadows (Used by: 23 & Sky3D & Sun & Moon & 3D Obj)

// ---- LEVEL >= 30
/*5*/	SHADER_NORMAL_BUMP,							//035 TextureBump.hlsl	: W3D v1.3	public MAIN (Used by: 47 & 3D Obj)

		// Use Global: Light
/*6*/	SHADER_TEXTURE_LIGHT_RENDERSHADOW,			//036 LightRenderShadow.hlsl: Render Shadows
/*7*/	SHADER_TEXTURE_LIGHT_SAVESHADOW,			//036 ShadowMap.hlsl        : Aux. Shader (render in texture)

		// Have to be last:
		SHADER_TEXTURE_LIGHT_INSTANCED,				//040 LightInstance.hlsl: like 23 but INSTANCED
		SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED,	//040 Aux. Shader (render in texture), but using Instances (used on 41,42)
		SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED,	//041 ShadowMapInstance.hlsl: like 36 but INSTANCED shadow
		SHADER_NORMAL_BUMP_INSTANCED,				//099 INSTANCED like 35 bump, but using Instances
		SHADER_TEXTURE_WATER,						//054 Water waves - hlsl\054Texture.hlsl

		// TERRAINS:
		SHADER_Double_Color_Terrain,				//053 Double_Color_Terrain.hlsl
		SHADER_Slope_Texture_Terrain,				//055 Slope_Texture_Terrain.hlsl
		SHADER_Terrain_Texture_DEMO19,				//056 TUTORIAL_CHAP >= 19 // TERRAIN
		SHADER_Terrain_Texture_DEMO21,				//057 TUTORIAL_CHAP >= 21 // TERRAIN
		SHADER_Terrain_Texture_DEMO22,				//058 TUTORIAL_CHAP >= 22 // TERRAIN
		SHADER_Terrain_Texture_DEMO23,				//059 TUTORIAL_CHAP >= 23 // TERRAIN
		SHADER_Terrain_Texture_DEMO60,				//060 TUTORIAL_CHAP >= 24 // TERRAIN
		SHADER_Terrain_Texture_DEMO61,				//061 MY_SOURCE_CODE\WorldOfMiddleAge\WoMA_PartII\engine\024terrain_fog_slope_detail_mapping.hlsl

		SHADER_FIRE,								//72
		SHADER_TEXTURE_GS_INSTANCED,				//77

		SHADER_MESH,								//80 & 81 Only

        SHADER_TEXTURE_LIGHT_FAST,					//83
/*25*/	SHADER_BILLBOARD_ATLAS_FAST,				//93

		SHADER_USE_CURVED_SKY_PLANE,				//95
		SHADER_USE_CURVED_REAL_SKY_PLANE,			//96

		SHADER_USE_WATERFALL,						//97
		SHADER_USE_SMOKE,							//98
		SHADER_TEXTURE_POINTS_OF_LIGHT_INSTANCED,	//98
/*31*/	SHADER_TEXTURE_LIGHT98,						//98

		SHADER_Terrain_Texture_DEMO99,				//99


};

struct Capabilities 
{
	// DX Capabilities:
	bool CapDX9 = false;
	bool CapDX10_11 = false;
	bool CapDX12 = false;

	STRING SHADER_TYPE_NAME=TEXT("");

	BOOL   inStereoAdapterMode=false;

	size_t nTotalAvailableGPUMemory;		//< Total available GPU memory in kilobytes, 0 if it was not possible to determine this value, this value may not match your graphics card specification (e.g. "512 MiB" may get you "480 MiB" in here)

	bool   USE_DXDRIVER_FONTSBoolean = false;

	bool   DXGI10=false;
	bool   DXGI11 = false;
	bool   DXGI12 = false;

	BOOL   MSAA_SUPPORTBoolean = false;	// MSAA Available?
	UINT   MSAAmultiSampleCount = 0;	// HW: Max.Available
	UINT   MSAAquality = 0;				// HW: Max.Available

	// ----------------------------------------------------------------------------
	bool	ComputeShadersSupported = false;
	float	maxTextureAnisotropy = 0;
	bool	occlusionQueryBoolean = false;
	bool	instancedArraysBoolean = false;
	bool	framebufferMultisampleBoolean = false;
	bool	shaderTextureLODBoolean = false;

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
    UINT WomaIntegrityCheck = 1234525217;

	virtual void Initialize2Dand3DCamera()=0;
	virtual bool Initialize(float* clearColor)=0;
	virtual void FinalizeInitialization()=0;

    virtual bool OnInit(int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits,
        float screenDepth, float screenNear, BOOL msaa, bool vsync,
        BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize) = 0;
	virtual void Shutdown()=0;
	virtual void Shutdown2D()=0;

	virtual void BeginScene(UINT monitorWindow) = 0;
	virtual void EndScene(UINT monitorWindow) = 0;
	virtual void ClearDepthBuffer(void* pContext) = 0;

#if defined USE_RASTERIZER_STATE
	virtual void SetRasterizerState(void* pContext, UINT cullMode, UINT fillMode) = 0;
#endif
	virtual void TurnZBufferOn(void* pContext) = 0;
	virtual void TurnZBufferOff(void* pContext) = 0;

	#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
	virtual void TurnOnAlphaBlending(void* pContext) = 0;
	virtual void TurnOffAlphaBlending(void* pContext) = 0;
	#endif

	#if defined USE_DX_DRIVER_FONT
	virtual void addText(int Xpos, int Ypos, TCHAR* text, float R, float G, float B) = 0;
	virtual void RenderDriverText(void* pContext) = 0;
	#endif

	#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	virtual ImageLoaderClass* CaptureScreenShot(int screenWidth, int screenHeight) = 0;
	#endif

	// Video Card Capabilities:
	// ----------------------------------------------------------------------------
	Capabilities m_sCapabilities;

	UINT	ShaderVersionH = 0, ShaderVersionL = 0;	// Basics of Refresh rate / Shaver Version:
	bool	RenderfirstTime = true;

#if defined USE_ALPHA_BLENDING
	float blendFactor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	bool g_AlphaBlend = false;
#endif
	bool g_Zbuffer = false;

	// Video Card Info:
	// ----------------------------------------------------------------------------
	//UINT use_this_graphic_card_adapter = 0;

	TCHAR driverName[MAX_STR_LEN];			// STRING driverName;
	TCHAR ShaderModel[MAX_STR_LEN];			// STRING ShaderModel;   // "x_y"
	TCHAR szShaderModel[MAX_STR_LEN];		// STRING szShaderModel; // "x.y"
	// ------------------
	
	CHAR	m_videoCardDescription[128];	//NEED TO BE CHAR
	int		m_videoCardMemory = 0;
	TCHAR	adapterDesc_Description[MAX_STR_LEN];
	UINT	ufreededicatedVideoMem = 0;

	// List of resolutions available to Use
	// ----------------------------------------------------------------------------
	UINT numerator, denominator = 0;
	UINT MonitorNumber = 0;					// Total number of Monitors
	UINT numModes = 0;						// Number ResolutionModes

	BOOL	m_VSYNC_ENABLED = false;
	float	driver_ClearColor[4] = {};

	// MSAA Used:
	// ----------------------------------------------------------------------------
	UINT	MSAA_COUNT = 1;					//Anti-Aliasing: MultiSample tech. (1 = off, 4, 8, 16)		4	8	16
	UINT	MSAA_QUALITY = 0;				//Anti-Aliasing: Texture Filtering tech. (MSAA > 0)			0	0	 0

#if defined USE_FRUSTUM
	FrustumClass* frustum=NULL;
#endif
};

