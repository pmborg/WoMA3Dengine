// --------------------------------------------------------------------------------------------
// Filename: womadriverclass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234526920;

#pragma once
#ifndef womadriverclass_h
#define womadriverclass_h

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


#define PASS_OPAC			0 // used by DX11 & VK
#define PASS_TRANSPARENT	1 // used by DX11 & VK
#define PASS_SHADOWS		2 // used by DX11 & VK
#define PASS_MAINMAP		3 // MAIN MAP (VK)
#define PASS_MINIMAP		4 // MINI MAP (VK)
#define PASS_BILL			5 // lvl70: For Billboards (like trees, grass, etc.) (DX11 & VK)


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

#include "vertexTypes.h"	
#include "virtualCameraClass.h"

#if defined USE_FRUSTUM 
#if defined WINDOWS_PLATFORM
#include "DXfrustumClass.h"
#define FrustumClass DXfrustumClass
#else
#include "GLfrustumClass.h"
#define FrustumClass GLfrustumClass
#endif
#endif

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG && defined DX11
#include "ImageLoaderClass.h"
#endif

#define initLoadTexture3D(model, texture, vertexVector, IndexList, shader_type)\
{\
    std::vector<STRING> Textures;\
    Textures.push_back(texture);\
\
    CREATE_MODEL_IF_NOT_EXCEPTION(model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);\
\
    if (shader_type == SHADER_TEXTURE)\
        ASSERT(model->LoadTexture(pContext, texture, m_Driver, shader_type, &Textures, &vertexVector, &IndexList));\
}

#define initModelwithTexture2D(model, texture, vertexVector, IndexList, shader_type)\
{\
    std::vector<STRING> Textures;\
    Textures.push_back(texture);\
\
    CREATE_MODEL_IF_NOT_EXCEPTION(model, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);\
\
    ASSERT(model->LoadTexture(pContext, texture, m_Driver, shader_type, &Textures, &vertexVector, &IndexList));\
}

#define initModelwithTexture2DMAP(model, texture, vertexVector, IndexList, shader_type, alfa)\
{\
    std::vector<STRING> Textures;\
    Textures.push_back(texture);\
\
    CREATE_MODEL_IF_NOT_EXCEPTION(model, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);\
\
    model->ModelHASAlfaColor = true;\
    model->ModelAlfaColor = alfa;\
\
    ASSERT(model->LoadTexture(pContext, texture, m_Driver, shader_type, &Textures, &vertexVector, &IndexList));\
}

#define initLoadTextureLight3D(model, texture, vertexVector, IndexList, shader_type)\
{\
    std::vector<STRING> Textures;\
    Textures.push_back(texture);\
\
    CREATE_MODEL_IF_NOT_EXCEPTION(model, I_AM_3D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);\
\
    if (shader_type == SHADER_TEXTURE_LIGHT)\
        ASSERT(model->LoadLight(pContext, texture, m_Driver, shader_type, &Textures, &vertexVector, &IndexList));\
}

#define initLoadTextureLight2D(model, texture, vertexVector, IndexList, shader_type)\
{\
    std::vector<STRING> Textures;\
    Textures.push_back(texture);\
\
    CREATE_MODEL_IF_NOT_EXCEPTION(model, I_AM_2D, I_HAVE_NO_SHADOWS, I_HAVE_NO_SHADOWS);\
\
    if (shader_type == SHADER_TEXTURE_LIGHT)\
        ASSERT(model->LoadLight(pContext, texture, m_Driver, shader_type, &Textures, &vertexVector, &IndexList));\
}

// ----------------------------------------------------------------------------------------------
enum CAMERA_TYPE
{
	CAMERA_NORMAL = 0,			// PLAYER CAMERA
	CAMERA_SKY,					// FIXED CAMERA AT 0,0,0
	CAMERA_MINIMAP
};

enum PROJECTION_TYPE
{
	PROJECTION_PERSPECTIVE = 0, // 3D
	PROJECTION_ORTHOGRAPH,		// 2D
	PROJECTION_MINIMAP			// Render to MAP&Mini Texture
};


