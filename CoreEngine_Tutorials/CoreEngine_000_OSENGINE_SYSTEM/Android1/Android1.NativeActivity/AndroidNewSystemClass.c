// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: AndroidNewSystemClass.c
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2024
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
// 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "AndroidNewSystemClass.h"
#include "OSengine.h"
extern bool android_APPLICATION_INIT_SYSTEM();
extern int android_APPLICATION_MAIN_LOOP();

//----------------------------------------------------------------------------
AndroidNewSystemClass::AndroidNewSystemClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567847;
}

AndroidNewSystemClass::~AndroidNewSystemClass() { CLASSDELETE(); }

//----------------------------------------------------------------------------------
AndroidNewSystemClass::AndroidNewSystemClass(WOMA::Settings* appSettings) : SystemClass()
//----------------------------------------------------------------------------------
{
	CLASSLOADER();

	//public:
	SystemHandle = this;
}

bool AndroidNewSystemClass::APPLICATION_INIT_SYSTEM()
{
	bool res = android_APPLICATION_INIT_SYSTEM();
	return res;
}

int AndroidNewSystemClass::APPLICATION_MAIN_LOOP()
{
	int res = android_APPLICATION_MAIN_LOOP();
	return res;
}