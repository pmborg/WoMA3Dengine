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

// --------------------------------------------------------------------------------------------
// Includes:
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#include "xml_loader.h"

	#include "winsystemclass.h"			// Are we a Windows Instance?

#include "xml_loader.h"
//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID

#if TUTORIAL_PRE_CHAP >= 72
#include "../sound/soundClass.h" // To include SOUND3D
#endif

// --------------------------------------------------------------------------------------------
// Globals:
// --------------------------------------------------------------------------------------------
generalsettings GenSettings;

	TiXmlElement* child_screen = NULL;

#if _NOT_ // ENGINE_LEVEL >= 30
	camera cam;
#endif

// -------------------------------------------------------------------------------------------
bool initAppicationSettings(TCHAR* filename) //Note: Have to be char
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

	#if defined USE_SOUND_MANAGER
	    SystemHandle->AppSettings->SOUND_ENABLED = (strcmp (GenSettings.soundEffectsEnabled, "true") == 0) ?  true : false;
	#endif//

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
bool loadConfigSettings (TCHAR* file_) // Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	CHAR file[MAX_STR_LEN] = {0}; 
	wtoa(file, file_, MAX_STR_LEN); // tchar ==> char

	static TiXmlDocument doc( file );
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

		//CAMERA:

		//FOG:

		//SOUND:
	#if defined USE_SOUND_MANAGER
		/*<sound>*/TiXmlElement* child_sound = root->FirstChildElement( "sound" );
		if ( child_sound )
		{
			/*Element*/TiXmlElement* element = child_sound->ToElement();
			#if defined USE_SOUND_MANAGER
			strcpy (GenSettings.soundEffectsEnabled, element->Attribute("effects"));
			#endif
		}
	#endif

		// PLAYER DEFINITIONS:
	#if TUTORIAL_PRE_CHAP >= 60 // 80
		/*<player>*/TiXmlElement* child_player = root->FirstChildElement( "player" );
		if ( child_player )
		{
			/*Element*/TiXmlElement* element = child_player->ToElement();
			strcpy (GenSettings.playerName, element->Attribute("name"));
			strcpy (GenSettings.faction, element->Attribute("faction"));
			strcpy (GenSettings.meshType, element->Attribute("meshType"));
		}
	#endif

		// SERVER NETWORK SETTINGS:
		//}
	} else
        return false; // File not found for parsing error...

	return true;
}

