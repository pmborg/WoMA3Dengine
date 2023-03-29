// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: ImageLoaderClass.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// Supported Formats:
//
// JPG	JPG Compress
// PNG  ZLIB Compress
// BMP  Uncompressed
// TGA  Uncompressed
// DDS
// TIF
//
// --------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "platform.h"
#include "mem_leak.h"

#include <setjmp.h>
#include "ImageLoaderClass.h"

ImageLoaderClass::ImageLoaderClass() 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567890;

	//private:
	pixels = NULL;

	//public:
	m_textureID = NULL;
}

ImageLoaderClass::ImageLoaderClass(const FORMAT fmt, const int w, const int h, const int d, const int mipMapCount, const int arraysize)
{
	CLASSLOADER();

	format = fmt;
	width = w;
	height = h;
	depth = d;
	nMipMaps = mipMapCount;
	arraySize = arraysize;

	//public:
	m_textureID = NULL;
	
	pixels = NEW unsigned char[w*h*3];	
}

ImageLoaderClass::~ImageLoaderClass() 
{
	//SAFE_DELETE (pixels);
	CLASSDELETE();
}

unsigned char* Convert24to32bits(unsigned char* src, UINT imageSize32bits, bool fixEndian, bool fixTarga=false)
{
	unsigned char* dest = NEW unsigned char[imageSize32bits];
	IF_NOT_THROW_EXCEPTION(dest);
	UINT channel = 0;

	for (UINT i = 0; i < imageSize32bits; i += 4)
	{
		if (fixTarga)							//TGA
		{ 
			// RBGA:
			dest[i+2] = src[channel++];			// R
			dest[i] = src[channel++];			// G
			dest[i+1] = src[channel++];			// B
			dest[i + 3] = 0;					// Add Alfa channel: 0 = no transparent
		}
		else
		{
			if (fixEndian)						//JPG
			{
				// BGRA:
				dest[i+2] = src[channel++];		// R
				dest[i + 1] = src[channel++];	// G
				dest[i] = src[channel++];		// B
				dest[i + 3] = 0;				// Add Alfa channel: 0 = no transparent
			}
			else
			{
				// RGBA:
				dest[i] = src[channel++];		// R
				dest[i + 1] = src[channel++];	// G
				dest[i + 2] = src[channel++];	// B
				dest[i + 3] = 0;				// Add Alfa channel: 0 = no transparent
			}
		}
	}

	free(src);		// Free old 24 bits image
	return dest;	// return the 32 bits image
}

unsigned char* Convert32to24bits(unsigned char* src, UINT imageSize24bits)
{
	unsigned char* dest = NEW unsigned char[imageSize24bits];
	IF_NOT_THROW_EXCEPTION(dest);
	UINT channel = 0;

	for (UINT i = 0; i < imageSize24bits; i += 3)
	{
		dest[i] = src[channel++];		// R
		dest[i + 1] = src[channel++];	// G
		dest[i + 2] = src[channel++];	// B
		channel++;						// Skip Alfa
	}

	free(src);		// Free old 32 bits image
	return dest;	// return the 24 bits image
}

// SOURCE: http://www.libtiff.org/libtiff.html
////////////////////////////////////////////////////////////////////////////////
// TIF
///////////////////////////////////////////////////////////////////////////////
#include "tiffio.h"

unsigned char* ImageLoaderClass::loadTIF (TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits)
{
	pixels = NULL;
	uint32 npixels;
	uint32* raster = NULL;

  #if defined UNICODE
	TIFF* tif = TIFFOpenW(filename, "r");
  #else
	TIFF* tif = TIFFOpen(filename, "r");
  #endif
    if (tif) 
	{
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, height);

		npixels = (*width) * (*height);
		*imageSize = npixels * sizeof (uint32);

		//raster = (uint32*) _TIFFmalloc(*imageSize);
		pixels = NEW unsigned char[*imageSize];
		raster = (uint32*)pixels;
		if (raster != NULL) 
			ASSERT( TIFFReadRGBAImage(tif, *width, *height, raster, 0) );

		TIFFClose(tif);
	}
	else {
			STRING err = TEXT("File not found: "); err += filename;
			WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), /*Fatal*/TRUE);
			WomaFatalExceptionW((TCHAR*)err.c_str());
	}

	pixels = (unsigned char*)raster; //32bits

	if (loadimagebits == LOAD_IMAGE_24bits) // Convert 32 bits to 24 bits ?
	{
		*imageSize = (*width) * (*height) * 3;
		return Convert32to24bits(pixels, *imageSize);
	}

	//fixEndian: // BGRA: --> RGBA
	for (UINT i = 0; i < *imageSize; i += 4)
	{
		char color = pixels[i];				
		pixels[i] = pixels[i + 2];			// R
		//pixels[i + 1] = src[channel++];	// G
		pixels[i + 2] = color;				// B
		//pixels[i + 3] = 0;				// Add Alfa channel: 0 = no transparent
	}

	// Invert the Image:
	unsigned char* bottomUpImage = NEW unsigned char[*imageSize];
	IF_NOT_THROW_EXCEPTION(bottomUpImage);

	UINT rowSize = (*width) * 4;
	for (UINT i = 0; i < (*height); i++)
		memcpy(bottomUpImage + i * rowSize, pixels + ((*height) - i - 1) * rowSize, rowSize);

	free(pixels);

	return bottomUpImage; //32 bits image on TIFF!
}

