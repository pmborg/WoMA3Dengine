// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: xml_loader.h
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
//WomaIntegrityCheck = 1234567831;

#pragma once
#pragma warning( disable : 5208 ) // warning C5208: unnamed class used in typedef name cannot declare members other than non-static data members, member enumerations, or member classes

#define _CRT_SECURE_NO_WARNINGS
#include "platform.h"

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

#include "tinyxml.h"

#if UNICODE
#ifdef X64
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "x64/WDebug/TinyXML_LIBX64_d.lib" )	//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/WRelease/TinyXML_LIBX64.lib" )	//RELEASE
	#else
		#pragma comment( lib, "x64/WRelease/TinyXML_LIBX64.lib" )	//DBGREL
	#endif
#else
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "Win32/WDebug/TinyXML_LIB_d.lib" )	//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/WRelease/TinyXML_LIB.lib" )	//RELEASE
	#else
		#pragma comment( lib, "Win32/WRelease/TinyXML_LIB.lib" )	//DBGREL
	#endif
#endif
#else
#ifdef X64
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "x64/Debug/TinyXML_LIBX64_d.lib" )	//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "x64/Release/TinyXML_LIBX64.lib" )	//RELEASE
	#else
		#pragma comment( lib, "x64/Release/TinyXML_LIBX64.lib" )	//DBGREL
	#endif
#else
	#if defined(_DEBUG) & !defined(NDEBUG)
		#pragma comment( lib, "Win32/Debug/TinyXML_LIB_d.lib" )		//DEBUG
	#elif !defined _DEBUG && defined NDEBUG
		#pragma comment( lib, "Win32/Release/TinyXML_LIB.lib" )		//RELEASE
	#else
		#pragma comment( lib, "Win32/Release/TinyXML_LIB.lib" )		//DBGREL
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
	char uiMonitor[10], screenFullScreen[10], allowResize[10], posX[10], posY[10], screenWidth[10], screenHeight[10], bitsPerPixel[10], screenNear[10], screenDepth[10];

	// Driver Settings:
	// --------------------------------------------------------------------------------------------
	char driverName[10], UseAllMonitors[10], useDoubleBuffering[10], vsync[10], msaa[10];

	// Map Settings:
	// --------------------------------------------------------------------------------------------

		char initPosX[10], initPosY[10], initPosZ[10];
		char initRotX[10], initRotY[10], initRotZ[10];

		char MaxTextureSize[10], Bilinear[10], Trilinear[10], Anisotropic[10], AnisotropicLevel[10];

	// Sound Settings:
	// --------------------------------------------------------------------------------------------
	#if defined USE_PLAY_MUSIC
		char musicEnabled[10];
	#endif
	#if DX_ENGINE_LEVEL >= 29 && defined USE_SOUND_MANAGER
		char soundEffectsEnabled[10];
	#endif

	// Player Settings:
	// --------------------------------------------------------------------------------------------

	// Network Settings:
	// --------------------------------------------------------------------------------------------

} generalsettings;


// -------------------------------------------------------------------------------------------
typedef struct {

	// World Settings:
	// --------------------------------------------------------------------------------------------
    char hVisibility[10], seaLevel[10], size[10], patchSize[10], skyDayTexture[MAX_STR_LEN], skyNightTexture[MAX_STR_LEN];

} worldsettings;

struct WOMA_OBJECT
{
	WOMA_OBJECT() {}
	WOMA_OBJECT(SHADER_TYPE shader,
		BOOL castShadows_, BOOL renderShadows_, BOOL modelHASlight_, UINT instances_)
	{
		shaderType = shader;
		castShadows = castShadows_; renderShadows = renderShadows_; modelHASlight = modelHASlight_;
		instances = instances_;
	}

	SHADER_TYPE shaderType;
	BOOL		castShadows;
	BOOL		renderShadows;
	BOOL		modelHASlight;
	UINT		instances;
};

//<world>
typedef struct {
	int fromPage, toPage, depend = 0;
	float scale = 1, posX = 0, posZ = 0, translateY = 0;
	float rotX = 0, rotY = 0, rotZ = 0;
	int shader = 0;
	char filename[256];
	WOMA_OBJECT WOMA_object;
} obj3d;

class XMLloader
{
public:
	XMLloader();
	~XMLloader();

	// --------------------------------------------------------------------------------------------
	bool saveConfigSettings(char* file); //Note: Have to be char
	bool loadConfigSettings(TCHAR* file); //Note: Have to be char
	bool initAppicationSettings(TCHAR* filename); //Note: Have to be char

	bool loadWorld(TCHAR* file);
	bool InitWorldLoader(TCHAR* filename);				//Note: Have to be char

	obj3d object3d;
	std::vector<obj3d> theWorld;

	// --------------------------------------------------------------------------------------------
	// Globals:
	// --------------------------------------------------------------------------------------------
	generalsettings GenSettings;

	TiXmlElement* child_screen = NULL;
	TiXmlElement* child_world = NULL;
	TiXmlElement* child_object = NULL;


	worldsettings worldSettings;
};

