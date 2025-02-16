// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: woma_macros.h
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
// PURPOSE: DEFINE COMMON WorldOfMiddleAge 3D ENGINE MACROS
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#pragma once

#include "stateMachine.h"

// --------------------------------------------------------------------------------------------
// ENGINE MACROS
// --------------------------------------------------------------------------------------------

// String Converters:
// --------------------------------------------------------------------------------------------
#ifdef UNICODE
	#define atow(strW,strA,lenA) MultiByteToWideChar( CP_ACP, 0, strA, -1, strW, lenA )     // "char" to "WCHAR" converter:
	#define wtoa(strA,strW,lenA) WideCharToMultiByte(CP_ACP,0,strW,-1,strA,lenA,NULL,NULL)  // "WCHAR" to "char" converter:
#else
	#define atow(strW,strA,lenA) strcpy_s(strW,lenA,strA)	// USE SAMPLE: TCHAR Wbuffer[MAX_STR_LEN]={ 0 };	atow(Wbuffer, buffer, MAX_STR_LEN);
	#define wtoa(strA,strW,lenA) strcpy_s(strA,lenA,strW)	// USE SAMPLE: CHAR Aip[MAX_STR_LEN]={ 0 };			wtoa(Aip, ip, MAX_STR_LEN);
#endif

// int to TCHAR*
// --------------------------------------------------------------------------------------------
	#ifdef UNICODE
		#define ItoA _itow
	#else
		#define ItoA _itoa
	#endif

// Save all aplication entry command line parameter:
// --------------------------------------------------------------------------------------------
#if !defined WOMA_WIN32_APPLICATION
	#define SYSTEM_SAVE_ARGS() { \
		ARGc = argc; \
		ARGv = argv; \
	}
#else
	#define SYSTEM_SAVE_ARGS() { \
		Scmdline = pScmdline; \
		Cmdshow =  iCmdshow; \
	}
#endif

// Global Statments - WOMA Checks
// --------------------------------------------------------------------------------------------
	#define IF_FAILED_RETURN_FALSE(x)	{ if( FAILED(x) ) { return false; } }		// Used with "HRESULT" IF FAILED

#define IF_NOT_RETURN_FALSE(x)			{ if( (!(x)) ) { return false; } }			// Used with "bool" IF NOT

#if _DEBUG
	#if defined VERBOSE_MEMORY_DEBUG
		#define IF_NOT_THROW_EXCEPTION(x) { WOMA_LOGManager_DebugMSG("[MEM_DEBUG] NEW %s CREATED!\n", #x); if (!x) WomaFatalException("Not Enough Memory!"); }
	#else
		#define IF_NOT_THROW_EXCEPTION(x) { if (!x) WomaFatalException("Not Enough Memory!"); }
	#endif
#else
	#define IF_NOT_THROW_EXCEPTION(x) ASSERT(x)
#endif

#define ASSERT(x) { if (!(x)) WomaFatalException("Assert failed!"); }

#define ThrowIfFailed(hr)\
{\
	if (FAILED(hr)) { WomaFatalException("Assert failed!"); }\
}

extern const wchar_t* GetWC(const char* c);

	#ifdef UNICODE
		#define WomaFatalExceptionW( wmsg ) { CHAR msg[MAX_STR_LEN]={ 0 }; wtoa(msg, wmsg, MAX_STR_LEN); throw woma_exception(msg, __FILE__, __FUNCTION__, __LINE__); } // TODO: String convert
		#define WomaFatalException( msg ) { throw woma_exception(msg, __FILE__, __FUNCTION__, __LINE__); } // TODO: String convert
	#else
		#define WomaFatalExceptionW( msg ) throw woma_exception(msg, __FILE__, __FUNCTION__, __LINE__);
		#define WomaFatalException( msg ) throw woma_exception(msg, __FILE__, __FUNCTION__, __LINE__);
	#endif

	#define WOMA_LOGManager_DebugMSGAUTO if (WOMA::logManager) WOMA::logManager->DEBUG_MSG
	#define WOMA_LOGManager_DebugMSG	 if (WOMA::logManager) WOMA::logManager->DEBUG_MSG
	#define WOMA_LOGManager_DebugMSGW	 if (WOMA::logManager) WOMA::logManager->DEBUG_MSG

