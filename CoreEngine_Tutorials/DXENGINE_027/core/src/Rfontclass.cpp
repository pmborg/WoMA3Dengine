///////////////////////////////////////////////////////////////////////////////
// Filename: RFontClass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "OSengine.h"
#include "Rfontclass.h"

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
            strcpy(fontFilename, "data/fonts/008fontdata2.txt");
            strcpy(fontTextureFilename, "data/fonts/008font2.tga");
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
/*
class asset_streambuf : public std::streambuf
{
public:
	asset_streambuf(AAssetManager* manager, const std::string& filename)
		: manager(manager)
	{
		asset = AAssetManager_open(manager, filename.c_str(), AASSET_MODE_STREAMING);
		buffer.resize(1024);

		setg(0, 0, 0);
		setp(&buffer.front(), &buffer.front() + buffer.size());
	}

	virtual ~asset_streambuf()
	{
		sync();
		AAsset_close(asset);
	}

	std::streambuf::int_type underflow() override
	{
		auto bufferPtr = &buffer.front();
		auto counter = AAsset_read(asset, bufferPtr, buffer.size());

		if (counter == 0)
			return traits_type::eof();
		if (counter < 0) //error, what to do now?
			return traits_type::eof();

		setg(bufferPtr, bufferPtr, bufferPtr + counter);

		return traits_type::to_int_type(*gptr());
	}

	std::streambuf::int_type overflow(std::streambuf::int_type value) override
	{
		return traits_type::eof();
	};

	int sync() override
	{
		std::streambuf::int_type result = overflow(traits_type::eof());

		return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
	}

private:
	AAssetManager* manager;
	AAsset* asset;
	std::vector<char> buffer;
};


class assetistream : public std::istream
{
public:
	assetistream(AAssetManager* manager, const std::string& file)
		: std::istream(new asset_streambuf(manager, file))
	{
	}
	assetistream(const std::string& file)
		: std::istream(new asset_streambuf(manager, file))
	{
	}

	virtual ~assetistream()
	{
		delete rdbuf();
	}

	static void setAssetManager(AAssetManager* m)
	{
		manager = m;
	}

private:
	static AAssetManager* manager;
};
*/
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

//#include <fstream>
//#include <C:/WoMAengine2023/strtk.hpp>   // http://www.partow.net/programming/strtk

bool RFontClass::LoadFontData(char* filename)
{
	// Create the font spacing buffer.
	m_Font = new FontType[95];

#if !defined ANDROID_PLATFORM
	ifstream fin;
	int i;
	char temp;

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
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