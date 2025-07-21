// --------------------------------------------------------------------------------------------
// Filename: textFontClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234525217;


#include "platform.h"
#if defined USE_RASTERTEK_TEXT_FONT //&& defined DX_ENGINE
#include "OSengine.h"
#pragma warning(disable : 4473) // Disable warning C4473: 'printf' : not enough arguments passed for format string
#pragma warning(disable : 4005) // Disable warning C4005: '' : macro redefinition
#pragma warning(disable : 4244) // warning C4244: '=': conversion from 'int' to 'float', possible

#include "textFontClass.h"
#include "virtualModelClass.h"
#include "mem_leak.h"

#if defined DX_ENGINE
#include "dxWinSystemClass.h"
#endif
#if defined DX11
#include "dx11Class.h"
#endif
#if defined DX12
#include "dx12Class.h"
#endif

textFontClass::textFontClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

}

textFontClass::~textFontClass() { Shutdown(); CLASSDELETE();}

bool textFontClass::Initialize(void* g_driver, TCHAR* fontFilename, TCHAR* textureFilename)
{
	HRESULT hr = S_FALSE;
	_tprintf(TEXT("textFontClass::Initialize ()\n"));

	if (!LoadFontData(WOMA::LoadFile(fontFilename)))				// Load in the text file containing the font data.
		return false;

	// [PATTERN] Image loader:
#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_driver = (DirectX::DX12Class*)g_driver;
		m_Texture = NEW DX12TextureClass;
		IF_NOT_THROW_EXCEPTION(m_Texture);
		bool result = m_Texture->Initialize(g_driver, WOMA::LoadFile(textureFilename), 0, /*wrap*/ false);	// Initialize the texture object:
		if (result)
			hr = S_OK;
	}
#endif
#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
	{
		m_driver11 = (DirectX::DX11Class*)g_driver;
		//[TEMMPLATE] LOAD TEXTURE DX11:
		#define m_driver11 ((DirectX::DX11Class*)m_driver11)
		LOADTEXTURE(WOMA::LoadFile(textureFilename), m_Texture11);
		if (SUCCEEDED(hr)) {
			meshSRV11.push_back(m_Texture11);
		}
		else {
			return S_FALSE;
		}
	}
#endif
#if (defined OPENGL3 || defined OPENGL4)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		// Create the texture object for this model:
		gl_Texture = NEW GLtextureClass;
		IF_NOT_THROW_EXCEPTION(gl_Texture);

		// Initialize the texture object:
		bool result = gl_Texture->Initialize(WOMA::LoadFile(textureFilename), 0, false/*Model3D*/);
		if (!result)
		{
			WomaMessageBox(textureFilename, TEXT("Texture File not found")); return false;
		}
	}
#endif	

	return true;
}

void textFontClass::Shutdown()
{
#if defined DX11
	meshSRV11.clear();
#endif
#if defined DX12
	SAFE_DELETE(m_Texture);
#endif
#if (defined OPENGL3 || defined OPENGL4)
	SAFE_SHUTDOWN(gl_Texture);
#endif
	ReleaseFontData();	// Release the font data.
}

bool textFontClass::LoadFontData(TCHAR* filename)
{
	ifstream fin;
	int i;
	char temp;

	_tprintf(TEXT("textFontClass::LoadFontData(%s)\n"), filename);

	// Create the font spacing buffer.
	m_Font = NEW FontType[95];
	IF_NOT_THROW_EXCEPTION (m_Font);

	// Read in the font size and spacing between chars.
	CHAR file[MAX_STR_LEN] = { 0 }; wtoa(file, filename, 100);
	fin.open(file);
	womalogauto(TEXT("LoadFontData: %s\n"), file);
	if(fin.fail()){return false;}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		//ignore 1st column
		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);

		//ignore 2nd column
		fin.get(temp);
		printf("%c ", temp);
		while(temp != ' ')
			fin.get(temp);

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
		printf("L: %f R: %f S: %i \n", m_Font[i].left, m_Font[i].right, m_Font[i].size);
	}
	
	fin.close();	// Close the file.

	return true;
}


void textFontClass::ReleaseFontData()
{
	// Release the font data array.
	SAFE_DELETE_ARRAY(m_Font);
}

