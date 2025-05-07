// --------------------------------------------------------------------------------------------
// Filename: woma_engine_assets.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
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

#include "level.h"
#include "tech-level.h"

//--------------------------------------------------------------------------------------------------------
#define PROJECT_NAME		TEXT("WoMAengine")
#define COMPANY_DIRECTORY	TEXT("Pmborg")
#define PROJECT_DIRECTORY	TEXT("WoMAengine2023")
#define ICON_FILE			TEXT("WoMA.ico")
#define GENERALSETTINGS		"generalsettings"

#if defined ANDROID_PLATFORM
#define FILE_REPORT_LOG		TEXT("/report.txt") // /data/user/0/com.woma/cache/report.txt
#else
#define FILE_REPORT_LOG		TEXT("report.txt")	// C:\WoMAengine2023\DXEngine_055\report.txt
#endif											// \\wsl.localhost\Ubuntu\home\pedro\projects\LinuxWoma\bin\x64\Debug\report.txt
//--------------------------------------------------------------------------------------------------------

#if CORE_ENGINE_LEVEL == 0
	#define DEMO_TITLE TEXT("00: WOMA Hello World!")
#endif

#if CORE_ENGINE_LEVEL == 1
	#define DEMO_TITLE TEXT("01: Write report.txt file")
#endif
#if CORE_ENGINE_LEVEL == 2
	#define DEMO_TITLE TEXT("02: Adding the MAIN WINDOW! (basic static version)")
#endif
#if CORE_ENGINE_LEVEL == 3
	#define DEMO_TITLE TEXT("03: Testing MINI FILE DUMPER: C:\\Users\\Public\\Documents\\<user>\\<core-level>\\<dx-level>\\_WoMA3Dengine_<DATE>.dmp")
#endif
#if CORE_ENGINE_LEVEL >= 4 
	#if CORE_ENGINE_LEVEL == 4 
	#define DEMO_TITLE TEXT("04: From now on: PRESS [F1] for: [SYSTEM MANAGER DISPLAY] and added FPS(title bar)")
	#endif
	#define MIN_WIN32_FONT_X 22
	#define MIN_WIN32_FONT_Y 25
#endif
#if CORE_ENGINE_LEVEL == 5
  #if defined LINUX_PLATFORM
	#define DEMO_ROOT_DIR TEXT("/projects/LinuxWoma005/CoreEngine_Tutorials/CoreEngine_005_PAINT_SETTINGS_XML")
  #endif
#if !defined DEMO_LEVEL
  #if defined LINUX_PLATFORM
  #else
	#define SETTINGS_FILE		TEXT("settings_005.xml")
  #endif
#endif
	#define DEMO_TITLE TEXT("05: From now on: PRESS [F6] for: [GAME SETUP] and added FILE: C:\\Users\\Public\\Documents\\<user>\\<core-level>\\<dx-level>\\settings.xml")
#endif
#if CORE_ENGINE_LEVEL == 6
  #if defined LINUX_PLATFORM
	#define DEMO_ROOT_DIR TEXT("/projects/LinuxWoma006/CoreEngine_Tutorials/CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK")
  #endif
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_006.xml")
#endif
	#define DEMO_TITLE TEXT("06: [THIS TOOL] Will pack all engine directory into: engine.pck (encoded file)")
	#if !defined WINDOWS_PLATFORM
	static_assert(false, "This Chap is for windows only!");
	#endif
#endif
#if CORE_ENGINE_LEVEL == 7
  #if defined LINUX_PLATFORM
	#define DEMO_ROOT_DIR TEXT("/projects/LinuxWoma007/CoreEngine_Tutorials/CoreEngine_007_PAINT_REALTIMECELESTIAL_ASTROCLASS")
  #endif
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_007.xml")
#endif
	#define DEMO_TITLE TEXT("07: Loading a files from engine.pck and Press [F2] for RealTime Celestial Positions of Sun and Moon accordingly with user Location")
	#define BACKGROUND_IMAGE TEXT("engine/data/basics/logotipo_small_backgroundV2.bmp")
#endif
#if CORE_ENGINE_LEVEL >= 7
	#define GEO_DATABASE TEXT("engine/data/GeoLite2-City.mmdb")
