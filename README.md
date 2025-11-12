# WoMA3Dengine

⚙️ A multi-platform, modular 3D game engine written in C++ with support for DirectX 11, DirectX 12, Android (OpenGL ES), and Linux (OpenGL).  
📚 Built for learning and showcasing 3D engine development — from fundamentals to terrain rendering, animation, and PBR.

---

## Platforms
[![Platform: Windows](https://img.shields.io/badge/platform-Windows-blue)]()
[![Platform: Linux](https://img.shields.io/badge/platform-Linux-yellow)]()
[![Platform: Android](https://img.shields.io/badge/platform-Android-green)]()
[![License: MIT](https://img.shields.io/badge/license-MIT-green)]()

<details>
<summary>📘 Table of Contents</summary>

- [✨ Features](#-features)
  - [🖥️ Platform & API Support](#️-platform--api-support)
  - [🔧 Engine Architecture](#-engine-architecture)
  - [🧱 Rendering Features](#-rendering-features)
  - [🌍 Scene & Asset Management](#-scene--asset-management)
  - [🎮 Animation & Effects](#-animation--effects)
  - [📐 Math & Utilities](#-math--utilities)
  - [🧪 Development Tools](#-development-tools)
- [🧪 Which Project to Build?](#-which-project-to-build)
  - [🪟 Windows](#-windows-visual-studio-2022)
  - [🤖 Android](#-android-visual-studio-2022)
  - [🐧 Linux](#-linux-visual-studio-2022)
- [🎮 Controls & Hotkeys](#woma3dengine-at-level-29-intro-demo)
- [📂 Tutorial Series](#http-woma-3d-engine-tutorial-list)
  - [🔺 Advanced Mesh Series](#3d---advanced-mesh-tutorial-series-begin)
  - [🌿 Billboard Series](#3d---billboard-tutorial-series-begin)
  - [🌄 Terrain Series](#3d---terrain-tutorial-series-start)
  - [📘 Intermediate Tutorials](#intermediate-3d-engine-tutorial-list)
  - [📗 Basic Tutorials](#basic-3d-engine-tutorial-list)
  - [⚙️ Core System Tutorials](#3d---core-tutorial-series-begin)

</details>

## ✨ Features

The **WoMA3Dengine** is a modular, cross-platform 3D graphics engine that showcases modern rendering techniques and engine design through a step-by-step tutorial approach.

### 🖥️ Platform & API Support
- **Cross-platform**: Windows, Linux, Android
- **Graphics APIs**: DirectX 9, 11, 12, OpenGL (3.3+), Vulkan (planned)
- **Shader Languages**: HLSL & GLSL
- **Build System**: Visual Studio, GCC, Android NDK

### 🔧 Engine Architecture
- Modular and extensible engine layers using `DX_ENGINE_LEVEL` and `CORE_ENGINE_LEVEL`
- Separation of rendering pipeline, asset management, and scene logic
- Runtime and editor/debug builds
- Memory leak detection and structured exception handling

### 🧱 Rendering Features
- Physically Based Rendering (PBR) using glTF material specs
- Multiple lighting models: Phong, Blinn-Phong, and physically-based
- Dynamic day/night lighting system with real-time sun/moon direction
- Real-time shadows: PCF and shadow maps
- Frustum culling and bounding-box optimization
- Instancing & billboarding
- Screen-space effects: fog, depth-based shading

### 🌍 Scene & Asset Management
- Mesh loading via **ASSIMP** (.obj, .fbx, .dae, .3ds, .blend)
- Texture pipeline with automatic DDS conversion (_DEBUG)
- Cyphered asset loading from packed W3D formats
- LOD generation and selection
- Sky dome, water, terrain heightmap loading

### 🎮 Animation & Effects
- FBX animation support with skeleton/bone playback
- Multiple animation merging and metadata extraction
- Billboarded particles and vegetation
- Celestial simulation: real sun/moon positioning, METAR-based weather

### 📐 Math & Utilities
- Custom camera system (1st-person, 3rd-person, orbit)
- Ray casting / object picking
- AABB and bounding sphere generation
- Full linear algebra library (vectors, matrices, quaternions)

### 🧪 Development Tools
- Modular tutorial progression (89+ stages)
- Logging system to console, VS output, and log file
- Debug render modes (wireframe, UV checker, LOD, normals)
- Editor toolkits planned (map/scene builder)

<hr>
<h1>WoMA3Dengine at Level 29 INTRO DEMO</h1>
<table>
<tr>
	<td>
		<h2>Movement Keys: (at level28)</h2>
	</td>
	<td>
		<h2>Keys / Hints:</h2>
	</td>
</tr>
<tr>
<td>
  - <b>Left:		</b> arrow or a<br>
  - <b>Right:	 	</b>	arrow or d<br>
  - <b>Front:		</b> arrow or w<br>
  - <b>Back:		</b> arrow or s<br>
  - <b>Look Up:		</b> pg. Up<br>
  - <b>Look Down:	</b>	pg. Down<br>
  - <b>Go Up: 		</b>	R (GOD MODE: true)<br>
  - <b>Go Down: 	</b>	F (GOD MODE: true)<br>
</td>
<td>
<b>[F1]</b> Full system info (CPU, GPU, OS, memory, etc.)<br>
<b>[F2]</b> Celestial info (Sun, Moon, sunrise/sunset, azimuth)<br>
Latitude/Longitude<br>
Calculate Sun Rise / Sun Set<br>
Moon Distance<br>
Sun Geometric elevation<br>
Moon Geometric elevation<br>
Moon Azimuth<br>
<b>[F3]</b> World map with real location<br>
<b>[F4]</b> Weather data (METAR, clouds, temperature, humidity) (when network option is: ON)<br>
<b>[F5]</b> <b>Game Rendering</b>, Default engine run mode (gameplay / scene view)<br>
<b>[F6]</b> Engine setup / configuration screen<br>
Try to Change the driver on the fly without the need of close and open the Application.<br>
- DirectX 12<br>
- DirectX 11<br>
- DirectX 9<br>
- OPENGL 4<br>

<b>[F11]</b> Switch God Mode (on debug only: flying fast over the terrain)<br>
</td>
</tr>
</table>

<h1>HTTP WOMA 3D ENGINE TUTORIAL LIST</h1>

<hr>
<h2>3D - ADVANCED TUTORIAL SERIES</h2>

<!-- ------------------------------------------------------------------------------------------------ -->

<h2>3D - ADVANCED MESH TUTORIAL SERIES: BEGIN</h2>

<hr>

- <b>LEVEL-98: Day and Night</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_098.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_098.png)	


![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_098B.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_098B.png)	


<hr>

- <b>LEVEL-97: The Waterfall</b><br>
	- The water Waterfall
	- The smoke of Waterfall
	- The sound of Waterfall
	- The Dungeon Gate
	- The Sound of ocean waves

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_097.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_097.png)	

<hr>

- <b>LEVEL-96: Use curved real sky plane</b><br>
Level/Sample about how to add an external code from Rastertek.<br>
https://rastertek.com/tertut12.html<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_096.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_096.png)	

<hr>

- <b>LEVEL-94: Optimizing Dynamic memory allocation for Billboard vertices - Atlas Texture - Part 2</b><br>
Improving  from **900** FPS to **930** FPS (soon)<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_094.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_094.png)	

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_094B.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_094B.png)	


<hr>

- <b>LEVEL-93: Optimizing the Billboard objects render all in 1 obj, using - Atlas Texture - Part 1</b><br>
Improving  from **850** FPS to **900** FPS.<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_093.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_093.png)	

Billboard Atlas (Automatic generated:)<br>
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/93billboard_atlas_preview.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/93billboard_atlas_preview.png)	

<hr>

- <b>LEVEL-92: Use from now on the **modern**: **DX11.1**</b><br>
There is a Huge list of improvements<br>
Improving with DX11.1 from **815** FPS to **850** FPS.<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_092.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_092.png)	

<hr>
- <b>LEVEL-91: Using threads for rendering</b><br>
Improving from **760** FPS to **815** FPS.<br>
  
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_091.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_091.png)	

<hr>

- <b>LEVEL-90: Replacing Billboards with advanced gaming like objects(Low Poly)</b><br>
  
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_090.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_090.png)	

<hr>

- <b>LEVEL-89: The object Model Map Editor</b><br>
**Editor Keys/Controls:**<br>
	- **Mouse**, select the object<br>
	- **Arrow Keys**, move left and right, front and back. (NOTE: use shift to do a minor movements)<br>
	- **PgUp and PgDown**, move up and down. (NOTE: use shift to do a minor movements)<br>
	- **Numpad-4 and Numpad-6**, rotate the object.<br>
	- **"+" and "-"**, Scale it.<br>
Then update the "world.xml" with the new positions shown on screen.<br>
NOTE: Use GodMode toggle: **F11** to have a better view.<br>
  
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_089.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_089.png)	

<hr>
- <b>LEVEL-88: USE_3RD_PERSON_CAMERA</b><br>
    3rd person camera: (W A S D) + left (CTRL or SHIFT) to run + STRAFE (Q E):<br>
    Key 1: Draw sword<br>
  
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_088.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_088.png)	

<hr>

- <b>LEVEL-87: THE MAIN CHARACTER - The other sample variants</b><br>
    Using animation:<br>
    - Idle, Idle2, Idle3 for each variant.
 
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_087.jpg](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_087.jpg)	

<hr>

- <b>LEVEL-86: The main character (With idle animation)</b><br>
Later: (and much more)
 Multi animation Character: 33
    - Attack(3)
    - Idle(3)
    - Walk(3)
    - Walk back (3)
    - Walk left (3)
    - Walk right (3)
    - Run (3)
    - Death (3)
    - Hide weapon (2)
    - Draw weapon (2)
    - Get hit (2)
    - Jump (3)
   <br>
   (Art/Animation Credits: <b>Palasky</b>)
  
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_086.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_086.png)	

<hr>

- <b>LEVEL-85: Expand MiniMap</b><br>
 
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_085.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_085.png)	

<hr>
- <b>LEVEL-84: ASSIMP: Animated / walking mesh on the terrain, following a path:</b><br>
 
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_084.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_084.png)	

<hr>

- <b>LEVEL-83: ASSIMP: Update billboard shader:</b><br>
 
<b>RELEASE version (1920x1080 Fullscreen) - about 1070 FPS (using AVX2) @target WIN10</b><br>
<b>RELEASE version (1920x1080 Fullscreen) - about 1321 FPS (using AVX512) @target WIN11</b>
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_083-RELEASE.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_083-RELEASE.png)	

<hr>

- <b>LEVEL-82: ASSIMP: Loading an animated mesh:</b><br>
 
<b>DEBUG version:</b>
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_082.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_082.png)	

<b>RELEASE version - about 1070 FPS (using AVX512):</b><br>
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_082-RELEASE.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_082-RELEASE.png)	

<hr>
<h2>3D - ADVANCED MESH TUTORIAL SERIES: END</h2>
<hr>

<!-- ------------------------------------------------------------------------------------------------ -->

<hr>
<h2>3D - INTERMEDIATE TUTORIAL SERIES</h2>
<hr>


<hr>
<h2>3D - BILLBOARD TUTORIAL SERIES: BEGIN</h2>
<hr>

- <b>LEVEL-78: Colision detection with objects</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_078.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_078.png)

<hr> 

- <b>LEVEL-77: Using instances for TREEs</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_077.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_077.png)

<hr> 

- <b>LEVEL-76: UTIL: INTRO VIDEO DEMO (Play: MP4)</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_076.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_076.png)

<hr> 

- <b>LEVEL-75: UTIL: PROGRESS BAR FOR TERRAIN LOADING</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_075.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_075.png)
 
<hr> 

- <b>LEVEL-74: CROSS BILLBOARDs and CLONEs</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_074.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_074.png)
  
<hr> 

- <b>LEVEL-73: GRASS - TYPE I</b><br>
 
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_073.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_073.png)	

<hr> 

- <b>LEVEL-72: FIRE (Add 3D Sound / Effects.)</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_072.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_072.png)	
  
<hr> 

- <b>LEVEL-71: BILLBOARD for FENCES</b><br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_071.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_071.png)	 

<hr> 

- <b>LEVEL-70: BILLBOARD for Trees / Flowers</b><br> 

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_070.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_070.png)	 

<hr> 

<hr>
<h2>3D - BILLBOARD TUTORIAL SERIES: END</h2>
<hr>

<!-- ------------------------------------------------------------------------------------------------ -->

<hr>
<h2>3D - TERRAIN TUTORIAL SERIES: START</h2>
<hr>

- <b>LEVEL-65: WORLD.XML: TERRAIN: Walking on Terrain.</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_065.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_065.png)	 

