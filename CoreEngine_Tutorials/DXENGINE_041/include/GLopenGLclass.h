// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: GLopenGLclass.h
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
// PURPOSE: Define OPEN GL engine interfaces
// --------------------------------------------------------------------------------------------
#pragma once

#ifndef GLopenGLclass_h_
#define GLopenGLclass_h_

#include "platform.h"
#if defined OPENGL3 && DX_ENGINE_LEVEL >= 19

//////////////
// INCLUDES //
//////////////

//-----------------------------------------------------------------------------
// This file was automatic generated: 
// ==================================
// more info:						https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library#GL3W
// 
// The GL3W library focuses on the core profile of OpenGL 3 and 4. It only loads the core entrypoints for these OpenGL versions.It supports Windows, Mac OS X, Linux, and FreeBSD.
// Note: GL3W loads core OpenGL only by default.All OpenGL extensions will be loaded if the --ext flag is specified to gl3w_gen.py.
//
// Download Python 2.7.3 or newer:	http://www.python.org/download/releases/2.7.3/
// Download gl3w.cpp Generator:		https://github.com/skaslev/gl3w
// Run:								c:\Python27\python.exe gl3w_gen.py
// or								python.exe --ext gl3w_gen.py
// 
// generates:						gl3w.h and gl3w.c
// Source:							Generated support for (Windows / Apple / Linux)
// 

#include <GL/gl3w.h>				//Note that GL/gl3w.h must be included before any other OpenGL

/////////////
// LINKING //
/////////////
//dll(32 bits) : C:\Windows\SysWOW64\opengl32.dll
//dll(64 bits) : C:\Windows\System32\opengl32.dll
//Kits do Windows\10\Lib\<10.0.17763.0>\um\x86\opengl32.lib
//Kits do Windows\10\Lib\<10.0.17763.0>\um\x64\opengl32.lib
#pragma comment(lib, "opengl32.lib")// GL3W lib dependency: Interface to Windows

// NOTE:
// glew.h its for < GL3.3 ~DX9		It was designed to target hardware capable of supporting Direct3D 10.
// gl3w.h its for >= GL3.3 ~DX10	It was designed to target hardware capable of supporting Direct3D 11.

// GLUT - OpenGL Utility Toolkit: Its the DXSDK for OPENGL
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>	// Used for Check():	http://freeglut.sourceforge.net/index.php#download
#endif

// Windows context loader for all Extensions:
#include <GL/wglext.h>				// Ver 4.3, download from : http://www.opengl.org/registry/

	#include "GLcameraClass.h"

#include "womadriverclass.h"	//woma

////////////////////////////////////////////////////////////////////////////////
// Class name: GLopenGLclass
////////////////////////////////////////////////////////////////////////////////
class GLopenGLclass : public WomaDriverClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	GLopenGLclass();
	~GLopenGLclass();
	void Shutdown2D();
	void Shutdown();

	void RenderDriverText();
	void addText(int Xpos, int Ypos, TCHAR* printText, float R, float G, float B);

	void SetCamera2D();
	void Initialize3DCamera();
	bool Initialize(float* clearColor);
	void Finalize();

	void TurnZBufferOn();
	void TurnZBufferOff();

	bool OnInit(	int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits,
					float screenDepth, float screenNear, BOOL msaa, bool vsync, 
					BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize);
	
	void BeginScene(UINT monitorWindow);
	void ClearDepthBuffer();
	void EndScene(UINT monitorWindow);

	void SetRasterizerState(UINT cullMode, UINT fillMode);

	//We have two new functions for turning on and off alpha blending:
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void GetWorldMatrix(float*);
	void GetProjectionMatrix(float*);
	void GetVideoCardInfo(char*);
	void BuildOrthoMatrix(mat4* matrix, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool leftHand);

public:
	mat4 m_projectionMatrix;
	mat4 m_orthoMatrix;

	int GLMajorVer, GLMinorVer;

	GLcameraClass* gl_Camera = NULL;

	GLcameraClass* gl_CameraSKY = NULL;

private:
	HDC m_deviceContext;
	char m_videoCardDescription[128];
};

#endif
#endif
