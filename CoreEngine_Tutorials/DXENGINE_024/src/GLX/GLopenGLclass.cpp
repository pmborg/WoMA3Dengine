// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// ----------------------------------------------------------------------------------------------
// Filename: openglclass.cpp
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

/*
WomaDriverClass
	GLopenGLclass
		eGLopenGLclass.cpp EGL OpenGL "WINDOWS"
		glxOpenGLClass.cpp GLX OpenGL "LINUX"

		OpenGL ES 2.0 --> API Android 2.2++
*/

#include "platform.h"
#include "mem_leak.h"
#include "stateMachine.h"
#include "womadriverclass.h"
#include "OSengine.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"

GLmathClass* mathClass;

GLopenGLclass::GLopenGLclass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567890;

	mathClass = NULL;
	_tcscpy_s(driverName, TEXT("GL3+")); // driverName = TEXT ("GL3+");
	_tcscpy_s(szShaderModel, TEXT(""));
	
	m_Camera = NULL;
}

GLopenGLclass::~GLopenGLclass() { Shutdown(); CLASSDELETE(); }

void GLopenGLclass::Finalize() {} //not used on OPENGL

void GLopenGLclass::Shutdown2D() {}

void GLopenGLclass::Shutdown()
{
	SAFE_DELETE(mathClass);

	if(m_Camera) { delete ((GLcameraClass*)m_Camera); m_Camera=NULL; }	//Cant use: SAFE_DELETE (m_Camera);

	Shutdown2D();
}

#if zero
void GLopenGLclass::GetWorldMatrix(float* matrix)
{
	memcpy (matrix, m_worldMatrix, 16*sizeof(float));
	return;
}
#endif

void GLopenGLclass::addText(int Xpos, int Ypos, TCHAR* printText, float R, float G, float B)
{
}

void GLopenGLclass::RenderDriverText()
{
}

void GLopenGLclass::GetProjectionMatrix(float* matrix)
{
	matrix = (float*) &m_projectionMatrix;	//memcpy (matrix, m_projectionMatrix, 16*sizeof(float));
}


void GLopenGLclass::GetVideoCardInfo(char* cardName)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
}

void GLopenGLclass::BuildOrthoMatrix(mat4* matrix, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool leftHand)
{
	if (leftHand == true)
	{
		matrix->m[0] = 2.0f / screenWidth;
		matrix->m[1] = 0.0f;
		matrix->m[2] = 0.0f;
		matrix->m[3] = 0.0f;

		matrix->m[4] = 0.0f;
		matrix->m[5] = 2.0f / screenHeight;
		matrix->m[6] = 0.0f;
		matrix->m[7] = 0.0f;

		matrix->m[8] = 0.0f;
		matrix->m[9] = 0.0f;
		matrix->m[10] = 1.0f / (screenDepth - screenNear);
		matrix->m[11] = 0.0f;

		matrix->m[12] = 0.0f;
		matrix->m[13] = 0.0f;
		matrix->m[14] = screenNear / (screenNear - screenDepth);
		matrix->m[15] = 1.0f;
	}
	else
	{
		matrix->m[0] = 2.0f / screenWidth;
		matrix->m[1] = 0.0f;
		matrix->m[2] = 0.0f;
		matrix->m[3] = 0.0f;

		matrix->m[4] = 0.0f;
		matrix->m[5] = 2.0f / screenHeight;
		matrix->m[6] = 0.0f;
		matrix->m[7] = 0.0f;

		matrix->m[8] = 0.0f;
		matrix->m[9] = 0.0f;
		matrix->m[10] = 1.0f / (screenNear - screenDepth);
		matrix->m[11] = 0.0f;

		matrix->m[12] = 0.0f;
		matrix->m[13] = 0.0f;
		matrix->m[14] = screenNear / (screenNear - screenDepth);
		matrix->m[15] = 1.0f;
	}

	return;
}
//NOT USED: But need to be declared!
bool GLopenGLclass::OnInit(int _USE_MONITOR, /*HWND*/void* hwnd, int screenWidth, int screenHeight, UINT depthBits, float screenDepth, float screenNear, 
								BOOL msaa, bool vsync, BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize)  { 

	BuildOrthoMatrix(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth, true /*leftHand*/);

	return true; 
}

// -----------------------------------------------------------------
void GLopenGLclass::BeginScene(UINT monitorWindow)
{
	// Clear Screen - with "ClearColor" defined: GLopenGLclass::Initialize
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	// Clear the screen and depth buffer.
}

void GLopenGLclass::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLopenGLclass::EndScene(UINT monitorWindow){} // Not implemented in MAIN DRIVER only in context Driver.

