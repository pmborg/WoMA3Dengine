// ----------------------------------------------------------------------------------------------
// Filename: glxopenglclass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: 
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once

#include "platform.h"
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

	MyWin       Win_;
	GLXContext	m_deviceContext;
	Display*	Xdisplay;
	int			screen;
	GLXFBConfig fbconfig;
	XVisualInfo *visinfo;
	Window		window;

private:
	//bool LoadExtensionList();

public:

};

#endif
