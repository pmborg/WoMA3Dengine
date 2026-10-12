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
	//==============================================================================================
	// LINUX WINDOW SYSTEM (X11)
	//==============================================================================================
	// PURPOSE:
	//   Create and configure the native X11 operating system window.
	//
	// RESPONSIBILITIES:
	//   - Connect to the X Server.
	//   - Create the native X11 window.
	//   - Configure fullscreen, title and input events.
	//   - Store the native window handles (Display*, Window).
	//
	// NOTE:
	//   This section is graphics API independent.
	//   It is shared by OpenGL and Vulkan.
	//==============================================================================================
	
	// Init globals:
	Win.width = WOMA::AppSettings->WINDOW_WIDTH;	// screenWidth;
	Win.height = WOMA::AppSettings->WINDOW_HEIGHT;	//screenHeight;
	Win.ready = false;										// No Render yet!
	Screen* defaultScreen;

	// [1] Open a connection to the X server on the local computer.
	Display* display = Win.display = XOpenDisplay(NULL);
	if (!display) {
		womalog("Cannot open X display\n");
		return false;
	}

	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		if (!glXQueryExtension(display, 0, 0)) {
			womalog("X Server doesn't support GLX extension\n");
			return false;
		}
	}

	// [2] Get a handle to the root window.
	int screen = DefaultScreen(display);
	Window root_win = DefaultRootWindow(display);




	XVisualInfo* visinfo = nullptr;
	XSetWindowAttributes winAttr{};
	XVisualInfo visualInfo{};
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		int fbcount;

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

		GLXFBConfig* fbc = glXChooseFBConfig(display, screen, attribs, &fbcount);
		GLXFBConfig fbconfig{};
		if (fbc)
		{
			if (fbcount >= 1)
				fbconfig = fbc[0];

			XFree(fbc);
		}

		//----------------------------------------------------------------------------
		//Visual* visual;
		// Obter o ponteiro temporário do GLX
		XVisualInfo* temp_vis = glXGetVisualFromFBConfig(display, fbconfig);
		if (!temp_vis)
		{
			womalog("Failed to get XVisualInfo\n");
			return false;
		}

		visualInfo = *temp_vis;  // COPIA por valor para a nossa variável segura da stack
		visinfo = &visualInfo;   // Aponta para a nossa variável estável
		XFree(temp_vis);         // Liberta IMEDIATAMENTE a memória alocada pelo GLX

		womalog("GL3 X Visual ID = 0x%.2x\n", int(visinfo->visualid));

		// Atributos da janela para GL3
		winAttr.colormap = XCreateColormap(display, root_win, visinfo->visual, AllocNone);
		winAttr.event_mask = StructureNotifyMask | KeyPressMask;
		winAttr.background_pixmap = None;
		winAttr.background_pixel = 0;
		winAttr.border_pixel = 0;
		winAttr.override_redirect = 1;
	}
	else if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		// [1] Obter o Visual padrão do ecrã para o Vulkan
		Visual* defaultVisual = DefaultVisual(display, screen);
		if (!defaultVisual)
		{
			womalog("Failed to get Default Visual for Vulkan\n");
			return false;
		}

		// [2] Alocar e preencher a estrutura XVisualInfo (opcional, mas mantém compatibilidade se usares depois)
		visinfo = &visualInfo; // Aponta com segurança para a stack
		if (!visinfo)
		{
			womalog("Failed to allocate XVisualInfo\n");
			return false;
		}

		int depth = DefaultDepth(display, screen);
		if (XMatchVisualInfo(display, screen, depth, TrueColor, visinfo) == 0)
		{
			// Fallback caso não encontre TrueColor exato, usa a info básica
			visinfo->visual = defaultVisual;
			visinfo->visualid = XVisualIDFromVisual(defaultVisual);
			visinfo->screen = screen;
			visinfo->depth = depth;
		}

		womalog("Vulkan X Visual ID = 0x%.2x\n", int(visinfo->visualid));

		// [3] Criar o Colormap baseado no Visual escolhido: Atributos da janela para Vulkan (Idênticos ao GL3)
		winAttr.colormap = XCreateColormap(display, root_win, visinfo->visual, AllocNone);
		winAttr.event_mask = StructureNotifyMask | KeyPressMask;
		winAttr.background_pixmap = None;
		winAttr.background_pixel = 0;
		winAttr.border_pixel = 0;
		winAttr.override_redirect = 1; // Mantém igual ao GL3 se precisares do mesmo comportamento de janela
	}

	unsigned int mask = CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask;

	// [4] Get the size of the default screen.
	if (WOMA::AppSettings->FULL_SCREEN)
	{
		defaultScreen = XDefaultScreenOfDisplay(display);
		Win.width = XWidthOfScreen(defaultScreen);
		Win.height = XHeightOfScreen(defaultScreen);
	}

	Window win = None;
	//if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		// [5] Create the X window with the desired settings.
		win = Win.window = XCreateWindow(display, root_win,
			WOMA::settings.WINDOW_Xpos, WOMA::settings.WINDOW_Ypos,
			Win.width,
			Win.height,
			0,
			visinfo->depth, InputOutput,
			visinfo->visual, mask, &winAttr);
	}
	//else if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	//{
	//	//...
	//}

	WOMA::WindowDataContainer used_screen;

	used_screen.MonitorName = TEXT("Default");
	used_screen.width = Win.width;
	used_screen.height = Win.height;
	
	used_screen.refreshRate_Numerator = 60;
	used_screen.refreshRate_Denominator = 1;
	
	used_screen.display = display;
	used_screen.window = win;

	WOMA::windowsToRenderArray.push_back(used_screen);
	WOMA::allWindowsHwArray.push_back(used_screen);
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
	if (WOMA::AppSettings->FULL_SCREEN)
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

	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
	//==============================================================================================
	// OPENGL (GLX)
	//==============================================================================================
	// PURPOSE:
	//   Create the OpenGL rendering context and attach it to the X11 window.
	//
	// RESPONSIBILITIES:
	//   - Create the GLX context.
	//   - Make the context current.
	//   - Validate the OpenGL version.
	//   - Configure VSync.
	//
	// NOTE:
	//   Vulkan does NOT execute this section.
	//   Vulkan creates its own rendering context inside vkClass.
	//   OpenGL is the only API that requires a GLXContext.

	// [9] Create an OpenGL rendering context.
	GLXContext context = glXCreateContext(display, visinfo, NULL, 1);
	if (!context)
		return false;
	Win.context = context;

	// [10] Attach the OpenGL rendering context to the newly created window.
	if (!glXMakeCurrent(display, win, context)) {
		womalog("glXMakeCurrent failed.\n");
		return false;
	}

	// [11] Check that OpenGL 4.0 is supported at a minimum: Got Ver: 4.2
	int majorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	if (majorVersion < 4)
		return false;

	int minorVersion;
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	//if (majorVersion < 4)
	//	return false;

	// [12]  Confirm that we have a direct rendering context.
	if (!glXIsDirect(display, glXGetCurrentContext())) {
		womalog("Indirect GLX rendering context obtained\n");
		return false;
	}

	check("CHECK: createWindow()");

	//----------------------------------------------------------------------------
	womalog("Window Size = %d x %d\n", WOMA::AppSettings->WINDOW_WIDTH/*screenWidth*/, WOMA::AppSettings->WINDOW_HEIGHT/*screenHeight*/);
	womalog("Window Samples = %d\n", NUM_SAMPLES);


	// Get the current drawable so we can modify the vertical sync swapping.
	GLXDrawable drawable = glXGetCurrentDrawable();

	// Turn on or off the vertical sync depending on the input bool value.
	/*
	if (WOMA::AppSettings->VSYNC_ENABLED)
	{
		glXSwapIntervalEXT(display, drawable, 1);
	}
	else
	{
		glXSwapIntervalEXT(display, drawable, 0);
	}
	*/
	glXSwapIntervalEXT(display, drawable, WOMA::AppSettings->VSYNC_ENABLED);
	}
	else if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		// O Vulkan não usa contextos GLX. O contexto real (VkInstance/VkDevice) 
		// será criado dentro da vkClass usando o Win.display e Win.window.
		//Win.context = nullptr;

		// Executar o check básico de sanidade do motor se necessário
		check("CHECK: createWindow() Vulkan");

		//----------------------------------------------------------------------------
		// Manter os logs idênticos ao GL3 para consistência do motor WOMA
		womalog("Vulkan Window Size = %d x %d\n", WOMA::AppSettings->WINDOW_WIDTH, WOMA::AppSettings->WINDOW_HEIGHT);

		// Nota: O controlo de VSync no Vulkan não é feito aqui, mas sim na 
		// criação da Swapchain escolhendo o VkPresentModeKHR (FIFO = VSync On, IMMEDIATE/MAILBOX = VSync Off).
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
		womalog( "You hit the 'k' key\n" );
		break;

	case 0:
		switch ( sym )
		{
		case XK_Left :
			womalog( "You hit the Left Arrow key\n" );
			break;

		case XK_Right :
			womalog( "You hit the Right Arrow key\n" );
			break;
		}
		break;
	}
}


