// --------------------------------------------------------------------------------------------
// Filename: xml_settings_loader.cpp
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//  REMEMBER:
//  - Debug use local: "settings".xml
//  - Release use:     C:\Users\<user>\AppData\Local\Pmborg\Woma2017\"settings".xml (WOMA::APPDATA)
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;
 
//#define _CRT_SECURE_NO_WARNINGS

#include "OSengine.h"
#include "OSmain_dir.h"
#include "fileLoader.h"
#if defined USE_TINYXML_LOADER
#if defined ANDROID_PLATFORM
#include "AndroidEngine.h"
#include <android\asset_manager.h>
#endif

#include "xml_loader.h"

#if DX_ENGINE_LEVEL >= 72
#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D //SOUND3D
#include "soundclass.h"
#endif
#include "mem_leak.h"
#endif

XMLloader::XMLloader()
{
	CLASSLOADER();
}

XMLloader::~XMLloader()
{
#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D //SOUND3D
	for (size_t i = 0; i < theWorld.size(); i++)
	{
		SAFE_SHUTDOWN(theWorld[i].audio);
	}
#endif
	CLASSDELETE();
}

#if DX_ENGINE_LEVEL >= 20 // Initializing Engine: <world hVisibility="128" seaLevel="0" size="512" patchSize="256" skyTexture="">
// -------------------------------------------------------------------------------------------
bool XMLloader::InitWorldLoader(TCHAR* filename) //Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	if (loadWorld (filename))  // <--- PARSE XML FILE
	{
	#if defined INTRO_DEMO || DX_ENGINE_LEVEL >= 21 // Color Shader	// INITIAL CAMERA POSITION:
		SystemHandle->AppSettings->SCREEN_NEAR = (float)atof(GenSettings.screenNear);
		SystemHandle->AppSettings->SCREEN_DEPTH = (float)atof(GenSettings.screenDepth);

		SystemHandle->AppSettings->INIT_CAMX = (float)atof(GenSettings.initPosX);
		SystemHandle->AppSettings->INIT_CAMY = (float)atof(GenSettings.initPosY);
		SystemHandle->AppSettings->INIT_CAMZ = (float)atof(GenSettings.initPosZ);

		SystemHandle->AppSettings->INIT_ROTX = (float)atof(GenSettings.initRotX);
		SystemHandle->AppSettings->INIT_ROTY = (float)atof(GenSettings.initRotY);
		SystemHandle->AppSettings->INIT_ROTZ = (float)atof(GenSettings.initRotZ);
	#endif
	#if DX_ENGINE_LEVEL >= 30
		SystemHandle->world.hVisibility = atoi(worldSettings.hVisibility);
		SystemHandle->world.seaLevel = atoi(worldSettings.seaLevel);
		SystemHandle->world.size = atoi(worldSettings.size);
		SystemHandle->world.patchSize = atoi(worldSettings.patchSize);
		SystemHandle->world.skySize = atoi(worldSettings.skySize);

		SystemHandle->world.clearColorR = (float)atoi(worldSettings.clearColorR);
		SystemHandle->world.clearColorG = (float)atoi(worldSettings.clearColorG);
		SystemHandle->world.clearColorB = (float)atoi(worldSettings.clearColorB);

		SystemHandle->world.water = worldSettings.water;
		SystemHandle->world.waterTexture = worldSettings.waterTexture;
		SystemHandle->world.mainTexture = worldSettings.mainTexture;
		SystemHandle->world.skyDayTexture = worldSettings.skyDayTexture;
		SystemHandle->world.skyNightTexture = worldSettings.skyNightTexture;
	#endif
	#if DX_ENGINE_LEVEL >= 31 //FOG:
		SystemHandle->AppSettings->START_FOG = (float)atoi(GenSettings.fogStart);
		SystemHandle->AppSettings->END_FOG = (float)atoi(GenSettings.fogEnd); //SCREEN_DEPTH = END_FOG;
	#endif
	} else
		return false;

	return true;
}
#endif


