// --------------------------------------------------------------------------------------------
// Filename: tech-level.h
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
//WomaIntegrityCheck = 1234525217;

#pragma once
#pragma warning(push)
#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition

#define WOMAENGINE
#if defined NewWomaEngine
    #define WOMAENGINE_BASIC
#endif

//------------------------------------------------------------------------------------------------------------
//#define CORE_ENGINE_LEVEL 10
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
    //#define WOMA_SKIP_CREATE_MAIN_WINDOW  // Default: off - for command line application only!
#endif

#if CORE_ENGINE_LEVEL >= 3
    #if defined WINDOWS_PLATFORM
    #define USE_WOMA_EXCEPTION      //3
    #define USE_MINIDUMPER          //3
    #endif
#endif

#if CORE_ENGINE_LEVEL >= 4
    #define USE_USER_SETUP          //4
    #if !defined ANDROID_PLATFORM
        #define USE_PROCESS_OS_KEYS //4 - NOTE: Madatory at 4
        #define USE_SYSTEM_CHECK    //4
    #endif
    #define USE_CHECK_IO            //4
    #define USE_TIMER_CLASS         //4
    #endif

#if CORE_ENGINE_LEVEL >= 5
    #if !defined ANDROID_PLATFORM
    #define USE_TINYXML_LOADER      //5
    #endif
    #if defined WINDOWS_PLATFORM
    #define CLIENT_SCENE_SETUP      //5 (PAINT+settings.xml)
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
        #define ALLOW_LOADING_SPLASH        //7 
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
        #if !defined MAINENGINE
            #define USE_NETWORK //9
        #endif
    #endif
    //#define GENERATE // Default: off - (Airports List)
#endif

//-------------------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 10
//-------------------------------------------------------------------------------------------------------
    #if defined _MSC_VER    //DX9 DX11+DX10 DX12 OPENGL3
        //#define DX9sdk        //(ORIGINAL DX9SDK) not fully recovered/implemented
        #define DX9             //Using DX11 API
        #define DX11
        #define DX12            //(defined DX12 && D3D11_SPEC_DATE_YEAR > 2009)     
    #endif

	#define OPENGL3 //WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM

  #if defined WINDOWS_PLATFORM && (defined DX9sdk || defined DX9 || defined DX11 || defined DX12)
        #define DX_ENGINE       //Turn on: #include "DXengine.h"
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
  #endif

