// --------------------------------------------------------------------------------------------
// Filename: tech-level.h
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
//WomaIntegrityCheck = 1234526920;

#pragma once
#pragma warning(push)
#pragma warning( disable : 4005 )       // Disable warning C4005: '' : macro redefinition

#define WOMAENGINE
#if defined NewWomaEngine
    #define WOMAENGINE_BASIC
#endif

#if CORE_ENGINE_LEVEL < 10 && defined WINDOWS_PLATFORM
	#define USE_NETWORK
#endif

// ----------------------------------------------------------------------------------------------------------------
// OS COMMON LEVELS: Start at 0 and end at 9.
//
// Platform-independent engine foundations shared by all render backends.
// ----------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// CORE_ENGINE_LEVEL >= 0
//------------------------------------------------------------------------------------------------------------
#if defined NDEBUG
    #define USE_NETWORK
#else
    #if CORE_ENGINE_LEVEL > 9 && (defined _DEBUG || defined DEBUG)
        #define USE_FORCE_HOUR  /*24*/ /*18*/ 12
        #define USE_FORCE_MIN   00
    #endif
#endif

#if defined _DEBUG
    //#define VERBOSE_MEMORY_DEBUG          // Default: off - [ON/OFF]: used (woma_macros.h)
#endif

//------------------------------------------------------------------------------------------------------------

#if CORE_ENGINE_LEVEL >= 1
    #define USE_LOG_MANAGER         //1
#endif

#if CORE_ENGINE_LEVEL >= 2 && !defined WOMAENGINE_BASIC
	//"First Window with mouse click: "
    //#define WOMA_SKIP_CREATE_MAIN_WINDOW  // Default: off - for command line applications only!
#endif

#if CORE_ENGINE_LEVEL >= 3
    #if defined WINDOWS_PLATFORM
    #define USE_WOMA_EXCEPTION      //3
    //#define USE_MINIDUMPER          //3
    #endif
#endif

#if CORE_ENGINE_LEVEL >= 4
    #define USE_USER_SETUP          //4
    #if !defined ANDROID_PLATFORM
        #define USE_PROCESS_OS_KEYS //4 - NOTE: Mandatory at 4
        #define USE_SYSTEM_CHECK    //4
    #endif
    #define USE_CHECK_IO            //4
    #define USE_TIMER_CLASS         //4
    #endif

#if CORE_ENGINE_LEVEL >= 5
    //#if !defined ANDROID_PLATFORM
    #define USE_TINYXML_LOADER      //5
    //#endif
    #if defined WINDOWS_PLATFORM
    #define CLIENT_SCENE_SETUP      //5 (PAINT+settings.xml)
    #endif
    #if defined WINDOWS_PLATFORM
        #define ALLOW_LOADING_SPLASH        //7 
    #endif
#endif

#if CORE_ENGINE_LEVEL >= 6 && defined WINDOWS_PLATFORM
    #define USE_TIMER           //6 Calculate FPS & dT for animations
    #define USE_IDEA_PACK       //6
#endif

#if CORE_ENGINE_LEVEL >= 7 && defined WINDOWS_PLATFORM
    #define USE_ASTRO_CLASS                 //7 CoreEngine_007 (PAINT RealTimeCelestial)        
    #if CORE_ENGINE_LEVEL == 7 || DX_ENGINE_LEVEL == 25 || DX_ENGINE_LEVEL == 29
    #define USE_NETWORK
    #endif
    #if defined WINDOWS_PLATFORM
       #define USE_ALLOW_MAINWINDOW_RESIZE //7 Allow main Window to Resize
    #endif
#endif

#if CORE_ENGINE_LEVEL >= 8 && defined WINDOWS_PLATFORM
    #define USE_GPS_MAP         //8 CoreEngine_008 (PAINT RealTimeMapGPSPosition)   
    #if CORE_ENGINE_LEVEL == 8
        #define USE_NETWORK     //8
    #endif
#endif

#if CORE_ENGINE_LEVEL >= 9 && defined WINDOWS_PLATFORM
    #define USE_METARCLASS      //9 CoreEngine_009 (PAINT RealTimeWeather) TODO: Android read asset file
    #if CORE_ENGINE_LEVEL >= 9
        #if !defined MAINENGINE || CORE_ENGINE_LEVEL == 9
            #define USE_NETWORK //9
        #endif
    #endif
    //#define GENERATE // Default: off - (Airports List)
#endif

// ----------------------------------------------------------------------------------------------------------------
// BASIC SERIES: Start at 19 and end at 29.
//
// The early levels may look deceptively simple, but they are cumulative engine milestones.
// Each tutorial introduces exactly one new capability while preserving everything that came before it.
// Together they build the foundations of a complete cross-platform 3D engine.
// ----------------------------------------------------------------------------------------------------------------
 
// -----------------------------------------------------------------------------
// Rendering Backend Selection
//
// Multiple rendering backends may be compiled simultaneously.
// The active renderer is selected at runtime from settings.xml
// or from the Setup dialog.
//
// Supported backends:
//   - DX9 (implemented through the DX11 runtime)
//   - DX11
//   - DX12
//   - OpenGL 3.x
//   - Vulkan
//
// Define VULKAN_ONLY to build a minimal Vulkan-only executable.
// -----------------------------------------------------------------------------

//#if DX_ENGINE_LEVEL == 19
//#define VULKAN_ONLY		//DEBUG A LEVEL ONLY!
//#endif

//
//WOMA DEFINES FOR DRIVERS:
// 
//DX_ENGINE
//DX9sdk	// Original DX9 original Driver
//DX9		// DX9 feature set over the DX11 runtime
//DX11
//DX12
//
//OPENGL3
//OPENGL40
//
//GLES2
//GLES3
//
//VULKAN

