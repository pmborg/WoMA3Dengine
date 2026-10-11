// --------------------------------------------------------------------------------------------
// Filename: SystemClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: Export APIs for systemclass.cpp which is the common OS API
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once
#ifndef __SYSTEMCLASS_H__

#include "main.h" 
#include "default_settings_xml.h"

#if (defined LINUX_PLATFORM) || (defined ANDROID_PLATFORM)
#include <sys/utsname.h>
#endif

const int MAX_LANG_LEN = 50;


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	~SystemClass();

	void Shutdown();

	bool			mMaximized;
	void FrameUpdate();
    void ParseCommandLineAndApplySettings(int argc, char* argv[]);

public:

};

#endif
