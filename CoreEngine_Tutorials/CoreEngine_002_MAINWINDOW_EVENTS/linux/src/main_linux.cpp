// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
#include "main.h"
#include "OSengine.h"

#if defined __GNUC__
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

#include "linux.h"
#include "OSengine.h"
#include "OSmain_dir.h"

#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <X11/Xatom.h>

#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

//----------------------------------------------------------------------------
//const int WIN_XPOS = 256;
//const int WIN_YPOS = 20;
const int NUM_SAMPLES = 4;

//----------------------------------------------------------------------------
MyWin Win;
double elapsedMsec( const struct timeval &start, const struct timeval &stop );
double elapsedUsec( const struct timeval &start, const struct timeval &stop );
GLXFBConfig main_chooseFBConfig(Display* display, int screen);


void linux_window_title()
{
}

bool createWindow()
{
	//----------------------------------------------------------------------------
	//bool LinuxSystemClass::ApplicationInitMainWindow(void* OpenGL)
	// 
	// Init globals:
	Win.width = SystemHandle->AppSettings->WINDOW_WIDTH;	// screenWidth;
	Win.height = SystemHandle->AppSettings->WINDOW_HEIGHT;	//screenHeight;
	Win.ready = false;										// No Render yet!
	Screen* defaultScreen;

	// [1] Open a connection to the X server on the local computer.
	Display* display = Win.display = XOpenDisplay(NULL);
	if (!display) {
		WOMA_LOGManager_DebugMSG("Cannot open X display\n");
		return false;
	}
	if (!glXQueryExtension(display, 0, 0)) {
		WOMA_LOGManager_DebugMSG("X Server doesn't support GLX extension\n");
		return false;
	}

	// [2] Get a handle to the root window.
	int screen = DefaultScreen(display);
	//Window root_win = RootWindow(display, screen);
	Window root_win = DefaultRootWindow(display);

	//XSelectInput(display, root_win, ButtonPressMask);

	// Default template
	static const int Visual_attribs[] =
	{
		GLX_X_RENDERABLE    , GL_TRUE,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RGBA			, GL_TRUE,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , GL_TRUE,
		GLX_SAMPLE_BUFFERS  , 1,
		GLX_SAMPLES         , NUM_SAMPLES,
		None
	};

	int attribs[MAX_STR_LEN];
	memcpy(attribs, Visual_attribs, sizeof(Visual_attribs));

	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(display, screen, attribs, &fbcount);
	GLXFBConfig fbconfig;
	if (fbc)
	{
		if (fbcount >= 1)
			fbconfig = fbc[0];

		XFree(fbc);
	}

	//----------------------------------------------------------------------------
	Visual* visual;
	XVisualInfo* visinfo = glXGetVisualFromFBConfig(display, fbconfig);
	if (!visinfo)
	{
		WOMA_LOGManager_DebugMSG("Failed to get XVisualInfo\n");
		return false;
	}
	WOMA_LOGManager_DebugMSG("X Visual ID = 0x%.2x\n", int(visinfo->visualid));

	//----------------------------------------------------------------------------
	XSetWindowAttributes winAttr;

	// [3] Create a color map for the window for the specified visual type.
	winAttr.colormap = XCreateColormap(display, root_win, visinfo->visual, AllocNone);
	winAttr.event_mask = StructureNotifyMask | KeyPressMask;
	winAttr.background_pixmap = None;
	winAttr.background_pixel = 0;
	winAttr.border_pixel = 0;
	winAttr.override_redirect = 1;

	unsigned int mask = CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask;

	// [4] Get the size of the default screen.
	if (SystemHandle->AppSettings->FULL_SCREEN)
	{
		defaultScreen = XDefaultScreenOfDisplay(display);
		Win.width = XWidthOfScreen(defaultScreen);
		Win.height = XHeightOfScreen(defaultScreen);
	}

	// [5] Create the X window with the desired settings.
	Window win = Win.window = XCreateWindow(display, root_win,
		WOMA::settings.WINDOW_Xpos, WOMA::settings.WINDOW_Ypos,
		Win.width,
		Win.height, 
		0,
		visinfo->depth, InputOutput,
		visinfo->visual, mask, &winAttr);

	// -----------------------------------------------------------------------------------------------
	// [6] Map the newly created regular window to the video display.
	XMapWindow(display, win);

	// [7]  Set the name of the window.
	linux_window_title();

	int state;
	XGetInputFocus(display, &root_win, &state);
	XSelectInput(display, win, KeyPressMask | ButtonPressMask /* | PointerMotionMask*/);

	// [8] If full screen mode then we send the full screen event and remove the border decoration.
	Atom wmState, fullScreenState, motifHints;
	long motifHintList[5];
	XEvent fullScreenEvent;
	if (SystemHandle->AppSettings->FULL_SCREEN)
	{
		// Setup the full screen states. 
		wmState = XInternAtom(display, "_NET_WM_STATE", False);
		fullScreenState = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

		// Setup the X11 event message that we need to send to make the screen go full screen mode.
		memset(&fullScreenEvent, 0, sizeof(fullScreenEvent));

		fullScreenEvent.type = ClientMessage;
		fullScreenEvent.xclient.window = win;
		fullScreenEvent.xclient.message_type = wmState;
		fullScreenEvent.xclient.format = 32;
		fullScreenEvent.xclient.data.l[0] = 1;
		fullScreenEvent.xclient.data.l[1] = fullScreenState;
		fullScreenEvent.xclient.data.l[2] = 0;

		// Send the full screen event message to the X11 server.
		XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &fullScreenEvent);

		// Setup the motif hints to remove the border in full screen mode.
		motifHints = XInternAtom(display, "_MOTIF_WM_HINTS", False);

		motifHintList[0] = 2;  // Remove border.
		motifHintList[1] = 0;
		motifHintList[2] = 0;
		motifHintList[3] = 0;
		motifHintList[4] = 0;

		// Change the window property and remove the border.
		XChangeProperty(display, win, motifHints, motifHints, 32, PropModeReplace, (unsigned char*)&motifHintList, 5);

		// Flush the display to apply all the full screen settings.
		XFlush(display);
	}

	return true;
}

