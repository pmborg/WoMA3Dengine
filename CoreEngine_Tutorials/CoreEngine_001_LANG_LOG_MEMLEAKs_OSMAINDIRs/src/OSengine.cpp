// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: OSengine.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: START and STOP WorldOfMiddleAge 3D ENGINE
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "OSengine.h"
#include <inttypes.h>

#if defined WINDOWS_PLATFORM
	WinSystemClass* SystemHandle = NULL;
#endif

#if defined ANDROID_PLATFORM 
#if !defined NewWomaEngine
	AndroidSystemClass* SystemHandle = NULL;
#else
	AndroidNewSystemClass* SystemHandle = NULL;
	#endif
#endif

#if defined LINUX_PLATFORM
	LinuxSystemClass* SystemHandle = NULL;

	extern MyWin Win;
	#include <gtk/gtk.h>		//libgtk4
	#if RUN_ASMAIN
	#include <GL/freeglut.h>
	#endif
#endif

#include "OSmain_dir.h"
#include "log.h"

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
	RApplicationClass* r_Application;
#endif

TCHAR* DEMO_NAME[] =
{
//{"07:Loading a files from engine.pck and Press[F2] for RealTime Celestial Positions of Sun and Moon accordingly with user Location"},
//{"08 : From now on : PRESS[F3] for : [REAL TIME MAP] user location."},
//{"09 : From now on : PRESS[F4] for : [REAL Wheather] at user location."},
//{"19 : PRESS[F6] for SETUP and change Driver : OPENGL, DX9, DX11, DX12 : Initiate the 3D Graphic Drivers and attache the swapchain to mainwindow."},
//{"20 : OPENGL, DX9, DX11, DX12 : Allow mainwindow resizing."},
{"TUTORIAL 21: The Basic triangle and squar using vertexes with indexes.HLSL - The COLOR shader"},
{"TUTORIAL 22: Loading and Rendering Textures.HLSL - The TEXTURE shader"},
{"TUTORIAL 23: Adding the first light, 'Diffuse Lighting'.HLSL - The LIGHT shader"},
{"TUTORIAL 24: Adding Orthogonal Projection - Used on 2D Orthogonal Sprites"},
{"TUTORIAL 25: DX10DRIVER_FONTS"},
{"TUTORIAL 26: The first 3D objects: the CUBEs and SPHEREs - COLOR Shader, TEXTURE Shader and LIGHT Shader"},
{"TUTORIAL 27: The rastertek fonts on DX9, DX11, DX12 and OPENGL"},
{"TUTORIAL 28: The DX direct input, the Sky Camera and the Sphere Skydome"},
{"TUTORIAL 29: This DEMO: Use music (ogg loader) and the sound effects"},
{"TUTORIAL 30: WORLD.XML: load OBJ 3D file format, using COLOR shader"},
{"TUTORIAL 31: WORLD.XML: load OBJ 3D file format, with MULTIPLE TEXTURES, using TEXTURE shader"},
{"TUTORIAL 32: WORLD.XML: load OBJ 3D file format, the first advanced object, using LIGHT shader"},
{"TUTORIAL 33: WORLD.XML: load OBJ 3D file format, the compond, using transparent textures ALFA MAP and ALFA COLOR"},
{"TUTORIAL 34: WORLD.XML: load OBJ 3D file format, with SPECULAR and SHININESS"},
{"TUTORIAL 35: WORLD.XML: load OBJ 3D file format, with BUMP MAP"},
{"TUTORIAL 36: WORLD.XML: load OBJ 3D file format, render a SHADOW MAP"},
{"TUTORIAL 37: WORLD.XML: load OBJ 3D file format, render chess board pieces, adding MAIN THREAD and the LOADER THREAD"},
{"TUTORIAL 38: WORLD.XML: load OBJ 3D file format, save to W3D Woma 3D fast file FORMAT"},
{"TUTORIAL 39: WORLD.XML: load W3D(the Woma 3D file format) files up to 30x faster than OBJ files in DX LEVEl38"},
{"TUTORIAL 40: WORLD.XML: load W3D and use INSTANCES to clone objects using GPU"},
{"TUTORIAL 41: WORLD.XML: load W3D and use SHADOW INSTANCES"},
{"TUTORIAL 42: WORLD.XML: load W3D and use SHADOW INSTANCES with ROTATION per instance"},
{"43"},
{"44"},
{"45"},
{"46"},
{"47"},
{"48"},
{"49: TERRAIN: Generate under water terrain. ModelTextureVertexType"},
{"50: TERRAIN: the under water terrain. ModelTextureVertexType"},
{"51: TERRAIN: Add fog ModelTextureVertexType"},
{"52: TERRAIN: Use Light Shader + Add Normals + Add Index(s) ModelTextureVertexType"},
{"53: TERRAIN: Add extra color terrain(extra Green in this example) ModelTextureVertexType"},
{"54: TERRAIN: Add Water waves ModelTextureVertexType"},
{"55: TERRAIN: 256x256 with Slope Texture Shader[0]ModelTextureVertexType"},
{"56: TERRAIN: 256x256 Terrain Texture Multi Layers "},
{"57: "},
{"58: "},
{"59: "},
{"60: "},
{"61: "},
{DEMO_TITLE},//62
{DEMO_TITLE},//63
{DEMO_TITLE},//64
{DEMO_TITLE},//65
{DEMO_TITLE},//66
{DEMO_TITLE},//67
{DEMO_TITLE},//68
{DEMO_TITLE},//69
{DEMO_TITLE},//70
{DEMO_TITLE},//71
{DEMO_TITLE},//72
{DEMO_TITLE},//73
{DEMO_TITLE},//74
{DEMO_TITLE},//75
{DEMO_TITLE},//76
{DEMO_TITLE},//77
{DEMO_TITLE},//78
{DEMO_TITLE},//79
{DEMO_TITLE},//80
{DEMO_TITLE},//81
{DEMO_TITLE},//82
{DEMO_TITLE},//83
{DEMO_TITLE},//84
{DEMO_TITLE},//85
{DEMO_TITLE},//86
{DEMO_TITLE},//87
{DEMO_TITLE},//88
{DEMO_TITLE},//89
{DEMO_TITLE},//90
{DEMO_TITLE},//91
{DEMO_TITLE},//92
{DEMO_TITLE},//93
{DEMO_TITLE},//94
{DEMO_TITLE},//95
{DEMO_TITLE},//96
{DEMO_TITLE},//97
{DEMO_TITLE},//98
{DEMO_TITLE},//99
};