<hr> 
- <b>LEVEL-64: WORLD.XML: TERRAIN: Add a model with 2 pass and add a 3D static animated model</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_064.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_064.png)	 

<hr> 
- <b>LEVEL-63: WORLD.XML: TERRAIN: MAIN-MAP and MINI-MAP</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_063.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_063.png)	 

<hr> 
- <b>LEVEL-61: WORLD.XML: TERRAIN: 256x256 MULTI-LAYER DEMO</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_061.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_061.png)	 

<hr> 
- <b>LEVEL-60: WORLD.XML: TERRAIN: 256x256 MULTI-LAYER DEMO</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_060.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_060.png)	 

<hr> 
- <b>LEVEL-54: WORLD.XML: TERRAIN: Add <b>Water waves</b> ModelTextureVertexType</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_054.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_054.png)	 

<hr> 
- <b>LEVEL-53: WORLD.XML: TERRAIN: Add extra color terrain(extra Green in this example) ModelTextureVertexType</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_053.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_053.png)	 

<hr> 
- <b>LEVEL-52: WORLD.XML: TERRAIN: Use Light Shader + Add Normals + Add Index(s) ModelTextureVertexType.</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_052.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_052.png)	 

<hr> 
- <b>LEVEL-51: WORLD.XML: Add <b>fog</b> (ModelTextureVertexType). </b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_051.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_051.png)	 