// --------------------------------
// Engine | Focus 
// Level  | 
// --------------------------------
// 21–29	DX11 Foundations
// 30–34	Material Evolution
// 35–41	Shadow Era
// 53–61	Terrain Era
// 72–99	Advanced Effects
// 82–93	Animated Mesh
// 94-	    Billboard Atlas
// 97-	    Particles waterfall & smoke
// 102+		Unified Constant Buffer

// -----------------------------------------------------------------------------
// SHADER ABI RULE
// -----------------------------------------------------------------------------
// Each SHADER_TYPE owns a unique shader pair (.hlsl/.spv) and a fixed resource
// layout (Descriptor Sets, UBOs, Push Constants and Samplers).
//
// If a shader requires a new mandatory binding or a different ABI, create a
// new SHADER_TYPE instead of extending an existing one.
// -----------------------------------------------------------------------------
enum SHADER_TYPE
{
	SHADER_AUTO = 0,
	//      ------------------------------------------------------------------------------------------------------------------
	/*1*/	SHADER_COLOR,								//021Color.hlsl			: W3D v1.0	public MAIN (Used by: 21 & Sun Ray & 3D Obj)
	/*2*/	SHADER_TEXTURE,								//022Texture.hlsl		: W3D v1.1	public MAIN (Used by: 22 & Banner & Sky2D & SplashIntro & UnderWater & Font & 3D Obj)

	// Use Material Light
	/*3*/	SHADER_TEXTURE_FONT,						//027Texture.hlsl		: Fade: using alfa-color (Before Light)

	/*4*/	SHADER_TEXTURE_LIGHT,						//023Light.hlsl			: Base Light used on W3D v1.2
	//033Light.hlsl			: Same ABI with Alpha Color only
//      ------------------------------------------------------------------------------------------------------------------
/*5*/	SHADER_NORMAL_BUMP,							//035TextureBump.hlsl	: W3D v1.3	public MAIN (Used by: 47 & 3D Obj)

// Use Global: Light
/*6*/	SHADER_TEXTURE_LIGHT_RENDERSHADOW,			//036LightRenderShadow.hlsl: Render Shadows
/*7*/	SHADER_TEXTURE_LIGHT_SAVESHADOW,			//036ShadowMap.hlsl        : Aux. Shader (render in texture)

/*8*/	SHADER_TEXTURE_LIGHT_ALPHAMAP,				//034Light.hlsl			   : SPECULAR SHININESS + ALFA MAP (Used by: 34 & 3D Obj)

//      ------------------------------------------------------------------------------------------------------------------
		// Have to be last (after basic shaders):
	SHADER_TEXTURE_LIGHT_INSTANCED,				//040LightInstance.hlsl - like 23 but INSTANCED
	SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED,	//041ShadowMapInstance.hlsl - Aux. Shader (render in texture), but using Instances (used on 41,42)
	SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED,	//using: 036LightRenderShadow.hlsl but use the INSTANCED shadows
	SHADER_NORMAL_BUMP_INSTANCED,				//---099 INSTANCED - Reserved (WorldOfMiddleAge roadmap). Engine Level reserved; implementation deferred.
	SHADER_TEXTURE_WATER,						//054Texture.hlsl - Water waves

