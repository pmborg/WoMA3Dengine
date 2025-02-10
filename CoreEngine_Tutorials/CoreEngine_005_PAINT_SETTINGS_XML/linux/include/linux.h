// --------------------------------------------------------------------------------------------
// Filename: linux.h
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
#pragma once

#ifndef __LINUX_H__

//#include "platform.h"
#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>

#if defined ANDROID_PLATFORM
#define __WORDSIZE   32
#endif

#ifndef WINDOWS_PLATFORM
#include <string>
#include <string.h>
#include <stdarg.h>

typedef void				VOID;
typedef int					INT;
typedef unsigned int		UINT;
typedef unsigned short		WORD;
typedef unsigned int		DWORD;
typedef unsigned long		ULONG;
typedef unsigned char		BYTE;
typedef long				LONG;
typedef short				SHORT;
typedef unsigned short		USHORT;
typedef char				CHAR;
typedef unsigned char		UCHAR;

typedef unsigned short		WCHAR;
typedef int					WIN_BOOL;
typedef double				DATE;
typedef double				DOUBLE;
typedef long long			LONGLONG;
typedef unsigned long long  ULONGLONG;
#define HWND UINT

// PLATFORM_LINUX:
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>  

#include <cmath>	// LEVEL 8: for "sin"

//#include "linux.h"

typedef struct MyWin
{
    Display* display;
    Window		window;
    int			width;
    int			height;
    GLXContext	context;

    bool		ready;
};
extern MyWin Win;

# define __int64 long int

typedef signed char         INT8;
typedef signed short        INT16;
//typedef signed int          INT32;
typedef signed __int64      INT64;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned __int64    UINT64;

typedef ULONGLONG			DWORDLONG;

typedef unsigned __int64 size_t;

typedef char	TCHAR;

#define FALSE   0
#define TRUE    1
typedef int     BOOL;
#define NEW     new

typedef void*	HGLRC;

#define _ttof       atof

typedef int64_t LARGE_INTEGER;

typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
//#define _FILETIME_

#define TEXT(quote)	quote
#define MAX_PATH	260
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define DeleteFile(a) std::remove(a)

extern bool download(const std::string url, const std::string file_path);
#define URLDownloadToFile(a,url, file_path, d, e) download(url, file_path)

#define _tfopen     fopen
#define _tfopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
//#define sprintf_s(buffer, buffer_size, format, ...) (sprintf(buffer, format, __VA_ARGS__))
//#define sprintf_s(a,b,format, ...) vsprintf_s(a,sizeof(a),b,format, __VA_ARGS__)

#define _stprintf _tprintf
#define vsprintf_s(b,l,f,v) vsprintf(b,f,v);
#define StringCchPrintf(a,b,format, ...) sprintf (a,format, __VA_ARGS__);
#define _tcslen strlen
#define _tgetenv getenv
#define _tcscpy_s strcpy
#define strcat_s strcat
#define _tcscat_s strcat
#define _itoa ItoA
#define Sleep usleep
#define _tprintf printf

#define OutputDebugString _tprintf

#ifndef FLT_MAX
	#define FLT_MAX         3.402823466e+38F        /* max value */
#endif

#ifndef UNREFERENCED_PARAMETER 
	#define UNREFERENCED_PARAMETER(x) /* x */ 
#endif

#define _stscanf		sscanf
#define _strnicmp		strncasecmp
#define _stricmp		strcasecmp
#define _strnicmp		strncasecmp

#define _tcstod			strtod
//#define _tcsicmp		strcasecmp
#define _tcstol			strtol

/* String functions */

#define _tcschr         strchr
#define _tcscspn        strcspn
#define _tcsncat        strncat
#define _tcsncat_s      strncat_s
#define _tcsncat_l      _strncat_l
#define _tcsncat_s_l    _strncat_s_l
#define _tcsncpy        strncpy
#define _tcsncpy_s      strncpy_s
#define _tcsncpy_l      _strncpy_l
#define _tcsncpy_s_l    _strncpy_s_l
#define _tcspbrk        strpbrk
#define _tcsrchr        strrchr
#define _tcsspn         strspn
#define _tcsstr         strstr
#define _tcstok         strtok
#define _tcstok_s       strtok_s
#define _tcstok_l       _strtok_l
#define _tcstok_s_l     _strtok_s_l

#define _tcsnset        _strnset
#define _tcsnset_s      _strnset_s
#define _tcsnset_l      _strnset_l
#define _tcsnset_s_l    _strnset_s_l
#define _tcsrev         _strrev
#define _tcsset         _strset
#define _tcsset_s       _strset_s
#define _tcsset_l       _strset_l
#define _tcsset_s_l     _strset_s_l

#define _tcscmp         strcmp
#define _tcsicmp        _stricmp
#define _tcsicmp_l      _stricmp_l
#define _tcsnccmp       strncmp
#define _tcsncmp        strncmp
#define _tcsncicmp      _strnicmp
#define _tcsncicmp_l    _strnicmp_l
#define _tcsnicmp       _strnicmp
#define _tcsnicmp_l     _strnicmp_l

#define _tcscoll        strcoll
#define _tcscoll_l      _strcoll_l
#define _tcsicoll       _stricoll
#define _tcsicoll_l     _stricoll_l
#define _tcsnccoll      _strncoll
#define _tcsnccoll_l    _strncoll_l
#define _tcsncoll       _strncoll
#define _tcsncoll_l     _strncoll_l
#define _tcsncicoll     _strnicoll
#define _tcsncicoll_l   _strnicoll_l
#define _tcsnicoll      _strnicoll
#define _tcsnicoll_l    _strnicoll_l



#define IDYES 1

// GTK+		http://www.gtk.org/download/index.php
// OPENGL	https://www.opengl.org/documentation/implementations/

extern void strcpy_s(char *strDestination, const char *strSource);
extern void strcpy_s(char *strDestination, UINT numberOfElements, const char *strSource);
//extern void ItoA (int value, char* dest, int _Radix=10);

extern bool QueryPerformanceFrequency(LARGE_INTEGER *frequency);
extern bool QueryPerformanceCounter(LARGE_INTEGER *performance_count);

extern unsigned int timeGetTime();

void check(const char hdr[] = "");
#endif

#ifndef _countof
#define _countof(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#endif

#ifndef S_OK
#define S_OK         ((HRESULT)0L)
#endif

#ifndef S_FALSE
#define S_FALSE      ((HRESULT)1L)
#endif

#endif
