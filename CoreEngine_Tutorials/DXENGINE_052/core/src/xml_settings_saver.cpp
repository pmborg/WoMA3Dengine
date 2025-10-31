// --------------------------------------------------------------------------------------------
// Filename: xml_settings_saver.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
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
//WomaIntegrityCheck = 1234525217;

#pragma warning(disable: 4244)

#include "OSengine.h"
#if defined CLIENT_SCENE_SETUP //#if CORE_ENGINE_LEVEL > 9
#include "xml_loader.h"

//*********************************************************************************************/
bool XMLloader::saveXMLsettingsFile (char* file) // Note: Have to be char
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
			_itoa(WOMA::AppSettings->ADAPTOR, stri, 10);
			child_screen->SetAttribute("adapter", stri);

			_itoa(WOMA::AppSettings->UI_MONITOR, stri, 10);
			child_screen->SetAttribute("uiMonitor", stri);

			str = (WOMA::AppSettings->FULL_SCREEN) ? "true" : "false";
			child_screen->SetAttribute("fullScreen", str.c_str());

            str = (WOMA::AppSettings->FULLSCREEN_ON_WINDOWED) ? "true" : "false";
            child_screen->SetAttribute("fullScreenWindowed", str.c_str());

			_itoa(WOMA::AppSettings->WINDOW_Xpos_ori, stri, 10);
			child_screen->SetAttribute("posX", stri);

			_itoa(WOMA::AppSettings->WINDOW_Ypos_ori, stri, 10);
			child_screen->SetAttribute("posY", stri);

			_itoa(WOMA::AppSettings->WINDOW_WIDTH_ori, stri, 10);
			child_screen->SetAttribute("width", stri);

			_itoa(WOMA::AppSettings->WINDOW_HEIGHT_ori, stri, 10);
			child_screen->SetAttribute("height", stri);
				 
			str = (WOMA::AppSettings->AllowResize) ? "true" : "false";
			child_screen->SetAttribute("allowResize", str.c_str());

			str = (WOMA::AppSettings->VSYNC_ENABLED) ? "true" : "false";
			child_screen->SetAttribute("vsync", str.c_str());

			_itoa(WOMA::AppSettings->BITSPERPEL, stri, 10);
			child_screen->SetAttribute("bitsPerPixel", stri);
		}

		///*<driver>*/tinyxml2::XMLElement* child_driver = root->FirstChildElement( "driver" );
		//tinyxml2::XMLElement* child_driver = NEW tinyxml2::XMLElement("driver");
		tinyxml2::XMLElement* child_driver = root->InsertNewChildElement("driver");

		root->LinkEndChild(child_driver);
		if (child_driver)
		{
			/*
				DRIVER_DX11 = 0,
				DRIVER_GL3,
				DRIVER_DX9,
				DRIVER_DX12
			*/
			if (WOMA::AppSettings->DRIVER == DRIVER_DX11)
				child_driver->SetAttribute("name", "DX11");
			if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
				child_driver->SetAttribute("name", "GL3+");
			if (WOMA::AppSettings->DRIVER == DRIVER_DX9)
				child_driver->SetAttribute("name", "DX9");
			if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
				child_driver->SetAttribute("name", "DX12");

			str = (WOMA::AppSettings->UseAllMonitors) ? "true" : "false";
			child_driver->SetAttribute("UseAllMonitors", str.c_str());

			str = (WOMA::AppSettings->UseDoubleBuffering) ? "true" : "false";
			child_driver->SetAttribute("useDoubleBuffering", str.c_str());

			str = (WOMA::AppSettings->UseTripleBuffering) ? "true" : "false";
			child_driver->SetAttribute("useTripleBuffering", str.c_str());
		}

		///*<camera>*/tinyxml2::XMLElement* child_camera = root->FirstChildElement("camera");
		//tinyxml2::XMLElement* child_camera = NEW tinyxml2::XMLElement("camera");
		tinyxml2::XMLElement* child_camera = root->InsertNewChildElement("camera");
		root->LinkEndChild(child_camera);
		if (child_camera)
		{
			_itoa(WOMA::AppSettings->SCREEN_NEAR, stri, 10);
			if (stri[0] == '0')
				child_camera->SetAttribute("screenNear", "0.1");
			else
				child_camera->SetAttribute("screenNear", stri);

			_itoa(WOMA::AppSettings->SCREEN_DEPTH, stri, 10);
			child_camera->SetAttribute("screenDepth", stri);

			///*<initPos>*/tinyxml2::XMLElement* child_initPos = child_camera->FirstChildElement("initPos");
			//tinyxml2::XMLElement* child_initPos = NEW tinyxml2::XMLElement("initPos");
			tinyxml2::XMLElement* child_initPos = root->InsertNewChildElement("initPos");
			child_camera->LinkEndChild(child_initPos);
			if (child_initPos)
			{
				_itoa(WOMA::AppSettings->INIT_CAMX, stri, 10);
				child_initPos->SetAttribute("x", stri);

				_itoa(WOMA::AppSettings->INIT_CAMY, stri, 10);
				child_initPos->SetAttribute("y", stri);

				_itoa(WOMA::AppSettings->INIT_CAMZ, stri, 10);
				child_initPos->SetAttribute("z", stri);
			}

			///*<initRot>*/tinyxml2::XMLElement* child_initRot = child_camera->FirstChildElement("initRot");
			tinyxml2::XMLElement* child_initRot = root->InsertNewChildElement("initRot");
			child_camera->LinkEndChild(child_initRot);
			if (child_initRot)
			{
				_itoa(WOMA::AppSettings->INIT_ROTX, stri, 10);
				child_initRot->SetAttribute("x", stri);

				_itoa(WOMA::AppSettings->INIT_ROTY, stri, 10);
				child_initRot->SetAttribute("y", stri);

				_itoa(WOMA::AppSettings->INIT_ROTZ, stri, 10);
				child_initRot->SetAttribute("z", stri);
			}
		}

		///*<texture>*/tinyxml2::XMLElement* child_texture = root->FirstChildElement("texture");
		tinyxml2::XMLElement* child_texture = root->InsertNewChildElement("texture");
		root->LinkEndChild(child_texture);
		if (child_texture)
		{
			_itoa(WOMA::AppSettings->MaxTextureSize, stri, 10);
			child_texture->SetAttribute("maxTexture", stri);

			str = (WOMA::AppSettings->MSAA_bilinear) ? "true" : "false";
			child_texture->SetAttribute("bilinear", str.c_str());

			str = (WOMA::AppSettings->MSAA_trilinear) ? "true" : "false";
			child_texture->SetAttribute("trilinear", str.c_str());

			str = (WOMA::AppSettings->MSAA_Anisotropic) ? "true" : "false";
			child_texture->SetAttribute("Anisotropic", str.c_str());

			_itoa(WOMA::AppSettings->MSAA_AnisotropicLevel, stri, 10);
			child_texture->SetAttribute("AnisotropicLevel", stri);
		}

		///*<sound>*/tinyxml2::XMLElement* child_sound = root->FirstChildElement( "sound" );
		tinyxml2::XMLElement* child_sound = root->InsertNewChildElement("sound");
		root->LinkEndChild(child_sound);
		if ( child_sound )
		{
			str = (WOMA::AppSettings->MUSIC_ENABLED) ? "true" : "false";
			child_sound->SetAttribute("music", str.c_str());

			str = (WOMA::AppSettings->SOUND_ENABLED) ? "true" : "false";
			child_sound->SetAttribute("effects", str.c_str());
		}

	}
	else
		return false;

	doc.SaveFile(file);
#endif

	return true;
}
#endif
