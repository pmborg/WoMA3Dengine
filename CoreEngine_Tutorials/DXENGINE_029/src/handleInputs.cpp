// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: handleInputs.cpp
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
// PURPOSE: Work as Input Manager, using inputClass as a Direct Driver
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567829;

#include "platform.h"

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"

#include "dx11Class.h"
#include "dx12Class.h"

#include "ApplicationClass.h"
#include "OSengine.h"
#include "dxWinSystemClass.h"
 
bool g_GOD_MODE = false;

UINT KeyFront[]	= {DIK_W, DIK_UP};
UINT KeyBack[]	= {DIK_S, DIK_DOWN};
UINT KeyLeft[]	= {DIK_A, DIK_LEFT};
UINT KeyRigth[]	= {DIK_D, DIK_RIGHT};

//UINT KeyRun		= {DIK_LCONTROL};
UINT KeyRun =	  { DIK_LSHIFT };

UINT KeyLookUp	= {DIK_PGUP};
UINT KeyLookDown= {DIK_PGDN};

	UINT KeyGodMode	= {DIK_F11};
	UINT KeyFlyUp	= {DIK_R};
	UINT KeyFlyDown	= {DIK_F};

//The ProcessInput function is where we deal with the changes that have happened in the input devices since the last frame. 
//For this tutorial we will just do a simple mouse location update similar to how Windows keeps track of where the mouse cursor is. 
//To do so we use the m_mouseX and m_mouseY variables that were initialized to zero and simply add the changes in the mouse position 
//to these two variables. This will maintain the position of the mouse based on the user moving the mouse around.

//Note that we do check to make sure the mouse location never goes off the screen. 
//Even if the user keeps moving the mouse to the left we will just keep the cursor at the zero position until they start 
//moving it to the right again.

void DXInputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	#define m_ourPlayer DXsystemHandle->m_player[g_NetID]

	// Process Keyboard STATE: (CHECK)
	// =======================
	//EXIT:
	m_ourPlayer->p_player.IsEscapePressed	= (m_keyboardState[DIK_ESCAPE] & 0x80) ? true:false;

	//BASE MOVEMENT:
	m_ourPlayer->p_player.IsUpPressed		= (m_keyboardState[KeyFront[0]] & 0x80) || (m_keyboardState[KeyFront[1]] & 0x80) ? true:false;
	m_ourPlayer->p_player.IsDownPressed		= (m_keyboardState[KeyBack[0]] & 0x80) || (m_keyboardState[KeyBack[1]] & 0x80) ? true:false;
	m_ourPlayer->p_player.IsLeftPressed		= (m_keyboardState[KeyLeft[0]] & 0x80) || (m_keyboardState[KeyLeft[1]] & 0x80) ? true:false;
	m_ourPlayer->p_player.IsRightPressed	= (m_keyboardState[KeyRigth[0]] & 0x80) || (m_keyboardState[KeyRigth[1]] & 0x80) ? true:false;

	m_ourPlayer->p_player.IsLeftCtrlPressed = (m_keyboardState[KeyRun] & 0x80) ? true : false;				//RUN

	m_ourPlayer->p_player.IsPgUpPressed		= (m_keyboardState[KeyLookUp] & 0x80) ? true:false;				//Look UP
	m_ourPlayer->p_player.IsPgDownPressed	= (m_keyboardState[KeyLookDown] & 0x80) ? true : false;			//Look DOWN

	m_ourPlayer->p_player.IsGodModePressed	= (m_keyboardState[KeyGodMode] & 0x80) ? true:false;

	m_ourPlayer->p_player.Is1Pressed = (m_keyboardState[KeyFlyUp] & 0x80) ? true : false;
	m_ourPlayer->p_player.IsQPressed = (m_keyboardState[KeyFlyDown] & 0x80) ? true : false;
    
	//COMPOUND DEBUG:

	//MAP

	//WEAPONS

	//BAG:
}