// Class Loaders - for automatic class load log
// --------------------------------------------------------------------------------------------
#define LEVELHIGHLIGHT(level) {WOMA::ENGINE_LEVEL_USED = level; if (CORE_ENGINE_LEVEL == level) {HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY|FOREGROUND_BLUE | FOREGROUND_INTENSITY);}}
#define LEVELNORMAL() { HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);}

#if _DEBUG //defined CLASS_DEBUG
#define CLASSLOADER() { HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); WOMA_LOGManager_DebugMSG((CHAR*) "[CLASS_LOAD %d] %s\n", CLASS_LOAD_N++, __FUNCTION__); SetConsoleTextAttribute(hConsole, FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE); }
#define CLASSDELETE() { HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, FOREGROUND_RED); WOMA_LOGManager_DebugMSG((CHAR*) "[CLASS_DELETE %d] %s\n", CLASS_DELETE_N++, __FUNCTION__); SetConsoleTextAttribute(hConsole, FOREGROUND_RED+FOREGROUND_GREEN+FOREGROUND_BLUE); }
#else
	#define CLASSLOADER() {}
	#define CLASSDELETE() {}
#endif

// Basic OS low level MACROS:
// --------------------------------------------------------------------------------------------
	#define OS_REDRAW_WINDOW RedrawWindow(SystemHandle->m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE| RDW_ERASE);  // Invoke: Window PAINT

		#define OS_KEY_DOWN(key) SystemHandle->m_OsInput->IsKeyDown(key)

// Global MEM HANDLERS - WOMA Macros:
// --------------------------------------------------------------------------------------------
#define KBs 1024
#define MBs (1024*KBs)
#define GBs (1024*MBs)

#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))

#ifndef SAFE_SHUTDOWN
	#if defined VERBOSE_MEMORY_DEBUG
		#define SAFE_SHUTDOWN(p) { if(p) { (p)->Shutdown(); WOMA_LOGManager_DebugMSGAUTO(TEXT("[MEM_DEBUG] SHUTDOWN %s\n"), #p); delete (p); (p)=NULL; } }
	#else
		#define SAFE_SHUTDOWN(p) { if(p) { (p)->Shutdown(); delete (p); (p)=NULL; } }
	#endif
#endif

#ifndef SAFE_RELEASE
	#if defined VERBOSE_MEMORY_DEBUG
		#define SAFE_RELEASE(p)  { if(p) { WOMA_LOGManager_DebugMSGAUTO(TEXT("[MEM_DEBUG] RELEASE %s\n"), #p); (p)->Release(); (p)=NULL; } }
	#else
		#define SAFE_RELEASE(p)  { if(p) { (p)->Release(); (p)=NULL; } }
	#endif
#endif

#ifndef SAFE_DELETE
	#if defined VERBOSE_MEMORY_DEBUG
		#define SAFE_DELETE(p)   { if(p) {  WOMA_LOGManager_DebugMSGAUTO(TEXT("[MEM_DEBUG] DELETE %s\n"), #p); delete (p); (p)=NULL; } }
	#else
		#define SAFE_DELETE(p)   { if(p) {  delete (p); (p)=NULL; } }
	#endif
#endif

#ifndef SAFE_DELETE_ARRAY
	#if defined VERBOSE_MEMORY_DEBUG
		#define SAFE_DELETE_ARRAY(p) { if(p) { WOMA_LOGManager_DebugMSGAUTO(TEXT("[MEM_DEBUG] DELETE_ARRAY %s\n"), #p); delete[] (p); (p)=NULL; } }
	#else
		#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
	#endif
#endif

// Global GAME STOP!
// --------------------------------------------------------------------------------------------

	#define Publish_Quit_Message(){ \
		for (int i = 0; i < SystemHandle->windowsArray.size(); i++)\
			::PostMessage(SystemHandle->m_hWnd, WM_CLOSE, 0, 0); /*NOTE: dont use PostQuitMessage(WM_QUIT) on mutiple threads!*/\
		WOMA::game_state = GAME_STOP; \
	} 
