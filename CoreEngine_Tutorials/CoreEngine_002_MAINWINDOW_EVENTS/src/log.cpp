// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: log.cpp
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
// PURPOSE: Output - LOG INFO and FATAL ERRORs to "report".txt file
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "main.h"
#if defined USE_LOG_MANAGER
#include "OSengine.h"

#if defined WINDOWS_PLATFORM
#include <shlwapi.h>
#endif

#if defined ANDROID_PLATFORM && !defined NewWomaEngine
#include "AndroidEngine.h"
#endif

#include "log.h"

#include "OSmain_dir.h"
#include "language.h"

namespace WOMA
{
class LogManager : public ILogManager
{
public:
#ifdef UNICODE
	void OutputDebugStringReportW(WCHAR* x);
#endif
	void OutputDebugStringReport(char* x);

	STRING REPORT_FILE;	// Report FileName
	FILE* debugFile;	// WOMA "debug file" pointer
	bool ON;			// Log is ON

	LogManager();
	~LogManager();

	TCHAR* getLogFileName();

#if defined WINDOWS_PLATFORM
	void DEBUG_MSG(WCHAR* strMsg, ...);
#endif

	void DEBUG_MSG(CHAR* strMsg, ...);
};

//-------------------------------------------------------------------------------------------
ILogManager* ILogManager::CreateInstance()
//-------------------------------------------------------------------------------------------
{
	logManager = NEW LogManager; // Just to allow CLASSLOADER() immediately
	CLASSLOADER();

	return logManager;
}

void ILogManager::ShutdownInstance()
{
	if (logManager)
		delete ((LogManager*)logManager);
}
#if defined ANDROID_PLATFORM
std::string android_temp_folder(struct android_app* app) 
{
	JNIEnv* env;
	app->activity->vm->AttachCurrentThread(&env, NULL);

	jclass activityClass = env->FindClass("android/app/NativeActivity");
	jmethodID getCacheDir = env->GetMethodID(activityClass, "getCacheDir", "()Ljava/io/File;");
	jobject cache_dir = env->CallObjectMethod(app->activity->clazz, getCacheDir);

	jclass fileClass = env->FindClass("java/io/File");
	jmethodID getPath = env->GetMethodID(fileClass, "getPath", "()Ljava/lang/String;");
	jstring path_string = (jstring)env->CallObjectMethod(cache_dir, getPath);

	const char* path_chars = env->GetStringUTFChars(path_string, NULL);
	std::string temp_folder(path_chars);

	env->ReleaseStringUTFChars(path_string, path_chars);
	app->activity->vm->DetachCurrentThread();
	return temp_folder;
}
#endif
//-------------------------------------------------------------------------------------------
LogManager::LogManager()
//-------------------------------------------------------------------------------------------
{
	//private:
	debugFile = NULL; // Our "debug file" pointer

	//public:
#if defined ANDROID_PLATFORM
	REPORT_FILE = android_temp_folder(engine.app);
	REPORT_FILE.append(FILE_REPORT_LOG);
#else
	REPORT_FILE = PUBLIC_DOCUMENTS;
	REPORT_FILE.append(FILE_REPORT_LOG);
#endif

	// OPEN FILE:
	UINT errno_t = _tfopen_s(&debugFile, REPORT_FILE.c_str(), TEXT("w"));
	if (errno_t != 0) {
		// This means that the file is locked by another Woma APP instance, dont abort because of that!
		//CANT USE WomaMessageBox:
		WOMA_LOGManager_DebugMSG(TEXT("[ERROR] WARNING! - LOG MANGER - Cant Open for Write: %s\n"), REPORT_FILE.c_str());
		#if defined WINDOWS_PLATFORM
		MessageBox(NULL, REPORT_FILE.c_str(), TEXT("WARNING! - LOG MANGER - Cant Open for Write"), MB_ICONWARNING);
		#endif
		ON = false;
	}
	else
		ON = true;

	_tprintf(TEXT("*LOG FILE: %s\n"), REPORT_FILE.c_str()); // NOTE: Need to be "_tprintf"
}

LogManager::~LogManager()
{
	if (logManager)
	{
		CLASSDELETE(); // Exception need to be before...

		if (CLASS_LOAD_N != CLASS_DELETE_N)
			WOMA_LOGManager_DebugMSG(TEXT("N_CLASS_LOAD != N_CLASS_DELETE\n"));

		if (debugFile)
		{
			WOMA_LOGManager_DebugMSG(TEXT("-------------------------------------------------------------------------------\n"));
			WOMA_LOGManager_DebugMSG(TEXT("THE END! WoMA Author: %s\n"), TEXT(WOMAAUTHOR_ID));
			WOMA_LOGManager_DebugMSG(TEXT("-------------------------------------------------------------------------------\n"));

			fclose(debugFile);
			debugFile = NULL;
		}
	}

}

TCHAR* LogManager::getLogFileName()
{
	return (TCHAR*)REPORT_FILE.c_str();
}

// PUBLIC FUNCTIONS:

#ifdef UNICODE
//------------------------------------------------------------------
void LogManager::OutputDebugStringReportW(WCHAR* x)
//------------------------------------------------------------------
{
	_tprintf(x);
	fwprintf(debugFile, x);
}
#endif

//------------------------------------------------------------------
void LogManager::OutputDebugStringReport(char* x)
//------------------------------------------------------------------
{
	_tprintf("%s", x);									//NOTE: Cant Use here: _tprintf
	fwrite(x, sizeof(char), strlen(x), debugFile);	//NOTE: Cant Use here: _tcslen
}

#if defined WINDOWS_PLATFORM
//-------------------------------------------------------------------------------------------
void LogManager::DEBUG_MSG(WCHAR* strMsg, ...)
//-------------------------------------------------------------------------------------------
{
	WCHAR WstrBuffer[MAXBUFF] = { 0 };

	// Get Full STRING and process STRING patameters:
	va_list args;
	va_start(args, strMsg);
	vswprintf_s(WstrBuffer /*+ wcslen(WstrBuffer)*/, MAXBUFF /*- wcslen(WstrBuffer)*/, strMsg, args);
	WstrBuffer[MAXBUFF - 1] = 0;
	va_end(args);

	// OUTPUT: messages on Dev Studio Console (Windows)
#if defined WINDOWS_PLATFORM
#ifdef UNICODE
	OutputDebugString(WstrBuffer);	// Input is Wchar
#else
	CHAR strBuffer[MAXBUFF] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, WstrBuffer, -1, strBuffer, MAXBUFF, NULL, NULL); //Note: Cant use: wtoa
	OutputDebugString(strBuffer);	// Input is char
#endif
#endif

