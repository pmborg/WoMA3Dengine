// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: xml_settings_saver.cpp
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
//  - Debug use local: "file".xml
//  - Release use:     C:\Users\<user>\AppData\Local\Pmborg\Woma2014\"file".xml (WOMA::APPDATA)
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;

#pragma warning(disable: 4244)

#include "OSengine.h"
#if defined CLIENT_SCENE_SETUP //#if CORE_ENGINE_LEVEL > 9
#include "xml_loader.h"

//*********************************************************************************************/
bool XMLloader::saveConfigSettings (char* file) // Note: Have to be char
//*********************************************************************************************/
{
	// TUTORIALv1: https://www.cs.cmu.edu/~preethi/src/tinyxml/docs/tutorial0.html

#if defined WINDOWS_PLATFORM && defined USE_TINYXML_LOADER
	/*TiXmlElement*/ tinyxml2::XMLDocument doc;

	//Optional:
	//TiXmlDeclaration* decl = NEW TiXmlDeclaration("1.0", "", "");
	//doc.LinkEndChild(decl);

	std::string str = { 0 };
	CHAR stri[MAX_STR_LEN]  = { 0 };

	// SAVE Settings:
	auto* root = doc.NewElement( "generalsettings" );
	doc.LinkEndChild(root);
	if ( root )
	{
		//TiXmlElement* child_screen = root->FirstChildElement( "screen" );
		tinyxml2::XMLElement* child_screen = root->InsertNewChildElement("screen");
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

		///*<texture>*/tinyxml2::XMLElement* child_texture = root->FirstChildElement("texture");
		tinyxml2::XMLElement* child_texture = root->InsertNewChildElement("texture");
		root->LinkEndChild(child_texture);
		if (child_texture)
		{
			_itoa(SystemHandle->AppSettings->MaxTextureSize, stri, 10);
			child_texture->SetAttribute("maxTexture", stri);

			str = (SystemHandle->AppSettings->MSAA_bilinear) ? "true" : "false";
			child_texture->SetAttribute("bilinear", str.c_str());

			str = (SystemHandle->AppSettings->MSAA_trilinear) ? "true" : "false";
			child_texture->SetAttribute("trilinear", str.c_str());

			str = (SystemHandle->AppSettings->MSAA_Anisotropic) ? "true" : "false";
			child_texture->SetAttribute("Anisotropic", str.c_str());

			_itoa(SystemHandle->AppSettings->MSAA_AnisotropicLevel, stri, 10);
			child_texture->SetAttribute("AnisotropicLevel", stri);
		}

#if defined USE_WIN32_SOUND_MANAGER || defined USE_WIN32_PLAY_MUSIC
		///*<sound>*/tinyxml2::XMLElement* child_sound = root->FirstChildElement( "sound" );
		tinyxml2::XMLElement* child_sound = root->InsertNewChildElement("sound");
		root->LinkEndChild(child_sound);
		if ( child_sound )
		{
			str = (SystemHandle->AppSettings->MUSIC_ENABLED) ? "true" : "false";
			child_sound->SetAttribute("music", str.c_str());

			str = (SystemHandle->AppSettings->SOUND_ENABLED) ? "true" : "false";
			child_sound->SetAttribute("effects", str.c_str());
		}
#endif

	}
	else
		return false;

	doc.SaveFile(file);
#endif

	return true;
}
#endif
