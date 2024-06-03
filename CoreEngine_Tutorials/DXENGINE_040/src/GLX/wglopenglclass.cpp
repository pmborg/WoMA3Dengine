// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: wGLopenGLclass.cpp
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

#include "platform.h"
#if defined OPENGL3 && defined WINDOWS_PLATFORM && DX_ENGINE_LEVEL >= 19  // Initializing Engine && defined WINDOWS_PLATFORM
#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "wGLopenGLclass.h"
#include "GLmathClass.h"
#include "log.h"

//#define old_school
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB;

wGLopenGLclass::wGLopenGLclass()
{
	CLASS_LOAD_N--; 
	CLASSLOADER();  //dont count super class!
	WomaIntegrityCheck = 1234567831;

	m_deviceContext = NULL;
	m_renderingContext1 = NULL;
	m_renderingContext2 = NULL;

	GLMajorVer = 0;
	GLMinorVer = 0;

    // double buffered, depth, color w/ alpha
	glutInitDisplayMode( GLUT_RIGHT_BUTTON | GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1, 1);
	
	int w = glutCreateWindow("OpenGL2023");
	HWND wh = FindWindow(NULL, TEXT("OpenGL2023"));
	ShowWindow(wh, SW_HIDE);

	int feedback = gl3wInit(); //Init GL3W
	if (feedback != -1) {
		glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVer);
		glGetIntegerv(GL_MINOR_VERSION, &GLMinorVer);
	}

	WOMA_LOGManager_DebugMSG ( "OpenGL: %s, GLSL: %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Release the temporary window
	glFinish();
	glutDestroyWindow( w );
	DestroyWindow(wh);

	WOMA_LOGManager_DebugMSG (TEXT("Render Driver: %s\n"), TEXT("wglOpenGL"));
	WOMA_LOGManager_DebugMSG (TEXT("Open GL version: %d.%d\n"), GLMajorVer, GLMinorVer);
}

wGLopenGLclass::~wGLopenGLclass() { 
	Shutdown();
	CLASSDELETE();
}

