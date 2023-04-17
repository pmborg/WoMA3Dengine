// --------------------------------------------------------------------------------------------
// Filename: OSengine.cpp
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
// PURPOSE: START and STOP WorldOfMiddleAge 3D ENGINE
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#include "main.h"
#include "OSengine.h"

WinSystemClass* SystemHandle = NULL;

#include "mem_leak.h"	//NEW()
#include "OSmain_dir.h"

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"


// Global Public:
UINT CLASS_LOAD_N = 1;
UINT CLASS_DELETE_N = 1;

// WINDOWS vs LINUX
// -------------------------------------------------------------------------------------------------------------------------------------
// "C:\Program Files\APP_PROJECT_NAME\"										==> "/opt/APP_PROJECT_NAME"
// "C:\Users\[username]\AppData\Local\APP_COMPANY_NAME\APP_PROJECT_NAME\"   ==> ~/.APP_COMPANY_NAME/APP_PROJECT_NAME
// -------------------------------------------------------------------------------------------------------------------------------------
namespace WOMA
{
	// SUBSYSTEM:WINDOWS
	PSTR    Scmdline = { 0 };
	int     Cmdshow = SW_SHOWDEFAULT;           //Useful when using: WOMA_CONSOLE_APPLICATION
	// SUBSYSTEM:CONSOLE
	int     ARGc = 0;
	CHAR**	ARGv = { 0 };

	TCHAR strConsoleTitle[MAX_STR_LEN] = { 0 };

	TCHAR   APP_COMPANY_NAME[] = TEXT("Pmborg");		// "Company" Directory Name: 1st lvl
	TCHAR   APP_PROJECT_NAME[] = TEXT("WoMA3Dengine");	// "Project" Directory Name: 2nd lvl

	TCHAR   APP_NAME[MAX_STR_LEN] = { 0 };              // "Aplication Name"

	TCHAR   APP_FULLNAME[MAX_STR_LEN] = { 0 };          // "Aplication FullName"

	TCHAR   APP_SETTINGS_FILE[] = TEXT("settings.xml"); // SETUP/Configuration file name

	TCHAR	APP_ICO[] = TEXT("WoMA.ico");			// "Define" Main Window: Icon 

	MiniDumper* miniDumper = NULL;
int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption, bool yesORno)
//----------------------------------------------------------------------------------
{
	TCHAR fullMsg[5000] = { 0 };
	StringCchPrintf(fullMsg, sizeof(fullMsg), TEXT("[MessageBox] %s\n"), lpText);

	{ HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); }

	WOMA_LOGManager_DebugMSGAUTO(fullMsg);
	{ HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); }

	int res = 0;
	if (SystemHandle)
		res = MessageBox(SystemHandle->m_hWnd, lpText, lpCaption, (yesORno) ? MB_YESNO : MB_OK);
	else
		res = MessageBox(NULL, lpText, lpCaption, (yesORno) ? MB_YESNO : MB_OK);

	return res;
}

// --------------------------------------------------------------------------------------------
// GLOBAL - MACROS:
// --------------------------------------------------------------------------------------------
#if !defined(_DEBUG) && defined(NDEBUG) // Get String of the Build Type: Debug / Release
#define BUILD "RELEASE"
#else
#define BUILD "DEBUG"
#endif

#if defined _DEBUG && !defined NDEBUG	//_DEBUG;_CONSOLE
	STRING BINARY = TEXT("DEBUG");
#elif !defined _DEBUG && defined NDEBUG
	STRING BINARY = TEXT("RELEASE");	//NDEBUG
#else
	STRING BINARY = TEXT("DBGREL");		//_DEBUG;NDEBUG
#endif

	int getTaskBarHeight()
	{
		RECT rect;
		HWND taskBar = FindWindow(TEXT("Shell_traywnd"), NULL);
		return (taskBar && GetWindowRect(taskBar, &rect)) ? (rect.bottom - rect.top) : 0;
	}

