// --------------------------------------------------------------------------------------------
// Filename: GlTextClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: 
// Original DX11 version of the code was inpired on: https://www.rastertek.com/dx11tut12.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once

#include "platform.h"
#pragma warning(disable : 4706)	// Disable warning C4706: assignment within conditional expression
#pragma warning(disable : 4715)	//C4715: 'g_SetCameraPosition': not all control paths return a value
#include "OSengine.h"
#include "GlTextClass.h"

#if defined USE_RASTERTEK_TEXT_FONT || defined USE_RASTERTEK_TEXT_FONTV2
ApplicationTextClass* g_AppTextClass = NULL;
#endif
#if defined USE_RASTERTEK_TEXT_FONTV2
#include "Rapplicationclass.h"
extern RApplicationClass* r_Application;
#endif

TCHAR* g_SetCameraRotation(float rotX, float rotY, float rotZ, float* pixelColor = NULL, int* positionX=NULL, int* positionY=NULL)
{
	static TCHAR dataString[50];
	static LightClass* light = SystemHandle->m_Application->app_Light;

	// Setup the X rotation string: ((float) ((int)(rotX*10))/10), ((float) ((int)(rotY*10))/10)
#if defined TEXT_TEST
	StringCchPrintf(dataString, sizeof(dataString), TEXT("rX:%5.1f rY:%5.1f"), rotX, rotY);
#else
	StringCchPrintf(dataString, sizeof(dataString), TEXT("rX:%5.1f rY:%5.1f Light: %5.1f, %5.1f, %5.1f"), rotX, rotY,
//#if defined WINDOWS_PLATFORM
		light->m_lightDirection.x, light->m_lightDirection.y, light->m_lightDirection.z);
//#else
//		light->m_lightDirection[0], light->m_lightDirection[1], light->m_lightDirection[2]);
//#endif
#endif

#if defined USE_RASTERTEK_TEXT_FONT
#if !defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
#endif
	{
		ASSERT(g_AppTextClass->m_Text->UpdateSentence(g_AppTextClass->m_sentence[TEXT_ROT], dataString,
			10, Y_INIT + 40,	//position
			0.0f, 1.0f, 0.0f)); //RGB
	}
#endif

#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		pixelColor[0] = 0;
		pixelColor[1] = 1;
		pixelColor[2] = 0;
		pixelColor[3] = 1.0f;

		*positionX = 10;
		*positionY = Y_INIT + 40;
	}
#endif
	return dataString;
}


TCHAR* g_SetFps(int fps, float* pixelColor = NULL, int* positionX=NULL, int* positionY= NULL)
{
	float red = 0.0f, green = 0.0f, blue = 0.0f;

	if (fps >= 60)
		green = 1.0f;	// If fps is 60 or above set the fps color to green.
	else
		red = 1.0f;		// If fps is below 30 set the fps color to red.

	if (fps > 30)
		green = 1.0f;	// If fps is between 30 and 60 set the fps color to "yellow."

	// avoid division by zero:
	if (fps == 0)
		fps = 1;

#if defined DEBUG_TEXT_A_ONLY
	static TCHAR strBuffer[30] = "A";
#else
	static TCHAR strBuffer[30];
#if defined EXTRA_INFO
	StringCchPrintf(strBuffer, sizeof(strBuffer), TEXT("Fps: %d"), fps);
#else
	StringCchPrintf(strBuffer, sizeof(strBuffer), TEXT("Fps: %d  - ms: %4.2f"), fps, 1000.0f / fps);
#endif
#endif


#if defined USE_RASTERTEK_TEXT_FONT     // VULKAN: Linux / Android
#if !defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
#endif
	{
	ASSERT(g_AppTextClass->m_Text->UpdateSentence(g_AppTextClass->m_sentence[TEXT_FPS], strBuffer, 
		WOMA::AppSettings->WINDOW_WIDTH - 180, Y_INIT + 20, //position
		red, green, blue)); //RGB
	}
#endif

	//V2: - Linux/Android OPENGL
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		pixelColor[0] = red;
		pixelColor[1] = green;
		pixelColor[2] = blue;
		pixelColor[3] = 1.0f;

		*positionX = WOMA::AppSettings->WINDOW_WIDTH - 180;
		*positionY = Y_INIT + 20;
	}
#endif
	return strBuffer;
}


TCHAR* g_SetCameraPosition(float posX, float posY, float posZ, float* pixelColor = NULL, int* positionX = NULL, int* positionY = NULL)
{
	static TCHAR dataString[50];

	// Setup the position string.
	StringCchPrintf(dataString, sizeof(dataString), TEXT("X:%5.1f Z:%5.1f Y:%5.1f"), posX, posZ, posY);

#if defined USE_RASTERTEK_TEXT_FONT
#if !defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
#endif
	{
	ASSERT(g_AppTextClass->m_Text->UpdateSentence(g_AppTextClass->m_sentence[TEXT_POS], dataString,
		10, Y_INIT + 20,	 //position
		0.0f, 1.0f, 0.0f));  //RGB
	}
#endif

#if defined USE_RASTERTEK_TEXT_FONT2
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		pixelColor[0] = 0;
		pixelColor[1] = 1;
		pixelColor[2] = 0;
		pixelColor[3] = 1.0f;

		*positionX = 10;
		*positionY = Y_INIT + 20;
	}
#endif
	return dataString;
}