int OPENGL_defaultMonitor() 
{
	int monitorIndex = 0;
#if defined WINDOWS_PLATFORM
	// Retrieve the primary monitor information
	DISPLAY_DEVICE dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.cb = sizeof(dd);

	// Enumerate display devices to get the default (primary) monitor
	monitorIndex = -1;
	for (int i = 0; EnumDisplayDevices(NULL, i, &dd, 0); ++i) {
		if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
			// Check if the current display device is the primary monitor
			DEVMODE devmode;
			ZeroMemory(&devmode, sizeof(devmode));
			devmode.dmSize = sizeof(devmode);
			if (EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &devmode)) {
				if (devmode.dmPosition.x == 0 && devmode.dmPosition.y == 0) {
					monitorIndex = i; // Primary monitor found
					break;
				}
			}
		}
	}

	// Output the result
	if (monitorIndex != -1) {
		std::cout << "Default monitor index: " << monitorIndex << std::endl;
	}
	else {
		std::cout << "Primary monitor not found!" << std::endl;
	}
#endif
	return monitorIndex;
}



// -------------------------------------------------------------------------------------------
bool XMLloader::initAppicationSettings(TCHAR* filename) //Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	bool res = false;
	try
	{
		res = loadXMLsettingsFile(filename);
	}
	catch (const std::exception& e)
	{
		fprintf(stderr, "main()::Threw an exception: %s\n", e.what());
		WomaMessageBox(TEXT("XMLloader::initAppicationSettings exception!"), TEXT("FATAL ERROR"), MB_OK);
		return EXIT_FAILURE;
	}

	if (res)  // <--- PARSE XML FILE
	{
		// Process DATA imported from XML:
		SystemHandle->AppSettings->UI_MONITOR = atoi(GenSettings.uiMonitor);
		#if defined USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE || CORE_ENGINE_LEVEL < 9
		SystemHandle->AppSettings->FULL_SCREEN = (strcmp (GenSettings.screenFullScreen, "true") == 0) ?  true : false;
        SystemHandle->AppSettings->FULLSCREEN_ON_WINDOWED = (strcmp(GenSettings.screenFullScreenWindowed, "true") == 0) ? true : false;
		#else
		SystemHandle->AppSettings->FULL_SCREEN = false;
		#endif
	#if CORE_ENGINE_LEVEL < 28 // Moved to WORLD
		SystemHandle->AppSettings->WINDOW_Xpos = atoi(GenSettings.posX);
		SystemHandle->AppSettings->WINDOW_Xpos_ori = SystemHandle->AppSettings->WINDOW_Xpos;
		SystemHandle->AppSettings->WINDOW_Ypos = atoi(GenSettings.posY);
		SystemHandle->AppSettings->WINDOW_Ypos_ori = SystemHandle->AppSettings->WINDOW_Ypos;
	#endif
		SystemHandle->AppSettings->WINDOW_WIDTH = atoi(GenSettings.screenWidth);
		SystemHandle->AppSettings->WINDOW_WIDTH_ori = SystemHandle->AppSettings->WINDOW_WIDTH;
		SystemHandle->AppSettings->WINDOW_HEIGHT = atoi(GenSettings.screenHeight);
		SystemHandle->AppSettings->WINDOW_HEIGHT_ori = SystemHandle->AppSettings->WINDOW_HEIGHT;

#if defined USE_ALLOW_RESIZE
		SystemHandle->AppSettings->AllowResize = (strcmp (GenSettings.allowResize, "true") == 0) ?  true : false;
#else
		SystemHandle->AppSettings->AllowResize = false;
#endif
		SystemHandle->AppSettings->VSYNC_ENABLED = (strcmp(GenSettings.vsync, "true") == 0) ? true : false;
		SystemHandle->AppSettings->BITSPERPEL = atoi(GenSettings.bitsPerPixel);

	#if CORE_ENGINE_LEVEL >= 10 // Initializing Engine
		#if defined DX9
			SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX9") == 0) ? DRIVER_DX9 : -1;
	    #endif
		#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 // defined WIN10
			if (SystemHandle->AppSettings->DRIVER<0)
				SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX12") == 0) ? DRIVER_DX12 : -1;
	    #endif
        #if (defined OPENGL3 || defined OPENGL40)  //defined WIN6x || defined WIN10 || defined LINUX_PLATFORM
			if (SystemHandle->AppSettings->DRIVER < 0)
				SystemHandle->AppSettings->DRIVER = (strcmp (GenSettings.driverName, "GL3+") == 0) ? DRIVER_GL3: -1;
		#endif
		#if defined DX11 //defined WIN6x || defined WIN10 || defined LINUX_PLATFORM
			if (SystemHandle->AppSettings->DRIVER < 0)
				SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX11") == 0) ? DRIVER_DX11 : -1;
		#endif

		//NOTE: OPENGL Only work wiht main monitor, so force it!:
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
			SystemHandle->AppSettings->UI_MONITOR = OPENGL_defaultMonitor();
			SystemHandle->AppSettings->FULL_SCREEN = false;
			if (SystemHandle->AppSettings->WINDOW_WIDTH == 0)
				SystemHandle->AppSettings->WINDOW_WIDTH = 1920;
			if (SystemHandle->AppSettings->WINDOW_HEIGHT == 0)
				SystemHandle->AppSettings->WINDOW_HEIGHT = 1080;
		}

	//	------------------------------------------------------------------------------------------------------
