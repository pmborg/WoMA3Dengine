// --------------------------------------------------------------------------------------------
// Filename: glxOpenGLClass.cpp
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
#include "platform.h" //LPCSTR
#if defined LINUX_PLATFORM
#define LINUX_PLATFORM
//#define ENGINE_LEVEL 20

#include "OSengine.h"

#include "platform.h"
#if defined OPENGL3 && defined LINUX_PLATFORM && DX_ENGINE_LEVEL >= 19 // Initializing Engine 

//////////////
// INCLUDES //
//////////////
#include <math.h>

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "glxOpenGLclass.h"


glxOpenGLClass::glxOpenGLClass()
{
	CLASSLOADER();

	m_deviceContext = NULL;

	GLMajorVer = NULL;
	GLMinorVer = NULL;
}

glxOpenGLClass::~glxOpenGLClass(){CLASSDELETE();}

// SOURCE: https://github.com/datenwolf/codesamples/blob/master/samples/OpenGL/x11argb_opengl/x11argb_opengl.c
static int isExtensionSupported(const char *extList, const char *extension)
{
  const char *start;
  const char *where, *terminator;
 
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if ( where || *extension == '\0' )
    return 0;
 
  /* It takes a bit of care to be fool-proof about parsing the
OpenGL extensions string. Don't be fooled by sub-strings,
etc. */
  for ( start = extList; ; ) {
    where = strstr( start, extension );
 
    if ( !where )
      break;
 
    terminator = where + strlen( extension );
 
    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return 1;
 
    start = terminator;
  }
  return 0;
}

static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
    WOMA_LOGManager_DebugMSG("WARNING: at context creation\n");
    return 0;
}

// SOURCE: D:\WoMAengine2014\woma_developer\SAMPLES\Sample020_StartEngine\Src\original_sample_code.cxx
// GLXContext createContext( Display *display, int screen, GLXFBConfig fbconfig, XVisualInfo *visinfo, Window window )

extern MyWin Win;

bool glxOpenGLClass::OnInit(	int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits, 
									float screenDepth, float screenNear, BOOL msaa, bool vsync, 
									BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize)
{
	Xdisplay = Win.display;

	int dummy;
	if (!glXQueryExtension((Display*) Xdisplay, &dummy, &dummy)) 
	{
		printf("OpenGL not supported by X server\n");
		return false;
	}

	m_deviceContext = Win.context;

  return true;
}

void glxOpenGLClass::Shutdown()
{
	return;
}

void glxOpenGLClass::EndScene(UINT monitorWindow)
{
	glXSwapBuffers( Win.display, Win.window);

	return;
}

#endif

#endif