// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: log.cpp
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
//
// PURPOSE: Output - LOG INFO and FATAL ERRORs to "report".txt file
//
// --------------------------------------------------------------------------------------------
#include "main.h"
#include <shlwapi.h>
#include "log.h"
#include "mem_leak.h"
#include "OSmain_dir.h"
#include "language.h"

#define MAXBUFF 5*KBs

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

public:
	LogManager();
	~LogManager();

	TCHAR* getLogFileName();

	void DEBUG_MSG(WCHAR* strMsg, ...);

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

//-------------------------------------------------------------------------------------------
LogManager::LogManager()
//-------------------------------------------------------------------------------------------
{
	//private:
	debugFile = NULL; // Our "debug file" pointer

	//public:
	REPORT_FILE = PUBLIC_DOCUMENTS;
	REPORT_FILE.append(TEXT("REPORT.txt"));

	// OPEN FILE:
	UINT errno_t = _tfopen_s(&debugFile, REPORT_FILE.c_str(), TEXT("w"));
	if (errno_t != 0) {
		// This means that the file is locked by another Woma APP instance, dont abort because of that!
		//CANT USE WomaMessageBox:
		MessageBox(NULL, REPORT_FILE.c_str(), TEXT("WARNING! - LOG MANGER - Cant Open for Write"), MB_ICONWARNING);
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
	printf("%s", x);									//NOTE: Cant Use here: _tprintf
	fwrite(x, sizeof(char), strlen(x), debugFile);	//NOTE: Cant Use here: _tcslen
}

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
#ifdef UNICODE
	OutputDebugString(WstrBuffer);	// Input is Wchar
#else
	CHAR strBuffer[MAXBUFF] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, WstrBuffer, -1, strBuffer, MAXBUFF, NULL, NULL); //Note: Cant use: wtoa
	OutputDebugString(strBuffer);	// Input is char
#endif

	// OUTPUT: OS CONSOLE & FILE ".txt":
#if defined(UNICODE)
//	if (ON)
//		OutputDebugStringReportW( WstrBuffer );
	CHAR strBuffer[MAXBUFF] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, WstrBuffer, -1, strBuffer, MAXBUFF, NULL, NULL); //Note: Cant use: wtoa
#endif
	//#else
	if (ON)
		OutputDebugStringReport(strBuffer);

}

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
#ifdef UNICODE
	WCHAR WstrBuffer[MAXBUFF] = { 0 };
	atow(WstrBuffer, strBuffer, MAXBUFF);	 // char ==> wchar
	OutputDebugString(WstrBuffer);		// Input is Wchar
#else
	OutputDebugString(strBuffer);	// Input is Wchar
#endif

	// OUTPUT: OS CONSOLE & FILE ".txt":
	if (ON)
		OutputDebugStringReport(strBuffer);
}



	int endian()
	{
		short int word = 0x0001;
		char* byte = (char*)&word;
		return (byte[0] ? LITTLE_ENDIAN : BIG_ENDIAN);
	}

	void start_log_manager()
	{
		// [2]  LogManager::CreateInstance (After init_os_main_dirs)!
		// -------------------------------------------------------------------------------------------
		logManager = ILogManager::CreateInstance();
		WOMA_LOGManager_DebugMSGAUTO(TEXT("LogManager Started\n"));

		// [3]  PRINT Log Dirs: (After init_os_main_dirs & After logManager)
		// -------------------------------------------------------------------------------------------
		WOMA_LOGManager_DebugMSGAUTO(TEXT("Init: logDirs()\n"));
		logDirs();

		// [5] Log Binary Type:
		// -------------------------------------------------------------------------------------------
		// ALSO: After logManager!
#ifdef _DEBUG
		WOMA_LOGManager_DebugMSGAUTO(TEXT("Binary Type: [DEBUG]\n"));
#else
		WOMA_LOGManager_DebugMSG("Binary Type: [NDEBUG]\n");
#endif

	}

}