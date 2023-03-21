#pragma once

#define BIG_ENDIAN 0
#define LITTLE_ENDIAN 1

namespace WOMA
{
	extern int endian();
	extern bool GetLangStringFromLangId(DWORD dwLangID_i);
}