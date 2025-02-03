// --------------------------------------------------------------------------------------------
// Filename: woma_engine_assets.h
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

#include "level.h"
#include "tech-level.h"

//--------------------------------------------------------------------------------------------------------
#define COMPANY_DIRECTORY	TEXT("Pmborg")
#define PROJECT_DIRECTORY	TEXT("WoMA3Dengine")
#define ICON_FILE			TEXT("WoMA.ico")
#define GENERALSETTINGS		"generalsettings"

#if defined ANDROID_PLATFORM
#define FILE_REPORT_LOG		TEXT("/report.txt") // /data/user/0/com.woma/cache/report.txt
#else
#define FILE_REPORT_LOG		TEXT("report.txt")	// C:\WoMAengine2023\DXEngine_055\report.txt
#endif											// \\wsl.localhost\Ubuntu\home\pedro\projects\LinuxWoma\bin\x64\Debug\report.txt
//--------------------------------------------------------------------------------------------------------

#if LEVEL == 0
	#define DEMO_TITLE TEXT("00: WOMA Hello World!")
#endif

#if LEVEL == 1
	#define DEMO_TITLE TEXT("01: Write report.txt file")
	//#if !defined WINDOWS_PLATFORM
	//static_assert(false, "This Chap is for windows only!");
	//#endif
#endif
#if LEVEL == 2
	#define DEMO_TITLE TEXT("02: Adding the MAIN WINDOW! (basic static version)")
#endif
#if LEVEL == 3
	#define DEMO_TITLE TEXT("03: Testing MINI FILE DUMPER: C:\\Users\\Public\\Documents\\<user>\\<core-level>\\<dx-level>\\_WoMA3Dengine_<DATE>.dmp")
#endif
#if LEVEL >= 4 
	#if LEVEL == 4 
	#define DEMO_TITLE TEXT("04: From now on: PRESS [F1] for: [SYSTEM MANAGER DISPLAY] and added FPS(title bar)")
	#endif
	#define MIN_WIN32_FONT_X 22
	#define MIN_WIN32_FONT_Y 25
#endif
#if LEVEL == 5
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_005.xml")
#endif
	#define DEMO_TITLE TEXT("05: From now on: PRESS [F6] for: [GAME SETUP] and added FILE: C:\\Users\\Public\\Documents\\<user>\\<core-level>\\<dx-level>\\settings.xml")
#endif
#if LEVEL == 6
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_006.xml")
#endif
	#define DEMO_TITLE TEXT("06: [THIS TOOL] Will pack all engine directory into: engine.pck (encoded file)")
	#if !defined WINDOWS_PLATFORM
	static_assert(false, "This Chap is for windows only!");
	#endif
#endif
#if LEVEL == 7
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_007.xml")
#endif
	#define DEMO_TITLE TEXT("07: Loading a files from engine.pck and Press [F2] for RealTime Celestial Positions of Sun and Moon accordingly with user Location")
	#define BACKGROUND_IMAGE TEXT("engine/data/basics/logotipo_small_backgroundV2.bmp")
#endif
#if LEVEL >= 8
	#if LEVEL == 8
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_008.xml")
#endif
	#define DEMO_TITLE TEXT("08: From now on: PRESS [F3] for: [REAL TIME MAP] user location.")
	#endif
	#define BACKGROUND_IMAGE TEXT("engine/data/basics/logotipo_small_backgroundV2.bmp")
	#define POSITION_TARGET_IMAGE TEXT("engine/data/basics/target.bmp")
	#define WORLD_IP_TARGET_IMAGE TEXT("engine/data/original/Earth_Diffuse.bmp")
#endif
#if LEVEL == 9 //saveConfigSettings
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_009.xml")
#endif
	#define DEMO_TITLE TEXT("09: From now on: PRESS [F4] for: [REAL Wheather] at user location.")
#endif

#if LEVEL == 10
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_010.xml")
#endif
	//#define DEMO_TITLE		TEXT("10: CLEAR SCREEN")
	#define DEMO_TITLE TEXT("10: PRESS [F6] for SETUP and change Driver: OPENGL, DX9, DX11, DX12: Initiate the 3D Graphic Drivers and attache the swapchain to mainwindow.")
