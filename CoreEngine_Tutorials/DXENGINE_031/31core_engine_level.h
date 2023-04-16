#pragma once

//------------------------------------------------------------------------------------------------------------
#define CORE_ENGINE_LEVEL 10
//------------------------------------------------------------------------------------------------------------
#if defined NDEBUG
	#define USE_NETWORK
#endif
#if !defined USE_NETWORK
	#if CORE_ENGINE_LEVEL > 9
	#if defined _DEBUG 
		//#define USE_FORCE_HOUR	/*24*/ /*18*/ 12
		//#define USE_FORCE_MIN	00
	#endif
	#endif
#endif

#if defined _NOT //def RELEASE
	#define USE_MAIN_THREAD		// ON/OFF (Note: Dont work on openGL, NOTE!!! This is buggy with maximize!!! Dont use for now
	// Solution? https://msdn.microsoft.com/en-us/library/windows/desktop/ee417025.aspx
#endif

#if defined _DEBUG
	//#define VERBOSE_MEMORY_DEBUG			// [ON/OFF]: used (woma_macros.h)
#endif

//------------------------------------------------------------------------------------------------------------
#if !defined GOTO_BASICS
	#if CORE_ENGINE_LEVEL >= 1
		#define USE_LOG_MANAGER		//1
	#endif

	#if CORE_ENGINE_LEVEL >= 2

	#endif

	#if CORE_ENGINE_LEVEL >= 3
		#define USE_PROCESS_OS_KEYS	//2
		#define USE_MINIDUMPER		//3
		#define USE_WOMA_EXCEPTION	//3
	#endif

	#if CORE_ENGINE_LEVEL >= 4
		#define USE_SYSTEM_CHECK	//4
		#define USE_TIMER_CLASS		//4
		#define ALLOW_LOADING_SPLASH//4
  		#define USE_CHECK_IO		//4
	  #endif

	#if CORE_ENGINE_LEVEL >= 5
		#define USE_TINYXML_LOADER	//5
		#define CLIENT_SCENE_SETUP	//CoreEngine_005 (PAINT+settings.xml)
	#endif

	#if CORE_ENGINE_LEVEL >= 6
		#define USE_IDEA_PACK		//6
	#endif

	#if CORE_ENGINE_LEVEL >= 7
		#define USE_ASTRO_CLASS     //CoreEngine_007 (PAINT RealTimeCelestial)		
		#if CORE_ENGINE_LEVEL == 7
			#define USE_NETWORK
		#endif
	#endif

	#if CORE_ENGINE_LEVEL >= 8
		#define USE_GPS_MAP			//CoreEngine_008 (PAINT RealTimeMapGPSPosition)	
		#if CORE_ENGINE_LEVEL == 8
			#define USE_NETWORK
		#endif
	#endif

	#if CORE_ENGINE_LEVEL >= 9
		#define USE_METARCLASS		//CoreEngine_009 (PAINT RealTimeWeather)		
		#if CORE_ENGINE_LEVEL == 9
			#define USE_NETWORK
		#endif
	#endif

	//-------------------------------------------------------------------------------------------------------
	#if CORE_ENGINE_LEVEL == 10

		#if DX_ENGINE_LEVEL >= 19
			//#define DX9sdk		//(ORIGINAL DX9SDK) not fully recovered/implemented yet
			#define DX9				//19- Using DX11 API in backport compatibility mode
			#define DX11			//19- D3D11_SPEC_DATE_YEAR == 2009 || D3D11_SPEC_DATE_YEAR > 2009
			#define OPENGL3			//19- OPENGL 3/4
			#define DX12			//19- (defined DX12 && D3D11_SPEC_DATE_YEAR > 2009)		
		#endif

		#if DX_ENGINE_LEVEL >= 20 && !defined GOTO_BASICS
			#define USE_ALLOW_MAINWINDOW_RESIZE				//20- Allow main Window to Resize
			#define USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE	//20-	
			//#define USE_ASPECT_RATIO
		#endif

		#if DX_ENGINE_LEVEL >= 21
			#define ROTATE_SQUARE				//21-
			#define USE_RASTERIZER_STATE		//21- Mandatory for DX12
			//#define USE_PRECOMPILED_SHADERS	// [ON/OFF] ON: Merge Shader's Code on .EXE / OFF: Compile in run-time
		#endif

		#if DX_ENGINE_LEVEL >= 22
			#define USE_IMAGE_BMP				//22
			#define USE_IMAGE_JPG				//22
			#define USE_IMAGE_PNG				//22
			#define USE_IMAGE_TIFF				//22
			#define USE_IMAGE_DDS				//22
			#define USE_IMAGE_TGA				//22
		#endif

		#if DX_ENGINE_LEVEL >= 23 && !defined GOTO_BASICS
			#define USE_LIGHT_RAY				//23- ON/OFF - Render light ray (_DEBUG)
		#endif

		#if DX_ENGINE_LEVEL >= 24
			#define USE_TITLE_BANNER			//24- 
			#define USE_ALPHA_BLENDING			//24- 
			#define USE_VIEW2D_SPRITES			//24- 
			#define USE_RASTERIZER_STATE		//24- 
            #define USE_DSV						//24- for DX12
		#endif

		#if DX_ENGINE_LEVEL >= 25
			#define USE_CUBE					//25-
		#endif

		#if DX_ENGINE_LEVEL >= 26
			#define USE_SPHERE					//26-
		#endif

		#if DX_ENGINE_LEVEL >= 27
			#define USE_RASTERTEK_TEXT_FONT		//27-Allow Legends for all Levels since 20: with "techno" of 28 (Text Fonts)
		#endif

		#if DX_ENGINE_LEVEL >= 28
			#define USE_SKY_DOME				//28-we have sky in the world
			#define USE_SKYSPHERE				//28-the sky is a sphere
			#define USE_DIRECT_INPUT			//28-we will use DX input method
			#define ALLOW_PRINT_SCREEN_SAVE_PNG //28-print screen key will generate an image at DESKTOP
		#endif

		#if DX_ENGINE_LEVEL >= 29
			#define USE_SOUND_MANAGER	// 29-SOUND: Sound Manager/"ogg" Loader
			#define USE_PLAY_MUSIC		// 29-MUSIC: "ogg" Loader
			#define INTRO_DEMO			// 29-Close the Intro / Demo of what was learned so far.
			#define FORCE_LOAD_ALL		true
		#endif

		#if DX_ENGINE_LEVEL >= 30
			#undef OPENGL3				//to be added later
			#undef USE_SOUND_MANAGER
			#undef USE_PLAY_MUSIC
			#undef INTRO_DEMO
			#define FORCE_LOAD_ALL		false
			#define USE_SCENE_MANAGER	//30-
			#define USE_FRUSTRUM		//30-
		#endif

		#if DX_ENGINE_LEVEL >= 31
			#define RENDER_MULTIPLE_TEXTURES
			#if defined _NOT
			//AQUI!: Search for this string in order to Discover the secret of passing multiple textures in DX12
			#endif
		#endif

		//-------------------------------------------------------------------------------------------------------
		#if defined RELEASE && DX_ENGINE_LEVEL < 29
		#error "WOMA COMPILATION ERROR: RELEASE bin is not Supported before DX_ENGINE_LEVEL 29"
		#endif

		#if defined INTRO_DEMO
			#define USE_NETWORK
		#endif

		#if defined (DX9) || defined (DX11) | defined (DX12)
			#define DX_ENGINE
		#endif
	#endif

#endif