#endif
#if CORE_ENGINE_LEVEL >= 8
  #if defined LINUX_PLATFORM
	#define DEMO_ROOT_DIR TEXT("/projects/LinuxWoma008/CoreEngine_Tutorials/CoreEngine_008_PAINT_REALTIME_GPS_MAP_LOCATION")
  #endif
	#if CORE_ENGINE_LEVEL == 8
  #if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_008.xml")
  #endif
	#define DEMO_TITLE TEXT("08: From now on: PRESS [F3] for: [REAL TIME MAP] user location.")
	#endif
	#define BACKGROUND_IMAGE TEXT("engine/data/basics/logotipo_small_backgroundV2.bmp")
	#define POSITION_TARGET_IMAGE TEXT("engine/data/basics/target.bmp")
	#define WORLD_IP_TARGET_IMAGE TEXT("engine/data/original/Earth_Diffuse.bmp")
#endif
#if CORE_ENGINE_LEVEL == 9 //saveConfigSettings
  #if defined LINUX_PLATFORM
	#define DEMO_ROOT_DIR TEXT("/projects/LinuxWoma009/CoreEngine_Tutorials/CoreEngine_009_PAINT_METAR_WEATHER_REALTIME")
  #endif
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_009.xml")
#endif
	#define DEMO_TITLE TEXT("09: From now on: PRESS [F4] for: [REAL Wheather] at user location.")
#endif

#if DX_ENGINE_LEVEL == 10
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_010.xml")
#endif
	//#define DEMO_TITLE		TEXT("10: CLEAR SCREEN")
	#define DEMO_TITLE TEXT("10: PRESS [F6] for SETUP and change Driver: OPENGL, DX9, DX11, DX12: Initiate the 3D Graphic Drivers and attache the swapchain to mainwindow.")
#endif
#if DX_ENGINE_LEVEL >= 10
#define USE_THIS_GRAPHIC_CARD_ADAPTER 0
#endif

//--------------------------------------------------------------------------------------------------------
//WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM
//--------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL == 11
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_011.xml")
#endif
	#define DEMO_TITLE TEXT("11: WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM")
#endif

#if DX_ENGINE_LEVEL == 19
	#define DEMO_TITLE TEXT("19: PRESS[F6] for SETUP and change Driver : OPENGL, DX9, DX11, DX12 : Initiate the 3D Graphic Drivers and attache the swapchain to mainwindow.")
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_019.xml")
#endif
#endif

#if DX_ENGINE_LEVEL == 20
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_020.xml")
#endif
	#define DEMO_TITLE TEXT("20: OPENGL, DX9, DX11, DX12: Allow mainwindow resizing. + ALT ENTER")
#endif

#if DX_ENGINE_LEVEL == 21
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_021.xml")
#endif
	#define DEMO_TITLE TEXT("21: The Basic triangle and square using vertexes with indexes. HLSL: The COLOR shader")
#endif
#if DX_ENGINE_LEVEL >= 21 && LEVEL < 30
	#define WORLD_XML TEXT("world.xml")
#endif

#if DX_ENGINE_LEVEL == 22
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_022.xml")
#endif
	#define DEMO_TITLE TEXT("22: Loading and Rendering Textures. HLSL: The TEXTURE shader")
#endif
#if DX_ENGINE_LEVEL >= 22
	//22:
	#define LEVEL22_DEMO_TEXTURE TEXT("engine/data/basics/seafloor.bmp")

	#if !defined NO_SCENE_IMAGE_LOAD
		#define LEVEL22_IMAGE_bmp TEXT("engine/data/basics/Earth_Diffuse.bmp")
		#define LEVEL22_IMAGE_jpg TEXT("engine/data/basics/Earth_Diffuse.jpg")
		#define LEVEL22_IMAGE_png TEXT("engine/data/basics/Earth_Diffuse.png")
		#define LEVEL22_IMAGE_tif TEXT("engine/data/basics/Earth_Diffuse.tif")
		#define LEVEL22_IMAGE_dds TEXT("engine/data/basics/Earth_Diffuse.dds")
		#define LEVEL22_IMAGE_tga TEXT("engine/data/basics/Earth_Diffuse.tga")
	#endif
#endif
#if DX_ENGINE_LEVEL == 23
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_023.xml")
#endif
	#define DEMO_TITLE TEXT("23: Adding the first light, 'Diffuse Lighting'. HLSL: The LIGHT shader")
#endif
#if DX_ENGINE_LEVEL >= 23
	#define LEVEL22_DEMO_TEXTURE TEXT("engine/data/basics/seafloor.png")