//--------------------------------------------------------------------------------------------------------
//WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM
//--------------------------------------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 11
        #undef CLIENT_SCENE_SETUP // For fps benchmark only!
    #endif

    #if DX_ENGINE_LEVEL >= 19 && !defined WOMAENGINE_BASIC
		#undef USE_ALLOW_MAINWINDOW_RESIZE
		#define USE_NETWORK
        #if defined WINDOWS_PLATFORM
        #define CLIENT_SCENE_SETUP
        #endif

        #define SET_DEVICE_CAPABILITIES //report.txt
    #endif

    #if DX_ENGINE_LEVEL >= 20 && !defined WOMAENGINE_BASIC
		#define USE_ALLOW_MAINWINDOW_RESIZE
        #define USE_STATUSBAR
        #define USE_ALLOW_RESIZE
        #define USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE //Dep: USE_ALLOW_RESIZE
        //#define USE_ASPECT_RATIO // Default: off - Dep: USE_ALLOW_RESIZE
    #endif

    #if DX_ENGINE_LEVEL >= 21 && !defined WOMAENGINE_BASIC
		//#undef USE_NORMAL_WINDOW
        #if defined WINDOWS_PLATFORM && defined DX12
        #define USE_RASTERIZER_STATE //Mandatory for DX12
        #endif

        #if !defined ANDROID_PLATFORM
        #define GL_SETRASTERIZERSTATE
        #endif

        #define GL_CLEAR_DEPTH
        #define SCENE_COLOR
        #define ROTATE_SQUARE

        #define MAIN_RENDER
        //#define USE_PRECOMPILED_SHADERS   // Default: off - [ON/OFF] ON: Merge Shader's Code on .EXE / OFF: Compile in run-time
    #endif

    #if DX_ENGINE_LEVEL >= 22
        #define TEX_MODE_BORDER     //DX12 texture style.
        #define SCENE_TEXTURE       //DEMO 22
        #define USE_IMAGE_BMP       //Mandatory!

        #undef SCENE_COLOR
        //#define NO_SCENE_IMAGE_LOAD // Default: off - uncomment for DEBUG (debugging only!)

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
            
        #define USE_LIGHT_RAY                   // ON/OFF - Render light ray !!Dep!!: #undef  dx12_upload_old_way
        #define SUN_LIGHT_DEMO_ANIMATION true   // Animate the beam of light
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
        #define SENTENCE_MAX_LEN    140
    #endif

    #if DX_ENGINE_LEVEL >= 25 //WINDOWS ONLY
        #if DX_ENGINE_LEVEL == 25
            #undef SCENE_TEXTURE_LIGHT
        #endif
        #if defined DX_ENGINE
            #define USE_DX_DRIVER_FONT
            #define USE_ALPHA_BLENDING
        #endif
    #endif

    #if DX_ENGINE_LEVEL >= 26
        #define USE_CUBE                    //26-
        #define USE_SPHERE                  //26-
    #endif

    #if DX_ENGINE_LEVEL >= 27
        #if defined WINDOWS_PLATFORM
        #define USE_RASTERTEK_TEXT_FONT     //27-Allow Legends for all Levels since 20: with "techno" of 28 (Text Fonts)
        //#define TEXT_TEST                 // Default: off - (debug only!)
        #else
        #define USE_RASTERTEK_TEXT_FONTV2
        #endif
    #endif

    #if DX_ENGINE_LEVEL >= 28
        #if defined WINDOWS_PLATFORM
            #if defined DX_ENGINE && defined NDEBUG
                #define ALLOW_PRINT_SCREEN_SAVE_PNG // Level:22
            #endif
            #define USE_DIRECT_INPUT                //28-we will use DX input method
            //#define USE_JOY                       // Default: off - Need to be tested...
        #endif

        #define SUN_LIGHT_DEMO_ANIMATION true
            
        #define USE_SKY_CAMERA_DOME         //28-we have sky camera in the world
        #define USE_SKYSPHERE               //28-the sky is a Sphere

        #if defined DX_ENGINE && defined NDEBUG
        #define ALLOW_PRINT_SCREEN_SAVE_PNG //28-print screen key will generate an image at DESKTOP
        #endif
    #endif

    #if DX_ENGINE_LEVEL >= 29
	//---------------------------------------------------------------------------------------
        #define SCENE_COLOR             //FORCE!
        #define SCENE_TEXTURE           //FORCE!
        #define SCENE_TEXTURE_LIGHT     //FORCE!
        #define INTRO_DEMO              // 29-Close the Intro / Demo of what was learned so far.

        #if defined ANDROID_PLATFORM
            #define USE_ANDROID_SOUND
        #endif
        #if defined WINDOWS_PLATFORM
            #define USE_WIN32_SOUND_MANAGER // 29-SOUND: Sound Manager/"ogg" Loader
            #define USE_WIN32_PLAY_MUSIC    // 29-MUSIC: "ogg" Loader
            #if DX_ENGINE_LEVEL == 29
            #define USE_DX_DRIVER_FONT  //FORCE!       
            #endif
        #endif
    #endif

    //--------------------------------------------------------------------------------------------------------------------------
    #if DX_ENGINE_LEVEL >= 30 // WORLD.XML: load "COLOR" .OBJ
        #define NO_SCENE_IMAGE_LOAD
        #undef SCENE_COLOR
        #undef SCENE_TEXTURE
        #undef SCENE_TEXTURE_LIGHT
        #undef USE_WIN32_SOUND_MANAGER
        #undef USE_WIN32_PLAY_MUSIC
        #undef INTRO_DEMO
        #define SUN_LIGHT_DEMO_ANIMATION true	//back
        #define USE_SCENE_MANAGER				//30-
        #define USE_FRUSTUM						//30-
        #undef DX12								//to be added later
    #endif

    #if DX_ENGINE_LEVEL >= 31
        #define RENDER_MULTIPLE_TEXTURES
    #endif

    #if DX_ENGINE_LEVEL >= 32
        #define RENDER_OBJ_WITH_LIGHT
    #endif

    #if DX_ENGINE_LEVEL >= 33
        #define RENDER_OBJ_WITH_ALFA                   
    #endif

    #if DX_ENGINE_LEVEL >= 34
        #define RENDER_OBJ_WITH_SPECULAR_SHININESS
    #endif

    #if DX_ENGINE_LEVEL >= 35   //35-
        #define RENDER_OBJ_WITH_TEXTURE_BUMP
    #endif

    #if DX_ENGINE_LEVEL >= 36   //36-
        #define USE_SHADOW_MAP
        #define USELIGHTSIZE 10
    #endif

    #if DX_ENGINE_LEVEL >= 38   //38-
            #define SAVEW3D
    #endif

    #if DX_ENGINE_LEVEL >= 39
        #undef SAVEW3D
        #define LOADW3D 
        #undef OPENGL3
        #undef OPENGL40
    #endif

    #if DX_ENGINE_LEVEL >= 40   //40-
        #define USE_INSTANCES
        #define USE_REAL_SUNLIGHT_DIRECTION //FOR DEBUG ONLY!
        #undef USE_LIGHT_RAY
    #endif

    #if DX_ENGINE_LEVEL >= 41   //41-
        #define USE_SHADOW_INSTANCES
        #undef USE_REAL_SUNLIGHT_DIRECTION
        #define USE_LIGHT_RAY
    #endif

    #if DX_ENGINE_LEVEL >= 42   //42-
        #define USE_SHADOW_INSTANCES_ROTATION_W_V_P
    #endif

	#if DX_ENGINE_LEVEL == 45
		#define USE_IMGUI				// ASSIMP (Animated + Character using HLSL)
		#if !defined ANDROID_PLATFORM
		#define RUN_ASMAIN true			//#define RUN_ASMAIN false //true
		#endif
	#endif

    // TERRAINS:
    //--------------------------------------------------------------------------------------------------------------------------
    //TO BE DONE! DEMO - FUTURE 43: TECH AVAILABLE:
    //#define RENDER_OBJ_WITH_ALFA          //--> _DX_ENGINE_LEVEL >= 33    //to be added later!
    #if DX_ENGINE_LEVEL >= 49   //49-
        #if _DEBUG
        #undef USE_LOADING_THREADS          //Use Thread to load Graphics
        #endif
        #define USE_REAL_SUNLIGHT_DIRECTION //Static light
        #undef USE_LIGHT_RAY                //Static light
            
        #define SCENE_GENERATEDUNDERWATER
        #undef SCENE_TERRAIN_COLLISION
        #define FIXEDBORDERHEIGHT -5.0f
    #endif

    #if DX_ENGINE_LEVEL >= 50   //50-
        #if _DEBUG
        #undef USE_LOADING_THREADS          //Use Thread to load Graphics
        #endif
        #define SCENE_GENERATEDUNDERWATER   //0
        #define SCENE_WATER_TERRAIN         //1
        #define SCENE_MAIN_TOPO_TERRAIN     //2
        #undef SCENE_TERRAIN_COLLISION
    #endif

    #if DX_ENGINE_LEVEL >= 51   //51-
        #define SCENE_FOG
        #undef SCENE_TERRAIN_COLLISION
    #endif

    #if DX_ENGINE_LEVEL >= 52   //52-
        #undef SCENE_GENERATEDUNDERWATER        //WITH BE USED AS "HEIGHT"
        #define SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
        #define SCENE_TERRAIN_WITH_NORMALS

        #undef USE_REAL_SUNLIGHT_DIRECTION  //Static light
        #define USE_LIGHT_RAY               //Static light
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
        //#define DX_INPUT_DEBUG_TEST // Default: off
    #endif
    #if DX_ENGINE_LEVEL >= 63
        #define USE_MINI_MAP
    #endif
    #if DX_ENGINE_LEVEL >= 64
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
	
	
	
	//-------------------------------------------------------------------------------------------------------
	//MAIN_RENDER:
	//-------------------------------------------------------------------------------------------------------
	#define MAIN_RENDER_TITLE           //24
	#define MAIN_RENDER_DRIVER_FONT     //25
	#define MAIN_RENDER_RASTERTEK_FONT  //27
	#define MAIN_RENDER_SKY             //28
	#define MAIN_RENDER_MAIN_OBJ        //30
	#define MAIN_RENDER_WATER           //50
	#define MAIN_RENDER_TERRAIN         //55/65
	#define MAIN_RENDER_MINIMAP         //63
	#define MAIN_RENDER_BILLBOARDS      //70/74
	#define MAIN_RENDER_ASSIMP          //82
	
	
    #if DX_ENGINE_LEVEL >= 79
        #undef  USE_IDEA_PACK
        #define USE_CYPHER_PACK
        #define MULTIPACK_PACK  TEXT("C:/WoMAengine2023/engine/data/womamulti.pack")
        #define MULTIPACK_PCK   TEXT("womamulti.pck")
    #endif
    #if DX_ENGINE_LEVEL >= 82
		#define USE_DX11_1_SETUP
        #define USE_ASSIMP_LATEST
    #endif
    #if DX_ENGINE_LEVEL >= 83
        //#define SAVE_WALK // Default: off - Updated billboard SHADER & SAVE_WALK FILME
    #endif
    #if DX_ENGINE_LEVEL >= 84
        #define LOAD_WALK
        #define SCENE_MOVIN_SKIN
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
      #if DX_ENGINE_LEVEL < 88
        #define USE_MODEL4
      #endif
    #endif
    
    #if DX_ENGINE_LEVEL >= 88
        #define USE_3RD_PERSON_CAMERA
    #endif
        
