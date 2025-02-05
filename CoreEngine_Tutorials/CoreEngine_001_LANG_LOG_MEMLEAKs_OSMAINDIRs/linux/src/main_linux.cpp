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


	
	}
}



#endif
