// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: xml_settings_loader.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234567831;
 
// --------------------------------------------------------------------------------------------
// Includes:
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#include "xml_loader.h"

	#include "dxwinsystemclass.h"			// Are we a Windows Instance?

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
		SystemHandle->world.hVisibility = atoi(worldSettings.hVisibility);
		SystemHandle->world.seaLevel = atoi(worldSettings.seaLevel);
		SystemHandle->world.size = atoi(worldSettings.size);
		SystemHandle->world.patchSize = atoi(worldSettings.patchSize);
		SystemHandle->world.skyDayTexture = worldSettings.skyDayTexture;
		SystemHandle->world.skyNightTexture = worldSettings.skyNightTexture;
	} else
		return false;

	return true;
}

// -------------------------------------------------------------------------------------------
bool XMLloader::initAppicationSettings(TCHAR* filename) //Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	if (loadConfigSettings (filename))  // <--- PARSE XML FILE
	{
		// Process DATA imported from XML:
		SystemHandle->AppSettings->UI_MONITOR = atoi(GenSettings.uiMonitor);
		SystemHandle->AppSettings->FULL_SCREEN = (strcmp (GenSettings.screenFullScreen, "true") == 0) ?  true : false;

		SystemHandle->AppSettings->WINDOW_Xpos = atoi(GenSettings.posX);
		SystemHandle->AppSettings->WINDOW_Xpos_ori = SystemHandle->AppSettings->WINDOW_Xpos;
		SystemHandle->AppSettings->WINDOW_Ypos = atoi(GenSettings.posY);
		SystemHandle->AppSettings->WINDOW_Ypos_ori = SystemHandle->AppSettings->WINDOW_Ypos;

		SystemHandle->AppSettings->WINDOW_WIDTH = atoi(GenSettings.screenWidth);
		SystemHandle->AppSettings->WINDOW_WIDTH_ori = SystemHandle->AppSettings->WINDOW_WIDTH;
		SystemHandle->AppSettings->WINDOW_HEIGHT = atoi(GenSettings.screenHeight);
		SystemHandle->AppSettings->WINDOW_HEIGHT_ori = SystemHandle->AppSettings->WINDOW_HEIGHT;

		SystemHandle->AppSettings->AllowResize = (strcmp (GenSettings.allowResize, "true") == 0) ?  true : false;
		SystemHandle->AppSettings->VSYNC_ENABLED = (strcmp(GenSettings.vsync, "true") == 0) ? true : false;
		SystemHandle->AppSettings->BITSPERPEL = atoi(GenSettings.bitsPerPixel);

			SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX9") == 0) ? DRIVER_DX9 : -1;
		#if defined OPENGL3 //defined WIN6x || defined WIN10 || defined LINUX_PLATFORM
			if (SystemHandle->AppSettings->DRIVER < 0)
				SystemHandle->AppSettings->DRIVER = (strcmp (GenSettings.driverName, "GL3+") == 0) ? DRIVER_GL3: -1;
		#endif
			if (SystemHandle->AppSettings->DRIVER < 0)
				SystemHandle->AppSettings->DRIVER = (strcmp(GenSettings.driverName, "DX11") == 0) ? DRIVER_DX11 : -1;

	//	------------------------------------------------------------------------------------------------------
	//	------------------------------------------------------------------------------------------------------
	if (SystemHandle->AppSettings->DRIVER < 0) 
	{

			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX11;
	#if defined OPENGL3
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_GL3;
	#endif
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX9;
	#if defined DX9sdk		// Pure DX9
			if (SystemHandle->AppSettings->DRIVER == -1)
				SystemHandle->AppSettings->DRIVER = DRIVER_DX9;
	#endif
	}

		//	------------------------------------------------------------------------------------------------------
		SystemHandle->AppSettings->UseAllMonitors = (strcmp(GenSettings.UseAllMonitors, "true") == 0) ? true : false;
		SystemHandle->AppSettings->UseDoubleBuffering = (strcmp(GenSettings.useDoubleBuffering, "true") == 0) ? true : false;

	    // MSAA Settings:
		SystemHandle->AppSettings->MSAA_ENABLED = (strcmp (GenSettings.msaa, "true") == 0) ?  true : false; // True is autodetect
		SystemHandle->AppSettings->MSAA_X = 0; // 0 = Autodetecy

	    if (strcmp (GenSettings.msaa, "X1") == 0) {
		    SystemHandle->AppSettings->MSAA_ENABLED = true;
			SystemHandle->AppSettings->MSAA_X = 1;
	    } else
	    if (strcmp (GenSettings.msaa, "X2") == 0) {
		    SystemHandle->AppSettings->MSAA_ENABLED = true;
		    SystemHandle->AppSettings->MSAA_X = 2;
	    } else
	    if (strcmp (GenSettings.msaa, "X4") == 0) {
		    SystemHandle->AppSettings->MSAA_ENABLED = true;
		    SystemHandle->AppSettings->MSAA_X = 4;
	    } else
	    if (strcmp (GenSettings.msaa, "X8") == 0) {
		    SystemHandle->AppSettings->MSAA_ENABLED = true;
		    SystemHandle->AppSettings->MSAA_X = 8;
	    }

		SystemHandle->AppSettings->SCREEN_NEAR  = (float) atof (GenSettings.screenNear);
		SystemHandle->AppSettings->SCREEN_DEPTH = (float) atof (GenSettings.screenDepth);

		SystemHandle->AppSettings->INIT_CAMX = (float) atof (GenSettings.initPosX);
		SystemHandle->AppSettings->INIT_CAMY = (float) atof (GenSettings.initPosY);
		SystemHandle->AppSettings->INIT_CAMZ = (float) atof (GenSettings.initPosZ);

		SystemHandle->AppSettings->INIT_ROTX = (float) atof (GenSettings.initRotX);
		SystemHandle->AppSettings->INIT_ROTY = (float) atof (GenSettings.initRotY);
		SystemHandle->AppSettings->INIT_ROTZ = (float) atof (GenSettings.initRotZ);

        SystemHandle->AppSettings->MaxTextureSize = atoi(GenSettings.MaxTextureSize);
		SystemHandle->AppSettings->bilinear	= (strcmp (GenSettings.Bilinear, "true") == 0) ?  true : false;
		SystemHandle->AppSettings->trilinear	= (strcmp (GenSettings.Trilinear, "true") == 0) ?  true : false;
		
		SystemHandle->AppSettings->Anisotropic = (strcmp (GenSettings.Anisotropic, "true") == 0) ?  true : false;
		SystemHandle->AppSettings->AnisotropicLevel = atoi (GenSettings.AnisotropicLevel);

		SystemHandle->AppSettings->START_FOG = (float) atoi (GenSettings.fogStart);
		SystemHandle->AppSettings->END_FOG = (float) atoi (GenSettings.fogEnd); //SCREEN_DEPTH = END_FOG;

	#if defined USE_PLAY_MUSIC
	    SystemHandle->AppSettings->MUSIC_ENABLED = (strcmp (GenSettings.musicEnabled, "true") == 0) ?  true : false;
	#endif//
	#if DX_ENGINE_LEVEL >= 29 && defined USE_SOUND_MANAGER
	    SystemHandle->AppSettings->SOUND_ENABLED = (strcmp (GenSettings.soundEffectsEnabled, "true") == 0) ?  true : false;
	#endif//

    } else 
        return false;

    return true;
}


