// ----------------------------------------------------------------------------------------------
// Filename: openglclass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
//
// PURPOSE: 
//
//WomaDriverClass
//	GLopenGLclass
//		eGLopenGLclass.cpp EGL	OpenGL "WINDOWS"
//		glxOpenGLClass.cpp GLX	OpenGL "LINUX"
//		glesOpenGLClass.cpp		OpenGL ES 2.0 --> API "Android 2.2++"
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#if (defined OPENGL3 || defined OPENGL4) && DX_ENGINE_LEVEL >= 19 
#pragma warning(push)
#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition
#include "mem_leak.h"
#include "stateMachine.h"
#include "womadriverclass.h"
#include "OSengine.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"

#if defined ANDROID_PLATFORM
#if defined GLES2 && USE_GL_GLCOREARB
#include "GLES2/gl2ext.h"
#include "C:\WoMA3Dengine\ThirdParty\GL3Plus\GL\glcorearb.h"
#endif
#endif

GLmathClass* mathClass;

GLopenGLclass::GLopenGLclass()
{
	CLASSLOADER();
	

	mathClass = NULL;
	_tcscpy_s(driverName, TEXT("GL3+")); // driverName = TEXT ("GL3+");
	_tcscpy_s(szShaderModel, TEXT(""));
	
	gl_Camera = NULL;
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	gl_CameraSKY = NULL;
#endif
}

GLopenGLclass::~GLopenGLclass() { 
	Shutdown(); 
	CLASSDELETE(); 
}

void GLopenGLclass::Finalize() {} //not used on OPENGL

void GLopenGLclass::Shutdown2D() {}

void GLopenGLclass::Shutdown()
{
	SAFE_DELETE(mathClass);

	if(gl_Camera) { delete ((GLcameraClass*)gl_Camera); gl_Camera=NULL; }	//Cant use: SAFE_DELETE (m_Camera);
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	if (gl_CameraSKY) { delete ((GLcameraClass*)gl_CameraSKY); gl_CameraSKY = NULL; }	//Cant use: SAFE_DELETE (m_Camera);
#endif
	Shutdown2D();

#if defined USE_FRUSTRUM
	SAFE_DELETE (frustum);
#endif
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

void GLopenGLclass::RenderDriverText(void* pContext)
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

#if defined USE_FRUSTRUM
	frustum = NEW DXfrustumClass;	// Create Frustum
#endif

	return true; 
}

