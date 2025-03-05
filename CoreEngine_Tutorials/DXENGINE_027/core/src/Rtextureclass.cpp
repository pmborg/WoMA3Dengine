////////////////////////////////////////////////////////////////////////////////
// Filename: RTextureClass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rtextureclass.h"
#include "ImageLoaderClass.h"

RTextureClass::RTextureClass()
{
    m_loaded = false;
}

/*
RTextureClass::RTextureClass(const RTextureClass& other)
{
}
*/

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

#if !true
    //UINT width, height;
    TargaHeader targaFileHeader;
    FILE* filePtr;
    int bpp, error;// , i, j;
    unsigned long count;

    // Open the targa file for reading in binary.
    filePtr = fopen(filename, "rb");
    if(filePtr == NULL)
        { return false; }

    // Read in the file header.
    count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
    if(count != 1)
    {
        return false;
    }

    // Get the important information from the header.
    m_width = (int)targaFileHeader.width;
    m_height = (int)targaFileHeader.height;
    bpp = (int)targaFileHeader.bpp;

    // Check that it is 32 bit and not 24 bit.
    if(bpp != 32)
    {
        return false;
    }

    // Calculate the size of the 32 bit image data.
    imageSize = m_width * m_height * 4;

    // Allocate memory for the targa image data.
    targaImage = new unsigned char[imageSize];

    // Read in the targa image data.
    count = fread(targaImage, 1, imageSize, filePtr);
    if(count != imageSize)
    {
        return false;
    }

    // Close the file.
    error = fclose(filePtr);
    if(error != 0)
    {
        return false;
    }
#else
    //int index, i, j;
    ImageLoaderClass ImageLoader;
    UINT Width, Height, ImageSize;
    targaImage = ImageLoader.loadTGA(filename, &Width, &Height, &ImageSize, LOAD_IMAGE_32bits);
    m_width = Width;
    m_height = Height;
    imageSize = ImageSize;
#endif

/*
    // Allocate memory for the targa destination data.
    unsigned char* targaData = new unsigned char[imageSize];

    // Initialize the index into the targa destination data array.
    index = 0;

    // Now copy the targa image data into the targa destination array in the correct order since the targa format is not stored in the RGBA order.
    for(int j=0; j<m_height; j++)
    {
        for(int i=0; i<m_width; i++)
        {
            targaData[index + 0] = targaImage[index + 2];  // Red.
            targaData[index + 1] = targaImage[index + 1];  // Green.
            targaData[index + 2] = targaImage[index + 0];  // Blue
            targaData[index + 3] = targaImage[index + 3];  // Alpha

            // Increment the indexes into the targa data.
            index += 4;
        }
    }
    */
    // Set the unique texture unit in which to store the data.
    glActiveTexture(GL_TEXTURE0);

    // Generate an ID for the texture.
    glGenTextures(1, &m_textureID);

    // Bind the texture as a 2D texture.
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Load the image data into the texture unit.
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, targaImage/*targaData*/);

#if !defined LINUX_PLATFORM
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
    /*
    // Release the targa image data.
    delete [] targaData;
    targaData = 0;
    */
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