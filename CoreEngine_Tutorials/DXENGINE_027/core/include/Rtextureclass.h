////////////////////////////////////////////////////////////////////////////////
// Filename: RTextureClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RTextureClass_H_
#define _RTextureClass_H_

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//////////////
// INCLUDES //
//////////////
#include <stdio.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Ropenglclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RTextureClass
////////////////////////////////////////////////////////////////////////////////
class RTextureClass
{
private:
    struct TargaHeader
	{
        unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
    RTextureClass();
    //RTextureClass(const RTextureClass&);
    ~RTextureClass();

    bool Initialize(OpenGLClass*, char*, unsigned int, bool);
    void Shutdown();

    void SetTexture(OpenGLClass*);

    int GetWidth();
    int GetHeight();

private:
    bool LoadTarga32Bit(OpenGLClass*, char*, bool);

private:
    unsigned int m_textureUnit, m_textureID;
    int m_width, m_height;
    bool m_loaded;
};

#endif
#endif