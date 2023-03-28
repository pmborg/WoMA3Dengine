// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: playerClass.h
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// -------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------
#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#include "main.h"

	#include "DX9Class.h"

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
	XMMATRIX	m_world;	// World Matrix: our position in the 3D world space
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
	UINT WomaIntegrityCheck = 1234567890;
	PlayerClass(UINT id);
	~PlayerClass();

public:
	playerType p_player;

};

