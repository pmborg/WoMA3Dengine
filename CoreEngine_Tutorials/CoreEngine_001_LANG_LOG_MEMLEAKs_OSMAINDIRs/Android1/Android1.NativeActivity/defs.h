#pragma once

#include "platform.h"
#include "standard_platform.h"

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
#define _tcsrchr    strrchr
#define _tcsicmp    _stricmp
#define _stricmp	strcasecmp

#define _tfopen     fopen
#define _tfopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL

#define _tprintf(...)						(void)__android_log_print(ANDROID_LOG_ERROR,"[WOMA]", __VA_ARGS__)
#define StringCchPrintf(A,B,...)			_stprintf(A,__VA_ARGS__)
#define _stprintf							sprintf
#define strcpy_s(a,n,b)						strcpy(a,b)
#define _tcscpy_s							strcpy
#define vsprintf_s(b,l,f,v)					vsprintf(b,f,v);
#define OutputDebugString _tprintf

#define _ttof       atof

#define ZeroMemory(Destination,Length)		memset((Destination),0,(Length))
#define DeleteFile(a) std::remove(a)
//extern bool download(const std::string url, const std::string file_path);
#define URLDownloadToFile(a,url, file_path, d, e) download(url, file_path)

#define Sleep(x)							sleep(x/1000)

#define NULL								0
#define HWND UINT
#define UNREFERENCED_PARAMETER				{}
#define check(A)							{}

/* Standard data types. These are the same for emulator and library. */
typedef void				VOID;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#if !defined UNICODE
typedef char				TCHAR;
#define TEXT(quote)			(quote)   // r_winnt
#else
typedef wchar				TCHAR;
#endif

typedef int                 BOOL;

//
// typedefs
//
typedef char                CHAR;
typedef signed char         INT8;
typedef unsigned char       UCHAR;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef unsigned short      WORD;
typedef int                 INT;
//typedef signed int        INT32;
//typedef signed __int64    INT64;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef __int64_t           LONGLONG;
typedef __int64_t           LONG64;
typedef unsigned long long  UINT64;
#endif

#define GLCLEARCOLOR(...) { _tprintf ("%s", "glClearColor"); glClearColor (__VA_ARGS__);}
#define GLCLEAR(...) { _tprintf ("%s", "glClear"); glClear (__VA_ARGS__);}

//extern void ItoA(int value, char* dest, int _Radix = 10);

#ifndef _countof
#define _countof(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#endif

#ifndef S_OK
#define S_OK         ((HRESULT)0L)
#endif

#ifndef S_FALSE
#define S_FALSE      ((HRESULT)1L)
#endif

typedef LONG         HRESULT;

#define _tcslen strlen

//#define WomaMessageBox ShowAlert
#define _stscanf		sscanf
#define _strnicmp		strncasecmp
#define _stricmp		strcasecmp
#define _strnicmp		strncasecmp

#define _tcscmp         strcmp
#define _tcsicmp        _stricmp
#define _tcsicmp_l      _stricmp_l
#define _tcsnccmp       strncmp
#define _tcsncmp        strncmp
#define _tcsncicmp      _strnicmp
#define _tcsncicmp_l    _strnicmp_l
#define _tcsnicmp       _strnicmp
#define _tcsnicmp_l     _strnicmp_l

#define _tcstod			strtod
#define _tcstol			strtol