<hr> 
- <b>LEVEL-50: WORLD.XML: The water and terrain above. </b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_050.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_050.png)	 

<hr> 
- <b>LEVEL-49: WORLD.XML: Generate under water terrain. </b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_049.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_049.png)	 

<hr>
<h2>3D - TERRAIN TUTORIAL SERIES: START</h2>
<hr>

<!-- ------------------------------------------------------------------------------------------------ -->

<hr>
<h2>INTERMEDIATE 3D ENGINE TUTORIAL LIST</h2>
<hr>
 
<hr>
<h2>BASIC 3D ENGINE TUTORIAL LIST</h2>
<hr>

- <b>LEVEL-42: WORLD.XML: load W3D and use SHADOW INSTANCES but now with GPU ROTATION on (BLACK_BISHOPs and BLACK_KNIGHTs)</b><br> 
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_042.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_042.png)	 

<hr>

- <b>LEVEL-41: WORLD.XML: load W3D and use SHADOW INSTANCES</b><br> 
	- This is a join of DX40 with DX39 now all together.
	- 041ShadowMapInstance.hlsl is equivalent to 036ShadowMap.hlsl but with instances.
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_041.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_041.png)	

<hr>

- <b>LEVEL-40: WORLD.XML: load W3D and use INSTANCES to clone objects using GPU</b><br> 
	- This is a very powerful tool, with less we can do more, using Shader INSTANCES to clone objects into different positions
	- 040LightInstance.hlsl is equivalent to 023Light.hlsl but with instances
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_040.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_040.png)	