// -----------------------------------------------------------------
void GLopenGLclass::BeginScene(UINT monitorWindow)
{
	// Clear Screen: driver_ClearColor
	glClearColor(driver_ClearColor[0], driver_ClearColor[1], driver_ClearColor[2], driver_ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	// Clear the screen and depth buffer.
}

void GLopenGLclass::ClearDepthBuffer(void* pContext)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLopenGLclass::EndScene(UINT monitorWindow){} // Not implemented in MAIN DRIVER only in context Driver.

#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING
// MORE INFO: https://www.opengl.org/archives/resources/faq/technical/transparency.htm
void GLopenGLclass::TurnOnAlphaBlending(void* pContext)
{
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLopenGLclass::TurnOffAlphaBlending(void* pContext)
{
	glDisable (GL_BLEND);
}
#endif

// -----------------------------------------------------------------
void GLopenGLclass::TurnZBufferOn(void* pContext)
{
    if (g_Zbuffer) return;
	glEnable(GL_DEPTH_TEST);
    g_Zbuffer = true;
}

void GLopenGLclass::TurnZBufferOff(void* pContext)
{
    if (!g_Zbuffer) return;
	glDisable(GL_DEPTH_TEST);
    g_Zbuffer = false;
}

void GLopenGLclass::SetCamera2D()
{
	RenderfirstTime = true;

	GLcameraClass GLm_Camera;

	// Set Fixed Camera for 2D:
	GLm_Camera.SetRotation(0, 0, 0);			// NOTE: On 2D This values have always these values!
	GLm_Camera.SetPosition(0.0f, 0.0f, -1.0f);	// NOTE: On 2D This values have always these values!

	// Calculate: 2D ViewMatrix
	GLm_Camera.CalculateViewMatrix();						// ((OpenGLClass*)m_Driver)->m_Camera->Render(); || ((DX_CLASS*)m_Driver)->m_Camera->Render();

	//GLm_Camera.Use2DViewMatrix();				// Get the view from the camera and 2D objects.
	gl_Camera->m_viewmatrix2D.m[0]  = GLm_Camera.m_viewMatrix.m[0];
	gl_Camera->m_viewmatrix2D.m[1]  = GLm_Camera.m_viewMatrix.m[1];
	gl_Camera->m_viewmatrix2D.m[2]  = GLm_Camera.m_viewMatrix.m[2];
	gl_Camera->m_viewmatrix2D.m[3]  = GLm_Camera.m_viewMatrix.m[3];

	gl_Camera->m_viewmatrix2D.m[4]  = GLm_Camera.m_viewMatrix.m[4];
	gl_Camera->m_viewmatrix2D.m[5]  = GLm_Camera.m_viewMatrix.m[5];
	gl_Camera->m_viewmatrix2D.m[6]  = GLm_Camera.m_viewMatrix.m[6];
	gl_Camera->m_viewmatrix2D.m[7]  = GLm_Camera.m_viewMatrix.m[7];

	gl_Camera->m_viewmatrix2D.m[8]  = GLm_Camera.m_viewMatrix.m[8];
	gl_Camera->m_viewmatrix2D.m[9]  = GLm_Camera.m_viewMatrix.m[9];
	gl_Camera->m_viewmatrix2D.m[10] = GLm_Camera.m_viewMatrix.m[10];
	gl_Camera->m_viewmatrix2D.m[11] = GLm_Camera.m_viewMatrix.m[11];

	gl_Camera->m_viewmatrix2D.m[12] = GLm_Camera.m_viewMatrix.m[12];
	gl_Camera->m_viewmatrix2D.m[13] = GLm_Camera.m_viewMatrix.m[13];
	gl_Camera->m_viewmatrix2D.m[14] = GLm_Camera.m_viewMatrix.m[14];
	gl_Camera->m_viewmatrix2D.m[15] = GLm_Camera.m_viewMatrix.m[15];

}

// ------------------------------------------------------------------
void GLopenGLclass::Initialize3DCamera()
// ------------------------------------------------------------------
{
#if defined USE_VIEW2D_SPRITES // 26
	SetCamera2D();
#endif

	gl_Camera->SetPosition(	SystemHandle->AppSettings->INIT_CAMX, 
							SystemHandle->AppSettings->INIT_CAMY+0.35f,
							SystemHandle->AppSettings->INIT_CAMZ);

	gl_Camera->SetRotation(	SystemHandle->AppSettings->INIT_ROTX, 
							SystemHandle->AppSettings->INIT_ROTY,
							SystemHandle->AppSettings->INIT_ROTZ);

	gl_Camera->CalculateViewMatrix();

	// SETUP 3D Sky Camera:
#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28
	if (!gl_CameraSKY) {
		gl_CameraSKY = NEW GLcameraClass; // DX Implementation
		IF_NOT_THROW_EXCEPTION(gl_CameraSKY);
	}

	gl_CameraSKY->SetPosition(0.0f, 0.0f, 0.0f);
	gl_CameraSKY->SetRotation(	SystemHandle->AppSettings->INIT_ROTX, 
								SystemHandle->AppSettings->INIT_ROTY,
								SystemHandle->AppSettings->INIT_ROTZ);

	gl_CameraSKY->CalculateViewMatrix();
#endif
}




void GLopenGLclass::SetRasterizerState(void* pContext, UINT cullMode, UINT fillMode) 
{
	/*
	#define CULL_FRONT	0
	#define CULL_BACK	1
	#define CULL_NONE	2

	#define FILL_SOLID	0
	#define FILL_WIRE	1
	*/

#if defined ANDROID_PLATFORM
#define glPolygonMode glPolygonModeNV
#endif

#ifdef GL_SETRASTERIZERSTATE
	// DEFAULT: CULL_BACK, FILL_SOLID
	if (fillMode == FILL_WIRE)
		glPolygonMode(GL_FRONT_AND_BACK, FILL_SOLID);

	if (fillMode == GL_FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

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

	womalog ("GLopenGLclass::Initialize()\n");

	// Set default clear color:
	driver_ClearColor[0] = *clearColor++;
	driver_ClearColor[1] = *clearColor++;
	driver_ClearColor[2] = *clearColor++;
	driver_ClearColor[3] = *clearColor++;

	if (!gl_Camera)
	{
		gl_Camera = NEW GLcameraClass; // GL Implementation
		IF_NOT_THROW_EXCEPTION (gl_Camera);
	}

	Initialize3DCamera();

	// Init OpenGL:
#if !defined ANDROID_PLATFORM
	glViewport  ( 0, 0, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT); 
	//glClearColor(driver_ClearColor[0], driver_ClearColor[1], driver_ClearColor[2], driver_ClearColor[3]);
#endif

	// OpenGL settings:
	glFrontFace(GL_CW);	// Set the polygon winding to front facing for the left handed system.

	#if !defined ANDROID_PLATFORM
	glColorMask(1, 1, 1, 1);
	glDepthMask(GL_TRUE);
	#if defined LINUX_PLATFORM
	glClearDepth(1.0f);		// LINUX: Set the depth buffer to be entirely cleared to 1.0 values.
	#else
	glClearDepthf(1.0f);	// ANDROID/WINDOWS: Set the depth buffer to be entirely cleared to 1.0 values.
	#endif
	glDepthFunc(GL_LESS);
	#endif

	TurnZBufferOn(NULL);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the field of view and screen aspect ratio.
	float fieldOfView = 3.14159265358979323846f / 4.0f;
	float screenAspect = (float)SystemHandle->AppSettings->WINDOW_WIDTH / (float)SystemHandle->AppSettings->WINDOW_HEIGHT;

	// Build the perspective projection matrix.
	m_projectionMatrix = mathClass->BuildPerspectiveFovLHMatrix(fieldOfView, screenAspect, SystemHandle->AppSettings->SCREEN_NEAR, SystemHandle->AppSettings->SCREEN_DEPTH);

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

#pragma warning(pop)
#endif
