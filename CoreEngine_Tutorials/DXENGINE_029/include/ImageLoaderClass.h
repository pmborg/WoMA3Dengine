// --------------------------------------------------------------------------------------------
// Filename: ImageLoaderClass.h
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

#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "main.h"

//////////////
// INCLUDES //
//////////////
#include <stdio.h>

#define LOAD_IMAGE_24bits 24
#define LOAD_IMAGE_32bits 32


#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "TIFF_LIBX64_d.lib" )
#else
#pragma comment( lib, "TIFF_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "TIFF_LIB_d.lib" )
#else
#pragma comment( lib, "TIFF_LIB.lib" )
#endif
#endif

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "ZLIB_LIBX64_d.lib" )
#else
#pragma comment( lib, "ZLIB_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "ZLIB_LIB_d.lib" )
#else
#pragma comment( lib, "ZLIB_LIB.lib" )
#endif
#endif

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "PNG_LIBX64_d.lib" )
#else
#pragma comment( lib, "PNG_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "PNG_LIB_d.lib" )
#else
#pragma comment( lib, "PNG_LIB.lib" )
#endif
#endif

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "JPG_LIBX64_d.lib" )
#else
#pragma comment( lib, "JPG_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "JPG_LIB_d.lib" )
#else
#pragma comment( lib, "JPG_LIB.lib" )
#endif
#endif

#define ALL_MIPMAPS 127

enum FORMAT {
	FORMAT_NONE     = 0,

	// Unsigned formats
	FORMAT_R8       = 1,
	FORMAT_RG8      = 2,
	FORMAT_RGB8     = 3,
	FORMAT_RGBA8    = 4,

	FORMAT_R16      = 5,
	FORMAT_RG16     = 6,
	FORMAT_RGB16    = 7,
	FORMAT_RGBA16   = 8,

	// Signed formats
	FORMAT_R8S      = 9,
	FORMAT_RG8S     = 10,
	FORMAT_RGB8S    = 11,
	FORMAT_RGBA8S   = 12,

	FORMAT_R16S     = 13,
	FORMAT_RG16S    = 14,
	FORMAT_RGB16S   = 15,
	FORMAT_RGBA16S  = 16,

	// Float formats
	FORMAT_R16F     = 17,
	FORMAT_RG16F    = 18,
	FORMAT_RGB16F   = 19,
	FORMAT_RGBA16F  = 20,

	FORMAT_R32F     = 21,
	FORMAT_RG32F    = 22,
	FORMAT_RGB32F   = 23,
	FORMAT_RGBA32F  = 24,

	// Signed integer formats
	FORMAT_R16I     = 25,
	FORMAT_RG16I    = 26,
	FORMAT_RGB16I   = 27,
	FORMAT_RGBA16I  = 28,

	FORMAT_R32I     = 29,
	FORMAT_RG32I    = 30,
	FORMAT_RGB32I   = 31,
	FORMAT_RGBA32I  = 32,

	// Unsigned integer formats
	FORMAT_R16UI    = 33,
	FORMAT_RG16UI   = 34,
	FORMAT_RGB16UI  = 35,
	FORMAT_RGBA16UI = 36,

	FORMAT_R32UI    = 37,
	FORMAT_RG32UI   = 38,
	FORMAT_RGB32UI  = 39,
	FORMAT_RGBA32UI = 40,

	// Packed formats
	FORMAT_RGBE8    = 41,
	FORMAT_RGB9E5   = 42,
	FORMAT_RG11B10F = 43,
	FORMAT_RGB565   = 44,
	FORMAT_RGBA4    = 45,
	FORMAT_RGB10A2  = 46,

	// Depth formats
	FORMAT_D16      = 47,
	FORMAT_D24      = 48,
	FORMAT_D24S8    = 49,
	FORMAT_D32F     = 50,

	// Compressed formats
	FORMAT_DXT1     = 51,
	FORMAT_DXT3     = 52,
	FORMAT_DXT5     = 53,
	FORMAT_ATI1N    = 54,
	FORMAT_ATI2N    = 55,
	FORMAT_BC7		= 56,
};

inline int getChannelCount(const FORMAT format){
	/*static*/ const int chCount[] = {
		0,
		1, 2, 3, 4,       //  8-bit unsigned
		1, 2, 3, 4,       // 16-bit unsigned
		1, 2, 3, 4,       //  8-bit signed
		1, 2, 3, 4,       // 16-bit signed
		1, 2, 3, 4,       // 16-bit float
		1, 2, 3, 4,       // 32-bit float
		1, 2, 3, 4,       // 16-bit unsigned integer
		1, 2, 3, 4,       // 32-bit unsigned integer
		1, 2, 3, 4,       // 16-bit signed integer
		1, 2, 3, 4,       // 32-bit signed integer
		3, 3, 3, 3, 4, 4, // Packed
		1, 1, 2, 1,       // Depth
		3, 4, 4, 1, 2,    // Compressed
	};
	return chCount[format];
}

// HUMUS:
// Define some sized types
typedef unsigned char uint8;
typedef   signed char  int8;

typedef unsigned short uint16;
typedef   signed short  int16;

typedef unsigned int uint32;
typedef   signed int  int32;

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;

////////////////////////////////////////////////////////////////////////////////
// Class name: ImageLoaderClass
////////////////////////////////////////////////////////////////////////////////
class ImageLoaderClass
{
private:
	// TGA
	struct TargaHeader
	{
		unsigned char	data1[12];
		/*
		uint8  descriptionlen;
		uint8  cmaptype;			//Color Map Type
		uint8  imagetype;			//Image Type
		uint16 cmapstart;			//Color Map Specification
		uint16 cmapentries;			//Color Map Specification
		uint8  cmapbits;			//Color Map Specification
		uint16 xoffset;				//Image Specification
		uint16 yoffset;				//Image Specification
		*/
		unsigned short	width;		//Image Specification
		unsigned short	height;		//Image Specification
		unsigned char	bpp;		//Image Specification
		unsigned char	attrib;		//Image Specification
	};

	unsigned char* pixels; //image in 32bits pixels

public:
	UINT WomaIntegrityCheck = 1234567829;
	ImageLoaderClass();
	ImageLoaderClass(FORMAT fmt, int w, int h, int d, int mipMapCount, int arraysize=1);
	~ImageLoaderClass();

	//DDS
	bool isCube()  const { return (depth == 0); }
	unsigned char *getPixels() const { return pixels; }
	unsigned char *getPixels(const int mipMapLevel) const;
	unsigned char *getPixels(const int mipMapLevel, const int arraySlice) const;
	int getWidth (const int mipMapLevel) const;
	int getHeight(const int mipMapLevel) const;
	int getDepth (const int mipMapLevel) const;
	int getMipMappedSize(const int firstMipMapLevel = 0, int nMipMapLevels = ALL_MIPMAPS, FORMAT srcFormat = FORMAT_NONE) const;

	unsigned char* loadJPEG	(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits = LOAD_IMAGE_32bits);
	unsigned char* loadPNG	(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits = LOAD_IMAGE_32bits);
	unsigned char* loadBMP	(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits = LOAD_IMAGE_32bits);
	unsigned char* loadTGA	(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits = LOAD_IMAGE_32bits);
	unsigned char* loadDDS  (TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT flags, UINT loadimagebits = LOAD_IMAGE_32bits);
	unsigned char* loadTIF	(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits = LOAD_IMAGE_32bits);

	bool savePNG(TCHAR *fileName);

public:
	unsigned int m_textureID;	//OPENGL3 //3.3+

	//DDS
	int width, height, depth;
	int arraySize;
	int nMipMaps;
	FORMAT format;
};
