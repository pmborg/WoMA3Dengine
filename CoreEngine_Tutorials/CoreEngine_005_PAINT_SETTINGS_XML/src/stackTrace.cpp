// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: stackTrace.cpp
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
// Original Source Code:
// http://social.msdn.microsoft.com/Forums/vstudio/en-US/f93a211a-9c95-42f0-8581-50314457b729/generating-the-stack-traces-in-the-c-code
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "platform.h"
#include "main.h"
// --------------------------------------------------------------------------------------------
// Auxiliar Class: sym_handler
// --------------------------------------------------------------------------------------------

#include <DbgHelp.h>
#include <stdio.h>
#include <conio.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

// --------------------------------------------------------------------------------------------
#include "stackTrace.h"
// --------------------------------------------------------------------------------------------

class sym_handler  
{  
public:  
    static sym_handler& get_instance()  
    {  
        static sym_handler instance;  
        return instance;  
    }  
  
    std::string get_symbol_info(DWORD64 addr)  
    {  
		char tmp[3*KBs];
        std::stringstream ss;  

        DWORD64 displacement64 = 0;  
        DWORD displacement = 0;  
		char symbol_buffer[sizeof(SYMBOL_INFO) + 256] = {0}; //memset(symbol_buffer, 0, sizeof(symbol_buffer));

        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbol_buffer);  
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);  
        symbol->MaxNameLen = 3*KBs;
  
		IMAGEHLP_LINE64 line ={0};  
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);	// memset(&line,0,sizeof(line));
  
		const HANDLE process = GetCurrentProcess();

		sprintf_s(tmp, "%i\n", (int)addr);
		//ss << tmp;

        if (m_initialized)  
        {  

            if (SymFromAddr(process, addr, &displacement64, symbol))  
            {  
                ss << symbol->Name;  

                if (SymGetLineFromAddr64(process, addr, &displacement, &line))  
                {  
					sprintf_s(tmp, "() [%s]:%d\n", line.FileName, line.LineNumber);
					ss << tmp;  
                }  
            } else {
                #if NDEBUG // This always faild on DEBUG:
				DWORD err = GetLastError();
                WOMA_LOGManager_DebugMSGAUTO(TEXT ("ERROR: %d:\n"), err);
                WomaMessageBox(NULL, TEXT("ERROR: SymFromAddr(GetCurrentProcess(), addr, &displacement64, symbol)"));
                #endif
			}
        }  
        return ss.str();  
    }  
  
    void capture_stack_trace(CONTEXT* context, DWORD64* frame_ptrs, size_t count, size_t skip)  
    {  
        if (m_initialized)  
        {  
            CONTEXT current_context;  
            if (!context)  
            {  
				#ifdef X64
                RtlCaptureContext(&current_context);  //Note: This fail with 32bits
				#else
				// Check CC Compiler Option for: Program Database (/Zi)
				current_context.ContextFlags = CONTEXT_FULL;
				__asm
				{
				Label:
					mov[current_context.Ebp], ebp;
					mov[current_context.Esp], esp;
					mov eax, [Label];
					mov[current_context.Eip], eax;
				}
				#endif
				context = &current_context;
            }  
			current_context = *context;
  
            DWORD machine_type;  
            STACKFRAME64 frame;  
            //memset(&frame, 0, sizeof(frame));  
            frame.AddrPC.Mode = AddrModeFlat;  
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Mode = AddrModeFlat;
			frame.AddrReturn.Mode = AddrModeFlat;
			frame.AddrBStore.Mode = AddrModeFlat;
#ifdef _M_X64  
            frame.AddrPC.Offset = context->Rip;  
            frame.AddrFrame.Offset = context->Rbp;  
            frame.AddrStack.Offset = context->Rsp;  
            machine_type = IMAGE_FILE_MACHINE_AMD64;  
#else  
            frame.AddrPC.Offset = context->Eip;  
            frame.AddrPC.Offset = context->Ebp;  
            frame.AddrPC.Offset = context->Esp;  
            machine_type = IMAGE_FILE_MACHINE_I386;  
#endif 
            for (size_t i = 0; i < count + skip; i++)  
            {  
                if (StackWalk64(machine_type, GetCurrentProcess(), GetCurrentThread(),  
                                &frame, context, NULL,  
                                SymFunctionTableAccess64, SymGetModuleBase64, NULL))  
                {  
                    if (i >= skip)  
                    {  
                        frame_ptrs[i - skip] = frame.AddrPC.Offset;  
                    }  
                }  
                else  
                {  
                    break;  
                }  
            }  
        }  
    }  
  
private:  
    sym_handler()  
    {  
        m_initialized = SymInitialize(GetCurrentProcess(), NULL, TRUE) == TRUE;  
    }  
  
    ~sym_handler()  
    {  
        if (m_initialized)  
        {  
            SymCleanup(GetCurrentProcess());  
            m_initialized = false;  
        }  
    }  
  
    bool m_initialized;  
};  

stack_trace::stack_trace(CONTEXT* context, size_t skip)  
{  
	for (size_t i = 0; i < max_frame_ptrs;  ++i)
		m_frame_ptrs[i]=0;
	
    sym_handler::get_instance().capture_stack_trace(context, m_frame_ptrs, max_frame_ptrs, skip);  
  
}  
  
std::string stack_trace::to_string()  
{  
    std::stringstream ss;  
    for (size_t i = 0; i < max_frame_ptrs && m_frame_ptrs[i];  ++i)  
    {  
        ss << sym_handler::get_instance().get_symbol_info(m_frame_ptrs[i]) << "\n";  
    }  
    return ss.str();  
}  