bool wGLopenGLclass::OnInit(int _USE_MONITOR, /*HWND*/void* hwnd, int screenWidth, int screenHeight, UINT depthBits, float screenDepth, float screenNear, 
						BOOL msaa, bool vsync, BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize)
{
	int pixelFormat;
	int result;
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {0}; // memset(&pixelFormatDescriptor, 0, sizeof(PIXELFORMATDESCRIPTOR));

	if (!hwnd)
		{ WomaFatalExceptionW (TEXT("HWND: NULL"));	return false; }

	// Get the device context for this window.
	m_deviceContext = GetDC((HWND)hwnd);
	IF_NOT_RETURN_FALSE(m_deviceContext);
	
#if defined old_school
	float fieldOfView, screenAspect;
	char *vendorString, *rendererString;
	int attributeListInt[19];
	unsigned int formatCount;
	int attributeList[5];

	// Support for OpenGL rendering.
	attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
	attributeListInt[1] = TRUE;

	// Support for rendering to a window.
	attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
	attributeListInt[3] = TRUE;

	// Support for hardware acceleration.
	attributeListInt[4] = WGL_ACCELERATION_ARB;
	attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;

	// Support for 24bit color.
	attributeListInt[6] = WGL_COLOR_BITS_ARB;
	attributeListInt[7] = 24;

	// Support for 24 bit depth buffer.
	attributeListInt[8] = WGL_DEPTH_BITS_ARB;
	attributeListInt[9] = 24;

	// Support for double buffer.
	attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
	attributeListInt[11] = TRUE;

	// Support for swapping front and back buffer.
	attributeListInt[12] = WGL_SWAP_METHOD_ARB;
	attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;

	// Support for the RGBA pixel type.
	attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
	attributeListInt[15] = WGL_TYPE_RGBA_ARB;

	// Support for a 8 bit stencil buffer.
	attributeListInt[16] = WGL_STENCIL_BITS_ARB;
	attributeListInt[17] = 8;

	// Null terminate the attribute list.
	attributeListInt[18] = 0;
	
	// Query for a pixel format that fits the attributes we want.
	result = wglChoosePixelFormatARB(m_deviceContext, attributeListInt, NULL, 1, &pixelFormat, &formatCount);
	if(result != 1)
	{
		return false;
	}
#else

    // assign a simple OpenGL pixel format that everyone supports
    pixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    //pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
    pixelFormatDescriptor.cColorBits = depthBits;	//Default: 24
    pixelFormatDescriptor.cDepthBits = depthBits;	//Default: 24
	pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD
	pixelFormatDescriptor.cStencilBits = 8;

	// NOTE: Using useDoubleBuffering="false" is not working!
	//if (g_UseDoubleBuffering)
		pixelFormatDescriptor.dwFlags |= PFD_DOUBLEBUFFER;

/*
static PIXELFORMATDESCRIPTOR    pixelFormatDescriptor = {
	sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
	1,                              // Version of this structure
	PFD_DRAW_TO_WINDOW |            // Draw to Window (not to bitmap)
	PFD_SUPPORT_OPENGL |            // Support OpenGL calls in window
	PFD_DOUBLEBUFFER |              // Double buffered mode
	PFD_STEREO_DONTCARE,
	PFD_TYPE_RGBA,                  // RGBA Color mode
	32,                             // Want the display bit depth
	0,0,0,0,0,0,                    // Not used to select mode
	0,0,                            // Not used to select mode
	0,0,0,0,0,                      // Not used to select mode
	24,                             // Size of depth buffer
	8,                              // bit stencil
	0,                              // Not used to select mode
	PFD_MAIN_PLANE,                 // Draw in main plane
	0,                              // Not used to select mode
	0,0,0
};
*/
	// MODEs LIST more info: https://www.opengl.org/discussion_boards/showthread.php/178106-Pixel-format-and-display-settings
	pixelFormat = ChoosePixelFormat(m_deviceContext, &pixelFormatDescriptor);
	if (!pixelFormat) { WomaFatalExceptionW (TEXT("Error on ChoosePixelFormat"));	return false; }
#endif

	// If the video card/display can handle our desired pixel format then we set it as the current one.
	result = SetPixelFormat(m_deviceContext, pixelFormat, &pixelFormatDescriptor);
	if(result != 1) { WomaFatalExceptionW (TEXT("Error setting SetPixelFormat"));	return false; }

#if defined old_school
	// Set the 4.0 version of OpenGL in the attribute list.
	int attributeList[] = {
			 //WGL_SAMPLES_ARB,  3,
			 WGL_CONTEXT_MAJOR_VERSION_ARB,  4,
			 WGL_CONTEXT_MINOR_VERSION_ARB,  0,
			 0,0 };

	// Create a OpenGL 4.0 rendering context.
	m_renderingContext1 = wglCreateContextAttribsARB(m_deviceContext, 0, attributeList);
	if(m_renderingContext1 == NULL)
	{
		return false;
	}
#else
	m_renderingContext1 = wglCreateContext(m_deviceContext);
    m_renderingContext2 = wglCreateContext(m_deviceContext);			// For multi-threading

	BOOL error=wglShareLists(m_renderingContext1, m_renderingContext2);	// For multi-threading
	if(error == FALSE)
	{
	DWORD errorCode=GetLastError();
	LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL);
		WOMA::WomaMessageBox( (TCHAR*)lpMsgBuf, TEXT("Error"));
		LocalFree(lpMsgBuf);
		//Destroy the GL context and just use 1 GL context
		wglDeleteContext(m_renderingContext2);
	}

    if (!gl3wIsSupported(3, 3))  
		{WOMA::WomaMessageBox(TEXT("Warning: OpenGL 3.3 not supported!\n")); }
#endif

	// Set the rendering context to active.
	result = wglMakeCurrent(m_deviceContext, m_renderingContext1);
	if(result != 1)
		{ WomaFatalExceptionW (TEXT("Error setting wglMakeCurrent"));	return false; }

	// Turn on or off the vertical sync depending on the input bool value.
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
#if defined old_school
	if(vsync)
		result = wglSwapIntervalEXT(1);
	else
		result = wglSwapIntervalEXT(0);
#else
	result = wglSwapIntervalEXT(vsync);	// SET VSYNC
#endif
	if(result != 1) // Check if vsync was set correctly.
		{ WomaFatalExceptionW (TEXT("Error on wglSwapIntervalEXT")); return false; }

	return true;
}

void wGLopenGLclass::Shutdown()
{
	// Reset to default:
	wglMakeCurrent(NULL, NULL);

	// Release the rendering context.
	if(m_renderingContext1)
	{
		wglDeleteContext(m_renderingContext1);
		m_renderingContext1 = NULL;
	}

	// Release the rendering context.
	if (m_renderingContext2)
	{
		wglDeleteContext(m_renderingContext2);
		m_renderingContext2 = NULL;
	}

	// Release the device context.
	if(m_deviceContext)
	{
		ReleaseDC(hWnd, m_deviceContext);
		m_deviceContext = NULL;
	}
}

void wGLopenGLclass::EndScene(UINT monitorWindow)
{
	// Present the back buffer to the screen since rendering is complete.
	ASSERT ( SwapBuffers(m_deviceContext) );
}

#endif
