// --------------------------------------------------------------------------------------------
// Filename: GLtextureClass.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// PURPOSE:
//
// --------------------------------------------------------------------------------------------

#include "platform.h"

#if (defined OPENGL3 || defined OPENGL4) && DX_ENGINE_LEVEL >= 22
#include "OSengine.h"
#include "WomaDriverClass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "GLTextureClass.h"
#include "log.h"




#if defined ANDROID_PLATFORM
#if defined GLES2
#include "C:\WoMAengine2023\ExternalTools\GL3Plus\GL\glcorearb.h"
#endif

#include "defs.h"
#include "AndroidEngine.h"
#endif

GLtextureClass::GLtextureClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567155;

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
	unsigned char* pixels = NULL;  // Image in 32bits pixels
	UINT imageSize;
	int decodeResult=0;

	_tprintf(TEXT("[%d]: GLtextureClass::loadImage(%s)\n"), gettid(), fileName);

#if defined ANDROID_PLATFORM_
	//C:\WoMAengine2023\Android-WomaEngine\Android2\Android2.Packaging\ARM64\Debug\Package\assets\engine\data
	//C:\WoMAengine2023\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft PNG seafloor.dds
/*
	* -JPEG
	* -PNG
	* -GIF
	* -WebP
	* -BMP
	* -ICO
	* -WBMP
	* -HEIF
*/
	//AAssetManager* assetManager = NULL; // engine_state.app->activity->assetManager;
	AAssetManager* assetManager = engine_state.app->activity->assetManager;
	auto spAndroidRobotTexture = TextureAsset::loadAsset(assetManager, fileName);

#else
	const TCHAR *extension = _tcsrchr(fileName, '.');
	if (extension == NULL) return false;

#if defined USE_IMAGE_JPG
	if (_tcsicmp(extension, TEXT(".jpg")) == 0 || _tcsicmp(extension, TEXT(".jpeg")) == 0)
	{
		pixels = ImageLoader.loadJPEG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_PNG
	if (_tcsicmp(extension, TEXT(".png")) == 0)
	{
		pixels = ImageLoader.loadPNG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_TGA
	if (_tcsicmp(extension, TEXT(".tga")) == 0)
	{
		pixels = ImageLoader.loadTGA(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_BMP
	if (_tcsicmp(extension, TEXT(".bmp")) == 0)
	{
		pixels = ImageLoader.loadBMP(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_TIFF
	if (_tcsicmp(extension, TEXT(".tif")) == 0)
	{
		pixels = ImageLoader.loadTIF(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
#if defined USE_IMAGE_DDS
	if (_tcsicmp(extension, TEXT(".dds")) == 0)
	{
		pixels = ImageLoader.loadDDS(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits);
	} else 
#endif
	{
		WomaFatalException ("IMAGE: Format not supported!");
		return false;
	}

	if (!pixels)
		return false;

	AddTexture(pixels, width, height, wrap);
	
	SAFE_DELETE_ARRAY(pixels);	// Release the image loaded from file
#endif

	return true;
}
#pragma warning( pop )

void GLtextureClass::AddTexture(unsigned char* image32b, int width, int height, bool wrap)
{
	// Create the texture

	GLenum  err;
	//glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &m_textureID);				// Generate an ID for the texture.
	err = glGetError(); if (err != GL_NO_ERROR) {_tprintf(TEXT("glGenTextures err: %04x"), err);}
	glBindTexture(GL_TEXTURE_2D, m_textureID);	// Bind the texture as a 2D texture.
	err = glGetError(); if (err != GL_NO_ERROR) {_tprintf(TEXT("glBindTexture err: %04x"), err);}

	//32bits
#if defined WINDOWS_PLATFORM
	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image32b); // Load/Copy the image data into the texture unit: WINDOWS/ANDROID
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image32b);	// LINUX & ANDROID
#endif 

	// Set the texture color to either wrap around or clamp to the edge.
	if (wrap)
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

	// Set the texture filtering: Anisotropic filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);		// Generate mipmaps for the texture.
	err = glGetError(); 
	if (err != GL_NO_ERROR) 
		{ _tprintf(TEXT("ERROR! glGenerateMipmap err: %04x"), err); }
}

#endif