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

	//static TiXmlDocument doc( file );
	//static TiXmlDocument doc( "new.xml" );
	TiXmlDocument doc;

	//Optional:
	//TiXmlDeclaration* decl = NEW TiXmlDeclaration("1.0", "", "");
	//doc.LinkEndChild(decl);

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

#if _NOT_
		///*<texture>*/TiXmlElement* child_texture = root->FirstChildElement("texture");
		TiXmlElement* child_texture = NEW TiXmlElement("texture");
		root->LinkEndChild(child_texture);
		if (child_texture)
		{
			_itoa(SystemHandle->AppSettings->MaxTextureSize, stri, 10);
			child_texture->SetAttribute("maxTexture", stri);

			str = (SystemHandle->AppSettings->bilinear) ? "true" : "false";
			child_texture->SetAttribute("bilinear", str.c_str());

			str = (SystemHandle->AppSettings->trilinear) ? "true" : "false";
			child_texture->SetAttribute("trilinear", str.c_str());

			str = (SystemHandle->AppSettings->Anisotropic) ? "true" : "false";
			child_texture->SetAttribute("Anisotropic", str.c_str());

			_itoa(SystemHandle->AppSettings->AnisotropicLevel, stri, 10);
			child_texture->SetAttribute("AnisotropicLevel", stri);
		}

		///*<fog>*/TiXmlElement* child_fog = root->FirstChildElement("fog");
		TiXmlElement* child_fog = NEW TiXmlElement("fog");
		root->LinkEndChild(child_fog);
		if (child_fog)
		{
			_itoa(SystemHandle->AppSettings->START_FOG, stri, 10);
			child_fog->SetAttribute("start", stri);

			_itoa(SystemHandle->AppSettings->END_FOG, stri, 10);
			child_fog->SetAttribute("end", stri);
		}

		///*<sound>*/TiXmlElement* child_sound = root->FirstChildElement( "sound" );
		TiXmlElement* child_sound = NEW TiXmlElement("sound");
		root->LinkEndChild(child_sound);
		if ( child_sound )
		{
			str = (SystemHandle->AppSettings->MUSIC_ENABLED) ? "true" : "false";
			child_sound->SetAttribute("music", str.c_str());

			str = (SystemHandle->AppSettings->SOUND_ENABLED) ? "true" : "false";
			child_sound->SetAttribute("effects", str.c_str());
		}
#endif

#if _NOT_
		/*<minimap>*/TiXmlElement* child_minimap = root->FirstChildElement("minimap");
		if (child_minimap)
		{
			child_minimap->SetAttribute("enabled", (SendMessage(m_MiniMap, BM_GETSTATE, 0, 0) == BST_CHECKED) ? "true" : "false");
		}
		/*<network>*/TiXmlElement* child_network = root->FirstChildElement( "network" );
		if ( child_network )
		{
			child_network->SetAttribute("enabled", (SendMessage(m_Network, BM_GETSTATE, 0, 0) == BST_CHECKED) ? "true" : "false");
		}
#endif
	}
	else
		return false;

	doc.SaveFile(file);

	return true;
}
