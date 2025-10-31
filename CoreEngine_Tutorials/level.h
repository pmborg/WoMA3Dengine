// --------------------------------------------------------------------------------------------
// Filename: level.h
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

#if defined DEMO_LEVEL
    #define LEVEL DEMO_LEVEL
    #define SETTINGS_FILE TEXT("settings.xml")
#else
    //FOR WINDOWS-LEVEL55 or Android or Linux!
    //--------------------------------------------------------------------------------------------------------------------------
    // WINDOWS + LINUX CORE LEVEL:
    //--------------------------------------------------------------------------------------------------------------------------
    #if defined WINDOWS_PLATFORM || defined LINUX_PLATFORM || defined ANDROID_PLATFORM
    //#define LEVEL  0  // OSengine - The basic pillar foundations of the 3D WoMA Engine Multi - Platform
    //#define LEVEL  1  // FOR WINDOWS + LINUX: Memory leak detector
                        // For all: Write log on file: report.txt
    //#define LEVEL  2  // FOR WINDOWS + LINUX: Adding the MAIN WINDOW! (basic hello world version)
    #endif              // FOR ANDROID: 

    #if defined WINDOWS_PLATFORM || defined LINUX_PLATFORM
    //#define LEVEL  3 // FOR WINDOWS: [TOOL]: MINI EXCEPTION FILE DUMPER
    //#define LEVEL  4 // FOR WINDOWS: "F1" on LINUX (REPORT.txt only!) TINYXML + OS_KEYS + SYSTEM_CHECK + CHECK_IO + TIMER + The User International Language 
    #endif

    //#define LEVEL  5 // FOR WINDOWS: "F6" GAME SETUP + MAIN WINDOW! (with resize)
    //#define LEVEL  6 // FOR WINDOWS: [TOOL] IDEA: C:\WoMA3Dengine\windows_engine\windows.pack + windows.pck (IDEA)
    //#define LEVEL  7 // FOR WINDOWS: "F2" Load engine.pck (IDEA+ZIP) use image on background + RealTime Celestial Positions of Sun and Moon
    //#define LEVEL  8 // FOR WINDOWS: "F3" REAL TIME MAP GEOPOSITION (initWorld), test with VPN!
    //#define LEVEL  9 // FOR WINDOWS: "F4" REAL TIME Weather

    #if defined WINDOWS_PLATFORM || defined LINUX_PLATFORM || defined ANDROID_PLATFORM
    //--------------------------------------------------------------------------------------------------------------------------
    // |WINDOWS| + |LINUX| + |ANDROID| DX/OPENGL LEVEL:
    //--------------------------------------------------------------------------------------------------------------------------
    //#define LEVEL 10  // For all Platforms: The Main window + Clean Screen + WITH SETUP
                        // BASIC NOTE: [Keys WINDOWS Only] F1, F2, F3, F4, and "F5" + "F6"

    //#define LEVEL 11  // For all Platforms: The Main window + Clean Screen + NO SETUP NOW!
                        // 15636 fps DX11
                        //  4504 fps GL3 +
                        // 15665 fps DX9(API_DX11)  
                        //  3460 fps DX12           

    //#define LEVEL 19  // DEVICE_CAPABILITIES & STATUSBAR
    //#define LEVEL 20  // For all Platforms Allow mainwindow resizing. + ALT ENTER
    //#define LEVEL 21  // For all Platforms COLOR
    //#define LEVEL 22  // For all Platforms TEXTURE + IMAGE LOADER (BMP,JPG,PNG, TIFF, DDS, TGA) 
                            //#define     PS_USE_BilinearInterpolation
                            //#define     PS_USE_TrilinearInterpolation
    //#define LEVEL 23  // For all Platforms LIGHT
    //#define LEVEL 24  // For all Platforms 2D Rendering(Orthognoal Projection) 
#if defined WINDOWS_PLATFORM 
    //#define LEVEL 25  // WINDOWS DX10 FONT DRIVER.
