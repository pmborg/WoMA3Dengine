// --------------------------------------------------------------------------------------------
// Filename: textFontClass.cpp
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// -------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// Used to render fast Dynamic text on screen, as orthogonal projection
//
// ********************************************************************************************
#pragma once

#include "platform.h"
#ifdef USE_RASTERTEK_TEXT_FONT // #if ENGINE_LEVEL >= 28

#pragma warning( disable : 4005 )	// Disable warning C4005: '' : macro redefinition//#include "main.h"
#include "textFontClass.h"
//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "virtualModelClass.h"
#include "mem_leak.h"

#include "dx11Class.h"
#include "dx12Class.h"

textFontClass::textFontClass()
{
	CLASSLOADER();

	//public:
	m_Font = NULL;
	m_Texture = NULL;
}

textFontClass::~textFontClass() { Shutdown(); CLASSDELETE();}

bool textFontClass::Initialize(void* g_driver, TCHAR* fontFilename, TCHAR* textureFilename)
{
	HRESULT hr = S_FALSE;

	if(!LoadFontData(WOMA::LoadFile(fontFilename)))				// Load in the text file containing the font data.
		return false;

	// [PATTERN] Image loader:
	//DX12TextureClass* m_Texture;
	m_Texture = NEW DX12TextureClass;
	IF_NOT_THROW_EXCEPTION(m_Texture);
	bool result = m_Texture->Initialize(g_driver, textureFilename, 0, /*wrap*/ false);	// Initialize the texture object:
	if (result)
		hr = S_OK;

	IF_FAILED_RETURN_FALSE(hr);

	return true;
}


void textFontClass::Shutdown()
{
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

/*
bool textFontClass::LoadTexture(void* Driver, TCHAR* filename)
{
	//if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3) 
	//	{ m_Text = NEW GLmodelClass(model3D); IF_NOT_THROW_EXCEPTION (model); }

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11) 
		if (FAILED( ((DX_CLASS*)Driver)->CreateShaderResourceViewFromFileMANAGED(((DX_CLASS*)Driver)->m_device, filename, NULL, NULL, &m_Texture, NULL, false) ))
			return false;

	return true;
}
*/

ID3D11ShaderResourceView* textFontClass::GetTexture()
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

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}


#endif//
