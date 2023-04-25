// --------------------------------------------------------------------------------------------
// Filename: win32InputClass.cpp
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
// --------------------------------------------------------------------------------------------
#include "main.h"

#include "win32InputClass.h"
#include "dxWinSystemClass.h"

InputClass::InputClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

	ZeroMemory (&m_keys, sizeof(m_keys));
}

InputClass::~InputClass() {CLASSDELETE();}

void InputClass::Initialize()
{
	// Initialize all the keys to being released and not pressed.
	for(int i=0; i<256; i++)
		m_keys[i] = false;
}

#include "minwindef.h"
void InputClass::KeyDown(unsigned int lparam, unsigned int wparam)
{
	WORD vkCode = LOWORD(wparam);                                 // virtual-key code
	WORD keyFlags = HIWORD(lparam);
	WORD scanCode = LOBYTE(keyFlags);                             // scan code
	BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED; // extended-key flag, 1 if scancode has 0xE0 prefix

	unsigned int key = (lparam >> 16) & 0xFF;
	if (isExtendedKey)
		key += 0x80;
	//printf("DOWN: lparam, wparam: %#08x, %#08x\n", lparam, wparam);
	DXsystemHandle->m_Input->m_keyboardState[key] = 0x80;
	m_keys[key] = true;
}


void InputClass::KeyUp(unsigned int lparam, unsigned int wparam)
{
	WORD vkCode = LOWORD(wparam);                                 // virtual-key code
	WORD keyFlags = HIWORD(lparam);
	WORD scanCode = LOBYTE(keyFlags);                             // scan code
	BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED; // extended-key flag, 1 if scancode has 0xE0 prefix

	unsigned int key = (lparam >> 16) & 0xFF;
	if (isExtendedKey)
		key += 0x80;
	//printf("UP: lparam, wparam: %#08x, %#08x\n", lparam, wparam);
	DXsystemHandle->m_Input->m_keyboardState[key] = 0;
	m_keys[key] = false;
}

void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

