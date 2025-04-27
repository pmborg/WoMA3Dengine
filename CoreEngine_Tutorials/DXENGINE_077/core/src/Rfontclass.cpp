// --------------------------------------------------------------------------------------------
// Filename: Rfontclass.h
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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut14.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "OSengine.h"
#include "Rfontclass.h"
#include "OSmain_dir.h"

RFontClass::RFontClass()
{
    m_OpenGLPtr = 0;
	m_Font = 0;
	m_Texture = 0;
}


RFontClass::RFontClass(const RFontClass& other)
{
}


RFontClass::~RFontClass()
{
}


bool RFontClass::Initialize(OpenGLClass* OpenGL, int fontChoice)
{
    char fontFilename[128];
    char fontTextureFilename[128];
	bool result;


    // Store a pointer to the OpenGL object.
    m_OpenGLPtr = OpenGL;

    // Choose one of the available fonts, and default to the first font otherwise.
    switch(fontChoice)
    {
        case 0:
        {
            strcpy(fontFilename, "engine/data/fonts/font01.txt");
            strcpy(fontTextureFilename, "engine/data/fonts/font01.tga");
		#if defined ANDROID_PLATFORM
            m_fontHeight = 80.0f;
		#else
			m_fontHeight = 40.0f;
		#endif
            m_spaceSize = 1;
            break;
        }
        case 1:
        {
            strcpy(fontFilename, "engine/data/fonts/008fontdata.txt");
            strcpy(fontTextureFilename, "engine/data/fonts/008font.tga");
            m_fontHeight = 16.0f;
            m_spaceSize = 2;
            break;
        }		
        case 2:
        {
            strcpy(fontFilename, "engine/data/fonts/008fontdata2.txt");
            strcpy(fontTextureFilename, "engine/data/fonts/008font2.tga");
            m_fontHeight = 32.0f;
            m_spaceSize = 3;
            break;
        }				
        default:
        {
            strcpy(fontFilename, "engine/data/fonts/font01.txt");
            strcpy(fontTextureFilename, "engine/data/fonts/font01.tga");
            m_fontHeight = 32.0f;
            m_spaceSize = 3;
            break;
        }
    }

	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(fontTextureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void RFontClass::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();

    // Release the pointer to the OpenGL object.
    m_OpenGLPtr = 0;

	return;
}

#if defined ANDROID_PLATFORM
class asset_streambuf : public std::streambuf {
public:
	asset_streambuf(AAsset* the_asset)
		: the_asset_(the_asset) {
		char* begin = (char*)AAsset_getBuffer(the_asset);
		char* end = begin + AAsset_getLength64(the_asset);
		setg(begin, begin, end);
	}
	~asset_streambuf() {
		AAsset_close(the_asset_);
	}
private:
	AAsset* the_asset_;
};
#endif


void read_feature(std::istream& feature_file, float* features) {
	//feature_file.seekg(sizeof(float));
	feature_file.read((char*)features, sizeof(float));
}

bool RFontClass::LoadFontData(char* filename)
{
	// Create the font spacing buffer.
	m_Font = new FontType[95];

#if !defined ANDROID_PLATFORM
	ifstream fin;
	int i;
	char temp;

	// Read in the font size and spacing between chars.
	WOMA_LOGManager_DebugMSGAUTO(TEXT("LoadFontData: %s\n"), filename);
	fin.open(filename);
	if (fin.fail())
	{
		STRING dir = WOMA::getCurrentDir();
		dir.append("/../../WoMA3Dengine/CoreEngine_Tutorials/DXENGINE_0");
		dir.append(std::to_string(DX_ENGINE_LEVEL));
		dir.append("/");
		dir.append(filename);
		fin.open(dir);
		if (fin.fail())
		{
			return false;
		}
	}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();
#else
	AAssetManager* manager = engine.app->activity->assetManager;
	AAsset* asset = AAssetManager_open(manager, filename, AASSET_MODE_BUFFER);
	asset_streambuf sb(asset);
	std::istream fin(&sb);
	char temp;

	for (int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}
#endif

	//AAsset_close(asset);
	return true;
}


void RFontClass::ReleaseFontData()
{
	// Release the font data array.
	SAFE_DELETE(m_Font);
}


bool RFontClass::LoadTexture(char* textureFilename)
{
	// Create and initialize the font texture object.
	m_Texture = new RTextureClass;

	IF_NOT_RETURN_FALSE(m_Texture->Initialize(m_OpenGLPtr, textureFilename, 0, false));

	return true;
}


void RFontClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
		SAFE_SHUTDOWN(m_Texture);
}