#if CORE_ENGINE_LEVEL >= 10 // Initializing Engine
	//	------------------------------------------------------------------------------------------------------
	if (SystemHandle->AppSettings->DRIVER < 0) 
	{
	#if defined LINUX_PLATFORM
	// LINUX
			SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "GL3+") == 0) ? DRIVER_GL3 : -1;
	#endif

	#if defined DX12
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX12;
	#endif
	#if defined DX11 //Pure DX11
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX11;
	#endif
#if (defined OPENGL3 || defined OPENGL40) 
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_GL3;
	#endif
	#if defined DX9	// DX9 using DX11 API
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX9;
	#endif
	#if _NOT //defined DX9sdk		// Pure DX9
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX9;
	#endif
	}
#endif

		//	------------------------------------------------------------------------------------------------------
		SystemHandle->AppSettings->UseAllMonitors = (strcmp(GenSettings.UseAllMonitors, "true") == 0) ? true : false;
		SystemHandle->AppSettings->UseDoubleBuffering = (strcmp(GenSettings.useDoubleBuffering, "true") == 0) ? true : false;
	#endif

	#if defined INTRO_DEMO || DX_ENGINE_LEVEL >= 22 // Texturing	
        SystemHandle->AppSettings->MaxTextureSize = atoi(GenSettings.MaxTextureSize);

		//MSAA_ENABLED = MSAA_point if (MSAA_bilinear = MSAA_trilinear = MSAA_Anisotropic = false)
		//SystemHandle->AppSettings->MSAA_ENABLED = (strcmp(GenSettings.msaa, "true") == 0) ? true : false;
		SystemHandle->AppSettings->MSAA_bilinear = (strcmp (GenSettings.Bilinear, "true") == 0) ?  true : false;
		SystemHandle->AppSettings->MSAA_trilinear	= (strcmp (GenSettings.Trilinear, "true") == 0) ?  true : false;

		SystemHandle->AppSettings->MSAA_Anisotropic = (strcmp (GenSettings.Anisotropic, "true") == 0) ?  true : false;
		SystemHandle->AppSettings->MSAA_AnisotropicLevel = atoi (GenSettings.AnisotropicLevel);			// 0 = Auto detect MAX
	#endif

	#if DX_ENGINE_LEVEL >= 29 //&& defined USE_WIN32_PLAY_MUSIC
	    SystemHandle->AppSettings->MUSIC_ENABLED = (strcmp (GenSettings.musicEnabled, "true") == 0) ?  true : false;
	#endif
	#if DX_ENGINE_LEVEL >= 29 //&& defined USE_WIN32_SOUND_MANAGER
	    SystemHandle->AppSettings->SOUND_ENABLED = (strcmp (GenSettings.soundEffectsEnabled, "true") == 0) ?  true : false;
	#endif

	#if TUTORIAL_PRE_CHAP >= 60 // 80
	    strcpy_s (g_PLAYER_NAME, GenSettings.playerName);
	    g_FACTION = (strcmp (GenSettings.faction, "1") == 0) ?  true : false;
	    g_MESH_TYPE = (BYTE) atoi (GenSettings.meshType);
	#endif

	#if DX_ENGINE_LEVEL >= 140 // #if TUTORIAL_PRE_CHAP >= 90
	    SystemHandle->AppSettings->NETWORK_ENABLED = (strcmp (GenSettings.networkEnabled, "true") == 0) ?  true : false;
	    strcpy_s (SystemHandle->AppSettings->SERVER_ADDRESS, sizeof(SystemHandle->AppSettings->SERVER_ADDRESS), GenSettings.networkServerIP);
	    SystemHandle->AppSettings->SERVER_PORT = (unsigned short) atoi (GenSettings.networkPort);
	#endif

    } else 
        return false;

    return true;
}


