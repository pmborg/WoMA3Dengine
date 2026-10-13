// --------------------------------------------------------------------------------------------
// Filename: ApplicationTextClass.cpp
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
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#include "platform.h"
#if ((defined VULKAN || defined OPENGL3 || defined OPENGL4) || defined DX_ENGINE || defined INTRO_DEMO) && defined USE_RASTERTEK_TEXT_FONT
#include "OSengine.h"
#include "mem_leak.h"
#include "ApplicationTextClass.h"
#include "GlTextClass.h"
#include "GLopenGLclass.h"

#if defined DX_ENGINE
#include "DxTextClass.h"
#endif

#if defined VULKAN
#include "VkTextClass.h"
#endif

extern float terrain_nx;
extern float terrain_nz;
extern float nextHeight;

ApplicationTextClass::ApplicationTextClass()
{
	CLASSLOADER();
	m_Text = NULL;
}

ApplicationTextClass::~ApplicationTextClass() { CLASSDELETE();}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The SetFps function takes the fps integer value given to it and then converts it to a string. 
//Once the fps count is in a string format it gets concatenated to another string so it has a prefix indicating that 
//it is the fps speed. After that it is stored in the sentence structure for rendering. 
//The SetFps function also sets the color of the fps string to green if above 60 fps, yellow if below 60 fps, and 
//red if below 30 fps.

#if !defined TEXT_TEST
//The SetCpu function is similar to the SetFps function. It takes the cpu value and converts it to a string which is then 
//stored in the sentence structure and rendered.

void ApplicationTextClass::SetCpu(int cpu)
{
	static TCHAR cpuString[20];

	// Setup the cpu string.
    StringCchPrintf (cpuString, sizeof(cpuString), TEXT("Cpu: %d %%"), cpu);

	// Update the sentence vertex buffer with the new string information.
	ASSERT (m_Text->UpdateSentence(m_sentence[TEXT_CPU], cpuString, WOMA::AppSettings->WINDOW_WIDTH - 180, Y_INIT+40, 0.0f, 1.0f, 0.0f));
}

#endif

#if TUTORIAL_CHAP >= 60 && DX_ENGINE_LEVEL <= 92 && !defined RELEASE // BILLBOARD
//10
void ApplicationTextClass::SetBillRenderCount(int count, UINT deltaTime)
{
	TCHAR countString[80];
	#if defined  NO3DBILL
	StringCchPrintf(countString, sizeof(countString), TEXT("N.Bill: %d (billangle: %3.1f) - Anim.Time: %d ms"), count, SystemHandle->m_Application->billangle, (UINT)deltaTime);
	#else
	StringCchPrintf(countString, sizeof(countString), TEXT("N.Bill: %d - Anim.Time: %d ms"), count, (UINT)deltaTime);
	#endif

	// Update the sentence vertex buffer with the new string information.
	ASSERT(m_Text->UpdateSentence(m_sentence[TEXT_BILLRENDERCOUNT], countString, 10, 130, 0.0f, 1.0f, 0.0f));
}
#endif

#if TUTORIAL_CHAP >= 85 && defined _DEBUG
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

bool ApplicationTextClass::Initialize(void* pContext, void* Driver)
{
	// TODO GL
#if (defined OPENGL3 || defined OPENGL40) 
	if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
	{
		m_Text = NEW GlTextClass(); IF_NOT_THROW_EXCEPTION(m_Text);
	}
#endif

#if defined(VULKAN)
	if (WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
	{
		m_Text = NEW VkTextClass();
		IF_NOT_THROW_EXCEPTION(m_Text);
	}
#endif

#if defined DX9 || defined DX11
	if (WOMA::AppSettings->DRIVER == DRIVER_DX9 || WOMA::AppSettings->DRIVER == DRIVER_DX11)
	{
		m_Text = NEW DxTextClass(); IF_NOT_THROW_EXCEPTION(m_Text);
	}
#endif
#ifdef DX12
	if (WOMA::AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Text = NEW DxTextClass(); IF_NOT_THROW_EXCEPTION(m_Text);
	}
#endif

	// Initialize the Text object:
    if (!m_Text->Initialize(Driver, pContext))
		WomaFatalException(("Could not initialize the Text Object"));

	// TextClass: PART2
	for (UINT i = (UINT)_countof(m_sentence); i < N_TEXT_MAX_SENTENCE; i++)
		m_sentence[i] = NULL; //Reset not used slots

	for (UINT i = 0; i < (UINT)_countof(m_sentence); i++) //Init used slots
		IF_NOT_RETURN_FALSE(m_Text->InitializeSentence(&m_sentence[i], SENTENCE_MAX_LEN));

#if defined DX12 || defined VULKAN
	if (WOMA::AppSettings->DRIVER == DRIVER_DX12 || WOMA::AppSettings->DRIVER == DRIVER_VULKAN)
		m_Text->InitializeTexture(Driver);
#endif

	return true;
}

bool ApplicationTextClass::Shutdown()
{
	if (m_Text)
	{
#if (defined OPENGL3 || defined OPENGL4) && defined USE_RASTERTEK_TEXT_FONT
		if (WOMA::AppSettings->DRIVER == DRIVER_GL3)
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

		switch (WOMA::AppSettings->DRIVER)
		{
#if (defined OPENGL3 || defined OPENGL40) 
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

#if defined(VULKAN)
			case DRIVER_VULKAN:
			{
				((VkTextClass*)m_Text)->Shutdown();
				delete ((VkTextClass*)m_Text);
				m_Text = NULL;
			}
			break;
#endif

		default:
#if defined USE_WOMA_EXCEPTION
			throw woma_exception("No driver support, yet!", __FILE__, __FUNCTION__, __LINE__);
#else
			WomaFatalException("No driver support, yet!");
#endif
			break;
		}
	}

	return true;
}

//#define DEBUG_SETSHADERPARAMETERS

void ApplicationTextClass::Render(void* pContext)		// Render the text user interface elements:
{
#if defined _DEBUG || defined DEBUG_SETSHADERPARAMETERS
	if (m_Driver->RenderfirstTime)
	{
		womalog("APPLICATIONTEXTCLASS::RENDER() START\n");
	}
#endif

	for (UINT i = 0; i < _countof(m_sentence); i++) {
		if (m_sentence[i])
			m_Text->RenderSentence(pContext, m_sentence[i]);
	}

}

#endif