// NOT Used
//----------------------------------------------------------------------------
void reshapeCB( int width, int height )
{
	Win.width = WOMA::AppSettings->WINDOW_WIDTH = width;
	Win.height = WOMA::AppSettings->WINDOW_HEIGHT = height;
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
			womalog("mousex: %d mouseY: %d\n", xpos, ypos);

			if (event.xbutton.button == Button1)
			{
				womalog("BUTTON1\n");
				if ((xpos < 30 && ypos < 60) && (xpos > 0 && ypos > 0))
				{
					womalog("RENDER_PAGE = 25\n");
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

	//
	// Main loop
	//
	while (WOMA::game_state != ENGINE_RESTART)
	{
		if (WOMA::main_loop_state < 0) {
			WOMA::game_state = GAME_STOP;
			break;
		}

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

		{
			static timeval last_xcheck2 = { 0,0 };
			struct timeval now2;
			gettimeofday(&now2, 0);
			// Check X events every 1/10 second
			if (elapsedMsec(last_xcheck2, now2) > 1000)
			{
				linux_window_title();
				SystemHandle->refreshTitle();
				XStoreName( Win.display, Win.window, SystemHandle->SystemClass::pstrFPS);
				//womalog("GetTime(): %f\n", SystemHandle->SystemClass::m_Timer.GetTime());
				last_xcheck2 = now2;
			}
		}

		// Redraw window (after it's mapped)
		if ( Win.ready )
			SystemHandle->ProcessFrame();
	}
}



#endif