#if DX_ENGINE_LEVEL >= 20 // Initializing Engine
// -------------------------------------------------------------------------------------------
bool XMLloader::loadWorld (TCHAR* file_) // Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	CHAR XMLFILE[MAX_STR_LEN] = { 0 }; wtoa(XMLFILE, file_, MAX_STR_LEN); // tchar ==> char

	///*static*/ /*TiXmlElement*/ tinyxml2::XMLElement; 
	tinyxml2::XMLDocument doc;

#if defined ANDROID_PLATFORM
	//STRING XML_FILE = LOAD_ASSET_SAVE_TO_CACHE(XMLFILE);
	// 
	//LOAD FROM: C:\WoMAengine2023\Android-WomaEngine\Android2\Android2.Packaging\ARM64\Debug\Package\assets
	AAssetManager* manager = engine_state.app->activity->assetManager;
	AAsset* thisxmlFile = AAssetManager_open(manager, XMLFILE, AASSET_MODE_BUFFER);
	const char* fileBuffer = (char*)AAsset_getBuffer(thisxmlFile);
	off_t fileSize = AAsset_getLength(thisxmlFile);

	//SAVE TO: /data/user/0/com.woma/cache/
	FILE* saveXmlFile = NULL;
	STRING XML_FILE = WOMA::android_temp_folder(engine_state.app);
	XML_FILE.append(TEXT("/"));
	XML_FILE.append(XMLFILE);
	_tprintf("FILE: %s", XML_FILE.c_str());

	UINT errno_t = _tfopen_s(&saveXmlFile, XML_FILE.c_str(), TEXT("w"));
	fwrite(fileBuffer, sizeof(char), fileSize, saveXmlFile);
#if DEBUG
	__android_log_print(ANDROID_LOG_ERROR, "[WOMA]", fileBuffer);
#endif
	AAsset_close(thisxmlFile);
	fclose(saveXmlFile);

	doc.LoadFile(XML_FILE.c_str());
#else
	//WORLD.XML
	STRING dir = WOMA::getCurrentDir();
	tinyxml2::XMLError error = doc.LoadFile(XMLFILE);
	if (error == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
	#if MAINENGINE
		CHAR home[MAX_STR_LEN] = { 0 }; wtoa(home, WOMA::Home, MAX_STR_LEN); // wchar ==> char
		std::string file = home;
		file.append("/projects/LinuxWoma/DXEngine_055/"); //need to be: std::string
	#else
		#if defined LINUX_PLATFORM && CORE_ENGINE_LEVEL < 10
			STRING file = WOMA::Home;
			file.append(DEMO_ROOT_DIR);
		#else
			std::string file = "../../../CoreEngine_Tutorials/DXENGINE_0"; //CHAR
			file.append(std::to_string(DX_ENGINE_LEVEL));
		#endif
	#endif
		file.append("/");
		file.append(XMLFILE);
		#if defined RELEASE
			doc.LoadFile(XMLFILE);
		#else
			doc.LoadFile(file.c_str());
		#endif
	}
