// --------------------------------------------------------------------------------------------
// Filename: Systemclass.cpp
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
// Downloaded from : http://woma.servegame.com
// --------------------------------------------------------------------------------------------
//
// PURPOSE: Define APIs for systemclass.cpp which is the common OS API
//
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "SystemClass.h"
#include "winSystemClass.h"
#include "default_settings_xml.h"
#include "OSengine.h"
#include "woma_macros.h"


SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();

	AppSettings = NULL;

}


SystemClass::~SystemClass() { CLASSDELETE(); }

void SystemClass::Shutdown()
{

	AppSettings = NULL;				// Pointer to Static object, no need to free.
}

