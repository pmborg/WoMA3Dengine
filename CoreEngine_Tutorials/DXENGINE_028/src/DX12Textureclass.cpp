// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DX12TextureClass.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : woma.servegame.com
//
// PURPOSE:
//
// --------------------------------------------------------------------------------------------

#include "platform.h"

#include "DX12TextureClass.h"
#include "log.h"

#include "DX12Class.h"

DX12TextureClass::DX12TextureClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567891;

	//m_textureID = NULL;
	loaded = false;
}

DX12TextureClass::~DX12TextureClass() {CLASSDELETE();}

bool DX12TextureClass::Initialize(void* g_driver, TCHAR* filename, unsigned int textureUnit, bool wrap)
{
	// Load the Image file.
	loaded = loadImage(g_driver, filename, wrap);

	return loaded;
}


void DX12TextureClass::Shutdown()
{
	// If the texture was loaded then make sure to release it on shutdown.
	if(loaded)
	{
		//glDeleteTextures(1, &m_textureID);
		loaded = false;
	}
}

#pragma warning( push )
#pragma warning( disable : 4996 )
// ----------------------------------------------------------------------------------------
bool DX12TextureClass::loadImage(void* g_driver, TCHAR *fileName, bool wrap)
// ----------------------------------------------------------------------------------------
{
	UINT imageSize = 0;
	unsigned char* pixels=NULL;  // Image in 32bits pixels
	UINT width, height;			 // Image Dimensions

	const TCHAR *extension = _tcsrchr(fileName, '.');
	if (extension == NULL) return false;

	if (_tcsicmp(extension, TEXT(".jpg")) == 0 || _tcsicmp /*stricmp*/(extension, TEXT(".jpeg")) == 0)
	{
		pixels = ImageLoader.loadJPEG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits); // Note: AddTexture use 32bits Image!
	}
	else if (_tcsicmp(extension, TEXT(".png")) == 0)
	{
		pixels = ImageLoader.loadPNG(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits); // Note: AddTexture use 32bits Image!
	}
	else if (_tcsicmp(extension, TEXT(".tga")) == 0)
	{
		pixels = ImageLoader.loadTGA(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits); // Note: AddTexture use 32bits Image!
	}
	else if (_tcsicmp(extension, TEXT(".bmp")) == 0)
	{
		pixels = ImageLoader.loadBMP(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits); // Note: AddTexture use 32bits Image!
	}
	else if (_tcsicmp(extension, TEXT(".tif")) == 0)
	{
		pixels = ImageLoader.loadTIF(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits); // Note: AddTexture use 32bits Image!
	}
	else if (_tcsicmp(extension, TEXT(".dds")) == 0)
	{
		pixels = ImageLoader.loadDDS(fileName, &width, &height, &imageSize, LOAD_IMAGE_32bits); // Note: AddTexture use 32bits Image!
	}

	else {
		WomaFatalExceptionW(TEXT("IMAGE: Format not supported!"));
		return false;
	}

	if (!pixels)
		return false;

	AddTexture(g_driver, pixels, width, height, LOAD_IMAGE_32bits, wrap); // 32bits Image!

	// Release the image loaded from file
	SAFE_DELETE_ARRAY (pixels);

	return true;
}
#pragma warning( pop )

// ----------------------------------------------------------------------------------------
void DX12TextureClass::AddTexture (void* g_driver, unsigned char* image32b, int _width, int _height, int depth, bool wrap)
// ----------------------------------------------------------------------------------------
{
	DirectX::DX12Class* m_driver = (DirectX::DX12Class*)g_driver;
	#define m_pDevice m_driver->m_device
	
	width = _width;
	height = _height;

	ASSERT(depth == 32);

	// Describe and create a Texture2D:
	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = m_driver->BUFFER_COLOR_FORMAT; //DXGI_FORMAT_UNKNOWN; // 
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE; //D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; //D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // 

	ThrowIfFailed (m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_pTexture)));

	UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_pTexture.Get(), 0, 1);
		
	// Create the GPU upload buffer:
	ThrowIfFailed (m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadOnlyTexture)));


	// Copy data to the intermediate upload heap and then schedule a copy 
	// from the upload heap to the Texture2D.
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = image32b;
	textureData.RowPitch = width * (depth / 8); // TexturePixelSize;
	textureData.SlicePitch = textureData.RowPitch * height;

	UpdateSubresources(m_driver->m_commandList.Get(), m_pTexture.Get(), uploadOnlyTexture.Get(), 0, 0, 1, &textureData);
	m_driver->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTexture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