TCHAR* g_SetRenderCount(int Count, int compoundCount, UINT totalCompoundLoaded)
{
	static TCHAR countString[80];

	// Setup the render count string.
	StringCchPrintf(countString, sizeof(countString), TEXT("Vertex count : %d Objects on QUAD %d/%d (loaded/total)"), (int)SystemHandle->TotalVertexCounter, compoundCount, totalCompoundLoaded);

#if defined USE_RASTERTEK_TEXT_FONT
#if !defined WINDOWS_PLATFORM
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
#endif
	{
	ASSERT(g_AppTextClass->m_Text->UpdateSentence(g_AppTextClass->m_sentence[TEXT_TERRAINRENDERCOUNT], countString, 10, 100, 0.0f, 0.5f, 1.0f));
	}
#endif

	return countString;
}

#if defined USE_RASTERTEK_TEXT_FONT
void ApplicationTextClass::SetCameraRotation(float rotX, float rotY, float rotZ) 
{ 
	g_SetCameraRotation(rotX, rotY, rotZ); 
}

void ApplicationTextClass::SetFps(int fps) 
{ 
	g_SetFps(fps); 
}

void ApplicationTextClass::SetCameraPosition(float posX, float posY, float posZ)
{
	g_SetCameraPosition(posX, posY, posZ);
}

#if DX_ENGINE_LEVEL >= 30 && defined _DEBUG
//08
void ApplicationTextClass::SetRenderCount(int Count, int compoundCount, UINT totalCompoundLoaded)
{
	g_SetRenderCount(Count, compoundCount, totalCompoundLoaded);
}
#endif

#endif





#if (defined OPENGL3 || defined OPENGL4) && defined USE_RASTERTEK_TEXT_FONT
//OPENGL for Windows Platform only

#include "mem_leak.h"
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
	if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11)
	{
		if (*sentence)
		{
#if defined DX_ENGINE
			SAFE_DELETE_ARRAY((*sentence)->vertices);
#endif
#if (defined OPENGL3 || defined OPENGL4)
			SAFE_DELETE_ARRAY((*sentence)->GLvertices);
#endif
			SAFE_DELETE_ARRAY((*sentence)->indices);

			// Release the sentence.
			delete* sentence;
			*sentence = 0;
		}
	}
}

//SIMILAR: DXmodelClass::CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType)
bool GlTextClass::Initialize(void* Driver, void* pContext)
{
	bool result=false;

	womalog(TEXT("GlTextClass::Initialize ()\n"));

	// TextClass: Initialize the font object. PART1
	m_Font = NEW textFontClass;
	IF_NOT_THROW_EXCEPTION(m_Font); // Create the font object.
	IF_NOT_RETURN_FALSE(m_Font->Initialize(pContext, Driver, FONT_DATA_FILE, FONT_DATA_TEXTURE));

#if (defined OPENGL3 || defined OPENGL4)
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
		m_spriteShader = NEW GLshaderClass;
#endif
	IF_NOT_THROW_EXCEPTION(m_spriteShader);

#if (defined OPENGL3 || defined OPENGL4)
	result = m_spriteShader->Initialize(SHADER_TEXTURE_FONT);
#endif
	if (!result)
	{
		WomaFatalException(("Could not initialize the Shader: check HLSL/GLSL file and the error in code")); //return false;
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
	womalog(TEXT("GlTextClass::InitializeSentence()\n"));

	// Create a new sentence object.
	*sentence = NEW SentenceType;
	IF_NOT_THROW_EXCEPTION(*sentence);

	// Set the maximum length of the sentence.
	//(*sentence)->maxLength = maxLength;
	textmaxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;
	//(*sentence)->vertexCount = 6 * (*sentence)->numLetters;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	(*sentence)->GLvertices = NEW ModelTextureVertexType[(*sentence)->vertexCount];
	IF_NOT_THROW_EXCEPTION((*sentence)->GLvertices);

	// Create the index array.
	// NOTE Vulkan requires 32-bit indices here.
	//		Never use unsigned long: MSVC=4 bytes, GCC/Clang x64=8 bytes.
	(*sentence)->indices = NEW uint32_t[(*sentence)->indexCount];
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
	//int numLetters;
	ModelTextureVertexType* vertices;
	float drawX, drawY;

	positionY += 36;

	if (!sentence)
		{ WomaFatalException(("Text: sentence = NULL")); }

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	sentence->numLetters = (int)_tcslen(text);

	// Check for possible buffer overflow.
	ASSERT_DEBUG(sentence->numLetters <= textmaxLength);

	// Only render the required triangles (6 vertices per character).
	sentence->indexCount = sentence->numLetters * 6;

	// Create the vertex array.
	vertices = NEW ModelTextureVertexType[sentence->indexCount];
	IF_NOT_THROW_EXCEPTION(vertices);

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(ModelTextureVertexType) * sentence->indexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((WOMA::AppSettings->WINDOW_WIDTH / 2) * -1) + positionX);
	drawY = (float)((WOMA::AppSettings->WINDOW_HEIGHT / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Bind the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, sentence->m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sentence->indexCount * sizeof(ModelTextureVertexType), vertices, GL_STATIC_DRAW);	// Bind the vertex

	// Release the vertex array as it is no longer needed.
	SAFE_DELETE_ARRAY(vertices);

	return true;
}

void GlTextClass::RenderSentence(void* pContext, SentenceType* sentence)
{
#if (defined OPENGL3 || defined OPENGL4)
	m_spriteShader->SetShader();

	GLopenGLclass* driver = (GLopenGLclass*)driverList[WOMA::AppSettings->DRIVER];

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

	SystemHandle->TotalVertexCounter += sentence->indexCount;
#endif
}


#endif
