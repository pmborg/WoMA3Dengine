// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: dumpUploader.cpp
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
// PURPOSE: Upload the "file".dmp and "report".txt to Woma Server (woma.servegame.com) via FTP
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#include "WinSystemClass.h"
#include "OSmain_dir.h"

#ifdef UNICODE
#include <AtlBase.h>	//CW2T
#include <AtlConv.h>	//CW2T
#endif

#include "uh_ctrl.h"
#include "ut_clnt.h"
#include "ftp_c.h"

#include "stateMachine.h"
#include "log.h"

using namespace std;	//endl
#include <sstream>		//wstring
#include "dumpUploader.h"
#include <Shlwapi.h>

namespace WOMA 
{
int dumpUploader(STRING file)
// --------------------------------------------------------------------------------------------
{
	int retcode = 0;
	static CUT_FTPClient ftpClient;

	STRING w_filetosend = file;
	STRING w_fileToSave = file;

    //Define Target of DUMP uploader:
	TCHAR addr[256] = TEXT("woma.servegame.com");
	TCHAR user[256] = TEXT("userftp");
	TCHAR pass[256] = TEXT("woma");
	TCHAR account[256];

	Publish_Quit_Message();			// FATAL, non expected error!

	STRING REPORT_FILE = WOMA::logManager->getLogFileName();
	SAFE_DELETE (WOMA::logManager);	// Write, Close and Free the "Log" file (after free MiniDumper)

	//CONNECT:
	if (_tcslen(addr) > 0)
	{
		retcode = ftpClient.FTPConnect(addr, user, pass, account);
		if (retcode == UTE_SUCCESS){
			int index = 0;
			// v4.2 GetMultiLineResponse refactored
			size_t size = 0;
			_TCHAR	pBuf[MAX_PATH + 1];
			*pBuf = 0;
			while (ftpClient.GetMultiLineResponse(pBuf, MAX_PATH, index, &size) == UTE_SUCCESS) {
				index++;
			}
		}
		else {
			return retcode;
		}
	}

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)"\nSend DMP File: %s\n", w_filetosend.c_str());

	//SEND DUMP FILE:
	{
	#ifdef UNICODE
		CW2T filetosend(w_filetosend.c_str());	// Convert std::wstring -> _TCHAR
		CW2T fileToSave(w_fileToSave.c_str());	// Convert std::wstring -> _TCHAR
		retcode = ftpClient.SendFile(filetosend, fileToSave);
	#else
		retcode = ftpClient.SendFileA((char*)w_filetosend.c_str(), (char*)w_fileToSave.c_str());
	#endif
		if (retcode != UTE_SUCCESS)
			return retcode;
	}

	{
	//SEND "report.txt" FILE:
	w_fileToSave += TEXT("_report.txt");

	WOMA_LOGManager_DebugMSGAUTO((TCHAR*)"\nSend TXT File: %s\n", REPORT_FILE.c_str());

	//SEND TXT FILE:
	#ifdef UNICODE
		CW2T filetosend(REPORT_FILE.c_str());	// Convert std::wstring -> _TCHAR
		CW2T fileToSave(w_fileToSave.c_str());	// Convert std::wstring -> _TCHAR
		retcode = ftpClient.SendFile(filetosend, fileToSave);
	#else
	retcode = ftpClient.SendFileA((char*)REPORT_FILE.c_str(), (char*)w_fileToSave.c_str());
	#endif
		if (retcode != UTE_SUCCESS)
			return retcode;
	}

	//DISCONNECT:
	if (ftpClient.Close() != UTE_SUCCESS) {
		return retcode;
	}

	return retcode;
}

} // WOMA
