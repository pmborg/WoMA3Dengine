// --------------------------------------------------------------------------------------------
// Filename: linux.cpp (for LINUX & ANDROID)
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : woma.no-ip.org
//
// PURPOSE:
//
// ********************************************************************************************

#include "platform.h" //LPCSTR
#if defined LINUX_PLATFORM
#include "main.h"
#include "OSengine.h"

// ---------------------------------------------------------------------------
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

/* Helpful conversion constants. */
static const unsigned usec_per_sec = 1000000;
static const unsigned usec_per_msec = 1000;

/* These functions are written to match the win32
   signatures and behavior as closely as possible.
*/
bool QueryPerformanceFrequency(LARGE_INTEGER *frequency)
{
    /* Sanity check. */
    ASSERT(frequency != NULL);

    /* gettimeofday reports to microsecond accuracy. */
    *frequency = usec_per_sec;

    return true;
}

bool QueryPerformanceCounter(LARGE_INTEGER *performance_count)
{
    struct timeval time;

    /* Sanity check. */
    ASSERT(performance_count != NULL);

    /* Grab the current time. */
    gettimeofday(&time, NULL);
    *performance_count = time.tv_usec + /* Microseconds. */
                         time.tv_sec * usec_per_sec; /* Seconds. */

    return true;
}

// ---------------------------------------------------------------------------
// Windows function equivalent:
unsigned int timeGetTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec*1000) + (now.tv_usec/1000);
}

// ---------------------------------------------------------------------------

// Emulate Windows Calls:
// ---------------------------------------------------------------------------
void strcpy_s(char *strDestination, const char *strSource) { strcpy(strDestination,strSource); }
void strcpy_s(char *strDestination, UINT numberOfElements, const char *strSource) { strcpy(strDestination,strSource); }

// ---------------------------------------------------------------------------
#if defined GTK_GUI
#include <gtk/gtk.h>		//libgtk4

bool PLATFORM_INIT_GTK2() 
{
	WOMA_LOGManager_DebugMSG ("INIT: GTK2...");	//DONT USE (need sudo): WOMA_LOGManager_DebugMSG ("INIT: GTK2...");

	gtk_disable_setlocale();
	gtk_init(&WOMA::ARGc, &WOMA::ARGv);

	if (!gtk_init_check(&WOMA::ARGc, &WOMA::ARGv)) {
		WOMA_LOGManager_DebugMSG ("gtk_init_check: Failed!\n");
		return false;
	}

	return true;
}
#endif

// LINUX:
#if defined LINUX_PLATFORM
#define fatal false

#if CORE_ENGINE_LEVEL >= 1
extern TCHAR	APP_NAME[MAX_STR_LEN];	// "Aplication Name"
#endif

namespace WOMA
{
	int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption, bool yesORno)
	{
		// http://stackoverflow.com/questions/263/gtk-implementation-of-messagebox
		char text[MAX_STR_LEN+1]; 
		wtoa(text, lpText, MAX_STR_LEN); //w to a
		GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(Win.window), GTK_DIALOG_MODAL, 
							(fatal) ?	GTK_MESSAGE_ERROR	: GTK_MESSAGE_WARNING, 
							(yesORno) ? GTK_BUTTONS_YES_NO	: GTK_BUTTONS_OK, text);

		char caption[MAX_STR_LEN]; wtoa(caption, lpCaption, MAX_STR_LEN);	// wide to ansi
		gtk_window_set_title(GTK_WINDOW(dialog), caption);					// Title
		gint result = gtk_dialog_run (GTK_DIALOG (dialog));	// Run Dialog

		gtk_widget_destroy (dialog);			// Destroy

		while (gtk_events_pending ())			// Wait for all gtk events to be consumed ...
			gtk_main_iteration_do (FALSE);

		if (yesORno) 
		{
			switch (result) 
			{
				default:
				case GTK_RESPONSE_DELETE_EVENT:
				case GTK_RESPONSE_NO:
					return 0;

				case GTK_RESPONSE_ACCEPT:
				case GTK_RESPONSE_YES:
					return 1;
			}
		}

		return result;
	}
}
#endif

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
#ifdef ANDROID_PLATFORM
#include <android/log.h>

//WOMA_LOGManager_DebugMSG
void LogInfo(const char *sTag, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  __android_log_vprint(ANDROID_LOG_INFO, sTag, fmt, ap);
  va_end(ap);
}

#define  LOG_TAG    "testjni"
#define  MY_LOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

namespace WOMA
{
	int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption, /*bool fatal,*/ bool yesORno = false)
	{
		MY_LOG("%s", lpText);
	}
}
#endif
// ---------------------------------------------------------------------------
#endif