//-------------------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 10
//-------------------------------------------------------------------------------------------------------
    #if defined _MSC_VER    //DX9 DX11+DX10 DX12 OPENGL3
	  #if !defined VULKAN_ONLY
			//#define DX9sdk        //(ORIGINAL DX9SDK) not fully recovered/implemented

			#define DX9             //Using DX11 API
			#define DX11
			#define DX12            //(defined DX12 && D3D11_SPEC_DATE_YEAR > 2009)
		#else
			#undef DX9				// DX9 feature set over the DX11 runtime
			#undef DX11
			#undef DX12
			#undef DX_ENGINE
			#undef OPENGL3
			#undef OPENGL40
			#undef GLES2
			#undef GLES3

			#define D3D11_SDK_VERSION 0
		#endif
	#endif

	#define VULKAN

    #ifndef VULKAN_ONLY
      #define OPENGL3 //WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM

	  #if defined WINDOWS_PLATFORM && (defined DX9sdk || defined DX9 || defined DX11 || defined DX12)
			#define DX_ENGINE       //Turn on/off: #include "DXengine.h"
	  #endif
	#endif

    #ifndef DX_ENGINE
		#undef DX9
		#undef DX11
		#undef DX12
    #endif
    
    #if defined DX12 //SELECT DXGI_API version:
      //#define DX12_DXGI_API 3   // Default: off - IDXGIFactory3
      //#define DX12_DXGI_API 4   // Default: off - IDXGIFactory4
      //#define DX12_DXGI_API 5   // Default: off - IDXGIFactory5
		#define DX12_DXGI_API 6     //IDXGIFactory6
    #endif

//--------------------------------------------------------------------------------------------------------
//WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM
//--------------------------------------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 11
        #undef CLIENT_SCENE_SETUP // For fps benchmark only!
    #endif

	#if DX_ENGINE_LEVEL >= 12 &&  DX_ENGINE_LEVEL <= 18
		#error "LEVEL RESERVED"
	#endif

	// -----------------------------------------------------------------------------
	// LEVEL19 - Cross-Platform Regression Baseline (WoMA 2026)
	// -----------------------------------------------------------------------------
	// Based on:
	//     Microsoft DirectX-Graphics-Samples (D3D12HelloWindow)
	//     Synchronized with the latest Microsoft implementation (2026).
	//
	//		Reference Benchmark (Empty Scene)
	//		ClearColor + Present only
	// 
	// STATUS:
	//     DX12 initialization pipeline verified and aligned with the current Microsoft DirectX samples.
	//			PERFORMANCE (Reference):
	//		    GPU: NVIDIA RTX 4080
	//		    Build: Release x64 AVX512
	//		    Resolution: 1500x900
	//
	// NOTES:
	//     - IDXGIFactory6 + EnumAdapterByGpuPreference()
	//     - High Performance GPU selection
	//     - D3D_FEATURE_LEVEL_12_2
	//     - DXGI video memory reporting
	// 
	// WoMA Driver Benchmarks(Reference)
	// 
	// Machine
	// -------
	// GPU        : NVIDIA RTX 4080
	// CPU		  : AMD Ryzen 9 7950X 16-Core Processor
	// Build	  : Release x64 AVX512
	// Resolution : 1500x900
	// 
	// Results
	// -------
	// DX11   : 12036 FPS
	// DX12   : 8623  FPS
	// Vulkan : 9410  FPS
	// OpenGL : 10142 FPS
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 19 && !defined WOMAENGINE_BASIC
	// -----------------------------------------------------------------------------
	// VULKAN1_1 - Frames-In-Flight synchronization (Khronos/Sascha Willems model)
	// Aug 2026
	// 
	// Implements the Khronos/Sascha Willems Frames-In-Flight synchronization model.
	// The current frame fence must be reset immediately before vkQueueSubmit().
	// Resetting it earlier (BeginScene) causes a deadlock when imagesInFlight
	// waits on a fence that has already been reset.
	//
	// Default: On
	// -----------------------------------------------------------------------------
		#define VULKAN1_1			// [ON/OFF] ON: Enable Vulkan 1.1 Frames-In-Flight synchronization / OFF: Use original Vulkan 1.0 synchronization (legacy compatibility)

        #undef USE_ALLOW_MAINWINDOW_RESIZE
        #define USE_NETWORK

        #if defined WINDOWS_PLATFORM
        #define CLIENT_SCENE_SETUP				// F6 - Key.
		#define WoMA_FIX_WINDOWS_GUI_SCALE_2026 // Win32 DPI fix for mixed-DPI multi-monitor setups (4K + FullHD).
        #endif

#if defined DX_ENGINE
        #define SET_DEVICE_CAPABILITIES		//Add info on: report.txt
#endif
#if defined DX12
		#define USE_DX12_PRESENT_DXGI_1_0	//for: DX12!
		#define USE_DX12_OLDWAY		//DX12 - [ON/OFF] ON: Default
	#if defined _DEBUG
		#define USE_DX12_DEBUG	//DX12 - [ON/OFF] OFF: Default
	#endif
#endif
    #endif


	// -----------------------------------------------------------------------------
	// LEVEL20 - Cross-Platform Regression Baseline (WoMA 2026)
	// -----------------------------------------------------------------------------
	// Window + Clear + Resize
	// → Blue Screen
	// → Resizable
    #if DX_ENGINE_LEVEL >= 20 && !defined WOMAENGINE_BASIC //settings.xml (UseAllMonitors="true")
	#define USE_ALLOW_MAINWINDOW_RESIZE
	#define USE_ALLOW_RESIZE
	#define USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE // Dep: USE_ALLOW_RESIZE
#if defined WINDOWS_PLATFORM

	#if defined _DEBUG
		#define USE_STATUSBAR
	#endif

	//#define USE_ASPECT_RATIO						// Default: off - Dep: USE_ALLOW_RESIZE