////////////////////////////////////////////////////////////////////////////////
// JPG
///////////////////////////////////////////////////////////////////////////////


// ORIGINAL SOURCE: Humus
// JPEG LIB: D:\projects\SoftPixelEngine(v.3.2)\SoftPixelEngine\repository\sources\Plugins\jpeglib
// http://osdn.jp/projects/sfnet_softpixelengine/downloads/SoftPixel%20Engine%20SDK/SoftPixelEngine%28v.3.2%29.zip/

#include "/WoMAengine2023/_EXTERNAL_/IMAGING/JPG_LIB/include/jpeglib.h"

unsigned char* ImageLoaderClass::loadJPEG(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits)
{
	pixels = NULL; // Reset before load

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE *file;
	if ((file = _tfopen(filename, TEXT("rb"))) == NULL)
	{
		STRING err = TEXT("File not found: "); err += filename;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), /*Fatal*/TRUE);
		WomaFatalExceptionW((TCHAR*)err.c_str());
	}
	
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	bool FORMAT_RGB8 = false;
	bool FORMAT_RGBA8 = false;
	switch (cinfo.num_components)
	{
		case 1:
			//format = FORMAT_I8;
			woma_exception("FORMAT_I8: Error reading file!", __FILE__, __FUNCTION__, __LINE__);
			break;
		case 3:
			FORMAT_RGB8 = true;
			break;
		case 4:
			FORMAT_RGBA8 = true;
			break;
	}

	*width  = cinfo.output_width;
	*height = cinfo.output_height;

	*imageSize = (*width) * (*height) * cinfo.num_components;
	pixels = NEW unsigned char[*imageSize];
	IF_NOT_THROW_EXCEPTION (pixels);

	unsigned char *curr_scanline = pixels;

	while (cinfo.output_scanline < cinfo.output_height){
		jpeg_read_scanlines(&cinfo, &curr_scanline, 1);
		curr_scanline += (*width) * cinfo.num_components;
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(file);

	if (loadimagebits == LOAD_IMAGE_32bits)	// Convert 24 bits to 32 bits ?
	{
		if (FORMAT_RGB8) // 24 bits?
		{
			*imageSize = (*width) * (*height) * 4;
			return Convert24to32bits(pixels, *imageSize, true); //true for JPG
		}
	}

	return pixels; //24 or 32 bits image!
}

////////////////////////////////////////////////////////////////////////////////
// PNG
///////////////////////////////////////////////////////////////////////////////
// FUNCTION: From SoftPixel
//
/* Checks whether the supplied bytes match the PNG signature.  We allow
 * checking less than the full 8-byte signature so that those apps that
 * already read the first few bytes of a file to determine the file type
 * can simply check the remaining bytes for extra assurance.  Returns
 * an integer less than, equal to, or greater than zero if sig is found,
 * respectively, to be less than, to match, or be greater than the correct
 * PNG signature (this is the same behaviour as strcmp, memcmp, etc).
 */

#define png_memcmp		memcmp
typedef unsigned char	png_byte;
typedef png_byte FAR *	png_bytep;
typedef size_t			png_size_t;
#define png_sizeof(x)	sizeof (x)

int png_sig_cmp(png_bytep sig, png_size_t start, png_size_t num_to_check)
{
   png_byte png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
   if (num_to_check > 8)
      num_to_check = 8;
   else if (num_to_check < 1)
      return (-1);

   if (start > 7)
      return (-1);

   if (start + num_to_check > 8)
      num_to_check = 8 - start;

   return ((int)(png_memcmp(&sig[start], &png_signature[start], num_to_check)));
}

// ORIGINAL SOURCE: HUMUS - _LIBS\_HUMUS\Framework3\Framework3\Imaging\Image.cpp
// http://www.humus.name/3D/Framework3.zip

#define PNG_STDIO_SUPPORTED
#include "/WoMAengine2023/_EXTERNAL_/IMAGING/ZLIB_LIB/include/zconf.h"
#include "/WoMAengine2023/_EXTERNAL_/IMAGING/ZLIB_LIB/include/zlib.h"
#include "/WoMAengine2023/_EXTERNAL_/IMAGING/PNG_LIB/include/pngconf.h"
#include "/WoMAengine2023/_EXTERNAL_/IMAGING/PNG_LIB/include/png.h"

void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length){
	fwrite(data, length, 1, (FILE *) png_get_io_ptr(png_ptr));
}