// Global Public:
UINT CLASS_LOAD_N = 1;
UINT CLASS_DELETE_N = 1;

#if CORE_ENGINE_LEVEL >= 10 & !defined NewWomaEngine
UINT RENDER_PAGE = DX_ENGINE_LEVEL;
std::vector<WomaDriverClass*> driverList;
WomaDriverClass* g_contextDriver = NULL;	// Note: Used only at 20  /*wGLopenGLclass*/
#else
UINT RENDER_PAGE;
#endif

// WINDOWS vs LINUX
// -------------------------------------------------------------------------------------------------------------------------------------
// "C:\Program Files\APP_PROJECT_NAME\"										==> "/opt/APP_PROJECT_NAME"
// "C:\Users\[username]\AppData\Local\APP_COMPANY_NAME\APP_PROJECT_NAME\"   ==> ~/.APP_COMPANY_NAME/APP_PROJECT_NAME
// -------------------------------------------------------------------------------------------------------------------------------------
namespace WOMA
{
	// State Vars:
	//---------------------------------------------------
	int		game_state = GAME_LOADING;
	int		previous_game_state = GAME_LOADING;

	UINT ENGINE_LEVEL_USED = 0;
	int  main_loop_state = 0;

#if defined WINDOWS_PLATFORM // SUBSYSTEM:WINDOWS
	PSTR    Scmdline = { 0 };
	int     Cmdshow = SW_SHOWDEFAULT;           //Useful when using: WOMA_CONSOLE_APPLICATION
#endif
	// SUBSYSTEM:CONSOLE
	BOOL	UseWarpDevice = FALSE;
	int     ARGc = 0;
	CHAR**	ARGv = { 0 };