#endif
#if DX_ENGINE_LEVEL == 24
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_024.xml")
#endif
	#define DEMO_TITLE TEXT("24: Adding Orthogonal Projection: Used on 2D Orthogonal Sprites")
#endif
#if DX_ENGINE_LEVEL >= 24
	#if defined ANDROID_PLATFORM
		#define DEMO_TITLE_TEXTURE TEXT("engine/data/TITLE/WOMA_ENGINE-BIG.png")
	#else
		#define DEMO_TITLE_TEXTURE TEXT("engine/data/TITLE/WOMA_ENGINE.png")
	#endif
#endif
#if DX_ENGINE_LEVEL == 25
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_025.xml")
#endif
	#define DEMO_TITLE TEXT("25: The 2D FONTS DRIVER")
#endif
#if DX_ENGINE_LEVEL == 26
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_026.xml")
#endif
	#define DEMO_TITLE TEXT("The first 3D SPHEREs")
#endif
#if DX_ENGINE_LEVEL >= 26
	#define SKY_DOME_DAY_TEXTURE TEXT("engine/data/sky/NEW_SKY_DOME.jpg")
	#define SKY_DOME_NIGHT_TEXTURE TEXT("engine/data/sky/NEW_SKY_NIGHT.jpg")
#endif
#if DX_ENGINE_LEVEL == 27
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_027.xml")
#endif
	#define DEMO_TITLE TEXT("The rastertek fonts on DX9, DX11, DX12 and OPENGL")
#endif
#if DX_ENGINE_LEVEL >= 27
	#define FONT_DATA_FILE TEXT("engine/data/fonts/008fontdata.txt")
	#define FONT_DATA_TEXTURE TEXT("engine/data/fonts/008font.png")
#endif
#if DX_ENGINE_LEVEL == 28
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_028.xml")
#endif
	#define DEMO_TITLE TEXT("The DX direct input, the Sky Camera and the Sphere Skydome")
	#define WORLD_XML TEXT("world_28.xml")
#endif

#if DX_ENGINE_LEVEL >= 29
	#if DX_ENGINE_LEVEL == 29
	#if (!defined DEMO_LEVEL) || DX_ENGINE_LEVEL == 29
		#if defined LINUX_PLATFORM
			#define SETTINGS_FILE		TEXT("settings.xml")
		#else
			#define SETTINGS_FILE		TEXT("settings_029.xml")
		#endif
	#endif

	#define DEMO_TITLE TEXT("DEMO: The music and the sound effects")
	#define WORLD_XML TEXT("world_29.xml")
	#endif

	#if defined WINDOWS_PLATFORM
		#define DEMO1_BANNER1_TEXTURE TEXT("engine/data/banners/TEXT1_WoMA_Muti_Platform_Engine_.png")
	#else
		#define DEMO1_BANNER1_TEXTURE TEXT("engine/data/banners/TEXT1_WoMA_Muti_Platform_Engine.png")
	#endif

	#define DEMO1_BANNER2_TEXTURE TEXT("engine/data/banners/TEXT2_is_a_Pmborg_Production.png")
	#define DEMO1_BANNER3_TEXTURE TEXT("engine/data/banners/TEXT3_WoMA_Code_By.png")
	#define DEMO1_BANNER4_TEXTURE TEXT("engine/data/banners/TEXT4_Pedro_Borges.png")
	#define DEMO1_BANNER5_TEXTURE TEXT("engine/data/banners/TEXT5_Music_By.png")
	#if defined WINDOWS_PLATFORM
		#define DEMO1_BANNER6_TEXTURE TEXT("engine/data/banners/TEXT6_Deceased_Superior_Technician_.png")
	#else
		#define DEMO1_BANNER6_TEXTURE TEXT("engine/data/banners/TEXT6_Deceased_Superior_Technician.png")
	#endif
	#if defined WINDOWS_PLATFORM
		#define AUDIO_FILE "engine/audio/DST-Aircord.ogg"
	#endif
	#if defined LINUX_PLATFORM
		#define AUDIO_FILE "engine/audio/DST-Aircord.wav"
	#endif
	#if defined ANDROID_PLATFORM
		#define AUDIO_FILE TEXT("DST-Aircord.wav")
		#define AUDIO_LINK TEXT("https://github.com/pmborg/WoMA3Dengine/raw/refs/heads/main/CoreEngine_Tutorials/DXENGINE_029/engine/audio/DST-Aircord.wav")
	#endif
#endif