void user_flush_data(png_structp png_ptr){
	fflush((FILE *) png_get_io_ptr(png_ptr));
}

void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length){
	fread(data, length, 1, (FILE *) png_get_io_ptr(png_ptr));
}

png_voidp malloc_fn(png_structp png_ptr, png_size_t size){
	return malloc(size);
}
void free_fn(png_structp png_ptr, png_voidp ptr){
	free(ptr);
}


unsigned char* ImageLoaderClass::loadPNG(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits)
{
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
    FILE *file;

    // open the PNG input file
    if ((file = _tfopen(filename, TEXT("rb"))) == NULL) 
	{
		STRING err = TEXT("File not found: "); err += filename;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), /*Fatal*/TRUE);
		WomaFatalExceptionW((TCHAR*)err.c_str());
	}

    // first check the eight byte PNG signature
    png_byte pbSig[8];
    fread(pbSig, 1, 8, file);
    if (!png_check_sig(pbSig, 8)){
		fclose(file);
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);
	}

	// Block From SoftPixel:
    // Check for valid magic number
    if (!png_check_sig(pbSig, sizeof(pbSig)))
    {
		throw woma_exception("PNG file has incorrect magic number", __FILE__, __FUNCTION__, __LINE__);
        return 0;
    }

    // create the two png(-info) structures
    if ((png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, malloc_fn, free_fn)) == NULL){
		fclose(file);
        throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);
    }

    if ((info_ptr = png_create_info_struct(png_ptr)) == NULL){
        png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(file);
        throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);
    }

	// initialize the png structure
	png_set_read_fn(png_ptr, file, user_read_data);
	png_set_sig_bytes(png_ptr, 8);
	
	// read all PNG info up to image data
	png_read_info(png_ptr, info_ptr);

	// get width, height, bit-depth and color-type
	png_uint_32 w, h;
    int bitDepth, colorType;
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bitDepth, &colorType, NULL, NULL, NULL);

	*width = w;
	*height = h;
	int nChannels = png_get_channels(png_ptr, info_ptr);

	int rowSize = (*width) * nChannels * bitDepth / 8;

	// now we can allocate memory to store the image
	*imageSize = rowSize * (*height);
	pixels = NEW unsigned char[*imageSize];
	IF_NOT_THROW_EXCEPTION (pixels);

	// set the individual row-pointers to point at the correct offsets
    png_byte **ppbRowPointers = NEW png_bytep[*height];

	for (UINT i = 0; i < *height; i++)
		ppbRowPointers[i] = pixels + i * rowSize;

	// now we can go ahead and just read the whole image
	png_read_image(png_ptr, ppbRowPointers);

	// read the additional chunks in the PNG file (not really needed)
	png_read_end(png_ptr, NULL);
	
	SAFE_DELETE_ARRAY (ppbRowPointers); // delete [] ppbRowPointers;

	// and we're done
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);

	if (colorType == PNG_COLOR_TYPE_PALETTE)
		throw woma_exception ("ERROR LOADING: Image, Palettes not supported!", __FILE__, __FUNCTION__, __LINE__);

	// Added by me:
	if (bitDepth == 8)
	{
		// Fix endian
		for (UINT i = 0; i < *imageSize; i += nChannels)
		{
			unsigned char tmp = pixels[i];
			pixels[i] = pixels[i + 2];
			pixels[i + 2] = tmp;
		}
	}

	if (bitDepth == 16)
	{
		// Fix endian
		for (UINT i = 0; i < *imageSize; i += 2)
		{
			unsigned char tmp = pixels[i];
			pixels[i] = pixels[i + 1];
			pixels[i + 1] = tmp;
		}
	}

	if (loadimagebits == LOAD_IMAGE_32bits) // Convert 24 bits to 32 bits ?
	{
		if (nChannels == 3)
		{
			*imageSize = (*width) * (*height) * 4;
			return Convert24to32bits(pixels, *imageSize, false); //32 bits image!
		}
	}

	if (loadimagebits == LOAD_IMAGE_24bits) // Convert 32 bits to 24 bits ?
	{
		if (nChannels == 4) 
		{
			*imageSize = (*width) * (*height) * 3;
			return Convert32to24bits(pixels, *imageSize); //24 bits image!
		}
	}

	return pixels; //24 or 32 bits image!
}

////////////////////////////////////////////////////////////////////////////////
// BMP
////////////////////////////////////////////////////////////////////////////////
// ORIGINAL SOURCE: rastertek terrain bmp
// http://www.rastertek.com/tertut03.html
// bool TerrainClass::LoadHeightMapTerrain(char* filename)