	TCHAR	strConsoleTitle[MAX_STR_LEN] = { 0 };

#if defined USE_LOADING_THREADS || defined USE_MAIN_THREAD //vars
	UINT	num_running_THREADS = 0;
#endif
#if defined USE_LOADING_THREADS || DX_ENGINE_LEVEL >= 37
	UINT	num_loading_objects = 1;
#endif

	TCHAR   APP_NAME[MAX_STR_LEN] = { 0 };              // "Aplication Name"

#if defined NOTES
// More Info MessageBox: http://msdn.microsoft.com/en-us/library/windows/desktop/ms645505%28v=vs.85%29.aspx
#endif

// LINUX:
#if defined LINUX_PLATFORM
#define fatal false
#endif

	extern TCHAR	APP_FULLNAME[MAX_STR_LEN];

// --------------------------------------------------------------------------------------------
// GLOBAL - MACROS:
// --------------------------------------------------------------------------------------------
#if !defined(_DEBUG) && defined(NDEBUG) // Get String of the Build Type: Debug / Release
#define BUILD "RELEASE"
#else
#define BUILD "DEBUG"
#endif

#if (defined _DEBUG || defined DEBUG || defined ANDROID_PLATFORM) && !defined NDEBUG	//_DEBUG;_CONSOLE
	STRING BINARY = TEXT("DEBUG");
#elif !defined _DEBUG && defined NDEBUG
	STRING BINARY = TEXT("RELEASE");	//NDEBUG
#else
	STRING BINARY = TEXT("DBGREL");		//_DEBUG;NDEBUG
#endif

#if defined WINDOWS_PLATFORM
	int getTaskBarHeight()
	{
		RECT rect;
		HWND taskBar = FindWindow(TEXT("Shell_traywnd"), NULL);
		return (taskBar && GetWindowRect(taskBar, &rect)) ? (rect.bottom - rect.top) : 0;
	}
#endif

#if defined WINDOWS_PLATFORM && defined WOMA_CONSOLE_APPLICATION 
	// "Console Window": Set handler:
	static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
	{
		//if (SystemHandle)
			::PostMessage(NULL, WM_QUIT, 0, 0); // NOTE: (SystemHandle->m_hWnd) CANT be used!
		return TRUE;
	}
#endif
}//namespace WOMA

namespace WOMA
{
	int endian()
	{
		short int word = 0x0001;
		char* byte = (char*)&word;
		return (byte[0] ? LITTLE_ENDIAN : BIG_ENDIAN);
	}
}

void DefineConsoleTitle() 
{
	// -------------------------------------------------------------------------------------------
	// SETUP: "Console Window":
	// -------------------------------------------------------------------------------------------
	TCHAR cpu_type[256] = { 0 };
#if defined CPU_X86
	#define cpu_type TEXT("32 bits")
#elif defined X64
	#define cpu_type TEXT("64 bits")
#endif

	TCHAR charSet[256] = { 0 };
#if defined UNICODE
	#define charSet TEXT("UNICODE")
#else
	#define charSet TEXT("MultiByte")
#endif

	StringCchPrintf(WOMA::strConsoleTitle, sizeof(WOMA::strConsoleTitle), TEXT("%s ENGINE Level: %d - %s [%s] %s\n"), WOMAOS, (int)CORE_ENGINE_LEVEL, charSet, WOMA::BINARY.c_str(), cpu_type);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("------------------------------------------------------------------------------------------\n"));
	WOMA_LOGManager_DebugMSGAUTO(WOMA::strConsoleTitle);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("------------------------------------------------------------------------------------------\n"));

#if defined WINDOWS_PLATFORM 
#if defined WOMA_CONSOLE_APPLICATION 
	// "Console Window": Set handler:
	SetConsoleCtrlHandler(WOMA::console_ctrl_handler, TRUE);

	// "Console Window": Move away... :)
	int Bar = WOMA::getTaskBarHeight();
	HWND hConsole = GetConsoleWindow();
	MoveWindow(hConsole, GetSystemMetrics(SM_CXSCREEN) - CONSOLE_LOG_WIDTH, 0, CONSOLE_LOG_WIDTH, GetSystemMetrics(SM_CYSCREEN) - Bar, TRUE);

	// "Console Window": Set Title
	SetConsoleTitle(TEXT("WOMA ENGINE - Console Window"));

	LEVELHIGHLIGHT(CORE_ENGINE_LEVEL);