#if defined DX9 || defined DX11
ID3D11ShaderResourceView* textFontClass::GetTexture11()
{
	return m_Texture11;
}
#endif
#if defined DX12
DX12TextureClass* textFontClass::GetTexture()
{
	return m_Texture;
}
#endif
void textFontClass::BuildVertexArray(void* vertices, TCHAR* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;
	//WomaDriverClass* m_Driver = driverList[SystemHandle->AppSettings->DRIVER];

	if (m_Driver->RenderfirstTime)
		_tprintf(TEXT("[%d]: BuildVertexArray(%s, drawX: %f, drawY: %f)\n"), gettid(), sentence, drawX, drawY);

	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)_tcslen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + m_spaceSize;
		}
		else
		{
			// First triangle in quad.
			#define _pos1 drawX, drawY, 0.0f
			#define _tex1 m_Font[letter].left, 0.0f

			#define _pos2 (drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f
			#define _tex2 m_Font[letter].right, 1.0f

			#define _pos3 drawX, (drawY - m_fontHeight), 0.0f
			#define _tex3 m_Font[letter].left, 1.0f

			// Second triangle in quad.
			#define _pos4 drawX, drawY, 0.0f
			#define _tex4 m_Font[letter].left, 0.0f

			#define _pos5 drawX + m_Font[letter].size, drawY, 0.0f
			#define _tex5 m_Font[letter].right, 0.0f

			#define _pos6 (drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f
			#define _tex6 m_Font[letter].right, 1.0f
#if defined DX_ENGINE
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(_pos1);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(_tex1);
			index++;

			vertexPtr[index].position = XMFLOAT3(_pos2);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(_tex2);
			index++;

			vertexPtr[index].position = XMFLOAT3(_pos3);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(_tex3);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(_pos4);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(_tex4);
			index++;

			vertexPtr[index].position = XMFLOAT3(_pos5);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(_tex5);
			index++;

			vertexPtr[index].position = XMFLOAT3(_pos6);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(_tex6);
			index++;
#else
			vertexPtr[index].position = { _pos1 };
			vertexPtr[index].texture = { _tex1 };									// Top left.
			index++;

			vertexPtr[index].position = { _pos2 };  // Bottom right.
			vertexPtr[index].texture ={ _tex2 };
			index++;

			vertexPtr[index].position = { _pos3 };							// Bottom left.
			vertexPtr[index].texture ={ _tex3 };
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = { _pos4 };						// Top left.
			vertexPtr[index].texture ={ _tex4 };
			index++;

			vertexPtr[index].position = { _pos5 };  // Top right.
			vertexPtr[index].texture ={ _tex5 };
			index++;

			vertexPtr[index].position = { _pos6 };  // Bottom right.
			vertexPtr[index].texture ={ _tex6 };
			index++;
#endif
			
#if defined yes_please_debug_me
			if (m_Driver->RenderfirstTime) {
				_tprintf("XMFLOAT3(drawX: %f, drawY: %f, %f) ", _pos1); _tprintf("XMFLOAT2(u: %f, v: %f, %f)\n", _tex1);
			}
			if (m_Driver->RenderfirstTime) {
				_tprintf("XMFLOAT3(drawX: %f, drawY: %f, %f) ", _pos2); _tprintf("XMFLOAT2(u: %f, v: %f, %f)\n", _tex2);
			}
			if (m_Driver->RenderfirstTime) {
				_tprintf("XMFLOAT3(drawX: %f, drawY: %f, %f) ", _pos3); _tprintf("XMFLOAT2(u: %f, v: %f, %f)\n", _tex3);
			}
			if (m_Driver->RenderfirstTime) {
				_tprintf("XMFLOAT3(drawX: %f, drawY: %f, %f) ", _pos4); _tprintf("XMFLOAT2(u: %f, v: %f, %f)\n", _tex4);
			}
			if (m_Driver->RenderfirstTime) {
				_tprintf("XMFLOAT3(drawX: %f, drawY: %f, %f) ", _pos5); _tprintf("XMFLOAT2(u: %f, v: %f, %f)\n", _tex5);
			}
			if (m_Driver->RenderfirstTime) {
				_tprintf("XMFLOAT3(drawX: %f, drawY: %f, %f) ", _pos6); _tprintf("XMFLOAT2(u: %f, v: %f, %f)\n", _tex6);
			}
#endif

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 2.0f;
		}
	}
}


int textFontClass::GetSentencePixelLength(char* sentence)
{
	int pixelLength, numLetters, i, letter;

	pixelLength = 0;
	numLetters = (int)strlen(sentence);
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;
		// If the letter is a space then count it as three pixels.
		if (letter == 0)
		{
			pixelLength += m_spaceSize;
		}
		else
		{
			pixelLength += (m_Font[letter].size + 1);
		}
	}

	return pixelLength;
}


int textFontClass::GetFontHeight()
{
	return (int)m_fontHeight;
}

#endif