//-----------------------------------------------------------------------------
/// check() - Check for GL errors, and report any queued

void check( const char hdr[] )
{
	int err;

	while ( ( err = glGetError() ) != GL_NO_ERROR )
		fprintf( stderr, "OpenGL Error at %s: %s\n", hdr, gluErrorString(err) );
}

//void GLopenGLclass::BeginScene(UINT monitorWindow)
//----------------------------------------------------------------------------
void displayCB()
{
	SystemHandle->ProcessFrame();
}

// linuxsystemclass.cpp
//----------------------------------------------------------------------------
void keyboardCB( KeySym sym, unsigned char key, int x, int y,
				bool &setting_change )
{
	switch ( tolower( key ) )
	{
	case 27:
		// ESCape - We're done!
		exit (0);
		break;

	case 'k':
		WOMA_LOGManager_DebugMSG( "You hit the 'k' key\n" );
		break;

	case 0:
		switch ( sym )
		{
		case XK_Left :
			WOMA_LOGManager_DebugMSG( "You hit the Left Arrow key\n" );
			break;

		case XK_Right :
			WOMA_LOGManager_DebugMSG( "You hit the Right Arrow key\n" );
			break;
		}
		break;
	}
}


// NOT Used
//----------------------------------------------------------------------------
void reshapeCB( int width, int height )
{
	Win.width = SystemHandle->AppSettings->WINDOW_WIDTH = width;
	Win.height = SystemHandle->AppSettings->WINDOW_HEIGHT = height;
}

