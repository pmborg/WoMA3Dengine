// --------------------------------------------------------------------------------------------
// Filename: LinuxSystemClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// PURPOSE:
//
// ********************************************************************************************
#pragma once

#include "platform.h"
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

};

/////////////
// GLOBALS //
/////////////
extern LinuxSystemClass* SystemHandle;

#endif
