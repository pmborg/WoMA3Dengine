// -------------------------------------------------------------------------------------------------------------------------------------
// Filename: AndroidNewSystemClass.h
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
//WomaIntegrityCheck = 1234567142;

#pragma once

#include "SystemClass.h"

class AndroidNewSystemClass : public SystemClass
{
public:
	AndroidNewSystemClass();
	~AndroidNewSystemClass();
	AndroidNewSystemClass(WOMA::Settings* AppSettings);

	bool APPLICATION_CORE_SYSTEM();
	bool APPLICATION_INIT_SYSTEM();
	int APPLICATION_MAIN_LOOP();
};

//#if defined WINDOWS_PLATFORM && !defined ANDROID_PLATFORM
//&& !defined NewWomaEngine

