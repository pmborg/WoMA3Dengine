// --------------------------------------------------------------------------------------------
// Filename: inputclass.h
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

//private:
	bool m_keys[256];
};