#if DX_ENGINE_LEVEL == 30
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_030.xml")
#endif
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, using COLOR shader")
	#define WORLD_XML TEXT("world_30.xml")
#endif
#if DX_ENGINE_LEVEL >= 30
	#define SETTINGS_FILE		TEXT("settings.xml")
#endif
#if DX_ENGINE_LEVEL == 31
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, with MULTIPLE TEXTURES, using TEXTURE shader")
	#define WORLD_XML TEXT("world_31.xml")
#endif
#if DX_ENGINE_LEVEL == 32
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, the first advanced object, using LIGHT shader")
	#define WORLD_XML TEXT("world_32.xml")
#endif
#if DX_ENGINE_LEVEL == 33
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, the compond, using transparent textures ALFA MAP and ALFA COLOR")
	#define WORLD_XML TEXT("world_33.xml")
#endif
#if DX_ENGINE_LEVEL == 34
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, with SPECULAR and SHININESS")
	#define WORLD_XML TEXT("world_34.xml")
#endif
#if DX_ENGINE_LEVEL == 35
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, with BUMP MAP")
	#define WORLD_XML TEXT("world_35.xml")
#endif
#if DX_ENGINE_LEVEL == 36
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, render a SHADOW MAP")
	#define WORLD_XML TEXT("world_36.xml")
#endif
#if DX_ENGINE_LEVEL == 37
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, render chess board pieces, adding MAIN THREAD and the LOADER THREAD")
	#define WORLD_XML TEXT("world_37.xml")
#endif
#if DX_ENGINE_LEVEL == 38
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, save to W3D Woma 3D fast file FORMAT")
	#define WORLD_XML TEXT("world_38.xml")
#endif
#if DX_ENGINE_LEVEL == 39
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D (the Woma 3D file format) files up to 30x faster than OBJ files in DX LEVEl38")
	#define WORLD_XML TEXT("world_39.xml")
#endif
#if DX_ENGINE_LEVEL == 40
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D and use INSTANCES to clone objects using GPU")
	#define WORLD_XML TEXT("world_40.xml")
#endif
#if DX_ENGINE_LEVEL == 41
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D and use SHADOW INSTANCES")
	#define WORLD_XML TEXT("world_41.xml")
#endif
#if DX_ENGINE_LEVEL == 42
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D and use SHADOW INSTANCES with ROTATION per instance")
	#define WORLD_XML TEXT("world_42.xml")
#endif

#if DX_ENGINE_LEVEL >= 49 
	#if DX_ENGINE_LEVEL == 49
	#define DEMO_TITLE TEXT("49:TERRAIN: Generate under water terrain.")
	#define WORLD_XML TEXT("world_49.xml")
	#endif
	//49: Terrain[0]
	#define OCEANFLOOR_TEXTURE TEXT("engine/data/terrain/OceanFloor.jpg")
#endif
#if DX_ENGINE_LEVEL >= 50
	#if DX_ENGINE_LEVEL == 50
	#define DEMO_TITLE TEXT("50:TERRAIN: the under water terrain.")
	#define WORLD_XML TEXT("world_50.xml")
	#endif
	//50: Terrain[1]
	#define OCEANWATER_HMAP TEXT("engine/data/Terrain/heightmap512x512.bmp")
	#define OCEANWATER_TEXTURE TEXT("engine/data/Terrain/008water_tex04.jpg")
	//    Terrain[2]
	#define TERRAIN_LEVEL50_HMAP TEXT("engine/data/Terrain/011map1_heightmap01_v7.bmp")
	#define TERRAIN_LEVEL50_TEXTURE TEXT("engine/data/scene30/grassTexture.jpg")
#endif
#if DX_ENGINE_LEVEL == 51
	#define DEMO_TITLE TEXT("51:TERRAIN: With fog")
	#define WORLD_XML TEXT("world_51.xml")
#endif
#if DX_ENGINE_LEVEL == 52
	#define DEMO_TITLE TEXT("52:TERRAIN: With Light Shader + Add Normals + Add Index(s)")
	#define WORLD_XML TEXT("world_52.xml")
#endif
#if DX_ENGINE_LEVEL >= 53
	#if DX_ENGINE_LEVEL == 53
	#define DEMO_TITLE TEXT("53:TERRAIN: With extra color terrain (with terrain collision)")
	#define WORLD_XML TEXT("world_53.xml")
	#endif
	#define TERRAIN_LEVEL53_COLOR_MAP XMFLOAT3 MAP_COLOR(10,35,10);
