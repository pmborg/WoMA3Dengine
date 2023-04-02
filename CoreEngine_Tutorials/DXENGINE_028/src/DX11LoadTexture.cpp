// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DX11LoadTexture.cpp
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
//WomaIntegrityCheck = 1234567891;

#include "platform.h"

/*
OLD DX 11 WAY: (D3D11_SPEC_DATE_YEAR == 2009)
==============
TEXTURE COMPRESSION: http://en.wikipedia.org/wiki/S3_Texture_Compression
FMT DX9		| FMT DX10
--------------------------------
D3DFMT_DXT1 | DXGI_FORMAT_BC1_UNORM
D3DFMT_DXT2 | DXGI_FORMAT_BC2_UNORM
D3DFMT_DXT3 | DXGI_FORMAT_BC2_UNORM
D3DFMT_DXT4 | DXGI_FORMAT_BC3_UNORM
D3DFMT_DXT5 | DXGI_FORMAT_BC3_UNORM

loadInfo.Format = DXGI_FORMAT_BC1_UNORM;
D3DX11CreateTextureFromFile(...);

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
HRESULT result;
D3DX11_IMAGE_LOAD_INFO loadInfo;
ZeroMemory( &loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO) );
loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
loadInfo.Format = DXGI_FORMAT_BC3_UNORM;
loadInfo.MipLevels = D3DX11_DEFAULT; // then will produce the greatest mipmaps layer.
loadInfo.MipFilter = D3DX11_FILTER_LINEAR;

// Create a texture resource view from a file.
result = D3DX11CreateShaderResourceViewFromFile(device, filename, &loadInfo, NULL, &m_texture, NULL);
if(FAILED(result))
return false;

// These codes are debugging texture attributes ...
ID3D11Texture2D* tex;
m_texture->GetResource((ID3D11Resource**)&tex);
D3D11_TEXTURE2D_DESC texdesc;
tex->GetDesc(&texdesc);
return true;
}
*/

#include "womadriverclass.h"
#include "dx11Class.h"
//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "winsystemclass.h"	// SystemHandle


// http://directxtk.codeplex.com/wikipage?title=WICTextureLoader&referringTitle=Home
// #include <WICTextureLoader.h>

#if D3D11_SPEC_DATE_YEAR > 2009
	#include "DDSTextureLoader.h"
	#include "WICTextureLoader.h"

	#include "ImageLoaderClass.h"
	#include "DirectXTex.h"
#endif

