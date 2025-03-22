////////////////////////////////////////////////////////////////////////////////
// Filename: eGLopenGLclass.cpp
//
// Context Driver
//
////////////////////////////////////////////////////////////////////////////////

#include "platform.h"
#if DX_ENGINE_LEVEL >= 19 && defined ANDROID_PLATFORM // Initializing Engine 
#include "defs.h"
#include "GLopenGLclass.h"
#include "eGLopenGLclass.h"
#include "GLmathClass.h"

eGLopenGLclass::eGLopenGLclass()
{
	CLASSLOADER();
	WOMA_LOGManager_DebugMSG (TEXT("Render Driver: %s\n"), "eglOpenGL");
}

eGLopenGLclass::~eGLopenGLclass() {
	CLASSDELETE();
}

bool eGLopenGLclass::OnInit(int g_USE_MONITOR, void* hwnd, int screenWidth, int screenHeight, UINT depthBits,
		float screenDepth, float screenNear, BOOL msaa, bool vsync, BOOL fullscreen, BOOL g_UseDoubleBuffering, BOOL g_AllowResize)
{
/*
	struct engine* engine = (struct engine*) _engine;

    // Selects the first OpenGL configuration found.
	// -----------------------------------------------------------------------------------------------
    WOMA_LOGManager_DebugMSG("Selecting a display config.");

	EGLint lFormat, lNumConfigs, lErrorResult;

	// Defines display requirements.
    const EGLint lAttributes[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    if(!eglChooseConfig(mDisplay, lAttributes, &lConfig, 1, &lNumConfigs) || (lNumConfigs <= 0)) 
		return false;

    // Reconfigures the Android window with the EGL format.
    WOMA_LOGManager_DebugMSG("Configuring window format.");
	EGLConfig lConfig;
    if (!eglGetConfigAttrib(mDisplay, lConfig, EGL_NATIVE_VISUAL_ID, &lFormat)) 
		return false;

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        WomaFatalException("Unable to eglMakeCurrent"); //LOGW
        return false;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;
*/

	return true;
}

void eGLopenGLclass::BeginScene(UINT monitorWindow)
{
	_tprintf("BAD: %s", "BeginScene");
}

void eGLopenGLclass::ClearDepthBuffer()
{
	_tprintf("BAD: %s", "ClearDepthBuffer");
}

void eGLopenGLclass::Shutdown2D()
{
	_tprintf("BAD: %s", "Shutdown2D");
}

void eGLopenGLclass::Shutdown() //engine_term_display
{
	_tprintf("BAD: %s", "Shutdown");
/*
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
*/
}

void eGLopenGLclass::RenderDriverText()
{
	_tprintf("BAD: %s", "RenderDriverText");
}

void eGLopenGLclass::addText(int Xpos, int Ypos, TCHAR * printText, float R, float G, float B)
{
	_tprintf("BAD: %s", "addText");
}

void eGLopenGLclass::SetCamera2D()
{
	_tprintf("BAD: %s", "SetCamera2D");
}

void eGLopenGLclass::Initialize3DCamera()
{
	_tprintf("BAD: %s", "Initialize3DCamera");
}

bool eGLopenGLclass::Initialize(float * clearColor)
{
	_tprintf("BAD: %s", "Initialize");
	return false;
}

void eGLopenGLclass::Finalize()
{
	_tprintf("BAD: %s", "Finalize");
}

void eGLopenGLclass::TurnZBufferOn()
{
	_tprintf("BAD: %s", "TurnZBufferOn");
}

void eGLopenGLclass::TurnZBufferOff()
{
	_tprintf("BAD: %s", "TurnZBufferOff");
}

void eGLopenGLclass::EndScene(UINT monitorWindow)
{
	// Display it
	//LINUX:	glXSwapBuffers( Win.display, Win.win );
	//ANDROID:	eglSwapBuffers(mDisplay, mSurface);
	_tprintf("BAD: %s", "EndScene");
	return;
}

void eGLopenGLclass::SetRasterizerState(UINT cullMode, UINT fillMode)
{
	_tprintf("BAD: %s", "SetRasterizerState");
}

void eGLopenGLclass::TurnOnAlphaBlending()
{
	_tprintf("BAD: %s", "TurnOnAlphaBlending");
}

void eGLopenGLclass::TurnOffAlphaBlending()
{
	_tprintf("BAD: %s", "v");
}



void eGLopenGLclass::GetWorldMatrix(float *)
{
	_tprintf("BAD: %s", "GetWorldMatrix");
}

void eGLopenGLclass::GetProjectionMatrix(float *)
{
	_tprintf("BAD: %s", "GetProjectionMatrix");
}

void eGLopenGLclass::GetVideoCardInfo(char *)
{
	_tprintf("BAD: %s", "GetVideoCardInfo");
}

void eGLopenGLclass::BuildOrthoMatrix(mat4 * matrix, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool leftHand)
{
	_tprintf("BAD: %s", "BuildOrthoMatrix");
}

#endif