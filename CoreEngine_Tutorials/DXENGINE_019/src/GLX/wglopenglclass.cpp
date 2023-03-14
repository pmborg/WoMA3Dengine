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
//
// PURPOSE: 
//
// --------------------------------------------------------------------------------------------

#include "platform.h"

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "wGLopenGLclass.h"
#include "GLmathClass.h"
#include "log.h" //#include "logManager.h"

wGLopenGLclass::wGLopenGLclass()
{
	CLASS_LOAD_N--; 
	CLASSLOADER();  //dont count super class!

	m_deviceContext = 0;
	m_renderingContext1 = NULL;

	GLMajorVer = 0;
	GLMinorVer = 0;

    // double buffered, depth, color w/ alpha
	glutInitDisplayMode( GLUT_RIGHT_BUTTON | GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1, 1);
	
	int w = glutCreateWindow("OpenGL2003");
	HWND wh = FindWindow(NULL, TEXT("OpenGL2003"));
	ShowWindow(wh, SW_HIDE);

	int feedback = gl3wInit(); //Init GL3W
	if (feedback != -1) {
		glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVer);
		glGetIntegerv(GL_MINOR_VERSION, &GLMinorVer);
	}

	WOMA_LOGManager_DebugMSG ( "OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Release the temporary window
	glFinish();
	glutDestroyWindow( w );
	DestroyWindow(wh);

	WOMA_LOGManager_DebugMSG (TEXT("Render Driver: %s\n"), TEXT("wglOpenGL"));
	WOMA_LOGManager_DebugMSG (TEXT("Open GL version: %d.%d\n"), GLMajorVer, GLMinorVer);
}

wGLopenGLclass::~wGLopenGLclass() { 
	CLASSDELETE(); 
}

// Based: D:\ogre\v2-0\RenderSystems\GL\src\Win32\OgreWin32GLSupport.cpp
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
	
#ifdef old_school
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
	result = wglChoosePixelFormatARB(m_deviceContext, attributeListInt, NULL, 1, pixelFormat, &formatCount);
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
	//pixelFormatDescriptor.cStencilBits = 8;

	// NOTE: Using useDoubleBuffering="false" is not working!
	if (g_UseDoubleBuffering)
		pixelFormatDescriptor.dwFlags |= PFD_DOUBLEBUFFER;

/*
pixelFormat = (32bpp video mode)

Number of formats: 134
	1 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=OpenGL
	2 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=OpenGL
	3 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=OpenGL
	4 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=OpenGL
	5 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=OpenGL
	6 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=OpenGL
	7 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	8 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	9 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=OpenGL DoubleBuffer
	10 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=OpenGL DoubleBuffer
	11 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	12 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	13 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	14 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	15 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=OpenGL DoubleBuffer
	16 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=OpenGL DoubleBuffer
	17 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	18 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	19 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	20 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	21 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	22 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	23 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	24 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	25 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	26 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	27 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	28 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	29 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	30 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	31 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	32 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	33 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	34 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	35 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	36 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	37 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	38 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	39 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	40 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	41 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	42 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	43 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	44 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	45 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	46 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	47 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	48 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	49 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	50 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	51 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	52 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	53 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	54 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	55 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	56 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	57 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	58 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	59 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	60 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	61 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	62 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	63 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	64 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	65 Colors=32 ARGB=0 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	66 Colors=32 ARGB=8 8 8 8 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	67 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	68 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	69 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	70 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	71 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	72 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	73 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	74 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	75 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	76 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	77 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	78 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	79 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	80 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	81 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	82 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	83 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	84 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	85 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	86 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	87 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	88 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	89 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	90 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	91 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	92 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	93 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	94 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	95 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	96 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	97 Colors=32 ARGB=0 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	98 Colors=32 ARGB=8 8 8 8 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	99 Colors=32 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	100 Colors=32 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	101 Colors=32 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	102 Colors=32 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	103 Colors=32 ARGB=8 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	104 Colors=32 ARGB=8 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	105 Colors=32 ARGB=8 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	106 Colors=32 ARGB=8 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	107 Colors=32 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	108 Colors=32 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	109 Colors=32 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	110 Colors=32 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	111 Colors=24 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	112 Colors=24 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	113 Colors=24 ARGB=8 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	114 Colors=24 ARGB=8 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	115 Colors=24 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	116 Colors=24 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	117 Colors=16 ARGB=0 5 5 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	118 Colors=16 ARGB=0 5 5 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	119 Colors=16 ARGB=8 5 5 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	120 Colors=16 ARGB=8 5 5 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	121 Colors=16 ARGB=0 5 5 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	122 Colors=16 ARGB=0 5 5 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	123 Colors=8 ARGB=0 3 3 2 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	124 Colors=8 ARGB=0 3 3 2 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	125 Colors=8 ARGB=8 3 3 2 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	126 Colors=8 ARGB=8 3 3 2 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	127 Colors=8 ARGB=0 3 3 2 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	128 Colors=8 ARGB=0 3 3 2 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	129 Colors=4 ARGB=0 1 1 1 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	130 Colors=4 ARGB=0 1 1 1 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	131 Colors=4 ARGB=8 1 1 1 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	132 Colors=4 ARGB=8 1 1 1 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	133 Colors=4 ARGB=0 1 1 1 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	134 Colors=4 ARGB=0 1 1 1 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
*/