// -------------------------------------------------------------------------------------------
bool XMLloader::loadWorld (TCHAR* file_) // Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	CHAR file[MAX_STR_LEN] = { 0 }; wtoa(file, file_, MAX_STR_LEN); // tchar ==> char

	/*static*/ TiXmlDocument doc(file);

	doc.LoadFile();

	/*<woma>*/TiXmlElement* root = doc.FirstChildElement( "woma" );
	if ( root )
	{
		child_world = root->FirstChildElement("world");
		if (child_world) 
		{
			strcpy(worldSettings.hVisibility, child_world->ToElement()->Attribute("hVisibility"));
			strcpy(worldSettings.seaLevel, child_world->ToElement()->Attribute("seaLevel"));
			strcpy(worldSettings.size, child_world->ToElement()->Attribute("size"));
			strcpy(worldSettings.patchSize, child_world->ToElement()->Attribute("patchSize"));
			strcpy(worldSettings.skyDayTexture, child_world->ToElement()->Attribute("skyDayTexture"));
			strcpy(worldSettings.skyNightTexture, child_world->ToElement()->Attribute("skyNightTexture"));
		}
		/*<object>*/TiXmlElement* child_object = root->FirstChildElement("object");
		if (child_object)
		{
			/*Element*/TiXmlElement* element = child_object->ToElement();

			do {
				object3d.fromPage = atoi(element->Attribute("fromPage"));
				object3d.toPage = atoi(element->Attribute("toPage"));
				object3d.depend = atoi(element->Attribute("depend"));

				object3d.scale = (float)atof(element->Attribute("scale"));
				object3d.posX = (float)atof(element->Attribute("posX"));
				object3d.posZ = (float)atof(element->Attribute("posZ"));
				object3d.translateY = (float)atof(element->Attribute("translateY"));

				object3d.rotX = (float)atof(element->Attribute("rotX"));
				object3d.rotY = (float)atof(element->Attribute("rotY"));
				object3d.rotZ = (float)atof(element->Attribute("rotZ"));

				object3d.shader = atoi(element->Attribute("shader"));
				strcpy(object3d.filename, element->Attribute("filename"));
				theWorld.push_back(object3d); // add a new object to our list
				element = element->NextSiblingElement();
			} while (element != NULL);
		}
	} else
		return false;

	return true;
}

