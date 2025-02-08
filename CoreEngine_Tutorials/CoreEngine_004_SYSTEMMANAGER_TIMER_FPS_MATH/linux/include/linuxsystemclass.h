// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: LinuxSystemClass.h
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
#pragma once

#ifndef __LINUXSYSTEMCLASS_H__

//#include "platform.h"
#include "SystemClass.h"

#ifdef LINUX_PLATFORM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: LinuxSystemClass
////////////////////////////////////////////////////////////////////////////////
class LinuxSystemClass : public SystemClass
{
public:
	//LinuxSystemClass();				//NEW
	LinuxSystemClass(WOMA::Settings* appSettings);
	~LinuxSystemClass();
	void Shutdown();

	int	 APPLICATION_MAIN_LOOP();

	void ProcessFrame();

	bool APPLICATION_CORE_SYSTEM();
	bool APPLICATION_INIT_SYSTEM(); //bool ApplicationInit();

	void* m_hWnd = NULL;

	int xbutton_x = -1, xbutton_y = -1;
	int xbutton = 0;

private:
	bool mResizing;
#if CORE_ENGINE_LEVEL >= 2
	bool ApplicationInitMainWindow(/*glxOpenGLClass*/ void* OpenGL);
#endif
};

/////////////
// GLOBALS //
/////////////
extern LinuxSystemClass* SystemHandle;

#endif
#endif