#endif
    
    #endif

	// -----------------------------------------------------------------------------
	// LEVEL21 - Cross-Platform Regression Baseline (WoMA 2026)
	// Basic 3D Color Rendering (Triangle + Square + Vertex/Index Buffers + Color Shader)
	// Reference Window Size: 500 x 1000 (Different benchmark from Level 19 empty-scene test.)
	// -----------------------------------------------------------------------------
	//
	// Windows
	//   DX11              : 11278 FPS
	//   DX12              :  8134 FPS
	//   OpenGL            :  8964 FPS
	//   Vulkan            :  8853 FPS
	//
	// Linux (WSL Debug)
	//   Vulkan            :   ~329 FPS (visual parity verified)
	//   OpenGL            :   ~749 FPS (visual parity verified)
	//
	// Android
	// - Galaxy A52        : Sustained ~520 FPS Vulkan 1.1 (visual parity verified)
	//
	// - Galaxy Z Fold7 (Adreno 830) Vulkan 1.3 
	//   Vulkan            : ~8200 FPS (~0.122 ms) (visual parity verified)
	//   OpenGL            :   321 FPS (~3.12 ms)  (visual parity verified)
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 21 && !defined WOMAENGINE_BASIC
		#if (defined DX12) || (defined VULKAN)
			#define USE_RASTERIZER_STATE // Required by DX12/Vulkan pipeline state
		#endif

        #if !defined ANDROID_PLATFORM
			#define GL_SETRASTERIZERSTATE
        #endif

        #define GL_CLEAR_DEPTH
        #define SCENE_COLOR
        #define ROTATE_SQUARE

        #define MAIN_RENDER

		#if defined ANDROID_PLATFORM
		#define SHOW_ANDROID_FPS			// Default: on
		#endif

        //#define USE_PRECOMPILED_SHADERS   // Default: off - [ON/OFF] ON: Merge Shader's Code on .EXE / OFF: Compile in run-time
		//#undef USE_NORMAL_WINDOW			// Default: off - [ON/OFF] ON: Normal Window / OFF: Borderless Window
    #endif


	// -----------------------------------------------------------------------------
	// LEVEL22 - Cross-Platform Texture Regression Baseline (WoMA 2026)
	// Textured Rendering (BMP, PNG, JPG, TGA, DDS, TIFF)
	//
	// Windows (Release)
	//   DX11     : 10308 FPS (~0.09 ms) 🥇
	//   DX12     :  8539 FPS (~0.12 ms) 🥈
	//   Vulkan   :  8418 FPS (~0.12 ms) 🥉
	//   OpenGL   :  7856 FPS (~0.13 ms)
	// 
	// Android (Release) 1968×2184: Galaxy Z Fold7 (Adreno 830, Vulkan 1.3)
	//   Vulkan  : Sustained:  ~3880 FPS (~0.25 ms) 📱
	//   OpenGL  : Sustained:    317 FPS (~3.1 ms) 📱
	// 
	// Linux 
	//   OpenGL (WSL Debug)      705 FPS (~1.4 ms) - visual parity verified
	//   Vulkan (WSL Debug)      320 FPS (~3.0 ms) - visual parity verified
	// 
	// Status
	//   ✓ SHADER_TEXTURE validated
	//   ✓ Descriptor Sets validated
	//   ✓ Sampler validated
	//   ✓ BMP / PNG / JPG / TGA / DDS / TIFF visual parity
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 22
        #define TEX_MODE_BORDER       // DX12 border sampler mode (Point + Border).
        #define SCENE_TEXTURE         // DEMO 22
        #define USE_IMAGE_BMP         // Mandatory base image format!
									  
		#undef SCENE_COLOR			  // Disable color scene for Level 22 (Texture Demo)
        //#define NO_SCENE_IMAGE_LOAD // Default: OFF - uncomment for DEBUG (debugging only!)

        #if !defined NO_SCENE_IMAGE_LOAD
            #if defined WINDOWS_PLATFORM
                #define USE_IMAGE_TIFF
            #endif

			#define USE_IMAGE_DDS            
            #define USE_IMAGE_PNG
            #define USE_IMAGE_JPG
            #define USE_IMAGE_TGA
        #endif
    #endif


	// -----------------------------------------------------------------------------
	// LEVEL23 - Cross-Platform 3D Math & MVP Transform Baseline (WoMA 2026)
	//
	// Vertex Transformation (MVP: World, View, Projection)
	// SHADER_TEXTURE_LIGHT
	// VS/PS Uniform Buffers + Descriptor Sets + Samplers
	// -----------------------------------------------------------------------------
	// 
	// Windows (Release) 1920x1080
	//   DX11    : 11095 FPS (~0.09 ms) 🥇
	//   Vulkan  :  8261 FPS (~0.12 ms) 🥈
	//   OpenGL  :  8105 FPS (~0.12 ms) 🥉
	//   DX12    :  6720 FPS (~0.15 ms)
	//
	// Android (Release) 1968×2184: Galaxy Z Fold7 (Adreno 830, Vulkan 1.3)
	//   Vulkan  : Sustained: 2590 FPS (~0.39 ms)
	//   Opengl  : Fixed:      648 FPS
	// 
	// Linux 
	//   OpenGL (WSL Debug)      764 FPS (~1.3 ms) - visual parity verified
	//   Vulkan (WSL Debug)      307 FPS (~3.2 ms) - visual parity verified
	// 
	// Status
	//   ✓ SHADER_TEXTURE_LIGHT validated
	//   ✓ VS Uniform Buffer validated
	//   ✓ PS Uniform Buffer validated
	//   ✓ Descriptor Sets (VS + PS + Sampler)
	//   ✓ Texture filtering comparison validated
	//   ✓ Large texture rendering validated
	//   ✓ Cross-platform visual parity verified
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 23 && !defined GOTO_BASICS
        #undef SCENE_TEXTURE
        #define SCENE_TEXTURE_LIGHT             
		//#define DEBUG_LOG_MAIN_RENDER_LIGHT_RAY

		#define MAIN_RENDER_LIGHT_RAY           // Default: on - [ON/OFF] - Render light ray NOTE! [Depend]: #undef  dx12_upload_old_way
        #define SUN_LIGHT_DEMO_ANIMATION true   // Default: on - [ON/OFF] - Animate the beam of light
    #endif


	// -----------------------------------------------------------------------------
	// LEVEL24 - Cross-Platform 2D Overlay & Alpha Blending Baseline (WoMA 2026)
	//
	// UI Overlay (Title Banner)
	// Alpha Blending
	// 2D Sprite Rendering
	// Depth Stencil integration (DX12)
	// -----------------------------------------------------------------------------
	//
	// Windows (Release) 1920x1080
	//   DX11    : 11138 FPS (~0.090 ms) 🥇
	//   OpenGL  :  8394 FPS (~0.119 ms) 🥈
	//   Vulkan  :  8230 FPS (~0.122 ms) 🥉
	//   DX12    :  6928 FPS (~0.144 ms)
	//
	// Android (Release) 1968×2184: Galaxy Z Fold7 (Adreno 830, Vulkan 1.3)
	//   Vulkan  : 8097 FPS (~0.123 ms)
	//   OpenGL  :  340 FPS (~2.94 ms)
	//
	// Linux (WSL Debug)
	//   Vulkan  : 315 FPS (~3.17 ms)
	//   OpenGL  : 755 FPS (~1.32 ms)
	//
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 24
		#undef DEBUG_LOG_MAIN_RENDER_LIGHT_RAY
        #if DX_ENGINE_LEVEL == 24
            #undef SCENE_COLOR
            #undef SCENE_TEXTURE
            #undef SCENE_TEXTURE_LIGHT
            #undef MAIN_RENDER_LIGHT_RAY
            //#define NO_SCENE_IMAGE_LOAD //(for debugging only!)
        #endif
        #define USE_TITLE_BANNER
        #define USE_ALPHA_BLENDING
        #define USE_VIEW2D_SPRITES
        #define USE_RASTERIZER_STATE
        #define USE_DSV					// DX12 Depth Stencil View support.
		#define SENTENCE_MAX_LEN	200 // Maximum characters per rendered sentence.
    #endif

	// -----------------------------------------------------------------------------
	// LEVEL25 - Native Windows Font Rendering Baseline (WoMA 2026)
	//
	// Native Driver Font Rendering (DX9/DX11/DX12)
	// Transition from sprite-based UI to native driver text rendering.
	//
	// Windows (Release)
	//   DX11 : 3009 FPS (~0.332 ms)
	//
	// Notes
	//   - DirectX native font rendering intentionally replaces the OpenGL/Vulkan
	//     tutorial path to isolate the Windows text pipeline.
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 25 //WINDOWS ONLY
        #if DX_ENGINE_LEVEL == 25
			// Disable previous rendering demos to isolate the native font tutorial.
            #undef SCENE_TEXTURE_LIGHT
			#undef MAIN_RENDER_LIGHT_RAY

			// Force DirectX-only rendering path for this tutorial.
			#undef GLES2
			#undef GLES3
			#undef VULKAN
			#undef OPENGL3
			#undef OPENGL40
        #endif

        #if defined DX_ENGINE
			#define USE_DX_DRIVER_FONT //WINDOWS NATIVE FONT: DX12 + DX11 + DX9
        #endif
    #endif

	// -----------------------------------------------------------------------------
	// LEVEL26 - Cross-Platform 3D Geometry & Directional Lighting Baseline (WoMA 2026)
	//
	// First Cubes + Spheres
	// Color Shader + Texture Shader + Light Shader
	// Animated Light Ray
	// -----------------------------------------------------------------------------
	//
	// Windows (Release) 1920x1080
	//   DX11    : 11266 FPS (~0.089 ms) 🥇
	//   Vulkan  :  8867 FPS (~0.113 ms) 🥈
	//   OpenGL  :  7417 FPS (~0.135 ms) 🥉
	//   DX12    :  6420 FPS (~0.156 ms)
	//
	// Android (Release) 1968×2184: Galaxy Z Fold7 (Adreno 830, Vulkan 1.3)
	//   Vulkan  : 3622 FPS (~0.276 ms) - visual parity verified
	//   OpenGL  :  261 FPS (~3.83 ms)  - visual parity verified
	//
	// Linux (WSL Debug)
	//   OpenGL  : 702 FPS (~1.43 ms) - visual parity verified
	//   Vulkan  : 119 FPS (~8.40 ms) - visual parity verified
	//
	// -----------------------------------------------------------------------------
	// Status
	//   ✓ Cube primitive validated
	//   ✓ Sphere primitive validated
	//   ✓ Directional lighting validated
	//   ✓ Debug Light Ray visual alignment
	//   ✓ Cross-platform primitive pipeline
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 26
		#if DX_ENGINE_LEVEL == 26
			// Disable previous scene demos.
			#undef SCENE_COLOR
			#undef SCENE_TEXTURE
			#undef SCENE_TEXTURE_LIGHT
		#endif

        #define USE_CUBE                    // Level 26
        #define USE_SPHERE                  // Level 26

		#undef USE_DX_DRIVER_FONT
		// Note:
		//   USE_DX_DRIVER_FONT is disabled again at Level 26 to keep the
		//   geometry benchmark independent from the native font pipeline.
    #endif

	// -----------------------------------------------------------------------------
	// LEVEL27 - Optimized dynamic sentence updates. (WoMA 2026)
	//
	// Text rendering now uploads only the required geometry(numLetters × 6) on 
	// Vulkan, OpenGL and DX11.DX12 preserves compatibility with the current UpdateSubresources() path while 
	// maintaining identical visual output.
	// -----------------------------------------------------------------------------
	//
	// Windows (Release) 1920x1080
	//   DX11    : ---- FPS () 🥇
	//   Vulkan  : ---- FPS () 🥈
	//   OpenGL  : ---- FPS () 🥉
	//   DX12    : ---- FPS ()
	//
	// Android (Release) 1968×2184: Galaxy Z Fold7 (Adreno 830, Vulkan 1.3)
	//   Vulkan  : ---- FPS () - visual parity verified
	//   OpenGL  : ---- FPS ()  - visual parity verified
	//
	// Linux (WSL Debug)
	//   OpenGL  : --- FPS () - visual parity verified
	//   Vulkan  : --- FPS () - visual parity verified
	//
	// -----------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 27
		#undef SCENE_COLOR
		#undef SCENE_TEXTURE
		#undef SCENE_TEXTURE_LIGHT

		#if DX_ENGINE_LEVEL == 27
			#undef MAIN_RENDER_LIGHT_RAY
			//#define DEBUG_TEXT_FONT		// DEBUG:
			#undef USE_CUBE                 //26-
			#undef USE_SPHERE               //26-
		#endif

        #if defined WINDOWS_PLATFORM
			#define USE_RASTERTEK_TEXT_FONT     // DX9, DX11, DX12, OPENGL, VULKAN: Windows
        #else
			#if defined VULKAN && defined ANDROID_PLATFORM
			#define USE_RASTERTEK_TEXT_FONT     // VULKAN: Linux / Android (inspired on: https://rastertek.com/dx11win10tut14.html)
			#endif
			#if defined OPENGL3 || defined OPENGL40
			//#define USE_RASTERTEK_TEXT_FONTV2	// OPENGL: Linux / Android (Inspired on: https://rastertek.com/gl4linuxtut14.html)
			#endif
        #endif
    #endif

    #if DX_ENGINE_LEVEL >= 28

		#if DX_ENGINE_LEVEL == 28
			#undef SCENE_COLOR
			#undef SCENE_TEXTURE
			#undef SCENE_TEXTURE_LIGHT
			#undef USE_CUBE                 //26-
			//#undef USE_SPHERE               //26-
		#endif
        #if defined WINDOWS_PLATFORM
            #if defined DX_ENGINE && defined NDEBUG
                #define ALLOW_PRINT_SCREEN_SAVE_PNG
            #endif
            #define USE_DIRECT_INPUT                // 28 We will use DX input method
            //#define USE_JOY                       // Default: off - Need to be tested...

			#if defined USE_DIRECT_INPUT
				//#define USE_MULTI_MONITOR
			#endif
        #endif

        #define SUN_LIGHT_DEMO_ANIMATION true
            
        #define USE_SKY_CAMERA_DOME         //28-we have sky camera in the world
        #define USE_SKYSPHERE               //28-the sky is a Sphere

        #if defined DX_ENGINE && defined NDEBUG
        #define ALLOW_PRINT_SCREEN_SAVE_PNG //28-print screen key will generate an image at DESKTOP
        #endif
    #endif

	#if DX_ENGINE_LEVEL == 29
		// USE_MAIN_THREAD restored(2026)
		// Restored the original WoMA dual - thread architecture.
		// Main thread handles Win32 orchestration and message processing.
		// Render thread executes ProcessFrame() continuously.
		// Eliminates first - frame audio stutter in Debug.
		// Preserves clean shutdown(WM_QUIT, queue drain, official wParam return).
		// No measurable performance penalty; slight gains in Vulkan and excellent DX11 throughput.
		// 
		#define USE_MAIN_THREAD

		#undef ALLOW_CBIND_PROGRESS_BAR

		#define USE_DEMO29
		#define INTRO_DEMO              // 29-Close the Intro / Demo of what was learned so far.