<hr>

- <b>LEVEL-39: WORLD.XML: load W3D (the Woma 3D file format) files up to 30x faster than OBJ files in DX LEVEL 38</b><br> 
	- Loading the generated files (in DX38) ".W3D" Woma3DFormat. in 0.5 seconds vs 13.5 seconds using the ".OBJ" 3D file format
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_039.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_039.png)	

<hr>

- <b>LEVEL-38: WORLD.XML: load OBJ 3D file format, save to W3D Woma 3D fast file FORMAT</b><br> 
	- Load and Convert the powerfull/simple but slow ".OBJ" format 3D file, into to a super fast native WOMA file format: ".W3D" 20x faster!
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_038.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_038.png)	

<hr>

- <b>LEVEL-37: WORLD.XML: load OBJ 3D file format, render chess board pieces</b><br> 
	- Adding MAIN THREAD and the LOADER THREAD
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_037.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_037.png)	


<hr>

- <b>LEVEL-36: WORLD.XML: load OBJ 3D file format, render a SHADOW MAP</b><br> 
	- Loading an OBJ with SHADOW MAP, using a tecnique called: render to texture
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_036.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_036.png)	


<hr>

- <b>LEVEL-35: WORLD.XML: load OBJ 3D file format, with BUMP MAP</b><br> 
	- Loading an OBJ with BUMP MAP texture
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_035.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_035.png)	

<hr>
- <b>LEVEL-34: WORLD.XML: load OBJ 3D file format, with SPECULAR and SHININESS</b><br> 
	- Adding SPECULAR and SHININESS to OBJ loader and shaders
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_034.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_034.png)	

<hr>
- <b>LEVEL-33: WORLD.XML: load OBJ 3D file format, the compond, using transparent textures ALFA MAP and ALFA COLOR</b><br> 
	- Loading now OBJ with alfa color and alfamap
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_033.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_033.png)	


<hr>
	
- <b>LEVEL-32: WORLD.XML: load OBJ 3D file format, the first advanced object, using LIGHT shader</b><br> 
	- Loading more complex objects with light, the: OLD HOUSE.
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_032.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_032.png)	

<hr>
<h3>LEVEL-31: WORLD.XML: load OBJ 3D file format, with MULTIPLE TEXTURES, using TEXTURE Shader:</h3>
	- Loading Multiple Textures in one object
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_031.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_031.png)	