#endif
#if DX_ENGINE_LEVEL == 54
	#define DEMO_TITLE TEXT("54:TERRAIN: Water waves (with terrain collision)")
	#define WORLD_XML TEXT("world_54.xml")
#endif
#if DX_ENGINE_LEVEL >= 55
	#define SETTINGS_FILE		TEXT("settings.xml")
	#if DX_ENGINE_LEVEL == 55
	#define DEMO_TITLE TEXT("55:TERRAIN: 256x256 with Slope Texture Shader")
	#define WORLD_XML TEXT("world_55.xml")
	#endif
	//55:
	#define TERRAIN_SLOP_GRASS_TEXTURE	TEXT("engine/data/Terrain/017grass.jpg")
	#define TERRAIN_SLOP_ROCK_TEXTURE	TEXT("engine/data/Terrain/017rock.jpg")
	#define TERRAIN_SLOP_SLOPE_TEXTURE	TEXT("engine/data/Terrain/017slope.jpg")
	#define TERRAIN_SLOP_ROCK2_TEXTURE	TEXT("engine/data/Terrain/013rock_2_4w.bmp")
#endif

//------------------------------------------------------------------------------------------------------------------

#if DX_ENGINE_LEVEL == 60
	#define DEMO_TITLE TEXT("60: TERRAIN: 256x256 multi-layer")
	#define WORLD_XML TEXT("world_60.xml")
#endif

#if DX_ENGINE_LEVEL == 61
	#define DEMO_TITLE TEXT("61: TERRAIN: 512x512 advanced multi-layer")
	#define WORLD_XML TEXT("world_61.xml")
#endif

#if DX_ENGINE_LEVEL == 62
	#define DEMO_TITLE TEXT("62: The MAIN MAP key:M")
	#define WORLD_XML TEXT("world_62.xml")
#endif
#if DX_ENGINE_LEVEL >= 62
	#define DEMO_MAINMMAP_TEXTURE TEXT("engine/data/scene62MAP/mapFramev4.bmp")
#endif
#if DX_ENGINE_LEVEL == 63
	#define DEMO_TITLE TEXT("63: The MINI-MAP")
	#define WORLD_XML TEXT("world_63.xml")
#endif
#if DX_ENGINE_LEVEL >= 63
	#define DEMO_MINIMMAP_TEXTURE TEXT("engine/data/scene63MAP/015MiniMap.png")
	#define DEMO_MINIMMAP_BORDER_TEXTURE TEXT("engine/data/scene63MAP/015MiniMap.png")
	#define DEMO_MINIMMAP_ARROW_TEXTURE TEXT("engine/data/scene63MAP/015ArrowV2.png")
#endif
//----------------------------------------------------------------------------------------------------------------------------------								
#if DX_ENGINE_LEVEL == 64
	#define DEMO_TITLE TEXT("64: Add a model with 2 pass and add a 3D static animated model")
	#define WORLD_XML TEXT("world_64.xml")
#endif
#if DX_ENGINE_LEVEL == 65
	#define DEMO_TITLE TEXT("65: Walking on Terrain.")
	#define WORLD_XML TEXT("world_65.xml")
#endif

//----------------------------------------------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL == 70
	#define DEMO_TITLE TEXT("70: BILLBOARD for Trees / Flowers, (LIGHT and Fog)")
	#define WORLD_XML TEXT("world_70.xml")
	//#define BILLBOARD_MODEL TEXT("engine/data/scene30/squareTexture.obj")
#endif

#if DX_ENGINE_LEVEL >= 70
	#define BILLBOARD_TERRAIN TEXT("engine/data/scene73grass/t_025TerrainMappingV4.bmp")
	#define BILLBOARD_MODEL TEXT("engine/data/scene70Bill/060square.obj")

	#define BILL_TREE_0 TEXT("engine/data/scene70Bill/060SH10_1bill.tif")	// 0 TREEs
	#define BILL_TREE_1 TEXT("engine/data/scene70Bill/060SH10_2bill.tif")	// 1
	#define BILL_TREE_2 TEXT("engine/data/scene70Bill/060SH10_3bill.tif")	// 2
	#define BILL_TREE_3 TEXT("engine/data/scene70Bill/060EU03a_bill.tif")	// 3
	#define BILL_TREE_4 TEXT("engine/data/scene70Bill/060EU03m_bill.tif")	// 4
	#define BILL_TREE_5 TEXT("engine/data/scene70Bill/060EU03y_bill.tif")	// 5

	#define BILL_FLOWER_0 TEXT("engine/data/scene70Bill/060rosa.png") 		// 6	//FLOWERs
	#define BILL_FLOWER_1 TEXT("engine/data/scene70Bill/060tulia.png") 		// 7
	#define BILL_FLOWER_2 TEXT("engine/data/scene70Bill/060flor1.png") 		// 8
	#define BILL_FLOWER_3 TEXT("engine/data/scene70Bill/060girasol.png") 	// 9 
	#define BILL_FLOWER_4 TEXT("engine/data/scene70Bill/060flor2.png") 		//10
