// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: textFontClass.cpp
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
// This code was inpired on: https://www.rastertek.com/dx11tut12.html
// --------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"
#if defined USE_RASTERTEK_TEXT_FONT && defined DX_ENGINE

#pragma warning( disable : 4005 )	// Disable warning C4005: '' : macro redefinition
#include "dxWinSystemClass.h"
#include "textFontClass.h"
#include "virtualModelClass.h"
#include "mem_leak.h"

#include "dx11Class.h"
#include "dx12Class.h"

textFontClass::textFontClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

}

textFontClass::~textFontClass() { Shutdown(); CLASSDELETE();}

bool textFontClass::Initialize(void* g_driver, TCHAR* fontFilename, TCHAR* textureFilename)
{
	HRESULT hr = S_FALSE;

	if (!LoadFontData(WOMA::LoadFile(fontFilename)))				// Load in the text file containing the font data.
		return false;

	// [PATTERN] Image loader:
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_driver = (DirectX::DX12Class*)g_driver;
		m_Texture = NEW DX12TextureClass;
		IF_NOT_THROW_EXCEPTION(m_Texture);
		bool result = m_Texture->Initialize(g_driver, WOMA::LoadFile(textureFilename), 0, /*wrap*/ false);	// Initialize the texture object:
		if (result)
			hr = S_OK;
	}
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
	{
		m_driver11 = (DirectX::DX11Class*)g_driver;
		//[TEMMPLATE] LOAD TEXTURE DX11:
		#define m_driver11 ((DirectX::DX11Class*)m_driver11)
		LOADTEXTURE(textureFilename, m_Texture11);
		if (SUCCEEDED(hr)) {
			meshSRV11.push_back(m_Texture11);
		}
		else {
			return S_FALSE;
		}
	}
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		hr = D3DXCreateTextureFromFile(((DX_CLASS*)g_driver)->m_device, WOMA::LoadFile(textureFilename), &m_Texture9);
		IF_FAILED_RETURN_FALSE(hr);
	}
#endif
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		// Create the texture object for this model:
		gl_Texture = NEW GLtextureClass;
		IF_NOT_THROW_EXCEPTION(gl_Texture);

		// Initialize the texture object:
		bool result = gl_Texture->Initialize(WOMA::LoadFile(textureFilename), 0, false/*Model3D*/);
		if (!result)
		{
			WOMA::WomaMessageBox(textureFilename, TEXT("Texture File not found")); return false;
		}
	}

	return true;
}

void textFontClass::Shutdown()
{
	meshSRV11.clear();

	SAFE_SHUTDOWN(gl_Texture);
	ReleaseFontData();	// Release the font data.
}

bool textFontClass::LoadFontData(TCHAR* filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = NEW FontType[95];
	IF_NOT_THROW_EXCEPTION (m_Font);

	// Read in the font size and spacing between chars.
	CHAR file[MAX_STR_LEN] = { 0 }; wtoa(file, filename, 100);
	fin.open(file);
	if(fin.fail()){return false;}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);

		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}
	
	fin.close();	// Close the file.

	return true;
}


void textFontClass::ReleaseFontData()
{
	// Release the font data array.
	SAFE_DELETE_ARRAY(m_Font);
}

ID3D11ShaderResourceView* textFontClass::GetTexture11()
{
	return m_Texture11;
}
#if defined DX9sdk
LPDIRECT3DTEXTURE9 textFontClass::GetTexture()
{
	return m_Texture9;
}
#endif
DX12TextureClass* textFontClass::GetTexture()
{
	return m_Texture;
}
void textFontClass::BuildVertexArray(void* vertices, TCHAR* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)_tcslen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	int m_fontHeight = 18;

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + 2.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - m_fontHeight), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 3.0f;
		}
	}
}

#endif