<hr>
<h3>LEVEL-30: [Scene Graph][Node Tree]  WORLD.XML: load OBJ 3D file format, using COLOR Shader:</h3>
	- SceneManager and SceneNode<br> 
	- QuadTree and Frustum<br> 
	- <b>World</b> and <b>Objects</b>: Loading objects as a descriptor:	<b>World.xml</b><br>
	- Load <b>obj</b> format<br>
	<li>Load <b>obj</b> format<br></li><br>
 
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_030.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_030.png)	

<hr>
<h2>INTERMEDIATE 3D ENGINE TUTORIAL LIST: END</h2>
<hr>

<!-- ------------------------------------------------------------------------------------------------ -->

<h3>LEVEL-29: INTRO DEMO OF BASIC TUTORIAL SERIES:</h3>
<li>Adding the Sound Manager<br></li>
<li>Adding Play background Music (Windows, Linux and Android)<br></li>
<li>Generate the first RELEASE BUILD<br></li>
<li>Add the installer of INTRO DEMO OF BASIC TUTORIAL SERIES<br></li>
<li>Adding the first release demo at LEVEL 29m for Windows, Linux and Android.<br></li>

<p align=center>
Intel/AMD X64 VIDEO:
<table align=center><tr><td>
	
[![WoMA3Dengine at Level 29 BASIC INTRO DEMO X64](https://i.imgur.com/q3AMNrr.png)](https://www.youtube.com/watch?v=tdMsRX_lQ5A)
</td></tr></table>
</p>

<p align=center>
Android ARM64/x86 VIDEO:<br>
<table align=center><tr><td>
	
[![WoMA3Dengine at Level 29 BASIC INTRO DEMO Android ARM64/x86](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/refs/heads/main/Image_Tutorial_Snapshot/ANDROIDEngine_029.png)](https://www.youtube.com/watch?v=O1IvEg84ayM)
</td></tr></table>
</p>

<hr>

<h3>LEVEL-28: [Raw Mouse/Keyboard Input] Adding the fixed camera Sky and adding take snapshot:</h3>
	- Adding the fixed camera Sky<br>
	- Adding Direct input for DX Drivers<br>
	- Allow snapshot file dump using "print screen" key, in any of these image formats:<br>
  
			- Bmp
			- Png
			- Ico
			- Jpeg
			- Tiff
			- Gif
			- Wmp
			- Dds
	
<table>
<tr>
<td>WINDOWS:</td>
<td>LINUX:</td>
<td>ANDROID:</td>
</tr>
<td>

<tr>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_028.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_028.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_028.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_028.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_028.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_028.png)	
</td>

</tr></table>

<hr>

<h3>LEVEL-27: The Rastertek text font:</h3>
<li><b>based on my mentor :)</b></li>
<li>https://rastertek.com/dx11win10tut14.html</li>
<table>
<tr>
<td>WINDOWS:</td>
<td>LINUX:</td>
<td>ANDROID:</td>
</tr>
<td>

<tr>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_027.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_027.png)	

</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_027.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_027.png)	

</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_027.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_027.png)	

</td>

</tr></table>


<hr>
<h3>LEVEL-26: The Spheres (COLOR, TEXTURE and LIGHT Shaders):</h3>
<li><b>One for each Shader type: (COLOR, TEXTURE and LIGHT Shaders)</b></li>

<table>
<tr>
<td>WINDOWS:</td>
<td>LINUX:</td>
<td>ANDROID:</td>
</tr>
<td>

<tr>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_026.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_026.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_026.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_026.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_026.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_026.png)	
</td>

</tr></table>

<hr>
<h3>LEVEL-25: The CUBEs (the first auto-generated 3D object):</h3>
One for each Shader type: (COLOR, TEXTURE and LIGHT Shaders)<br>
	
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_025.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_025.png)	

<hr>
<h3>LEVEL-24: The 2D Rendering [Dynamic Vertex] (for Sprites):</h3>
	
<table>
<tr>
<td>WINDOWS:</td>
<td>LINUX:</td>
<td>ANDROID:</td>
</tr>
<td>

<tr>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_024.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_024.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_024.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_024.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_024.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_024.png)	
</td>

</tr></table>

<hr>
<h3>LEVEL-23: The [Directional Light] Shader with environment color:</h3>
<b>Adding the first light, the Directional light.</b><br>
<table>
<tr>
<td>WINDOWS:</td>
<td>LINUX:</td>
<td>ANDROID:</td>
</tr>
<td>

<tr>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_023.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_023.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_023.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_023.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_023.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_023.png)	
</td>

</tr></table>

