// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXinputClass.cpp
// ----------------------------------------------------------------------------------------------
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
// PURPOSE: Used to Manage: the Mouse and the Joystick
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
//#define DX_INPUT_DEBUG_TEST	//ON/OFF

#include "DXinputClass.h"
#include "OSengine.h"

#include "GLopenGLclass.h"

DXInputClass::DXInputClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

	// NOTE: rgbButtons holds the state of four of the mouse buttons. 
	// 0 is for the left button
	// 1 for the right
	// 2 for the middle.
#if defined OLDWAY
	m_mouseState.lX = 0;
	m_mouseState.lY = 0;
	m_mouseState.lZ = 0;
	for (UINT i=0; i<4 or 8; i++)
		m_mouseState.rgbButtons[i]=0;
#else
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
#endif

	m_mouse = 0;
	m_mouseX = m_mouseY = NULL;
	mouseWheel = 0;
	
	m_keyboard = 0;
	
	ZeroMemory (m_keyboardState, sizeof(m_keyboardState));

	// private:
	m_directInput = 0;
}

DXInputClass::~DXInputClass() {Shutdown(); CLASSDELETE();}

//The Shutdown function releases the two devices and the interface to Direct Input. 
//Notice that the devices are always un-acquired first and then released.
void DXInputClass::Shutdown()
{
	//private:

	// Release the mouse.
	if(m_mouse)
		m_mouse->Unacquire();
	SAFE_RELEASE (m_mouse);

	// Release the keyboard.
	if(m_keyboard)
		m_keyboard->Unacquire();
	SAFE_RELEASE (m_keyboard);

	// Release the main interface to direct input.
	SAFE_RELEASE (m_directInput);
}

