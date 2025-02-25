// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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
//WomaIntegrityCheck = 1234567222;
 
#define _CRT_SECURE_NO_WARNINGS

#include "OSengine.h"
#include "OSmain_dir.h"

#if defined USE_TINYXML_LOADER
#if defined ANDROID_PLATFORM
#include "AndroidEngine.h"
#include <android\asset_manager.h>
#endif

#include "xml_loader.h"

XMLloader::XMLloader()
{
}

XMLloader::~XMLloader()
{
}

// -------------------------------------------------------------------------------------------
bool XMLloader::InitWorldLoader(TCHAR* filename) //Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	if (loadWorld (filename))  // <--- PARSE XML FILE
	{
		SystemHandle->AppSettings->SCREEN_NEAR = (float)atof(GenSettings.screenNear);
		SystemHandle->AppSettings->SCREEN_DEPTH = (float)atof(GenSettings.screenDepth);

		SystemHandle->AppSettings->INIT_CAMX = (float)atof(GenSettings.initPosX);
		SystemHandle->AppSettings->INIT_CAMY = (float)atof(GenSettings.initPosY);
		SystemHandle->AppSettings->INIT_CAMZ = (float)atof(GenSettings.initPosZ);

		SystemHandle->AppSettings->INIT_ROTX = (float)atof(GenSettings.initRotX);
		SystemHandle->AppSettings->INIT_ROTY = (float)atof(GenSettings.initRotY);
		SystemHandle->AppSettings->INIT_ROTZ = (float)atof(GenSettings.initRotZ);
	} else
		return false;

	return true;
}

#if defined WINDOWS_PLATFORM
int OPENGL_defaultMonitor()
{
	// Check if we have a monitor
	int default_mon = 0;

	// Iterate over all displays and check if we have a valid one.
	//  If the device ID contains the string default_monitor no monitor is attached.
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(dd);
	int deviceIndex = 0;
	while (EnumDisplayDevices(0, deviceIndex, &dd, 0))
	{
		STRING deviceName = dd.DeviceName;
		int monitorIndex = 0;
		while (EnumDisplayDevices(deviceName.c_str(), monitorIndex, &dd, 0))
		{
			size_t len = _tcslen(dd.DeviceID);
			for (size_t i = 0; i < len; ++i)
				dd.DeviceID[i] = _totlower(dd.DeviceID[i]);
			printf("%s\n", dd.DeviceID);
			//monitor\aci24ac\{4d36e96e-e325-11ce-bfc1-08002be10318}\0002   --> System > Display: 1 (default)
			//monitor\dela198\{4d36e96e-e325-11ce-bfc1-08002be10318}\0003   --> System > Display: 2
			//monitor\aus24a1\{4d36e96e-e325-11ce-bfc1-08002be10318}\0001   --> System > Display: 3

			STRING deviceID = dd.DeviceID;
			int i = (int)deviceID.find_last_of('\\');
			STRING ID = deviceID.substr(i + 1, 4);
			int id = std::stoi(ID) - 1; //"-1" Convert from 1, 2, 3 to 0, 1, 2... Default of the above sample is now: 1
			if (monitorIndex == 0) {
				return id;
			}

			++monitorIndex;
		}
		++deviceIndex;
	}

	return default_mon;
}
#endif