// The HandleUserInput function does all the processing related to the user input from the keyboard and mouse.
// ==================================================================================================================================
bool ApplicationClass::HandleUserInput(float frameTime)
// ==================================================================================================================================
{
    float posX, posZ;

    //	------------------------------------------------------------------------------------------------------------------------
    // Action Keys HERE:
    //	------------------------------------------------------------------------------------------------------------------------

    // Proccess "F11": GOD MODE SWITCH
    // --------------------------------------------------------------------------------------------
    static bool f11state = false;

    if ((DXsystemHandle->m_player[g_NetID]->p_player.IsGodModePressed) && (!f11state)) {
        f11state = true;
    }
    if ((!DXsystemHandle->m_player[g_NetID]->p_player.IsGodModePressed) && (f11state)) {
        g_GOD_MODE = !g_GOD_MODE;
        f11state = false;
    }

    // Proccess "EXIT MAP": with "ESC" key
    // --------------------------------------------------------------------------------------------

    //[1] Set the frame time for calculating the updated position:
    // --------------------------------------------------------------------------------------------
    m_NextPosition->SetFrameTime(frameTime);					// Time frame To animate: our player

    // Copy Current Position to "Possible" next position...
    m_NextPosition->m_positionX =m_Position[g_NetID]->m_positionX;
    m_NextPosition->m_positionY =m_Position[g_NetID]->m_positionY;
    m_NextPosition->m_positionZ =m_Position[g_NetID]->m_positionZ;
    m_NextPosition->m_rotationY =m_Position[g_NetID]->m_rotationY;
    m_NextPosition->m_rotationX =m_Position[g_NetID]->m_rotationX;


	// --------------------------------------------------------------------------------------------
    //[2] KEYBOARD & MOUSE WHELL: Handle the "USER Input" and get a "possible" next position:
    // --------------------------------------------------------------------------------------------

	// --------------------------------------------------------------------------------------------
    // [3] MOUSE: Process Mouse Movement for Camera :
    // --------------------------------------------------------------------------------------------
    if (DXsystemHandle->m_Input->m_mouseState.rgbButtons[MOUSE_LEFT] & 0x80)
    {
        m_NextPosition->m_rotationY += 0.1f * DXsystemHandle->m_Input->m_mouseX;	// (0.005/0.0174532925f)
        m_NextPosition->m_rotationX += 0.1f * DXsystemHandle->m_Input->m_mouseY;	// (0.005/0.0174532925f)
    }

	{  // An OBJ was not selected so we are NOT in EDIT MODE.

		m_NextPosition->TurnLeft(DXsystemHandle->m_player[g_NetID]->p_player.IsLeftPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed);	//LEFT ARROW
		m_NextPosition->TurnRight(DXsystemHandle->m_player[g_NetID]->p_player.IsRightPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed);//RIGHT ARROW

		//On water? make it slow...:
		bool onWater = (m_NextPosition->m_positionY <= -0.25);
		m_NextPosition->MoveForward(DXsystemHandle->m_player[g_NetID]->p_player.IsUpPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed, DXsystemHandle->m_Input->mouseWheelUp(), onWater ? true : false);	//FRONT ARROW
		m_NextPosition->MoveBackward(DXsystemHandle->m_player[g_NetID]->p_player.IsDownPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed, DXsystemHandle->m_Input->mouseWheelDown(), onWater ? true : false);	//BACK ARROW
	
		m_NextPosition->LookUpward(DXsystemHandle->m_player[g_NetID]->p_player.IsPgUpPressed);		//PG UP
		m_NextPosition->LookDownward(DXsystemHandle->m_player[g_NetID]->p_player.IsPgDownPressed);	//PG DOWN
	}



	// --------------------------------------------------------------------------------------------
    float height = SystemHandle->AppSettings->INIT_CAMY;

	// --------------------------------------------------------------------------------------------
    // If GOD MODE Allow 1/Q keys: Move UP and DOWN
    //if (g_GOD_MODE) 
	{
        m_NextPosition->MoveUpward(DXsystemHandle->m_player[g_NetID]->p_player.Is1Pressed);
        m_NextPosition->MoveDownward(DXsystemHandle->m_player[g_NetID]->p_player.IsQPressed);

        height = m_NextPosition->m_positionY;
    }

    static float nextHeight = 0;

    //[3] - Check Terrain Colisions: CHAP 14!!
    // --------------------------------------------------------------------------------------------
    //[4] Get the new allowed positions:
   m_Position[g_NetID]->m_positionZ = m_NextPosition->m_positionZ;
   m_Position[g_NetID]->m_positionX = m_NextPosition->m_positionX;

   m_Position[g_NetID]->m_positionY = height;
   posX =m_Position[g_NetID]->m_positionX;
   posZ =m_Position[g_NetID]->m_positionZ;

    // Update also the "Rotations":
    // --------------------------------------------------------------------------------------------
   m_Position[g_NetID]->m_rotationY = m_NextPosition->m_rotationY;
   m_Position[g_NetID]->m_rotationX = m_NextPosition->m_rotationX;

    //[5] After the movement for this frame has been calculated we then get the position and rotation 
    //from the PositionObject and update the CameraClass and TextClass object with the new viewing position.
    // Get the view point position/rotation.
    // --------------------------------------------------------------------------------------------
	if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3) {
		DXsystemHandle->m_Camera->m_positionY = height + 1.25f; // Camera are: "x.xf" above our feets.
		DXsystemHandle->m_Camera->m_positionX = posX;
		DXsystemHandle->m_Camera->m_positionZ = posZ;

		DXsystemHandle->m_Camera->m_rotationY =m_Position[g_NetID]->m_rotationY;
		DXsystemHandle->m_Camera->m_rotationX =m_Position[g_NetID]->m_rotationX;
	}
	else
	{
		GLopenGLclass* driver = (GLopenGLclass*) DXsystemHandle->driverList[SystemHandle->AppSettings->DRIVER];

		driver->gl_Camera->m_positionY = height + 1.25f; // Camera are: "x.xf" above our feets.
		driver->gl_Camera->m_positionX = posX;
		driver->gl_Camera->m_positionZ = posZ;

		driver->gl_Camera->m_rotationY = m_Position[g_NetID]->m_rotationY;
		driver->gl_Camera->m_rotationX = m_Position[g_NetID]->m_rotationX;
	}

    return true;
}
