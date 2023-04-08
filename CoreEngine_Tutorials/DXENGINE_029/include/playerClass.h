// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: playerClass.h
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#include "main.h"

#if defined DX9sdk
	#include "DX9Class.h"
#endif

// -------------------------------------------------------------------------------------------
// Use OLD xnamath from DirectX SDK June2010 or Windows Kit 8?
// -------------------------------------------------------------------------------------------
	#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition
	#include <d3d11.h>

#if D3D11_SPEC_DATE_YEAR == 2009		// Use the OLD DirectX_SDK_June2010 ?
	#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
	#pragma warning( disable : 4838 )
	#include <xnamath.h>				// #include <d3dx10math.h>
#else
	#include <DirectXMath.h>			// Use the NEW DirectX11
	using namespace DirectX;
#endif

	// DX12 includes
#include <dxgi1_4.h>	// Always 1st!	(Select Driver)
#include <d3d12.h>		// DX12			(Select Device)
#include <D3Dcompiler.h>// Use Compiler
#include <DirectXMath.h>// Use Math
using namespace DirectX;

	
/////////////
// GLOBALS //
/////////////

/*
#if TUTORIAL_PRE_CHAP < 90
#define N_MAX_PLAYERS 1
#else
#define N_MAX_PLAYERS 10	//network support added on CH 90: Align with: network\server\networkmessages.h
#endif//
*/

typedef struct
{	
	// Current Position:
	// --------------------------------------------------------------------------------
	#if defined DX_ENGINE
	XMMATRIX	m_world;	// World Matrix: our position in the 3D world space
	#endif
    bool online;			// Network State

    // ids:
	// --------------------------------------------------------------------------------

	//Keyboard State:
	// --------------------------------------------------------------------------------

	//EXIT:
	bool IsEscapePressed;	

	//BASE MOVEMENT:
	bool IsLeftPressed;		// <- or a
	bool IsRightPressed;	// -> or d
	bool IsUpPressed;		// ^ or w
	bool IsDownPressed;		// v or s
	bool IsLeftCtrlPressed; // RUN:			left ctrl
	bool IsPgUpPressed;		// LOOK UP:		pg up
	bool IsPgDownPressed;	// LOOK DOWN:	pg down

	//GOD_MODE:
	bool IsGodModePressed;	// F11

	bool Is1Pressed;		// GOD MODE:	1
	bool IsQPressed;		// GOD MODE:	q

    //DEBUG / COMPOUND:
	bool IsNumPadPlus;		// work also with L CTRL or R CTRL
	bool IsNumPadMinus;		// work also with L CTRL or R CTRL

	//MAP:

	//BAG:

} playerType;


///////////////////////////////////////////////////////////////////////////////
// Class name: PlayerClass
///////////////////////////////////////////////////////////////////////////////
class PlayerClass
{
public:
	UINT WomaIntegrityCheck = 1234567829;
	PlayerClass(UINT id);
	~PlayerClass();

public:
	playerType p_player;

};

