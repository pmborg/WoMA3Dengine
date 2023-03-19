// --------------------------------------------------------------------------------------------
// Filename: win32InputClass.h
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
// PURPOSE: A basic input used in first WOMA LEVELs using OS functions.
// --------------------------------------------------------------------------------------------#pragma once
#pragma once

#include "platform.h"
#include "standard_platform.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	~InputClass();

	void Initialize();

	void KeyDown(UINT);
	void KeyUp(UINT);

	bool IsKeyDown(UINT);

	bool m_keys[256];
};