#if defined WINDOWS_PLATFORM && defined WOMA_CONSOLE_APPLICATION 
	// "Console Window": Set handler:
	static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
	{
		if (SystemHandle)
			::PostMessage(NULL, WM_QUIT, 0, 0); // NOTE: (SystemHandle->m_hWnd) CANT be used!
		return TRUE;
	}
#endif
}

void DefineConsoleTitle() 
{
	// -------------------------------------------------------------------------------------------
	// SETUP: "Console Window":
	// -------------------------------------------------------------------------------------------
	TCHAR cpu_type[256] = { 0 };
#if defined CPU_X86
	_stprintf(cpu_type, TEXT("32 bits"));
#elif defined X64
	_stprintf(cpu_type, TEXT("64 bits"));
#endif

	TCHAR charSet[256] = { 0 };
#if defined UNICODE
	_stprintf(charSet, TEXT("UNICODE"));
#else
	_stprintf(charSet, TEXT("MultiByte"));
#endif

	StringCchPrintf(WOMA::strConsoleTitle, sizeof(WOMA::strConsoleTitle), TEXT("WOMA ENGINE Level: %d - %s [%s] %s\n"), CORE_ENGINE_LEVEL, charSet, WOMA::BINARY.c_str(), cpu_type);

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
	//Use printf: WOMA_LOGManager dont exist yet
	_tprintf(TEXT("------------------------------------------------------------------------------------------\n"));
	_tprintf(WOMA::strConsoleTitle);
	_tprintf(TEXT("------------------------------------------------------------------------------------------\n"));
#endif

#ifdef UNICODE
	OutputDebugStringW(TEXT("------------------------------------------------------------------------------------------\n"));
	OutputDebugStringW(WOMA::strConsoleTitle);
	OutputDebugStringW(TEXT("------------------------------------------------------------------------------------------\n"));
#else
	OutputDebugString(TEXT("------------------------------------------------------------------------------------------\n"));
	OutputDebugString(WOMA::strConsoleTitle);
	OutputDebugString(TEXT("------------------------------------------------------------------------------------------"));
#endif

	LEVELNORMAL();
	OutputDebugString(TEXT("\n"));

}

void START_OS_SYSTEM_KERNEL(int argc, char* argv[])
{
	
	WOMA::setup_OSmain_dirs();				//1 Keep this order!
	WOMA::activate_mem_leak_detection();	//2
	WOMA::start_log_manager();				//3
	
	DefineConsoleTitle();

	// Save Command Line Arguments to use later on
#if defined WOMA_CONSOLE_APPLICATION  || !defined WINDOWS_PLATFORM
	WOMA::ARGc = argc;
	WOMA::ARGv = argv;

#if defined WOMA_WIN32_APPLICATION && defined WINDOWS_PLATFORM 
	// Convert WINDOWS CommandLine --> ARGc & ARGc:
	WOMA::ARGv = CommandLineToArgvA(GetCommandLineA(), &WOMA::ARGc); // NOTE: CommandLine is allways CHAR
#endif

#endif

	// [4] Set A Top level "Exception handler" for all Exceptions. Catch, Dump & Send Report WOMA ENGINE HOME using FTP!
	// -------------------------------------------------------------------------------------------
	WOMA::miniDumper = NEW MiniDumper();	// NOTE: After logManager!

	// [6-1] Start LINUX Platform GUI: Settings
	// -------------------------------------------------------------------------------------------
	// ALSO: After logManager!

	// [7] Start LINUX Platform: "OpenGL" (LoadExtensions)
	// -------------------------------------------------------------------------------------------
	glutInit(&WOMA::ARGc, WOMA::ARGv);
}

void OS_ENGINE_STOP()
{
	CoUninitialize();

	SAFE_DELETE(WOMA::miniDumper);		// Free Top level Exception handler & Mini-Dumper.

	WOMA::logManager->ShutdownInstance();	// Write, Close & Free: The logManager.
	WOMA::logManager = NULL;				// Because of STATIC Classes Shutdown: Do not log

#ifdef RELEASE
	WOMA::DeleteDirectory(WOMA::womaTempPATH.c_str());	// Delete TEMP files
#endif
}
