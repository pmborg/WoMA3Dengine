// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: xml_settings_saver.cpp
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
//  - Debug use local: "file".xml
//  - Release use:     C:\Users\<user>\AppData\Local\Pmborg\Woma2014\"file".xml (WOMA::APPDATA)
//
//*********************************************************************************************

// --------------------------------------------------------------------------------------------
// Includes:
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS	// Ignore: warning C4996
#pragma warning(disable: 4244)
#include "platform.h"
#include "xml_loader.h"
#include "mem_leak.h"

#include "winsystemclass.h"		// Are we a Windows Instance?

//*********************************************************************************************/
bool saveConfigSettings (char* file) // Note: Have to be char
//*********************************************************************************************/
{
	// TUTORIAL: https://www.cs.cmu.edu/~preethi/src/tinyxml/docs/tutorial0.html
	TiXmlDocument doc;

	std::string str = { 0 };
	CHAR stri[MAX_STR_LEN]  = { 0 };

	// SAVE Settings:
	TiXmlElement* root = NEW TiXmlElement( "generalsettings" );
	doc.LinkEndChild(root);
	if ( root )
	{
		///*<screen>*/TiXmlElement* child_screen = root->FirstChildElement( "screen" );
		TiXmlElement* child_screen = NEW TiXmlElement("screen");
		root->LinkEndChild(child_screen);
		if ( child_screen )
		{
			_itoa(SystemHandle->AppSettings->UI_MONITOR, stri, 10);
			child_screen->SetAttribute("uiMonitor", stri);

			str = (SystemHandle->AppSettings->FULL_SCREEN) ? "true" : "false";
			child_screen->SetAttribute("fullScreen", str.c_str());

			_itoa(SystemHandle->AppSettings->WINDOW_Xpos_ori, stri, 10);
			child_screen->SetAttribute("posX", stri);

			_itoa(SystemHandle->AppSettings->WINDOW_Ypos_ori, stri, 10);
			child_screen->SetAttribute("posY", stri);

			_itoa(SystemHandle->AppSettings->WINDOW_WIDTH_ori, stri, 10);
			child_screen->SetAttribute("width", stri);

			_itoa(SystemHandle->AppSettings->WINDOW_HEIGHT_ori, stri, 10);
			child_screen->SetAttribute("height", stri);
				 
			str = (SystemHandle->AppSettings->AllowResize) ? "true" : "false";
			child_screen->SetAttribute("allowResize", str.c_str());

			str = (SystemHandle->AppSettings->VSYNC_ENABLED) ? "true" : "false";
			child_screen->SetAttribute("vsync", str.c_str());

			_itoa(SystemHandle->AppSettings->BITSPERPEL, stri, 10);
			child_screen->SetAttribute("bitsPerPixel", stri);
		}

	}
	else
		return false;

	doc.SaveFile(file);

	return true;
}