unsigned char* ImageLoaderClass::loadBMP (TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits)
{
	pixels = NULL; // Reset before load

	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	//int imageSize;

	//[1] Begin by opening the file and then read it into a unsigned char array. Close the file after we are finished reading the data from it.
	//    Open the height map file in binary.
	error = _tfopen_s(&filePtr, filename, TEXT("rb"));
	if(error != 0)
	{
		STRING err = TEXT("File not found: "); err += filename;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), /*Fatal*/TRUE);
		WomaFatalExceptionW((TCHAR*)err.c_str());
	}

	/*
	bitmapFileHeader:
	u16	Id;					//	BM - Windows 3.1x, 95, NT, 98, 2000, ME, XP
							//	BA - OS/2 Bitmap Array
							//	CI - OS/2 Color Icon
							//	CP - OS/2 Color Pointer
							//	IC - OS/2 Icon
							//	PT - OS/2 Pointer
	u32	FileSize;
	u32	Reserved;
	u32	BitmapDataOffset;
	*/

	//[2] Read in the "file header".
	count = (UINT) fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	//[3] Check Signature:
	if (bitmapFileHeader.bfType != 0x4d42)	// "BM"
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	/*
	u32	BitmapHeaderSize;	// should be 28h for windows bitmaps or
							// 0Ch for OS/2 1.x or F0h for OS/2 2.x
	u32 Width;
	u32 Height;
	u16 Planes;
	u16 BPP;				// 1: Monochrome bitmap
							// 4: 16 color bitmap
							// 8: 256 color bitmap
							// 16: 16bit (high color) bitmap
							// 24: 24bit (true color) bitmap
							// 32: 32bit (true color) bitmap

	u32  Compression;		// 0: none (Also identified by BI_RGB)
							// 1: RLE 8-bit / pixel (Also identified by BI_RLE4)
							// 2: RLE 4-bit / pixel (Also identified by BI_RLE8)
							// 3: Bitfields  (Also identified by BI_BITFIELDS)

	u32  BitmapDataSize;	// Size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary.
	u32  PixelPerMeterX;
	u32  PixelPerMeterY;
	u32  Colors;
	u32  ImportantColors;
	*/

	//[4] Read in the "bitmap info header".
	count = (UINT) fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	//[5] 1, 4 and 8 not supported:
	if(bitmapInfoHeader.biBitCount < 24)
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	//Store the size of the terrain so we can use these values for building the vertex and index buffers as well as rendering the terrain.

	// Save the dimensions of the terrain.
	*width = bitmapInfoHeader.biWidth;
	*height = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	*imageSize = (*width) * (*height) * bitmapInfoHeader.biBitCount / 8;

	// Allocate memory for the bitmap image data.
	unsigned char* bottomUpImage = NEW unsigned char[*imageSize];
	IF_NOT_THROW_EXCEPTION (bottomUpImage);

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap "image data".
	count = (UINT) fread(bottomUpImage, 1, (*imageSize), filePtr);
	if(count != (*imageSize))
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	// Close the file.
	if (filePtr)
		error = fclose(filePtr);

	if(error != 0)
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	// Invert the BMP Image
	pixels = NEW unsigned char[*imageSize];
	IF_NOT_THROW_EXCEPTION (pixels);

	UINT rowSize = (*width) * bitmapInfoHeader.biBitCount / 8;
    for (UINT i = 0; i < (*height); i++)
		memcpy (pixels + i * rowSize, bottomUpImage + ((*height) - i - 1) * rowSize, rowSize);

	free (bottomUpImage);

	if (loadimagebits == LOAD_IMAGE_32bits) // Convert 24 bits to 32 bits ?
	{
		if (bitmapInfoHeader.biBitCount == 24)
		{
			*imageSize = (*width) * (*height) * 4;
			return Convert24to32bits(pixels, *imageSize, false);	//32 bits image!
		}
	}

	return pixels; //24 or 32 bits image!
}

////////////////////////////////////////////////////////////////////////////////
// TGA
////////////////////////////////////////////////////////////////////////////////
// ORIGINAL SOURCE: rastertek opengl
// http://www.rastertek.com/gl40tut05.html
// bool TextureClass::LoadTarga(GLopenGLclass* OpenGL, char* filename, unsigned int textureUnit, bool wrap)

unsigned char* ImageLoaderClass::loadTGA(TCHAR* filename, UINT* width, UINT* height, UINT* imageSize, UINT loadimagebits)
{
	int error, bpp;
	FILE* filePtr;
	UINT count;
	TargaHeader targaFileHeader;

	pixels = NULL; // Reset before load

	// Open the targa file for reading in binary.
	error = _tfopen_s(&filePtr, filename, TEXT("rb"));
	if (error != 0)
	{
		STRING err = TEXT("File not found: "); err += filename;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), /*Fatal*/TRUE);
		WomaFatalExceptionW((TCHAR*)err.c_str());
	}

	// Read in the file header.
	count = (UINT)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
		throw woma_exception("File not found!", __FILE__, __FUNCTION__, __LINE__);

	bool isCompressed = (targaFileHeader.data1[2] == 10);	// Note:  2 => DeCompressed
	ASSERT(!isCompressed);									// Note: 10 => IsCompressed

	// Get the important information from the header.
	*width = (int)targaFileHeader.width;
	*height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	// Calculate the size of the 32 bit image data.
	*imageSize = (*width) * (*height) * (bpp/8);

	// Allocate memory for the targa image data.
	pixels = NEW unsigned char[*imageSize];
	IF_NOT_THROW_EXCEPTION(pixels);

	// Read in the targa image data.
	count = (UINT)fread(pixels, 1, *imageSize, filePtr);
	//if (count != *imageSize)
	//	throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
		throw woma_exception("Error reading file!", __FILE__, __FUNCTION__, __LINE__);

	// Invert the Image:
