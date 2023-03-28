// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: wGLopenGLclass.h
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
#pragma once

#include "platform.h"

/////////////
// LINKING //
/////////////
// Download Python 2.7.3:		http://www.python.org/download/releases/2.7.3/
// Download gl3w.cpp Generator: https://github.com/skaslev/gl3w
// Run:							c:\Python27\python.exe gl3w_gen.py

#include <GL/gl3w.h>				// Source: Generated (Windows / Apple / Linux)
#pragma comment(lib, "opengl32.lib")// GL3W lib dependency: Interface to Windows

// NOTE:
// glew.h its for >= GL2.1 & < GL3.3 ~DX9
// gl3w.h its for >= GL3.3 ~DX10
//
// OpenGL 3.3: It was designed to target hardware capable of supporting Direct3D 10.
// OpenGL 4.0: It was designed to target hardware capable of supporting Direct3D 11.

// GLUT - OpenGL Utility Toolkit: Its the DXSDK for OPENGL
#include <GL/glut.h>	// Used for Check():	http://freeglut.sourceforge.net/index.php#download

// Windows loader for all Extensions:
#include <GL/wglext.h>				// Ver 4.3, download from : http://www.opengl.org/registry/


//////////////
// INCLUDES //
//////////////
#include <GL/gl.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: GLopenGLclass
////////////////////////////////////////////////////////////////////////////////
class wGLopenGLclass : public GLopenGLclass
{
public:
	UINT WomaIntegrityCheck = 1234567890;
	wGLopenGLclass();
	~wGLopenGLclass();

	bool OnInit(	int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits, 
						float screenDepth, float screenNear, BOOL msaa, bool vsync, 
						BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize);
	void Shutdown();
	void EndScene(UINT monitorWindow);

	//void GetWorldMatrix(float*);
	//void GetProjectionMatrix(float*);
	//void GetVideoCardInfo(char*);

	bool	LoadExtensionList();
	HDC		m_deviceContext=NULL;
	HGLRC	m_renderingContext1 = NULL;
	HGLRC	m_renderingContext2 = NULL;

private:
	HWND	hWnd = NULL;

public:
};