#endif

	LEVELNORMAL();
	//Use printf or OutputDebugString: WOMA_LOGManager dont exist yet
	OutputDebugString(TEXT("\n"));
#endif

}

void APPLICATION_STARTUP(int argc, char* argv[], int Command)
{
	srand(27); // to have always the same random numbers... the true Random is: "srand(time(0));"

#if defined WINDOWS_PLATFORM
	#if defined _NOTUSED
		setpriority(PRIO_PROCESS, 0, 20);	// Be nice to other processes, helps reduce mouse lag
	#else
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	#endif

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr)) WomaFatalException("CoInitializeEx Failed!");
#endif

#ifdef MATH_BENCH && LEVEL < 60 // Disabled at 60: TrigonometryMathClass.cpp
	TimerClass m_Timer;
	m_Timer.Initialize();	// Initialize the timer object.

	//------------------------------------------------------
	INT64 currentTime, currentTime2;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	for (int time = 0; time < 1000000; time++)
	{
		//------------------------------------------------------

		float t = FAST_sqrt((float)time); // 50% faster
		//float t = sqrt((float)time);

		//------------------------------------------------------
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime2);
	INT64 delta = (INT64)((((float)currentTime2 - (float)currentTime) / m_Timer.m_ticksPerUs) / 1000.0f);
	TCHAR txt[100];
	sprintf_s(txt, 100, "1 000 000 times took: %" PRIu64 " µs(micro Seconds)\n", delta); //_INT64
	OutputDebugString(txt);
	//------------------------------------------------------
#endif

#if CORE_ENGINE_LEVEL >= 1 && defined WINDOWS_PLATFORM
	WOMA::setup_OSmain_dirs();				//1 Keep this order!
	WOMA::activate_mem_leak_detection();	//2
#endif
	DefineConsoleTitle();

	// Save Command Line Arguments to use later on
#if defined WOMA_CONSOLE_APPLICATION  || !defined WINDOWS_PLATFORM
	WOMA::ARGc = argc;
	WOMA::ARGv = argv;
#endif

	// [4] Set A Top level "Exception handler" for all Exceptions. Catch, Dump & Send Report WOMA ENGINE HOME using FTP!
	// -------------------------------------------------------------------------------------------

#ifdef LINUX_PLATFORM 
	// [6-1] Start LINUX Platform GUI: Settings
	// -------------------------------------------------------------------------------------------
	// ALSO: After logManager!
	char* value = getenv("DISPLAY");
	putenv("DISPLAY=:0");
	value = getenv("DISPLAY");
	WOMA_LOGManager_DebugMSGAUTO(TEXT("env.DISPLAY: %s\n"), value);

	WOMA_LOGManager_DebugMSGAUTO(TEXT("Init: INIT_GTK2()\n"));
	if (!PLATFORM_INIT_GTK2())
		WOMA_LOGManager_DebugMSGAUTO(TEXT("Could not initialize GTK2!")); // Note: Dont use DEBUG_MSG yet...
#endif

}

void APPLICATION_STOP()
{
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
	r_Application->Shutdown();
#endif

#if defined WINDOWS_PLATFORM
	CoUninitialize();
#endif

#if defined ANDROID_PLATFORM
	engine.has_focus_ = false;
	terminate();
#endif
}

#if CORE_ENGINE_LEVEL >= 1 && defined ANDROID_PLATFORM  && !defined NewWomaEngine
#include <android/asset_manager.h>

//file.cpp
#define g_pAssetManager engine.app->activity->assetManager

bool File::open(const char* path, const char* mode) {
	if (strchr(mode, 'w')) {
		_File = fopen(path, mode);
		return true;
	}
	_A = AAssetManager_open(g_pAssetManager, path, 0);
	if (_A) {
		_File = funopen(_A,
			androidRead,
			androidWrite,
			androidSeek,
			androidClose);
	}
	if (_File != nullptr) {
		return true;
	}

	return false;
}

#endif

