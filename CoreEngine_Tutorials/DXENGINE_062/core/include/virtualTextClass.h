// --------------------------------------------------------------------------------------------
// Filename: virtualTextClass.h
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

#pragma once

#include "platform.h"
#if defined USE_RASTERTEK_TEXT_FONT || defined USE_RASTERTEK_TEXT_FONTV2

#include "main.h"
#include "womadriverclass.h"
#include "vertexTypes.h"

#if defined VULKAN
#include "vkClass.h"
#endif

struct SentenceType
{
#if (defined OPENGL3 || defined OPENGL4)
	ModelTextureVertexType* GLvertices = NULL;	//OPENGL
	unsigned int	m_vertexArrayId = NULL;		//OPENGL
	unsigned int	m_vertexBufferId = NULL;	//OPENGL
	unsigned int	m_indexBufferId = NULL;		//OPENGL
#endif

#if defined VULKAN
	textFontVertexType* VKvertices = NULL;

	VkBuffer       vkVertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory vkVertexBufferMemory = VK_NULL_HANDLE;

	VkBuffer       vkIndexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory vkIndexBufferMemory = VK_NULL_HANDLE;
#endif

#if defined DX_ENGINE
	DirectX::DXtextureVertexType* vertices = NULL;

#if defined DX9 || defined DX11
	ID3D11Buffer* vertexBuffer = NULL;	//DX11
	ID3D11Buffer* indexBuffer = NULL;	//DX11
#endif
#if defined DX12
	ID3D12Resource* vertexBufferUpload = NULL;	//DX12
	ID3D12Resource* indexBufferUpload = NULL;	//DX12
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView;	//DX12
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView; 	//DX12
	ID3D12Resource* m_vertexBuffer = NULL;		//DX12
	ID3D12Resource* m_indexBuffer = NULL; 		//DX12
#endif
#endif

	uint32_t* indices = NULL;								//Common
	int vertexCount = 0, indexCount = 0/*, maxLength = 0*/;	//Common
	float red = -1, green = -1, blue = -1;					//Common

	int numLetters=0;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: VirtualTextClass
////////////////////////////////////////////////////////////////////////////////
class VirtualTextClass
{
public:
	virtual void Shutdown()=0;

	virtual bool Initialize(void* g_driver, void* pContext)=0;
	virtual bool InitializeTexture(void* Driver) = 0;
	virtual bool UpdateSentence(SentenceType*, TCHAR*, int, int, float, float, float)=0;
	virtual bool InitializeSentence(SentenceType**, int)=0;
	
	virtual void ReleaseSentence(SentenceType**)=0;
	virtual void RenderSentence(void* pContext, SentenceType*)=0;

	int textmaxLength = 200;
};

#endif
