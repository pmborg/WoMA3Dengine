// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
#include "platform.h"
#if defined ALLOW_PRINT_SCREEN_SAVE_PNG
#include "dx11Class.h"
#include "ImageLoaderClass.h"
#include "mem_leak.h"

// ----------------------------------------------------------------------------------------------
// Copy Image from Back-Buffer:
// ----------------------------------------------------------------------------------------------
namespace DirectX {

// Original Sourco code: HUMUS:
// ----------------------------------------------------------------------------------------------
ImageLoaderClass* DX11Class::CaptureScreenShot(int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = screenWidth;
	desc.Height = screenHeight;
	desc.Format = BUFFER_COLOR_FORMAT;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = ReqMSAAmultiSampleCount;
	desc.SampleDesc.Quality = ReqMSAAquality - 1;		// MSAA
	desc.BindFlags = 0;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	bool result = false;

	ImageLoaderClass* img = NULL;

	ID3D11Texture2D *texture;
	if (SUCCEEDED(m_device->CreateTexture2D(&desc, NULL, &texture)))
	{
		if (msaaSamples > 1)
		{
			ID3D11Texture2D *resolved = NULL;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;

			if (SUCCEEDED(m_device->CreateTexture2D(&desc, NULL, &resolved)))
			{
				m_deviceContext->ResolveSubresource(resolved, 0, DX11windowsArray[0].m_backBuffer, 0, desc.Format);
				m_deviceContext->CopyResource(texture, resolved);
				resolved->Release();
			}
		}
		else
		{
			m_deviceContext->CopyResource(texture, DX11windowsArray[0].m_backBuffer);
		}

		D3D11_MAPPED_SUBRESOURCE map;
		if (SUCCEEDED(m_deviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &map)))
		{

			//backBufferFormat?
			if (BUFFER_COLOR_FORMAT == DXGI_FORMAT_R10G10B10A2_UNORM)
			{
				//uint32 *dst = (uint32 *)img.create(FORMAT_RGB10A2, width, height, 1, 1);
				img = NEW ImageLoaderClass(FORMAT_RGB10A2, screenWidth, screenHeight, 1, 1);
				uint32 *dst = (uint32 *)img->getPixels();

				ubyte *src = (ubyte *)map.pData;

				for (int y = 0; y < screenHeight; y++)
				{
					for (int x = 0; x < screenWidth; x++)
					{
						dst[x] = ((uint32 *)src)[x] | 0xC0000000;
					}
					dst += screenWidth;
					src += map.RowPitch;
				}
			}
			else
			{
				//ubyte *dst = img.create(FORMAT_RGB8, width, height, 1, 1);
				img = NEW ImageLoaderClass(FORMAT_RGB8, screenWidth, screenHeight, 1, 1);
				ubyte *dst = img->getPixels();

				ubyte *src = (ubyte *)map.pData;

				for (int y = 0; y < screenHeight; y++)
				{
					for (int x = 0; x < screenWidth; x++)
					{
						dst[3 * x + 0] = src[4 * x + 0];
						dst[3 * x + 1] = src[4 * x + 1];
						dst[3 * x + 2] = src[4 * x + 2];
					}
					dst += screenWidth * 3;
					src += map.RowPitch;
				}
			}
			
			result = true;

			m_deviceContext->Unmap(texture, 0);
		}

		texture->Release();
	}

	return img;
}
}

#endif