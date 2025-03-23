// --------------------------------------------------------------------------------------------
// Filename: eGLopenGLclass.h
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
#pragma once

#include "main.h"
#if (defined GLES2 || defined GLES3)  && defined ANDROID_PLATFORM && DX_ENGINE_LEVEL >= 19 // Initializing Engine

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <EGL/egl.h>

//////////////
// INCLUDES //
//////////////
#include <math.h>

#include "GLmathClass.h"
#include "womadriverclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GLopenGLclass
////////////////////////////////////////////////////////////////////////////////

class eGLopenGLclass : public WomaDriverClass
{
public:
	eGLopenGLclass();
	~eGLopenGLclass();

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

	bool OnInit(int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits,
		float screenDepth, float screenNear, BOOL msaa, bool vsync,
		BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize);

	void BeginScene(UINT monitorWindow);
	void ClearDepthBuffer();
	void EndScene(UINT monitorWindow);

	void SetRasterizerState(UINT cullMode, UINT fillMode);

#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING //|| ENGINE_LEVEL >= 26
	//We have two new functions for turning on and off alpha blending:
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
#endif

	void GetWorldMatrix(float*);
	void GetProjectionMatrix(float*);
	void GetVideoCardInfo(char*);
	void BuildOrthoMatrix(mat4* matrix, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool leftHand);

#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
	ImageLoaderClass* CaptureScreenShot(int screenWidth, int screenHeight);
#endif


	// Video Card Capabilities:
	// ----------------------------------------------------------------------------
	NativeWindowType displayWindow;
	EGLContext eglContext;

private:
	bool LoadExtensionList();
	EGLSurface mSurface;

public:
};

#endif