#endif
#if DX_ENGINE_LEVEL == 71
	#define DEMO_TITLE TEXT("71: BILLBOARD for FENCES, (LIGHT and Fog)")
	#define WORLD_XML TEXT("world_71.xml")
#endif
#if DX_ENGINE_LEVEL >= 71
	#define BILLBOARD_FENCE_MODEL TEXT("engine/data/scene70Bill/fence.obj")
#endif
#if DX_ENGINE_LEVEL == 72
	#define DEMO_TITLE TEXT("72: FIRE (Add 3D Sound / Effects)")
	#define WORLD_XML TEXT("world_72.xml")
#endif
#if DX_ENGINE_LEVEL >= 72
	#define BILLBOARD_FIRE_MODEL TEXT("engine/data/scene72Fire/072fire.obj")
#endif
#if DX_ENGINE_LEVEL == 73
	#define DEMO_TITLE TEXT("73: WINDY GRASS")
	#define WORLD_XML TEXT("world_73.xml")
#endif																															
#if DX_ENGINE_LEVEL >= 73
	#define BILLBOARD_GRASS_MODEL TEXT("engine/data/scene73grass/grass.obj")
	#define BILL_GRASS TEXT("engine/data/scene73grass/Grass_Patch_3.png")	//11
#endif
#if DX_ENGINE_LEVEL == 74
	#define DEMO_TITLE TEXT("74: CROSS BILLBOARDs and CLONE BILLBOARDs")
	#define WORLD_XML TEXT("world_74.xml")
#endif
#if DX_ENGINE_LEVEL >= 74
	#define BILLBOARD_BUSH_MODEL TEXT("engine/data/scene74bush/bush.obj")
	#define BILL_BUSH_0 TEXT("engine/data/scene74bush/billboardgrass0001.png") 		//21
#endif
#if DX_ENGINE_LEVEL == 75
	#define DEMO_TITLE TEXT("75: UTIL: PROGRESS BAR FOR TERRAIN LOADING")
	#define WORLD_XML TEXT("world_75.xml")
	#define VIDEO_INTRO TEXT("engine/data/76video/Logo.mp4")													 
#endif																	  
#if DX_ENGINE_LEVEL == 76
	#define DEMO_TITLE TEXT("76: UTIL: INTRO VIDEO DEMO")
	#define WORLD_XML TEXT("world_76.xml")
#endif
#if DX_ENGINE_LEVEL >= 76
	#define VIDEO_INTRO TEXT("engine/data/75video/Logo.mp4")
#endif
#if DX_ENGINE_LEVEL == 77
	#define DEMO_TITLE TEXT("77: Use instances for TREEs")
	#define WORLD_XML TEXT("world_77.xml")
#endif
#if DX_ENGINE_LEVEL >= 77
    #define BILL_GS TEXT("engine/data/77GS/maple-tree.obj")
#endif
#if DX_ENGINE_LEVEL == 78
	#define DEMO_TITLE TEXT("78: Check 'WORLD.XML' Object collision")
	#define WORLD_XML TEXT("world_78.xml")
#endif

#if DX_ENGINE_LEVEL == 80
	#define DEMO_TITLE TEXT("80: ASSIMP: Loading a static mesh")
	#define WORLD_XML TEXT("world_80.xml")
#endif
#if DX_ENGINE_LEVEL == 81
	#define DEMO_TITLE TEXT("81: ASSIMP: Loading an animated mesh")
	#define WORLD_XML TEXT("world_81.xml")
#endif
#if DX_ENGINE_LEVEL == 82
	#define DEMO_TITLE TEXT("82: ASSIMP: Animated / walking mesh on the terrain, following a path")
	#define WORLD_XML TEXT("world_82.xml")
#endif