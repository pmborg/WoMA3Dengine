// --------------------------------------------------------------------------------------------
// Filename: handleInputs.cpp
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
// PURPOSE: Work as Input Manager, using inputClass as a Direct Driver
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#include "platform.h"
#if defined USE_DIRECT_INPUT
#if defined DX_ENGINE
#include "WinSystemClass.h"
#include "dxWinSystemClass.h"
#endif
#include "WomaDriverClass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"

#if defined DX11
#include "dx11Class.h"
#endif
#if defined DX12
#include "dx12Class.h"
#endif

#include "ApplicationClass.h"
#include "OSengine.h"

bool f11GodState = false;
bool g_GOD_MODE = false;

#if defined DX_ENGINE
UINT KeyFront[]	= {DIK_W, DIK_UP};
UINT KeyBack[]	= {DIK_S, DIK_DOWN};
UINT KeyLeft[]	= {DIK_A, DIK_LEFT};
UINT KeyRigth[]	= {DIK_D, DIK_RIGHT};

//UINT KeyRun		= {DIK_LCONTROL};
UINT KeyRun =	  { DIK_LSHIFT };

UINT KeyLookUp	= {DIK_PGUP};
UINT KeyLookDown= {DIK_PGDN};

#if !defined RELEASE
UINT KeyGodMode	= {DIK_F11};
UINT KeyFlyUp	= {DIK_R};
UINT KeyFlyDown	= {DIK_F};
#endif

float terrain_nx = 0.0f;
float terrain_nz = 0.0f;
float nextHeight = 0;

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

	#if !defined RELEASE
	m_ourPlayer->p_player.IsGodModePressed	= (m_keyboardState[KeyGodMode] & 0x80) ? true:false;
	m_ourPlayer->p_player.Is1Pressed = (m_keyboardState[KeyFlyUp] & 0x80) ? true : false;
	m_ourPlayer->p_player.IsQPressed = (m_keyboardState[KeyFlyDown] & 0x80) ? true : false;
	#endif
    
	//COMPOUND DEBUG:

	//MAP
	m_ourPlayer->p_player.IsMapPressed		= (m_keyboardState[DIK_M] & 0x80) ? true:false;

	//WEAPONS

	//BAG:
}

#endif
#endif

