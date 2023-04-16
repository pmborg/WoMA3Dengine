# WoMA3Dengine
<hr>
<h1>WoMA3Dengine at Level 29 INTRO DEMO</h1>

[![WoMA3Dengine at Level 29 INTRO DEMO](https://i.imgur.com/q3AMNrr.png)](https://www.youtube.com/watch?v=tdMsRX_lQ5A)

<h2>Movement Keys: (at level28)</h2> <br>
  - <b>Left:		</b> arrow or a<br>
  - <b>Right:	 	</b>	arrow or d<br>
  - <b>Front:		</b> arrow or w<br>
  - <b>Back:		</b> arrow or s<br>
  - <b>Look Up:		</b> pg. Up<br>
  - <b>Look Down:	</b>	pg. Down<br>
  - <b>Go Up: 		</b>	R<br>
  - <b>Go Down: 	</b>	F<br>
  
<h2>Keys / Hints:</h2> <br>
<b>[F1]</b> System Info<br>
<b>[F2]</b> Celestial Info<br>
Latitude/Longitude<br>
Calculate Sun Rise / Sun Set<br>
Moon Distance<br>
Sun Geometric elevation<br>
Moon Geometric elevation<br>
Moon Azimuth<br>
<b>[F3]</b> Map with current location<br>
<b>[F4]</b> Get real time weather info (when network option is: ON)<br>
<b>[F5]</b> Game Rendering page (defaultt page)<br>
<b>[F6]</b> SETUP<br>
Try to Change the driver on the fly without the need of close and open the Application.<br>
- DirectX 12<br>
- DirectX 11<br>
- DirectX 9<br>
- OPENGL 4<br>

<h2>CORE ENGINE TUTORIALS LIST</h2>

<hr>

- <b>000 OSengine - The basic pillar foundations of the 3D WoMA Engine</b><br>
 Debug Console<br>
 default settings<br>
 SystemClass (common for all platforms)<br>
 <b>WinSystemClass (for windows)</b><br>
 XboxSystemClass<br>
 LinuxSystemClass<br>
 AndroidSystemClass<br>
 ...<br>

<hr>

- <b>001 The User International Language</b><br>
	- User language detection (later automatic translation)<br>
	- Log manager, will log on: <b>REPORT.txt</b>, on Visual Studio Console and on Console Window<br>
	- Memory leak detector level 1 <br>
	- OS manager for main OS directories (DEBUG/RELEASE)<br>

<hr>

- <b>002 MAIN_Window</b><br>
	- Create MAIN OS Window<br>
	- Handle the OS Windows events<br>
	- Main Window Paint events<br>
	- OS INPUT manager<br>

<hr>

- <b>003 EXCEPTION_trace + DUMP_upload</b><br>
	- Exception Stack trace viewer in detail for an _DEBUG build<br>
	- Exception Stack trace viewer and create  the respective Mini Dump report<br>
	- Option to Upload the report for an FTP site.<br>

<hr>

- <b>004 SystemManager + TIMER + FPS</b><br>
	- Adding the first version of SystemManager viewer<br>
	- Adding Precision Timer<br>
	- Add FPS (Frame per second) calculation<br>

<hr>

- <b>005 The System settings (settings.xml)</b><br>
	- Adding the OS Paint events<br>
	- Allow Load/Save Game System Settigns in the <b>settings.xml</b> file.<br>

<hr>

- <b>006 PACKmanager + IDEA + MATH + CPU BENCH</b><br>
	- Create a pack file called <b>engine.pack</b>, which is a zip with the contents of all engine directory.<br>
	- Cypher the file with IDEA and call it: <b>engine.pck</b><br>
	- Add the first Math libs for DX and OPENGL<br>
	- Add a simple CPU benchmark, to measure the performance improvement<br>

<hr>

- <b>007 Add the Real Time Celestial calculations.</b><br>
	- Loading files from engine.pck on RELEASE BUILD (explained later in an advanced chapeter with the installer)<br>
	- Calculate SunRise, SunSet<br>
	- Calculate Sun altitude angle and azimuth angle<br>
	- Moon Phase (based of realime date)<br>
	- Calculate Moon Distance<br>
	- Calculate moon Elevation, taking into account atmospheric refraction<br>
	- Calculate Moon Ecliptic Latitude, Ecliptic Longitude and Azimuth<br>

(later will be used to re-create more real astro positions)

<hr>

- <b>008 RealTimeMapGPSPosition</b><br>

<hr>

- <b>009 Get current RealTime Weather using the aviation METAR</b><br>

<hr>
<h2>3D - BASIC TUTORIAL SERIES</h2>

<hr>

- <b>DX19: The Main window</b><br>
	- Add Render Engine for Windows only, with these drivers:<br>
	- DX9sdk (the original sdk, will be added/revealed later, at least on basic series)<br>
	- DX9(using 11)<br>
	- DX11<br>
	- OPENGL3/4<br>
	- DX12<br>

<hr>

- <b>DX20: Resize Main window</b><br>
 Allow alt enter swap
 And allow user to change the window size

<hr>

- <b>DX21: The COLOR Shader</b><br>
 Introduction to the: Rasterizer State

<hr>

- <b>DX22: The TEXTURE Shader</b><br>
Add third party image readers:<br>
	- BMP<br>
	- JPG<br>
	- PNG<br>
	- TIFF<br>
	- DDS<br>
	- TGA<br>

<hr>

- <b>DX23: The directional LIGHT Shader with environment color</b>

<hr>

- <b>DX24: The 2D Rendering (Sprite)</b>

<hr>

- <b>DX25: The CUBEs (COLOR, TEXTURE and LIGHT Shaders)</b>

<hr>

- <b>DX26: The Spheres (COLOR, TEXTURE and LIGHT Shaders)</b>

<hr>

- <b>DX27: The Rastertek text font</b>

<hr>

- <b>DX28: Sky dome with a sky sphere</b><br>
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

<hr>

- <b>DX29: INTRO DEMO OF BASIC TUTORIAL SERIES</b><br>
	-  Adding the Sound Manager<br>
	-  Adding Play background Music<br>
	-  Generate the first RELEASE BUILD<br>
	-  Add the installer of INTRO DEMO OF BASIC TUTORIAL SERIES<br>
  
 <hr>
 <h2>3D - INTERMEDIATE TUTORIAL SERIES</h2>

<b>M3D base versions:</b><br>
{030}	OBJ Loader "Color"<br>
		- <b>M3D v1.0</b><br>
{031}	OBJ Loader "Texture"<br>
		- <b>M3D v1.1</b><br>
{032}	OBJ Loader "Diffuse Light + Ambient Light"<br>
		- <b>M3D v1.2</b><br>
{033}	OBJ Loader "Diffuse Light"+"Ambient Light"+"Transparent" -> Load Advanced: COMPONDv1.0 with glass<br>
		- <b>M3D v1.2+</b><br>
{034}	OBJ Loader "Specular+Shininess"<br>
		- <b>M3D v1.3</b><br>

<hr>

So in resume... now all of that in a simple xml instead of the C++ demo code is possible? Let's find out...<hr>

- <b>DX30: WORLD.XML: load OBJ 3D file format, using COLOR shader</b><br> 
	- SceneManager and SceneNode<br> 
	- QuadTree and Frustum<br> 
	- <b>World</b> and <b>Objects</b>: Loading objects as a descriptor:	<b>World.xml</b><br>
	- Load <b>obj</b> format<br>
 
 - <b>DX31: WORLD.XML: load OBJ 3D file format, with MULTIPLE TEXTURES, using TEXTURE shader</b><br> 
	- Loading Multiple Textures in one object
	
- <b>DX32: WORLD.XML: load OBJ 3D file format, the first advanced object, using LIGHT shader</b><br> 
	- Loading more complex objects with light, the: OLD HOUSE.
	
 ...<br>
 ...<br>
- <b>DX 160 Full Demo (to be added later?)<br>


![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/1.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/1.png)
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/2.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/2.png)
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/3.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/3.png)
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/4.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/4.png)
![https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/5.png](https://raw.githubusercontent.com/pmborg/WoMA3Dengine/main/thumbs/5.png)