#ifndef _DEBUG
		#define USE_INTRO_VIDEO_DEMO
#endif
		#define INTRO_DEMO_INITIAL_PAGE 10		//10 is default INTRO_DEMO
		#define SPRITE_SCREEN_TO_SHOW   -5		//-5 is default INTRO_DEMO
		// or
		//#define INTRO_DEMO_INITIAL_PAGE 15	//skip 1st 5 pages... 10 is default  debug only! (Direct to INTRO)
		//#define SPRITE_SCREEN_TO_SHOW   0		//skip 1st 5 pages... -5 is default  debug only! (Direct to INTRO)
		// or
		//#define INTRO_DEMO_INITIAL_PAGE 21	//10 is default debug only!	   (Direct to DEMOS)
		//#define SPRITE_SCREEN_TO_SHOW   6		//-5 is default  debug only!   (Direct to DEMOS)

		#if defined WINDOWS_PLATFORM
		#define USE_DEMO29_v2 //NOTE: Need also: USE_DEMO29
		#define USE_DX_DRIVER_FONT		//FORCE!       
		#endif
	#endif
    #if DX_ENGINE_LEVEL >= 29
    //---------------------------------------------------------------------------------------
        #define SCENE_COLOR             // FORCE: Redefine:21
        #define SCENE_TEXTURE           // FORCE! Redefine:22
        #define SCENE_TEXTURE_LIGHT     // FORCE! Redefine:23

		#if defined LINUX_PLATFORM  
			#define USE_LINUX_SOUND		
		#endif
        #if defined ANDROID_PLATFORM  //RENAME: C:\WoMAengine2023\Android-WomaEngine\Android2\Android2.Packaging\ARM64\Release\Package\libs\arm64-v8a\liboboe.so
            #define USE_ANDROID_SOUND //LIBS:   %(LibraryDependencies);EGL;GLESv1_CM;GLESv2;GLESv3;m;z;jnigraphics;vulkan;oboe
        #endif
        #if defined WINDOWS_PLATFORM
            #define USE_WIN32_SOUND_MANAGER	// 29-SOUND: Sound Manager/"ogg" Loader
            #define USE_WIN32_PLAY_MUSIC	// 29-MUSIC: "ogg" Loader
        #endif
    #endif

	// 30+: Development branch.
	// Legacy drivers (DX9/DX12/OpenGL) stop evolving after Level 29.
	// DX11 remains the reference implementation.
	// Vulkan is re-enabled progressively as each level is ported.
    //--------------------------------------------------------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 30 // WORLD.XML: load "COLOR" .OBJ
		#undef DX9

		//Remove Basic demos:
        #define NO_SCENE_IMAGE_LOAD

        #undef SCENE_COLOR
        #undef SCENE_TEXTURE
        #undef SCENE_TEXTURE_LIGHT
		#undef USE_CUBE


		//Remove INTRO_DEMO:
        #undef USE_WIN32_SOUND_MANAGER
        #undef USE_WIN32_PLAY_MUSIC
        #undef INTRO_DEMO

		//Activate new engine features for level >=30:
        #define USE_SCENE_MANAGER               //30
        #define USE_FRUSTUM                     //30
    #endif

    #if DX_ENGINE_LEVEL >= 31
        #define RENDER_MULTIPLE_TEXTURES
		#define MAX_VULKAN_TEXTURES_MODEL 24 // Per-model maximum Vulkan texture slots (texArrayIndex 0..23).
    #endif

    #if DX_ENGINE_LEVEL >= 32
        #define RENDER_OBJ_WITH_LIGHT
    #endif

    #if DX_ENGINE_LEVEL >= 33
		//UPDATED: 022Texture.hlsl
		// 
		//ADDED:   Light shader upgrade: 033Light.hlsl
		//REMOVED: Light shader upgrade: 023Light.hlsl

		#undef OPENGL3
		#undef OPENGL40
		#undef DX12

        #define RENDER_OBJ_WITH_ALFA
    #endif

    #if DX_ENGINE_LEVEL >= 34
		//UPDATE:	021Color.hlsl
		//UPDATE:	022Texture.hlsl
		//NEW:		034Light.hlsl

		#define USE_ALFA_MAP	// new tech
        #define RENDER_OBJ_WITH_SPECULAR_SHININESS
    #endif

    #if DX_ENGINE_LEVEL >= 35   //35-
		//ADD:		035TextureBump.hlsl

        #define RENDER_OBJ_WITH_TEXTURE_BUMP
    #endif

    #if DX_ENGINE_LEVEL >= 36   //36-
		//-------------------------------------
		//Activate all needed to USE_SHADOW_MAP
		//-------------------------------------
		#define USE_SHADOW_MAP					//new tech
		//#define USE_LEVEL_36V2 //ON/OFF

		#define USE_RENDER_TO_TEXTURE			//new tech
        #define USELIGHTSIZE 15
		#define render_debug_2nd3DModel			//on/off
		#define activate_render_in_shadowMap
    #endif

	#if DX_ENGINE_LEVEL >= 37
	#endif

	// Offline Asset Pipeline:
	// Convert every OBJ introduced by previous tutorial levels into
	// WoMA's native W3D fast-loading format.
	// Level 38 is an asset converter, so Vulkan rendering is disabled.
	#if DX_ENGINE_LEVEL == 38
		#undef  VULKAN
		#define SAVEW3D
    #endif

	
	// LEVEL 39 – LOAD W3D RUNTIME
	// WORLD.XML now loads W3D (WoMA 3D Format) assets, replacing the OBJ files
	// introduced in Level 37 while achieving up to 30× faster loading.
	//
	// Runtime loading now uses the native W3D format generated in Level 38.
	// The rendering pipeline remains unchanged (CreateObject() is shared);
	// only the asset loader changes.
	//
	// Loading time: 0.5 seconds (W3D) vs 13.5 seconds (OBJ), demonstrating
	// the benefit of an offline asset conversion pipeline.

	// Benchmark Level 37 (OBJ)vs Level 39 (W3D)
	// 
	// WORLD objects
	// Driver	OBJ(L37)	W3D(L39)	Speed - up
	// DX11		3435 ms		200 ms		17.2×
	// Vulkan	3743 ms		461 ms		8.1×
	// 
	// Startup completo
	// Driver	OBJ(L37)	W3D(L39)	Speed - up
	// DX11		3609 ms		377 ms		9.6×
	// Vulkan	4102 ms		770 ms		5.3×

	// NOTE: All benchmarks were measured using RELEASE builds.
	// Repeated runs produced consistent results(199–200 ms in DX11), 
	// confirming that the W3D runtime loader behaves deterministically 
	// and that the observed speed - up comes from eliminating runtime OBJ parsing 
	// rather than measurement noise.

	#if DX_ENGINE_LEVEL >= 39
		#define LOADW3D
	#endif

    #if DX_ENGINE_LEVEL >= 40   //40-
		//UPGRADE: DXEngine_040\hlsl\023Light.hlsl
        #define USE_INSTANCES
    #endif

    #if DX_ENGINE_LEVEL >= 41   //41-
        #define USE_SHADOW_INSTANCES
        //#undef USE_REAL_SUNLIGHT_DIRECTION
        #define MAIN_RENDER_LIGHT_RAY
    #endif

	// LEVEL 42
	// Cross-platform parity (Windows/Linux/Android)
	// DX11 ↔ Vulkan shader parity
	// GPU Instancing
	// Instanced Shadow Maps
	// Per-instance Rotation
	// Common RenderScene() architecture refactor
    #if DX_ENGINE_LEVEL >= 42   //42-
        #define USE_SHADOW_INSTANCES_ROTATION_W_V_P
    #endif