#endif
#if LEVEL >= 10
#define USE_THIS_GRAPHIC_CARD_ADAPTER 0
#endif

//--------------------------------------------------------------------------------------------------------
//WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM
//--------------------------------------------------------------------------------------------------------
#if LEVEL == 11
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_011.xml")
#endif
	#define DEMO_TITLE TEXT("11: WINDOWS_PLATFORM + LINUX_PLATFORM + ANDROID_PLATFORM")
#endif

#if LEVEL == 19
	#define DEMO_TITLE TEXT("19: OPENGL, DX9, DX11, DX12: ALT ENTER")
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_019.xml")
#endif
#endif

#if LEVEL == 20
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_020.xml")
#endif
	#define DEMO_TITLE TEXT("20: OPENGL, DX9, DX11, DX12: Allow mainwindow resizing. + ALT ENTER")
#endif

#if LEVEL == 21
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_021.xml")
#endif
	#define DEMO_TITLE TEXT("21: The Basic triangle and squar using vertexes with indexes. HLSL: The COLOR shader")
#endif
#if LEVEL >= 21 && LEVEL < 30
	#define WORLD_XML TEXT("world.xml")
#endif

#if LEVEL == 22
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_022.xml")
#endif
	#define DEMO_TITLE TEXT("22: Loading and Rendering Textures. HLSL: The TEXTURE shader")
#endif
#if LEVEL >= 22
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
#if LEVEL == 23
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_023.xml")
#endif
	#define DEMO_TITLE TEXT("23: Adding the first light, 'Diffuse Lighting'. HLSL: The LIGHT shader")
#endif
#if LEVEL >= 23
	#define LEVEL22_DEMO_TEXTURE TEXT("engine/data/basics/seafloor.png")
#endif
#if LEVEL == 24
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_024.xml")
#endif
	#define DEMO_TITLE TEXT("24: Adding Orthogonal Projection: Used on 2D Orthogonal Sprites")
#endif
#if LEVEL >= 24
	#if defined ANDROID_PLATFORM
		#define DEMO_TITLE_TEXTURE TEXT("engine/data/TITLE/WOMA_ENGINE-BIG.png")
	#else
		#define DEMO_TITLE_TEXTURE TEXT("engine/data/TITLE/WOMA_ENGINE.png")
	#endif
#endif
#if LEVEL == 25
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_025.xml")
#endif
	#define DEMO_TITLE TEXT("25: The 2D FONTS DRIVER")
#endif
#if LEVEL == 26
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_026.xml")
#endif
	#define DEMO_TITLE TEXT("The first 3D SPHEREs")
#endif
#if LEVEL >= 26
	#define SKY_DOME_DAY_TEXTURE TEXT("engine/data/sky/NEW_SKY_DOME.jpg")
	#define SKY_DOME_NIGHT_TEXTURE TEXT("engine/data/sky/NEW_SKY_NIGHT.jpg")
#endif
#if LEVEL == 27
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_027.xml")
#endif
	#define DEMO_TITLE TEXT("The rastertek fonts on DX9, DX11, DX12 and OPENGL")
#endif
#if LEVEL >= 27
	#define FONT_DATA_FILE TEXT("engine/data/fonts/008fontdata.txt")
	#define FONT_DATA_TEXTURE TEXT("engine/data/fonts/008font.png")
#endif
#if LEVEL == 28
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_028.xml")
#endif
	#define DEMO_TITLE TEXT("The DX direct input, the Sky Camera and the Sphere Skydome")
	#define WORLD_XML TEXT("world_28.xml")
