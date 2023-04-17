// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: GLtextureClass.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// PURPOSE:
//
// --------------------------------------------------------------------------------------------

#include "platform.h"

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "GLtextureClass.h"
#include "log.h"

GLtextureClass::GLtextureClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

	m_textureID = NULL;
	loaded = false;
}

GLtextureClass::~GLtextureClass() {CLASSDELETE();}

bool GLtextureClass::Initialize(TCHAR* filename, unsigned int textureUnit, bool wrap)
{
	// Load the Image file.
	loaded = loadImage(filename, wrap);

	return loaded;
}


void GLtextureClass::Shutdown()
{
	// If the texture was loaded then make sure to release it on shutdown.
	if(loaded)
	{
		glDeleteTextures(1, &m_textureID);
		loaded = false;
	}
}

#pragma warning( push )
#pragma warning( disable : 4996 )
bool GLtextureClass::loadImage(TCHAR *fileName, bool wrap)
{
	unsigned char* pixels=NULL;  // Image in 32bits pixels
	UINT imageSize;

	const TCHAR *extension = _tcsrchr(fileName, '.');
	if (extension == NULL) return false;

	if (_tcsicmp(extension, TEXT(".jpg")) == 0 || _tcsicmp(extension, TEXT(".jpeg")) == 0)
	{
		pixels = ImageLoader.loadJPEG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	}
	else if (_tcsicmp(extension, TEXT(".png")) == 0)
	{
		pixels = ImageLoader.loadPNG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	}
	else if (_tcsicmp(extension, TEXT(".tga")) == 0)
	{
		pixels = ImageLoader.loadTGA(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	}
	else if (_tcsicmp(extension, TEXT(".bmp")) == 0)
	{
		pixels = ImageLoader.loadBMP(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	}
	else if (_tcsicmp(extension, TEXT(".tif")) == 0)
	{
		pixels = ImageLoader.loadTIF(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	}
	else if (_tcsicmp(extension, TEXT(".dds")) == 0)
	{
		pixels = ImageLoader.loadDDS(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	}
	else {
		WomaFatalException ("IMAGE: Format not supported!");
		return false;
	}

	if (!pixels)
		return false;

	AddTexture(pixels, width, height, wrap);

	// Release the image loaded from file
	SAFE_DELETE_ARRAY (pixels);

	return true;
}
#pragma warning( pop )

void GLtextureClass::AddTexture(unsigned char* image32b, int width, int height, bool wrap)
{
	glGenTextures(1, &m_textureID);				// Generate an ID for the texture.
	glBindTexture(GL_TEXTURE_2D, m_textureID);	// Bind the texture as a 2D texture.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image32b); // Load/Copy the image data into the texture unit.

	//TEXTURE 1D: glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, ss.wrapS);
	//TEXTURE 2D: glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, ss.wrapT);
	//TEXTURE 3D: glTexParameteri(glTarget, GL_TEXTURE_WRAP_R, ss.wrapR);

	// Set the texture color to either wrap around or clamp to the edge.
	if(wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ 0x2900);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ 0x2900);
	}
	
	// Set the texture filtering: Anisotropic filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);		// Generate mipmaps for the texture.
}




