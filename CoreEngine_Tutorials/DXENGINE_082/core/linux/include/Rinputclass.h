// --------------------------------------------------------------------------------------------
// Filename: RInputClass.h
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
// --------------------------------------------------------------------------------------------
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut14.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525256;

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#include "platform.h"

/////////////
// DEFINES //
/////////////
const int KEY_ESCAPE = 0;


////////////////////////////////////////////////////////////////////////////////
// Class name: RInputClass
////////////////////////////////////////////////////////////////////////////////
class RInputClass
{
public:
  RInputClass();
  ~RInputClass();
  
  void Initialize();

  void KeyDown(int);
  void KeyUp(int);

  bool IsEscapePressed();
    
//private:
  bool m_keyboardState[256];
};

#endif
