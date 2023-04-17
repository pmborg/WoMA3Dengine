// --------------------------------------------------------------------------------------------
// Filename: DX12TextureClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2014
// -------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// ********************************************************************************************
#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "DX12Class.h"

// DX12 includes
#include <dxgi1_4.h>	// Always 1st!	(Select Driver)
#include <d3d12.h>		// DX12			(Select Device)
#include <D3Dcompiler.h>// Use Compiler
#include <DirectXMath.h>// Use Math
using namespace DirectX;

#include "ImageLoaderClass.h"

//////////////
// INCLUDES //
//////////////
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: DX12TextureClass
////////////////////////////////////////////////////////////////////////////////
class DX12TextureClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	DX12TextureClass();
	~DX12TextureClass();

	bool Initialize(void* g_driver, TCHAR* filename, unsigned int textureUnit, bool wrap);
	void Shutdown();

private:
	bool loaded;
	ImageLoaderClass ImageLoader;

	bool loadImage(void* g_driver, TCHAR *fileName, bool=true);
	void AddTexture(void* g_driver, unsigned char* image32b, int width, int height, int depth, bool wrap);

public:
	int width = 0; 
	int height = 0;

	ComPtr<ID3D12Resource> uploadOnlyTexture;	// Buuffer on RAM to upload to GPU
	ComPtr<ID3D12Resource> m_pTexture;			// Resource on GPU
};
