////////////////////////////////////////////////////////////////////////////////
// Filename: RApplicationClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

/////////////
// GLOBALS //
/////////////
#include "OSengine.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//#define FULL_SCREEN SystemHandle->AppSettings->FULL_SCREEN
#define VSYNC_ENABLED SystemHandle->AppSettings->VSYNC_ENABLED
#define SCREEN_NEAR SystemHandle->AppSettings->SCREEN_NEAR
#define SCREEN_DEPTH SystemHandle->AppSettings->SCREEN_DEPTH

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Rinputclass.h"
#include "Ropenglclass.h"
#include "Rcameraclass.h"
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