#endif

	auto root = doc.FirstChildElement( "woma" );
	if ( root )
	{
		//CAMERA:
	#if DX_ENGINE_LEVEL >= 21 // Color Shader
		/*<camera>*//*TiXmlElement*/ tinyxml2::XMLElement* element_camera = root->FirstChildElement("camera");
		////////////
		if (element_camera)
		{
			strcpy(GenSettings.screenNear, element_camera->Attribute("screenNear"));
			strcpy(GenSettings.screenDepth, element_camera->Attribute("screenDepth"));

			/*<initPos>*//*TiXmlElement*/ tinyxml2::XMLElement* child_initPos = element_camera->FirstChildElement("initPos");
			if (child_initPos)
			{
				/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_initPos->ToElement();
				strcpy(GenSettings.initPosX, element->Attribute("x"));
				strcpy(GenSettings.initPosY, element->Attribute("y"));
				strcpy(GenSettings.initPosZ, element->Attribute("z"));
			}
			/*<initRot>*//*TiXmlElement*/ tinyxml2::XMLElement* child_initRot = element_camera->FirstChildElement("initRot");
			if (child_initPos)
			{
				/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_initRot->ToElement();
				strcpy(GenSettings.initRotX, element->Attribute("x"));
				strcpy(GenSettings.initRotY, element->Attribute("y"));
				strcpy(GenSettings.initRotZ, element->Attribute("z"));
			}
		}
	#endif

	#if DX_ENGINE_LEVEL >= 30 // Initializing Engine
		child_world = root->FirstChildElement("world");
		if (child_world) 
		{
			strcpy(worldSettings.hVisibility, child_world->ToElement()->Attribute("hVisibility"));
			strcpy(worldSettings.seaLevel, child_world->ToElement()->Attribute("seaLevel"));
			strcpy(worldSettings.size, child_world->ToElement()->Attribute("size"));
			strcpy(worldSettings.patchSize, child_world->ToElement()->Attribute("patchSize"));
		#if DX_ENGINE_LEVEL >= 60
			strcpy(worldSettings.skySize, child_world->ToElement()->Attribute("skySize"));
		#endif
		#if DX_ENGINE_LEVEL >= 62
			strcpy(worldSettings.clearColorR, child_world->ToElement()->Attribute("clearColorR"));
			strcpy(worldSettings.clearColorG, child_world->ToElement()->Attribute("clearColorG"));
			strcpy(worldSettings.clearColorB, child_world->ToElement()->Attribute("clearColorB"));
		#endif
		#if DX_ENGINE_LEVEL >= 54
				strcpy(worldSettings.water, child_world->ToElement()->Attribute("water"));
				strcpy(worldSettings.waterTexture, child_world->ToElement()->Attribute("waterTexture"));
		#endif
		#if DX_ENGINE_LEVEL >= 50
			strcpy(worldSettings.mainTexture, child_world->ToElement()->Attribute("mainTexture"));
		#endif
			strcpy(worldSettings.skyDayTexture, child_world->ToElement()->Attribute("skyDayTexture"));
			strcpy(worldSettings.skyNightTexture, child_world->ToElement()->Attribute("skyNightTexture"));
		}
		/*<object>*//*TiXmlElement*/ tinyxml2::XMLElement* child_object = root->FirstChildElement("object");
		if (child_object)
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_object->ToElement();

			do {
	#if DX_ENGINE_LEVEL >= 40
				object3d.id = atoi(element->Attribute("id"));
	#endif
				object3d.fromPage = atoi(element->Attribute("fromPage"));
				object3d.toPage = atoi(element->Attribute("toPage"));
				object3d.depend = atoi(element->Attribute("depend"));

				object3d.scale = (float)atof(element->Attribute("scale"));
                if (element->Attribute("scaleY"))
                    object3d.scaleY = (float)atof(element->Attribute("scaleY"));
                else
                    object3d.scaleY = 1;
				object3d.posX = (float)atof(element->Attribute("posX"));
				object3d.posZ = (float)atof(element->Attribute("posZ"));
				object3d.translateY = (float)atof(element->Attribute("translateY"));
#if _NOT //DX_ENGINE_LEVEL >= 89
                STRING s = element->Attribute("rotY");//debug only
                if (s == "PI") {
                    object3d.rotY = static_cast<float>(PI);
                }
                else 
                if (s == "PI/2") {
                    object3d.rotY = static_cast<float>(PI/2);
                }
                else
                if (s == "-PI/2") {
                    object3d.rotY = static_cast<float>(-PI/2);
                }
                else
#endif
                {
                    object3d.rotY = (float)atof(element->Attribute("rotY"));
                }
        
                object3d.rotX = (float)atof(element->Attribute("rotX"));
                object3d.rotZ = (float)atof(element->Attribute("rotZ"));
        
				object3d.shader = atoi(element->Attribute("shader"));
				strcpy(object3d.filename, element->Attribute("filename"));
	#if DX_ENGINE_LEVEL >= 40
				object3d.instances = atoi(element->Attribute("instances"));
	#endif
	#if DX_ENGINE_LEVEL >= 41 && defined USE_SHADOW_INSTANCES
				object3d.castShadow = atoi(element->Attribute("castShadow"));
				object3d.renderShadows = atoi(element->Attribute("renderShadows"));
	#endif
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D //SOUND3D
                const TCHAR* soundRange = element->Attribute("soundRange");
                if (soundRange) 
                {
                    object3d.soundRange = (float)atof(soundRange);
                    if (object3d.soundRange > 0)
                    {
                        strcpy(object3d.audioFilename, element->Attribute("audio"));
                        object3d.audio = NEW SoundClass;
                        if (!object3d.audio->Initialize(SystemHandle->m_hWnd, WOMA::LoadFile(object3d.audioFilename)))
                        {
                            WomaMessageBox(TEXT("Could not initialize Direct 3D Sound"), TEXT("Error: "));
                        }

                        IF_NOT_RETURN_FALSE(object3d.audio->PlayWaveFile(object3d.posX, 1, object3d.posZ, /*loop?*/true, object3d.soundRange));
                    }
                }
	#endif
				theWorld.push_back(object3d); // add a new object to our list
				element = element->NextSiblingElement();
			} while (element != NULL);
		}
	#endif
		//FOG:
	#if DX_ENGINE_LEVEL >= 31
		/*<fog>*//*TiXmlElement*/ tinyxml2::XMLElement* child_fog = root->FirstChildElement("fog");
		if (child_fog)
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_fog->ToElement();
			strcpy(GenSettings.fogStart, element->Attribute("start"));
			strcpy(GenSettings.fogEnd, element->Attribute("end"));
		}
	#endif
	} else
		return false;

	return true;
}
#endif


