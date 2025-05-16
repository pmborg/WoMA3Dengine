// --------------------------------------------------------------------------------------------
// Filename: ApplicationTextClass.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567155;

#include "platform.h"
#if ((defined OPENGL3 || defined OPENGL4) || defined DX_ENGINE || defined INTRO_DEMO) && defined USE_RASTERTEK_TEXT_FONT
#include "OSengine.h"
#include "mem_leak.h"
#include "ApplicationTextClass.h"
#include "GlTextClass.h"
#include "GLopenGLclass.h"
#if defined DX_ENGINE
#include "DxTextClass.h"
#endif

extern float terrain_nx;
extern float terrain_nz;
extern float nextHeight;

namespace DirectX {

ApplicationTextClass::ApplicationTextClass()
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567155;

	m_Text = NULL;
}

ApplicationTextClass::~ApplicationTextClass() { CLASSDELETE();}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The SetFps function takes the fps integer value given to it and then converts it to a string. 
//Once the fps count is in a string format it gets concatenated to another string so it has a prefix indicating that 
//it is the fps speed. After that it is stored in the sentence structure for rendering. 
//The SetFps function also sets the color of the fps string to green if above 60 fps, yellow if below 60 fps, and 
//red if below 30 fps.

#define Y_INIT 20

//00-----------------------------------------------------------------------------------------
void ApplicationTextClass::SetFps(int fps)
{
	static TCHAR strBuffer[30];
	float red=0.0f, green=0.0f, blue=0.0f;

	// avoid division by zero:
	if (fps == 0)
		fps=1;
	
    StringCchPrintf(strBuffer, sizeof(strBuffer), TEXT("Fps: %d  - ms: %4.2f"), fps, 1000.0f/fps);

	if(fps >= 60)
		green = 1.0f;	// If fps is 60 or above set the fps color to green.
	else 
		red = 1.0f;		// If fps is below 30 set the fps color to red.

	if(fps > 30)
		green = 1.0f;	// If fps is between 30 and 60 set the fps color to "yellow."

	// Update the sentence vertex buffer with the new string information.
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_FPS], strBuffer, SystemHandle->AppSettings->WINDOW_WIDTH - 180, Y_INIT+20, red, green, blue));
}

#if !defined TEXT_TEST
//The SetCpu function is similar to the SetFps function. It takes the cpu value and converts it to a string which is then 
//stored in the sentence structure and rendered.
//01-----------------------------------------------------------------------------------------
void ApplicationTextClass::SetCpu(int cpu)
{
	static TCHAR cpuString[20];

	// Setup the cpu string.
    StringCchPrintf (cpuString, sizeof(cpuString), TEXT("Cpu: %d %%"), cpu);

	// Update the sentence vertex buffer with the new string information.
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_CPU], cpuString, SystemHandle->AppSettings->WINDOW_WIDTH - 180, Y_INIT+40, 0.0f, 1.0f, 0.0f));
}

//02-----------------------------------------------------------------------------------------
void ApplicationTextClass::SetCameraPosition(float posX, float posY, float posZ)
{
	static TCHAR dataString[40];

	// Setup the position string.
	StringCchPrintf (dataString, sizeof(dataString), TEXT("X:%5.1f Z:%5.1f Y:%5.1f"), posX, posZ, posY);
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_POS], dataString, 10, Y_INIT+20, 0.0f, 1.0f, 0.0f));
}

//03-----------------------------------------------------------------------------------------
void ApplicationTextClass::SetCameraRotation(float rotX, float rotY, float rotZ)
{
	static TCHAR dataString[40];

	// Setup the X rotation string: ((float) ((int)(rotX*10))/10), ((float) ((int)(rotY*10))/10)
	StringCchPrintf (dataString, sizeof(dataString), TEXT("rX:%5.1f rY:%5.1f"), rotX, rotY);
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_ROT], dataString, 10, Y_INIT+40, 0.0f, 1.0f, 0.0f));
}
#endif

#if defined EXTRA_INFO
void ApplicationTextClass::SetInfoA(UINT h, UINT m)
{
	static TCHAR dataString[40];

	StringCchPrintf(dataString, sizeof(dataString), TEXT("nx: %5.2f nz: %5.2f"), terrain_nx, terrain_nz);
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_CLOCK], dataString, 10, Y_INIT + 120, 0.0f, 1.0f, 0.0f));
}

void ApplicationTextClass::SetInfoB(float rotX, float rotY, float rotZ)
{
	static TCHAR dataString[40];
	StringCchPrintf(dataString, sizeof(dataString), TEXT("nextHeight: %5.2f - m_positionY: %5.2f"), nextHeight, SystemHandle->m_Application->m_Position[g_NetID]->m_positionY);
	float nextHeight = 0;
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_LIGHT_DIR], dataString, 10, Y_INIT + 60, 0.0f, 1.0f, 0.0f));
}
#endif

