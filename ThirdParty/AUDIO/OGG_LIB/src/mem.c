#include <stdlib.h>
#include <windows.h>// 1: Windows-specific header file for the C/C++ programming language which contains declarations for all of the functions in the Windows API
#include <strsafe.h>// 3: StringCchPrintf Secure Function to Replace: sprintf

void* my_malloc(size_t size, const char *file, int line, const char *func)
{
	void *p = malloc(size);

	char fullText[256] = { 0 };
	StringCchPrintf(fullText, 256, "OGG_LIB/vorbis_LIB/vorbisfile_LIB Allocated = %s, %i, %s, %p[%li]\n", file, line, func, p, size);
	OutputDebugString(fullText);

	return p;
}