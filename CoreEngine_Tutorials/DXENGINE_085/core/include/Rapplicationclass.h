// --------------------------------------------------------------------------------------------
// Filename: Rapplicationclass.h
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

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

/////////////
// GLOBALS //
/////////////
#include "OSengine.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#define VSYNC_ENABLED SystemHandle->AppSettings->VSYNC_ENABLED
#define SCREEN_NEAR SystemHandle->AppSettings->SCREEN_NEAR
#define SCREEN_DEPTH SystemHandle->AppSettings->SCREEN_DEPTH

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Rinputclass.h"
#include "Ropenglclass.h"
#include "RCameraClass.h"
#include "Rfontshaderclass.h"
#include "Rfontclass.h"
#include "Rtextclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class Name: RApplicationClass
////////////////////////////////////////////////////////////////////////////////
class RApplicationClass
{
public:
    RApplicationClass();
    ~RApplicationClass();

#if !defined ANDROID_PLATFORM
    bool Initialize(/*Display*, Window,*/ int, int);
#else
    bool Initialize(int, int);
#endif
    void Shutdown();
    bool Frame(RInputClass*);

private:
    bool Render();

private:
    OpenGLClass* m_OpenGL;
    RCameraClass* m_Camera;
    RFontShaderClass* m_FontShader;
    RFontClass* m_Font;
    RTextClass* m_TextString1;
};

#endif
#endif