<hr>
<h3>LEVEL-22: [The Textures] Shader:</h3>
Add third party image readers:<br>
	- BMP (WINDOWS, LINUX and ANDROID)<br>
	- JPG (WINDOWS, LINUX and ANDROID)<br>
	- PNG (WINDOWS, LINUX and ANDROID)<br>
	- TIFF (WINDOWS, LINUX and ANDROID)<br>
	- DDS (WINDOWS)<br>
	- TGA (WINDOWS)<br>
 
<table>
<tr>
<td>WINDOWS:</td>
<td>LINUX:</td>
<td>ANDROID:</td>
</tr>
<td>

<tr>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_022.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_022.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_022.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_022.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_022.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_022.png)	
</td>

</tr></table>

<hr>
<h3>LEVEL-21: The COLOR Shader [First Triangle and Quad]:</h3>
 Introduction to the: Rasterizer State
<table>
<tr>
<td>WINDOWS:</td>
<td>LINUX:</td>
<td>ANDROID:</td>
</tr>
<td>

<tr>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_021.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_021.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_021.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/LINUXEngine_021.png)	
</td>
<td>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_021.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/ANDROIDEngine_021.png)	
</td>

</tr></table>

<hr>
<h3>LEVEL-20: Adding the Resize to Main Window:</h3>
 Allow alt+enter swap<br>
 And allow user to change the window size

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_020.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_020.png)	

<hr>
<h3>LEVEL-19: The Main 3D simple window:</h3>
<b>Add Render Engine for "Windows":<br></b>
<li>DX9sdk (the original sdk, will be added/revealed later, at least on basic series)<br></li>
<li>DX9(using 11)<br></li>
<li>DX11<br></li>
<li>OPENGL3/4<br></li>
<li>DX12<br></li>
<b>Add Render Engine for "Linux":<br></b>
<li>OPENGL3/4<br></li>
<b>Add Render Engine for "Android":<br></b>
<li>GLES2/3<br></li>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_019.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_019.png)	

<hr>
<h3>Level 19: [Create Device / Swap Chain] System Info image (F1):</h3>
<li>Improve System Info, now with max CPU clock speed detection.</li>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_019-F1.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/DXEngine_019-F1.png)	

<hr>
<h2>BASIC 3D ENGINE TUTORIAL LIST: END</h2>
<hr>

<hr>
<h2>3D - CORE TUTORIAL SERIES: BEGIN</h2>
<hr>

<h3>009 Get current RealTime Weather using the aviation METAR:</h3>
<b>METAR and TAF(later)</b><br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_009_PAINT_METAR_WEATHER_REALTIME.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_009_PAINT_METAR_WEATHER_REALTIME.png)	

<hr>
<h3>008 RealTimeMapGPSPosition</h3>
<li>maxminddb_LIB<br></li>
<li>GeoLite2PP_LIB<br></li>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_008_PAINT_REALTIME_GPS_MAP_LOCATION.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_008_PAINT_REALTIME_GPS_MAP_LOCATION.png)	

<hr>	 
<h3>007 Add the Real Time Celestial calculations:</h3>
<b>These calculations will be used later:</b><br>
	- Loading files from engine.pck on RELEASE BUILD (explained later in an advanced chapeter with the installer)<br>
	- Calculate SunRise, SunSet<br>
	- Calculate Sun altitude angle and azimuth angle<br>
	- Moon Phase (based of realime date)<br>
	- Calculate Moon Distance<br>
	- Calculate moon Elevation, taking into account atmospheric refraction<br>
	- calculate Moon Ecliptic Latitude, Ecliptic Longitude and Azimuth<br>
<br>
(NOTE: Later will be used to re-create more realistic astro positions)<br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_007_PAINT_REALTIMECELESTIAL_ASTROCLASS.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_007_PAINT_REALTIMECELESTIAL_ASTROCLASS.png)	

<hr>	
<h3>006 Pack manager, the IDEA, the first simple MATH lib and CPU BENCH:</h3>
        <b>This will be used on bin release LEVEL 29:</b><br>
	- Create a pack file called <b>engine.pack</b>, which is a zip with the contents of all engine directory.<br>
	- Cypher the file with IDEA and call it: <b>engine.pck</b><br>
	- Add the first Math libs for DX and OPENGL<br>
	- Add a simple CPU benchmark, to measure the performance improvement<br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_006_PACKMANAGER_IDEA_CPU_BENCHMARK.png)	

<hr>	
<h3>005 [COM(Component Object Model)] The System Settings (settings.xml):</h3>
	- Adding the OS Paint events<br>
	- Allow Load/Save Game System Settings in the <b>settings.xml</b> file.<br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_005_PAINT_SETTINGS_XML.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_005_PAINT_SETTINGS_XML.png)	

