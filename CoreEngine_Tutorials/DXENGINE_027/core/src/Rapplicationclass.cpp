////////////////////////////////////////////////////////////////////////////////
// Filename: RApplicationClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
#include "woma_macros.h"

#if !defined ANDROID_PLATFORM
extern MyWin Win;
#endif

#define _tprintf printf

RApplicationClass::RApplicationClass()
{
    m_OpenGL = 0;
    m_Camera = 0;
    m_FontShader = 0;
    m_Font = 0;
    m_TextString1 = 0;
}

RApplicationClass::~RApplicationClass()
{
}
#if !defined ANDROID_PLATFORM
bool RApplicationClass::Initialize(/*Display* display, Window win,*/ int screenWidth, int screenHeight)
{
    // Create and initialize the OpenGL object.
    m_OpenGL = new OpenGLClass;
	_tprintf("m_OpenGL->Initialize()\n");
    IF_NOT_RETURN_FALSE (m_OpenGL->Initialize(/*display, win,*/ screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED));

    // Create and initialize the camera object.
    m_Camera = new RCameraClass;
	_tprintf("new RCameraClass\n");
    m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
    m_Camera->Render();

    // Create and initialize the font shader object.
    m_FontShader = new RFontShaderClass;
	
	_tprintf("m_FontShader->Initialize(m_OpenGL)\n");
    IF_NOT_RETURN_FALSE(m_FontShader->Initialize(m_OpenGL));

    // Create and initialize the font object.
    m_Font = new RFontClass;
	_tprintf("m_Font->Initialize(m_OpenGL, 1)\n");
    IF_NOT_RETURN_FALSE(m_Font->Initialize(m_OpenGL, 0));


    return true;
}
#else
bool RApplicationClass::Initialize(int screenWidth, int screenHeight)
{
    // Create and initialize the OpenGL object.
    m_OpenGL = new OpenGLClass;
    _tprintf("m_OpenGL->Initialize()\n");
#if !defined ANDROID_PLATFORM
    IF_NOT_RETURN_FALSE(m_OpenGL->Initialize(display, win, screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED));
#else
    IF_NOT_RETURN_FALSE(m_OpenGL->Initialize(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED));
#endif
    // Create and initialize the camera object.
    m_Camera = new RCameraClass;
    _tprintf("new RCameraClass\n");
    m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
    m_Camera->Render();

    // Create and initialize the font shader object.
    m_FontShader = new RFontShaderClass;

    _tprintf("m_FontShader->Initialize(m_OpenGL)\n");
    IF_NOT_RETURN_FALSE(m_FontShader->Initialize(m_OpenGL));

    // Create and initialize the font object.
    m_Font = new RFontClass;
    _tprintf("m_Font->Initialize(m_OpenGL, 1)\n");
    IF_NOT_RETURN_FALSE(m_Font->Initialize(m_OpenGL, 0));


    return true;
}
#endif
void RApplicationClass::Shutdown()
{
    SAFE_SHUTDOWN(m_Font);
    SAFE_SHUTDOWN(m_FontShader);
    SAFE_DELETE(m_Camera);
    SAFE_DELETE(m_OpenGL);

    return;
}

bool RApplicationClass::Frame(RInputClass* Input)
{
    // Render the graphics scene.
    IF_NOT_RETURN_FALSE (Render());

    return true;
}

bool RApplicationClass::Render()
{
    float worldMatrix[16], viewMatrix[16], orthoMatrix[16];
    float pixelColor[4];
    bool result;

    char FPSString[MAX_STR_LEN/4];//, testString2[32];
    // Set the strings we want to display.
    StringCchPrintf(FPSString, sizeof(FPSString), TEXT("Fps: %d  - ms: %4.2f"), SystemHandle->fps, 1000.0f / SystemHandle->fps);
    //strcpy(testString1, "Hello");

    // Create and initialize the first text object.
    m_TextString1 = new RTextClass;
    //_tprintf("m_TextString1->Initialize()\n");
    IF_NOT_RETURN_FALSE(m_TextString1->Initialize(m_OpenGL, SystemHandle->AppSettings->WINDOW_WIDTH, SystemHandle->AppSettings->WINDOW_HEIGHT, MAX_STR_LEN/4, m_Font, FPSString,
                        SystemHandle->AppSettings->WINDOW_WIDTH - 180, 40,             // Position
                        0.0f, 1.0f, 0.0f)); // Color

	// Clear the buffers to begin the scene.
    //m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Get the world, view, and ortho matrices from the opengl and camera objects.
    m_OpenGL->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_OpenGL->GetOrthoMatrix(orthoMatrix);

#if future
	switch (projection)
	{
		case PROJECTION_PERSPECTIVE:
			 projectionMatrix = (driver->m_projectionMatrix);
			break;

		#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
		case PROJECTION_ORTHOGRAPH:
			projectionMatrix = driver->m_orthoMatrix;
		break;
		#endif
	}

	switch (camera)
	{
		case CAMERA_NORMAL:
			if (projection == PROJECTION_PERSPECTIVE)
			{
				//PROJECTION_PERSPECTIVE
				m_viewMatrix = ((GLcameraClass*)driver->gl_Camera)->m_viewMatrix;
			}
			else
			{	
				//PROJECTION_ORTHOGRAPH:
				m_viewMatrix = m_viewMatrix;
				m_viewMatrix.mat4identity();
				m_viewMatrix.m[14] = 1;
			}
			break;

		#if defined USE_SKY_DOME && DX_ENGINE_LEVEL >= 28
		case CAMERA_SKY:
			m_viewMatrix = ((GLcameraClass*)driver->gl_CameraSKY)->m_viewMatrix;
			break;
		#endif
	}
#endif

    // Disable the Z buffer and enable alpha blending for 2D rendering.
    //m_OpenGL->TurnZBufferOff();
    //m_OpenGL->EnableAlphaBlending();

    // Get the color to render the text as.
	m_TextString1->GetPixelColor(pixelColor);

	// Set the font shader as active and set its parameters.
    IF_NOT_RETURN_FALSE (m_FontShader->SetShaderParameters(worldMatrix, viewMatrix, orthoMatrix, pixelColor));

	// Set the font texture as the active texture.
	m_Font->SetTexture();

    // Render the first text string using the font shader.
    m_TextString1->Render();

    // Enable the Z buffer and disable alpha blending now that 2D rendering is complete.
    //m_OpenGL->TurnZBufferOn();
    //m_OpenGL->DisableAlphaBlending();

    SAFE_SHUTDOWN(m_TextString1);

    // Present the rendered scene to the screen.
    //m_OpenGL->EndScene();

    return true;
}

#endif