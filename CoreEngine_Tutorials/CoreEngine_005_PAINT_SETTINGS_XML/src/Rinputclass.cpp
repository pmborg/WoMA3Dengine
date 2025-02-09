////////////////////////////////////////////////////////////////////////////////
// Filename: RInputClass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "platform.h"
//#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
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

//#endif