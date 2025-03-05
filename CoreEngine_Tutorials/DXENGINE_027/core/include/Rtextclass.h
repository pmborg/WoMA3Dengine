////////////////////////////////////////////////////////////////////////////////
// Filename: RTextClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RTextClass_H_
#define _RTextClass_H_

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
//////////////
// INCLUDES //
//////////////
#include "Rfontclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RTextClass
////////////////////////////////////////////////////////////////////////////////
class RTextClass
{
private:
	struct VertexType
	{
		float x, y, z;
        float tu, tv;
	};

public:
	RTextClass();
	RTextClass(const RTextClass&);
	~RTextClass();

	bool Initialize(OpenGLClass*, int, int, int, RFontClass*, char*, int, int, float, float, float);
	void Shutdown();
	void Render();

	bool UpdateText(RFontClass*, char*, int, int, float, float, float);
    void GetPixelColor(float*);

private:
	bool InitializeBuffers(RFontClass*, char*, int, int, float, float, float);
	void ShutdownBuffers();
	void RenderBuffers();

private:
    OpenGLClass* m_OpenGLPtr;
    int m_screenWidth, m_screenHeight, m_maxLength, m_vertexCount, m_indexCount;
    unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
    float m_pixelColor[4];
};

#endif
#endif