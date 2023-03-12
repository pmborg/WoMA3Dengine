// stdutfx.h
// standard include file for Ultimate TCPIP macros etc

// This file is not intended to be included directly by the core Ultimate TCP/IP source code.
// It is used in the samples (included in each of the respective samples stdafx.h files) to 
// centralize the TODO pragma, and may also contain version defines and warning #pragmas 
// that should not exist in the core code.

#pragma warning ( disable : 4710 )

//#if _MSC_VER < 1400
//#define WINVER 0x400
//#else 
//#define WINVER 0x503
//#endif

#if _MSC_VER < 1400
#define ULONG_PTR LONG
#define LONG_PTR LONG
#endif

// comment this out for clean compilation!
#define SHOW_TODO		

// #pragma TODO() definition
#if defined SHOW_TODO

#if !defined TODO_PRAG
#define TODO_PRAG
#define STRING2(x) #x
#define STRING(x) STRING2(x)
// usage: #pragma TODO("Changed - remove commented code when tested")
// expands to provide file and line number in output window - dclick navigation.
#define TODO(x) message ( __FILE__ "(" STRING(__LINE__) ") : " x)
#endif

#else

#define TODO(x) 

#endif