#if _INVERT_TAG_IMAGE_
	unsigned char* bottomUpImage = NEW unsigned char[*imageSize];
	IF_NOT_THROW_EXCEPTION(bottomUpImage);

	UINT rowSize = (*width) * (bpp/8);
	for (UINT i = 0; i < (*height); i++)
		memcpy(bottomUpImage + i * rowSize, pixels + ((*height) - i - 1) * rowSize, rowSize);
	free(pixels);
#else
	unsigned char* bottomUpImage = pixels;
#endif

	// Convert 24 bits to 32 bits ?
	if (loadimagebits == LOAD_IMAGE_32bits) 
	{
		if(bpp == 24) 
		{
			*imageSize = (*width) * (*height) * 4;
			return Convert24to32bits(bottomUpImage /*pixels*/, *imageSize, false, true); //32 bits image!
		}
	}
	return bottomUpImage/*pixels*/; //24 or 32 bits image!
}


////////////////////////////////////////////////////////////////////////////////
// DDS
////////////////////////////////////////////////////////////////////////////////
// ORIGINAL SOURCE: HUMUS - _LIBS\_HUMUS\Framework3\Framework3\Imaging\Image.cpp
// http://www.humus.name/3D/Framework3.zip

#define DDSD_CAPS        0x00000001
#define DDSD_HEIGHT      0x00000002
#define DDSD_WIDTH       0x00000004
#define DDSD_PITCH       0x00000008
#define DDSD_PIXELFORMAT 0x00001000
#define DDSD_MIPMAPCOUNT 0x00020000
#define DDSD_LINEARSIZE  0x00080000
#define DDSD_DEPTH       0x00800000

#define DDSCAPS_COMPLEX  0x00000008 
#define DDSCAPS_TEXTURE  0x00001000 
#define DDSCAPS_MIPMAP   0x00400000 

#define DDSCAPS2_CUBEMAP 0x00000200 
#define DDSCAPS2_VOLUME  0x00200000 

#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000
#define DDSCAPS2_CUBEMAP_ALL_FACES (DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX | DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ)

#define D3D10_RESOURCE_MISC_TEXTURECUBE 0x4
#define D3D10_RESOURCE_DIMENSION_BUFFER    1
#define D3D10_RESOURCE_DIMENSION_TEXTURE1D 2
#define D3D10_RESOURCE_DIMENSION_TEXTURE2D 3
#define D3D10_RESOURCE_DIMENSION_TEXTURE3D 4

struct DDSHeader {
	uint32 dwMagic;
	uint32 dwSize;
	uint32 dwFlags;
	uint32 dwHeight;
	uint32 dwWidth;
	uint32 dwPitchOrLinearSize;
	uint32 dwDepth; 
	uint32 dwMipMapCount;
	uint32 dwReserved[11];

	struct {
        uint32 dwSize;
		uint32 dwFlags;
		uint32 dwFourCC;
		uint32 dwRGBBitCount;
		uint32 dwRBitMask;
		uint32 dwGBitMask;
		uint32 dwBBitMask;
		uint32 dwRGBAlphaBitMask; 
	} ddpfPixelFormat;

	struct {
		uint32 dwCaps1;
		uint32 dwCaps2;
		uint32 Reserved[2];
	} ddsCaps;

	uint32 dwReserved2;
};

struct DDSHeaderDX10 {
    uint32 dxgiFormat;
    uint32 resourceDimension;
    uint32 miscFlag;
    uint32 arraySize;
    uint32 reserved;
};

// Image loading flags
#define DONT_LOAD_MIPMAPS 0x1



struct FormatString {
	FORMAT format;
	const char *string;
};