// -------------------------------------------------------------------------------------------
bool XMLloader::loadXMLsettingsFile (TCHAR* file_) // Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	CHAR XMLFILE[MAX_STR_LEN] = {0}; 
	wtoa(XMLFILE, file_, MAX_STR_LEN); // tchar ==> char
	tinyxml2::XMLDocument doc;

#if defined ANDROID_PLATFORM
	//STRING XML_FILE = LOAD_ASSET_SAVE_TO_CACHE(XMLFILE);

	//LOAD FROM: C:\WoMAengine2023\Android-WomaEngine\Android2\Android2.Packaging\ARM64\Debug\Package\assets
	AAssetManager* manager = engine_state.app->activity->assetManager;
	AAsset* thisxmlFile = AAssetManager_open(manager, XMLFILE, AASSET_MODE_BUFFER);
	const char* fileBuffer = (char*)AAsset_getBuffer(thisxmlFile);
	off_t fileSize = AAsset_getLength(thisxmlFile);

	//SAVE TO: /data/user/0/com.woma/cache/
	FILE* saveXmlFile = NULL;
	STRING XML_FILE = WOMA::android_temp_folder(engine_state.app);
	XML_FILE.append(TEXT("/"));
	XML_FILE.append(XMLFILE);
	_tprintf("FILE: %s", XML_FILE.c_str());

	UINT errno_t = _tfopen_s(&saveXmlFile, XML_FILE.c_str(), TEXT("w"));
	fwrite(fileBuffer, sizeof(char), fileSize, saveXmlFile);
#if DEBUG
	__android_log_print(ANDROID_LOG_ERROR, "[WOMA]", fileBuffer);
#endif
	AAsset_close(thisxmlFile);
	fclose(saveXmlFile);

	doc.LoadFile(XML_FILE.c_str());
#else
	//SETTINGS.XML
    STRING dir=WOMA::getCurrentDir();
	tinyxml2::XMLError error = doc.LoadFile(XMLFILE);
	if (error == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
	#if MAINENGINE
		CHAR home[MAX_STR_LEN] = { 0 }; wtoa(home, WOMA::Home, MAX_STR_LEN); // wchar ==> char
		std::string file = home;
		file.append("/projects/LinuxWoma/DXEngine_055/"); //need to be: std::string
	#else
		#if defined LINUX_PLATFORM && CORE_ENGINE_LEVEL < 10
			STRING file = WOMA::Home;
			file.append(DEMO_ROOT_DIR);
		#else
			std::string file = "../../../CoreEngine_Tutorials/DXENGINE_0"; //CHAR
			file.append(std::to_string(DX_ENGINE_LEVEL));
		#endif
	#endif
		file.append("/");
		file.append(XMLFILE);
		#if defined RELEASE
			doc.LoadFile(XMLFILE);
		#else
			doc.LoadFile(file.c_str());
		#endif
	}