#if DX_ENGINE_LEVEL >= 30 && _DEBUG
//08
void ApplicationTextClass::SetRenderCount(int Count, int compoundCount, UINT totalCompoundLoaded)
{
	TCHAR countString[80];

	// Setup the render count string.
    //StringCchPrintf(countString, sizeof(countString), TEXT("Terrain Render Count: %dK Objs: %d/%d"), terrainCount/1024, compoundCount, totalCompoundLoaded);
	StringCchPrintf(countString, sizeof(countString), TEXT("Vertex count : %d Objects on QUAD %d/%d (loaded/total)"), (int)SystemHandle->TotalVertexCounter, compoundCount, totalCompoundLoaded);

	// Update the sentence vertex buffer with the new string information:
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_TERRAINRENDERCOUNT], countString, 10, 100, 0.0f, 0.5f, 1.0f));
}
#endif

#if TUTORIAL_CHAP >= 60 && _DEBUG // BILLBOARD
//10
void ApplicationTextClass::SetBillRenderCount(int count)
{
	char countString[80];
	StringCchPrintf(countString, sizeof(countString), TEXT("Angle: %d"), (int)SystemHandle->m_Application->billangle);

	// Update the sentence vertex buffer with the new string information.
	ASSERT(m_Text->UpdateSentence(m_sentence[TEXT_BILLRENDERCOUNT], countString, 10, 130, 0.0f, 1.0f, 0.0f));
}
#endif

#if TUTORIAL_CHAP >= 85 && _DEBUG
//09
void ApplicationTextClass::SetLoboRenderCount(int count)
{
	char tempString[32];
	char countString[32];

	// Convert the count integer to string format.
	//_itoa_s(count, tempString, 10);	//Bug on ANDROID

	// Setup the render count string.
	strcpy_s(countString, "Animal Render Count: ");
	strcat_s(countString, tempString);

	// Update the sentence vertex buffer with the new string information.
	ASSERT (m_Text.UpdateSentence(m_sentence[TEXT_WATERRENDERCOUNT], countString, 10, 110, 0.0f, 1.0f, 0.0f));
}
#endif

}

bool ApplicationTextClass::Initialize(void* Driver)
{
	// TODO GL
#ifdef OPENGL3
	if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
	{
		m_Text = NEW GlTextClass(); IF_NOT_THROW_EXCEPTION(m_Text);
	}
#endif
#if defined DX9 || defined DX11
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
	{
		m_Text = NEW DxTextClass(); IF_NOT_THROW_EXCEPTION(m_Text);
	}
#endif
#ifdef DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Text = NEW DxTextClass(); IF_NOT_THROW_EXCEPTION(m_Text);
	}
#endif

	// Initialize the Text object:
	if (!m_Text->Initialize(Driver))
	{
		WomaFatalException(("Could not initialize the Text Object")); /*return false;*/
	}

	// TextClass: PART2
	for (UINT i = (UINT)_countof(m_sentence); i < N_TEXT_MAX_SENTENCE; i++)
		m_sentence[i] = NULL;

	for (UINT i = 0; i < (UINT)_countof(m_sentence); i++)
		IF_NOT_RETURN_FALSE(m_Text->InitializeSentence(&m_sentence[i], 80));

#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Text->InitializeTexture(Driver);
	}
#endif

	return true;
}

bool ApplicationTextClass::Shutdown()
{
	if (m_Text)
	{
#if (defined OPENGL3 || defined OPENGL4) && defined USE_RASTERTEK_TEXT_FONT
		if (SystemHandle->AppSettings->DRIVER == DRIVER_GL3)
		{
			for (UINT i = 0; i < _countof(m_sentence); i++)
			{
				if (m_sentence[i])
				{
					// Release the vertex buffer.
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glDeleteBuffers(1, &m_sentence[i]->m_vertexBufferId);

					// Release the index buffer.
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glDeleteBuffers(1, &m_sentence[i]->m_indexBufferId);

					// Release the vertex array object.
					glBindVertexArray(0);
					glDeleteVertexArrays(1, &m_sentence[i]->m_vertexArrayId);
				}
			}
		}
#endif
		for (UINT i = 0; i < _countof(m_sentence); i++) {
			m_Text->ReleaseSentence(&m_sentence[i]);
			SAFE_DELETE(m_sentence[i]);
		}

		switch (SystemHandle->AppSettings->DRIVER)
		{
#ifdef OPENGL3
		case DRIVER_GL3:
			if (m_Text)
			{
				(m_Text)->Shutdown();
				delete ((GlTextClass*)m_Text);
				m_Text = NULL;
			}
			break;
#endif

#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
		case DRIVER_DX9:
		case DRIVER_DX11:
			if (m_Text)
			{
				(m_Text)->Shutdown();
				delete ((DirectX::DxTextClass*)m_Text);
				m_Text = NULL;
			}
			break;
#endif

#ifdef DX12
		case DRIVER_DX12:
			if (m_Text)
			{
				(m_Text)->Shutdown();
				delete ((DirectX::DxTextClass*)m_Text);
				m_Text = NULL;
			}
			break;
#endif

		default:
#if defined USE_WOMA_EXCEPTION
			throw woma_exception("Not Enough Memory!", __FILE__, __FUNCTION__, __LINE__);
#else
			WomaFatalException("Not Enough Memory!");
#endif
			break;
		}
	}

	return true;
}

void ApplicationTextClass::Render()		// Render the text user interface elements:
{
	for (UINT i = 0; i < _countof(m_sentence); i++) {
		if (m_sentence[i]->red > -1)
			m_Text->RenderSentence(m_sentence[i]);
	}
}

#endif