// -------------------------------------------------------------------------------------------
bool XMLloader::initAppicationSettings(TCHAR* filename) //Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	bool res = false;
	try
	{
		res = loadConfigSettings(filename);
	}
	catch (const std::exception& e)
	{
		fprintf(stderr, "main()::Threw an exception: %s\n", e.what());
		WomaMessageBox("loadConfigSettings()::Threw an exception!", TEXT("FATAL ERROR"), MB_OK);
		return EXIT_FAILURE;
	}

	if (res)  // <--- PARSE XML FILE
	{
		// Process DATA imported from XML:
		SystemHandle->AppSettings->UI_MONITOR = atoi(GenSettings.uiMonitor);
		#if defined USE_ALTENTER_SWAP_FULLSCREEN_WINDOWMODE
		SystemHandle->AppSettings->FULL_SCREEN = (strcmp (GenSettings.screenFullScreen, "true") == 0) ?  true : false;
		#else
		SystemHandle->AppSettings->FULL_SCREEN = false;
		#endif
		SystemHandle->AppSettings->WINDOW_Xpos = atoi(GenSettings.posX);
		SystemHandle->AppSettings->WINDOW_Xpos_ori = SystemHandle->AppSettings->WINDOW_Xpos;
		SystemHandle->AppSettings->WINDOW_Ypos = atoi(GenSettings.posY);
		SystemHandle->AppSettings->WINDOW_Ypos_ori = SystemHandle->AppSettings->WINDOW_Ypos;
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

		#if defined DX9
			SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX9") == 0) ? DRIVER_DX9 : -1;
	    #endif
		#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009 // defined WIN10
			if (SystemHandle->AppSettings->DRIVER<0)
				SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX12") == 0) ? DRIVER_DX12 : -1;
	    #endif
		#if defined OPENGL3 //defined WIN6x || defined WIN10 || defined LINUX_PLATFORM
			if (SystemHandle->AppSettings->DRIVER < 0)
				SystemHandle->AppSettings->DRIVER = (strcmp (GenSettings.driverName, "GL3+") == 0) ? DRIVER_GL3: -1;
		#endif
		#if defined DX11 //defined WIN6x || defined WIN10 || defined LINUX_PLATFORM
			if (SystemHandle->AppSettings->DRIVER < 0)
				SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX11") == 0) ? DRIVER_DX11 : -1;
		#endif

		//NOTE: OPENGL Only work wiht main monitor, so force it!:
		//if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
		//	SystemHandle->AppSettings->UI_MONITOR = OPENGL_defaultMonitor();
		}

	//	------------------------------------------------------------------------------------------------------
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
	#if defined OPENGL3
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_GL3;
	#endif
	#if defined DX9	// DX9 using DX11 API
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX9;
	#endif
	#if defined DX9sdk		// Pure DX9
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX9;
	#endif
	}

		//	------------------------------------------------------------------------------------------------------
		SystemHandle->AppSettings->UseAllMonitors = (strcmp(GenSettings.UseAllMonitors, "true") == 0) ? true : false;
		SystemHandle->AppSettings->UseDoubleBuffering = (strcmp(GenSettings.useDoubleBuffering, "true") == 0) ? true : false;

	#if TUTORIAL_PRE_CHAP >= 60 // 80
	    strcpy_s (g_PLAYER_NAME, GenSettings.playerName);
	    g_FACTION = (strcmp (GenSettings.faction, "1") == 0) ?  true : false;
	    g_MESH_TYPE = (BYTE) atoi (GenSettings.meshType);
	#endif

    } else 
        return false;

    return true;
}


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
	doc.LoadFile(XMLFILE);
#endif

	auto root = doc.FirstChildElement( "woma" );
	if ( root )
	{
		//CAMERA:
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

		//FOG:
	} else
		return false;

	return true;
}


// -------------------------------------------------------------------------------------------
bool XMLloader::loadConfigSettings (TCHAR* file_) // Note: Have to be char
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
	STRING dir = WOMA::getCurrentDir();
	tinyxml2::XMLError error = doc.LoadFile(XMLFILE);
	if (error == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
	#if MAINENGINE
		STRING file = "../../../DXEngine_0";
	#else
		STRING file = "../../../CoreEngine_Tutorials/DXENGINE_0";
	#endif
		file.append(std::to_string(DX_ENGINE_LEVEL));
		file.append("/");
		file.append(XMLFILE);
		doc.LoadFile(file.c_str());
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

		//SOUND:

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
		//}
	} else
        return false; // File not found for parsing error...

	return true;
}

#endif
