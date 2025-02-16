// --------------------------------------------------------------------------------------------
// Filename: tech-level.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2024
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234567311;

#pragma once

#define WOMAENGINE
#if defined NewWomaEngine
	#define WOMAENGINE_BASIC
#endif

//------------------------------------------------------------------------------------------------------------
//#define CORE_ENGINE_LEVEL 10
//------------------------------------------------------------------------------------------------------------
#if defined RELEASE
	#define USE_NETWORK
#else
	#if CORE_ENGINE_LEVEL > 9 && (defined _DEBUG || defined DEBUG)
		#define USE_FORCE_HOUR	/*24*/ /*18*/ 12
		#define USE_FORCE_MIN	00
	#endif
#endif

#if defined _DEBUG
	//#define VERBOSE_MEMORY_DEBUG			// [ON/OFF]: used (woma_macros.h)
#endif

//------------------------------------------------------------------------------------------------------------

#if CORE_ENGINE_LEVEL >= 1 && !defined ANDROID_PLATFORM
#define USE_LOG_MANAGER			//1
#endif

#if CORE_ENGINE_LEVEL >= 2 && !defined WOMAENGINE_BASIC
	//
#endif

#if CORE_ENGINE_LEVEL >= 3 //&& !defined WOMAENGINE_BASIC
	#if defined WINDOWS_PLATFORM
	#define USE_WOMA_EXCEPTION		//3
	#define USE_MINIDUMPER			//3
	#endif
#endif

#if CORE_ENGINE_LEVEL >= 4 //&& !defined WOMAENGINE_BASIC
	#define USE_USER_SETUP
	#if !defined ANDROID_PLATFORM
		#define USE_PROCESS_OS_KEYS	//4 - NOTE: Madatory at 4
	//#endif
	//#if defined WINDOWS_PLATFORM || defined LINUX_PLATFORM
		#define USE_SYSTEM_CHECK	//4
	#endif
  	#define USE_CHECK_IO			//4
	#define USE_TIMER_CLASS			//4
#endif

#if CORE_ENGINE_LEVEL >= 5 //&& defined WINDOWS_PLATFORM  && !defined WOMAENGINE_BASIC
	#if !defined ANDROID_PLATFORM
	#define USE_TINYXML_LOADER
	#endif
	#if defined WINDOWS_PLATFORM
	#define CLIENT_SCENE_SETUP	//CoreEngine_005 (PAINT+settings.xml)
	#endif
#endif

#if CORE_ENGINE_LEVEL >= 6 && defined WINDOWS_PLATFORM  //&& !defined WOMAENGINE_BASIC
	#define USE_TIMER			//6 Calculate FPS & dT for animations
	#define USE_IDEA_PACK		//6
	
#endif

#if CORE_ENGINE_LEVEL >= 7 && defined WINDOWS_PLATFORM  //&& !defined WOMAENGINE_BASIC
	#define USE_ASTRO_CLASS     //CoreEngine_007 (PAINT RealTimeCelestial)		
	#if CORE_ENGINE_LEVEL == 7 || DX_ENGINE_LEVEL == 25 || DX_ENGINE_LEVEL == 29
	#define USE_NETWORK
	#endif
	#if defined WINDOWS_PLATFORM
		#define ALLOW_LOADING_SPLASH
		#define USE_ALLOW_MAINWINDOW_RESIZE //Allow main Window to Resize
	#endif
#endif

#if CORE_ENGINE_LEVEL >= 8 && defined WINDOWS_PLATFORM //&& !defined WOMAENGINE_BASIC
	#define USE_GPS_MAP			//CoreEngine_008 (PAINT RealTimeMapGPSPosition)	
	#if CORE_ENGINE_LEVEL == 8
		#define USE_NETWORK
	#endif
#endif

#if CORE_ENGINE_LEVEL >= 9 && defined WINDOWS_PLATFORM //&& !defined WOMAENGINE_BASIC
	#define USE_METARCLASS	//CoreEngine_009 (PAINT RealTimeWeather) TODO: Android read asset file
	#if CORE_ENGINE_LEVEL == 9
		#define USE_NETWORK
	#endif
	//#define GENERATE //(Airports List)
