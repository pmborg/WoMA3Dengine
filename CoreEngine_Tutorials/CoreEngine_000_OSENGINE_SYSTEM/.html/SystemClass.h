// --------------------------------------------------------------------------------------------
// Filename: systemclass.h
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
// PURPOSE: Export APIs for systemclass.cpp which is the common OS API
//
// --------------------------------------------------------------------------------------------
#pragma once

#include "main.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	~SystemClass();

	void Shutdown();

	WOMA::Settings* AppSettings;

public:

};


