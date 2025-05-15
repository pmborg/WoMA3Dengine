// --------------------------------------------------------------------------------------------
// Filename: Rinputclass
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
//WomaIntegrityCheck = 1234567155;

#include "platform.h"
#include "Rinputclass.h"

RInputClass::RInputClass()
{
}


RInputClass::~RInputClass()
{
}


void RInputClass::Initialize()
{
  // Initialize the keyboard state.
  for(int i=0; i<256; i++)
  {
    m_keyboardState[i] = false;
  }
  
  return;
}


void RInputClass::KeyDown(int keySymbol)
{
  if(keySymbol == 65307) { m_keyboardState[KEY_ESCAPE] = true; }
  
  return;
}


void RInputClass::KeyUp(int keySymbol)
{
  if(keySymbol == 65307) { m_keyboardState[KEY_ESCAPE] = false; }
  
  return;
}


bool RInputClass::IsEscapePressed()
{
  return m_keyboardState[KEY_ESCAPE];
}