#if defined _NOT
	//TO BE DONE! DEMO - FUTURE 43: TECH AVAILABLE:
	//#define RENDER_OBJ_WITH_ALFA          //--> _DX_ENGINE_LEVEL >= 33    //to be added later!
#endif

	#if DX_ENGINE_LEVEL >= 43 &&  DX_ENGINE_LEVEL <= 48
		#error "LEVEL RESERVED"
	#endif

	// CHECKLIST PRÉ - GITHUB(LEVEL 42)
	// 
	// N_CLASS_LOAD == N_CLASS_DELETE(zero fugas aparentes).
	// Gerar todos os 100 Tech Levels.
	// Gerar todos os projetos(VS, Linux, Android, etc.).
	// Compilar DX11 e Vulkan em Release.
	// Testar Windows, Linux e Android.
	// Confirmar que o WORLD.XML usa.W3D a partir do Level 39.
	// Verificar que o conversor do Level 38 continua a regenerar todos os.W3D.
	// Atualizar screenshots dos novos levels(36–39).
	// Atualizar o README com os benchmarks do W3D.

    // TERRAINS:
	//--------------------------------------------------------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 49   //49-
      #if defined _DEBUG
        #undef USE_LOADING_THREADS          //Use Thread to load Graphics
      #endif

	#if defined WINDOWS_PLATFORM
        #define USE_REAL_SUNLIGHT_DIRECTION //Static light
        #undef MAIN_RENDER_LIGHT_RAY        //Static light
	#endif  

        #define SCENE_GENERATEDUNDERWATER
        #undef SCENE_TERRAIN_COLLISION
        #define FIXEDBORDERHEIGHT -5.0f
		#undef render_debug_2nd3DModel
    #endif

    #if DX_ENGINE_LEVEL >= 50   //50-
        #define SCENE_GENERATEDUNDERWATER   //0
        #define SCENE_WATER_TERRAIN         //1
        #define SCENE_MAIN_TOPO_TERRAIN     //2
        #undef SCENE_TERRAIN_COLLISION
    #endif

    #if DX_ENGINE_LEVEL >= 51   //51-
		// FOG = ON  whenever fog_start < 32
		// FOG = OFF otherwise
        #define SCENE_FOG
        #undef SCENE_TERRAIN_COLLISION
    #endif

    #if DX_ENGINE_LEVEL >= 52   //52-
        #undef SCENE_GENERATEDUNDERWATER    //WITH BE USED AS "HEIGHT"
        #define SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
        #define SCENE_TERRAIN_WITH_NORMALS

        #undef USE_REAL_SUNLIGHT_DIRECTION  //Static light
        #define MAIN_RENDER_LIGHT_RAY       //Static light
        #undef SCENE_TERRAIN_COLLISION
        //#define DEBUG_TERRAIN_VERTICES    // Default: off
    #endif

    #if DX_ENGINE_LEVEL >= 53 //TEXTURE+COLOR SHADER
        #define TERRAIN_COLLISION_NX 0.45f
        #define TERRAIN_COLLISION_NZ 0.45f
        #define SCENE_TERRAIN_COLLISION
        #define EXTRA_INFO
        //#define DEBUG_COLLISION_TERRAIN   // Default: off
    #endif

    #if DX_ENGINE_LEVEL >= 54
		//world.xml
		//Water waves
    #endif

	//From 55 to 59 RESERVED
	#if DX_ENGINE_LEVEL >= 55 &&  DX_ENGINE_LEVEL <= 59
		#error "LEVEL RESERVED"
	#endif

    #if DX_ENGINE_LEVEL >= 60

        #define SCENE_SLOPE_MAP_TEXTURE //55 use 4 textures
        #define USE_TERRAIN_TUTORIAL_CHAP_24 
        #undef SCENE_TERRAIN_COLLISION
    #endif

    #if DX_ENGINE_LEVEL >= 61
        #if DX_ENGINE_LEVEL == 61
        #define MATH_BENCH
        #endif
        #define USE_TERRAIN_512
    #endif

    #if DX_ENGINE_LEVEL >= 62
		#define USE_MAIN_MAP
    #endif
    #if DX_ENGINE_LEVEL >= 63
        #define USE_MINI_MAP
    #endif

    #if DX_ENGINE_LEVEL >= 64
		#undef VULKAN
        #undef USE_CUBE
    #endif
	
    #if DX_ENGINE_LEVEL >= 65
        #define SCENE_TERRAIN_COLLISION
        #define EXTRA_INFO
    #endif

    //--------------------------------------------------------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 70 //SCENE_BILLBOARDS
        #define TUTORIAL_CHAP 60
        #define SCENE_BILLBOARDS
    #endif
    #if DX_ENGINE_LEVEL >= 71 && defined SCENE_BILLBOARDS
        #define TUTORIAL_CHAP 61
        #define BILLBOARD_FOR_FENCES
    #endif
    #if DX_ENGINE_LEVEL >= 72 && defined SCENE_BILLBOARDS
        #define TUTORIAL_CHAP 62
        #define BILLBOARD_FOR_FIRE
        #define SOUND3D //3D Sound Effects
    #endif
    #if DX_ENGINE_LEVEL >= 73 && defined SCENE_BILLBOARDS
        #define TUTORIAL_CHAP 63
        #define BILLBOARD_FOR_WINDY_GRASS
    #endif
    #if DX_ENGINE_LEVEL >= 74 && defined SCENE_BILLBOARDS
        #define TUTORIAL_CHAP 64
    #endif
    #if DX_ENGINE_LEVEL >= 75 && defined SCENE_BILLBOARDS
        #define ALLOW_CBIND_PROGRESS_BAR
    #endif
    #if DX_ENGINE_LEVEL == 76 || defined RELEASE && DX_ENGINE_LEVEL >= 76
        #define USE_INTRO_VIDEO_DEMO
    #endif
    #if DX_ENGINE_LEVEL == 77 && defined SCENE_BILLBOARDS
        #define USE_INSTANCES_FOR_TREES
    #endif
    #if DX_ENGINE_LEVEL >= 78
        #define USE_BOUNDING_VOLUMES
        #define CHECK_OBJ_COLISION
    #endif

    //#define DEBUG_MESH
    #if defined ASSIMP //>= 79
        #if DX_ENGINE_LEVEL == 79
            //FBX/DAE
            #define MAVERICK
            #define USE_MATH3D          //79+80
            #define USE_ASSIMP_GLLIB    //79+80
            #define USE_ASSIMP_LATEST   //79-def
        #endif
        #if DX_ENGINE_LEVEL == 80
            #define USE_MATH3D          //79+80
            #define USE_ASSIMP_GLLIB    //79+80
            //#undef  USE_ASSIMP_LATEST //79-undef
        #endif
        #if DX_ENGINE_LEVEL == 81
            //#define USE_MATH3D
            #define USE_ASSIMP_DXLIB    //81
        #endif
    #endif

	#if defined SCENE_TERRAIN_COLLISION
		#if defined WINDOWS_PLATFORM
			#define USE_LEVEL_36V2 //ON/OFF
		#endif
		#if DX_ENGINE_LEVEL < 99
			#define USE_SMALL_SHADOWS
		#endif
	#endif

	//-------------------------------------------------------------------------------------------------------
    //MAIN_RENDER:
    //-------------------------------------------------------------------------------------------------------
