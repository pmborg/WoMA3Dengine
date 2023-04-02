// --------------------------------------------------------------------------------------------
// Filename: stackTrace.h
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
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567891;

#ifndef _objModelClass_H_
#define _objModelClass_H_

#include "main.h"

#include <DbgHelp.h>
#include <stdio.h>
#include <conio.h>

#include <string>
#include <iostream>
#include <sstream>

#pragma comment(lib, "psapi.lib")

class stack_trace  
{  
public:  
    stack_trace(CONTEXT* context, size_t skip);
  
    std::string to_string();
  
private:  
    static const size_t max_frame_ptrs = 16;  
    DWORD64 m_frame_ptrs[max_frame_ptrs];  
};  

#endif