// --------------------------------------------------------------------------------------------
// Filename: Rtextureclass.cpp
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
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rtextureclass.h"
#include "ImageLoaderClass.h"

RTextureClass::RTextureClass()
{
    m_loaded = false;
}

RTextureClass::~RTextureClass()
{
}

bool RTextureClass::Initialize(OpenGLClass* OpenGL, char* filename, unsigned int textureUnit, bool wrap)
{
    bool result;


    // Store the texture unit.
    m_textureUnit = textureUnit;

    result = LoadTarga32Bit(OpenGL, filename, wrap);
    if(!result)
    {
        return false;
    }

    // Set that the texture is loaded.
    m_loaded = true;

    return true;
}


void RTextureClass::Shutdown()
{
    // If the texture was loaded then make sure to release it on shutdown.
    if(m_loaded)
    {
        glDeleteTextures(1, &m_textureID);
	m_loaded = false;
    }

    return;
}


bool RTextureClass::LoadTarga32Bit(OpenGLClass* OpenGL, char* filename, bool wrap)
{
    unsigned char* targaImage;
    unsigned long imageSize;
    int index;

    //int index, i, j;
    ImageLoaderClass ImageLoader;
    UINT Width, Height, ImageSize;
    targaImage = ImageLoader.loadTGA(filename, &Width, &Height, &ImageSize, LOAD_IMAGE_32bits);
    m_width = Width;
    m_height = Height;
    imageSize = ImageSize;

    // Set the unique texture unit in which to store the data.
    glActiveTexture(GL_TEXTURE0);

    // Generate an ID for the texture.
    glGenTextures(1, &m_textureID);

    // Bind the texture as a 2D texture.
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Load the image data into the texture unit.
#if defined WINDOWS_PLATFORM
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, targaImage); // Load/Copy the image data into the texture unit: WINDOWS/ANDROID
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, targaImage);	// LINUX
#endif 

    if(wrap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
#if defined WINDOWS_PLATFORM
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
    }

    // Set the texture filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Generate mipmaps for the texture.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Release the targa image data now that it was copied into the destination array.
    delete[] targaImage;
    targaImage = 0;

    return true;
}


void RTextureClass::SetTexture(OpenGLClass* OpenGL)
{
    if(m_loaded)
    {
        // Set the texture unit we are working with.
        glActiveTexture(GL_TEXTURE0 + m_textureUnit);

        // Bind the texture as a 2D texture.
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    return;
}


int RTextureClass::GetWidth()
{
    return m_width;
}


int RTextureClass::GetHeight()
{
    return m_height;
}
#endif