static const FormatString formatStrings[] = {
	{ FORMAT_NONE,   "NONE"  },

	{ FORMAT_R8,     "R8"    },
	{ FORMAT_RG8,    "RG8"   },
	{ FORMAT_RGB8,   "RGB8"  },
	{ FORMAT_RGBA8,  "RGBA8" },
	
	{ FORMAT_R16,    "R16"   },
	{ FORMAT_RG16,   "RG16"  },
	{ FORMAT_RGB16,  "RGB16" },
	{ FORMAT_RGBA16, "RGBA16"},

	{ FORMAT_R16F,   "R16F"    },
	{ FORMAT_RG16F,  "RG16F"   },
	{ FORMAT_RGB16F, "RGB16F"  },
	{ FORMAT_RGBA16F,"RGBA16F" },

	{ FORMAT_R32F,   "R32F"    },
	{ FORMAT_RG32F,  "RG32F"   },
	{ FORMAT_RGB32F, "RGB32F"  },
	{ FORMAT_RGBA32F,"RGBA32F" },

	{ FORMAT_RGBE8,  "RGBE8"   },
	{ FORMAT_RGB565, "RGB565"  },
	{ FORMAT_RGBA4,  "RGBA4"   },
	{ FORMAT_RGB10A2,"RGB10A2" },

	{ FORMAT_DXT1,   "DXT1"  },
	{ FORMAT_DXT3,   "DXT3"  },
	{ FORMAT_DXT5,   "DXT5"  },
	{ FORMAT_ATI1N,  "ATI1N" },
	{ FORMAT_ATI2N,  "ATI2N" },
	{ FORMAT_BC7,    "BC7"   },
};

#define FORMAT_I8    FORMAT_R8
#define FORMAT_IA8   FORMAT_RG8
#define FORMAT_I16   FORMAT_R16
#define FORMAT_IA16  FORMAT_RG16
#define FORMAT_I16F  FORMAT_R16F
#define FORMAT_IA16F FORMAT_RG16F
#define FORMAT_I32F  FORMAT_R32F
#define FORMAT_IA32F FORMAT_RG32F

// Define some useful macros
#define MCHAR2(a, b) (a | (b << 8))
#define MCHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

unsigned char *ImageLoaderClass::getPixels(const int mipMapLevel) const {
	return (mipMapLevel < nMipMaps)? pixels + getMipMappedSize(0, mipMapLevel) : NULL;
}

unsigned char *ImageLoaderClass::getPixels(const int mipMapLevel, const int arraySlice) const {
	if (mipMapLevel >= nMipMaps || arraySlice >= arraySize) return NULL;

	return pixels + getMipMappedSize(0, nMipMaps) * arraySlice + getMipMappedSize(0, mipMapLevel);
}

inline bool isSignedFormat(const FORMAT format){
	return ((format >= FORMAT_R8S) && (format <= FORMAT_RGBA16S)) || ((format >= FORMAT_R16I) && (format <= FORMAT_RGBA32I));
}

inline bool isCompressedFormat(const FORMAT format){
	return (format >= FORMAT_DXT1) && (format <= FORMAT_BC7);
}

inline bool isFloatFormat(const FORMAT format){
//	return (format >= FORMAT_R16F && format <= FORMAT_RGBA32F);
	return (format >= FORMAT_R16F && format <= FORMAT_RG11B10F) || (format == FORMAT_D32F);
}

inline bool isIntegerFormat(const FORMAT format){
	return (format >= FORMAT_R16I && format <= FORMAT_RGBA32UI);
}

// Accepts only plain formats
inline int getBytesPerChannel(const FORMAT format){
	static const int bytesPC[] = {
		1, //  8-bit unsigned
		2, // 16-bit unsigned
		1, //  8-bit signed
		2, // 16-bit signed
		2, // 16-bit float
		4, // 32-bit float
		2, // 16-bit unsigned integer
		4, // 32-bit unsigned integer
		2, // 16-bit signed integer
		4, // 32-bit signed integer
	};

	return bytesPC[(format - 1) >> 2];
}

// Does not accept compressed formats
inline int getBytesPerPixel(const FORMAT format){
	static const int bytesPP[] = {
		0,
		1, 2, 3, 4,       //  8-bit unsigned
		2, 4, 6, 8,       // 16-bit unsigned
		1, 2, 3, 4,       //  8-bit signed
		2, 4, 6, 8,       // 16-bit signed
		2, 4, 6, 8,       // 16-bit float
		4, 8, 12, 16,     // 32-bit float
		2, 4, 6, 8,       // 16-bit unsigned integer
		4, 8, 12, 16,     // 32-bit unsigned integer
		2, 4, 6, 8,       // 16-bit signed integer
		4, 8, 12, 16,     // 32-bit signed integer
		4, 4, 4, 2, 2, 4, // Packed
		2, 4, 4, 4,       // Depth
	};
	return bytesPP[format];
}

// Accepts only compressed formats
inline int getBytesPerBlock(const FORMAT format){
	return (format == FORMAT_DXT1 || format == FORMAT_ATI1N)? 8 : 16;
}

int ImageLoaderClass::getWidth(const int mipMapLevel) const {
	int a = width >> mipMapLevel;
	return (a == 0)? 1 : a;
}

int ImageLoaderClass::getHeight(const int mipMapLevel) const {
	int a = height >> mipMapLevel;
	return (a == 0)? 1 : a;
}