	//      ------------------------------------------------------------------------------------------------------------------
			// TERRAINS: (after instance shaders)
	SHADER_Double_Color_Terrain,				//053Double_Color_Terrain.hlsl
	SHADER_Slope_Texture_Terrain,				//055Slope_Texture_Terrain.hlsl
	SHADER_Terrain_Texture_DEMO19,				//056Slope_Texture_Terrain.hlsl TUTORIAL_CHAP >= 19 // TERRAIN
	SHADER_Terrain_Texture_DEMO21,				//---057 TUTORIAL_CHAP >= 21 // TERRAIN - Reserved (WorldOfMiddleAge roadmap). Engine Level reserved; implementation deferred.
	SHADER_Terrain_Texture_DEMO22,				//---058 TUTORIAL_CHAP >= 22 // TERRAIN - Reserved (WorldOfMiddleAge roadmap). Engine Level reserved; implementation deferred.
	SHADER_Terrain_Texture_DEMO23,				//---059 TUTORIAL_CHAP >= 23 // TERRAIN - Reserved (WorldOfMiddleAge roadmap). Engine Level reserved; implementation deferred.	
	SHADER_Terrain_Texture_DEMO60,				//060Terrain.hlsl
	SHADER_Terrain_Texture_DEMO61,				//061Terrain.hlsl MY_SOURCE_CODE\WorldOfMiddleAge\WoMA_PartII\engine\024terrain_fog_slope_detail_mapping.hlsl

	//      ------------------------------------------------------------------------------------------------------------------
	SHADER_FIRE,								//072fire.hlsl
	SHADER_TEXTURE_GS_INSTANCED,				//077GSinstance.hlsl

	SHADER_MESH,								//080 & 081 Only (The advanced SHADER mesh is not listed here, once is in PGP)

	SHADER_TEXTURE_LIGHT_FAST,					//083Light.hlsl
	/*25*/	SHADER_BILLBOARD_ATLAS_FAST,				//093BillboardAtlas.hlsl

	SHADER_USE_CURVED_SKY_PLANE,				//---095 - Reserved (WorldOfMiddleAge roadmap). Engine Level reserved; implementation deferred.
	SHADER_USE_CURVED_REAL_SKY_PLANE,			//096curved_real_sky_plane.hlsl

	SHADER_USE_WATERFALL,						//097waterfall.hlsl
	SHADER_USE_SMOKE,							//097smoke.hlsl

	SHADER_TEXTURE_POINTS_OF_LIGHT_INSTANCED,	//098LightInstance.hlsl
	/*31*/	SHADER_TEXTURE_LIGHT98,				//098Light.hlsl

	SHADER_Terrain_Texture_DEMO99,				//099Terrain.hlsl

	//      ------------------------------------------------------------------------------------------------------------------
};

struct Capabilities
{
	// DX Capabilities:
	bool CapDX9 = false;
	bool CapDX10_11 = false;
	bool CapDX12 = false;

	STRING SHADER_TYPE_NAME = TEXT("");

	BOOL   inStereoAdapterMode = false;

	size_t nTotalAvailableGPUMemory;		//< Total available GPU memory in kilobytes, 0 if it was not possible to determine this value, this value may not match your graphics card specification (e.g. "512 MiB" may get you "480 MiB" in here)

	bool   USE_DXDRIVER_FONTSBoolean = false;

	bool   DXGI10 = false;
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
	BOOL StandardSwizzle64KBSupported;
	BOOL CrossAdapterRowMajorTextureSupported;
	BOOL VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation;
	UINT CrossNodeSharingTier;

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
	virtual void Initialize2Dand3DCamera() = 0;
	virtual bool Initialize(float* clearColor, void* pContext = NULL) = 0;
	virtual void FinalizeInitialization() = 0;

	virtual bool OnInit(int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits,
		float screenDepth, float screenNear, BOOL msaa, bool vsync,
		BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize) = 0;
	virtual void Shutdown() = 0;
	virtual void Shutdown2D() = 0;

	virtual bool Resize(UINT USE_MONITOR, int screenWidth, int screenHeight, float screenNear, float screenDepth, BOOL fullscreen, UINT depthBits) = 0;

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
	bool	Pending2DResize = false;

	bool RenderMapfirstTime = true;

#if defined USE_ALPHA_BLENDING
	float blendFactor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	bool g_AlphaBlend = false;
#endif
	bool g_Zbuffer = true; //DEFAULT: true

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
	FrustumClass* frustum = NULL;
#endif

	std::vector<STRING> g_allTextureNameArray;
	std::vector<void*>	g_allTexturePointerArray; //GLtextureClass*
};

#endif
