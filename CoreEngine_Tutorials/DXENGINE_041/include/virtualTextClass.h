// --------------------------------------------------------------------------------------------
// Filename: virtualTextClass.h
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#pragma once

#include "platform.h"

#include "main.h"
#include "womadriverclass.h"
#include "vertexTypes.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: VirtualTextClass
////////////////////////////////////////////////////////////////////////////////
class VirtualTextClass
{
public:
	virtual void Shutdown()=0;

	virtual bool Initialize(void* g_driver)=0;
	virtual bool InitializeTexture(void* Driver) = 0;
	virtual bool UpdateSentence(SentenceType*, TCHAR*, int, int, float, float, float)=0;
	virtual bool InitializeSentence(SentenceType**, int)=0;
	
	virtual void ReleaseSentence(SentenceType**)=0;
	virtual void RenderSentence(SentenceType*)=0;
};