#if defined USE_DIRECT_INPUT
// The HandleUserInput function does all the processing related to the user input from the keyboard and mouse.
// ==================================================================================================================================
bool ApplicationClass::HandleUserInput(double frameTime)
// ==================================================================================================================================
{
    float posX, posZ;

    //	------------------------------------------------------------------------------------------------------------------------
    // Action Keys HERE:
    //	------------------------------------------------------------------------------------------------------------------------

    // Proccess "F11": GOD MODE SWITCH
    // --------------------------------------------------------------------------------------------
#if defined DX_ENGINE
    if ((DXsystemHandle->m_player[g_NetID]->p_player.IsGodModePressed) && (!f11GodState)) {
        f11GodState = true;
    }
    if ((!DXsystemHandle->m_player[g_NetID]->p_player.IsGodModePressed) && (f11GodState)) {
        g_GOD_MODE = !g_GOD_MODE;
        f11GodState = false;
    }
#endif

    // Proccess "EXIT MAP": with "ESC" key
    // --------------------------------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 62 && defined USE_MAIN_MAP //TUTORIAL_PRE_CHAP >= 26 (G:\DRIVE_MY_SOURCE_CODE\WorldOfMiddleAge\WoMA_PartII)
    if (WOMA::game_state == GAME_MAP)
    {
        static bool state = false;

        if ((SystemHandle->m_player[g_NetID]->p_player.IsEscapePressed) && (!state)) {
            state = true;
        }
        if ((!SystemHandle->m_player[g_NetID]->p_player.IsEscapePressed) && (state)) {
			WOMA::game_state = GAME_RUN; // Return no normal mode, after "Escape" from MAP
            state = false;
        }
    }

    // Check the "M" Map key switch:
    // =============================
    if (SystemHandle->m_player[g_NetID]->p_player.IsMapPressed && mapKey == false && WOMA::game_state == GAME_RUN) {
        mapKey = true;
		WOMA::game_state = GAME_MAP;	// Action
    }
    if (!SystemHandle->m_player[g_NetID]->p_player.IsMapPressed && mapKey == true) {
        mapKey = false;
    }
#endif

    //[1] Set the frame time for calculating the updated position:
    // --------------------------------------------------------------------------------------------
    m_NextPosition->SetFrameTime(frameTime);					// Time frame To animate: our player

    // Copy Current Position to "Possible" next position...
    m_NextPosition->m_positionX =m_Position[g_NetID]->m_positionX;
    m_NextPosition->m_positionY =m_Position[g_NetID]->m_positionY;
    m_NextPosition->m_positionZ =m_Position[g_NetID]->m_positionZ;
    m_NextPosition->m_rotationY =m_Position[g_NetID]->m_rotationY;
    m_NextPosition->m_rotationX =m_Position[g_NetID]->m_rotationX;


#if defined CHECK_OBJ_COLISION //Check Compound Colision: STOP!
	bool CompoundXnormalOK = true;
	bool CompoundZnormalOK = true;
	
	if (!g_GOD_MODE) {
		if (DXsystemHandle->m_player[g_NetID]->p_player.IsUpPressed && (DXsystemHandle->m_Application->closestObjDist > -1) && (DXsystemHandle->m_Application->closestObjDist <= 1)) {
			CompoundXnormalOK = false;
			CompoundZnormalOK = false;
			m_NextPosition->m_forwardSpeed = 0; //Stop immediately
		}
	}
#endif

	// --------------------------------------------------------------------------------------------
    //[2] KEYBOARD & MOUSE WHELL: Handle the "USER Input" and get a "possible" next position:
    // --------------------------------------------------------------------------------------------

	// --------------------------------------------------------------------------------------------
    // [3] MOUSE: Process Mouse Movement for Camera :
    // --------------------------------------------------------------------------------------------
	float height = SystemHandle->AppSettings->INIT_CAMY;

#if defined DX_ENGINE
    if (DXsystemHandle->m_Input->m_mouseState.rgbButtons[MOUSE_LEFT] & 0x80)
    {
        m_NextPosition->m_rotationY += 0.1f * DXsystemHandle->m_Input->m_mouseX;	// (0.005/0.0174532925f)
        m_NextPosition->m_rotationX += 0.1f * DXsystemHandle->m_Input->m_mouseY;	// (0.005/0.0174532925f)
    }


	if (DXsystemHandle->m_Input->m_mouseState.rgbButtons[MOUSE_RIGHT] & 0x80) {
		m_Light->m_lightDirection.x /*m128_f32[0]*/ -= 0.001f* DXsystemHandle->m_Input->m_mouseX;		// X
		m_Light->m_lightDirection.y /*m128_f32[1]*/ += 0.002f* DXsystemHandle->m_Input->m_mouseY;		// Y
		//m_Light->m_lightDirection.m128_f32[2] += 0.001* m_Input->mouseWheel;	// Z
	}
	else
	{  // An OBJ was not selected so we are NOT in EDIT MODE.

		m_NextPosition->TurnLeft(DXsystemHandle->m_player[g_NetID]->p_player.IsLeftPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed);	//LEFT ARROW
		m_NextPosition->TurnRight(DXsystemHandle->m_player[g_NetID]->p_player.IsRightPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed);//RIGHT ARROW

		DXsystemHandle->m_player[g_NetID]->p_player.IsUpPressed = true;
		//On water? make it slow...:
		bool onWater = (m_NextPosition->m_positionY <= -0.25);
		m_NextPosition->MoveForward(DXsystemHandle->m_player[g_NetID]->p_player.IsUpPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed, DXsystemHandle->m_Input->mouseWheelUp(), onWater ? true : false);	//FRONT ARROW
		m_NextPosition->MoveBackward(DXsystemHandle->m_player[g_NetID]->p_player.IsDownPressed, DXsystemHandle->m_player[g_NetID]->p_player.IsLeftCtrlPressed, DXsystemHandle->m_Input->mouseWheelDown(), onWater ? true : false);	//BACK ARROW
	
		m_NextPosition->LookUpward(DXsystemHandle->m_player[g_NetID]->p_player.IsPgUpPressed);		//PG UP
		m_NextPosition->LookDownward(DXsystemHandle->m_player[g_NetID]->p_player.IsPgDownPressed);	//PG DOWN
	}

	// --------------------------------------------------------------------------------------------
    // If GOD MODE Allow 1/Q keys: Move UP and DOWN
    if (g_GOD_MODE) 
	{
        m_NextPosition->MoveUpward(DXsystemHandle->m_player[g_NetID]->p_player.Is1Pressed);
        m_NextPosition->MoveDownward(DXsystemHandle->m_player[g_NetID]->p_player.IsQPressed);

        height = m_NextPosition->m_positionY;
    }
#endif

    //[3] - Check Terrain Colisions
    // --------------------------------------------------------------------------------------------
#if defined SCENE_TERRAIN_COLLISION
#if NOTES
	#if   !defined SCENE_MAIN_TOPO_TERRAIN_USE_INDEX //DX_ENGINE_LEVEL < 52
		#define mainTerrain loadedTerrain[2]
		#define ID 2
	#else
		#define mainTerrain loadedTerrain[3]
		#define ID 3
	#endif
#endif

    //Detect colision with terrain LEFT / RIGHT:
    if (!g_GOD_MODE)
    {
		nextHeight = mainTerrain->getTerrainHeight(TERRAIN_ID, m_NextPosition->m_positionX, m_NextPosition->m_positionZ);
		int terrainHeight = mainTerrain->m_terrainHeight;
		int terrainWidth = mainTerrain->m_terrainWidth;
        if	((m_NextPosition->m_positionZ < terrainHeight) && 
			(m_NextPosition->m_positionX < terrainWidth) &&
            (m_NextPosition->m_positionZ > 1) && 
			(m_NextPosition->m_positionX > 1)
			)
		{
			terrain_nx = SystemHandle->m_Application->loadedTerrain[2]->modelVertexVector2[(mainTerrain->m_terrainHeight * ((int)m_NextPosition->m_positionZ)) + ((int)m_NextPosition->m_positionX)].nx;//TODO: all map types
			terrain_nz = SystemHandle->m_Application->loadedTerrain[2]->modelVertexVector2[(mainTerrain->m_terrainHeight * ((int)m_NextPosition->m_positionZ)) + ((int)m_NextPosition->m_positionX)].nz;

			{
				#if defined CHECK_OBJ_COLISION //Check!: CompoundZnormalOK
				if (CompoundZnormalOK)
				#endif
				   m_Position[g_NetID]->m_positionZ = m_NextPosition->m_positionZ;
			}
			{
				#if defined CHECK_OBJ_COLISION //Check!: CompoundXnormalOK
				if (CompoundXnormalOK)
				#endif
				   m_Position[g_NetID]->m_positionX = m_NextPosition->m_positionX;
			}
			
		}
    } else {
       m_Position[g_NetID]->m_positionZ = m_NextPosition->m_positionZ;
       m_Position[g_NetID]->m_positionX = m_NextPosition->m_positionX;
    }

    // Update also "Y" position (Get Height at our position)
    // --------------------------------------------------------------------------------------------
    if (!g_GOD_MODE)
        height = mainTerrain->getTerrainHeight(TERRAIN_ID, m_Position[g_NetID]->m_positionX, m_Position[g_NetID]->m_positionZ); // Gravity to Terrain
#else
    //[4] Get the new allowed positions:
   m_Position[g_NetID]->m_positionZ = m_NextPosition->m_positionZ;
   m_Position[g_NetID]->m_positionX = m_NextPosition->m_positionX;
#endif

   m_Position[g_NetID]->m_positionY = height;
   posX =m_Position[g_NetID]->m_positionX;
   posZ =m_Position[g_NetID]->m_positionZ;

    // Update also the "Rotations":
    // --------------------------------------------------------------------------------------------
   m_Position[g_NetID]->m_rotationY = m_NextPosition->m_rotationY;
   m_Position[g_NetID]->m_rotationX = m_NextPosition->m_rotationX;

#if defined USE_DIRECT_INPUT
    //[5] After the movement for this frame has been calculated we then get the position and rotation 
    //from the PositionObject and update the CameraClass and TextClass object with the new viewing position.
    // Get the view point position/rotation.
    // --------------------------------------------------------------------------------------------
	if (SystemHandle->AppSettings->DRIVER != DRIVER_GL3) {
	#if defined DX_ENGINE
		DXsystemHandle->m_Camera->m_positionY = height + 1.25f; // Camera are: "x.xf" above our feets.
		DXsystemHandle->m_Camera->m_positionX = posX;
		DXsystemHandle->m_Camera->m_positionZ = posZ;

		DXsystemHandle->m_Camera->m_rotationY =m_Position[g_NetID]->m_rotationY;
		DXsystemHandle->m_Camera->m_rotationX =m_Position[g_NetID]->m_rotationX;
	#endif
	}
	#if defined OPENGL3
	else
	{
		GLopenGLclass* driver = (GLopenGLclass*) /*SystemHandle->*/driverList[SystemHandle->AppSettings->DRIVER];

		driver->gl_Camera->m_positionY = height + 1.25f; // Camera are: "x.xf" above our feets.
		driver->gl_Camera->m_positionX = posX;
		driver->gl_Camera->m_positionZ = posZ;

		driver->gl_Camera->m_rotationY = m_Position[g_NetID]->m_rotationY;
		driver->gl_Camera->m_rotationX = m_Position[g_NetID]->m_rotationX;
	}
	#endif
#endif

#if DX_ENGINE_LEVEL >= 63 && defined (USE_MINI_MAP) 
    //The position of the user on the mini-map will then be accurately reflected each frame.
    // Update the location of the camera on the mini map.
    // --------------------------------------------------------------------------------------------
    for (UINT i = 0; i < HowManyPlayers; i++)
    {
        {
            posX =m_Position[i]->m_positionX;
            posZ =m_Position[i]->m_positionZ;

            PositionUpdate(i, posX, posZ);
            m_pointRotation[i] = (int)m_Position[i]->m_rotationY; //need to update rotY
        }
    }
#endif

    return true;
}
#endif
