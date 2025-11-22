// --------------------------------------------------------------------------------------------
// Filename: woma_exception.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#pragma once

#include "platform.h"
#if defined USE_WOMA_EXCEPTION

using namespace std;//endl
#include <sstream>	//wstring
#include <fstream>	//wifstream/wofstream



// --------------------------------------------------------------------------------------------
// CLASS:
// --------------------------------------------------------------------------------------------
#if defined WINDOWS_PLATFORM
	class woma_exception : public runtime_error
#else
	class woma_exception : public exception
#endif
{
	public:
	woma_exception(const std::string &arg, const char *file, const char *function, int line);
	~woma_exception() throw();
	};
#else
	#define woma_exception(X, FILE, FUNCTION, LINE) exception(X)
#endif
