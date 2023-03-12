// --------------------------------------------------------------------------------------------
// Filename: stackTrace.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// ********************************************************************************************

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
