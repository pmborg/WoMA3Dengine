// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: GlTextClass.cpp
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
// Original DX11 version of the code was inpired on: https://www.rastertek.com/dx11tut12.html
// --------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"

#pragma warning( disable : 4706 )	// Disable warning C4706: assignment within conditional expression
#include "mem_leak.h"
#include "WinSystemClass.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID

#include "GlTextClass.h"
#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "GLmodelClass.h"
#include "GLshaderClass.h"
#include "textFontClass.h"

GlTextClass::GlTextClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567829;

	//private:
	m_Font = NULL;
	m_spriteShader = NULL;
	m_baseViewMatrix = NULL;
}

GlTextClass::~GlTextClass() { Shutdown(); CLASSDELETE(); }

void GlTextClass::Shutdown()
{
	SAFE_SHUTDOWN(m_spriteShader);	// Release the font shader object.
	SAFE_SHUTDOWN(m_Font);			// Release the textFontClass object.

	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void GlTextClass::ReleaseSentence(SentenceType** sentence)
{
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
	{
		if (*sentence)
		{
#if defined DX_ENGINE
			SAFE_DELETE_ARRAY((*sentence)->vertices);
#endif
			SAFE_DELETE_ARRAY((*sentence)->GLvertices);
			SAFE_DELETE_ARRAY((*sentence)->indices);

			// Release the sentence.
			delete* sentence;
			*sentence = 0;
		}
	}
}

//SIMILAR: DXmodelClass::CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType)
bool GlTextClass::Initialize(void* Driver)
{
	bool result=false;

	// TextClass: Initialize the font object. PART1
	m_Font = NEW textFontClass;
	IF_NOT_THROW_EXCEPTION(m_Font); // Create the font object.
	IF_NOT_RETURN_FALSE(m_Font->Initialize(Driver, WOMA::LoadFile(TEXT("engine/data/008fontdata.txt")), WOMA::LoadFile(TEXT("engine/data/008font.png"))));

	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		m_spriteShader = NEW GLshaderClass;
	IF_NOT_THROW_EXCEPTION(m_spriteShader);

	result = m_spriteShader->Initialize(SHADER_TEXTURE_FONT);
	if (!result)
	{
		WomaFatalExceptionW(TEXT("Could not initialize the Shader: check HLSL/GLSL file and the error in code")); return false;
	}

	// Used "Color" and no default "Texture" so load it:
	m_spriteShader->isFont = true;
	m_spriteShader->hasTexture = false; // Will use Pixel Color

	return true;
}

bool GlTextClass::InitializeTexture(void* Driver)
{
	return true;
}

// SIMILAR: DXmodelClass::InitializeColorBuffers
bool GlTextClass::InitializeSentence(SentenceType** sentence, int maxLength)
{
	// Create a new sentence object.
	*sentence = NEW SentenceType;
	IF_NOT_THROW_EXCEPTION(*sentence);

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	(*sentence)->GLvertices = NEW ModelTextureVertexType[(*sentence)->vertexCount];
	IF_NOT_THROW_EXCEPTION((*sentence)->GLvertices);

	// Create the index array.
	(*sentence)->indices = NEW unsigned long[(*sentence)->indexCount];
	IF_NOT_THROW_EXCEPTION((*sentence)->indices);

	// Initialize the index array.
	for (int i = 0; i < (*sentence)->indexCount; i++)
		(*sentence)->indices[i] = i;

	glGenVertexArrays(1, &(*sentence)->m_vertexArrayId);	// Allocate an OpenGL vertex array object.
	glBindVertexArray((*sentence)->m_vertexArrayId);		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	glGenBuffers(1, &(*sentence)->m_vertexBufferId);		// Generate an ID for the vertex buffer.

	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, (*sentence)->m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, (*sentence)->vertexCount * sizeof(ModelTextureVertexType), (*sentence)->GLvertices, GL_STATIC_DRAW);

	//SetOpenGLBuffers(sizeof(ModelTextureVertexType), (UINT*)(*sentence)->indices);
	glEnableVertexAttribArray(0);		// 0: Vertex position.

	//case SHADER_TEXTURE_FONT:			// 0: Vertex position.
	glEnableVertexAttribArray(1);		// 1: Texture coordinates.

	// Specify the location and format of the position portion of the vertex buffer:
	// ----------------------------------------------------------------------------------------

	glBindBuffer(GL_ARRAY_BUFFER, (*sentence)->m_vertexBufferId);
	glVertexAttribPointer(0, sizeof(GLint), GL_FLOAT, false, sizeof(ModelTextureVertexType), 0);	// POSITION

	//case SHADER_TEXTURE_FONT:
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(ModelTextureVertexType), (unsigned char*)NULL + (3 * sizeof(float)));

	// Generate an ID for the index buffer:
	glGenBuffers(1, &(*sentence)->m_indexBufferId);

	// Bind the index buffer and load the index data into it:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*sentence)->m_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*sentence)->indexCount * sizeof(UINT), (*sentence)->indices, GL_STATIC_DRAW);

	// Now that the buffers have been loaded we can release the array data.
	SAFE_DELETE_ARRAY((*sentence)->GLvertices);
	SAFE_DELETE_ARRAY((*sentence)->indices);

	return true;
}

