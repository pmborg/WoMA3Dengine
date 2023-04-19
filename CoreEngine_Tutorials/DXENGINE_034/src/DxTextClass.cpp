// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXtextClass.cpp
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
#if defined USE_RASTERTEK_TEXT_FONT && defined DX_ENGINE

#pragma warning( disable : 4706 )	// Disable warning C4706: assignment within conditional expression
#include "DxTextClass.h"
#include "mem_leak.h"
#include "dxWinSystemClass.h"	// SystemHandle

namespace DirectX {

	DxTextClass::DxTextClass()
	{
		CLASSLOADER();
		WomaIntegrityCheck = 1234567831;

		//private:
		m_Font = NULL;
		m_baseViewMatrix = NULL;


#if defined DX9sdk
		m_driver9 = NULL;
#endif
#if defined DX_ENGINE
		m_spriteShader = NULL;
#endif
		m_driver11 = NULL;
		m_deviceContext11 = NULL;
	}

	DxTextClass::~DxTextClass() { Shutdown(); CLASSDELETE(); }

	void DxTextClass::Shutdown()
	{
		SAFE_SHUTDOWN(m_spriteShader);	// Release the font shader object.
		SAFE_SHUTDOWN(m_Font);			// Release the textFontClass object.
	}

	void DxTextClass::ReleaseSentence(SentenceType** sentence)
	{
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			if (*sentence)
			{
				SAFE_DELETE_ARRAY((*sentence)->vertices);
				SAFE_DELETE_ARRAY((*sentence)->indices);

				// Release the sentence vertex buffer.
				if ((ID3D11Buffer*)(*sentence)->vertexBuffer)
				{
					((ID3D11Buffer*)(*sentence)->vertexBuffer)->Release();
					(*sentence)->vertexBuffer = 0;
				}

				// Release the sentence index buffer.
				if ((ID3D11Buffer*)(*sentence)->indexBuffer)
				{
					((ID3D11Buffer*)(*sentence)->indexBuffer)->Release();
					(*sentence)->indexBuffer = 0;
				}

				// Release the sentence.
				delete* sentence;
				*sentence = 0;
			}
		}
	}

	//SIMILAR: DXmodelClass::CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType)
	bool DxTextClass::Initialize(void* Driver)
	{
		bool result;
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			m_driver11 = (DirectX::DX11Class*)Driver;
			m_deviceContext11 = m_driver11->m_deviceContext;
			m_baseViewMatrix = &DXsystemHandle->m_Camera->m_viewmatrix2D;
		}
		// TextClass: Initialize the font object. PART1
		m_Font = NEW textFontClass;
		IF_NOT_THROW_EXCEPTION(m_Font); // Create the font object.
		IF_NOT_RETURN_FALSE(m_Font->Initialize(Driver, TEXT("engine/data/008fontdata.txt"), TEXT("engine/data/008font.png")));

		// Create the color shader object:
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
			m_spriteShader = NEW DXshaderClass(m_driver11->ShaderVersionH, m_driver11->ShaderVersionL, true);
		IF_NOT_THROW_EXCEPTION(m_spriteShader);

		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
			result = m_spriteShader->Initialize(TEXT("Dx11TextClass"), SHADER_TEXTURE_FONT, m_driver11->m_device, SystemHandle->m_hWnd, TRIANGLELIST);
		if (!result)
		{
			WomaFatalExceptionW(TEXT("Could not initialize the Shader: check HLSL file and the error in code")); return false;
		}

		// Used "Color" and no default "Texture" so load it:
		m_spriteShader->isFontShader = true;
		m_spriteShader->hasTexture = false; // Will use Pixel Color

		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
			m_spriteShader->texture11 = m_Font->GetTexture11();

		return true;
	}

	bool DxTextClass::InitializeTexture(void* Driver)
	{
		return true;
	}


	// SIMILAR: DXmodelClass::InitializeColorBuffers
	bool DxTextClass::InitializeSentence(SentenceType** sentence, int maxLength)
	{
		HRESULT result;

		ID3D11Device* device11 = NULL;
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11) {
			device11 = m_driver11->m_device;
		}
		D3D11_BUFFER_DESC		vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA	vertexData, indexData;

		// Create a new sentence object.
		*sentence = NEW SentenceType;
		IF_NOT_THROW_EXCEPTION(*sentence);

		//memset(*sentence, 0, sizeof (SentenceType));

		// Initialize the sentence buffers to null.
		(*sentence)->vertexBuffer = NULL;
		(*sentence)->indexBuffer = NULL;
		// Set the maximum length of the sentence.
		(*sentence)->maxLength = maxLength;

		// Set the number of vertices in the vertex array.
		(*sentence)->vertexCount = 6 * maxLength;

		// Set the number of indexes in the index array.
		(*sentence)->indexCount = (*sentence)->vertexCount;

		// Create the vertex array.
		(*sentence)->vertices = NEW DXtextureVertexType[(*sentence)->vertexCount];
		IF_NOT_THROW_EXCEPTION((*sentence)->vertices);

		// Create the index array.
		(*sentence)->indices = NEW unsigned long[(*sentence)->indexCount];
		IF_NOT_THROW_EXCEPTION((*sentence)->indices);

		sizeofMODELvertex = sizeof(DXtextureVertexType);

		// Initialize vertex array to zeros at first.
		memset((*sentence)->vertices, 0, (sizeofMODELvertex * (*sentence)->vertexCount));

		// Initialize the index array.
		for (int i = 0; i < (*sentence)->indexCount; i++)
			(*sentence)->indices[i] = i;

		// SIMILAR: DXmodelClass::CreateDirectXBuffers

		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		{
		// Set up the description of the dynamic vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(DXtextureVertexType) * (*sentence)->vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = (*sentence)->vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		ID3D11Buffer* vertexBuffer = NULL;
		result = ((ID3D11Device*)device11)->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer); //IN IN OUT
		IF_FAILED_RETURN_FALSE(result);
		(*sentence)->vertexBuffer = vertexBuffer;

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = (*sentence)->indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		ID3D11Buffer* indexBuffer = NULL;
		result = ((ID3D11Device*)device11)/*device*/->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
		if (FAILED(result)) return false;
		(*sentence)->indexBuffer = indexBuffer;
		}
		// Release the arrays now that the vertex and index buffers have been created and loaded:
		// ----------------------------------------------------------------------------------------


		return true;
	}

	//Should be similar: bool DXmodelClass::UpdateBuffersRotY(void* Driver, int positionX, int positionY)
	bool DxTextClass::UpdateSentence(SentenceType* sentence, TCHAR* text, int positionX, int positionY, float red, float green, float blue)
	{
		int numLetters;
		DXtextureVertexType* vertices;
		float drawX, drawY;
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		DXtextureVertexType* verticesPtr;

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
		vertices = NEW DXtextureVertexType[sentence->vertexCount];
		IF_NOT_THROW_EXCEPTION(vertices);

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(DXtextureVertexType) * sentence->vertexCount));

		// Calculate the X and Y pixel position on the screen to start drawing to.
		drawX = (float)(((SystemHandle->AppSettings->WINDOW_WIDTH / 2) * -1) + positionX);
		drawY = (float)((SystemHandle->AppSettings->WINDOW_HEIGHT / 2) - positionY);

		// Use the font class to build the vertex array from the sentence text and sentence draw location.
		m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			// Lock the vertex buffer so it can be written to.
			result = m_deviceContext11->Map((ID3D11Buffer*)sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result)) return false;

			// Get a pointer to the data in the vertex buffer.
			verticesPtr = (DXtextureVertexType*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(verticesPtr, (void*)vertices, (sizeof(DXtextureVertexType) * sentence->vertexCount));

			// Unlock the vertex buffer.
			m_deviceContext11->Unmap((ID3D11Buffer*)sentence->vertexBuffer, 0);
		}

		// Release the vertex array as it is no longer needed.
		SAFE_DELETE_ARRAY(vertices);

		return true;
	}
	void DxTextClass::RenderSentence(SentenceType* sentence)
	{
		// SetBuffers
		static unsigned int stride = sizeof(DXtextureVertexType), offset = 0;
		/*static*/ XMMATRIX worldMatrix = XMMatrixIdentity();
		XMMATRIX* orthoMatrix = NULL;	// Cannot be Static (Once Window can be Re-Sized)

		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			orthoMatrix = &m_driver11->m_orthoMatrix;	
			ID3D11Buffer* vertexBuffer = (ID3D11Buffer*)sentence->vertexBuffer;

			m_deviceContext11->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);						// Set the vertex buffer to active in the input assembler so it can be rendered.
			m_deviceContext11->IASetIndexBuffer((ID3D11Buffer*)sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);	// Set the index buffer to active in the input assembler so it can be rendered.
			m_deviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);					// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.

			// Render - Create a pixel color vector with the input sentence color.
			m_spriteShader->pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

			m_deviceContext11->PSSetShaderResources(0, 1, &m_Font->m_Texture11);

			// Render the text using the font shader.
			m_spriteShader->Render(/*pass*/NULL, m_deviceContext11, sentence->indexCount, &worldMatrix, m_baseViewMatrix, orthoMatrix);
		}
	}
}

#endif