#if defined ANDROID_PLATFORM
int m_main_music_id = 0;
JNIEnv* jni=NULL;

void ShowFPS()
{
	//FILE: C:\WoMAengine2023\Android-WomaEngine\Android2\Android2.Packaging\src\com\woma\MyActivity.java
	//JAVA: public void updateFPS(final float fFPS)
	{
		float fFPS = 0;
		if (!jni)
			engine.app->activity->vm->AttachCurrentThread(&jni, NULL);
		jclass clazz = jni->GetObjectClass(engine.app->activity->clazz);

		// Signature has to match java implementation 
		jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");

		jni->CallVoidMethod(engine.app->activity->clazz, methodID, fFPS);
		//engine_state.app->activity->vm->DetachCurrentThread();
	}
}

void ShowAlert(const char* message)
{
	//JNIEnv* jni;

	//JAVA: public void ShowAlert(final String message)
	{
		if (!jni)
			engine.app->activity->vm->AttachCurrentThread(&jni, NULL);
		jclass clazz = jni->GetObjectClass(engine.app->activity->clazz);

		// Signature has to match java implementation (second string hints a t a java string parameter)
		jmethodID methodID = jni->GetMethodID(clazz, "ShowAlert", "(Ljava/lang/String;)V");

		// Strings passed to the function need to be converted to a java string object
		jstring jmessage = jni->NewStringUTF(message);
		jni->CallVoidMethod(engine.app->activity->clazz, methodID, jmessage);
		jni->DeleteLocalRef(jmessage);

		//engine_state.app->activity->vm->DetachCurrentThread();
	}
}

void finish_activity(UINT res) {
	JNIEnv* jni = NULL;
	if (!jni)
		engine.app->activity->vm->AttachCurrentThread(&jni, NULL);
	jclass clazz = jni->GetObjectClass(engine.app->activity->clazz);

	// Signature has to match java implementation
	jmethodID methodID = jni->GetMethodID(clazz, "finishActivity", "(I)V");

	jni->CallVoidMethod(engine.app->activity->clazz, methodID, 0);
	engine.app->activity->vm->DetachCurrentThread();

}

void DownloadFiles(const char* url, const char* file)
{
	//JAVA: public void DownloadFiles(final String file)
	{
		if (!jni)
			engine.app->activity->vm->AttachCurrentThread(&jni, NULL);
		jclass clazz = jni->GetObjectClass(engine.app->activity->clazz);

		jmethodID methodID = jni->GetMethodID(clazz, "DownloadFiles", "(Ljava/lang/String;Ljava/lang/String;)V");

		jstring jurl = jni->NewStringUTF(url);
		jstring jfile = jni->NewStringUTF(file);
		jni->CallVoidMethod(engine.app->activity->clazz, methodID, jurl, jfile);
		jni->DeleteLocalRef(jurl);
		jni->DeleteLocalRef(jfile);
		//engine_state.app->activity->vm->DetachCurrentThread();
	}
}


int playAudio(const char* audioFile)
{
	//JNIEnv* jni;
	jint id;

	//JAVA: public void playAudio(final String audioFile)
	{
		if (!jni)
			engine.app->activity->vm->AttachCurrentThread(&jni, NULL);
		jclass clazz = jni->GetObjectClass(engine.app->activity->clazz);

		// Signature has to match java implementation (second string hints a t a java string parameter)
		jmethodID methodID = jni->GetMethodID(clazz, "playAudio", "(Ljava/lang/String;)I");

		// Strings passed to the function need to be converted to a java string object
		jstring jaudioFile = jni->NewStringUTF(audioFile);
		id = jni->CallIntMethod(engine.app->activity->clazz, methodID, jaudioFile);
		//_tprintf("[%d]: playAudio()\n", id);
		jni->DeleteLocalRef(jaudioFile);

		//engine_state.app->activity->vm->DetachCurrentThread();
	}

	return id;
}