#if defined USE_TITLE_BANNER
  #define MAIN_RENDER_TITLE           //24
#endif
#if DX_ENGINE_LEVEL >= 25 
  #define MAIN_RENDER_DRIVER_FONT     //25
  #define MAIN_RENDER_RASTERTEK_FONT  //27
#if defined USE_SKYSPHERE
  #define MAIN_RENDER_SKY             //28
#endif
  #define MAIN_RENDER_MAIN_XML_OBJ    //30
#endif

#if DX_ENGINE_LEVEL >= 49
    #define MAIN_RENDER_WATER           //50
    #define MAIN_RENDER_TERRAIN         //55/65
    #define MAIN_RENDER_MINIMAP         //63
    #define MAIN_RENDER_BILLBOARDS      //70/74
    #define MAIN_RENDER_ASSIMP          //82
#endif
    //-------------------------------------------------------------------------------------------------------
    
    #if DX_ENGINE_LEVEL >= 79
        #undef  USE_IDEA_PACK
        #define USE_CYPHER_PACK
        #define MULTIPACK_PACK  TEXT("C:/WoMAengine2023/engine/data/womamulti.pack")
        #define MULTIPACK_PCK   TEXT("womamulti.pck")
    #endif
    #if DX_ENGINE_LEVEL >= 82
        #define USE_DX11_1_SETUP        //Mandatory: ON
		#undef USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE // Dep: USE_ALLOW_RESIZE
        #define USE_ASSIMP_LATEST       //Mandatory: ON
    #endif
    #if DX_ENGINE_LEVEL >= 83
        //#define SAVE_WALK // Default: off - Updated billboard SHADER & SAVE_WALK FILME
    #endif
    #if DX_ENGINE_LEVEL >= 84
        #define LOAD_WALK               //cant be undef
        #define SCENE_MOVIN_SKIN        //cant be undef
    #endif
    #if DX_ENGINE_LEVEL >= 85
        #define USE_MINIMAP_EXPANSION
    #endif

    //-------------------------------------------------
    #define USE_MODEL1
    #define USE_MODEL2

    #ifndef USE_MODEL2
        #undef SCENE_MOVIN_SKIN
    #endif

    #if DX_ENGINE_LEVEL >= 86
        #undef USE_STATUSBAR
        #define USE_INTRO_VIDEO_DEMO
      #if defined MAVERICK
        //#define USE_MODEL3 // Default: off
      #endif
      #if DX_ENGINE_LEVEL < 88
        #define USE_MODEL4
      #endif
    #endif
    
    #if DX_ENGINE_LEVEL >= 88
        #define USE_3RD_PERSON_CAMERA
    #endif