bool DXInputClass::AddPC_keyboard()
{
HRESULT result;

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	IF_FAILED_RETURN_FALSE (result);

	// get capabilities
	DIDEVCAPS caps;
	caps.dwSize = sizeof(DIDEVCAPS);
	m_keyboard->GetCapabilities(&caps);

	WOMA_LOGManager_DebugMSGAUTO (TEXT("Dx KeyBoard Num. Buttons: %d\n"), caps.dwButtons);

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	IF_FAILED_RETURN_FALSE (result);

	//Setting the cooperative level of the keyboard is important in both what it does and how you use the device from that point forward. 
	//In this case we will set it to not share with other programs (DISCL_EXCLUSIVE). 
	//This way if you press a key only your application can see that input and no other application will have access to it. 
	//However if you want other applications to have access to keyboard input while your program is running you can set it to 
	//non-exclusive (DISCL_NONEXCLUSIVE). Now the print screen key works again and other running applications can be controlled by 
	//the keyboard and so forth. Just remember that if it is non-exclusive and you are running in a windowed mode then you will need to 
	//check for when the device loses focus and when it re-gains that focus so it can re-acquire the device for use again. 
	//This focus loss generally happens when other windows become the main focus over your window or your window is minimized.
	
	// Set the cooperative level of the keyboard to not share with other programs.

	if ( SystemHandle->AppSettings->FULL_SCREEN )
		result = m_keyboard->SetCooperativeLevel(SystemHandle->m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	else
		result = m_keyboard->SetCooperativeLevel(SystemHandle->m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	IF_FAILED_RETURN_FALSE (result);

	//Once they keyboard is setup we then call Acquire to finally get access to the keyboard for use from this point forward.
	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	IF_FAILED_RETURN_FALSE (result);

	return true;
}


bool DXInputClass::AddPC_mouse()
{
HRESULT result;

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	IF_FAILED_RETURN_FALSE (result);

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	IF_FAILED_RETURN_FALSE (result);

	// We use non-exclusive cooperative settings for the mouse. We will have to check for when it goes in and out of focus and 
	// re-acquire it each time.

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(SystemHandle->m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// Once the mouse is setup we acquire it so that we can begin using it.
	// Acquire the mouse.
	result = m_mouse->Acquire();
	IF_FAILED_RETURN_FALSE (result);

	return true;
}


bool DXInputClass::Initialize(HINSTANCE hinstance)
{
	HRESULT result;

	if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
	{
		//This function call will initialize the interface to Direct Input. 
		//Once you have a Direct Input object you can initialize other input devices.

		// Initialize the main direct input interface.
		result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
		IF_FAILED_RETURN_FALSE (result);

		AddPC_keyboard();	// KEYBOARD: The first input device we will initialize will be the keyboard.
		AddPC_mouse();		// MOUSE: The next input device we setup is the mouse
	}
	return true;
}

void DXInputClass::GetKeyName(int key, char *keyn)
{	
	switch (key)
	{
		case DIK_ESCAPE:	strcpy(keyn, "ESCAPE");		break;
		case DIK_1:			strcpy(keyn, "1");			break;
		case DIK_2:         strcpy(keyn, "2");			break;
		case DIK_3:         strcpy(keyn, "3");			break;
		case DIK_4:         strcpy(keyn, "4");			break;
		case DIK_5:			strcpy(keyn, "5");			break;
		case DIK_6:         strcpy(keyn, "6");			break;
		case DIK_7:         strcpy(keyn, "7");			break;
		case DIK_8:         strcpy(keyn, "8");			break;
		case DIK_9:			strcpy(keyn, "9");			break;
		case DIK_0:         strcpy(keyn, "0");			break;
		case DIK_MINUS:		strcpy(keyn, "-");			break;	
		case DIK_EQUALS:	strcpy(keyn, "=");			break;
		case DIK_BACK:		strcpy(keyn, "BKSPC");		break;
		case DIK_TAB:		strcpy(keyn, "TAB");		break;
		case DIK_Q:			strcpy(keyn, "Q");			break;
		case DIK_W:			strcpy(keyn, "W");			break;
		case DIK_E:			strcpy(keyn, "E");			break;
		case DIK_R:			strcpy(keyn, "R");			break;
		case DIK_T:			strcpy(keyn, "T");			break;
		case DIK_Y:			strcpy(keyn, "Y");			break;
		case DIK_U:			strcpy(keyn, "U");			break;
		case DIK_I:			strcpy(keyn, "I");			break;
		case DIK_O:			strcpy(keyn, "O");			break;
		case DIK_P:			strcpy(keyn, "P");			break;
		case DIK_LBRACKET:	strcpy(keyn, "[");			break;
		case DIK_RBRACKET:	strcpy(keyn, "]");			break;
		case DIK_RETURN:	strcpy(keyn, "ENTER");		break;
		case DIK_LCONTROL:	strcpy(keyn, "L ");			break;
		case DIK_A:			strcpy(keyn, "A");			break;
		case DIK_S:			strcpy(keyn, "S");			break;
		case DIK_D:			strcpy(keyn, "D");			break;
		case DIK_F:			strcpy(keyn, "F");			break;
		case DIK_G:			strcpy(keyn, "G");			break;
		case DIK_H:			strcpy(keyn, "H");			break;
		case DIK_J:			strcpy(keyn, "J");			break;
		case DIK_K:			strcpy(keyn, "K");			break;
		case DIK_L:			strcpy(keyn, "L");			break;
		case DIK_SEMICOLON:	strcpy(keyn, ";");			break;
		case DIK_APOSTROPHE:strcpy(keyn, "'");			break;
		case DIK_LSHIFT:	strcpy(keyn, "L SHIFT");	break;
		case DIK_BACKSLASH:	strcpy(keyn, "\\");			break;
		case DIK_Z:			strcpy(keyn, "Z");			break;
		case DIK_X:			strcpy(keyn, "X");			break;
		case DIK_C:			strcpy(keyn, "C");			break;
		case DIK_V:			strcpy(keyn, "V");			break;	
		case DIK_B:			strcpy(keyn, "B");			break;
		case DIK_N:			strcpy(keyn, "N");			break;
		case DIK_M:			strcpy(keyn, "M");			break;
		case DIK_COMMA:		strcpy(keyn, ",");			break;
		case DIK_PERIOD:	strcpy(keyn, ".");			break;	
		case DIK_SLASH:		strcpy(keyn, "/");			break;
		case DIK_RSHIFT:	strcpy(keyn, "L SHIFT");	break;
		case DIK_MULTIPLY:	strcpy(keyn, "*");			break;
		case DIK_LMENU:		strcpy(keyn, "L ALT");		break;
		case DIK_SPACE:		strcpy(keyn, "SPACE");		break;	
		case DIK_F1:		strcpy(keyn, "F1");			break;
		case DIK_F2:		strcpy(keyn, "F2");			break;
		case DIK_F3:		strcpy(keyn, "F3");			break;
		case DIK_F4:		strcpy(keyn, "F4");			break;
		case DIK_F5:		strcpy(keyn, "F5");			break;
		case DIK_F6:		strcpy(keyn, "F6");			break;
		case DIK_F7:		strcpy(keyn, "F7");			break;
		case DIK_F8:		strcpy(keyn, "F8");			break;
		case DIK_F9:		strcpy(keyn, "F9");			break;
		case DIK_F10:		strcpy(keyn, "F10");		break;	
		case DIK_NUMLOCK:	strcpy(keyn, "NUMLOCK");	break;
		case DIK_SCROLL:	strcpy(keyn, "SCROLL LOCK");break;
		case DIK_NUMPAD7:	strcpy(keyn, "NUMPAD7");	break;
		case DIK_NUMPAD8:	strcpy(keyn, "NUMPAD8");	break;
		case DIK_NUMPAD9:	strcpy(keyn, "NUMPAD9");	break;
		case DIK_SUBTRACT:	strcpy(keyn, "-");			break;
		case DIK_NUMPAD4:	strcpy(keyn, "NUMPAD4");	break;
		case DIK_NUMPAD5:	strcpy(keyn, "NUMPAD5");	break;
		case DIK_NUMPAD6:	strcpy(keyn, "NUMPAD6");	break;
		case DIK_ADD:		strcpy(keyn, "NUMPAD+");	break;	
		case DIK_NUMPAD1:	strcpy(keyn, "NUMPAD1");	break;
		case DIK_NUMPAD2:	strcpy(keyn, "NUMPAD2");	break;
		case DIK_NUMPAD3:	strcpy(keyn, "NUMPAD3");	break;
		case DIK_NUMPAD0:	strcpy(keyn, "NUMPAD0");	break;
		case DIK_DECIMAL:	strcpy(keyn, "NUMPAD.");	break;	
		case DIK_F11:		strcpy(keyn, "F11");		break;
		case DIK_F12:		strcpy(keyn, "F12");		break;
		case DIK_UP:		strcpy(keyn, "UP");			break;
		case DIK_DOWN:		strcpy(keyn, "DOWN");		break;
		case DIK_LEFT:		strcpy(keyn, "LEFT");		break;
		case DIK_RIGHT:		strcpy(keyn, "RIGHT");		break;
		case DIK_RMENU:		strcpy(keyn, "R ALT");		break;
		case DIK_PRIOR:		strcpy(keyn, "PAGE UP");	break;
		case DIK_NEXT:		strcpy(keyn, "PAGE DN");	break;
		case DIK_END:		strcpy(keyn, "END");		break;
		case DIK_HOME:		strcpy(keyn, "HOME");		break;

		case DIK_SYSRQ:		strcpy(keyn, "PRINT SCREEN");		break;

		default:
			sprintf(keyn, "%d", key); break;
	}
}


//The Frame function for the InputClass will read the current state of the devices into state buffers we setup. 
//After the state of each device is read it then processes the changes.
bool DXInputClass::Frame()
{
	// Read the current state of the keyboard.
	if(!ReadKeyboard())return false;

	// Read the current state of the mouse.
	if(!ReadMouse())return false;

	m_mouseX = (float)m_mouseState.lX;
	m_mouseY = (float)m_mouseState.lY;
	
	#if defined DX_INPUT_DEBUG_TEST
	int count = 0;
	for (int i = 0; i < 256; i++)
	{
		if (m_keyboardState[i] & 0x80)	
		{
			count ++;
			char keyName[MAX_STR_LEN]={0};
			GetKeyName(i, keyName);
			WOMA_LOGManager_DebugMSG ("%s ", keyName);
		}
	}
	if (count)
		WOMA_LOGManager_DebugMSG ("\n");
	#endif

	ProcessInput();

	return true;
}

//ReadKeyboard will read the state of the keyboard into the m_keyboardState variable. The state will show any keys that are 
//currently pressed or not pressed. If it fails reading the keyboard then it can be for one of five different reasons. 
//The only two that we want to recover from are if the focus is lost or if it becomes un-acquired. 
//If this is the case we call acquire each frame until we do get control back. The window may be minimized in which case 
//Acquire will fail, but once the window comes to the foreground again then Acquire will succeed and we will be able to 
//read the keyboard state. The other three error types I don't want to recover from in this tutorial.
bool DXInputClass::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{ 
			memset(m_keyboardState, 0, sizeof(m_keyboardState)); //for (int i = 0; i < 256; i++) m_keyboardState[i] = 0;
			m_keyboard->Acquire();
		}
		else
			return false;
	}
		
	return true;
}

//ReadMouse will read the state of the mouse similar to how ReadKeyboard read in the state of the keyboard. 
//However the state of the mouse is just changes in the position of the mouse from the last frame. 
//So for example updates will look like the mouse has moved 5 units to the right, but it will not give you the actual 
//position of the mouse on the screen. This delta information is very useful for different purposes and we can maintain the 
//position of the mouse on the screen ourselves.
bool DXInputClass::ReadMouse()
{
	HRESULT result;

	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_mouse->Acquire();
		else
			return false;
	}

	// NOTE: Use MACRO "MOUSE_DOWN" to read "this" state!

	return true;
}

void DXInputClass::mouseWheelIn(WPARAM wParam)
{
    mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
}

bool DXInputClass::mouseWheelUp()
{
    int wheel = mouseWheel;
	if (wheel > 0)
		mouseWheel = 0;
    return (wheel > 0);
}

bool DXInputClass::mouseWheelDown()
{
    int wheel = mouseWheel;
	if (wheel < 0)
		mouseWheel = 0;
    return wheel < 0;
}