// -------------------------------------------------------------------------------------------
bool XMLloader::loadConfigSettings (TCHAR* file_) // Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	CHAR file[MAX_STR_LEN] = {0}; 
	wtoa(file, file_, MAX_STR_LEN); // tchar ==> char

	/*static*/ TiXmlDocument doc( file );
	doc.LoadFile();

	/*<scene>*/TiXmlElement* root = doc.FirstChildElement( "generalsettings" );
	if ( root )
	{

		child_screen = root->FirstChildElement( "screen" );
		if ( child_screen )
		{
			/*Element*/TiXmlElement* element = child_screen->ToElement();
			strcpy (GenSettings.uiMonitor, element->Attribute("uiMonitor"));
			strcpy (GenSettings.screenFullScreen, element->Attribute("fullScreen"));

			strcpy (GenSettings.posX, element->Attribute("posX"));
			strcpy (GenSettings.posY, element->Attribute("posY"));

			strcpy (GenSettings.screenWidth, element->Attribute("width"));
			strcpy (GenSettings.screenHeight, element->Attribute("height"));

			strcpy(GenSettings.allowResize, element->Attribute("allowResize"));
			strcpy(GenSettings.vsync, element->Attribute("vsync"));
			strcpy (GenSettings.bitsPerPixel, element->Attribute("bitsPerPixel"));
		}

		/*<driver>*/TiXmlElement* child_driver = root->FirstChildElement( "driver" );
		if ( child_driver )
		{
			/*Element*/TiXmlElement* element = child_driver->ToElement();
			strcpy (GenSettings.driverName, element->Attribute("name"));

			strcpy(GenSettings.UseAllMonitors, element->Attribute("UseAllMonitors"));
			strcpy (GenSettings.useDoubleBuffering, element->Attribute("useDoubleBuffering"));
			strcpy (GenSettings.msaa, element->Attribute("msaa"));
		}

		//CAMERA:
		/*<camera>*/TiXmlElement* element_camera = root->FirstChildElement( "camera" );
		////////////
		if ( element_camera )
		{
			strcpy (GenSettings.screenNear, element_camera->Attribute("screenNear"));
			strcpy (GenSettings.screenDepth, element_camera->Attribute("screenDepth"));

			/*<initPos>*/TiXmlElement* child_initPos = element_camera->FirstChildElement( "initPos" );
			if ( child_initPos )
			{
				/*Element*/TiXmlElement* element = child_initPos->ToElement();
				strcpy (GenSettings.initPosX, element->Attribute("x"));
				strcpy (GenSettings.initPosY, element->Attribute("y"));
				strcpy (GenSettings.initPosZ, element->Attribute("z"));
			}
			/*<initRot>*/TiXmlElement* child_initRot = element_camera->FirstChildElement( "initRot" );
			if ( child_initPos )
			{
				/*Element*/TiXmlElement* element = child_initRot->ToElement();
				strcpy (GenSettings.initRotX, element->Attribute("x"));
				strcpy (GenSettings.initRotY, element->Attribute("y"));
				strcpy (GenSettings.initRotZ, element->Attribute("z"));
			}
		}

		//TEXTURE:
		/*<texture>*/TiXmlElement* child_texture = root->FirstChildElement( "texture" );
		if ( child_texture )
		{
			/*Element*/TiXmlElement* element = child_texture->ToElement();

			strcpy(GenSettings.Bilinear, element->Attribute("bilinear"));
			strcpy(GenSettings.Trilinear, element->Attribute("trilinear"));
			strcpy(GenSettings.Anisotropic, element->Attribute("Anisotropic"));
			strcpy(GenSettings.AnisotropicLevel, element->Attribute("AnisotropicLevel"));
			strcpy(GenSettings.MaxTextureSize, element->Attribute("maxTexture"));
		}

		//FOG:

		//SOUND:
	#if DX_ENGINE_LEVEL >= 29 && defined USE_SOUND_MANAGER
		/*<sound>*/TiXmlElement* child_sound = root->FirstChildElement( "sound" );
		if ( child_sound )
		{
			/*Element*/TiXmlElement* element = child_sound->ToElement();
			#if	defined USE_PLAY_MUSIC
			strcpy (GenSettings.musicEnabled, element->Attribute("music"));
			#endif
			#if DX_ENGINE_LEVEL >= 29 && defined USE_SOUND_MANAGER
			strcpy (GenSettings.soundEffectsEnabled, element->Attribute("effects"));
			#endif
		}
	#endif

		// PLAYER DEFINITIONS:

		// SERVER NETWORK SETTINGS:
		//}
	} else
        return false; // File not found for parsing error...

	return true;
}