/*
pixelFormat = (16bpp video mode)

Number of formats: 96
	1 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=OpenGL
	2 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=OpenGL
	3 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=OpenGL
	4 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=OpenGL
	5 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	6 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	7 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=OpenGL DoubleBuffer
	8 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	9 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	10 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	11 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=OpenGL DoubleBuffer
	12 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=OpenGL DoubleBuffer
	13 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	14 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	15 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	16 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	17 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	18 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	19 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	20 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	21 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	22 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	23 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	24 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	25 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	26 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	27 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	28 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	29 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	30 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	31 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	32 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	33 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	34 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	35 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	36 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	37 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	38 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	39 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	40 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	41 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=0 Aux=4 Flags=DoubleBuffer
	42 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	43 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	44 Colors=16 ARGB=0 5 6 5 Depth=0 Stencil=0 Aux=4 Flags=DoubleBuffer
	45 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	46 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	47 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	48 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	49 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	50 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	51 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	52 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	53 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	54 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	55 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	56 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	57 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	58 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	59 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=0 Aux=4 Flags=DoubleBuffer
	60 Colors=16 ARGB=0 5 6 5 Depth=24 Stencil=8 Aux=4 Flags=DoubleBuffer
	61 Colors=16 ARGB=0 5 6 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	62 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	63 Colors=16 ARGB=0 5 6 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	64 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	65 Colors=16 ARGB=8 5 6 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	66 Colors=16 ARGB=8 5 6 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	67 Colors=16 ARGB=8 5 6 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	68 Colors=16 ARGB=8 5 6 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	69 Colors=16 ARGB=0 5 6 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	70 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	71 Colors=16 ARGB=0 5 6 5 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	72 Colors=16 ARGB=0 5 6 5 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic DoubleBuffer
	73 Colors=24 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	74 Colors=24 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	75 Colors=24 ARGB=8 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	76 Colors=24 ARGB=8 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	77 Colors=24 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	78 Colors=24 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	79 Colors=32 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	80 Colors=32 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	81 Colors=32 ARGB=8 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	82 Colors=32 ARGB=8 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	83 Colors=32 ARGB=0 8 8 8 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	84 Colors=32 ARGB=0 8 8 8 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	85 Colors=8 ARGB=0 3 3 2 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	86 Colors=8 ARGB=0 3 3 2 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	87 Colors=8 ARGB=8 3 3 2 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	88 Colors=8 ARGB=8 3 3 2 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	89 Colors=8 ARGB=0 3 3 2 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	90 Colors=8 ARGB=0 3 3 2 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	91 Colors=4 ARGB=0 1 1 1 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	92 Colors=4 ARGB=0 1 1 1 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	93 Colors=4 ARGB=8 1 1 1 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	94 Colors=4 ARGB=8 1 1 1 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
	95 Colors=4 ARGB=0 1 1 1 Depth=32 Stencil=8 Aux=0 Flags=OpenGL Generic
	96 Colors=4 ARGB=0 1 1 1 Depth=16 Stencil=8 Aux=0 Flags=OpenGL Generic
*/

	// MODEs LIST more info: https://www.opengl.org/discussion_boards/showthread.php/178106-Pixel-format-and-display-settings

	pixelFormat = ChoosePixelFormat(m_deviceContext, &pixelFormatDescriptor);
	if (!pixelFormat) { WomaFatalExceptionW (TEXT("Error on ChoosePixelFormat"));	return false; }
#endif

	// If the video card/display can handle our desired pixel format then we set it as the current one.
	result = SetPixelFormat(m_deviceContext, pixelFormat, &pixelFormatDescriptor);
	if(result != 1) { WomaFatalExceptionW (TEXT("Error setting SetPixelFormat"));	return false; }

#ifdef old_school
	// Set the 4.0 version of OpenGL in the attribute list.
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;

	// Null terminate the attribute list.
	attributeList[4] = 0;

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
#ifdef old_school
	if(vsync)
		result = wglSwapIntervalEXT(1);
	else
		result = wglSwapIntervalEXT(0);
#else
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
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

