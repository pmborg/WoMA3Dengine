// --------------------------------------------------------------------------------------------
// Filename: xml_loader.h
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
//WomaIntegrityCheck = 1234525217;

#pragma once
#pragma warning( disable : 5208 ) // warning C5208: unnamed class used in typedef name cannot declare members other than non-static data members, member enumerations, or member classes

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
	




#include "main.h"
#include <vector>

// -------------------------------------------------------------------------------------------
//<generalsettings>
typedef struct {

	// Screen Settings:
	// --------------------------------------------------------------------------------------------
	char adapter[10], uiMonitor[10], screenFullScreen[10], screenFullScreenWindowed[10], allowResize[10], posX[10], posY[10], screenWidth[10], screenHeight[10], bitsPerPixel[10], screenNear[10], screenDepth[10];

	// Driver Settings:
	// --------------------------------------------------------------------------------------------
	char driverName[10], UseAllMonitors[10], useDoubleBuffering[10], useTripleBuffering[10], vsync[10];

	// Map Settings:
	// --------------------------------------------------------------------------------------------
	#if TUTORIAL_PRE_CHAP >= 15
	char minimapEnabled[10];
	#endif

		char initPosX[10], initPosY[10], initPosZ[10];
		char initRotX[10], initRotY[10], initRotZ[10];

	#if defined INTRO_DEMO || DX_ENGINE_LEVEL >= 22 // Texturing
		char MaxTextureSize[10], Bilinear[10], Trilinear[10], Anisotropic[10], AnisotropicLevel[10];
	#endif

	// Sound Settings:
	// --------------------------------------------------------------------------------------------

	// Player Settings:
	// --------------------------------------------------------------------------------------------
	#if TUTORIAL_PRE_CHAP >= 60 //80
		char playerName[16], faction[10], meshType[16];
	#endif

	// Network Settings:
	// --------------------------------------------------------------------------------------------

} generalsettings;

typedef struct {
	int			hVisibility;
	int			seaLevel;
	int			size;
	int			patchSize;
	int			skySize;
	float		clearColorR, clearColorG, clearColorB;
	std::string	water;				//Note: have to be char! not STRING, due multi OS support.
	std::string	waterTexture;		//Note: have to be char! not STRING, due multi OS support.
	std::string	mainTexture;		//Note: have to be char! not STRING, due multi OS support.
	std::string	skyDayTexture;		//Note: have to be char! not STRING, due multi OS support.
	std::string	skyNightTexture;	//Note: have to be char! not STRING, due multi OS support.
} World;

#if defined USE_TINYXML_LOADER
extern World		world_map;
#endif

int get_world_hVisibility(int level = -1);
int get_world_seaLevel(int level = -1);
int get_world_size(int level = -1); //world.size
int get_world_patchSize(int level = -1);
int get_world_skySize(int level = -1);

float get_world_clearColorR(int level = -1);	//SystemHandle->world.clearColorR;
float get_world_clearColorG(int level = -1);	//SystemHandle->world.clearColorG;
float get_world_clearColorB(int level = -1);	//SystemHandle->world.clearColorB;

std::string get_world_water(int level = -1);
std::string get_world_waterTexture(int level = -1);	//get_world_waterTexture().c_str()
std::string get_world_mainTexture(int level = -1);		//SystemHandle->world.mainTexture
std::string get_world_skyDayTexture(int level = -1);	//SystemHandle->world.skyDayTexture
std::string get_world_skyNightTexture(int level = -1);	//SystemHandle->world.skyNightTexture

void set_world_hVisibility(int var, int level = -1);
void set_world_seaLevel(int var, int level = -1);
void set_world_size(int var, int level = -1);			//world.size	DXsystemHandle->world.size
void set_world_patchSize(int var, int level = -1);		//world.patchSize
void set_world_skySize(int var, int level = -1);

void set_world_clearColorR(float var, int level = -1);
void set_world_clearColorG(float var, int level = -1);
void set_world_clearColorB(float var, int level = -1);

void set_world_water(std::string var, int level = -1);
void set_world_waterTexture(std::string var, int level = -1);
void set_world_mainTexture(std::string var, int level = -1);		//
void set_world_skyDayTexture(std::string var, int level = -1);	//SystemHandle->world.skyDayTexture
void set_world_skyNightTexture(std::string var, int level = -1);	//SystemHandle->world.skyNightTexture

extern int get_world_hVisibility(int level);
extern int get_world_seaLevel(int level);
extern int get_world_size(int level); //world.size
extern int get_world_patchSize(int level);
extern int get_world_skySize(int level);

extern float get_world_clearColorR(int level);	//SystemHandle->world.clearColorR;
extern float get_world_clearColorG(int level);	//SystemHandle->world.clearColorG;
extern float get_world_clearColorB(int level);	//SystemHandle->world.clearColorB;

extern std::string get_world_water(int level);
extern std::string get_world_waterTexture(int level);	//get_world_waterTexture().c_str()
extern std::string get_world_mainTexture(int level);		//SystemHandle->world.mainTexture
extern std::string get_world_skyDayTexture(int level);	//SystemHandle->world.skyDayTexture
extern std::string get_world_skyNightTexture(int level);	//SystemHandle->world.skyNightTexture

extern void set_world_hVisibility(int var, int level);
extern void set_world_seaLevel(int var, int level);
extern void set_world_size(int var, int level);			//world.size	DXsystemHandle->world.size
extern void set_world_patchSize(int var, int level);		//world.patchSize
extern void set_world_skySize(int var, int level);

extern void set_world_clearColorR(float var, int level);
extern void set_world_clearColorG(float var, int level);
extern void set_world_clearColorB(float var, int level);

extern void set_world_water(std::string var, int level);
extern void set_world_waterTexture(std::string var, int level);
extern void set_world_mainTexture(std::string var, int level);		//
extern void set_world_skyDayTexture(std::string var, int level);	//SystemHandle->world.skyDayTexture
extern void set_world_skyNightTexture(std::string var, int level);	//SystemHandle->world.skyNightTexture

// -------------------------------------------------------------------------------------------

class XMLloader
{
public:
	XMLloader();
	~XMLloader();

	// --------------------------------------------------------------------------------------------
	bool saveXMLsettingsFile(char* file); //Note: Have to be char
	bool loadXMLsettingsFile(TCHAR* file); //Note: Have to be char
	bool initAppicationSettings(TCHAR* filename); //Note: Have to be char

	bool loadWorld(UINT level, TCHAR* file, void* outObjects = NULL);
	bool InitWorldLoader(UINT level, TCHAR* filename);

	// --------------------------------------------------------------------------------------------
	// GLOBALS:
	// --------------------------------------------------------------------------------------------
	generalsettings GenSettings;			//1



	/*TiXmlElement*/ tinyxml2::XMLDocument* child_screen = NULL;
	/*TiXmlElement*/ tinyxml2::XMLElement* child_world = NULL;
	/*TiXmlElement*/ tinyxml2::XMLNode* child_object = NULL;
};

#endif