int ImageLoaderClass::getDepth(const int mipMapLevel) const {
	int a = depth >> mipMapLevel;
	return (a == 0)? 1 : a;
}

int ImageLoaderClass::getMipMappedSize(const int firstMipMapLevel, int nMipMapLevels, FORMAT srcFormat) const
{
	int w = getWidth (firstMipMapLevel);
	int h = getHeight(firstMipMapLevel);
	int d = getDepth (firstMipMapLevel);

	if (srcFormat == FORMAT_NONE) srcFormat = format;
	
	int size = 0;
	while (nMipMapLevels){
		if (isCompressedFormat(srcFormat)){
			size += ((w + 3) >> 2) * ((h + 3) >> 2) * d;
		} else {
			size += w * h * d;
		}
		w >>= 1;
		h >>= 1;
		d >>= 1;
		if (w + h + d == 0) break;
		if (w == 0) w = 1;
		if (h == 0) h = 1;
		if (d == 0) d = 1;

		nMipMapLevels--;
	}

	if (isCompressedFormat(srcFormat)){
		size *= getBytesPerBlock(srcFormat);
	} else {
		size *= getBytesPerPixel(srcFormat);
	}

	return (depth == 0)? 6 * size : size;
}

template <typename DATA_TYPE>
inline void swapChannels(DATA_TYPE *pixels, int nPixels, const int channels, const int ch0, const int ch1){
	do {
		DATA_TYPE tmp = pixels[ch1];
		pixels[ch1] = pixels[ch0];
		pixels[ch0] = tmp;
		pixels += channels;
	} while (--nPixels);
}

unsigned char* ImageLoaderClass::loadDDS(TCHAR* filename, UINT* Width, UINT* Height, UINT* imageSize, UINT flags, UINT loadimagebits)
{
	FILE *file;
	if ((file = _tfopen(filename, TEXT("rb"))) == NULL) 
	{
		STRING err = TEXT("File not found: "); err += filename;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), /*Fatal*/TRUE);
		WomaFatalExceptionW((TCHAR*)err.c_str());
	}

	DDSHeader header;
	fread(&header, sizeof(header), 1, file);
	if (header.dwMagic != MCHAR4('D','D','S',' '))
	{
		fclose(file);
		return false;
	}

	width  = header.dwWidth;
	height = header.dwHeight;
	*Width = width;
	*Height = height;

	depth  = (header.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP)? 0 : (header.dwDepth == 0)? 1 : header.dwDepth;
	nMipMaps = ((flags & DONT_LOAD_MIPMAPS) || (header.dwMipMapCount == 0))? 1 : header.dwMipMapCount;
	arraySize = 1;

	if (header.ddpfPixelFormat.dwFourCC == MCHAR4('D','X','1','0')){
		DDSHeaderDX10 dx10Header;
		fread(&dx10Header, sizeof(dx10Header), 1, file);

		switch (dx10Header.dxgiFormat){
			case 61: format = FORMAT_R8; break;
			case 49: format = FORMAT_RG8; break;
			case 28: format = FORMAT_RGBA8; break;

			case 56: format = FORMAT_R16; break;
			case 35: format = FORMAT_RG16; break;
			case 11: format = FORMAT_RGBA16; break;

			case 54: format = FORMAT_R16F; break;
			case 34: format = FORMAT_RG16F; break;
			case 10: format = FORMAT_RGBA16F; break;

			case 41: format = FORMAT_R32F; break;
			case 16: format = FORMAT_RG32F; break;
			case 6:  format = FORMAT_RGB32F; break;
			case 2:  format = FORMAT_RGBA32F; break;

			case 59: format = FORMAT_R16I; break;
			case 57: format = FORMAT_R16UI; break;

			case 67: format = FORMAT_RGB9E5; break;
			case 26: format = FORMAT_RG11B10F; break;
			case 24: format = FORMAT_RGB10A2; break;

			case 71: format = FORMAT_DXT1; break;
			case 74: format = FORMAT_DXT3; break;
			case 77: format = FORMAT_DXT5; break;
			case 80: format = FORMAT_ATI1N; break;
			case 83: format = FORMAT_ATI2N; break;
			case 98: format = FORMAT_BC7; break;
			default:
				fclose(file);
				return false;
		}

	} else {

		switch (header.ddpfPixelFormat.dwFourCC){
			case 34:  format = FORMAT_RG16; break;
			case 36:  format = FORMAT_RGBA16; break;
			case 111: format = FORMAT_R16F; break;
			case 112: format = FORMAT_RG16F; break;
			case 113: format = FORMAT_RGBA16F; break;
			case 114: format = FORMAT_R32F; break;
			case 115: format = FORMAT_RG32F; break;
			case 116: format = FORMAT_RGBA32F; break;
			case MCHAR4('D','X','T','1'): format = FORMAT_DXT1; break;
			case MCHAR4('D','X','T','3'): format = FORMAT_DXT3; break;
			case MCHAR4('D','X','T','5'): format = FORMAT_DXT5; break;
			case MCHAR4('A','T','I','1'): format = FORMAT_ATI1N; break;
			case MCHAR4('A','T','I','2'): format = FORMAT_ATI2N; break;
			default:
				switch (header.ddpfPixelFormat.dwRGBBitCount){
					case 8: format = FORMAT_I8; break;
					case 16:
						format = (header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xF000)? FORMAT_RGBA4 : 
								 (header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xFF00)? FORMAT_IA8 : 
								 (header.ddpfPixelFormat.dwBBitMask == 0x1F)? FORMAT_RGB565 : FORMAT_I16;
						break;
					case 24: format = FORMAT_RGB8; break;
					case 32:
						format = (header.ddpfPixelFormat.dwRBitMask == 0x3FF00000)? FORMAT_RGB10A2 : FORMAT_RGBA8;
						break;
					default:
						fclose(file);
						return false;
				}
		}
	}

	*imageSize = getMipMappedSize(0, nMipMaps);
	pixels = NEW unsigned char[*imageSize];
	if (isCube()){
		for (int face = 0; face < 6; face++){
			for (int mipMapLevel = 0; mipMapLevel < nMipMaps; mipMapLevel++){
				int faceSize = getMipMappedSize(mipMapLevel, 1) / 6;
                unsigned char *src = getPixels(mipMapLevel) + face * faceSize;

				fread(src, 1, faceSize, file);
			}
			if ((flags & DONT_LOAD_MIPMAPS) && header.dwMipMapCount > 1){
				fseek(file, getMipMappedSize(1, header.dwMipMapCount - 1) / 6, SEEK_CUR);
			}
		}
	} else {
		fread(pixels, 1, *imageSize, file);
	}
	/*
	if ((format == FORMAT_RGB8 || format == FORMAT_RGBA8) && header.ddpfPixelFormat.dwBBitMask == 0xFF){
		int nChannels = getChannelCount(format);
		swapChannels(pixels, (*imageSize) / nChannels, nChannels, 0, 2);
	}
	*/
	fclose(file);
	return pixels; //32 bits image!
}