namespace DirectX {


#if (defined DX11 || defined DX9 ) && D3D11_SPEC_DATE_YEAR > 2009
HRESULT DX11Class::LoadTexture(ID3D11Device* pDevice, TCHAR* pSrcFile, ID3D11ShaderResourceView**  ppShaderResourceView)
{
	HRESULT hr = S_OK;

	// NEW PART:
	const TCHAR *extension = _tcsrchr(pSrcFile, '.');
	if (extension == NULL) return -1;

	if (_tcsicmp(extension, TEXT(".dds")) == 0) // Use: DirectXTK.lib
	{
		#ifdef UNICODE
		hr = DirectX::CreateDDSTextureFromFile(pDevice, m_deviceContext, pSrcFile, nullptr, ppShaderResourceView);
		#else
		WCHAR DX_pSrcFile[MAX_STR_LEN] = { 0 }; MultiByteToWideChar(CP_ACP, 0, pSrcFile, -1, DX_pSrcFile, MAX_STR_LEN);
		hr = DirectX::CreateDDSTextureFromFile(pDevice, m_deviceContext, /*pSrcFile*/DX_pSrcFile, nullptr, ppShaderResourceView);
		#endif
	}
	else 
	if (_tcsicmp(extension, TEXT(".tga")) == 0)  // Use: DirectXtexconv.lib
	{
		ScratchImage image;
		#ifdef UNICODE
		hr = LoadFromTGAFile(pSrcFile, nullptr, image);
		#else
		WCHAR DX_pSrcFile[MAX_STR_LEN] = { 0 }; MultiByteToWideChar(CP_ACP, 0, pSrcFile, -1, DX_pSrcFile, MAX_STR_LEN);
		hr = LoadFromTGAFile(/*pSrcFile*/DX_pSrcFile, nullptr, image);
		#endif
		if (SUCCEEDED(hr))
		{
			hr = CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), ppShaderResourceView);
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	else 
	{
		#ifdef UNICODE
		hr = DirectX::CreateWICTextureFromFile(pDevice, m_deviceContext, pSrcFile, nullptr, ppShaderResourceView);
		#else
		// BMP, JPG, PNG, TIF -> Use: DirectXTK.lib
		WCHAR DX_pSrcFile[MAX_STR_LEN] = { 0 }; MultiByteToWideChar(CP_ACP, 0, pSrcFile, -1, DX_pSrcFile, MAX_STR_LEN);
		hr = DirectX::CreateWICTextureFromFile(pDevice, m_deviceContext, /*pSrcFile*/DX_pSrcFile, nullptr, ppShaderResourceView);
		#endif
	}

	return hr;
}
#endif


#if D3D11_SPEC_DATE_YEAR == 2009
// ----------------------------------------------------------------------------------------------
HRESULT DX11Class::CreateShaderResourceViewFromFileMANAGED(
		ID3D11Device*				pDevice,
		TCHAR*						pSrcFile,
		D3DX11_IMAGE_LOAD_INFO*		pLoadInfo,
		ID3DX11ThreadPump*			pPump,
		ID3D11ShaderResourceView**  ppShaderResourceView,
		HRESULT*					pHResult,
		bool						shrinkTexture) 
// ----------------------------------------------------------------------------------------------
{	HRESULT hr = S_OK;

	// COMMON: Check if this Texture/Resource already exist? if yes... return our Resource, already cached!
	for (UINT i=0; i < allTextureNameArray.size(); i++) {
		if (lstrcmp (allTextureNameArray[i].c_str(), pSrcFile) == 0) {
			*ppShaderResourceView = allTexturePointerArray[i];
			return hr;
		}
	}

	// ORIGINAL: http://blog.naver.com/PostView.nhn?blogId=sorkelf&logNo=220846504837
	// More:	 https://translate.google.pt/translate?hl=pt-PT&sl=ko&u=http://blog.naver.com/PostView.nhn%3FblogId%3Dsorkelf%26logNo%3D220846504837&prev=search
	{
		char *ibuf = NULL;
		UINT SrcDataSize = 0;

		loadInfo.Width = D3DX11_DEFAULT;
		loadInfo.Height = D3DX11_DEFAULT;

		if (ibuf)
			hr = D3DX11CreateShaderResourceViewFromMemory(pDevice, ibuf, SrcDataSize, (shrinkTexture) ? pLoadInfo:NULL, pPump, ppShaderResourceView, pHResult);
		else
			hr = D3DX11CreateShaderResourceViewFromFile(pDevice, pSrcFile, (shrinkTexture) ? pLoadInfo:NULL, (shrinkTexture) ? pPump:NULL, ppShaderResourceView, pHResult);

		if(FAILED(hr))
			{ WOMA::WomaMessageBox(pSrcFile, TEXT("Failed to load or Texture file not found?:")); ASSERT (!FAILED(hr)); return hr; } // ASSERT -> FATAL & return -> NO FATAL 

		// Un-comment to speed up Texturing (lower tex. resolution)
		if (shrinkTexture) // Default: Disabled...
		{
			// Get the Resource from the SRV, in order to get texture info such as width height.. 
			ID3D11Resource* textureResource;
			(*ppShaderResourceView)->GetResource( &textureResource );
			ID3D11Texture2D* diffuseTexture = (ID3D11Texture2D*)textureResource;

			// Get the texture Descriptor to obtain texture info:
			D3D11_TEXTURE2D_DESC diffuseTextureDesc;
			diffuseTexture->GetDesc( &diffuseTextureDesc );
			UINT m_iTextureWidth = diffuseTextureDesc.Width;
			UINT m_iTextureHeight = diffuseTextureDesc.Height;

			SAFE_RELEASE (textureResource);

			// Reload if too bigger, checking Our XML Texture Setting:
			//
			// TextureWidth:
			if (m_iTextureWidth > SystemHandle->AppSettings->MaxTextureSize && (m_iTextureWidth >= m_iTextureHeight)) 
			{
				loadInfo.Width = SystemHandle->AppSettings->MaxTextureSize;
				loadInfo.Height = SystemHandle->AppSettings->MaxTextureSize * m_iTextureHeight / m_iTextureWidth;
				SAFE_RELEASE (*ppShaderResourceView);
				if (ibuf)
					hr = D3DX11CreateShaderResourceViewFromMemory(pDevice, ibuf, SrcDataSize, (shrinkTexture) ? pLoadInfo:NULL, pPump, ppShaderResourceView, pHResult);
				else
					hr = D3DX11CreateShaderResourceViewFromFile(pDevice, pSrcFile, (shrinkTexture) ? pLoadInfo:NULL, (shrinkTexture) ? pPump:NULL, ppShaderResourceView, pHResult);
			}
			// TextureHeight:
			if (m_iTextureHeight > SystemHandle->AppSettings->MaxTextureSize && (m_iTextureWidth < m_iTextureHeight)) 
			{
				loadInfo.Height = SystemHandle->AppSettings->MaxTextureSize;
				loadInfo.Width =  SystemHandle->AppSettings->MaxTextureSize * m_iTextureWidth / m_iTextureHeight;
		
				SAFE_RELEASE (*ppShaderResourceView);
				if (ibuf)
					hr = D3DX11CreateShaderResourceViewFromMemory(pDevice, ibuf, SrcDataSize, (shrinkTexture) ? pLoadInfo:NULL, pPump, ppShaderResourceView, pHResult);
				else
					hr = D3DX11CreateShaderResourceViewFromFile(pDevice, pSrcFile, (shrinkTexture) ? pLoadInfo:NULL, (shrinkTexture) ? pPump:NULL, ppShaderResourceView, pHResult);
			}
		}

	}

	// COMMON:
	if (hr == S_OK) 
	{
		allTextureNameArray.push_back(pSrcFile);
		allTexturePointerArray.push_back(*ppShaderResourceView);
	}

	return hr;
}
#else
HRESULT DX11Class::LOADTEXTURE_DX11_WIN_SDK8 ( 
		ID3D11Device*			    pDevice,
		TCHAR*					    pSrcFile,
		ID3D11ShaderResourceView**  ppShaderResourceView) 
{

HRESULT hr = S_OK;

	// COMMON: Check if this Texture/Resource already exist? if yes... return our Resource, already cached!
	for (UINT i=0; i < allTextureNameArray.size(); i++) {
		if (lstrcmp (allTextureNameArray[i].c_str(), pSrcFile) == 0) {
			*ppShaderResourceView = allTexturePointerArray[i];
			return hr;
		}
	}

	hr = LoadTexture(pDevice, pSrcFile, ppShaderResourceView);

	// COMMON:
	if(FAILED(hr))
	{
		WOMA::WomaMessageBox(pSrcFile, TEXT("Texture File not found"));
	} else {
		allTextureNameArray.push_back(pSrcFile);
		allTexturePointerArray.push_back(*ppShaderResourceView);
	}

	return hr;
}
#endif

}
