////////////////////////////////////////////////////////////////////////////////
// Filename: glxopenglclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "main.h"
#if defined OPENGL3 && defined LINUX_PLATFORM && DX_ENGINE_LEVEL >= 19 // Initializing Engine 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <GL/glut.h>
#include "linux.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GLopenGLclass
////////////////////////////////////////////////////////////////////////////////

 
class glxOpenGLClass : public GLopenGLclass
{
public:
	glxOpenGLClass();
	~glxOpenGLClass();

	bool OnInit(	int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits, 
						float screenDepth, float screenNear, BOOL msaa, bool vsync, 
						BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize);
	void Shutdown();
	void EndScene(UINT monitorWindow);

	MyWin        Win_;
	//HDC m_deviceContext;
	GLXContext m_deviceContext;

	//
	Display* Xdisplay;
	int screen;
	GLXFBConfig fbconfig;
	XVisualInfo *visinfo;
	Window window;

private:
	bool LoadExtensionList();
	//HGLRC m_renderingContext1;

public:

};

#endif
