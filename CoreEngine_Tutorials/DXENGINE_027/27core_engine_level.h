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
		#define USE_FORCE_HOUR	/*24*/ /*18*/ 12
		#define USE_FORCE_MIN	00
	#endif
	#endif
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

		#if DX_ENGINE_LEVEL >= 19	//DX9 DX11+DX10 DX12 OPENGL3
			//#define DX9sdk	//(ORIGINAL DX9SDK) not fully recovered/implemented yet

			#define DX9			//Using DX11 API in backport compatibility mode
			#define DX11		//D3D11_SPEC_DATE_YEAR == 2009 || D3D11_SPEC_DATE_YEAR > 2009
			#define OPENGL3
			#define DX12		//(defined DX12 && D3D11_SPEC_DATE_YEAR > 2009)		
		#endif

		#if DX_ENGINE_LEVEL >= 20 && !defined GOTO_BASICS
			#define USE_ALLOW_MAINWINDOW_RESIZE //Allow main Window to Resize
			//#define USE_ASPECT_RATIO
			#define USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE		
		#endif

		#if DX_ENGINE_LEVEL >= 21 && DX_ENGINE_LEVEL <= 23  && !defined GOTO_BASICS
			#define ROTATE_SQUAR
			#define USE_RASTERIZER_STATE //Mandatory for DX12
			//#define USE_PRECOMPILED_SHADERS	// [ON/OFF] ON: Merge Shader's Code on .EXE / OFF: Compile in run-time
		#endif

		#if DX_ENGINE_LEVEL >= 22
			#define USE_IMAGE_BMP
			#define USE_IMAGE_JPG
			#define USE_IMAGE_PNG
			#define USE_IMAGE_TIFF
			#define USE_IMAGE_DDS
			#define USE_IMAGE_TGA
		#endif

		#if DX_ENGINE_LEVEL >= 23 && !defined GOTO_BASICS
			#define USE_LIGHT_RAY					// ON/OFF - Render light ray (_DEBUG)
		#endif

		#if DX_ENGINE_LEVEL >= 24
			#define USE_TITLE_BANNER
			#define USE_ALPHA_BLENDING
			#define USE_VIEW2D_SPRITES
			#define USE_RASTERIZER_STATE
            #define USE_DSV //for DX12
		#endif

		#if DX_ENGINE_LEVEL >= 25
			#define USE_CUBE
		#endif

		#if DX_ENGINE_LEVEL >= 26
			#define USE_SPHERE
		#endif

		#if DX_ENGINE_LEVEL >= 27
			#undef OPENGL3						//NOT implemented yet
			#define USE_RASTERTEK_TEXT_FONT		//Allow Legends for all Levels since 20: with "techno" of 28 (Text Fonts)
		#endif

		//-------------------------------------------------------------------------------------------------------
		#if defined RELEASE && DX_ENGINE_LEVEL < 20
		#error "WOMA COMPILATION ERROR: RELEASE bin is not Supported before DX_ENGINE_LEVEL 20"
		#endif

		#if defined (DX9) || defined (DX11) | defined (DX12)
			#define DX_ENGINE
		#endif
	#endif

#endif
