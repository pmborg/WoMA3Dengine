////////////////////////////////////////////////////////////////////////////////
// Filename: RFontClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//////////////
// INCLUDES //
//////////////
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Rtextureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RFontClass
////////////////////////////////////////////////////////////////////////////////
class RFontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
        float x, y, z;
        float tu, tv;
	};

public:
	RFontClass();
	RFontClass(const RFontClass&);
	~RFontClass();

	bool Initialize(OpenGLClass*, int);
	void Shutdown();

	void BuildVertexArray(void*, char*, float, float);
	int GetSentencePixelLength(char*);
	int GetFontHeight();

	void SetTexture();

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(char*);
	void ReleaseTexture();

private:
    OpenGLClass* m_OpenGLPtr;
	FontType* m_Font;
	RTextureClass* m_Texture;
	float m_fontHeight;
	int m_spaceSize;
};

#endif
#endif