#if DX_ENGINE_LEVEL >= 89
    #define USE_MAP_EDITOR
    //#define USE_AABB_COLISION_CHECK // Default: off
#endif
    
#if DX_ENGINE_LEVEL >= 90       // Replacing Billboards with advanced gaming like objects(Low-Poly)
    #undef  USE_MAP_EDITOR
    #undef  ALLOW_CBIND_PROGRESS_BAR
    //#define USE_INSTANCES_FOR_TREES90
#endif

#if DX_ENGINE_LEVEL >= 91
	#define USE_SHADOW_THREAD	//Allow the other 2:
	#define USE_MINIMAP_REDENRING_THREAD
	#define USE_MESH_THREAD
#endif
    
#if DX_ENGINE_LEVEL >= 92
	#define USE_FASTER_BILL_SHADER			//Faster bills: v1
#endif

#if DX_ENGINE_LEVEL >= 93
	#define  NO3DBILL						// No 3D Trees
	#define GENERATE_ATLAS_INTEGRATION_DDS  // Faster bills: v2
#endif

#if DX_ENGINE_LEVEL >= 94
	// Try nothing and...:		//RELEASE: 906FPS
	//#define USE_TREE_POINTER	//v1: RELEASE: 900FPS
	#define USE_TREE_POINTERV2	//v2: RELEASE: 911FPS
#endif

#if DX_ENGINE_LEVEL >= 95
	#define USE_CURVED_SKY_PLANE
#endif
#if DX_ENGINE_LEVEL >= 96
	#define USE_CURVED_REAL_SKY_PLANE
#endif

#if DX_ENGINE_LEVEL >= 97
	#define USE_WATER_FALL
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
	#define USE_PLANET_EARTH
#endif

#endif








    //-------------------------------------------------------------------------------------------------------
	#undef  dx12_upload_old_way

	#if DX_ENGINE_LEVEL >= 19
		#define USE_MULTI_MONITOR
	#endif
	#if DX_ENGINE_LEVEL < 94
	#if DX_ENGINE_LEVEL != 76
	#undef USE_INTRO_VIDEO_DEMO
	#endif
	#endif
	#if defined WINDOWS_PLATFORM
		#define ALLOW_CBIND_PROGRESS_BAR
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

#pragma warning(pop)