// MORE INFO: https://www.opengl.org/archives/resources/faq/technical/transparency.htm
void GLopenGLclass::TurnOnAlphaBlending()
{
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLopenGLclass::TurnOffAlphaBlending()
{
	glDisable (GL_BLEND);
}

//static bool g_Zbuffer = false;
// -----------------------------------------------------------------
void GLopenGLclass::TurnZBufferOn()
{
    if (g_Zbuffer) return;
	glEnable(GL_DEPTH_TEST);
    g_Zbuffer = true;
}

void GLopenGLclass::TurnZBufferOff()
{
    if (!g_Zbuffer) return;
	glDisable(GL_DEPTH_TEST);
    g_Zbuffer = false;
}

// ------------------------------------------------------------------
void GLopenGLclass::Initialize3DCamera()
// ------------------------------------------------------------------
{
	SetCamera2D();

	m_Camera->SetPosition(	SystemHandle->AppSettings->INIT_CAMX, SystemHandle->AppSettings->INIT_CAMY+0.35f, 
							SystemHandle->AppSettings->INIT_CAMZ);

	m_Camera->SetRotation(	SystemHandle->AppSettings->INIT_ROTX, SystemHandle->AppSettings->INIT_ROTY, 
							SystemHandle->AppSettings->INIT_ROTZ);

	m_Camera->Render();
}

void GLopenGLclass::SetCamera2D()
{
	RenderfirstTime = true;

}


void GLopenGLclass::SetRasterizerState(UINT cullMode, UINT fillMode) 
{
	/*
	#define CULL_FRONT	0
	#define CULL_BACK	1
	#define CULL_NONE	2

	#define FILL_SOLID	0
	#define FILL_WIRE	1
	*/

	// DEFAULT: CULL_BACK, FILL_SOLID
	if (fillMode == FILL_WIRE)
		glPolygonMode(GL_FRONT_AND_BACK, FILL_SOLID);

	if (fillMode == GL_FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (cullMode != CULL_NONE)
	{
		// Enable back face culling.
		glEnable(GL_CULL_FACE);

		if (cullMode == CULL_FRONT)
			glCullFace(GL_FRONT);

		if (cullMode == CULL_BACK)
			glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
		glCullFace(GL_NONE);
	}
}

// ------------------------------------------------------------------
bool GLopenGLclass::Initialize(float* clearColor)
// ------------------------------------------------------------------
{
	mathClass = NEW GLmathClass;
	IF_NOT_THROW_EXCEPTION (mathClass);

	WOMA_LOGManager_DebugMSG ("GLopenGLclass::Initialize()\n");

	// Set default clear color:
	ClearColor[0] = *clearColor++;
	ClearColor[1] = *clearColor++;
	ClearColor[2] = *clearColor++;
	ClearColor[3] = *clearColor++;

	if (!m_Camera) 
	{
		m_Camera = NEW GLcameraClass; // GL Implementation
		IF_NOT_THROW_EXCEPTION (m_Camera);
	}

	Initialize3DCamera();

	// Init OpenGL:
	glViewport  ( 0, 0, SystemHandle->AppSettings->WINDOW_WIDTH, 
						SystemHandle->AppSettings->WINDOW_HEIGHT); 

	// OpenGL settings:
	glFrontFace(GL_CW);	// Set the polygon winding to front facing for the left handed system.

	glColorMask ( 1,1,1,1 );
	glDepthMask(GL_TRUE);

	glClearDepth(1.0f);	// Set the depth buffer to be entirely cleared to 1.0 values.
	glDepthFunc(GL_LESS);
	TurnZBufferOn();

	/*
	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	*/

	glClearColor(ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3]);

	// Set the field of view and screen aspect ratio.
	float fieldOfView = 3.14159265358979323846f / 4.0f;
	float screenAspect = (float)SystemHandle->AppSettings->WINDOW_WIDTH / (float)SystemHandle->AppSettings->WINDOW_HEIGHT;

	// Build the perspective projection matrix.
	//mathClass->BuildPerspectiveFovLHMatrix(m_projectionMatrix, fieldOfView, screenAspect, SystemHandle->AppSettings->SCREEN_NEAR, SystemHandle->AppSettings->SCREEN_DEPTH);
	m_projectionMatrix = 
		mathClass->BuildPerspectiveFovLHMatrix(fieldOfView, screenAspect, SystemHandle->AppSettings->SCREEN_NEAR, SystemHandle->AppSettings->SCREEN_DEPTH);

	return true;
}

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
// ----------------------------------------------------------------------------------------------
ImageLoaderClass* GLopenGLclass::CaptureScreenShot(int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	return false;
}
#endif

