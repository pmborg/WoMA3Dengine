// --------------------------------------------------------------------------------------------
// Filename: Rtextclass.cpp
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
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
#include "Rtextclass.h"
#if defined ANDROID_PLATFORM
#include <GLES3\gl32.h>
#endif

RTextClass::RTextClass()
{
    m_OpenGLPtr = 0;
}


RTextClass::RTextClass(const RTextClass& other)
{
}


RTextClass::~RTextClass()
{
}


bool RTextClass::Initialize(OpenGLClass* OpenGL, int screenWidth, int screenHeight, int maxLength, RFontClass* Font, char* text, int positionX, int positionY, float red, float green, float blue)
{
	bool result;


	// Store a pointer to the OpenGL object.
    m_OpenGLPtr = OpenGL;

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the maximum length of the sentence.
	m_maxLength = maxLength;

	// Initalize the sentence.
	result = InitializeBuffers(Font, text, positionX, positionY, red, green, blue);
	if(!result)
	{
		return false;
	}

	return true;
}


void RTextClass::Shutdown()
{
    // Release the vertex and index buffers.
    ShutdownBuffers();

    // Release the pointer to the OpenGL object.
    m_OpenGLPtr = 0;

	return;
}


void RTextClass::Render()
{
    // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
    RenderBuffers();

	return;
}


bool RTextClass::InitializeBuffers(RFontClass* Font, char* text, int positionX, int positionY, float red, float green, float blue)
{
	VertexType* vertices;
	unsigned int* indices;
	int i;
    bool result;


	// Set the vertex and index count.
	m_vertexCount = 6 * m_maxLength;
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned int[m_indexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Initialize the index array.
	for(i=0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}

    // Allocate an OpenGL vertex array object.
    glGenVertexArrays(1, &m_vertexArrayId);

    // Bind the vertex array object to store all the buffers and vertex attributes we create here.
    glBindVertexArray(m_vertexArrayId);

    // Generate an ID for the vertex buffer.
    glGenBuffers(1, &m_vertexBufferId);

    // Bind the vertex buffer and load the vertex data into the vertex buffer.  Set gpu hint to dynamic since it will change once in a while.
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_DYNAMIC_DRAW);

    // Enable the two vertex array attributes.
    glEnableVertexAttribArray(0);  // Vertex position.
    glEnableVertexAttribArray(1);  // Texture coordinates.

    // Specify the location and format of the position portion of the vertex buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

    // Specify the location and format of the texture coordinate portion of the vertex buffer.
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));

    // Generate an ID for the index buffer.
    glGenBuffers(1, &m_indexBufferId);

    // Bind the index buffer and load the index data into it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount* sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Release the vertex and index arrays as they are no longer needed.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	// Now add the text data to the sentence buffers.
	result = UpdateText(Font, text, positionX, positionY, red, green, blue);
	if(!result)
	{
		return false;
	}

	return true;
}


void RTextClass::ShutdownBuffers()
{
    // Disable the two vertex array attributes.
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Release the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_vertexBufferId);

    // Release the index buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_indexBufferId);

    // Release the vertex array object.
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vertexArrayId);

    return;
}


bool RTextClass::UpdateText(RFontClass* Font, char* text, int positionX, int positionY, float red, float green, float blue)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	void* dataPtr;


	// Store the color of the sentence.
	m_pixelColor[0] = red;
	m_pixelColor[1] = green;
	m_pixelColor[2] = blue;
	m_pixelColor[3] = 1.0f;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > m_maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Bind the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);	// Bind the vertex

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void RTextClass::RenderBuffers()
{
    // Bind the vertex array object that stored all the information about the vertex and index buffers.
    glBindVertexArray(m_vertexArrayId);

    // Render the vertex buffer using the index buffer.
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	return;
}


void RTextClass::GetPixelColor(float* color)
{
	color[0] = m_pixelColor[0];
	color[1] = m_pixelColor[1];
	color[2] = m_pixelColor[2];
	color[3] = m_pixelColor[3];
	return;
}
#endif