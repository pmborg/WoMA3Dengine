// --------------------------------------------------------------------------------------------
// Filename: xml_loader.h
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
//WomaIntegrityCheck = 1234525256;

#pragma once
#pragma warning( disable : 5208 ) // warning C5208: unnamed class used in typedef name cannot declare members other than non-static data members, member enumerations, or member classes

// Convert from tyni v1 to v2: https://phabricator.mitk.org/T27985

#include "platform.h"
#if defined USE_TINYXML_LOADER

#include "tinyxml2.h"

#ifdef TIXML_USE_STL
#include <iostream>
#include <sstream>
using namespace std;
#else
#include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
	#include <crtdbg.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#endif
	

#if UNICODE
#ifdef X64
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "x64/WDebug/TinyXMLv2_LIBX64_d.lib" )	//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/WRelease/TinyXMLv2_LIBX64.lib" )	//RELEASE
	#else
		#pragma comment( lib, "x64/WRelease/TinyXMLv2_LIBX64.lib" )	//DBGREL
	#endif
#else
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "Win32/WDebug/TinyXMLv2_LIB_d.lib" )	//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/WRelease/TinyXMLv2_LIB.lib" )	//RELEASE
	#else
		#pragma comment( lib, "Win32/WRelease/TinyXMLv2_LIB.lib" )	//DBGREL
	#endif
#endif
#else
#ifdef X64
	#if defined(_DEBUG)// && !defined(NDEBUG)
		#pragma comment( lib, "x64/Debug/TinyXMLv2_LIBX64_d.lib" )	//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/Release/TinyXMLv2_LIBX64.lib" )	//RELEASE
	#else
		#pragma comment( lib, "x64/Release/TinyXMLv2_LIBX64.lib" )	//DBGREL
	#endif
#else
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "Win32/Debug/TinyXMLv2_LIB_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/Release/TinyXMLv2_LIB.lib" )		//RELEASE
	#else
		#pragma comment( lib, "Win32/Release/TinyXMLv2_LIB.lib" )		//DBGREL
	#endif
#endif
#endif


#include "main.h"
#include <vector>

#include "womadriverclass.h"


// -------------------------------------------------------------------------------------------
//<generalsettings>
typedef struct {

	// Screen Settings:
	// --------------------------------------------------------------------------------------------
	char uiMonitor[10], screenFullScreen[10], screenFullScreenWindowed[10], allowResize[10], posX[10], posY[10], screenWidth[10], screenHeight[10], bitsPerPixel[10], screenNear[10], screenDepth[10];

	// Driver Settings:
	// --------------------------------------------------------------------------------------------
	char driverName[10], UseAllMonitors[10], useDoubleBuffering[10], vsync[10];// , msaa[10];

	// Map Settings:
	// --------------------------------------------------------------------------------------------
	#if TUTORIAL_PRE_CHAP >= 15
	char minimapEnabled[10];
	#endif

		char initPosX[10], initPosY[10], initPosZ[10];
		char initRotX[10], initRotY[10], initRotZ[10];

		char MaxTextureSize[10], Bilinear[10], Trilinear[10], Anisotropic[10], AnisotropicLevel[10];

		char fogStart[10], fogEnd[10];

	// Sound Settings:
	// --------------------------------------------------------------------------------------------
		char musicEnabled[10];
		char soundEffectsEnabled[10];

	// Player Settings:
	// --------------------------------------------------------------------------------------------
	#if TUTORIAL_PRE_CHAP >= 60 //80
		char playerName[16], faction[10], meshType[16];
	#endif

	// Network Settings:
	// --------------------------------------------------------------------------------------------

} generalsettings;


// -------------------------------------------------------------------------------------------
typedef struct {

	// World Settings:
	// --------------------------------------------------------------------------------------------
	char hVisibility[10], seaLevel[10], size[10], patchSize[10], skySize[10],
		clearColorR[10], clearColorG[10], clearColorB[10],
		mainTexture[MAX_STR_LEN], water[MAX_STR_LEN], waterTexture[MAX_STR_LEN],
		skyDayTexture[MAX_STR_LEN], skyNightTexture[MAX_STR_LEN];
} worldsettings;

struct WOMA_OBJECT
{
	WOMA_OBJECT() {}
	WOMA_OBJECT(SHADER_TYPE shader,
		BOOL castShadows_, BOOL renderShadows_, BOOL modelHASlight_, UINT instances_)
	{
		shaderType = shader;
		castShadows = castShadows_; 
		renderShadows = renderShadows_; 
		modelHASlight = modelHASlight_;
		instances = instances_;
	}

	SHADER_TYPE shaderType= SHADER_AUTO;
	BOOL		castShadows=false;
	BOOL		renderShadows = false;
	BOOL		modelHASlight = false;
	UINT		instances=0;
};

//<world>
typedef struct xmlobj3d_
{
	int id = 0;					//40
	int fromPage, toPage, depend = 0;
	float scale = 1, posX = 0, posZ = 0, translateY = 0;
	float rotX = 0, rotY = 0, rotZ = 0;
	int shader = 0;
	char filename[256];
	WOMA_OBJECT WOMA_object;
	int instances = 0;			//40
    bool render = true;
    float ry=0; //DX_ENGINE_LEVEL >= 88
} xmlobj3d;

class XMLloader
{
public:
	XMLloader();
	~XMLloader();

	// --------------------------------------------------------------------------------------------
	bool saveXMLsettingsFile(char* file); //Note: Have to be char
	bool loadXMLsettingsFile(TCHAR* file); //Note: Have to be char
	bool initAppicationSettings(TCHAR* filename); //Note: Have to be char

	bool loadWorld(TCHAR* file);
	bool InitWorldLoader(TCHAR* filename);				//Note: Have to be char
	xmlobj3d object3d;
	std::vector<xmlobj3d> theWorld;

	// --------------------------------------------------------------------------------------------
	// Globals:
	// --------------------------------------------------------------------------------------------
	generalsettings GenSettings;

	/*TiXmlElement*/ tinyxml2::XMLDocument* child_screen = NULL;
	/*TiXmlElement*/ tinyxml2::XMLElement* child_world = NULL;
	/*TiXmlElement*/ tinyxml2::XMLNode* child_object = NULL;

	worldsettings worldSettings;
};

#endif
