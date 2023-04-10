// --------------------------------------------------------------------------------------------
// Filename: fileLoader.cpp
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
// PURPOSE: Get full filename inside diretory or woma.pck files depending DEBUG or RELEASE build
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567829;

#include "main.h"

#include "fileLoader.h"
#include "OSmain_dir.h" //#include "OsDirectories.h"

namespace WOMA
{
	STRING lastfile;
	STRING file;

TCHAR* LoadFile(TCHAR* filename)
{
	static TCHAR file_[MAX_STR_LEN*2];
	ZeroMemory(&file_, sizeof(file));

#ifdef RELEASE
	file = WOMA::APPDATA; // WOMA::womaTempPATH;
	file.append(filename);
	lastfile = file;
	return (TCHAR*)file.c_str();
#else
	if (filename[0]!='.')
		StringCchPrintf(file_, sizeof(file_), TEXT("%s%s"), TEXT("./"), filename);
	else
		_tcscpy_s(file_, sizeof(file_),  filename);
	file = file_;
	lastfile = file;
	return (TCHAR*)&file_;
#endif
}

}