#endif

	auto root = doc.FirstChildElement(GENERALSETTINGS); //auto root = doc.FirstChildElement( "generalsettings" );
	ASSERT(root);
	if ( root )
	{
		/*<screen>*//*TiXmlElement*/ tinyxml2::XMLElement* child_screen = root->FirstChildElement( "screen" );
		if ( child_screen )
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_screen->ToElement();
			strcpy (GenSettings.uiMonitor, element->Attribute("uiMonitor"));
			strcpy (GenSettings.screenFullScreen, element->Attribute("fullScreen"));
            if (element->Attribute("fullScreenWindowed"))
                strcpy(GenSettings.screenFullScreenWindowed, element->Attribute("fullScreenWindowed"));

			//Moved to WORLD.XML
			//strcpy (GenSettings.posX, element->Attribute("posX"));
			//strcpy (GenSettings.posY, element->Attribute("posY"));

			strcpy (GenSettings.screenWidth, element->Attribute("width"));
			strcpy (GenSettings.screenHeight, element->Attribute("height"));

			strcpy(GenSettings.allowResize, element->Attribute("allowResize"));
			strcpy(GenSettings.vsync, element->Attribute("vsync"));
			strcpy (GenSettings.bitsPerPixel, element->Attribute("bitsPerPixel"));
		}
		else
			return false;

	#if CORE_ENGINE_LEVEL >= 10 // Initializing Engine
		/*<driver>*//*TiXmlElement*/ tinyxml2::XMLElement* child_driver = root->FirstChildElement( "driver" );
		if ( child_driver )
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_driver->ToElement();
			#if defined LINUX_PLATFORM
				strcpy(GenSettings.driverName, TEXT("GL3+"));
			#else
				strcpy(GenSettings.driverName, element->Attribute("name"));
			#endif
			strcpy(GenSettings.UseAllMonitors, element->Attribute("UseAllMonitors"));
			strcpy (GenSettings.useDoubleBuffering, element->Attribute("useDoubleBuffering"));
		}
		else
			return false;
	#endif

	#if DX_ENGINE_LEVEL >= 22 // Texturing
		//TEXTURE:
		/*<texture>*//*TiXmlElement*/ tinyxml2::XMLElement* child_texture = root->FirstChildElement( "texture" );
		if ( child_texture )
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_texture->ToElement();
			//strcpy(GenSettings.msaa, element->Attribute("msaa"));
			strcpy(GenSettings.Bilinear, element->Attribute("bilinear"));
			strcpy(GenSettings.Trilinear, element->Attribute("trilinear"));
			strcpy(GenSettings.Anisotropic, element->Attribute("Anisotropic"));
			strcpy(GenSettings.AnisotropicLevel, element->Attribute("AnisotropicLevel"));
			strcpy(GenSettings.MaxTextureSize, element->Attribute("maxTexture"));
		}
		else
			return false;
	#endif

		//SOUND:
	#if DX_ENGINE_LEVEL >= 29
		/*<sound>*//*TiXmlElement*/ tinyxml2::XMLElement* child_sound = root->FirstChildElement( "sound" );
		if ( child_sound )
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_sound->ToElement();
			#if	defined USE_WIN32_PLAY_MUSIC
			strcpy (GenSettings.musicEnabled, element->Attribute("music"));
			#endif
			#if DX_ENGINE_LEVEL >= 29 && defined USE_WIN32_SOUND_MANAGER
			strcpy (GenSettings.soundEffectsEnabled, element->Attribute("effects"));
			#endif
		}
		else
			return false;
	#endif

		// PLAYER DEFINITIONS:
	#if TUTORIAL_PRE_CHAP >= 60 // 80
		/*<player>*//*TiXmlElement*/ tinyxml2::XMLElement* child_player = root->FirstChildElement( "player" );
		if ( child_player )
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_player->ToElement();
			strcpy (GenSettings.playerName, element->Attribute("name"));
			strcpy (GenSettings.faction, element->Attribute("faction"));
			strcpy (GenSettings.meshType, element->Attribute("meshType"));
		}
		else
			return false;
	#endif

		// SERVER NETWORK SETTINGS:
	#if ENGINE_LEVEL >= 140 // #if TUTORIAL_PRE_CHAP >= 90
		/*<network>*//*TiXmlElement*/ tinyxml2::XMLElement* child_network = root->FirstChildElement( "network" );
		if ( child_network )
		{
			/*Element*//*TiXmlElement*/ tinyxml2::XMLElement* element = child_network->ToElement();
			strcpy (GenSettings.networkEnabled, element->Attribute("enabled"));
			strcpy (GenSettings.networkServerIP, element->Attribute("serverIP"));
			strcpy (GenSettings.networkPort, element->Attribute("serverPort"));
		}
		else
			return false;
	#endif
		//}
	} else
        return false; // File not found for parsing error...

	return true;
}

#endif