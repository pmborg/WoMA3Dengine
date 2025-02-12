// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: SystemClass.cpp
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
// PURPOSE: Define APIs for systemclass.cpp which is the common OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4477 )
#pragma warning( disable : 4838 )

#include "SystemClass.h"
#include "OSengine.h" //#include "WinSystemClass.h"
#include "default_settings_xml.h"
#include "woma_macros.h"

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SystemClass::SystemClass() // Make sure that all pointers in shutdown are here:
{
	// STARTING POINT of WOMA ENGINE!
	CLASSLOADER();
	WomaIntegrityCheck = 1234567311;

	AppSettings = NULL;

	// Reset Vars:
	// -------------------------------------------------------------------------------------------


#if defined USE_TIMER_CLASS
	fps = NULL;
	cpu = NULL;
#endif

}



SystemClass::~SystemClass() { CLASSDELETE(); }

void SystemClass::Shutdown()
{

	AppSettings = NULL;				// Pointer to Static object, no need to free.
}
#if defined ANDROID_PLATFORM
extern android_app* app;
#endif

void SystemClass::FrameUpdate()
{

#if defined LINUX_PLATFORM
if (WOMA::game_state == GAME_RUN)
{
	/*
	int err = XGrabPointer(Win.display, Win.window,
							True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
							GrabModeAsync, GrabModeAsync,
							Win.window, None, CurrentTime);

	processXEvents(wm_protocols, wm_delete_window);
	XUngrabPointer(Win.display, CurrentTime);

	#define mousex event.xbutton.x_root 
	#define mousey event.xbutton.y_root
	//if (event.xbutton.button == Button1)
	//	_tprintf("mousex: %d mouseY: %d\n", (mousex) - WOMA::settings.WINDOW_Xpos, (mousey) -WOMA::settings.WINDOW_Ypos);
	
	if ((mousex < 100 && mousey < 100) && (mousex > 0 && mousey > 0))
	{
		RENDER_PAGE = 25;
		WOMA::previous_game_state = GAME_IMGUI;
		WOMA::game_state = ENGINE_RESTART;
		return;
	}
	*/
}
#endif

#if defined ANDROID_PLATFORM && !defined NewWomaEngine
	if (WOMA::game_state == GAME_RUN)
	{
		struct womaengine* engine = (struct womaengine*)app->userData;

		#define mousex engine->state.x
		#define mousey engine->state.y
		//_tprintf("mousex: %d mouseY: %d\n", mousex, mousey);
		if ((mousex < 100 && mousey < 100) && (mousex > 0 && mousey > 0))
		{
			RENDER_PAGE = 25;
			WOMA::previous_game_state = GAME_IMGUI;
			WOMA::game_state = ENGINE_RESTART;
			return;
		}
	}
#endif

}