//Should be similar: bool DXmodelClass::UpdateBuffersRotY(void* Driver, int positionX, int positionY)
//bool TextClass::UpdateText(FontClass* Font, char* text, int positionX, int positionY, float red, float green, float blue)
////////////////////////////
bool GlTextClass::UpdateSentence(SentenceType* sentence, TCHAR* text, int positionX, int positionY, float red, float green, float blue)
{
	int numLetters;
	ModelTextureVertexType* vertices;
	float drawX, drawY;

	positionY += 36;

	if (!sentence)
	{
		WomaFatalExceptionW(TEXT("Text: sentece = NULL")); return false;
	}

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)_tcslen(text);

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		WomaFatalExceptionW(TEXT("Text Size Fatal Error")); return false;
	}

	// Create the vertex array.
	vertices = NEW ModelTextureVertexType[sentence->vertexCount];
	IF_NOT_THROW_EXCEPTION(vertices);

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(ModelTextureVertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((SystemHandle->AppSettings->WINDOW_WIDTH / 2) * -1) + positionX);
	drawY = (float)((SystemHandle->AppSettings->WINDOW_HEIGHT / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Bind the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, sentence->m_vertexBufferId);

	// Get a pointer to the buffer's actual location in memory.
	void* dataPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// Copy the vertex data into memory.
	memcpy(dataPtr, vertices, sentence->vertexCount * sizeof(VertexType));

	// Unlock the vertex buffer.
	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Release the vertex array as it is no longer needed.
	SAFE_DELETE_ARRAY(vertices);

	return true;
}

void GlTextClass::RenderSentence(SentenceType* sentence)
{
	m_spriteShader->SetShader();

	GLopenGLclass* driver = (GLopenGLclass*)SystemHandle->driverList[SystemHandle->AppSettings->DRIVER];

	static mat4 m_worldMatrix = m_worldMatrix.mat4identity();
	static mat4 m_viewMatrix = m_viewMatrix.mat4identity();
	m_viewMatrix.m[14] = 1;
	mat4 projectionMatrix = driver->m_orthoMatrix;

	m_spriteShader->pixelColor[0] = sentence->red;
	m_spriteShader->pixelColor[1] = sentence->green;
	m_spriteShader->pixelColor[2] = sentence->blue;
	m_spriteShader->pixelColor[3] = 1.0f;
	m_spriteShader->SetShaderParameters(SHADER_TEXTURE_FONT, &m_worldMatrix, &m_viewMatrix, &projectionMatrix, 0 /*m_Texture->m_textureID*/);

	/////////////////////////////////////
	// Step 3: Set shader texture resource(s)
	glActiveTexture(GL_TEXTURE0 + 0);	// Set the unique texture unit in which to store the data.
	glBindTexture(GL_TEXTURE_2D, m_Font->gl_Texture->m_textureID);

	////////////////////////////
	// Step 4: Render the model
	glBindVertexArray(sentence->m_vertexArrayId);				// SetBuffers(driver);

	// Render the vertex buffer using the index buffer:
	glDrawElements(GL_TRIANGLES, sentence->indexCount, GL_UNSIGNED_INT, 0);

	//unbind everything
	glBindVertexArray(NULL);				// Unbind our Vertex Array Object
	glBindTexture(GL_TEXTURE_2D, NULL);
	glUseProgram(0);						// shader->unbind(); // Unbind our shader
}