	// OUTPUT: OS CONSOLE & FILE ".txt":
#if defined(UNICODE)
	CHAR strBuffer[MAXBUFF] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, WstrBuffer, -1, strBuffer, MAXBUFF, NULL, NULL); //Note: Cant use: wtoa
#endif
	if (ON)
		OutputDebugStringReport(strBuffer);

#ifdef _DEBUG
	fflush(debugFile);
#endif
}
#endif

//-------------------------------------------------------------------------------------------
void LogManager::DEBUG_MSG(CHAR* strMsg, ...)
//-------------------------------------------------------------------------------------------
{
	CHAR strBuffer[MAXBUFF] = { 0 }; // Force to USE CHAR on UNICODE!

	// Get Full STRING and process STRING patameters:
	va_list args;
	va_start(args, strMsg);
	vsprintf_s(strBuffer /*+ strlen(strBuffer)*/, MAXBUFF /*- strlen(strBuffer)*/, strMsg, args);
	strBuffer[MAXBUFF - 1] = 0;
	va_end(args);

	// OUTPUT: IN VISUAL STUDIO CONSOLE:
#if defined WINDOWS_PLATFORM
#ifdef UNICODE
	WCHAR WstrBuffer[MAXBUFF] = { 0 };
	atow(WstrBuffer, strBuffer, MAXBUFF);	// char ==> wchar
	OutputDebugString(WstrBuffer);			// Input is Wchar
#else
	OutputDebugString(strBuffer);			// Input is Wchar
#endif
#endif

	// OUTPUT: OS CONSOLE & FILE ".txt":
	if (ON)
		OutputDebugStringReport(strBuffer);

#ifdef _DEBUG
	fflush(debugFile);
#endif
}

void start_log_manager()
{
	// [2]  LogManager::CreateInstance (After init_os_main_dirs)!
	// -------------------------------------------------------------------------------------------
#if defined USE_LOG_MANAGER
	logManager = ILogManager::CreateInstance();
#endif
	WOMA_LOGManager_DebugMSGAUTO(TEXT("LOGMANAGER STARTED\n"));
	WOMA_LOGManager_DebugMSGAUTO(TEXT("------------------------------------------------------------------------------------------\n"));

	// [3]  PRINT Log Dirs: (After init_os_main_dirs & After logManager)
	// -------------------------------------------------------------------------------------------
	//WOMA_LOGManager_DebugMSGAUTO(TEXT("Init: logDirs()\n"));
	logDirs();

	// [5] Log Binary Type:
	// -------------------------------------------------------------------------------------------
	// ALSO: After logManager!
#if defined _DEBUG || defined DEBUG
	WOMA_LOGManager_DebugMSGAUTO(TEXT("Binary Type: [DEBUG]\n"));
#else
	WOMA_LOGManager_DebugMSG("Binary Type: [NDEBUG]\n");
#endif

}

}//namespace

#endif