#if DX_ENGINE_LEVEL >= 89
    //ORI:          C:\WoMAengine2023\engine\data\worldMap
    //Compressed:   C:\WoMAengine2023\engine\data\scene87ForestHuntress\worldMap

    #define USE_MAP_EDITOR
    //#define USE_AABB_COLISION_CHECK // Default: off
#endif

#if DX_ENGINE_LEVEL >= 90       // Replacing Billboards with advanced gaming like objects(Low-Poly)
    #undef  USE_MAP_EDITOR
    #undef  ALLOW_CBIND_PROGRESS_BAR
    //#define USE_INSTANCES_FOR_TREES90
#endif

#if DX_ENGINE_LEVEL >= 91
    #define USE_UPDATEMESHANIMATIONS_THREAD	// Allow the other 2: Shadow threads will also do:
    #define USE_MINIMAP_REDENRING_THREAD	// Minimap rendering thread
    #define USE_MESH_LOADING_THREAD			// Mesh loading thread
#endif

#if DX_ENGINE_LEVEL >= 92
    #define USE_FASTER_BILL_SHADER          // Faster bills: v1
#endif

#if DX_ENGINE_LEVEL >= 93
    #define  NO3DBILL                       // No 3D Trees
    #define GENERATE_ATLAS_INTEGRATION_DDS  // Faster bills: v2

    #if defined MAVERICK                    // Means filtered from public code
    #if _DEBUG
    #define EXPORT_ATLAS_DEBUG_PNG          // Internal Debug
    #endif
    #endif
