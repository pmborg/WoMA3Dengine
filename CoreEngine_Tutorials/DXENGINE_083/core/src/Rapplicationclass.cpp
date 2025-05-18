// --------------------------------------------------------------------------------------------
// Filename: Rapplicationclass.cpp
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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut14.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
#include "woma_macros.h"

#if !defined ANDROID_PLATFORM
extern MyWin Win;
#endif

#define _tprintf printf

RApplicationClass::RApplicationClass()
{
    m_OpenGL = 0;
    m_Camera = 0;
    m_FontShader = 0;
    m_Font = 0;
    m_TextString1 = 0;
}

RApplicationClass::~RApplicationClass()
{
}

bool RApplicationClass::Initialize(int screenWidth, int screenHeight)
{
    // Create and initialize the OpenGL object.
    m_OpenGL = new OpenGLClass;
	_tprintf("m_OpenGL->Initialize()\n");
    IF_NOT_RETURN_FALSE (m_OpenGL->Initialize(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED));

    // Create and initialize the camera object.
    m_Camera = new RCameraClass;
	_tprintf("new RCameraClass\n");
    m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
    m_Camera->Render();

    // Create and initialize the font shader object.
    m_FontShader = new RFontShaderClass;
	
	_tprintf("m_FontShader->Initialize(m_OpenGL)\n");
    IF_NOT_RETURN_FALSE(m_FontShader->Initialize(m_OpenGL));

    // Create and initialize the font object.
    m_Font = new RFontClass;
	_tprintf("m_Font->Initialize(m_OpenGL, 0)\n");
    IF_NOT_RETURN_FALSE(m_Font->Initialize(m_OpenGL, 0));

    return true;
}

void RApplicationClass::Shutdown()
{
    SAFE_SHUTDOWN(m_Font);
    SAFE_SHUTDOWN(m_FontShader);
    SAFE_DELETE(m_Camera);
    SAFE_DELETE(m_OpenGL);

    return;
}

bool RApplicationClass::Frame(RInputClass* Input)
{
    // Render the graphics scene.
    IF_NOT_RETURN_FALSE (Render());

    return true;
}

bool RApplicationClass::Render()
{
    float worldMatrix[16], viewMatrix[16], orthoMatrix[16];
    float pixelColor[4];
    bool result;

    char FPSString[MAX_STR_LEN/4];//, testString2[32];
    // Set the strings we want to display.
    StringCchPrintf(FPSString, sizeof(FPSString), TEXT("Fps: %d  - ms: %4.2f"), SystemHandle->fps, 1000.0f / SystemHandle->fps);
    //strcpy(testString1, "Hello");

    // Create and initialize the first text object.
    m_TextString1 = new RTextClass;
    //_tprintf("m_TextString1->Initialize()\n");
    IF_NOT_RETURN_FALSE(m_TextString1->Initialize(m_OpenGL, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT, MAX_STR_LEN/4, m_Font, FPSString,
                        SystemHandle->AppSettings->WINDOW_WIDTH - 180, 40,             // Position
                        0.0f, 1.0f, 0.0f)); // Color

	// Clear the buffers to begin the scene.
    //m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Get the world, view, and ortho matrices from the opengl and camera objects.
    m_OpenGL->GetWorldMatrix(worldMatrix);	//W
    m_Camera->GetViewMatrix(viewMatrix);	//V
    m_OpenGL->GetOrthoMatrix(orthoMatrix);	//P

    // Get the color to render the text as.
	m_TextString1->GetPixelColor(pixelColor);

	// Set the font shader as active and set its parameters.
    IF_NOT_RETURN_FALSE (m_FontShader->SetShaderParameters(worldMatrix, viewMatrix, orthoMatrix, pixelColor));

	// Set the font texture as the active texture.
	m_Font->SetTexture();

    // Render the first text string using the font shader.
    m_TextString1->Render();

    SAFE_SHUTDOWN(m_TextString1);

    return true;
}

#endif