// --------------------------------------------------------------------------------------------
// Filename: DXinputClass.h
// ----------------------------------------------------------------------------------------------
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
// PURPOSE: Used to Manage: the Mouse and the Joystick
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;
#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//////////////
// INCLUDES //
//////////////
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "PlayerClass.h"

/* Mouse constants */
#define MOUSE_LEFT   0
#define MOUSE_RIGHT  1
#define MOUSE_MIDDLE 2

#define MOUSE_DOWN(button) (m_mouseState.rgbButtons[button] & 0x80)

#if defined SCENE_TERRAIN_COLLISION
#if DX_ENGINE_LEVEL == 49
#define mainTerrain SystemHandle->m_Application->loadedTerrain[0]
#define TERRAIN_ID 0
#elif !defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX
#define mainTerrain SystemHandle->m_Application->loadedTerrain[2]
#define TERRAIN_ID 2
#else
#define mainTerrain SystemHandle->m_Application->loadedTerrain[3]
#define TERRAIN_ID 3
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class DXInputClass
{
public:
	UINT WomaIntegrityCheck = 1234567155;
	DXInputClass();
	~DXInputClass();

	bool Initialize(HINSTANCE);
	void Shutdown();

	bool Frame();
	void ProcessInput();

	// Mouse
	void mouseWheelIn(WPARAM wParam);
	bool mouseWheelUp();
	bool mouseWheelDown();

	// Kb:
private: 
	bool AddPC_keyboard();
	bool ReadKeyboard();
	void GetKeyName(int key, char *keyn);

	// Mouse
	bool AddPC_mouse();
	bool ReadMouse();
	void GetMouseLocation(int&, int&);

private:
	IDirectInput8* m_directInput;		//the interfaces to Direct Input
		
public:
	// Used by Camera Movement:
#if DIRECTINPUT_VERSION >= 0x0700
	DIMOUSESTATE2 m_mouseState;			//Variable is used for recording the current state of the mouse device
#else
	DIMOUSESTATE m_mouseState;			//Variable is used for recording the current state of the mouse device
#endif

	float m_mouseX, m_mouseY;
	int mouseWheel;

	// Used to Check if all is ok...
	IDirectInputDevice8* m_keyboard;	//the keyboard device
	IDirectInputDevice8* m_mouse;		//the mouse device

	// Used by OS_KEY_DOWN MACRO
	unsigned char m_keyboardState[256];	//Variables is used for recording the current state of the keyboard:

};