<hr>
<h3>004 SystemManager, TIMER and FPS:</h3>
	- Adding the first version of SystemManager viewer<br>
	- Adding Precision Timer<br>
	- Add FPS (Frame per second) calculation<br>
	- OS [Keyboard] Manager<br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_004_SYSTEMMANAGER_TIMER_FPS_MATH.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_004_SYSTEMMANAGER_TIMER_FPS_MATH.png)

<hr>
<h3>003 [Error Handling] Central EXCEPTIONs trace + Mini-Dumper and the upload (just the concept):</h3>
	- Exception Stack trace viewer in detail for an _DEBUG build<br>
	- Exception Stack trace viewer and create  the respective Mini Dump report<br>
	- Option to Upload the report for an FTP site.<br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_003_MINIDUMPER_STACK_EXCEPTION.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_003_MINIDUMPER_STACK_EXCEPTION.png)

<hr>
<h3>002 The Main Window [Custom Icons]:</h3>
	- Create Main OS Window [Window Creation]<br>
	- Handle the OS Windows events [Message Loop / WndProc]<br>
	- Main Window Paint events [Window Messages]<br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_002_MAINWINDOW_EVENTS.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_002_MAINWINDOW_EVENTS.png)	


<hr>
<h3>001 User language detection (later automatic translation), MEM Leaks and LOG:</h3>
	- User language detection (later automatic translation)<br>
	- Log manager, will log on: <b>REPORT.txt</b>, on Visual Studio Console and on Console Window<br>
	- Memory leak detector level 1 <br>
	- OS manager for main OS directories (DEBUG/RELEASE)<br>
<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_001_LANG_LOG_MEMLEAK_OSMAINDIRs.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_001_LANG_LOG_MEMLEAK_OSMAINDIRs.png)	

<hr>
<h3>000 OSengine - The basic pillar foundations of the 3D WoMA Engine:</h3>
	- platform.h [Introduction] (Auto detect: all platforms: WINDOWS, LINUX and ANDROID)<br>
	- main.cpp [WinMain] (for all platforms: WINDOWS, LINUX and ANDROID)<br>
	- Debug Console<br>
	- The default settings<br>
	- SystemClass (common for all platforms: WINDOWS, LINUX and ANDROID)<br>
	- WinSystemClass <b>(for Windows)</b><br>
	- LinuxSystemClass <b>(for Linux)</b><br>
	- AndroidSystemClass <b>(for Android)</b><br>
	<br>

![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_000_OSENGINE_SYSTEM.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/Image_Tutorial_Snapshot/CoreEngine_000_OSENGINE_SYSTEM.png)	

<hr>
<h2>3D - CORE TUTORIAL SERIES: END</h2>
<hr>


## 🚀 Latest Installer Release