GLXFBConfig main_chooseFBConfig(Display *display, int screen)
{
	// Default template
	static const int Visual_attribs[] =
	{
		//GLX_X_RENDERABLE    , GL_TRUE,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		//GLX_DOUBLEBUFFER    , GL_TRUE,
		//GLX_SAMPLE_BUFFERS  , 1,
		//GLX_SAMPLES         , NUM_SAMPLES,
		None
	};

	int attribs[MAX_STR_LEN];
	memcpy(attribs, Visual_attribs, sizeof(Visual_attribs));

	GLXFBConfig ret = 0;

	int fbcount;
	GLXFBConfig *fbc = glXChooseFBConfig(display, screen, attribs, &fbcount);
	if (fbc)
	{
		if (fbcount >= 1)
			ret = fbc[0];

		XFree(fbc);
	}

	return ret;
}


//----------------------------------------------------------------------------

void processXEvents( Atom wm_protocols, Atom wm_delete_window )
{
	bool setting_change = false;
	XEvent event;

	while ( XEventsQueued( Win.display, QueuedAfterFlush ) )
	{
		XNextEvent(Win.display, &event);

		if (event.xany.window != Win.window)
			continue;

		switch ( event.type )
		{
		case MotionNotify:
		{
			int x = event.xmotion.x;
			int y = event.xmotion.y;
			//std::cout << "Mouse X:" << x << ", Y: " << y << "\n";
			break;
		}
	#if LEVEL >= 10
		case ButtonPress:
		{
			int xpos = (event.xbutton.x_root) - WOMA::settings.WINDOW_Xpos;
			int ypos = (event.xbutton.y_root) - WOMA::settings.WINDOW_Ypos;
			WOMA_LOGManager_DebugMSG("mousex: %d mouseY: %d\n", xpos, ypos);

			if (event.xbutton.button == Button1)
			{
				WOMA_LOGManager_DebugMSG("BUTTON1\n");
				if ((xpos < 30 && ypos < 60) && (xpos > 0 && ypos > 0))
				{
					WOMA_LOGManager_DebugMSG("RENDER_PAGE = 25\n");
					RENDER_PAGE = 25;
					WOMA::previous_game_state = GAME_IMGUI;
					WOMA::game_state = ENGINE_RESTART;
				}
			}
			break;
		}
	#endif
		case MapNotify:
			{
				Win.ready = true;
				break;
			}
		case ConfigureNotify:
			{
				XConfigureEvent &cevent = event.xconfigure;
				reshapeCB( cevent.width, cevent.height );
				break;
			}
		case KeyPress:
			{
				char chr;
				KeySym symbol;
				XComposeStatus status;

				XLookupString( &event.xkey, &chr, 1, &symbol, &status );

				keyboardCB( symbol, chr, event.xkey.x, event.xkey.y,
					setting_change );
				break;
			}
		case ClientMessage:
			{
				if ( event.xclient.message_type == wm_protocols &&
					Atom( event.xclient.data.l[0] ) == wm_delete_window )
				{
					//printf( "Received WM_DELETE_WINDOW\n" );
					exit(0);
				}
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------
Atom wm_protocols;
Atom wm_delete_window;

void mainLoop()
{
	ASSERT(Win.display);

	// Register to receive window close events (the "X" window manager button)
	wm_protocols = XInternAtom(Win.display, "WM_PROTOCOLS", False);
	wm_delete_window = XInternAtom(Win.display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(Win.display, Win.window, &wm_delete_window, True);

	//Frameless:
	Atom window_type = XInternAtom(Win.display, "_NET_WM_WINDOW_TYPE", False);
	long value = XInternAtom(Win.display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
	XChangeProperty(Win.display, Win.window, window_type, XA_ATOM, 32, PropModeReplace, (unsigned char*)&value, 1);

	while (WOMA::game_state != ENGINE_RESTART)
	{

		{
			// Update frame rate
			static timeval last_xcheck = {0,0};
			struct timeval now;
			gettimeofday( &now, 0 );
			// Check X events every 1/10 second
			if ( elapsedMsec( last_xcheck, now ) > 100 )
			{
				processXEvents( wm_protocols, wm_delete_window );
				last_xcheck = now;
			}
		}


	
	}
}



#endif
