// --------------------------------------------------------------------------------------------
// Filename: winSystemclass.cpp
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
// PURPOSE: Define APIs for winSystemClass.cpp which is the WINDOWS OS API
//
// --------------------------------------------------------------------------------------------

#include "OSengine.h"

//#include "winSystemClass.h" //#include "main.h" //LPCSTR

/////////////////////
// Windows GLOBALS //
/////////////////////


//----------------------------------------------------------------------------------
WinSystemClass::WinSystemClass() : SystemClass() 
//----------------------------------------------------------------------------------
{
	CLASSLOADER();
	//public:
	SystemHandle = this;
	WinSystemClass_init();
}

//----------------------------------------------------------------------------------
WinSystemClass::WinSystemClass(WOMA::Settings* appSettings): SystemClass() //	SystemClass::SystemClass() Will Run!
//----------------------------------------------------------------------------------
{
	CLASSLOADER();

	//public:
	AppSettings		= appSettings; // Super
	SystemHandle	= this;
	WinSystemClass_init();
}

void WinSystemClass::WinSystemClass_init()
{
// --------------------------------------------------------------
// Init Vars:
// --------------------------------------------------------------

}

WinSystemClass::~WinSystemClass()
{

	Shutdown();
	CLASSDELETE();
	SystemHandle = NULL;
}


//extern int Res;
bool WinSystemClass::InitializeSystem()
//----------------------------------------------------------------------------
{
	//  NOTE: Constructors run, First!
	//  SystemClass::SystemClass()				Run: 1st - OS common: low level
	//	ApplicationClass::ApplicationClass()	Run: 2th - User: level
	//	WinSystemClass::WinSystemClass()		Run: 3nd - This OS: hi-level (Check for another instance)

	MessageBox(NULL, WOMA::strConsoleTitle, TEXT("WOMA Tutorial 000: Hello World!"), MB_OK);
	Publish_Quit_Message();

	return true;				// GREEN LIGHT: to Start Rendering! :)
}


//----------------------------------------------------------------------------
int WinSystemClass::ApplicationMainLoop()		// [RUN] - MAIN "INFINITE" LOOP!
//----------------------------------------------------------------------------
{
	MSG msg = { 0 };						// Reset msg

	//DEBUG:
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// There is any OS messages to handle?
		{
			TranslateMessage(&msg); // TranslateMessage produces WM_CHAR messages only for keys that are mapped to ASCII characters by the keyboard driver.
			DispatchMessage(&msg);  // Process Msg:  (INVOKE: WinSystemClass::MessageHandler)
		}
	} while (msg.message != WM_QUIT);

	return S_OK;
}

void WinSystemClass::Shutdown()
//----------------------------------------------------------------------------------
{
	// WinSystemClass Shutdown:
	WOMA_LOGManager_DebugMSGAUTO ((TCHAR*)TEXT("WinSystemClass::Shutdown()\n"));

	// Destroy Drivers:
	SystemClass::Shutdown();

	
	ShutdownWindows();				// Shutdown the Main Window.
}


void WinSystemClass::ShutdownWindows()
//----------------------------------------------------------------------------
{

}

namespace WOMA
{
	extern TCHAR* getCurrentDir();
}