#endif
    //#define LEVEL 26  // For all Platforms CUBEs and SPHEREs
    //#define LEVEL 27  // For all Platforms TEXT FONT
    //#define LEVEL 28  // For all Platforms SKY + SKY_CAMERA + "print screen"
    //#define LEVEL 29  // INTRO DEMO: MUSIC/SOUND: OGG(win)/WAV(android) LOADER + DX10DRIVER_FONTS(win)
    #endif

    #if defined WINDOWS_PLATFORM 
    //--------------------------------------------------------------------------------------------------------------------------
    //INTERMEDIATE:
    //#define LEVEL 30 // WORLD.XML: load "COLOR" .OBJ from (WORLD.XML) + SceneManager + QuadTree + Frustum
    //#define LEVEL 31 // WORLD.XML: load "with MULTIPLE TEXTURES" .OBJ from (WORLD.XML)
    //#define LEVEL 32 // WORLD.XML: load OBJ 3D file format, the first advanced object, using LIGHT shader
    //#define LEVEL 33 // WORLD.XML: load OBJ 3D file format, the "compound", using transparent textures ALFA MAP and ALFA COLOR
    //#define LEVEL 34 // WORLD.XML: load OBJ 3D file format, with SPECULAR and SHININESS
    //#define LEVEL 35 // WORLD.XML: load OBJ 3D file format, with BUMP MAP
    //#define LEVEL 36 // WORLD.XML: load OBJ 3D file format, render a SHADOW MAP
    //#define LEVEL 37 // WORLD.XML: load OBJ 3D file format, render chess board pieces + MAIN THREAD + LOADER THREAD
    //#define LEVEL 38 // WORLD.XML: load OBJ 3D file format, save to W3D Woma 3D fast file FORMAT
    //#define LEVEL 39 // WORLD.XML: load W3D (the Woma 3D file format) files up to 30x faster than OBJ files in LEVEl38
    //#define LEVEL 40 // WORLD.XML: load W3D and use INSTANCES to clone objects using GPU
    //#define LEVEL 41 // WORLD.XML: load W3D and use SHADOW INSTANCES (DX40 + DX39)
    //#define LEVEL 42 // WORLD.XML: load W3D and use SHADOW INSTANCES (with GPU instant ROTATION)

	//#define LEVEL 45 // IMGUI
	// 
    //--------------------------------------------------------------------------------------------------------------------------
    //BASIC-TERRAIN:
    //#define LEVEL 49 // Terrain : 512 UNDERWATER
    //#define LEVEL 50 // Terrain : 512 Union : UNDERWATER + BASIC WATER
    //#define LEVEL 51 // Terrain : 512 FOG effect on Terrain and SKY 
    //#define LEVEL 52 // Terrain : 512 with : Light shader +(op) SCENE_TERRAIN_COLLISION

    //ADVANCED-TERRAIN:
    //--------------------------------------------------------------------------------------------------------
#if NOTES
    //{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },//16 Bits (RG)
    //{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#endif
    //#define LEVEL 53 // Terrain : 512 with : Indices and Terrain Slop Color shader
    //#define LEVEL 54 // Terrain : 256 Water waves

    //--------------------------------------------------------------------------------------------------------
#if NOTES
    //{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },//Light Texture Mapping
    //{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },//Big Texture Mapping and Alpha Mapping
    //{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },//BUMP MAP
    //{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },//BUMP MAP
#endif
    //#define LEVEL 60  // 60 ~ G:\DRIVE_MY_SOURCE_CODE\Dx11Engine3D\Dx11Engine3Dx64        #if TUTORIAL_CHAP >= 24
    //--------------------------------------------------------------------------------------------------------
                        // G:\DRIVE_MY_SOURCE_CODE\WorldOfMiddleAge\WorldOfMiddleAge.sln    
    //#define LEVEL 61  // TUTORIAL_CHAP 25  TERRAIN Bigger HEIGHT MAPs 512x512

    //#define LEVEL 63  // TUTORIAL_CHAP 27  TERRAIN MAIN MAP + MINI MAP 3D Navigation
    //#define LEVEL 64  // Mill + compound
    //#define LEVEL 65  // Walk on Terrain.
     
    //#define LEVEL 70  // BILLBOARD for Trees / Flowers, (LIGHT and Fog) : 
    //#define LEVEL 71  // BILLBOARD for FENCES, (LIGHT and Fog) : .........
    //#define LEVEL 72  // FIRE ........................................... 
    //#define LEVEL 73  // WINDY GRASS ....................................                                                                                               
    //#define LEVEL 74  // CROSS BILLBOARD and CLONES
    //#define LEVEL 75  // UTIL: PROGRESS BAR FOR TERRAIN LOADING                                                             
    //#define LEVEL 76  // UTIL: INTRO VIDEO DEMO
    //#define LEVEL 77  // USE INSTANCES FOR TREES
    //#define LEVEL 78  // COLISION DETECTION: Part I

    //#define LEVEL 79  // Advanced DAE/FBX     (not published)
    //#define LEVEL 80  // GL-ASSIMP EXT-PRJ    (not published)
    //#define LEVEL 81  // DX-ASSIMP            (not published)
    
    //#define LEVEL 82  // ASSIMP-ENGINE
    //#define LEVEL 83  // Updated billboard shader
    //#define LEVEL 84  // ASSIMP: Animated / walking mesh on the terrain, following a path
    //#define LEVEL 85  // EXPAND MINI-MAP
    //#define LEVEL 86  // THE MAIN CHARACTER
    //#define LEVEL 87  // THE MAIN CHARACTER - The other sample variants
	//#define LEVEL 88  // USE_3RD_PERSON_CAMERA
    //#define LEVEL 89  // PBR Shader + The object Model Map Editor
	//#define LEVEL 90  // Replacing Billboards with advanced gaming like objects(Low Poly)
	//#define LEVEL 91  // Using threads for rendering
	//#define LEVEL 92  // Use Adavanced DX11.1
	//#define LEVEL 93  // Use Generate atlas integration (dds)
	//#define LEVEL 94  // ASSETS download with progress bar
    //#define LEVEL 96	// Use curved real sky plane
	//#define LEVEL 97	// The Waterfall
	//#define LEVEL 98	// Day and Night
	#define LEVEL 99	// Demo99
	
    #endif
#endif

#if !defined LEVEL
    static_assert(false, "At least one LEVEL need to be specified!");
#endif

#if LEVEL < 10
    #define CORE_ENGINE_LEVEL LEVEL
    #define DX_ENGINE_LEVEL 0
#else
    #define CORE_ENGINE_LEVEL 10
    #define DX_ENGINE_LEVEL LEVEL
#endif