void stopAudio(const int audioFileIdx)
{
	//JNIEnv* jni;

	//JAVA: public void stopAudio(final Integer audioFileIdx)
	{
		if (!jni)
			engine.app->activity->vm->AttachCurrentThread(&jni, NULL);
		jclass clazz = jni->GetObjectClass(engine.app->activity->clazz);

		// Signature has to match java implementation 
		jmethodID methodID = jni->GetMethodID(clazz, "stopAudio", "(I)V");

		jni->CallVoidMethod(engine.app->activity->clazz, methodID, audioFileIdx);
		//engine_state.app->activity->vm->DetachCurrentThread();
	}
}
#endif


#if !defined LINUX_PLATFORM
int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption, bool yesORno)
//----------------------------------------------------------------------------------
{
	int res = yesORno ? 1 : 0;
	TCHAR fullMsg[5000] = { 0 };
	StringCchPrintf(fullMsg, sizeof(fullMsg), TEXT("%s %s\n"), lpCaption, lpText);

#if defined WINDOWS_PLATFORM
	{ HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); }

	WOMA_LOGManager_DebugMSGAUTO(fullMsg);
	{ HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); }

		res = MessageBox(NULL, lpText, lpCaption, (yesORno) ? MB_YESNO : MB_OK);
#endif

#if defined ANDROID_PLATFORM
	ShowAlert(lpCaption);
#endif

	return res;
}
#else//LINUX
int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption = "WOMA3D", bool yesORno = false)
{
	// MORE INFO: http://stackoverflow.com/questions/263/gtk-implementation-of-messagebox
	char text[MAX_STR_LEN]; wtoa(text, lpText, MAX_STR_LEN); //w to a
	GtkWidget* dialog = gtk_message_dialog_new(
		GTK_WINDOW(Win.window), GTK_DIALOG_MODAL,
		(fatal) ? GTK_MESSAGE_ERROR : GTK_MESSAGE_WARNING,
		(yesORno) ? GTK_BUTTONS_YES_NO : GTK_BUTTONS_OK, text);

	char caption[MAX_STR_LEN]; wtoa(caption, lpCaption, MAX_STR_LEN);	// wide to ansi
	gtk_window_set_title(GTK_WINDOW(dialog), caption);					// Title

	while (gtk_events_pending())			// Wait for all gtk events to be consumed ...
		gtk_main_iteration_do(FALSE);

	gint result = gtk_dialog_run(GTK_DIALOG(dialog));	// Run Dialog

	gtk_widget_destroy(dialog);			// Destroy

	while (gtk_events_pending())			// Wait for all gtk events to be consumed ...
		gtk_main_iteration_do(FALSE);

	if (yesORno)
	{
		switch (result)
		{
		default:
		case GTK_RESPONSE_DELETE_EVENT:
		case GTK_RESPONSE_NO:
			return 0; //NO

		case GTK_RESPONSE_ACCEPT:
		case GTK_RESPONSE_YES:
			return 1; //YES
		}
	}

	return result;
}
#endif

int WomaMessageBox(TCHAR* lpText)
{
	return WomaMessageBox(lpText, TEXT("ERROR:"), false);
}

int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption)
{
	return WomaMessageBox(lpText, lpCaption, false);
}

#if defined LINUX_PLATFORM //|| defined ANDROID_PLATFORM
#include <curl/curl.h>
#include <curl/easy.h>

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

bool download(const std::string url, const std::string file)
{
	FILE* fp;
	CURLcode res;
	CURL* curl = curl_easy_init();
	//std::string readBuffer;
	if (curl)
	{
		fp = fopen(file.c_str(), "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

		// Perform a file transfer synchronously.
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}

	if (res == CURLE_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if defined LINUX_PLATFORM
// ---------------------------------------------------------------------------
void ItoA(int value, char* dest, int _Radix)
{
	static std::string str;
	std::stringstream ss; //int x = 23;

	ss << value;
	str = ss.str();
	strcpy(dest, str.c_str());
}
#endif

#if defined ANDROID_PLATFORM
// Used by ANDROID: To be compatible with WINDOWS / LINUX / ANDROID:
int woma_atoi(TCHAR* _String)
{
	static int out;
	_stscanf(_String, TEXT("%d"), &out); //swscanf
}

void woma_itoa(char** _String, int in, int system)
{
	StringCchPrintf(*_String, MAX_STR_LEN, TEXT("%d"), in);

}
#endif