**[📦 WoMA3Dengine v0.901 Installer](https://github.com/pmborg/WoMA3Dengine/releases/tag/v0.901)** — July 2025  
No need to compile! This zip includes everything pre-packaged:

- 🧰 Downloads the **latest source code**
- ⚙️ Installs **precompiled demo binaries fore DEBUG/RELEASE** (from level 000 to 090)
- 🌐 Launch the engine instantly via HTML frontend

```plaintext
C:\WoMA3Dengine\CoreEngine_Tutorials\index.html
```

**Notes:**<br>

**Use:**<br>
Microsoft Visual Studio Community **2022 (64-bit)** - Version **17.14.8**<br>
<br>
For now please just clone all content at (From this path all Platforms will compile using "Visual Studio", **Windows**, **Linux** and **Android**):<br>
C:\> **git clone --recursive https://github.com/pmborg/WoMA3Dengine.git**<br>
Clone to: **C:\WoMA3Dengine**<br>
<br>
Later on I will make path free but for now a few paths are hardcoded there.<br>
<br>
<hr>

# cmake version need to be: 
<b>cmake-3.31.4</b>

## How did I install all environment from scratch:
[INSTALL_ALL_FROM_SCRATCH.md](https://github.com/pmborg/WoMA3Dengine/blob/main/HowTo/INSTALL_ALL_FROM_SCRATCH/INSTALL_ALL_FROM_SCRATCH.md)

<hr>
## 🧪 Which Project to Build?

WoMA3Dengine is divided into tutorial levels across three platforms:

### 🪟 Windows (Visual Studio 2022)
Choose the solution depending on your level of interest or tutorial stage:

| Solution | Tutorials | Description |
|----------|-----------|-------------|
| `WindowsEngine_000-009-Core.sln`         | 000–009 | Engine startup, math, debug, sun/moon |
| `WindowsEngine_019-029-Basic.sln`        | 019–029 | Texture mapping, camera, models, intro to terrain |
| `WindowsEngine_030-045-Intermediate.sln` | 030–045 | Billboards, instancing, lighting, input |
| `WindowsEngine_049-089-Terrain.sln`      | 049–089 | Full terrain system, weather, PBR, animation |

🔹 To explore the latest and most complete version, open and build:  
`WindowsEngine_049-089-Terrain.sln` → `CoreEngine_089`

<hr>

### 🤖 Android (Visual Studio 2022)
To build the Android version using NDK:<br>
`ANDROID1_engine_ALL.sln`

**Android-SDK:**<br>
C:\Program Files (x86)\Android\AndroidSDK\25<br>
**Android-NDK:**<br>
C:\Program Files (x86)\Android\AndroidNDK\android-ndk-r15c<br>
**Java:**<br>
C:\Java\jdk1.8.0_202<br>
**Ant:**<br>
C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Apps\apache-ant-1.9.3<br>

<hr>

### 🐧 Linux (Visual Studio 2022)
`LINUX_engine_ALL.sln`<br>
**Check this file for more information about how to setup WSL:**<br>
https://github.com/pmborg/WoMA3Dengine/blob/main/HowTo/HowToWSL2/What%20to%20install%20on%20WSL%20LINUX.txt



<hr>

---

## 🧩 Third-Party Components and License Information
*(This section summarizes all external dependencies used by the engine)*

The **WoMA3Dengine** integrates several external open-source and third-party libraries.  
Each component retains its original license, and all attributions are included for transparency and compliance.

📘 **Documentation:**
- [ThirdParty_Licenses.md](https://github.com/pmborg/WoMA3Dengine/blob/main/ThirdParty/ThirdParty_Licenses.md) —  
  Full list of external libraries, licenses, and copyright notices.
- [Dependency_Map.md](https://github.com/pmborg/WoMA3Dengine/blob/main/ThirdParty/Dependency_Map.md) —  
  Structured overview of where each dependency is used within the engine.

### 🔍 Summary by License Type

| License Type | Examples | Notes |
|---------------|-----------|-------|
| 🟢 **MIT / BSD** | TinyXMLv2, DirectXTex, DirectXTK, Assimp, PPG-master, libcurl | Open & permissive licenses |
| 🟣 **LGPL / BSD** | OpenAL, ALUT, Vorbis, VorbisFile | Audio codecs and sound engines |
| 🟠 **Apache 2.0 / CC BY-SA** | GeoLite2PP, MaxMindDB | IP geolocation libraries |
| 🔴 **Commercial** | UltimateTCP/IP SDK | Requires separate license |
| 🟡 **Educational / Tutorial** | Rastertek text fonts | Based on Rastertek DirectX tutorials |

🧾 These dependencies enable features such as:
- Asset importing (Assimp, PPG)
- Advanced PBR rendering (PPG + DirectXTK)
- 3D sound (OpenAL + Vorbis)
- XML parsing (TinyXMLv2)
- Network data (libcurl, UltimateTCP/IP)
- Geolocation services (GeoLite2PP, MaxMindDB)

---

<p align="center"><sub>End of README — see <a href="https://github.com/pmborg/WoMA3Dengine/tree/main/ThirdParty">/ThirdParty</a> for full details.</sub></p>


## 📘 Developer Documentation Index

The WoMA3Dengine project includes in-depth technical documentation for developers and contributors.

| 📄 Document | Description |
|-------------|-------------|
| [**ThirdParty_Licenses.md**](https://github.com/pmborg/WoMA3Dengine/blob/main/ThirdParty/ThirdParty_Licenses.md) | Complete overview of all third-party libraries, their licenses, and legal attributions. |
| [**Dependency_Map.md**](https://github.com/pmborg/WoMA3Dengine/blob/main/ThirdParty/Dependency_Map.md) | Visual map of engine dependencies by module, including internal utilities and cross-platform relations. |
| [**INSTALL_ALL_FROM_SCRATCH.md**](https://github.com/pmborg/WoMA3Dengine/blob/main/HowTo/INSTALL_ALL_FROM_SCRATCH/INSTALL_ALL_FROM_SCRATCH.md) | Step-by-step installation and build environment setup guide. |

📁 *All files are located under* [`/ThirdParty`](https://github.com/pmborg/WoMA3Dengine/tree/main/ThirdParty) *and* [`/HowTo`](https://github.com/pmborg/WoMA3Dengine/tree/main/HowTo).  

---

<p align="center">
<sub>© 2012–2025 Pedro Borges — WoMA3Dengine. All rights reserved.  
Open-source components remain property of their respective authors under their stated licenses.</sub>
</p>