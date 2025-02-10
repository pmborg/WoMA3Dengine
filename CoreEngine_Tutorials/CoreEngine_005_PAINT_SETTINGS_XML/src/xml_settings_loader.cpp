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
//WomaIntegrityCheck = 1234567311;
 
#define _CRT_SECURE_NO_WARNINGS
#include "OSengine.h"

#include "xml_loader.h"

XMLloader::XMLloader()
{
}

XMLloader::~XMLloader()
{
}


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
		SystemHandle->AppSettings->FULL_SCREEN = false;
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
bool XMLloader::loadConfigSettings (TCHAR* file_) // Note: Have to be char
// -------------------------------------------------------------------------------------------
{
	CHAR XMLFILE[MAX_STR_LEN] = {0}; 
	wtoa(XMLFILE, file_, MAX_STR_LEN); // tchar ==> char
	tinyxml2::XMLDocument doc;

	doc.LoadFile(XMLFILE);

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