#endif

#if DX_ENGINE_LEVEL >= 94
    // Try nothing and...:      //RELEASE: 906FPS
    //#define USE_TREE_POINTER  //v1: RELEASE: 900FPS
    #define USE_TREE_POINTERV2  //v2: RELEASE: 911FPS
#endif

#if DX_ENGINE_LEVEL >= 95
    #define USE_CURVED_SKY_PLANE
#endif

#if DX_ENGINE_LEVEL >= 96
    #define MAIN_RENDER_CURVED_REAL_SKY_PLANE
#endif

#if DX_ENGINE_LEVEL >= 97
    #define MAIN_RENDER_WATER_FALL
#endif

#if DX_ENGINE_LEVEL >= 98
    #define USE_DAY_AND_NIGHT

    #define USE_INSTANCES_FOR_LAMP
    #define USE_INSTANCES_FOR_LAMP_ROWS  1
    #define USE_INSTANCES_FOR_LAMP_LINES 13

    #define USE_POINTS_OF_LIGHT_FOR_LAMP
    #define MAX_POINT_LIGHTS 26
    #define ACTIVELAMPCOUNT USE_INSTANCES_FOR_LAMP_ROWS * USE_INSTANCES_FOR_LAMP_LINES
#endif

#if DX_ENGINE_LEVEL >= 99
	#define USE_TERRAIN_SHADOWS
	#undef USE_DAY_AND_NIGHT
#endif

#if DX_ENGINE_LEVEL >= 100 // Current LATEST LEVEL READY
	#define USE_MESH_SHADOWS
#endif

#if DX_ENGINE_LEVEL >= 101
	#define USE_SSAO
#endif

#if DX_ENGINE_LEVEL >= 102
	#define USE_PLANET_EARTH
#endif

#if DX_ENGINE_LEVEL >= 103
	#define USE_WOMA_ENGINE_ONE_CBUFFER
#endif

#if DX_ENGINE_LEVEL >= 104
	/*
	LEVEL 104 - GPU Procedural Terrain

	Vertex input:
		layout(location = 0) in vec2 inXZ;    // Apenas X,Z

	Vertex generation:
		vec3 localPos;
		localPos.xz = inXZ;
		localPos.y = ComputeTerrainHeight(localPos.xz);

		vs_out.worldPos = localPos;
		gl_Position = WVP * vec4(localPos, 1.0);
	*/
#endif

	// USE_IMGUI:
	//--------------------------------------------------------------------------------------------------------------------------
	//#if DX_ENGINE_LEVEL >= 45
	//	#define USE_IMGUI
	//
	//	#if !defined ANDROID_PLATFORM
	//		#define RUN_ASMAIN true
	//	#endif
	//#endif

	//-------------------------------------------------------------------------------------------------------
	// Final Defines:
    //-------------------------------------------------------------------------------------------------------
    #undef  dx12_upload_old_way

    #if DX_ENGINE_LEVEL < 94
    #if DX_ENGINE_LEVEL != 29 && DX_ENGINE_LEVEL != 76
    #undef USE_INTRO_VIDEO_DEMO
    #endif
    #endif
    #if defined WINDOWS_PLATFORM
		#if DX_ENGINE_LEVEL != 29
        #define ALLOW_CBIND_PROGRESS_BAR
		#endif
	#endif

    #if DX_ENGINE_LEVEL == 26
    #define SPHERE_GRIDPOINTS 100
    #else
    #if DX_ENGINE_LEVEL >= 94
        #define SPHERE_GRIDPOINTS 10
        #else
        #define SPHERE_GRIDPOINTS 25
    #endif
    #endif

	#define MAX_ENGINE_LEVEL 200

	#if DX_ENGINE_LEVEL >= 82 && !defined USE_ASSIMP_LATEST
		#error "USE_ASSIMP_LATEST need to be ON"
	#endif
	
	// DLL import/export handling:
	#if defined(COMMONFUNCTIONS_DLL_EXPORTS)
		#define COMMON_API __declspec(dllexport)
		#pragma message( "BUILD: dllexport" )  
	#elif defined(WOMA_DLL)
		#define COMMON_API __declspec(dllimport)
		#pragma message( "BUILD: dllimport" )  
	#else
		#define COMMON_API
		//#pragma message( "BUILD: export" )  
	#endif

	#ifndef MAX_POINT_LIGHTS
		#define MAX_POINT_LIGHTS 26
	#endif

	#pragma warning(pop)

//-------------------------------------------------
// THE END
//-------------------------------------------------
#endif