#endif

//-------------------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL == 10
//-------------------------------------------------------------------------------------------------------
	#if defined _MSC_VER	//DX9 DX11+DX10 DX12 OPENGL3
		//#define DX9sdk		//(ORIGINAL DX9SDK) not fully recovered/implemented
		#define DX9				//Using DX11 API
		#define DX11
		#define DX12			//(defined DX12 && D3D11_SPEC_DATE_YEAR > 2009)		
	#endif

	#if defined WINDOWS_PLATFORM && (defined DX9sdk || defined DX9 || defined DX11 || defined DX12)
		#define DX_ENGINE		//Turn on: #include "DXengine.h"
	#endif

	#define OPENGL3 //WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM

	#if defined DX12 //SELECT DXGI_API version:
		//#define DX12_DXGI_API 3	//IDXGIFactory3
		#define DX12_DXGI_API 4		//IDXGIFactory4
		//#define DX12_DXGI_API 5	//IDXGIFactory5
		//#define DX12_DXGI_API 6	//IDXGIFactory6
	#endif
//--------------------------------------------------------------------------------------------------------
//WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM
//--------------------------------------------------------------------------------------------------------
	#if LEVEL >= 11
		#undef CLIENT_SCENE_SETUP // For fps benchmark only!
	#endif

	#if LEVEL >= 19 && !defined WOMAENGINE_BASIC
	#if _DEBUG
		#undef USE_SYSTEM_CHECK // Just to make it faster in DEBUG
	#endif
		#if defined WINDOWS_PLATFORM
		#define CLIENT_SCENE_SETUP
		#endif
		#if defined DX11 //DEFAULT: DX11_0
		//#define USE_DX11_1
		//#define USE_DX11_3
		#endif
		#define SET_DEVICE_CAPABILITIES //report.txt
		#define USE_STATUSBAR
	#endif

	#if DX_ENGINE_LEVEL >= 20 && !defined WOMAENGINE_BASIC
 		#define USE_ALLOW_RESIZE
		#define USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE //Dep: USE_ALLOW_RESIZE
		//#define USE_ASPECT_RATIO //Dep: USE_ALLOW_RESIZE
	#endif

	#if DX_ENGINE_LEVEL >= 21 && !defined WOMAENGINE_BASIC
		//#define dx12_upload_old_way	//DX12 upload method
		

		#if defined WINDOWS_PLATFORM && defined DX12
		#define USE_RASTERIZER_STATE //Mandatory for DX12
		#endif

		#if !defined ANDROID_PLATFORM
		#define GL_SETRASTERIZERSTATE
		#endif

		#define GL_CLEAR_DEPTH
		#define SCENE_COLOR
		#define ROTATE_SQUARE

		//#define USE_PRECOMPILED_SHADERS	// [ON/OFF] ON: Merge Shader's Code on .EXE / OFF: Compile in run-time
	#endif

	#if DX_ENGINE_LEVEL >= 22
		#define TEX_MODE_BORDER		//DX12 texture style.
		#define SCENE_TEXTURE		//DEMO 22
		#define USE_IMAGE_BMP		//Mandatory!

		#undef SCENE_COLOR
		//#define NO_SCENE_IMAGE_LOAD //un-comment for DEBUG (debuging only!)

		#if !defined NO_SCENE_IMAGE_LOAD
			#if defined WINDOWS_PLATFORM
				#define USE_IMAGE_TIFF
				#define USE_IMAGE_DDS
			#endif
			
			#define USE_IMAGE_PNG
			#define USE_IMAGE_JPG
			#define USE_IMAGE_TGA
		#endif
		
	#endif

	#if DX_ENGINE_LEVEL >= 23 && !defined GOTO_BASICS
		#undef SCENE_TEXTURE
		#define SCENE_TEXTURE_LIGHT				
			
		#define USE_LIGHT_RAY					// ON/OFF - Render light ray !!Dep!!: #undef  dx12_upload_old_way
		#define SUN_LIGHT_DEMO_ANIMATION true	// Animate the beam of light
	#endif

	#if DX_ENGINE_LEVEL >= 24
		#if DX_ENGINE_LEVEL == 24
			#undef SCENE_COLOR
			#undef SCENE_TEXTURE
			#undef SCENE_TEXTURE_LIGHT
			#undef USE_LIGHT_RAY
			#define NO_SCENE_IMAGE_LOAD //(for debuging only!)
		#endif
		#define USE_TITLE_BANNER
		#define USE_ALPHA_BLENDING
		#define USE_VIEW2D_SPRITES
		#define USE_RASTERIZER_STATE
        #define USE_DSV //DX12
	#endif

	#if DX_ENGINE_LEVEL >= 25 //WINDOWS ONLY
		#if defined DX_ENGINE
			#if !defined _DEBUG
			#define USE_DX10DRIVER_FONTS
			#endif
			#define USE_ALPHA_BLENDING
		#endif
	#endif

	#if DX_ENGINE_LEVEL >= 26
		//#undef USE_IMGUI
		#define USE_CUBE					//26-
		#define USE_SPHERE					//26-
	#endif

	#if DX_ENGINE_LEVEL >= 27
		#if defined WINDOWS_PLATFORM
		#define USE_RASTERTEK_TEXT_FONT		//27-Allow Legends for all Levels since 20: with "techno" of 28 (Text Fonts)
		//#define TEXT_TEST //(debug only!)
		#else
		#define USE_RASTERTEK_TEXT_FONTV2
		#endif
	#endif

	#if DX_ENGINE_LEVEL >= 28
		#if defined WINDOWS_PLATFORM
			#if defined DX_ENGINE
				#define ALLOW_PRINT_SCREEN_SAVE_PNG	// Level:22
			#endif
			#define USE_DIRECT_INPUT			//28-we will use DX input method
			//#define USE_JOY					// Need to be tested...
		#endif

		#define SUN_LIGHT_DEMO_ANIMATION true
			
		#define USE_SKY_CAMERA_DOME			//28-we have sky camera in the world
		#define USE_SKYSPHERE				//28-the sky is a Sphere

		#if defined DX_ENGINE
		#define ALLOW_PRINT_SCREEN_SAVE_PNG //28-print screen key will generate an image at DESKTOP
		#endif
	#endif

	#if DX_ENGINE_LEVEL >= 29
		#define SCENE_COLOR				//FORCE!
		#define SCENE_TEXTURE			//FORCE!
		#define SCENE_TEXTURE_LIGHT		//FORCE!
		#define INTRO_DEMO				// 29-Close the Intro / Demo of what was learned so far.

		#if defined ANDROID_PLATFORM
			#define USE_ANDROID_SOUND
		#endif
		#if defined WINDOWS_PLATFORM
			#define USE_WIN32_SOUND_MANAGER	// 29-SOUND: Sound Manager/"ogg" Loader
			#define USE_WIN32_PLAY_MUSIC	// 29-MUSIC: "ogg" Loader
			#if DX_ENGINE_LEVEL == 29
			#define USE_DX10DRIVER_FONTS	//FORCE!	   
			#endif
		#endif
	#endif

	//--------------------------------------------------------------------------------------------------------------------------
	#if DX_ENGINE_LEVEL >= 30 // WORLD.XML: load "COLOR" .OBJ
		#define NO_SCENE_IMAGE_LOAD
		#undef USE_CUBE
		#undef SCENE_COLOR
		#undef SCENE_TEXTURE
		#undef SCENE_TEXTURE_LIGHT
		#undef USE_WIN32_SOUND_MANAGER
		#undef USE_WIN32_PLAY_MUSIC
		#undef INTRO_DEMO
		#define  SUN_LIGHT_DEMO_ANIMATION true //back

		#define USE_SCENE_MANAGER	//30-
		#define USE_FRUSTRUM		//30-
	#endif

	#if DX_ENGINE_LEVEL >= 31
		#define RENDER_MULTIPLE_TEXTURES
	#endif

	#if DX_ENGINE_LEVEL >= 32
		#define RENDER_OBJ_WITH_LIGHT
	#endif

	#if DX_ENGINE_LEVEL >= 33
		#undef DX12					//to be added later
		#define RENDER_OBJ_WITH_ALFA				   
	#endif

	#if DX_ENGINE_LEVEL >= 34
		#define RENDER_OBJ_WITH_SPECULAR_SHININESS
	#endif

	#if DX_ENGINE_LEVEL >= 35	//35-
		#define RENDER_OBJ_WITH_TEXTURE_BUMP
	#endif

	#if DX_ENGINE_LEVEL >= 36	//36-
		#define USE_SHADOW_MAP
		#define USELIGHTSIZE 10
		//#define USE_REAL_SUNLIGHT_DIRECTION
	#endif

	#if DX_ENGINE_LEVEL >= 37	//37-
		//#define USE_MAIN_THREAD		//MAIN LOOP is a THREAD
		//#define USE_MAIN_THREAD		// ON/OFF (Note: Dont work on openGL, NOTE!!! This is buggy with maximize!!! Dont use for now
		//#define USE_LOADING_THREADS	//Use Thread to load Graphics (Bug! will launch multiple!?)

		#if defined _NOT //ADDED ON C:\WoMAengine2023\DXEngine_037 BUT NOT USED ON DEBUG
		#define ALLOW_CBIND_PROGRESS_BAR	//Display Progress Bar
		#endif
	#endif

	#if DX_ENGINE_LEVEL >= 38	//38-
		#define SAVEW3D
	#endif

	#if DX_ENGINE_LEVEL >= 39	
		#undef SAVEW3D
		#define LOADW3D //39-
		#undef OPENGL3
	#endif

	#if DX_ENGINE_LEVEL >= 40	//40-
		#define USE_INSTANCES
		#define USE_REAL_SUNLIGHT_DIRECTION	//FOR DEBUG ONLY!
		#undef USE_LIGHT_RAY
	#endif

	#if DX_ENGINE_LEVEL >= 41	//41-
		#define USE_SHADOW_INSTANCES
		#undef USE_REAL_SUNLIGHT_DIRECTION
		#define USE_LIGHT_RAY
	#endif

	#if DX_ENGINE_LEVEL >= 42	//42-
		#define USE_SHADOW_INSTANCES_ROTATION_W_V_P
	#endif

	// TERRAINS:
	//--------------------------------------------------------------------------------------------------------------------------
	//TO BE DONE! DEMO - FUTURE 43: TECH AVAILABLE:
	//#define RENDER_OBJ_WITH_ALFA			//--> _DX_ENGINE_LEVEL >= 33	//to be added later!
	#if DX_ENGINE_LEVEL >= 49	//49-
		#if _DEBUG
		#undef USE_MAIN_THREAD				//MAIN LOOP is a THREAD
		#undef USE_LOADING_THREADS			//Use Thread to load Graphics
		#endif
		#define USE_REAL_SUNLIGHT_DIRECTION	//Static light
		#undef USE_LIGHT_RAY				//Static light
			
		#define SCENE_GENERATEDUNDERWATER	//NEW
	#endif

	#if DX_ENGINE_LEVEL >= 50	//50-
		#if _DEBUG
		#undef USE_MAIN_THREAD				//MAIN LOOP is a THREAD
		#undef USE_LOADING_THREADS			//Use Thread to load Graphics
		#endif
		#define SCENE_GENERATEDUNDERWATER	//0
		#define SCENE_WATER_TERRAIN			//1
		#define SCENE_MAIN_TOPO_TERRAIN		//2
	#endif

	#if DX_ENGINE_LEVEL >= 51	//51-
		#define SCENE_FOG
	#endif

	#if DX_ENGINE_LEVEL >= 52	//52-
		#undef SCENE_GENERATEDUNDERWATER		//WITH BE USED AS "HEIGHT"
		#define SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
		#define SCENE_TERRAIN_WITH_NORMALS

		#undef USE_REAL_SUNLIGHT_DIRECTION	//Static light
		#define USE_LIGHT_RAY				//Static light

		//#define DEBUG_TERRAIN_VERTICES
	#endif

	#if DX_ENGINE_LEVEL >= 53 //TEXTURE+COLOR SHADER
		#define SCENE_TERRAIN_COLLISION
	#endif

	#if DX_ENGINE_LEVEL >= 54
	/*world.xml
	<woma>
		<world hVisibility="128" seaLevel="0" size="512" patchSize="64" 
				water="engine/data/Terrains/heightmap256x256.bmp" 
				waterTexture="engine/data/Terrain/008water_tex04.jpg"
				mainTexture="engine/data/Terrain/011map1_heightmap01_v7.bmp" 
				skyDayTexture="engine/data/NEW_SKY_DOME.jpg" 
				skyNightTexture=""/>
	</woma>
	*/
	#endif

	#if DX_ENGINE_LEVEL >= 55 //Slope Texture Shader
		#define SCENE_SLOPE_MAP_TEXTURE //55 use 4 textures
	#endif

	#if DX_ENGINE_LEVEL >= 56 // 56 ~ 	#if TUTORIAL_CHAP >= 19 // TERRAIN
		#define USE_TERRAIN_TUTORIAL_CHAP_19 //TODO
	#endif
	#if DX_ENGINE_LEVEL >= 57 // 57 ~ 	#if TUTORIAL_CHAP >= 21 // TERRAIN
		#define USE_TERRAIN_TUTORIAL_CHAP_21 //TODO
	#endif
	#if DX_ENGINE_LEVEL >= 58 // 58 ~ 	#if TUTORIAL_CHAP >= 22 // TERRAIN
		#define USE_TERRAIN_TUTORIAL_CHAP_22 //TODO
	#endif
	#if DX_ENGINE_LEVEL >= 59 // 59 ~ 	#if TUTORIAL_CHAP >= 23 // TERRAIN
		#define USE_TERRAIN_TUTORIAL_CHAP_22 //TODO
	#endif
	#if DX_ENGINE_LEVEL >= 60 // 60 ~ 	#if TUTORIAL_CHAP >= 24 // TERRAIN
		#define USE_TERRAIN_TUTORIAL_CHAP_24 //ontop: SCENE_SLOPE_MAP_TEXTURE
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
		//#define DX_INPUT_DEBUG_TEST
	#endif
	#if DX_ENGINE_LEVEL >= 63
		#define USE_MINI_MAP
		//#undef USE_STATUSBAR //DEBUG!
	#endif
	#if DX_ENGINE_LEVEL >= 64
		#define LOADMD5
	#endif

	// 
	//--------------------------------------------------------------------------------------------------------------------------
	#if DX_ENGINE_LEVEL >= 70 
		#define SCENE_COMPOUND	//TUTORIAL_CHAP >= 55
	#endif

	#if DX_ENGINE_LEVEL >= 71 //Check Compound Colision
		#define CHECK_COMPOUND_COLISION	//TUTORIAL_CHAP >= 96
	#endif

	#if DX_ENGINE_LEVEL >= 80 //25 //IMGUI!
		#if DX_ENGINE_LEVEL == 25
			#undef SCENE_COLOR
			#undef SCENE_TEXTURE
			#undef SCENE_TEXTURE_LIGHT
			#undef USE_LIGHT_RAY
		#endif

		#define USE_IMGUI

		#if !defined ANDROID_PLATFORM
		#define RUN_ASMAIN true //#define RUN_ASMAIN false //true
		#endif
	#endif

	#if DX_ENGINE_LEVEL >= 80 //Render objects loaded by threads.
	#endif


	//#define USE_MULTI_MONITOR

	//-------------------------------------------------------------------------------------------------------
	#if !defined USE_LIGHT_RAY			// ON/OFF - Render light ray !!Dep!!: #undef  dx12_upload_old_way
		#define  dx12_upload_old_way	// DX12 upload method
	#else
		#undef  dx12_upload_old_way		// DX12 upload method
	#endif

	#undef  dx12_upload_old_way
	//-------------------------------------------------------------------------------------------------------
	#if defined RELEASE && DX_ENGINE_LEVEL < 29 && defined _MSC_VER
	#error "WOMA COMPILATION ERROR: RELEASE bin is not Supported before DX_ENGINE_LEVEL 29"
	#endif
#endif
	