#endif
#if LEVEL >= 29
	#if LEVEL == 29
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_029.xml")
#endif
	#define DEMO_TITLE TEXT("DEMO: The music and the sound effects")
	#define WORLD_XML TEXT("world_29.xml")
	#endif
	#define DEMO1_BANNER1_TEXTURE TEXT("engine/data/banners/TEXT1_WoMA_Muti_Platform_Engine.png")
	#define DEMO1_BANNER2_TEXTURE TEXT("engine/data/banners/TEXT2_is_a_Pmborg_Production.png")
	#define DEMO1_BANNER3_TEXTURE TEXT("engine/data/banners/TEXT3_WoMA_Code_By.png")
	#define DEMO1_BANNER4_TEXTURE TEXT("engine/data/banners/TEXT4_Pedro_Borges.png")
	#define DEMO1_BANNER5_TEXTURE TEXT("engine/data/banners/TEXT5_Music_By.png")
	#define DEMO1_BANNER6_TEXTURE TEXT("engine/data/banners/TEXT6_Deceased_Superior_Technician.png")
	#if defined WINDOWS_PLATFORM
	#define AUDIO_FILE "engine/audio/DST-Aircord.ogg"
	#endif
	#if defined ANDROID_PLATFORM
	#define AUDIO_FILE "DST-Aircord.wav"
	#endif
#endif

#if LEVEL == 30
#if !defined DEMO_LEVEL
	#define SETTINGS_FILE		TEXT("settings_030.xml")
#endif
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, using COLOR shader")
	#define WORLD_XML TEXT("world_30.xml")
#endif
#if LEVEL >= 30
	#define SETTINGS_FILE		TEXT("settings.xml")
#endif
#if LEVEL == 31
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, with MULTIPLE TEXTURES, using TEXTURE shader")
	#define WORLD_XML TEXT("world_31.xml")
#endif
#if LEVEL == 32
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, the first advanced object, using LIGHT shader")
	#define WORLD_XML TEXT("world_32.xml")
#endif
#if LEVEL == 33
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, the compond, using transparent textures ALFA MAP and ALFA COLOR")
	#define WORLD_XML TEXT("world_33.xml")
#endif
#if LEVEL == 34
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, with SPECULAR and SHININESS")
	#define WORLD_XML TEXT("world_34.xml")
#endif
#if LEVEL == 35
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, with BUMP MAP")
	#define WORLD_XML TEXT("world_35.xml")
#endif
#if LEVEL == 36
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, render a SHADOW MAP")
	#define WORLD_XML TEXT("world_36.xml")
#endif
#if LEVEL == 37
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, render chess board pieces, adding MAIN THREAD and the LOADER THREAD")
	#define WORLD_XML TEXT("world_37.xml")
#endif
#if LEVEL == 38
	#define DEMO_TITLE TEXT("WORLD.XML: load OBJ 3D file format, save to W3D Woma 3D fast file FORMAT")
	#define WORLD_XML TEXT("world_38.xml")
#endif
#if LEVEL == 39
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D (the Woma 3D file format) files up to 30x faster than OBJ files in DX LEVEl38")
	#define WORLD_XML TEXT("world_39.xml")
#endif
#if LEVEL == 40
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D and use INSTANCES to clone objects using GPU")
	#define WORLD_XML TEXT("world_40.xml")
#endif
#if LEVEL == 41
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D and use SHADOW INSTANCES")
	#define WORLD_XML TEXT("world_41.xml")
#endif
#if LEVEL == 42
	#define DEMO_TITLE TEXT("WORLD.XML: load W3D and use SHADOW INSTANCES with ROTATION per instance")
	#define WORLD_XML TEXT("world_42.xml")
#endif
#if LEVEL >= 49 
	#if LEVEL == 49
	#define DEMO_TITLE TEXT("49:TERRAIN: Generate under water terrain.")
	#define WORLD_XML TEXT("world_49.xml")
	#endif
	//49: Terrain[0]
	#define OCEANFLOOR_TEXTURE TEXT("engine/data/terrain/OceanFloor.jpg")
#endif
#if LEVEL >= 50
	#if LEVEL == 50
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
#if LEVEL == 51
	#define DEMO_TITLE TEXT("51:TERRAIN: With fog")
	#define WORLD_XML TEXT("world_51.xml")
#endif
#if LEVEL == 52
	#define DEMO_TITLE TEXT("52:TERRAIN: With Light Shader + Add Normals + Add Index(s)")
	#define WORLD_XML TEXT("world_52.xml")
#endif
#if LEVEL >= 53
	#if LEVEL == 53
	#define DEMO_TITLE TEXT("53:TERRAIN: With extra color terrain (extra Green in this example)")
	#define WORLD_XML TEXT("world_53.xml")
	#endif
	#define TERRAIN_LEVEL53_COLOR_MAP XMFLOAT3 MAP_COLOR(10,35,10);
	