#ifndef NO_PNG_SAVE
bool ImageLoaderClass::savePNG(TCHAR *fileName) 
{
	int type;

	switch (format) {
	case FORMAT_I8:
	case FORMAT_I16:
		type = PNG_COLOR_TYPE_GRAY;
		break;
	case FORMAT_IA8:
	case FORMAT_IA16:
		type = PNG_COLOR_TYPE_GRAY_ALPHA;
		break;
	case FORMAT_RGB8:
	case FORMAT_RGB16:
		type = PNG_COLOR_TYPE_RGB;
		break;
	case FORMAT_RGBA8:
	case FORMAT_RGBA16:
		type = PNG_COLOR_TYPE_RGBA;
		break;
	default:
		return false;
	}

	png_structp png_ptr;
	png_infop info_ptr;
	FILE *file;

	if ((file = _tfopen(fileName, TEXT("wb"))) == NULL)
	{
		STRING err = TEXT("Error Saving: "); err += fileName;
		WOMA::WomaMessageBox((TCHAR*)err.c_str(), TEXT("Error: "), /*Fatal*/TRUE);
		WomaFatalExceptionW((TCHAR*)err.c_str());
	}

	if ((png_ptr = png_create_write_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, malloc_fn, free_fn)) == NULL) {
		fclose(file);
		return false;
	}

	if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
		png_destroy_write_struct(&png_ptr, NULL);
		fclose(file);
		return false;
	}

	png_set_write_fn(png_ptr, file, user_write_data, user_flush_data);

	int bpp = (format >= FORMAT_I16) ? 16 : 8;

	png_set_IHDR(png_ptr, info_ptr, width, height, bpp, type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	//png_set_compression_level(png_ptr, 9);
	png_write_info(png_ptr, info_ptr);

	int nElements = width * getChannelCount(format);
	if (format >= FORMAT_I16) {
		ushort *line = NEW ushort[nElements];
		for (int y = 0; y < height; y++) {
			ushort *src = ((ushort *)pixels) + y * nElements;
			// Swap endian
			for (int x = 0; x < nElements; x++) {
				line[x] = (src[x] >> 8) | (src[x] << 8);
			}
			png_write_row(png_ptr, (ubyte *)line);
		}
		delete[] line;
	}
	else {
		// Added by me:
		//if (bitDepth == 8)
		{
			// Fix endian
			for (int i = 0; i < nElements*height; i += 3)
			{
				unsigned char tmp = pixels[i];
				pixels[i] = pixels[i + 2];
				pixels[i + 2] = tmp;
			}
		}

		for (int y = 0; y < height; y++) {
			png_write_row(png_ptr, pixels + y * nElements);
		}
	}

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(file);

	return true;
}
#endif // NO_PNG