void RFontClass::SetTexture()
{
    // Set the texture for the font.
    m_Texture->SetTexture(m_OpenGLPtr);

    return;
}


void RFontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
    VertexType* vertexPtr;
    int numLetters, index, i, letter;


    // Coerce the input vertices into a VertexType structure.
    vertexPtr = (VertexType*)vertices;

    // Get the number of letters in the sentence.
    numLetters = (int)strlen(sentence);

    // Initialize the index to the vertex array.
    index = 0;

    // Draw each letter onto a quad.
    for(i=0; i<numLetters; i++)
    {
        letter = ((int)sentence[i]) - 32;
	
	// If the letter is a space then just move over three pixels.
	if(letter == 0)
	{
	    drawX = drawX + (float)m_spaceSize;
	}
	else
	{
	    // First triangle in quad.
	  vertexPtr[index].x = drawX;  // Top left.
	  vertexPtr[index].y = drawY;
	  vertexPtr[index].z = 0.0f;
	  vertexPtr[index].tu = m_Font[letter].left;
	  vertexPtr[index].tv = 1.0f;
	  index++;

	  vertexPtr[index].x = drawX + m_Font[letter].size;  // Bottom right.
	  vertexPtr[index].y = drawY - m_fontHeight;
	  vertexPtr[index].z = 0.0f;
	  vertexPtr[index].tu = m_Font[letter].right;
	  vertexPtr[index].tv = 0.0f;
	  index++;

	  vertexPtr[index].x = drawX;  // Bottom left.
	  vertexPtr[index].y = drawY - m_fontHeight;
	  vertexPtr[index].z = 0.0f;
	  vertexPtr[index].tu = m_Font[letter].left;
	  vertexPtr[index].tv = 0.0f;
	  index++;

	  // Second triangle in quad.
	  vertexPtr[index].x = drawX;  // Top left.
	  vertexPtr[index].y = drawY;
	  vertexPtr[index].z = 0.0f;
	  vertexPtr[index].tu = m_Font[letter].left;
	  vertexPtr[index].tv = 1.0f;
	  index++;

	  vertexPtr[index].x = drawX + m_Font[letter].size;  // Top right.
	  vertexPtr[index].y = drawY;
	  vertexPtr[index].z = 0.0f;
	  vertexPtr[index].tu = m_Font[letter].right;
	  vertexPtr[index].tv = 1.0f;
	  index++;

	  vertexPtr[index].x = drawX + m_Font[letter].size;  // Bottom right.
	  vertexPtr[index].y = drawY - m_fontHeight;
	  vertexPtr[index].z = 0.0f;
	  vertexPtr[index].tu = m_Font[letter].right;
	  vertexPtr[index].tv = 0.0f;
	  index++;

	  // Update the x location for drawing by the size of the letter and one pixel.
	  drawX = drawX + m_Font[letter].size + 1.0f;
	}
    }
    
    return;
}


int RFontClass::GetSentencePixelLength(char* sentence)
{
    int pixelLength, numLetters, i, letter;


    pixelLength = 0;
    numLetters = (int)strlen(sentence);

    for(i=0; i<numLetters; i++)
    {
        letter = ((int)sentence[i]) - 32;
      
        // If the letter is a space then count it as three pixels.
        if(letter == 0)
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


int RFontClass::GetFontHeight()
{
    return (int)m_fontHeight;
}

#endif