#endif
#if LEVEL == 54
	#define DEMO_TITLE TEXT("54:TERRAIN: Water waves")
	#define WORLD_XML TEXT("world_54.xml")
#endif
#if LEVEL >= 55
	#define SETTINGS_FILE		TEXT("settings.xml")
	#if LEVEL == 55
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
//BASED ON: G:\DRIVE_MY_SOURCE_CODE\Dx11Engine3D\Dx11Engine3Dx64
#if DX_ENGINE_LEVEL >= 56 //TUTORIAL_CHAP 19 - TERRAIN Texture Mapping ........................ 019terrain_fog_slope_detail_mapping.hlsl
	//TODO!!half done
#endif

#if DX_ENGINE_LEVEL >= 57 //TUTORIAL_CHAP 21 - TERRAIN Bump Mapping ........................... 021terrain.hlsl
#endif
#if DX_ENGINE_LEVEL >= 58 //TUTORIAL_CHAP 22 - TERRAIN Texture + Alpha + Bump Mapping ......... 022terrain_fog_slope_detail_mapping.hlsl
#endif

#if DX_ENGINE_LEVEL >= 59 //TUTORIAL_CHAP 23 - TERRAIN Text. + Alpha + Bump + Light Mapping ... 023terrain_fog_slope_detail_mapping.hlsl
#endif

#if LEVEL == 60
	#define DEMO_TITLE TEXT("60:TERRAIN: 256x256 DEMO")
	#define WORLD_XML TEXT("world_60.xml")
#endif

#if LEVEL == 61
	#define DEMO_TITLE TEXT("61:TERRAIN: 512x512 DEMO")
	#define WORLD_XML TEXT("world_61.xml")
#endif

#if LEVEL == 62
	#define DEMO_TITLE TEXT("62: The MAIN MAP key:M")
	#define WORLD_XML TEXT("world_62.xml")
#endif
#if LEVEL >= 62
	#define DEMO_MAINMMAP_TEXTURE TEXT("engine/data/scene62MAP/mapFramev4.bmp")
#endif
#if LEVEL == 63
	#define DEMO_TITLE TEXT("63: The MINI-MAP")
	#define WORLD_XML TEXT("world_63.xml")
#endif
#if LEVEL >= 63
	#define DEMO_MINIMMAP_TEXTURE TEXT("engine/data/scene63MAP/015MiniMap.png")
	#define DEMO_MINIMMAP_BORDER_TEXTURE TEXT("engine/data/scene63MAP/015MiniMap.png")
	#define DEMO_MINIMMAP_ARROW_TEXTURE TEXT("engine/data/scene63MAP/015ArrowV2.png")
#endif
#if LEVEL == 64
	#define DEMO_TITLE TEXT("64: ASSIMP")
	#define WORLD_XML TEXT("world_64.xml")
#endif


//----------------------------------------------------------------------------------------------------------------------------------
#if LEVEL == 70 //Compound model with Alfa
	#define DEMO_TITLE TEXT("60:TERRAIN: 256x256 with compound")
	#define WORLD_XML TEXT("world_60.xml")
#endif
#if LEVEL == 71
	#define DEMO_TITLE TEXT("61:TERRAIN: 256x256 with compound and colision")
	#define WORLD_XML TEXT("world_61.xml")
#endif

#if DX_ENGINE_LEVEL >= 80 //25 //IMGUI!
	#if defined ANDROID_PLATFORM
		#define DEMO_SETTINGS_ICON TEXT("engine/data/basics/settings_929846-big.png")
	#else
		#define DEMO_SETTINGS_ICON TEXT("engine/data/basics/settings_929846.png")
	#endif
#endif



/*
#if defined _MSC_VER and defined _DEBUG //LETS DEBUG ONE:
#undef DX9sdk
#undef DX9
#undef DX10
#undef DX11
#undef DX12
#undef GLES2
#undef GLES3
#undef GLES31
#undef OPENGL21
#undef OPENGL3
#undef OPENGL40
#endif
*/