#pragma once

//------------------------------------------------------------------------------------------------------------
#define CORE_ENGINE_LEVEL 10
//------------------------------------------------------------------------------------------------------------
#if defined RELEASE
	#define USE_NETWORK
#else
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
		#define USE_PROCESS_OS_KEYS		//2
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
		//-------------------------------------------------------------------------------------------------------
		#define DX_ENGINE			//Turn on: #include "DXengine.h"

		#if DX_ENGINE_LEVEL >= 19	//DX9 DX11+DX10 DX12 OPENGL3
			//#define DX9sdk	//(ORIGINAL DX9SDK) not fully recovered/implemented

			#define DX9			//Using DX11 API
			#define DX11
			#define OPENGL3
			#define DX12		//(defined DX12 && D3D11_SPEC_DATE_YEAR > 2009)		
		#endif

		//-------------------------------------------------------------------------------------------------------
		#if defined RELEASE && DX_ENGINE_LEVEL < 20
		#error "WOMA COMPILATION ERROR: RELEASE bin is not Supported before DX_ENGINE_LEVEL 20"
		#endif
	#endif
	
#endif
